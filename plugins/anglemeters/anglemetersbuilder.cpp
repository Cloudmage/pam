#include "anglemetersbuilder.h"
#include <wx/panel.h>
#include "settings.h"
#include "settingevent.h"
#include "pnlAngleMeters.h"
#include "pnlMode.h"
#include "pnlOptions.h"
#include "pnlMeterSettings.h"

using namespace std;

AngleMetersBuilder::AngleMetersBuilder() : MonitorPluginBuilder(),
m_pAngleMeters(0)
{

    RegisterForSettingsUpdates(wxT("Mode"), this);
    RegisterForSettingsUpdates(wxT("Freeze"), this);
    RegisterForSettingsUpdates(wxT("Peaks"), this);
    RegisterForSettingsUpdates(wxT("Speed"), this);
    RegisterForSettingsUpdates(wxT("M3M6"), this);
    RegisterForSettingsUpdates(wxT("Stereo"), this);

    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&AngleMetersBuilder::OnSettingChanged);

}

void AngleMetersBuilder::SetAudioData(const timedbuffer* pBuffer)
{
    m_pAngleMeters->SetAudioData(pBuffer);
}

wxWindow* AngleMetersBuilder::CreateMonitorPanel(wxWindow* pParent)
{
    m_pAngleMeters = new pnlAngleMeters(pParent, this);
    return m_pAngleMeters;
}

list<pairOptionPanel_t> AngleMetersBuilder::CreateOptionPanels(wxWindow* pParent)
{
    list<pairOptionPanel_t> lstOptionPanels;

    pnlMode* pMode = new pnlMode(pParent, this);
    pnlOptions* pOptions = new pnlOptions(pParent, this);

    lstOptionPanels.push_back(make_pair(wxT("Mode"), pMode));
    lstOptionPanels.push_back(make_pair(wxT("Meters"), new pnlMeterSettings(pParent, this)));
    lstOptionPanels.push_back(make_pair(wxT("Options"), pOptions));

    return lstOptionPanels;
}




void AngleMetersBuilder::LoadSettings()
{


}


void AngleMetersBuilder::InputSession(const session& aSession)
{
    m_pAngleMeters->SetSession(aSession);
    m_pAngleMeters->SetMode(ReadSetting(wxT("Mode"),0));
}

void AngleMetersBuilder::OutputChannels(const std::vector<char>& vChannels)
{
    m_pAngleMeters->OutputChannels(vChannels);
}


void AngleMetersBuilder::OnSettingChanged(SettingEvent& event)
{
    if(event.GetKey() == wxT("Mode"))
    {
        m_pAngleMeters->SetMode(ReadSetting(wxT("Mode"),0));
    }
    else if(event.GetKey() == wxT("Freeze"))
    {
        m_pAngleMeters->Freeze((ReadSetting(wxT("Freeze"),0)==1));
    }
    else if(event.GetKey() == wxT("Peaks"))
    {
        m_pAngleMeters->ShowPeaks(ReadSetting(wxT("Peaks"),0));
    }
    else if(event.GetKey() == wxT("Speed"))
    {
        m_pAngleMeters->SetSpeed(ReadSetting(wxT("Speed"),1));
    }
    else if(event.GetKey() == wxT("M3M6"))
    {
        m_pAngleMeters->SetM3M6(ReadSetting(wxT("M3M6"),2));
    }
    else if(event.GetKey() == wxT("Stereo"))
    {
        m_pAngleMeters->UpdateMeterStereo();
    }

}


void AngleMetersBuilder::ClearMeter()
{
    m_pAngleMeters->ClearMeters();
}