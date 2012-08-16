/******************************************************************************
 * $Id: TIMComms_pi.h
 *
 * Project:  OpenCPN
 * Purpose:  TIMComms Plugin
 * Author:   Jonathan Pannaman
 *
 ***************************************************************************
 */

#ifndef _TIMCOMMSPI_H_
#define _TIMCOMMSPI_H_

#include "wx/wxprec.h"
#include "wx/socket.h"
#include "wx/ctb/serport.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#define     PLUGIN_VERSION_MAJOR    1
#define     PLUGIN_VERSION_MINOR    0

#define     MY_API_VERSION_MAJOR    1
#define     MY_API_VERSION_MINOR    5

#include "../../../include/ocpn_plugin.h"
#include <wx/fileconf.h>
#include "../forms/TIMCommsPrefs.h"



#include "nmea0183/nmea0183.h"

#define TIMER_ID	1
#define PLUGIN_SOCKET_ID	55

class SentenceEvent;

//----------------------------------------------------------------------------------------------------------
//    The PlugIn Class Definition
//----------------------------------------------------------------------------------------------------------


class TIMComms_pi : public wxEvtHandler, opencpn_plugin
{
public:
      TIMComms_pi(void *ppimgr):opencpn_plugin(ppimgr){m_pPpimgr = (wxEvtHandler*)ppimgr;};

	// The required PlugIn Methods
	int Init(void);
	bool DeInit(void);
	void ShowPreferencesDialog( wxWindow* parent );


	int GetAPIVersionMajor();
	int GetAPIVersionMinor();
	int GetPlugInVersionMajor();
	int GetPlugInVersionMinor();

	wxString GetCommonName();
	wxString GetShortDescription();
	wxString GetLongDescription();
      


private:
	wxWindow			*m_parent_window;
	wxEvtHandler		*m_pPpimgr;
	TIMCommsPrefs* m_pPrefsDlg;

	bool LoadConfig(void);
	bool SaveConfig(void);
	void ApplyConfig(void);

	wxString m_sSourceAddress;
	int m_iSourcePort;

	void SendSentence(wxString sentence);

	//void OnTimer(wxTimerEvent& event);
	void OnSocketEvent(wxSocketEvent& event);
	bool OpenCommsSocket();
	bool OpenSerialPort();

	wxIPV4address		m_addr;
	//wxSocketClient    *m_sock;
	wxDatagramSocket*	m_DatagramSock;
	wxSerialPort*		m_pSerialPort;

	wxFileConfig*		m_pconfig;

	//wxTimer			m_timer;

};



#endif