#ifndef __SENTENCEEVENT_H__
#define __SENTENCEEVENT_H__


#include "c:\wxwidgets-2.8.10\include\wx\wx.h"

#include "c:\wxwidgets-2.8.10\include\wx\event.h"

DECLARE_EVENT_TYPE(wxEVT_SENTENCE_SEND, 7777)

//extern const wxEventType wxEVT_SENTENCE_SEND;

class SentenceEvent : public wxCommandEvent
{
public:
	SentenceEvent(wxEventType commandType = wxEVT_SENTENCE_SEND, int id=0)
		: wxCommandEvent(commandType, id) {}
	//SentenceEvent();
	SentenceEvent( const SentenceEvent &event )
		: wxCommandEvent(event) { this->SetSentence(event.GetSentence()); }

	
	// function to retireve the NMEA sentence
	void SetSentence(const wxString& sentence) {m_sentence = sentence; }
	wxString GetSentence() const {return m_sentence; }
	virtual wxEvent *Clone() const
		{ return new SentenceEvent(*this); }
 

private:
	wxString m_sentence;

//DECLARE_DYNAMIC_CLASS(SentenceEvent)

};

typedef void (wxEvtHandler::*SentenceEventFunction)(SentenceEvent&);

//typedef void (wxEvtHandler::*SentenceEventHandler)(SentenceEvent&);

#define SentenceEventHandler(func)												\
	(wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)	\
	wxStaticCastEvent(SentenceEventFunction, &func)

#define EVT_SENTENCE(id, fn)											\
	DECLARE_EVENT_TABLE_ENTRY(wxEVT_SENTENCE_SEND, id, wxID_ANY,		\
	(wxObjectEventFunction)(wxEventFunction)							\
	(wxCommandEventFunction)wxStaticCastEvent(							\
	SentenceEventFunction, &fn), (wxObject*) NULL ),



#endif // __SENTENCEEVENT_H__
