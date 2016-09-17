; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMyFilterDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "myfilter.h"
LastPage=0

ClassCount=3
Class1=CMyFilterApp
Class2=CAboutDlg
Class3=CMyFilterDlg

ResourceCount=3
Resource1=IDD_MYFILTER_DIALOG
Resource2=IDD_ABOUTBOX
Resource3=IDR_MENU1

[CLS:CMyFilterApp]
Type=0
BaseClass=CWinApp
HeaderFile=MyFilter.h
ImplementationFile=MyFilter.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=MyFilterDlg.cpp
ImplementationFile=MyFilterDlg.cpp
LastObject=IDM_READ

[CLS:CMyFilterDlg]
Type=0
BaseClass=CDialog
HeaderFile=MyFilterDlg.h
ImplementationFile=MyFilterDlg.cpp
Filter=W
VirtualFilter=dWC
LastObject=IDC_LOWPASS_SLOPE

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_MYFILTER_DIALOG]
Type=1
Class=CMyFilterDlg
ControlCount=37
Control1=IDC_STATIC,button,1342177287
Control2=IDC_FILTER_CLASS_FIR,button,1342308361
Control3=IDC_FILTER_CLASS_IIR,button,1342177289
Control4=IDC_STATIC,button,1342177287
Control5=IDC_STATIC,button,1342177287
Control6=IDC_STATIC,button,1342177287
Control7=IDC_STATIC,button,1342177287
Control8=IDC_STATIC,static,1342308352
Control9=IDC_SAMPLE_FREQUENCY,edit,1350631552
Control10=IDC_FILTER_TYPE,button,1342308361
Control11=IDC_RADIO4,button,1342177289
Control12=IDC_RADIO5,button,1342177289
Control13=IDC_RADIO6,button,1342177289
Control14=IDC_FILTER_MODE,button,1342308361
Control15=IDC_RADIO8,button,1342177289
Control16=IDC_CHECK1,button,1342242819
Control17=IDC_STATIC,static,1342308352
Control18=IDC_LOWPASS_FREQUENCY,edit,1350631552
Control19=IDC_LOWPASS_SLOPE,combobox,1344340226
Control20=IDC_STATIC,static,1342308352
Control21=IDC_HIGHPASS_FREQUENCY,edit,1350631552
Control22=IDC_HIGHPASS_SLOPE,combobox,1344340226
Control23=IDC_STATIC,static,1342308352
Control24=IDC_START_FREQUENCY,edit,1350631552
Control25=IDC_START_SLOPE,combobox,1344340226
Control26=IDC_STATIC,static,1342308352
Control27=IDC_STOP_FREQUENCY,edit,1350631552
Control28=IDC_STOP_SLOPE,combobox,1344340226
Control29=IDC_SHOW_WINDOW,button,1342242816
Control30=IDC_ORIGINSIGNAL,static,1342308352
Control31=IDC_FUNCTION,static,1342308352
Control32=IDC_TRANSFORMEDSIGNAL,static,1342308352
Control33=IDC_STATIC,static,1342308352
Control34=IDC_STATIC,static,1342308352
Control35=IDC_STATIC,static,1342308352
Control36=IDC_STATIC,static,1342308352
Control37=IDC_STATIC,static,1342308352

[MNU:IDR_MENU1]
Type=1
Class=?
Command1=IDM_READ
Command2=IDM_WRITE
CommandCount=2

