// MyFilterDlg.h : header file
//

#if !defined(AFX_MYFILTERDLG_H__429F1472_7662_4D59_A323_833F8E18E366__INCLUDED_)
#define AFX_MYFILTERDLG_H__429F1472_7662_4D59_A323_833F8E18E366__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DigitalFilter.h"

/////////////////////////////////////////////////////////////////////////////
// CMyFilterDlg dialog
#define  dataLenght  500				//输入信号的长度


class CMyFilterDlg : public CDialog
{
public:
	tagDigitalFilterParameters MyselfFilter;				//设计滤波器参数结构体
	CDigitalFilter m_DigitalFilter;							//设计滤波器的类
	double m_dOriginSignal[dataLenght];						//原始的生理信号
 	double m_dTransformedSignal[dataLenght * 2];			//滤波后的信号
	double MultinomialCoefficientDenominator[MAX_ORDER];	//滤波器分母多项式的系数
	double MultinomialCoefficientNumerator[MAX_ORDER];		//滤波器分子多项式的系数

	bool m_IsTransformed;//信号是否通过滤波器
public:
// Construction
public:
	bool DisplayFunction();
	bool DisplaySignals();
	bool Initialization();
	bool FindMax(double *data, int datalen, int &datanum);
	CMyFilterDlg(CWnd* pParent = NULL);	// standard constructor
	~CMyFilterDlg();
	
// Dialog Data
	//{{AFX_DATA(CMyFilterDlg)
	enum { IDD = IDD_MYFILTER_DIALOG };
	int		m_nFilterClass;
	int		m_nFilterType;
	int		m_nFilterMode;
	int		m_nHighPassFrequency;
	int		m_nLowPassFrequency;
	int		m_nSampleFrequency;
	int		m_nStartFrequency;
	int		m_nStopFrequency;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyFilterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMyFilterDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnShowWindow();
	afx_msg void OnReadOriginalData();
	afx_msg void OnWriteTranslatedSignalToFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYFILTERDLG_H__429F1472_7662_4D59_A323_833F8E18E366__INCLUDED_)































































