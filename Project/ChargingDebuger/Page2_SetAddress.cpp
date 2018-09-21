// Page2_SetAddress.cpp : 实现文件
//

#include "stdafx.h"
#include "ChargingOption001.h"
#include "Page2_SetAddress.h"
#include "afxdialogex.h"


// CPage2_SetAddress 对话框

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
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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


// CPage2_SetAddress 消息处理程序


void CPage2_SetAddress::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	 
	CRect rect;  //矩形
	GetClientRect(&rect);
	rect.bottom -= 5;
	rect.left += 5;
	rect.top += 5;
	rect.right -= 5;
	dc.FillSolidRect(rect, RGB(226, 25, 24));
}



//读地址
void CPage2_SetAddress::OnBnClickedBtnRAddress()
{
	// TODO:  在此添加控件通知处理程序代码

	CButton *pCheck = (CButton*)GetDlgItem(IDC_RADIO1); 
	if (pCheck)
	{
		if (pCheck->GetCheck())  //  充电器 读地址
		{
			static CString strCom = L"*NF,X,0,";
			m_pSendFun(strCom);
		}
	}
	pCheck = (CButton*)GetDlgItem(IDC_RADIO2);
	if (pCheck)
	{
		if (pCheck->GetCheck())  //  继电器 读地址
		{
			static CString strCom = L"*NF,r,";
			m_pSendFun(strCom);
		}
	}

}

//写地址
void CPage2_SetAddress::OnBnClickedBtnWAddress()
{
	// TODO:  在此添加控件通知处理程序代码

	CButton *pCheck = (CButton*)GetDlgItem(IDC_RADIO1);
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
	if (pCheck && pEdit)
	{
		if (pCheck->GetCheck())  //  充电器 写地址
		{
			static CString strCom; 
			pEdit->GetWindowTextW(strCom);
			if (strCom.IsEmpty())
				AfxMessageBox(L"地址不能为空！");
			else{
				strCom = L"*NF,W,0," + strCom + L",";
				m_pSendFun(strCom);
			}
			
		}
	}
	pCheck = (CButton*)GetDlgItem(IDC_RADIO2);
	if (pCheck && pEdit)
	{
		if (pCheck->GetCheck())  //  继电器 写地址
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
	// TODO:  在此添加控件通知处理程序代码
	if (m_pParent)
		m_pParent->SetFocus();
}


void CPage2_SetAddress::OnBnClickedRadio2()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_pParent)
		m_pParent->SetFocus();
}


void CPage2_SetAddress::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
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
