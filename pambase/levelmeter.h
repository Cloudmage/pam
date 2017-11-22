#pragma once
#include <wx/window.h>
#include <vector>
#include "uirect.h"
#include <wx/bitmap.h>
#include "ppm.h"
#include "loud.h"
#include "dlldefine.h"

class PAMBASE_IMPEXPORT LevelMeter : public wxWindow
{
    DECLARE_EVENT_TABLE()
public:
    LevelMeter();
    virtual ~LevelMeter();

    LevelMeter(wxWindow *parent, wxWindowID id, const wxString & sText,double dMin, unsigned int nRouting, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);

    void InitMeter(const wxString & sText,double dMin);

    void SetLevels(const double dLevels[], size_t nSize);

    bool SetLightColours(double dLow, wxColour clrLow, double dMid, wxColour clrMid,  wxColour clrHigh);
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
    void SetNumberOfChannels(unsigned int nChannels);

    void SetPeakMode(int nMode);
    void FreezeMeter(bool bFreeze);

    void SetMeterMSMode(long nMode);
    void SetMeterSpeed(long nSpeed);

    void SetShading(bool bShading);


    enum {PEAK_HIDE, PEAK_SHOW, PEAK_HOLD};
    enum {LEFT=0, RIGHT=1, MIDDLE=8, SIDE=9, LEVELS=10};

    enum {PPM, PEAK, ENERGY, LOUD, TOTAL, AVERAGE};
protected:

    void ShowValue(double dValue, bool bdB=false);
    void ShowPeak(const float* pBuffer, unsigned int nBufferSize);
    void ShowAverage(const float* pBuffer, unsigned int nBufferSize);
    void ShowTotal(const float* pBuffer, unsigned int nBufferSize);
    void ShowVU(const float* pBuffer, unsigned int nBufferSize);
    void ShowPPM(const float* pBuffer, unsigned int nBufferSize);
    void ShowLUFS(const float* pBuffer, unsigned int nBufferSize);

    float GetPeak(const float* pBuffer, unsigned int nBufferSize);
    float GetTotal(const float* pBuffer, unsigned int nBufferSize);

    uiRect m_uiSimple;
    uiRect m_uiLevel[3];
    uiRect m_uiBlack;
    uiRect m_uiPeak;
    std::vector<uiRect>	m_vLevelText;
    std::vector<double> m_vLevels;

    uiRect m_uiLabel;
    uiRect m_uiLevelText;

    wxBitmap m_bmpMeter;
    wxColour m_clrBackground;

    double m_dMin;
    double m_dMax;
    unsigned short m_nLightGap;

    wxPen m_penText;
    wxColour m_clrText;

    bool m_bInit;

    double m_dLastValue;
    unsigned int m_nRouting;

    double m_dPeakValue;

    double m_dFall;

    double m_dPixelsPerdB;
    double m_dPixelsPerPPM;

    short m_nMeterDisplay;

    int m_nPeakMode;
    bool m_bFreeze;
    wxRect m_rectLastPeak;
    wxRect m_rectLastBlack;

    std::pair<double, wxColour> m_pairColour[3];

    ppm* m_pPPM;
    loud* m_pLoud;

    int m_nPeakCounter;

    long m_nMeterMSMode;
    long m_nMeterSpeed;

    bool m_bShading;

    unsigned int m_nChannels;
};

