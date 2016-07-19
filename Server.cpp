/////////////////////////////////////////////////////////////////////
//	Server.cpp : Defines the class behaviors for the application.
//  Copyright (C) 2000,  Jin-Soo Jeon
//
//	If this code works, it was written by Jin-Soo Jeon.
//	If not, I don't know who wrote it.
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerApp

BEGIN_MESSAGE_MAP(CServerApp, CWinApp)
	//{{AFX_MSG_MAP(CServerApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerApp construction

CServerApp::CServerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

CServerApp::~CServerApp()
{
	VERIFY(SOCKET_ERROR != WSACleanup());
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CServerApp object

CServerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CServerApp initialization
LONG WINAPI MyUnhandledExceptionFilter(struct _EXCEPTION_POINTERS *pExceptionPointers) ;
//出错回调seh
LONG WINAPI ExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo)
{
	static bool showex=false;
	if(!showex)
		showex=true;
	else
		return EXCEPTION_EXECUTE_HANDLER;
	TCHAR modulename[MAX_PATH],szError[1000],filename[MAX_PATH],*p;
	MEMORY_BASIC_INFORMATION mbinfo;
	VirtualQuery(pExceptionInfo->ExceptionRecord->ExceptionAddress,&mbinfo,sizeof(mbinfo));
	GetModuleFileName((HMODULE)mbinfo.AllocationBase,filename,MAX_PATH);
	p=_tcsrchr(filename,'\\');
	p++;
	lstrcpy(modulename,p);
	try
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		TCHAR FileName[MAX_PATH];
		::CreateDirectory(_T("Error"),NULL);
		wsprintf(FileName,_T("Error\\errorlog-%d-%d.txt"),st.wYear,st.wMonth);
		CFile syslog;
		syslog.Open(FileName,CFile::modeNoTruncate| CFile::modeCreate |CFile::modeReadWrite);
		syslog.SeekToEnd();

		TCHAR szDate[200];
		wsprintf(szDate,_T("%d-%d-%d %d:%d:%d\r\n"),st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
		syslog.Write(szDate,_tcslen(szDate));
		wsprintf(szError,_T("异常: 模块:%s, 模块基址:0x%x, 异常代码:0x%x, 指令地址:0x%x"),
			modulename,mbinfo.AllocationBase,pExceptionInfo->ExceptionRecord->ExceptionCode,pExceptionInfo->ExceptionRecord->ExceptionAddress);
		//USES_CONVERSION;
		//syslog.Write(T2A(szError),strlen(T2A(szError)));
		syslog.Write(szError,_tcslen(szError));
		char nextLine[] = ("\r\n");
		syslog.Write(nextLine,strlen(nextLine));
		syslog.Close();
	}
	catch(CException *e)
	{
		e->Delete();
	}
	wsprintf(szError,_T("程序遇到错误需关闭,请检查错误模块,运行调试解决.\n模块:%s,模块基址:0x%x,异常代码:0x%x,指令地址:0x%x"),
		modulename,mbinfo.AllocationBase,pExceptionInfo->ExceptionRecord->ExceptionCode,pExceptionInfo->ExceptionRecord->ExceptionAddress);
	//MessageBox(NULL,szError,_T("系统"),MB_ICONERROR|MB_OK);
	//MyUnhandledExceptionFilter(pExceptionInfo);
	return EXCEPTION_EXECUTE_HANDLER;
}

BOOL CServerApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	// Load Winsock 2.2
	if(!LoadWinsock()) return FALSE;

#ifdef _AFXDLL
//	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	//Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// 促惫绢 滚傈 父甸扁
	HINSTANCE hInstanceLanguagePack = LoadLibrary("DRRCKorean.dll");

	if (NULL != hInstanceLanguagePack)	
		AfxSetResourceHandle(hInstanceLanguagePack);
	//

	CServerDlg dlg;
	m_pMainWnd = &dlg;

	dlg.m_InstanceChecker.ActivateChecker();
	if (dlg.m_InstanceChecker.PreviousInstanceRunning())
	{
		//AfxMessageBox(_T("Previous version detected, will now restore it"), MB_OK);
		dlg.m_InstanceChecker.ActivatePreviousInstance();
		m_pMainWnd = NULL;
		return FALSE;
	}

	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	AfxSetResourceHandle(AfxGetInstanceHandle());

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////
//	Load Winsock DLL ver 2.2
//
BOOL CServerApp::LoadWinsock()
{
	CString strError;

	// request minimum Winsock 2.2
	WORD wVersionRequested = MAKEWORD( 2, 2 );

	WSADATA wsaData;
	int err = WSAStartup( wVersionRequested, &wsaData );

	if( err != 0 )
	{
		strError.Format(_T("WSAStartup Error, Error# = %d"), ::GetLastError());
		AfxMessageBox(strError);
		return FALSE;
	}

	if( LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 )
	{
		
		strError.Format( _T("Unsupported WinSock version %d.%d" ),
						 LOBYTE( wsaData.wVersion ),
						 HIBYTE( wsaData.wVersion ) );

		AfxMessageBox(strError);
		return FALSE;
	}

	return TRUE;
}
