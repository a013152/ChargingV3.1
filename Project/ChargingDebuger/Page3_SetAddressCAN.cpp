// Page3_SetAddressCAN.cpp : 实现文件
//

#include "stdafx.h"
#include "Page3_SetAddressCAN.h"
#include "afxdialogex.h"
//#include "Protocol.h"
//#include "Transmit.h"
//#define GET_T CTransmit::GetInstance()
//#define GET_P CProtocol::GetInstance()


// CPage3_SetAddressCAN 对话框

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


// CPage3_SetAddressCAN 消息处理程序


BOOL CPage3_SetAddressCAN::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CPage3_SetAddressCAN::OnPaint()
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
	dc.FillSolidRect(rect, RGB(202, 12, 22));
}


void CPage3_SetAddressCAN::OnBnClickedBtnOpenCan()
{
	// TODO:  在此添加控件通知处理程序代码
	//打开设备
	/*int result = GET_T->openCanDev();
	if (result == 0){
		m_pPrintfFun(L"打开CAN设备成功。\n");
		
	}
	else if (result == -1)
	{
		m_pPrintfFun(L"打开CAN设备失败！\n");
	}
	else if (result == -2){
		m_pPrintfFun(L"初始化CAN通讯库失败!\n");
	}
	else if (result != -3)
	{
		m_pPrintfFun(L"开启CAN设备通讯机制失败！\n");
	}*/

}


void CPage3_SetAddressCAN::OnBnClickedBtnCloseCan()
{
	// TODO:  在此添加控件通知处理程序代码
	/*if (GET_T->closeCanDev())
	{
		m_pPrintfFun(L"关闭CAN设备失败！\n");
		return;
	}
	m_pPrintfFun(L"关闭CAN设备成功！\n");*/
}
