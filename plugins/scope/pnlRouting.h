#ifndef PNLROUTING_H
#define PNLROUTING_H

//(*Headers(pnlRouting)
#include <wx/panel.h>
#include "wmlist.h"
//*)

class ScopeBuilder;

class pnlRouting: public wxPanel
{
	public:

		pnlRouting(wxWindow* parent,ScopeBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlRouting();

		//(*Declarations(pnlRouting)
		wmList* m_plstRouting2;
		wmList* m_plstRouting1;
		//*)

		void SetNumberOfChannels(unsigned int nChannels);

	protected:

		//(*Identifiers(pnlRouting)
		static const long ID_M_PLST16;
		static const long ID_M_PLST1;
		//*)

	private:

		//(*Handlers(pnlRouting)
		void OnlstScope_RoutingSelected(wxCommandEvent& event);
		void OnlstRouting2Selected(wxCommandEvent& event);
		//*)

		void ShowRouting(wmList* pLst, unsigned int nPlot, unsigned int nChannels);
		ScopeBuilder* m_pBuilder;

		DECLARE_EVENT_TABLE()
};

#endif
