// Page3_SetAddressCAN.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Page3_SetAddressCAN.h"
#include "afxdialogex.h" 
#include "CommonFunction.h"
#include "common.h"
extern char g_AppPath[256] = { 0 };
char g_szSendBuff[256] = { 0 };
char g_szReceBuff[MAX_BUF_SIZE] = { 0 };
#define MAX_SIZE 1024

// CPage3_SetAddressCAN �Ի���

IMPLEMENT_DYNAMIC(CPage3_SetAddressCAN, CDialogEx)

CPage3_SetAddressCAN::CPage3_SetAddressCAN(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPage3_SetAddressCAN::IDD, pParent)
{
	m_canDeviceProcessId = 0;
	m_hPipe = 0;
	m_pParent = pParent;
}

CPage3_SetAddressCAN::~CPage3_SetAddressCAN()
{
}

void CPage3_SetAddressCAN::setStateRect(CRect &rect)
{
	CWnd* pState = (CWnd*)GetDlgItem(IDC_STATIC);
	if (pState)
	{
		pState->MoveWindow(&rect);
	}
}

void CPage3_SetAddressCAN::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPage3_SetAddressCAN, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_OPEN_CAN, &CPage3_SetAddressCAN::OnBnClickedBtnOpenCan)	 
	ON_BN_CLICKED(IDC_BTN_R_ADRESS_CAN, &CPage3_SetAddressCAN::OnBnClickedBtnRAdressCan)
	ON_BN_CLICKED(IDC_BTN_W_ADDRESS_CAN, &CPage3_SetAddressCAN::OnBnClickedBtnWAddressCan)
	ON_EN_CHANGE(IDC_EDIT_CANID, &CPage3_SetAddressCAN::OnEnChangeEditCanid)
	ON_BN_CLICKED(IDC_BTN_Verify, &CPage3_SetAddressCAN::OnBnClickedBtnVerify)
	ON_BN_CLICKED(IDC_BTN_RBM, &CPage3_SetAddressCAN::OnBnClickedBtnRbm)
	ON_BN_CLICKED(IDC_BTN_WBM50, &CPage3_SetAddressCAN::OnBnClickedBtnWbm50)
	ON_BN_CLICKED(IDC_BTN_WBM100, &CPage3_SetAddressCAN::OnBnClickedBtnWbm100)
END_MESSAGE_MAP()


// CPage3_SetAddressCAN ��Ϣ�������


BOOL CPage3_SetAddressCAN::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CPage3_SetAddressCAN::OnPaint()
{ 
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()

	CRect rect;  //����
	GetClientRect(&rect);
	rect.bottom -= 5;
	rect.left += 5;
	rect.top += 5;
	rect.right -= 5;
	dc.FillSolidRect(rect, RGB(202, 12, 22));
}


bool CPage3_SetAddressCAN::isPrepareToSendCAN()
{
	if (m_canDeviceProcessId == 0)
	{
		m_pPrintfFun(CString(L"����CAN����δ�򿪣�����ͨѶ!\n"));
		return false;
	}
	if (m_hPipe == 0)
	{
		m_pPrintfFun(CString(L"����CAN��������ͨ��δ�򿪣�����ͨѶ!\n"));
		return false;
	}
	return true;
}
 

bool CPage3_SetAddressCAN::isPrepareCANID(CString & strCanId)
{
	GetDlgItem(IDC_EDIT_CANID)->GetWindowText(strCanId);
	if (strCanId.IsEmpty())
	{
		m_pPrintfFun(L"����CAN ID ����Ϊ�ա�\n");
		return false;
	}
	return true;
}

void CPage3_SetAddressCAN::OnBnClickedBtnOpenCan()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strOpenOrClose;
	GetDlgItem(IDC_BTN_OPEN_CAN)->GetWindowText(strOpenOrClose);
	if (strOpenOrClose.Compare(L"��CAN�豸") == 0)
	{
		CString strPrintf;
		//1 ��ѯ�������Ƿ���ڽ���
		strcpy_s(g_AppPath, 256, COM_F::WStringToMBytes(COM_F::getAppDir().c_str()).c_str());
		std::wstring strPath = COM_F::getAppDir(); strPath += L"set.ini";
		wchar_t szMainCharingProcessName[256] = { 0 };
		DWORD result = GetPrivateProfileStringW(L"SET",  L"windowTitle", L"",szMainCharingProcessName, 256,  strPath.c_str()  );
		if (wcslen(szMainCharingProcessName) != 0)
		{
			DWORD pid = COM_F::GetProcessidFromName(szMainCharingProcessName);
			if (pid){
				strPrintf.Format(L"���̡�%s�� �Ѿ����ڣ����ȹرա�\n", szMainCharingProcessName);
				m_pPrintfFun(strPrintf);
				return;
			}
		}

		int showMode = GetPrivateProfileIntW(L"SET", L"canDeviceProcessShowMode", 0, strPath.c_str()); // ��ʾģʽ 0 ���أ�1 ��ʾ

		//2 ����canͨѶ����id�� ���������ǿ�ƹرգ� ������canͨѶ���̣�	
		m_canDeviceProcessId = COM_F::GetProcessidFromName(COM_F::MBytesToWString(CANDEVICETRANSMITION).c_str());
		if (m_canDeviceProcessId)
			COM_F::closeProcessFromId(m_canDeviceProcessId);

		//����
		char szServerPath[256] = { 0 };
		sprintf_s(szServerPath, 256, "%s/%s", (g_AppPath), CANDEVICETRANSMITION);
		COM_F::startProcessFromPath(COM_F::MBytesToWString(szServerPath).c_str(), showMode);
		::Sleep(500);
		m_canDeviceProcessId = COM_F::GetProcessidFromName(COM_F::MBytesToWString(CANDEVICETRANSMITION).c_str());

		if (m_canDeviceProcessId <= 0)
		{
			m_pPrintfFun(CString("����CANͨѶ����ʧ��!"));
			return;
		}
		//3 ���������ܵ��� ʹ���ص�io ��ʽ��ȡ
		m_hPipe = COM_F::connectServerNamePipe(TEXT(PIPE_NAME));
		if (INVALID_HANDLE_VALUE == m_hPipe)
		{
			m_pPrintfFun(CString("��������ͨ��ʧ��!"));
			return;
		}

		//���豸
		sprintf_s(g_szSendBuff, 256, "%s,F1", C2S);
		sendToCanDeviceProcess(g_szSendBuff, strlen(g_szSendBuff));
		::Sleep(100);
		char szReceive[MAX_SIZE] = { 0 };
		receiveFromCanDeviceProcess(szReceive);

		//
		GetDlgItem(IDC_BTN_OPEN_CAN)->SetWindowText(L"�ر�CAN�豸");
	}
	else if (strOpenOrClose.Compare(L"�ر�CAN�豸") == 0)
	{
		//�жϽ���id
		if (m_canDeviceProcessId > 0)
		{
			//�ж�����ͨ�����
			if (m_hPipe > 0)
			{
				//���ͽ����Ƴ�����
				sprintf_s(g_szSendBuff, 256, "%s,%s", C2S, "FF");
				sendToCanDeviceProcess(g_szSendBuff, strlen(g_szSendBuff));
				receiveFromCanDeviceProcess(g_szReceBuff);				
				CloseHandle(m_hPipe);
				//m_canDeviceProcessId = 0;
				m_hPipe = 0;				
			}
			else if (COM_F::closeProcessFromId(m_canDeviceProcessId))
			{
				m_canDeviceProcessId = 0;
				m_hPipe = 0;
			}
			while (m_canDeviceProcessId)
			{
				Sleep(100);
				m_canDeviceProcessId = COM_F::GetProcessidFromName(COM_F::MBytesToWString(CANDEVICETRANSMITION).c_str());
			}
			m_pPrintfFun(CString(L"CANͨѶ����\n\n"));
		}
		GetDlgItem(IDC_BTN_OPEN_CAN)->SetWindowText(L"��CAN�豸");
	}	
	
	//���ø����ڽ���
	m_pParent->SetFocus();

}

 

//����
int CPage3_SetAddressCAN::sendToCanDeviceProcess(char * szData, int nLength)
{
	if (m_hPipe > 0)
	{
		DWORD wlen = 0;
		WriteFile(m_hPipe, szData, nLength, &wlen, 0);//��CAN����������
		CString str = L"���ͣ�"; str += szData; str += L"\n";
		m_pPrintfFun(str);
		Sleep(10);
		return wlen;
	}
	return 0;

}

//����
int CPage3_SetAddressCAN::receiveFromCanDeviceProcess(char * szData)
{
	if (m_hPipe > 0)
	{
		DWORD rlen = 0;
		ReadFile(m_hPipe, szData, MAX_SIZE, &rlen, NULL); //����CAN���̷��͹���������
		CString str = L"���գ�"; str += CString(szData);
		m_pPrintfFun(str);
	}
	return 0;
}


void CPage3_SetAddressCAN::OnBnClickedBtnRAdressCan()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (!isPrepareToSendCAN())
		return;
	sprintf_s(g_szSendBuff, 256, "%s,%s,R", C2S, "F3");
	sendToCanDeviceProcess(g_szSendBuff, strlen(g_szSendBuff));
	Sleep(200);
	receiveFromCanDeviceProcess(g_szReceBuff);
	m_pParent->SetFocus();
}


void CPage3_SetAddressCAN::OnBnClickedBtnWAddressCan()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (!isPrepareToSendCAN())
		return;

	CString strCanId; 
	GetDlgItem(IDC_EDIT_CANID)->GetWindowText(strCanId);
	if (strCanId.IsEmpty())
	{
		m_pPrintfFun(L"����CAN ID ����Ϊ�ա�\n"); 
		return;
	}
	sprintf_s(g_szSendBuff, 256, "%s,%s,W,%s", C2S, "F3", COM_F::WStringToMBytes(strCanId).c_str());
	sendToCanDeviceProcess(g_szSendBuff, strlen(g_szSendBuff));
	Sleep(200);
	receiveFromCanDeviceProcess(g_szReceBuff);

	m_pParent->SetFocus();
}


void CPage3_SetAddressCAN::OnEnChangeEditCanid()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	//m_pParent->SetFocus();
}


void CPage3_SetAddressCAN::OnBnClickedBtnVerify()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	
	if (!isPrepareToSendCAN())
		return;
	CString strCanId;
	if (!isPrepareCANID(strCanId))
		return;	 
	sprintf_s(g_szSendBuff, 256, "%s,%s,%s", C2S, "F4", COM_F::WStringToMBytes(strCanId).c_str());
	int len =strlen(g_szSendBuff);
	sendToCanDeviceProcess(g_szSendBuff, len);
	Sleep(200);
	receiveFromCanDeviceProcess(g_szReceBuff);

	m_pParent->SetFocus();
}


void CPage3_SetAddressCAN::OnBnClickedBtnRbm()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	if (!isPrepareToSendCAN())
		return;
	CString strCanId;
	if (!isPrepareCANID(strCanId))
		return;
	sprintf_s(g_szSendBuff, 256, "%s,%s,%s,R", C2S, "F5", COM_F::WStringToMBytes(strCanId).c_str());
	sendToCanDeviceProcess(g_szSendBuff, strlen(g_szSendBuff));
	receiveFromCanDeviceProcess(g_szReceBuff);
	m_pParent->SetFocus();
}


void CPage3_SetAddressCAN::OnBnClickedBtnWbm50()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	if (!isPrepareToSendCAN())
		return;
	CString strCanId;
	if (!isPrepareCANID(strCanId))
		return;
	sprintf_s(g_szSendBuff, 256, "%s,%s,%s,W,0x01", C2S, "F5", COM_F::WStringToMBytes(strCanId).c_str());
	sendToCanDeviceProcess(g_szSendBuff, strlen(g_szSendBuff));
	receiveFromCanDeviceProcess(g_szReceBuff);
	m_pParent->SetFocus();
}


void CPage3_SetAddressCAN::OnBnClickedBtnWbm100()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (!isPrepareToSendCAN())
		return;
	CString strCanId;
	if (!isPrepareCANID(strCanId))
		return;
	sprintf_s(g_szSendBuff, 256, "%s,%s,%s,W,0xff", C2S, "F5", COM_F::WStringToMBytes(strCanId).c_str());
	sendToCanDeviceProcess(g_szSendBuff, strlen(g_szSendBuff));
	receiveFromCanDeviceProcess(g_szReceBuff);
	m_pParent->SetFocus();
}
