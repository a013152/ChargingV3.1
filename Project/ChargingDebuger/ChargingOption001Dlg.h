
// ChargingOption001Dlg.h : ͷ�ļ�
//

#pragma once

#include "SerialPort.h" 
#include "afxcmn.h"
#include "Page1_General.h"
#include "Page2_SetAddress.h"
#include "Page3_SetAddressCAN.h"

// CChargingOption001Dlg �Ի���
class CChargingOption001Dlg : public CDialogEx
{
// ����
public:
	CChargingOption001Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CHARGINGOPTION001_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public: 
	afx_msg LRESULT OnReadyToRead(WPARAM, LPARAM);
	afx_msg LRESULT OnShowInfo(WPARAM, LPARAM);

	static void SendFunc(CString);
	static void PrintfFunc(CString);
	void SendFunc_(CString); 
public:
	void GetCom();
	void initBaudComboBox();
	void printf_textEdit(CString strCon, int nInfoType =0);
	CString toHEX(CString strSource);

	bool IsDealMessage(){ return m_bDealMessageToReadData; } //�Ƿ���Ҫ�㲥

	void SetDealMessage(bool flag) { m_bDealMessageToReadData = flag; }

	void OnOK(){}
private: 
	CSerialPort m_SerialPort;
	CPage1_General	*	m_ppage1;
	CPage2_SetAddress* m_ppage2;
	CPage3_SetAddressCAN* m_ppage3;

	CSerialPort m_SerialPort2;

	bool m_bOpenCom = false;
	bool m_bOpenCom2 = false;
	bool m_bDealMessageToReadData = true;  //��־���Ƿ�����Ϣ-��ȡ��������
public:
	afx_msg void OnBnClickedBtnOpenCom();
	CRichEditCtrl richeditctrl;
	afx_msg void OnBnClickedBtnClear();
	CTabCtrl m_tab;
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	CString TransCommand(CString strCommand);

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnOpenCom2();
};
