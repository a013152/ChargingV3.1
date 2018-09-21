#pragma once


// CPage2_SetAddress 对话框

class CPage2_SetAddress : public CDialogEx
{
	DECLARE_DYNAMIC(CPage2_SetAddress)

public:
	CPage2_SetAddress(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPage2_SetAddress();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

	void setStateRect(CRect &rect);//设置group边框
	void setSendFun(SENDCOMMAND fun){ m_pSendFun = fun; }//设置发送函数指针
protected: 
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	 
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	
private:
	SENDCOMMAND m_pSendFun = NULL; //发送函数
	CWnd* m_pParent;
public:
	afx_msg void OnBnClickedBtnRAddress();
	afx_msg void OnBnClickedBtnWAddress(); 
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
