#pragma once
 

// CPage1_General 对话框

class CPage1_General : public CDialogEx
{
	DECLARE_DYNAMIC(CPage1_General)

public:
	CPage1_General(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPage1_General();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };
	void setStateRect(CRect &rect);
	void setSendFun(SENDCOMMAND fun){ m_pSendFun = fun; }
protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnSend();

private:
	SENDCOMMAND m_pSendFun = NULL;
	CWnd* m_pParent;
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	void OnOK(){}

	afx_msg void OnBnClickedBtnSend2();
	afx_msg void OnBnClickedBtnSend3();
	afx_msg void OnBnClickedBtnSend4();
	afx_msg void OnBnClickedBtnSend5();
	afx_msg void OnBnClickedBtnSend6();
};
