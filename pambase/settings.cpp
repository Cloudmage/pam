#include "settings.h"
#include <wx/log.h>
#include "settingevent.h"

using   namespace std;


Settings::Settings()
{
    m_iniManager.ReadIniFile(wxT("pam2.ini"));
}


Settings& Settings::Get()
{
    static Settings settingManager;
    return settingManager;
}


wxString Settings::Read(const wxString& sSection, const wxString& sKey, const wxString& sDefault)
{
    return m_iniManager.GetIniString(sSection, sKey, sDefault);
}

int Settings::Read(const wxString& sSection, const wxString& sKey, int nDefault)
{
    return m_iniManager.GetIniInt(sSection, sKey, nDefault);
}

double Settings::Read(const wxString& sSection, const wxString& sKey, double dDefault)
{
    return m_iniManager.GetIniDouble(sSection, sKey, dDefault);
}

bool Settings::Write(const wxString& sSection, const wxString& sKey, const wxString& sValue)
{
    m_iniManager.SetSectionValue(sSection, sKey,sValue);
    bool bDone = m_iniManager.WriteIniFile(wxT("pam2.ini"));

    if(bDone)
    {
        wxString sHandler(wxString::Format(wxT("%s/%s"),sSection.c_str(),sKey.c_str()));
        for(multimap<wxString, wxEvtHandler*>::const_iterator itHandler = m_mmHandlers.lower_bound(sHandler); itHandler != m_mmHandlers.upper_bound(sHandler); ++itHandler)
        {
            SettingEvent event(sSection, sKey);
            wxPostEvent(itHandler->second, event);
        }
    }
    return bDone;
}

bool Settings::Write(const wxString& sSection, const wxString& sKey, int nValue)
{
    return Write(sSection, sKey, wxString::Format(wxT("%d"), nValue));
}

bool Settings::Write(const wxString& sSection, const wxString& sKey, double dValue)
{
    return Write(sSection, sKey, wxString::Format(wxT("%f"), dValue));
}


bool Settings::GetSectionDataBegin(const wxString& sSection, std::map<wxString, wxString>::const_iterator& itBegin) const
{
    const iniSection* pSection = m_iniManager.GetSection(sSection);
    if(pSection)
    {
        itBegin = pSection->GetDataBegin();
        return true;
    }
    return false;
}

bool Settings::GetSectionDataEnd(const wxString& sSection, std::map<wxString, wxString>::const_iterator& itEnd) const
{
    const iniSection* pSection = m_iniManager.GetSection(sSection);
    if(pSection)
    {
        itEnd = pSection->GetDataEnd();
        return true;
    }
    return false;
}



void Settings::AddHandler(const wxString& sSection, const wxString& sKey, wxEvtHandler* pHandler)
{
    m_mmHandlers.insert(make_pair(wxString::Format(wxT("%s/%s"), sSection.c_str(), sKey.c_str()), pHandler));
}


bool Settings::RemoveKey(const wxString& sSection, const wxString& sKey)
{
    m_iniManager.RemoveSectionValue(sSection, sKey);
    return m_iniManager.WriteIniFile(wxT("pam2.ini"));
}

bool Settings::RemoveSection(const wxString& sSection)
{
    m_iniManager.RemoveSection(sSection);
    return m_iniManager.WriteIniFile(wxT("pam2.ini"));
}