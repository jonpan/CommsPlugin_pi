/******************************************************************************
 * TIMComms_pi.h
 *
 * Project:  OpenCPN
 * Purpose:  TIMComms Plugin
 * Author:   Jonathan Pannaman
 *
 ***************************************************************************
 */


#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include "TIMComms_pi.h"
#include "SentenceEvent.hpp"


// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr)
{
    return (opencpn_plugin *)new TIMComms_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
    delete p;
}




//---------------------------------------------------------------------------------------------------------
//
//    TIMCommsboard PlugIn Implementation
//
//---------------------------------------------------------------------------------------------------------



//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------

//BEGIN_EVENT_TABLE(TIMComms_pi, wxEvtHandler)
//  EVT_TIMER(TIMER_ID, TIMComms_pi::OnTimer)
//  EVT_SENTENCE(wxID_ANY, TIMComms_pi::OnSentence)
//END_EVENT_TABLE()


int TIMComms_pi::Init(void)
{
	//      printf("TIMComms_pi Init()\n");

	// Get a pointer to the opencpn display canvas, to use as a parent for windows created
	//m_parent_window = GetOCPNCanvasWindow();

	// Create the Context Menu Items

	//    Get a pointer to the opencpn configuration object
	m_pconfig = GetOCPNConfigObject();

	//    And load the configuration items
	m_DatagramSock = NULL;

	LoadConfig();
	ApplyConfig();

	// now start refresh timer		
	//m_timer.SetOwner(this, TIMER_ID);
	//m_timer.Start(10);
	OpenCommsSocket();
	Connect(PLUGIN_SOCKET_ID, wxEVT_SOCKET, wxSocketEventHandler(TIMComms_pi::OnSocketEvent), NULL, this);

	OpenSerialPort();


      return (
		   WANTS_PREFERENCES         |
           WANTS_NMEA_SENTENCES           
            );
}

bool TIMComms_pi::DeInit(void)
{
//      printf("TIMComms_pi DeInit()\n");

		//m_timer.Stop();
		if(m_DatagramSock)
		{
			//this->ProcessPendingEvents();
			Disconnect(PLUGIN_SOCKET_ID, wxEVT_SOCKET, wxSocketEventHandler(TIMComms_pi::OnSocketEvent), NULL, this);
			//m_pPpimgr->ProcessPendingEvents();
			//m_DatagramSock->Destroy();
		}

     
      return true;
}

int TIMComms_pi::GetAPIVersionMajor()
{
      return MY_API_VERSION_MAJOR;
}

int TIMComms_pi::GetAPIVersionMinor()
{
      return MY_API_VERSION_MINOR;
}

int TIMComms_pi::GetPlugInVersionMajor()
{
      return PLUGIN_VERSION_MAJOR;
}

int TIMComms_pi::GetPlugInVersionMinor()
{
      return PLUGIN_VERSION_MINOR;
}

wxString TIMComms_pi::GetCommonName()
{
      return _("TIMComms");
}

wxString TIMComms_pi::GetShortDescription()
{
      return _("TIMComms PlugIn for OpenCPN");
}

wxString TIMComms_pi::GetLongDescription()
{
      return _("TIMComms PlugIn for OpenCPN\n\
Comms plugin  for TIM System from CyberNautical Systems.");

}

bool TIMComms_pi::LoadConfig(void)
{
      wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

      if(pConf)
      {
            pConf->SetPath( _T("/PlugIns/TIMComms") );

            wxString config;

            //pConf->Read( _T("InstrumentWidth"), &m_iInstrumentWidth, 250 );
			pConf->Read(_T("SourceAddress"), &m_sSourceAddress,_T("localhost"));
			pConf->Read(_T("SourcePort"), &m_iSourcePort, 1400);
            return true;
      }
      else
            return false;
}

bool TIMComms_pi::SaveConfig(void)
{
      wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

      if(pConf)
      {
            pConf->SetPath( _T( "/PlugIns/TIMComms" ) );
            //pConf->Write( _T( "InstrumentWidth" ), m_iInstrumentWidth );
			pConf->Write(_T("SourceAddress"), m_sSourceAddress);
			pConf->Write(_T("SourcePort"), m_iSourcePort);

            return true;
      }
      else
            return false;
}

void TIMComms_pi::ApplyConfig(void)
{
}

void TIMComms_pi::ShowPreferencesDialog( wxWindow* parent )
{
	m_pPrefsDlg = new TIMCommsPrefs(parent);

		m_pPrefsDlg->m_txtSourceAddress->SetValue(m_sSourceAddress);
		wxString str;
		str << m_iSourcePort;
		m_pPrefsDlg->m_txtSourcePort->SetValue(str);

	if(m_pPrefsDlg->ShowModal() == wxID_OK)
	{
		m_sSourceAddress = m_pPrefsDlg->m_txtSourceAddress->GetValue();
		m_pPrefsDlg->m_txtSourcePort->GetValue().ToLong((long*)&m_iSourcePort, 10);
		SaveConfig();
		OpenCommsSocket();

	}
	delete m_pPrefsDlg;

}

bool TIMComms_pi::OpenCommsSocket()
{
	wxString address;
	long port;

	// address first, check for localhost
	if(m_sSourceAddress.Contains(wxT("localhost")))
	{
		address = wxT("127.0.0.1");
	}
	else
	{
		if(m_sSourceAddress.Contains(wxT(":")))
		{
			address = m_sSourceAddress.Left(m_sSourceAddress.First(wxT(":")));
		}
		else
		{
			address = m_sSourceAddress;		//assumes the field contains valid address, may need to validate
		}
	}
	if(m_sSourceAddress.Contains(wxT(":")))
	{
		wxString p = m_sSourceAddress.Mid(m_sSourceAddress.First(wxT(":")) + 1);
		p.ToLong(&port, 10);
	}
	else
	{
		port = (long) m_iSourcePort;		// CNS default port
	}

	if(!wxSocketBase::IsInitialized())
	{
		bool ok = wxSocketBase::Initialize();
	}

	// Create the socket
	wxIPV4address addr;
	addr.Hostname(address);
	addr.Service(port);

	if(m_DatagramSock != NULL)
	{
		m_DatagramSock->Close();
		delete m_DatagramSock;
	}


	m_DatagramSock = new wxDatagramSocket(addr, wxSOCKET_NONE);

	// Setup the event handler and subscribe to most events

	m_DatagramSock->SetEventHandler(*this, PLUGIN_SOCKET_ID);

	m_DatagramSock->SetNotify(wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
	m_DatagramSock->Notify(TRUE);
	m_DatagramSock->SetTimeout(100000);              // Short timeout

	bool m = m_DatagramSock->IsInitialized();
	if(m == false)
		return false;
	else
		return true;



}
void TIMComms_pi::OnSocketEvent(wxSocketEvent& event)
{
	#define RD_BUF_SIZE    200

    unsigned char *bp;
    unsigned char buf[RD_BUF_SIZE + 1];
    int char_count;
    wxString token;
    //double dglat, dglon, dgcog, dgsog;
    //double dtime;
    wxDateTime fix_time;
    wxString str_buf;

  switch(event.GetSocketEvent())
  {
  case wxSOCKET_INPUT :                     // from gpsd Daemon
        //m_sock->SetFlags(wxSOCKET_WAITALL | wxSOCKET_BLOCK);      // was (wxSOCKET_NOWAIT);
                                                                  // We use wxSOCKET_BLOCK to avoid Yield() reentrancy problems
                                                                  // if a long ProgressDialog is active, as in S57 SENC creation.

		//}
        // Disable input event notifications to preclude re-entrancy on non-blocking socket

		// Read the reply, one character at a time, looking for 0x0a (lf)
		// If the reply contains no lf, break on the buffer full

        bp = buf;
        char_count = 0;
		m_DatagramSock->Read(buf, 100);
		if(m_DatagramSock->Error())
			return;

		str_buf = wxString::FromAscii((const char *)buf);
		// CNS units do not end in a 0x0a need to find the end of their strings

		if(str_buf.find(0x0a) != wxNOT_FOUND)		//line feed
		{
			str_buf = str_buf.Left(str_buf.find(0x0a) + 1);
		}
		else if(str_buf.find(0x0d) != wxNOT_FOUND)	//carriage return only
		{
			str_buf = str_buf.Left(str_buf.find(0x0d) + 1);
		}
		else if(str_buf.find(0x2a) != wxNOT_FOUND)	//asterisk
		{
			str_buf = str_buf.Left(str_buf.find(0x2a) + 3);
			str_buf += wxString(_T("\r\n"));
		}
	      
		this->SendSentence(str_buf);
  }

}

void TIMComms_pi::SendSentence(wxString sentence)
{
	SentenceEvent event(wxEVT_SENTENCE_SEND, wxID_ANY);
	event.SetSentence(sentence);
	event.SetEventObject(this);
	bool prop = event.ShouldPropagate();
	wxPostEvent(m_pPpimgr, event);
}
bool TIMComms_pi::OpenSerialPort()
{
	m_pSerialPort = new wxSerialPort();
	m_pSerialPort->SetBaudRate(wxBAUD_4800);
	//m_pSerialPort->
	int resp = m_pSerialPort->Open(wxCOM4);
	return m_pSerialPort->IsOpen();
}