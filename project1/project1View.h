// project1View.h : interface of the Cproject1View class
//

#pragma once

#include "project1Doc.h"
#include <vector>

class CHistogramDlg; // <-- 1. ADD THIS LINE SO THE VIEW KNOWS THE DIALOG EXISTS

class Cproject1View : public CScrollView
{
protected: // create from serialization only
	Cproject1View() noexcept;
	DECLARE_DYNCREATE(Cproject1View)
	BOOL   m_bPreviewRotationActive = FALSE;
	double m_previewRotationAngle = 0.0;
	// Attributes
public:
	Cproject1Doc* GetDocument() const;
	BOOL m_bFitToWindow;
	CImage m_imgBackup;
	CHistogramDlg* m_pHistDlg; // <-- 2. ADD THIS LINE TO TRACK THE OPENED WINDOW
	std::vector<BYTE> m_pixelBackupBuffer;
	// HSV processing functions
	void ApplyLiveHSV(int hDeg, int sSat, int vVal);
	void ApplyLiveHSVPreview(int hDeg, int sSat, int vVal, CImage* pSmallImg);
	// Operations
public:

	// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void OnInitialUpdate();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	
	// Implementation
public:
	virtual ~Cproject1View();
	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void RotateImage90(CImage& img);
	void RotateImage270(CImage& img);
	void RotateArbitrary(double angleDegrees);
	void RotateImageArbitrary(CImage& img, double angleDegrees);

protected:
	//afx_msg void OnColorsHsvadjustment();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnViewFittowindow();
	afx_msg void OnDestroy();
	afx_msg void OnUpdateViewFittowindow(CCmdUI* pCmdUI);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPointInvertcolors();
	afx_msg void OnPointBrightnessContrast();
	afx_msg void ApplyLiveBrightnessContrast(int bPos, int cPos);
	afx_msg void ApplyLiveHSI(int hDeg, int sSat, int iInt);
	afx_msg void ApplyLiveYCbCr(int yOffset, int cbOffset, int crOffset);
	afx_msg void OnPointprocessHsi();
	afx_msg void OnPointprocessLuminance();
	afx_msg void OnPointHistogramEqualization();
	afx_msg void ApplyLiveGamma(double gammaVal);
	afx_msg void OnPointprocessHistogramgraph();
	afx_msg void OnPointprocessGammacorrection();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void ApplyLiveHSIPreview(int hDeg, int sSat, int iInt, CImage* pSmallImg);
	afx_msg void OnPointprocessSolarization();
	void ApplyLiveSolarization(int threshold);
	void ApplyLivePosterization(int levels);
	void ApplyLiveBitPlane(int bitPlane);
	afx_msg void OnPointprocessPosterization();
	afx_msg void OnPointprocessBitplane();
	afx_msg void OnPointprocessContraststretch();
	void ApplyLiveContrastStretch(int minVal, int maxVal);
	afx_msg void OnPointprocessColorbalance();
	void ApplyLiveColorBalance(int deltaR, int deltaG, int deltaB);
	afx_msg void OnNeighborhoodprocessingAverageblur();
	afx_msg void OnNeighborhoodprocessingGaussianblur();
	afx_msg void OnNeighborhoodprocessingLowpassfilter();
	afx_msg void OnNeighborhoodprocessingHighpassfilter();
	afx_msg void OnNeighborhoodprocessingMedianfilter();
	afx_msg void OnNeighborhoodprocessingSobeledgedetection();
	afx_msg void OnNeighborhoodprocessingKrishedgedetection();
	afx_msg void OnNeighborhoodprocessingPrewittedgedetection();
	afx_msg void OnSpatialdomainfilteringScharredgedetection();
	afx_msg void OnSpatialdomainfilteringLaplacianofgaussian();
	afx_msg void OnSpatialdomainfilteringCannyedgedetector32810();
	afx_msg void OnSpatialdomainfilteringLaplacianfilter();
	afx_msg void OnSpatialdomainfilteringFrei();
	afx_msg void OnSpatialdomainfilteringRobertscrossoperator();
	afx_msg void OnSpatialdomainfilteringDifferenceofgaussians();
	afx_msg void OnSpatialdomainfilteringMarr();
	afx_msg void OnSpatialdomainfilteringKayyalioperator();
	afx_msg void OnSpatialdomainfilteringRobinsoncompassoperator();
	afx_msg void OnSpatialdomainfilteringNevatia();
	afx_msg void OnSpatialdomainfilteringDerichefilter();
	afx_msg void OnSpatialdomainfilteringUnsharpmasking();
	afx_msg void OnSpatialdomainfilteringEmbossfilter();
	afx_msg void OnSpatialdomainfilteringGredientmagnitude();
	afx_msg void OnSpatialdomainfilteringBilateralfilter();
	afx_msg void OnSpatialdomainfilteringNon();
	afx_msg void OnSpatialdomainfilteringWienerfilter();
	afx_msg void OnSpatialdomainfilteringErosion();
	afx_msg void OnSpatialdomainfilteringDomainfilteringopening();
	afx_msg void OnSpatialdomainfilteringDomainfilteringclosing();
	afx_msg void OnSpatialdomainfilteringDomainfilteringmorphologicalgradient();
	afx_msg	void OnTransformFlip();
	afx_msg void OnImageFlipRotate();
	afx_msg void FlipHorizontal();
	afx_msg void FlipVertical();
	afx_msg void Rotate180();
	afx_msg void Rotate90();
	afx_msg void Rotate270();
	afx_msg void SetPreviewRotationAngle(double angle, BOOL bActive);
	afx_msg void DrawArbitraryRotationPreview(CDC* pDC, CImage& srcImg, double angleDegrees);
	
	afx_msg void OnColorsHsvadjustment();
	afx_msg void OnPointprocessConverttograyscale();
	afx_msg void OnPointprocessHsl();
	afx_msg void ApplyLiveHsl(int hOffset, int sOffset, int lOffset);
	afx_msg void OnPointprocessLogtransformation();
	afx_msg void ApplyLiveLog(double cVal);
	afx_msg void ApplyLiveInverseLog(double cVal);
	afx_msg void OnPointprocessInverselogtransformation();
	afx_msg void OnPointprocessGrayslicing();
	afx_msg void ApplyLiveGraySlicing(int lowVal, int highVal, BOOL bPreserveBG);
	//void ApplyLivePseudoColor(int mapIndex);
	//afx_msg void ApplyLivePseudoColor(int mapIndex);
	afx_msg void ApplyLivePseudoColor(int mapIndex, int shiftValue = 0);
	afx_msg void OnColorprocessPseudocoloring();
};

#ifndef _DEBUG  // debug version in project1View.cpp
inline Cproject1Doc* Cproject1View::GetDocument() const
{
	return reinterpret_cast<Cproject1Doc*>(m_pDocument);
}
#endif




