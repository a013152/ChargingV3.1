
// ChargingOption001.cpp : ����Ӧ�ó��������Ϊ��
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


// CChargingOption001App ����

CChargingOption001App::CChargingOption001App()
{
	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CChargingOption001App ����

CChargingOption001App theApp;


// CChargingOption001App ��ʼ��

BOOL CChargingOption001App::InitInstance()
{
//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO:  Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	HMODULE hModule = LoadLibrary(L"RICHED20.DLL");
	AfxInitRichEdit();
	CChargingOption001Dlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO:  �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO:  �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}
	FreeLibrary(hModule);
	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}



BOOL CChargingOption001App::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���
	/*ASSERT(pMsg);
	/// ����Ǽ������Ϣ
	if (pMsg->message == WS_APPACTIVE)
	{
		/// ����ǰ������
		if (pMsg->wParam == VK_LBUTTON)
		{
			ASSERT(m_pMainWnd);
			/// ����������
			SetActiveWindow(m_pMainWnd->m_hWnd);
		}
	}*/
	return CWinApp::PreTranslateMessage(pMsg);
}
