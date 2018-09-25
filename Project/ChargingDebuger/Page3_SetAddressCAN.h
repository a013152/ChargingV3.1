#pragma once
#include "resource.h"

// CPage3_SetAddressCAN 对话框

class CPage3_SetAddressCAN : public CDialogEx
{
	DECLARE_DYNAMIC(CPage3_SetAddressCAN)

public:
	CPage3_SetAddressCAN(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPage3_SetAddressCAN();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };
	void setStateRect(CRect &rect);//设置group边框
	void setprintfFun(SENDCOMMAND fun){ m_pPrintfFun = fun; }//设置函数指针\

	int receiveFromCanDeviceProcess(char * szData);
	int sendToCanDeviceProcess(char * szData, int nLength);
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();


private:
	SENDCOMMAND m_pPrintfFun = NULL; //发送函数
	bool isPrepareToSendCAN();
	CWnd* m_pParent;
	DWORD m_canDeviceProcessId ;  //进程id
	HANDLE m_hPipe ;//管道句柄

	bool isPrepareCANID(CString & strID);
public:
	afx_msg void OnBnClickedBtnOpenCan();	
	afx_msg void OnBnClickedBtnRAdressCan();
	afx_msg void OnBnClickedBtnWAddressCan();
	afx_msg void OnEnChangeEditCanid();
	afx_msg void OnBnClickedBtnVerify();
	afx_msg void OnBnClickedBtnRbm();
	afx_msg void OnBnClickedBtnWbm50();
	afx_msg void OnBnClickedBtnWbm100();
};
