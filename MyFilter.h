// MyFilter.h : main header file for the MYFILTER application
//

#if !defined(AFX_MYFILTER_H__50D8EE25_5A2A_48D3_9AD1_A0158149901F__INCLUDED_)
#define AFX_MYFILTER_H__50D8EE25_5A2A_48D3_9AD1_A0158149901F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMyFilterApp:
// See MyFilter.cpp for the implementation of this class
//

class CMyFilterApp : public CWinApp
{
public:
	CMyFilterApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyFilterApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMyFilterApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYFILTER_H__50D8EE25_5A2A_48D3_9AD1_A0158149901F__INCLUDED_)
