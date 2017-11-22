#pragma once
#include <wx/window.h>
#include <vector>
#include "uirect.h"
#include <wx/bitmap.h>
#include "dlldefine.h"

#include "ppm.h"
#include "loud.h"

class PAMBASE_IMPEXPORT AngleMeter : public wxWindow
{
    DECLARE_EVENT_TABLE()
public:
    AngleMeter();
    virtual ~AngleMeter();

    AngleMeter(wxWindow *parent, wxWindowID id, const wxString & sText,double dMin, unsigned int nRouting, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);

    void InitMeter(const wxString & sText,double dMin);

    void SetLevels(const double dLevels[], size_t nSize);

    bool SetMeterColour(const wxColour& clrMeter1, const wxColour& clrMeter2);
    void ResetMeter(void);
    void OnPaint(wxPaintEvent& event);

    void SetLabel(const wxString& sLabel);

    void ShowMeter(const float* pBuffer, unsigned int nFrameCount);
    void SetRouting(short nRouting);
    void SetMeterDisplay(short nDisplay);
    void OnSize(wxSizeEvent& event);
    wxSize DoGetBestSize()
    {
        return wxSize(50,100);
    }

    void SetPeakMode(int nMode);
    void FreezeMeter(bool bFreeze);

    void SetMeterMSMode(long nMode);
    void SetMeterSpeed(long nSpeed);

    enum {PEAK_HIDE, PEAK_SHOW, PEAK_HOLD};
    enum {LEFT_RIGHT, MONO_STEREO};

    enum {PPM, PEAK, ENERGY, LOUD, TOTAL, AVERAGE};
protected:

    void ShowValue(double dValue[2], bool bdB=false);
    void ShowPeak(const float* pBuffer, unsigned int nFrameCount);
    void ShowEnergy(const float* pBuffer, unsigned int nFrameCount);
    void ShowPPM(const float* pBuffer, unsigned int nFrameCount);
    void ShowLUFS(const float* pBuffer, unsigned int nFrameCount);

    std::pair<float,float> GetPeak(const float* pBuffer, unsigned int nFrameCount);
    float GetTotal(const float* pBuffer, unsigned int nFrameCount);

    void WorkoutAngles(int i);

    std::vector<double> m_vLevels;

    uiRect m_uiLabel;
    uiRect m_uiLevelText[2];
    uiRect m_uiPeakText[2];

    wxBitmap m_bmpMeter;
    wxColour m_clrBackground;

    double m_dMin;
    double m_dMax;
    unsigned short m_nLightGap;

    wxPen m_penText;
    wxColour m_clrText;

    bool m_bInit;

    double m_dLastValue[2];
    unsigned int m_nRouting;

    double m_dPeakValue[2];

    double m_dFall;


    short m_nMeterDisplay;

    int m_nPeakMode;
    bool m_bFreeze;
    wxRect m_rectLastPeak;
    wxRect m_rectLastBlack;

    wxColour m_clrMeter[2];
    double m_dAngle[2];
    double m_dAngleMax[2];

    wxRect m_rectGrid;
    uiRect m_uiType;

    ppm* m_pPPM;
    loud* m_pLoud;

    int m_nPeakCounter[2];

    long m_nMeterMSMode;
    long m_nMeterSpeed;
};

