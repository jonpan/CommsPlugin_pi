///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __TIMCommsPreferences__
#define __TIMCommsPreferences__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class TIMCommsPreferences
///////////////////////////////////////////////////////////////////////////////
class TIMCommsPreferences : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText3;
		wxStaticText* m_staticText4;
		wxStdDialogButtonSizer* m_sdbSizer2;
		wxButton* m_sdbSizer2OK;
		wxButton* m_sdbSizer2Cancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnCancel( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnOK( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		wxTextCtrl* m_txtSourceAddress;
		wxTextCtrl* m_txtSourcePort;
		TIMCommsPreferences( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("TIM Comms Plugin Preferences"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 275,150 ), long style = wxCAPTION );
		~TIMCommsPreferences();
	
};

#endif //__TIMCommsPreferences__
