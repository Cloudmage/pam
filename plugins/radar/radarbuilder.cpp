#include "radarbuilder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "radarmeter.h"
#include "levelcalculator.h"
#include "pnlrouting.h"
#include "pnlDisplay.h"
#include "pnlMeters.h"
#include <wx/log.h>
#include "version.h"
#include "ppmtypes.h"

using namespace std;

RadarBuilder::RadarBuilder() : MonitorPluginBuilder(),
m_pRadar(0)
{

    RegisterForSettingsUpdates(wxT("Routing"), this);
    RegisterForSettingsUpdates(wxT("Timeframe"), this);
    RegisterForSettingsUpdates(wxT("RefreshRate"), this);
    RegisterForSettingsUpdates(wxT("MeterMode"), this);

    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&RadarBuilder::OnSettingChanged);

    m_pCalculator = new LevelCalculator(0);
    SetMeterMode();

    m_nInputChannels = 1;
    m_nDisplayChannel = 0;
}

RadarBuilder::~RadarBuilder()
{
    delete m_pCalculator;
}

void RadarBuilder::SetAudioData(const timedbuffer* pBuffer)
{
    if(m_nInputChannels != 0)
    {
        m_pCalculator->CalculateLevel(pBuffer);

        double dLevel;
        switch(m_nDisplayChannel)
        {
            case 9:
                dLevel = m_pCalculator->GetMSLevel(false);
                break;
            case 10:
                dLevel = m_pCalculator->GetMSLevel(true);
                break;
            default:
               dLevel = m_pCalculator->GetLevel(m_nDisplayChannel);
        }

//            case LevelCalculator::PPM:
//                dLevel *= 4.0;
//                dLevel -= 34.0;
//                m_pRadar->SetRadarLevel(dLevel, pBuffer->GetBufferSize()/m_nInputChannels, true);
                m_pRadar->SetRadarLevel(dLevel, pBuffer->GetBufferSize()/m_nInputChannels, false);
    }
}

wxWindow* RadarBuilder::CreateMonitorPanel(wxWindow* pParent)
{
    m_pRadar = new RadarMeter(pParent);
    return m_pRadar;
}

list<pairOptionPanel_t> RadarBuilder::CreateOptionPanels(wxWindow* pParent)
{
    list<pairOptionPanel_t> lstOptionPanels;

    m_ppnlRouting = new pnlRouting(pParent, this);

    lstOptionPanels.push_back(make_pair(wxT("Routing"), m_ppnlRouting));
    lstOptionPanels.push_back(make_pair(wxT("Time"), new pnlDisplay(pParent, this)));
    lstOptionPanels.push_back(make_pair(wxT("Meter"), new pnlMeters(pParent, this)));
//    lstOptionPanels.push_back(make_pair(wxT("Options"), pOptions));
//
    return lstOptionPanels;
}




void RadarBuilder::LoadSettings()
{
    if(m_pRadar)
    {
        m_nDisplayChannel = ReadSetting(wxT("Routing"),0);
        m_pRadar->SetChannel(m_nDisplayChannel);
        m_pRadar->SetTimespan(ReadSetting(wxT("Timeframe"),60));
        m_pRadar->SetRefreshRate(ReadSetting(wxT("RefreshRate"),250));
        SetMeterMode();
        ClearMeter();
    }

}


void RadarBuilder::InputSession(const session& aSession)
{
    m_pCalculator->InputSession(aSession);
    if(aSession.GetCurrentSubsession() != aSession.lstSubsession.end())
    {
        m_pRadar->SetSampleRate(min((unsigned int)256 ,aSession.GetCurrentSubsession()->nSampleRate));
        m_nInputChannels = min((unsigned int)256 ,aSession.GetCurrentSubsession()->nChannels);
        m_ppnlRouting->SetNumberOfChannels(m_nInputChannels);
    }
    else
    {
        m_pRadar->SetSampleRate(48000);
        m_nInputChannels = 0;
        m_ppnlRouting->SetNumberOfChannels(0);
    }
    ClearMeter();
}

void RadarBuilder::OutputChannels(const std::vector<char>& vChannels)
{
}


void RadarBuilder::OnSettingChanged(SettingEvent& event)
{
    if(event.GetKey() == wxT("Routing"))
    {
        m_nDisplayChannel = ReadSetting(wxT("Routing"),0);
        m_pRadar->SetChannel(m_nDisplayChannel);
        ClearMeter();
    }
    else if(event.GetKey() == wxT("Timeframe"))
    {
        m_pRadar->SetTimespan(ReadSetting(wxT("Timeframe"),60));
    }
    else if(event.GetKey() == wxT("RefreshRate"))
    {
        m_pRadar->SetRefreshRate(ReadSetting(wxT("RefreshRate"),250));
    }
    else if(event.GetKey() == wxT("MeterMode"))
    {
        SetMeterMode();
    }
}

void RadarBuilder::SetMeterMode()
{
    m_sMode = ReadSetting(wxT("MeterMode"), wxT("BBC"));
    map<wxString, ppmtype>::const_iterator itType = PPMTypeManager::Get().FindType(m_sMode);
    if(itType != PPMTypeManager::Get().GetTypeEnd())
    {
        m_pCalculator->SetMode(itType->second.nType);
        m_pCalculator->SetDynamicResponse(itType->second.dRiseTime, itType->second.dRisedB, itType->second.dFallTime, itType->second.dFalldB);
        if(m_pRadar)
        {
            m_pRadar->SetScale(itType->second.vLevels, itType->first);
        }
    }
}

void RadarBuilder::ClearMeter()
{
    m_pRadar->ClearMeter();
}



