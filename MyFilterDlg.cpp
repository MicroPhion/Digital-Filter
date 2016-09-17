// MyFilterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyFilter.h"
#include "MyFilterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyFilterDlg dialog

CMyFilterDlg::CMyFilterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyFilterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyFilterDlg)
	m_nFilterClass = 0;
	m_nFilterType = 0;
	m_nFilterMode = 0;
	m_nHighPassFrequency = 100;
	m_nLowPassFrequency = 200;
	m_nSampleFrequency = 2000;
	m_nStartFrequency = 100;
	m_nStopFrequency = 600;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	memset(m_dTransformedSignal, 0, dataLenght * 2 * sizeof(double));
	memset(MultinomialCoefficientDenominator, 0, MAX_ORDER * sizeof(double));
	memset(MultinomialCoefficientNumerator, 0, MAX_ORDER * sizeof(double));

	m_IsTransformed = 0;
	//initialization signal
	for (int i = 0 ; i < dataLenght; i ++)
		m_dOriginSignal[i] = sin (10 * i * 3.14 / 180);// + sin (30 * i * 3.14 / 180);
	
}
CMyFilterDlg::~CMyFilterDlg()
{
}

void CMyFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyFilterDlg)
	DDX_Radio(pDX, IDC_FILTER_CLASS_FIR, m_nFilterClass);
	DDX_Radio(pDX, IDC_FILTER_TYPE, m_nFilterType);
	DDX_Radio(pDX, IDC_FILTER_MODE, m_nFilterMode);
	DDX_Text(pDX, IDC_HIGHPASS_FREQUENCY, m_nHighPassFrequency);
	DDX_Text(pDX, IDC_LOWPASS_FREQUENCY, m_nLowPassFrequency);
	DDX_Text(pDX, IDC_SAMPLE_FREQUENCY, m_nSampleFrequency);
	DDX_Text(pDX, IDC_START_FREQUENCY, m_nStartFrequency);
	DDX_Text(pDX, IDC_STOP_FREQUENCY, m_nStopFrequency);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyFilterDlg, CDialog)
	//{{AFX_MSG_MAP(CMyFilterDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SHOW_WINDOW, OnShowWindow)
	ON_COMMAND(IDM_READ, OnReadOriginalData)
	ON_COMMAND(IDM_WRITE, OnWriteTranslatedSignalToFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyFilterDlg message handlers

BOOL CMyFilterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CMenu menu;						//可为局部变量
	menu.LoadMenu(IDR_MENU1);		//加载菜单资源使与CMenu  Object相关联
	SetMenu(&menu);					//  Sets the current menu to the specified menu( 这是CWnd的函数)
	menu.Detach();					//这一步很关键，因为menu为局部变量，使用此函数 Detaches a Windows menu 
						// return TRUE  unless you set the focus to a control

	
	CComboBox *mComb = (CComboBox*)GetDlgItem(IDC_HIGHPASS_SLOPE);
	mComb->SetCurSel(3);
	int nCount = mComb->GetCount();
	
	mComb = (CComboBox*)GetDlgItem(IDC_LOWPASS_SLOPE);
	mComb->SetCurSel(3);
	
	mComb = (CComboBox*)GetDlgItem(IDC_START_SLOPE);
	mComb->SetCurSel(3);
	
	mComb = (CComboBox*)GetDlgItem(IDC_STOP_SLOPE);
	mComb->SetCurSel(3);
	return TRUE;
//	UpdateWindow();
}

void CMyFilterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CMyFilterDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);


	}
	else
	{
		CDialog::OnPaint();
		DisplaySignals();

		if (m_IsTransformed)
		{
			DisplayFunction();
		}

	}

}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMyFilterDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMyFilterDlg::OnShowWindow() 
{
	// TODO: Add your control notification handler code here

	
	if(!Initialization())return;

	m_DigitalFilter.DesignFilter(	MyselfFilter,
									MultinomialCoefficientDenominator,
									MultinomialCoefficientNumerator);


	m_DigitalFilter.Convolution(	MyselfFilter,
									MultinomialCoefficientDenominator,
									MultinomialCoefficientNumerator,
									dataLenght,
									m_dTransformedSignal,
									m_dOriginSignal);
	m_IsTransformed = 1;

	Invalidate(1);


	
}

bool CMyFilterDlg::Initialization()
{
	//取衰减为60dB时的频率为截止频率fs,即使用海明窗
	UpdateData();
	CString str;

	InputParameter Input;
	
	//class/mode/type/Fs
	Input.Filter_Class = m_nFilterClass + 1;
	Input.Filter_Mode = m_nFilterMode + 1;
	Input.Filter_Type = m_nFilterType + 1;
	Input.SampleFrequency = m_nSampleFrequency;

	

	//fh/fl
	GetDlgItem(IDC_HIGHPASS_FREQUENCY)->GetWindowText(str);
	Input.HighPass_Frequency_CutOff = atoi(str);
	GetDlgItem(IDC_LOWPASS_FREQUENCY)->GetWindowText(str);
	Input.LowPass_Frequency_CutOff = atoi(str);
	GetDlgItem(IDC_START_FREQUENCY)->GetWindowText(str);
	Input.Start_Frequency = atoi(str);
	GetDlgItem(IDC_STOP_FREQUENCY)->GetWindowText(str);
	Input.Stop_Frequency = atoi(str);


	//fSlope
	CComboBox *mComb = (CComboBox*)GetDlgItem(IDC_HIGHPASS_SLOPE);
	Input.HighPass_Frequency_CutOff_Slope = (int)pow(2,mComb->GetCurSel()) * 6;
	
	mComb = (CComboBox*)GetDlgItem(IDC_LOWPASS_SLOPE);
	Input.LowPass_Frequency_CutOff_Slope = (int)pow(2,mComb->GetCurSel()) * 6;

	mComb = (CComboBox*)GetDlgItem(IDC_START_SLOPE);
	Input.Start_Frequency_Slope = (int)pow(2,mComb->GetCurSel()) * 6;

	mComb = (CComboBox*)GetDlgItem(IDC_STOP_SLOPE);
	Input.Stop_Frequency_Slope = (int)pow(2,mComb->GetCurSel()) * 6;


	//initialization
	if(0 == m_DigitalFilter.Initialization(	MyselfFilter, Input))return false;

	return true;
}

bool CMyFilterDlg::DisplaySignals()
{
	long i;
	int MaxNumOriginSignal;
	int MaxNumTransformedSignal;
	FindMax(m_dOriginSignal,dataLenght,MaxNumOriginSignal);
	FindMax(m_dTransformedSignal,dataLenght,MaxNumTransformedSignal);

	if(m_dOriginSignal[MaxNumOriginSignal] == 0)m_dOriginSignal[MaxNumOriginSignal] = 1;
	if(m_dTransformedSignal[MaxNumTransformedSignal] == 0)m_dTransformedSignal[MaxNumTransformedSignal] = 1;

/*
	double  MaxSignal = fabs(m_dOriginSignal[MaxNumOriginSignal]) >				\
						fabs(m_dTransformedSignal[MaxNumTransformedSignal]) ?   \
						fabs(m_dOriginSignal[MaxNumOriginSignal]) :				\
						fabs(m_dTransformedSignal[MaxNumTransformedSignal]);
*/

	double MaxSignal = fabs(m_dOriginSignal[MaxNumOriginSignal]);
	//OriginSignal
 	CWnd * pOriginSignalWnd;
	CRect OriginSignalRect;
 	pOriginSignalWnd = GetDlgItem(IDC_ORIGINSIGNAL);
	pOriginSignalWnd->GetWindowRect(&OriginSignalRect);


	CSize cs;
	cs.cx = OriginSignalRect.right - OriginSignalRect.left;
	cs.cy = OriginSignalRect.bottom - OriginSignalRect.top;
	CPaintDC pDC(pOriginSignalWnd);
	pDC.TextOut (500, 0,"OriginSignal");
/*
	for(i = 0; i < cs.cx ; i += 50)
	{
		pDC.MoveTo (i, 0);
		pDC.LineTo (i, cs.cy);
	}	
	for(i = 0; i < cs.cy; i += cs.cy / 4)
	{
		pDC.MoveTo (0,i);
		pDC.LineTo (cs.cx ,i);
	}
*/

	pDC.MoveTo(0,0);
	TRY
	{
		for (i = 0 ; i < dataLenght ; i ++)
		{
			pDC.LineTo(i,(int)((1 - m_dOriginSignal[i] / MaxSignal) 
							* cs.cy / 2));
		}

	}
	CATCH(CMemoryException, pEx)
	{

		MessageBox("The Origin signal Max is 0!");
		AfxAbort();
	}
	END_CATCH



	//TransformedSingal

	CWnd * pTransformedSingalWnd;
	CRect TransformedSignalRect;
	pTransformedSingalWnd = GetDlgItem(IDC_TRANSFORMEDSIGNAL);
	pTransformedSingalWnd->GetWindowRect(&TransformedSignalRect);

	cs.cx = TransformedSignalRect.right - TransformedSignalRect.left;
	cs.cy = TransformedSignalRect.bottom - TransformedSignalRect.top;

	
	CPen standardPen( PS_SOLID, 1, RGB(255,0,0) );
	CPaintDC pDC1(pTransformedSingalWnd);
	CPen *OldPen = pDC1.SelectObject(&standardPen);
	pDC1.TextOut (500, 0,"TranslatedSignal");
	pDC.MoveTo(0,0);
	TRY
	{
		for ( i = 0 ; i < dataLenght ; i ++)
		{
			pDC1.LineTo(i,(int)((1 - m_dTransformedSignal[i] / MaxSignal) * cs.cy /2));
		}
		
	}
	CATCH(CMemoryException, pEx)
	{
		MessageBox("the MAX of the Origin signal which has passed through the filter is 0! ");
		AfxAbort();
	}
	END_CATCH


	pDC.SelectObject(OldPen);
	return true;
}

bool CMyFilterDlg::FindMax(double *data, int datalen, int &datanum)
{
		double tempdata=data[0];
		datanum = 0;
		for(int i = 0; i < datalen; i ++)
		{
			if(tempdata < data[i])
			{
				datanum = i;
				tempdata = data[i];
			}
			if(tempdata < -data[i])
			{
				datanum = i; 
				tempdata = - data[i];
			}
		}
	return true;
}


bool CMyFilterDlg::DisplayFunction()
{

		//OriginSignal
		CWnd * pFunctionWnd;
		pFunctionWnd = GetDlgItem(IDC_FUNCTION);
		
		m_DigitalFilter.DisplayFunction(pFunctionWnd, 
										MyselfFilter,
										MultinomialCoefficientDenominator,
										MultinomialCoefficientNumerator);
		return true;
}

void CMyFilterDlg::OnReadOriginalData() 
{
	// TODO: Add your command handler code here
	CFileDialog dlg(true);
	if(dlg.DoModal()==IDOK)
	{
		CString lpszPathName=dlg.GetPathName ();
		CWnd* pWnd = GetDlgItem(IDC_ORIGINSIGNAL);
		pWnd->SetWindowText(lpszPathName);
		
		//read File Header
		CFile file; 
		if(!file.Open(lpszPathName, CFile::modeRead))
		{
			AfxMessageBox("打开文件失败！");
			return;
		}
		if(dataLenght > file.GetLength())
		{
			AfxMessageBox("输入数据过少，请重新选择输入文件");
			return;
		}
		
		file.Read(m_dOriginSignal, dataLenght * sizeof(double));
		
		file.Close();
		Invalidate(1);
		return ;
	}
}

void CMyFilterDlg::OnWriteTranslatedSignalToFile() 
{
	// TODO: Add your command handler code here
	if(!m_IsTransformed)
	{
		AfxMessageBox("还没有对数据进行滤波");
		return;
	}
	CFileDialog dlg(FALSE);
	if(IDOK == dlg.DoModal())
	{
		CString lpszPathName=dlg.GetPathName ();
		CFile file; 
		file.Open(lpszPathName, CFile::modeCreate | CFile::modeWrite);
		file.Write(m_dTransformedSignal, dataLenght * sizeof(double));
		file.Close();
	}
}





















