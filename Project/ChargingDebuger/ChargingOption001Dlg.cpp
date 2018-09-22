
// ChargingOption001Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChargingOption001.h"
#include "ChargingOption001Dlg.h"
#include "afxdialogex.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChargingOption001Dlg �Ի���

static CChargingOption001Dlg* pThis = NULL;

CChargingOption001Dlg::CChargingOption001Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChargingOption001Dlg::IDD, pParent)
{
	pThis = this;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChargingOption001Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT_INFO, richeditctrl);
	DDX_Control(pDX, IDC_TAB, m_tab);
}

BEGIN_MESSAGE_MAP(CChargingOption001Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON() 
	ON_BN_CLICKED(IDC_BTN_OPEN_COM, &CChargingOption001Dlg::OnBnClickedBtnOpenCom)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CChargingOption001Dlg::OnBnClickedBtnClear)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CChargingOption001Dlg::OnTcnSelchangeTab)
	ON_MESSAGE(WM_READYTOREAD, OnReadyToRead)
	ON_MESSAGE(WM_COMSTAT_LEN, OnShowInfo)
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_BTN_OPEN_COM2, &CChargingOption001Dlg::OnBnClickedBtnOpenCom2)
END_MESSAGE_MAP()


// CChargingOption001Dlg ��Ϣ�������

BOOL CChargingOption001Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	//��ȡ���ô���
	GetCom();

	//��ʼ��������ComboBox
	initBaudComboBox();

	m_tab.InsertItem(1, _T("����"));
	m_tab.InsertItem(2, _T("��ַ����"));
	m_tab.InsertItem(3, _T("CAN�豸��ַ����"));
	m_ppage1 = new CPage1_General(this);
	m_ppage2 = new CPage2_SetAddress(this);
	m_ppage3 = new CPage3_SetAddressCAN(this);
	m_ppage1->Create(IDD_DIALOG1, GetDlgItem(IDC_TAB));
	m_ppage2->Create(IDD_DIALOG2, GetDlgItem(IDC_TAB));
	m_ppage3->Create(IDD_DIALOG3, GetDlgItem(IDC_TAB));

	CRect rs;
	m_tab.GetClientRect(rs);//��õ�ǰIDC_TAB��rectֵ
	rs.top += 20;
	rs.bottom -= 1;
	rs.left -= 2;
	rs.right -= 1;//����rect��ֵ���и���

	m_ppage1->SetParent(GetDlgItem(IDC_TAB));
	m_ppage2->SetParent(GetDlgItem(IDC_TAB));
	m_ppage3->SetParent(GetDlgItem(IDC_TAB));

	m_ppage1->MoveWindow(rs);
	m_ppage2->MoveWindow(rs);
	m_ppage3->MoveWindow(rs);

	rs.top -= 22;
	rs.bottom -= 25;
	rs.left += 5; 
	m_ppage1->setStateRect(rs);
	m_ppage2->setStateRect(rs);
	m_ppage3->setStateRect(rs);
	m_ppage1->setSendFun(SendFunc);
	m_ppage2->setSendFun(SendFunc);
	m_ppage3->setprintfFun(PrintfFunc);
	m_ppage1->ShowWindow(SW_SHOW);//Ĭ�ϴ򿪵�һ���Ӵ���
	m_tab.SetCurSel(0);//ѡ�Ĭ��ѡ�е�һ��


	//CHARFORMAT cf;
	//ZeroMemory(&cf, sizeof(CHARFORMAT));
	//cf.cbSize = sizeof(CHARFORMAT);
	//cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE | CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
	//cf.dwEffects = 0;
	//_tcscpy_s(cf.szFaceName, _T("������"));//�������� 
	//cf.yHeight = 18 * 18;//���ָ߶�
	//cf.crTextColor = RGB(0, 0, 0); //������ɫ
	//richeditctrl.SetSel(-1, -1);  
	//richeditctrl.SetSelectionCharFormat(cf); 
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CChargingOption001Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CChargingOption001Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


 

void CChargingOption001Dlg::SendFunc(CString strCommand)
{
	pThis->SendFunc_(strCommand);
}
void CChargingOption001Dlg::PrintfFunc(CString strContent)
{
	pThis->printf_textEdit(strContent);
}
char* w2c(const wchar_t* wp)
{
	char *m_char;
	int len = WideCharToMultiByte(CP_ACP, 0, wp, wcslen(wp), NULL, 0, NULL, NULL);
	m_char = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, wp, wcslen(wp), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	return m_char;
}
void c2w(wchar_t *pwstr, size_t len, const char *str)
{
	if (str)
	{
		size_t nu = strlen(str);
		size_t n = (size_t)MultiByteToWideChar(CP_ACP, 0, (const char *)str, (int)nu, NULL, 0);
		if (n >= len)n = len - 1;
		MultiByteToWideChar(CP_ACP, 0, (const char *)str, (int)nu, pwstr, (int)n);
		pwstr[n] = 0;
	}
}
void calculateXOR(char* strCommand)
{
	char *mm = nullptr;
	char ba[256] = { 0 };
	memcpy(ba, strCommand, strlen(strCommand));

	mm = ba;
	char XOR = 0;
	for (size_t i = 1; i < strlen(mm); i++)
	{
		if (i == 1)
			XOR = mm[0] ^ mm[1];
		else
			XOR ^= mm[i];
	}
	if (XOR == 0x00 )
		XOR = 0x0B;

	ba[strlen(ba)] = XOR;
	ba[strlen(ba)] = ',';

	ba[strlen(ba)] = char(0x0d);
	ba[strlen(ba)] = char(0x0a);
	int tempLen = strlen(ba);
	memcpy(strCommand, ba, tempLen);
}

//���ͻص�����
void CChargingOption001Dlg::SendFunc_(CString strCommand)
{	
	if (strCommand.IsEmpty())
	{
		printf_textEdit(L"�����Ϊ��\r\n");
		return;
	}
	static char szSend[256] = { 0 }, szRevc[256] = { 0 };
	memset(szSend, 0, 256); memset(szRevc, 0, 256);
	memcpy(szSend, w2c(strCommand), strCommand.GetLength());
	calculateXOR(szSend);
	std::string strSend;
	strSend = (char*)szSend;
	int tempLen = strSend.length();
	int pos = strSend.find("\r\n");

	sprintf_s((char*)szSend, 256, "%s", strSend.substr(0, pos + 2).c_str());
	tempLen = strlen((char*)szSend);
	strSend = szSend;
	printf_textEdit(L"���ͣ�" + TransCommand(CString(strSend.c_str())) + (IsDlgButtonChecked(IDC_CHECK_SHOWHEX) ? L"\r\n" : L""));

		
	if (m_bOpenCom)
	{
		m_SerialPort.ReadData((char*)szRevc); //���
		memset(szRevc, 0, 256);
		//SetDealMessage(false);  //���ò���ҪSerialPort ֪ͨDlg�� ��Dlg����ȥ��ȡ��0.5�볬ʱ
		bool flag = m_SerialPort.WriteData((unsigned char*)szSend, tempLen);//��������
		char szTemp = 0; int i = 0, icount = 0;;
		std::string strRevc;
		
		while (1)
		{
			icount++;
			::Sleep(SLEEP_TIME_INTERVAL * 5);
			
			m_SerialPort.ReadData((char*)szRevc);
			strRevc += (char*)szRevc;
			int posBegin = strRevc.find("*NF");
			int posEnd = strRevc.find("\r\n");
			if (posBegin != -1 && posEnd != -1 && posBegin < posEnd && !strRevc.empty())
			{
				icount = 0;
				printf_textEdit(L"���յ���" + TransCommand(CString(strRevc.c_str())) + (IsDlgButtonChecked(IDC_CHECK_SHOWHEX) ? L"\r\n" : L""));
				break;
			}
			if (icount >= 3)  //3 * 10 * 50ms  =0.5s��ȴ�
			{ 
				CString str;
				if (strRevc.length() != 0)
				{
					str = L"���յ��������ݣ�";
					if (posBegin == -1)
						str += L"δ�ҵ�*NF��";
					if (posEnd == -1)
						str += L"δ�ҵ�\\r\\n��";
					if (posBegin > posEnd)
						str += L"*NF��\\r\\nλ�ò���ȷ";
					printf_textEdit(str + TransCommand(CString(strRevc.c_str())) + (IsDlgButtonChecked(IDC_CHECK_SHOWHEX) ? L"\r\n" : L"\r\n"),1);

				}
				else
				{
					str.Format(L"���յȴ�%d���볬ʱ\r\n", icount * SLEEP_TIME_INTERVAL * 10);
					printf_textEdit(str,1);
				}
				break;
			}
		}		
	}
	else

		printf_textEdit(L"����δ��\r\n",1);

	this->SetFocus();
}

void CChargingOption001Dlg::GetCom()
{
	 
	//��������ʱ��ȡȫ�����ô���
	//HANDLE  hCom;
	//int i, num, k;
	//CString str;
	//BOOL flag;
	//((CComboBox *)GetDlgItem(IDC_COMBO_SERIALPORT))->ResetContent();
	//flag = FALSE;
	//num = 0;
	//for (i = 1; i <= 16; i++)
	//{//�˳���֧��16������
	//	str.Format(L"\\\\.\\COM%d", i);
	//	hCom = CreateFile(str, 0, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	//	if (INVALID_HANDLE_VALUE != hCom)
	//	{//�ܴ򿪸ô��ڣ�����Ӹô���
	//		CloseHandle(hCom);
	//		str = str.Mid(4);
	//		((CComboBox *)GetDlgItem(IDC_COMBO_SERIALPORT))->AddString(str);
	//		if (flag == FALSE)
	//		{
	//			flag = TRUE;
	//			num = i;
	//		}
	//	}
	//}
	//i = ((CComboBox *)GetDlgItem(IDC_COMBO_SERIALPORT))->GetCount();
	//if (i == 0)
	//{//���Ҳ������ô�������á��򿪴��ڡ�����
	//	((CComboBox *)GetDlgItem(IDC_COMBO_SERIALPORT))->EnableWindow(FALSE);
	//}
	//else
	//{
	//	k = ((CComboBox *)GetDlgItem((IDC_COMBO_SERIALPORT)))->GetCount();
	//	((CComboBox *)GetDlgItem(IDC_COMBO_SERIALPORT))->SetCurSel(k - 1);
	//	//mCom.BindCommPort(num);
	//}

	HKEY hKey;
	//LPCWSTR subkey;
	wchar_t keyname[256]; //��������  
	char keyvalue[256];  //��ֵ����  
	DWORD keysize, type, valuesize;
	int indexnum;

	CString commresult;

	if (::RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), 0, KEY_READ, &hKey) != 0) {
		//CString error = "Cannot open regedit!";//�޷���ע���ʱ����error  
		return ;
	}//if(::RegOpenKeyEx(HKEY_LOCAL_MACHINE,subkey,0,KEY_READ,&hKey)!=0)  

	CString keymessage;//����  
	CString message;
	CString valuemessage;//��ֵ  

	for (int i = 0; i < 20; i++)
	{
		indexnum = i;//Ҫ��ȡ��ֵ��������  
		keysize = sizeof(keyname);
		valuesize = sizeof(keyvalue);
		if (RegEnumValue(hKey, indexnum, keyname, &keysize, 0, &type, (BYTE*)keyvalue, &valuesize) == 0) { //�оټ�����ֵ  
			for (int i = 0; i < (int)keysize; i++) {
				message = keyname[i];
				keymessage +=  message;
			}

			for (int j = 0; j < (int)valuesize; j++) {
				if (keyvalue[j] != 0x00) {
					valuemessage += keyvalue[j];
				}
			}
			((CComboBox *)GetDlgItem(IDC_COMBO_SERIALPORT))->AddString(valuemessage);
			((CComboBox *)GetDlgItem(IDC_COMBO_SERIALPORT2))->AddString(valuemessage);
			valuemessage = "";
		}
	
	}
	int k = ((CComboBox *)GetDlgItem((IDC_COMBO_SERIALPORT)))->GetCount();	
	((CComboBox *)GetDlgItem(IDC_COMBO_SERIALPORT))->SetCurSel(k - 1);
	k = ((CComboBox *)GetDlgItem((IDC_COMBO_SERIALPORT2)))->GetCount();
	((CComboBox *)GetDlgItem(IDC_COMBO_SERIALPORT2))->SetCurSel(k - 1);
	RegCloseKey(hKey);//�ر�ע���  

	return ; 
}

template<class T> //����ģ�壬ʹ�ô˺����ܱ����ö�����'*'����������������ʹ��
inline T square(T val) { //����ƽ������
	return val * val; //����ƽ��ֵ
}
void CChargingOption001Dlg::initBaudComboBox()
{
	CString strBaud; int baseBaud = 600, nBaud = 0, multiple=1;
	for (int i = 0; i < 13; i++)
	{
		multiple = 1;
		if (i < 5){
			multiple <<= i; nBaud = multiple *baseBaud;
		}
		else if (5 == i){
			nBaud = 24 * baseBaud;//
		}
		else if (6 == i){
			nBaud = 32 * baseBaud;
		}
		else if (7 == i)
			nBaud = 48 * baseBaud;
		else if (8 == i)
			nBaud = 64 * baseBaud;
		else if (9 == i)
			nBaud = 96 * baseBaud;
		else if (10 == i)
			nBaud = 192 * baseBaud;
		else if (11 == i)
			nBaud = 384 * baseBaud;
		else if (12 == i)
			nBaud = 768 * baseBaud;
		
		
		strBaud.Format(L"%d", nBaud);
		((CComboBox *)GetDlgItem(IDC_COMBO_BAUD1))->AddString(strBaud);
	}
	((CComboBox *)GetDlgItem(IDC_COMBO_BAUD1))->SetCurSel(6);
}

void CChargingOption001Dlg::OnBnClickedBtnOpenCom()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	
	CButton* pBtn = ((CButton *)GetDlgItem(IDC_BTN_OPEN_COM));
	CComboBox* pCom = ((CComboBox *)GetDlgItem(IDC_COMBO_SERIALPORT));
  	if (pBtn  && pCom)
	{
		CString strBtn;
		pBtn->GetWindowTextW(strBtn);
		CString strCom;
		pCom->GetWindowText(strCom);
		CString strCom2 = strCom.Trim(L"COM");
		pCom->GetWindowText(strCom);
		CString strBaud;UINT baud = 0;
		((CComboBox *)GetDlgItem(IDC_COMBO_BAUD))->GetWindowText(strBaud);
		baud = _ttoi(strBaud);
		if (strBtn.Compare(L"�򿪴���")==0)
		{		
			
			int temp = _ttoi(strCom2);
			
			//if (!m_SerialPort.InitPort(temp, baud, 'N', 8, 1, EV_RXCHAR, GetSafeHwnd()))
			if (!m_SerialPort.openPort(temp, baud, 'N', 8, 1, EV_RXCHAR))
			{
				printf_textEdit(strCom + L"��ʧ��\r\n",1);
			}
			else
			{
				pCom->EnableWindow(FALSE);
				printf_textEdit(strCom + L"�򿪳ɹ�\r\n");
				if (m_SerialPort.isThreadRunning())
				{
					printf_textEdit(L"�򿪼����̳߳ɹ�\r\n");
					m_bOpenCom = true;
					pBtn->SetWindowTextW(L"�رմ���");
				}
			}
		}
		else if (strBtn.Compare(L"�رմ���") == 0)
		{
			pCom->EnableWindow(TRUE);
			m_SerialPort.CloseListenTread();
			m_SerialPort.ClosePort();
			pBtn->SetWindowTextW(L"�򿪴���");
			printf_textEdit(strCom+L"�ر�\r\n");

			m_bOpenCom = false;
		}
	}
}

void CChargingOption001Dlg::printf_textEdit(CString strCon, int nInfoType)
{ 
	//static CHARFORMAT cf;
	//ZeroMemory(&cf, sizeof(CHARFORMAT));
	//cf.cbSize = sizeof(CHARFORMAT);
	//cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE | CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
	//cf.dwEffects = 0;
	//_tcscpy_s(cf.szFaceName, _T("������"));//��������

	//if (nInfoType == 0){
	//	cf.yHeight = 13 * 13;//���ָ߶�
	//	cf.crTextColor = RGB(0, 0, 0); //������ɫ
	//}
	//else if (nInfoType == 1){
	//	cf.yHeight = 16 * 16;//���ָ߶�
	//	//cf.crTextColor = RGB(250, 0, 0); //������ɫ
	//} 
	
	CString strL; richeditctrl.GetWindowTextW(strL);
	richeditctrl.SetSel(-1, -1);
	
	richeditctrl.ReplaceSel((LPCTSTR)strCon); 
	/*int nStart = strL.GetLength(), nLong = strCon.GetLength();
	richeditctrl.SetSel(nStart+4, nLong);
	richeditctrl.SetSelectionCharFormat(cf);*/
	/*static CString str;
	pEdit->GetWindowText(str);
	pEdit->SetSel(str.GetLength(), str.GetLength());*/
		 
	::SendMessage(richeditctrl.GetSafeHwnd(), WM_VSCROLL, SB_BOTTOM, 0); //��������
	/*int cnt = pEdit->GetLineCount();
	pEdit->LineScroll(cnt, 0);*/
	 
}

CString CChargingOption001Dlg::toHEX(CString strSource)
{ 
	TCHAR szTemp[10] = { 0 }, szTemp2;
	CString strDes;
	for (int i = 0; i < strSource.GetLength(); i++)
	{
		szTemp2 = strSource[i];
		swprintf_s(szTemp, 10, L"%02X ", szTemp2);
		strDes += szTemp;
	}
	return strDes;
}


void CChargingOption001Dlg::OnBnClickedBtnClear()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CRichEditCtrl* 	pEdit = (CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT_INFO);
	if (pEdit)
	{
		pEdit->SetWindowTextW(L"");
	}
}


void CChargingOption001Dlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	m_ppage1->ShowWindow(FALSE);
	m_ppage2->ShowWindow(FALSE);
	m_ppage3->ShowWindow(FALSE);
	switch (m_tab.GetCurSel())//Tabѡ���ѡ�е�ѡ��
	{
	case 0:
		m_ppage1->ShowWindow(TRUE);
		break;
	case 1: 
		m_ppage2->ShowWindow(TRUE); 
		break;
	case 2:
		m_ppage3->ShowWindow(TRUE);
		break;
	}
	m_ppage1->UpdateData(true);
	m_ppage2->UpdateData(true);
	m_ppage3->UpdateData(true);
	this->SetFocus();
}

CString CChargingOption001Dlg::TransCommand(CString strCommand)
{
	CButton *pCheck = (CButton*)GetDlgItem(IDC_CHECK_SHOWHEX);
	CString strRet;strRet = strCommand;
	if (pCheck && pCheck->GetCheck())
	{
		strRet = toHEX(strCommand);		  
	}
	return strRet;
}

//׼����ȡ����
LRESULT CChargingOption001Dlg::OnReadyToRead(WPARAM wp, LPARAM lp)
{
	bool flag = IsDealMessage();
	if (IsDealMessage()){
		static char szRevc[256] = { 0 }; memset(szRevc, 0, 256);
		static int tempLen;
		if (wp == 0){
			//m_SerialPort.ReadChar_Ba((char*)szRevc);
			m_SerialPort.ReadData(szRevc);
			printf_textEdit(L"�豸COM���յ���" + TransCommand(CString(szRevc)) + (IsDlgButtonChecked(IDC_CHECK_SHOWHEX) ? L"\r\n" : L""));
			tempLen = strlen(szRevc);
			if (m_bOpenCom2)
				m_SerialPort2.WriteData((unsigned char*)szRevc, tempLen);
		}
		else if (wp == MY_TRANSPOND_COM_OBJ_ID){
			if (m_bOpenCom2)
			{
				m_SerialPort2.ReadData((char*)szRevc);
				printf_textEdit(L"ת��COMд����" + TransCommand(CString(szRevc)) + (IsDlgButtonChecked(IDC_CHECK_SHOWHEX) ? L"\r\n" : L""));
				tempLen = strlen(szRevc);
				if (m_bOpenCom)
					m_SerialPort.WriteData((unsigned char*)szRevc, tempLen);
			}
		}
	}
	else{
		SetDealMessage(true);  //���ô���������ʱ����֪ͨ
	}
	return 0L;
}

LRESULT CChargingOption001Dlg::OnShowInfo(WPARAM wParam, LPARAM lParam)
{
	wchar_t* pwChar = (wchar_t*)wParam;
	CString str; str.Format(L"%s\r\n", pwChar);
	printf_textEdit(str);
	delete pwChar;
	pwChar = nullptr;
	return 0L;

}


void CChargingOption001Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (nID)
	{
	case SC_MAXIMIZE:
		ShowWindow(SW_SHOWMAXIMIZED);
		return;
		break;
	case SC_MINIMIZE:
		ShowWindow(SW_MINIMIZE);
		return;
		break;
	case SC_RESTORE:
		//	AfxMessageBox("Restore");
		ShowWindow(SW_RESTORE);
		//return;
		break;
	}
	CDialogEx::OnSysCommand(nID, lParam);
}


BOOL CChargingOption001Dlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���
	
	/// �ɼ�������ഫ����Ϣ
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CChargingOption001Dlg::OnBnClickedBtnOpenCom2()
{
	CButton* pBtn = ((CButton *)GetDlgItem(IDC_BTN_OPEN_COM2));
	CComboBox* pCom = ((CComboBox *)GetDlgItem(IDC_COMBO_SERIALPORT2));
	if (pBtn  && pCom)
	{
		CString strBtn;
		pBtn->GetWindowTextW(strBtn);
		CString strCom;
		pCom->GetWindowText(strCom);
		CString strCom2 = strCom.Trim(L"COM");
		pCom->GetWindowText(strCom);
		CString strBaud; UINT baud = 0;
		((CComboBox *)GetDlgItem(IDC_COMBO_BAUD))->GetWindowText(strBaud);
		baud = atoi((char*)strBaud.GetBuffer(0));
		if (strBtn.Compare(L"�򿪴���") == 0)
		{

			int temp = _ttoi(strCom2);
			//if (!m_SerialPort2.InitPort(temp, baud, 'N', 8, 1, EV_RXCHAR, GetSafeHwnd()))
			if (!m_SerialPort2.openPort(temp, baud, 'N', 8, 1, EV_RXCHAR))
			{
				printf_textEdit(strCom + L"��ʧ��2\r\n", 1);
			}
			else
			{
				pCom->EnableWindow(FALSE);
				printf_textEdit(strCom + L"�򿪳ɹ�\r\n");
				if (m_SerialPort2.isThreadRunning())
				{
					printf_textEdit(L"�򿪼����̳߳ɹ�2\r\n");
					m_bOpenCom2 = true;
					pBtn->SetWindowTextW(L"�رմ���"); 
				}
			}
		}
		else if (strBtn.Compare(L"�رմ���") == 0)
		{
			pCom->EnableWindow(TRUE);
			m_SerialPort2.CloseListenTread();
			m_SerialPort2.ClosePort();
			pBtn->SetWindowTextW(L"�򿪴���");
			printf_textEdit(strCom + L"�ر�\r\n");

			m_bOpenCom2 = false;
		}
	}
}
