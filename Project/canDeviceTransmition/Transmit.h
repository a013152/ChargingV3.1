
#include "Common.h"
#include "ControlCAN.h"
#pragma comment(lib, "ControlCAN.lib")

struct stCAN_DevData;
class CTransmit
{
public:
	
	//������GetInstance
	//���ܣ���ȡ��ʵ��
	//������
	//����ֵ��CTransmit ��ʵ��
	static CTransmit* GetInstance();

	//��������
	~CTransmit();
	 
	//������openCanDev
	//���ܣ����豸
	//������  
	//����ֵ��0 �ɹ��� -1 ��ʧ�ܣ�-2��ʼ��ʧ�ܣ� -3 ����ʧ��
	int openCanDev();

	//������closeCanDev
	//���ܣ��ر��豸
	//������  
	//����ֵ��0�ɹ��� -1 �ر�ʧ��
	int closeCanDev();

	//������isOpenCanDev
	//���ܣ��Ƿ���豸
	//������  
	//����ֵ��true�Ѿ��򿪣� falseδ��
	bool isOpenCanDev(){ return m_bOpen; }

	//������CAN_Receive
	//���ܣ�����CAN���յ�����
	//������  
	//����ֵ�� 
	void CAN_Receive();

	//������sendCanData
	//���ܣ�����VCI���ݣ����͵�CANЭ����
	//������dataObj�������ݣ� canID �豸��ID ����0x333
	//����ֵ��0 �ɹ��� -1 �豸δ�򿪣�-2 ����ʧ�ܡ�
	int sendCanData(stCAN_DevData &dataObj, UINT canID);
	
	//������setWnd
	//���ܣ����ô��ھ��������֪ͨ������ʾ���յ�������
	//������HWND wnd���ھ��
	//����ֵ��
	void setWnd(HWND wnd){ m_AppWnd = wnd; }

	//������getDebugData
	//���ܣ���ȡ���յ������ݣ�����֪ͨ������ʾ���յ�������
	//������   
	//����ֵ��std::string
	std::string getDebugData(){ return m_strDebugData; }

	//������m_pPrintfFun
	//���ܣ���ӡ����
	//������ 
	//����ֵ��
	TYPEcallbackPrintf m_pPrintfFun = NULL;

public:
	HANDLE			m_hReadEventArray[2];				//���߳��¼����� 0 �˳��� 1��ȡ
private:
	CTransmit();
	bool m_bOpen = false;
	std::string m_strDebugData, str1, str;
	char szTemp[256];
	HANDLE  m_hThread = NULL; //�߳̾��
	HWND    m_AppWnd = NULL;  //����
	BYTE m_bufRecive[MAX_BUF_SIZE]; //���յ�buf 
	int m_DevType = VCI_USBCAN2;
	int m_DevIndex = 0;
};

