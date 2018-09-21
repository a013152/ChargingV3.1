#pragma once


// CPage2_SetAddress �Ի���

class CPage2_SetAddress : public CDialogEx
{
	DECLARE_DYNAMIC(CPage2_SetAddress)

public:
	CPage2_SetAddress(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPage2_SetAddress();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

	void setStateRect(CRect &rect);//����group�߿�
	void setSendFun(SENDCOMMAND fun){ m_pSendFun = fun; }//���÷��ͺ���ָ��
protected: 
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	 
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	
private:
	SENDCOMMAND m_pSendFun = NULL; //���ͺ���
	CWnd* m_pParent;
public:
	afx_msg void OnBnClickedBtnRAddress();
	afx_msg void OnBnClickedBtnWAddress(); 
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
