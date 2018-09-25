#pragma once
#include "resource.h"

// CPage3_SetAddressCAN �Ի���

class CPage3_SetAddressCAN : public CDialogEx
{
	DECLARE_DYNAMIC(CPage3_SetAddressCAN)

public:
	CPage3_SetAddressCAN(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPage3_SetAddressCAN();

// �Ի�������
	enum { IDD = IDD_DIALOG3 };
	void setStateRect(CRect &rect);//����group�߿�
	void setprintfFun(SENDCOMMAND fun){ m_pPrintfFun = fun; }//���ú���ָ��\

	int receiveFromCanDeviceProcess(char * szData);
	int sendToCanDeviceProcess(char * szData, int nLength);
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();


private:
	SENDCOMMAND m_pPrintfFun = NULL; //���ͺ���
	bool isPrepareToSendCAN();
	CWnd* m_pParent;
	DWORD m_canDeviceProcessId ;  //����id
	HANDLE m_hPipe ;//�ܵ����

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
