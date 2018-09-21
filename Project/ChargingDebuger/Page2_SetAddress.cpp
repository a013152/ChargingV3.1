// Page2_SetAddress.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChargingOption001.h"
#include "Page2_SetAddress.h"
#include "afxdialogex.h"


// CPage2_SetAddress �Ի���

IMPLEMENT_DYNAMIC(CPage2_SetAddress, CDialogEx)

CPage2_SetAddress::CPage2_SetAddress(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPage2_SetAddress::IDD, pParent)
{
	m_pParent = pParent;
}

CPage2_SetAddress::~CPage2_SetAddress()
{
}

void CPage2_SetAddress::setStateRect(CRect &rect)
{
	CWnd* pState = (CWnd*)GetDlgItem(IDC_STATIC);
	if (pState)
	{
		pState->MoveWindow(&rect);
	}
}

 

BOOL CPage2_SetAddress::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CButton *pCheck = (CButton*)GetDlgItem(IDC_RADIO1);
	if (pCheck)
		pCheck->SetCheck(TRUE);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CPage2_SetAddress::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPage2_SetAddress, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_R_ADDRESS, &CPage2_SetAddress::OnBnClickedBtnRAddress)
	ON_BN_CLICKED(IDC_BTN_W_ADDRESS, &CPage2_SetAddress::OnBnClickedBtnWAddress)
	ON_BN_CLICKED(IDC_RADIO1, &CPage2_SetAddress::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CPage2_SetAddress::OnBnClickedRadio2)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CPage2_SetAddress ��Ϣ�������


void CPage2_SetAddress::OnPaint()
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
	dc.FillSolidRect(rect, RGB(226, 25, 24));
}



//����ַ
void CPage2_SetAddress::OnBnClickedBtnRAddress()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	CButton *pCheck = (CButton*)GetDlgItem(IDC_RADIO1); 
	if (pCheck)
	{
		if (pCheck->GetCheck())  //  ����� ����ַ
		{
			static CString strCom = L"*NF,X,0,";
			m_pSendFun(strCom);
		}
	}
	pCheck = (CButton*)GetDlgItem(IDC_RADIO2);
	if (pCheck)
	{
		if (pCheck->GetCheck())  //  �̵��� ����ַ
		{
			static CString strCom = L"*NF,r,";
			m_pSendFun(strCom);
		}
	}

}

//д��ַ
void CPage2_SetAddress::OnBnClickedBtnWAddress()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	CButton *pCheck = (CButton*)GetDlgItem(IDC_RADIO1);
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
	if (pCheck && pEdit)
	{
		if (pCheck->GetCheck())  //  ����� д��ַ
		{
			static CString strCom; 
			pEdit->GetWindowTextW(strCom);
			if (strCom.IsEmpty())
				AfxMessageBox(L"��ַ����Ϊ�գ�");
			else{
				strCom = L"*NF,W,0," + strCom + L",";
				m_pSendFun(strCom);
			}
			
		}
	}
	pCheck = (CButton*)GetDlgItem(IDC_RADIO2);
	if (pCheck && pEdit)
	{
		if (pCheck->GetCheck())  //  �̵��� д��ַ
		{
			static CString strCom;
			pEdit->GetWindowTextW(strCom);
			strCom = L"*NF,w," + strCom + L",";
			m_pSendFun(strCom);
		}
	}
}


 


void CPage2_SetAddress::OnBnClickedRadio1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_pParent)
		m_pParent->SetFocus();
}


void CPage2_SetAddress::OnBnClickedRadio2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_pParent)
		m_pParent->SetFocus();
}


void CPage2_SetAddress::OnLButtonUp(UINT nFlags, CPoint point)
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
