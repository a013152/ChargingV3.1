// Page1_General.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChargingOption001.h"
#include "Page1_General.h"
#include "afxdialogex.h"


// CPage1_General �Ի���

IMPLEMENT_DYNAMIC(CPage1_General, CDialogEx)

CPage1_General::CPage1_General(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPage1_General::IDD, pParent)
{
	m_pParent = pParent;
}

CPage1_General::~CPage1_General()
{
}

BOOL CPage1_General::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND);
	if (pEdit)
	{
		pEdit->SetWindowTextW(L"*NF,X,0,");
		pEdit->SetFocus();
	}
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND2);
	if (pEdit)
	{
		pEdit->SetWindowTextW(L"*NF,O,101,6S,4.20,3.00,");
		pEdit->SetFocus();
	}
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND3);
	if (pEdit)
	{
		pEdit->SetWindowTextW(L"*NF,O,101,4S,4.20,3.00,");
		pEdit->SetFocus();
	}
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND4);
	if (pEdit)
	{
		pEdit->SetWindowTextW(L"*NF,G,101,");
		pEdit->SetFocus();
	}
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND5);
	if (pEdit)
	{
		pEdit->SetWindowTextW(L"*NF,D,101,");
		pEdit->SetFocus();
	}
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND6);
	if (pEdit)
	{
		pEdit->SetWindowTextW(L"*NF,P,101,");
		pEdit->SetFocus();
	}
	return FALSE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CPage1_General::setStateRect(CRect &rect){
	CWnd* pState = (CWnd*)GetDlgItem(IDC_STATIC);
	if (pState)
	{
		pState->MoveWindow(&rect);
	}
}

void CPage1_General::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPage1_General, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_SEND, &CPage1_General::OnBnClickedBtnSend)
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BTN_SEND2, &CPage1_General::OnBnClickedBtnSend2)
	ON_BN_CLICKED(IDC_BTN_SEND3, &CPage1_General::OnBnClickedBtnSend3)
	ON_BN_CLICKED(IDC_BTN_SEND4, &CPage1_General::OnBnClickedBtnSend4)
	ON_BN_CLICKED(IDC_BTN_SEND5, &CPage1_General::OnBnClickedBtnSend5)
	ON_BN_CLICKED(IDC_BTN_SEND6, &CPage1_General::OnBnClickedBtnSend6)
END_MESSAGE_MAP()


// CPage1_General ��Ϣ�������


void CPage1_General::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()

	CRect rect;  //����
	GetClientRect(&rect);
	dc.FillSolidRect(rect, RGB( 240,240, 240));
}


void CPage1_General::OnBnClickedBtnSend()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	static CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND);
	static CString strCom;
	if (pEdit)
	{
		pEdit->GetWindowTextW(strCom);
		m_pSendFun(strCom);
	}
}


void CPage1_General::OnSetFocus(CWnd* pOldWnd)
{
	CDialogEx::OnSetFocus(pOldWnd);

	// TODO:  �ڴ˴������Ϣ����������
	m_pParent->SetFocus();
}


void CPage1_General::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_pParent){
		CRect rect; m_pParent->GetWindowRect(&rect);
		CPoint point2(point);
		m_pParent->ClientToScreen(&point2);
		if (rect.PtInRect(point2))
		{
			m_pParent->SetFocus();
		}
	}
	

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CPage1_General::OnBnClickedBtnSend2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	static CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND2);
	static CString strCom;
	if (pEdit)
	{
		pEdit->GetWindowTextW(strCom);
		m_pSendFun(strCom);
	}
}


void CPage1_General::OnBnClickedBtnSend3()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	static CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND3);
	static CString strCom;
	if (pEdit)
	{
		pEdit->GetWindowTextW(strCom);
		m_pSendFun(strCom);
	}
}


void CPage1_General::OnBnClickedBtnSend4()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	static CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND4);
	static CString strCom;
	if (pEdit)
	{
		pEdit->GetWindowTextW(strCom);
		m_pSendFun(strCom);
	}
}


void CPage1_General::OnBnClickedBtnSend5()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	static CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND5);
	static CString strCom;
	if (pEdit)
	{
		pEdit->GetWindowTextW(strCom);
		m_pSendFun(strCom);
	}
}


void CPage1_General::OnBnClickedBtnSend6()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	static CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND6);
	static CString strCom;
	if (pEdit)
	{
		pEdit->GetWindowTextW(strCom);
		m_pSendFun(strCom);
	}
}
