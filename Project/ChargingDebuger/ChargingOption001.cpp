
// ChargingOption001.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "ChargingOption001.h"
#include "ChargingOption001Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChargingOption001App

BEGIN_MESSAGE_MAP(CChargingOption001App, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CChargingOption001App 构造

CChargingOption001App::CChargingOption001App()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CChargingOption001App 对象

CChargingOption001App theApp;


// CChargingOption001App 初始化

BOOL CChargingOption001App::InitInstance()
{
//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO:  应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	HMODULE hModule = LoadLibrary(L"RICHED20.DLL");
	AfxInitRichEdit();
	CChargingOption001Dlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO:  在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO:  在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}
	FreeLibrary(hModule);
	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}



BOOL CChargingOption001App::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	/*ASSERT(pMsg);
	/// 如果是激活窗口消息
	if (pMsg->message == WS_APPACTIVE)
	{
		/// 如果是按下左键
		if (pMsg->wParam == VK_LBUTTON)
		{
			ASSERT(m_pMainWnd);
			/// 激活主窗口
			SetActiveWindow(m_pMainWnd->m_hWnd);
		}
	}*/
	return CWinApp::PreTranslateMessage(pMsg);
}
