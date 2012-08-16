#include "TIMCommsPrefs.h"

TIMCommsPrefs::TIMCommsPrefs( wxWindow* parent )
:
TIMCommsPreferences( parent )
{

}

void TIMCommsPrefs::OnCancel( wxCommandEvent& event )
{
	// TODO: Implement OnCancel
	this->Close();

}

void TIMCommsPrefs::OnOK( wxCommandEvent& event )
{
	// TODO: Implement OnOK
	this->AcceptAndClose();
}
