#include "pnlLissajou.h"
#include "session.h"
#include "lissajoubuilder.h"

//(*InternalHeaders(pnlLissajou)
#include <wx/intl.h>
#include <wx/string.h>
//*)

using namespace std;

//(*IdInit(pnlLissajou)
const long pnlLissajou::ID_CUSTOM7 = wxNewId();
const long pnlLissajou::ID_CUSTOM1 = wxNewId();
const long pnlLissajou::ID_CUSTOM6 = wxNewId();
const long pnlLissajou::ID_CUSTOM8 = wxNewId();
const long pnlLissajou::ID_PANEL19 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlLissajou,wxPanel)
	//(*EventTable(pnlLissajou)
	//*)
END_EVENT_TABLE()

LissajouBuilder* pnlLissajou::m_pBuilder = 0;

pnlLissajou::pnlLissajou(wxWindow* parent,LissajouBuilder* pBuilder,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(pnlLissajou)
	Create(parent, wxID_ANY, wxDefaultPosition, wxSize(800,480), wxTAB_TRAVERSAL, _T("wxID_ANY"));
	m_pJellyfish = new LissajouMeter(this,ID_CUSTOM7,wxPoint(0,0),wxSize(600,480));
	pnlLissajou_Meters = new wxPanel(this, ID_PANEL19, wxPoint(600,0), wxSize(200,481), wxTAB_TRAVERSAL, _T("ID_PANEL19"));
	pnlLissajou_Meters->SetBackgroundColour(wxColour(0,0,0));
	m_pMeterLevels = new LevelMeter(pnlLissajou_Meters,ID_CUSTOM1,wxT("Left"), -70, LevelMeter::LEVELS, wxPoint(0,0),wxSize(50,481));
	m_pMeterLeft = new LevelMeter(pnlLissajou_Meters,ID_CUSTOM6,wxT("Left"), -70, LevelMeter::LEFT, wxPoint(50,0),wxSize(50,481));
	m_pMeterRight = new LevelMeter(pnlLissajou_Meters,ID_CUSTOM8,wxT("Right"), -70, LevelMeter::RIGHT, wxPoint(105,0),wxSize(50,481));

	Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&pnlLissajou::OnLeftUp);
	Connect(wxEVT_SIZE,(wxObjectEventFunction)&pnlLissajou::OnResize);
	//*)
	SetSize(size);


	m_pBuilder = pBuilder;

//	m_pJellyfish->Connect(wxEVT_COMMAND_LEFT_CLICK,(wxObjectEventFunction)&pnlLissajou::OnLeftUp);

	double dLevels[15] = {0,-3, -6, -9, -12, -15, -18, -21, -24, -30, -36, -42, -48, -54, -60};
	m_pMeterLeft->SetLevels(dLevels,15);
    m_pMeterRight->SetLevels(dLevels,15);
    m_pMeterLevels->SetLevels(dLevels,15);
}

pnlLissajou::~pnlLissajou()
{
	//(*Destroy(pnlLissajou)
	//*)
}


void pnlLissajou::SetAudioData(const timedbuffer* pBuffer)
{
    m_pJellyfish->SetLissajouData(pBuffer->GetBuffer(), pBuffer->GetBufferSize());
    m_pMeterLeft->ShowMeter(pBuffer->GetBuffer(), pBuffer->GetBufferSize());
    m_pMeterRight->ShowMeter(pBuffer->GetBuffer(), pBuffer->GetBufferSize());
}


void pnlLissajou::SetSession(const session& aSession)
{
    m_pJellyfish->SetNumberOfChannels(aSession.nChannels);

    m_nChannels = aSession.nChannels;

    m_pMeterLeft->SetNumberOfChannels(aSession.nChannels);
    m_pMeterRight->SetNumberOfChannels(aSession.nChannels);
}


void pnlLissajou::SetScale(unsigned int nScale)
{
    m_pJellyfish->SetScaling(nScale);
}

void pnlLissajou::Rotate(bool bRotate)
{
    m_pJellyfish->RotateLissajou(bRotate);
}

void pnlLissajou::ShowLevels(bool bShow)
{
    m_pJellyfish->SetShowLevels(bShow);
}

void pnlLissajou::OnLeftUp(wxMouseEvent& event)
{
    m_pBuilder->Maximize((GetSize().x <=600));
}


void pnlLissajou::SetAxis(unsigned int nChannelX, unsigned int nChannelY)
{
    m_pJellyfish->SetAxis(nChannelX, nChannelY);

    if(m_nChannels == 2)
    {
        switch(nChannelX)
        {
        case 0:
            m_pMeterLeft->SetLabel(wxT("Left"));

            break;
        case 1:
            m_pMeterLeft->SetLabel(wxT("Right"));

            break;
        }

        switch(nChannelY)
        {
        case 0:
            m_pMeterRight->SetLabel(wxT("Left"));

            break;
        case 1:
            m_pMeterRight->SetLabel(wxT("Right"));

            break;
        }
    }
    else
    {
        m_pMeterLeft->SetLabel(wxString::Format(wxT("Ch %d"), nChannelX));
        m_pMeterRight->SetLabel(wxString::Format(wxT("Ch %d"), nChannelY));


    }
    m_pMeterLeft->SetRouting(nChannelX);
    m_pMeterRight->SetRouting(nChannelY);

    CheckAxis();
}


void pnlLissajou::OnResize(wxSizeEvent& event)
{
    wxLogDebug(wxT("Size = %d"), GetSize().x);
    if(event.GetSize().x == 600)
    {
        pnlLissajou_Meters->Hide();
    }
    else
    {
        pnlLissajou_Meters->Show();
    }
}


void pnlLissajou::SetMeterMode(unsigned int nMode)
{
    m_pMeterLeft->SetMeterDisplay(nMode);
    m_pMeterRight->SetMeterDisplay(nMode);
    m_pMeterLevels->SetMeterDisplay(nMode);
}


void pnlLissajou::OutputChannels(const std::vector<char>& vChannels)
{
    m_vOutputs = vChannels;
    CheckAxis();
}

void pnlLissajou::CheckAxis()
{

    //if the output channels are the same as our axis we colour the meters red and green...
    if(m_vOutputs.size() > 1)
    {
        if(m_vOutputs[0] == m_pBuilder->ReadSetting(wxT("Axis_X"),0))
        {
            m_pMeterLeft->SetLightColours(-38,wxColour(220,0,0), -8,wxColour(230,230,0),  wxColour(255,100,100));
        }
        else
        {
            m_pMeterLeft->SetLightColours(-38,wxColour(255,255,255), -8,wxColour(230,230,0), wxColour(255,100,100));
        }

        if(m_vOutputs[1] == m_pBuilder->ReadSetting(wxT("Axis_Y"),0))
        {
            m_pMeterRight->SetLightColours(-38,wxColour(0,220,0), -8, wxColour(230,230,0), wxColour(255,100,100));
        }
        else
        {
            m_pMeterRight->SetLightColours(-38,wxColour(255,255,255), -8,wxColour(230,230,0), wxColour(255,100,100));
        }
    }
}

void pnlLissajou::Follow()
{
    if(m_vOutputs.size() > 1)
    {
        m_pBuilder->WriteSetting(wxT("Axis_X"), m_vOutputs[0]);
        m_pBuilder->WriteSetting(wxT("Axis_Y"), m_vOutputs[1]);
    }
}