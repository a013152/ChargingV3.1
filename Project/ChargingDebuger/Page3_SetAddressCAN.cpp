// Page3_SetAddressCAN.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Page3_SetAddressCAN.h"
#include "afxdialogex.h"
//#include "Protocol.h"
//#include "Transmit.h"
//#define GET_T CTransmit::GetInstance()
//#define GET_P CProtocol::GetInstance()


// CPage3_SetAddressCAN �Ի���

IMPLEMENT_DYNAMIC(CPage3_SetAddressCAN, CDialogEx)

CPage3_SetAddressCAN::CPage3_SetAddressCAN(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPage3_SetAddressCAN::IDD, pParent)
{

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
	ON_BN_CLICKED(IDC_BTN_CLOSE_CAN, &CPage3_SetAddressCAN::OnBnClickedBtnCloseCan)
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


void CPage3_SetAddressCAN::OnBnClickedBtnOpenCan()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//���豸
	/*int result = GET_T->openCanDev();
	if (result == 0){
		m_pPrintfFun(L"��CAN�豸�ɹ���\n");
		
	}
	else if (result == -1)
	{
		m_pPrintfFun(L"��CAN�豸ʧ�ܣ�\n");
	}
	else if (result == -2){
		m_pPrintfFun(L"��ʼ��CANͨѶ��ʧ��!\n");
	}
	else if (result != -3)
	{
		m_pPrintfFun(L"����CAN�豸ͨѶ����ʧ�ܣ�\n");
	}*/

}


void CPage3_SetAddressCAN::OnBnClickedBtnCloseCan()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	/*if (GET_T->closeCanDev())
	{
		m_pPrintfFun(L"�ر�CAN�豸ʧ�ܣ�\n");
		return;
	}
	m_pPrintfFun(L"�ر�CAN�豸�ɹ���\n");*/
}
