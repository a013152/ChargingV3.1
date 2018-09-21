//////////////////////////////////////////////////////////////////////////  
/// COPYRIGHT NOTICE  
/// Copyright (c) 2009, ���пƼ���ѧtickTick Group  ����Ȩ������  
/// All rights reserved.  
///   
/// @file    SerialPort.h    
/// @brief   ����ͨ����ͷ�ļ�  
///  
/// ���ļ���ɴ���ͨ���������  
///  
/// @version 1.0     
/// @author  ¬��   
/// @E-mail��lujun.hust@gmail.com  
/// @date    2010/03/19  
///  
///  �޶�˵����  
//////////////////////////////////////////////////////////////////////////  

#ifndef SERIALPORT_H_  
#define SERIALPORT_H_  

#include <Windows.h>  

/** ����ͨ����
*
*  ����ʵ���˶Դ��ڵĻ�������
*  �����������ָ�����ڵ����ݡ�����ָ�����ݵ�����
*/

#define SP_MAX_COMM_DATA_LEN								256
#define SP_SPECIAL_BAUDRATE									19200  //������

//����궨���ʽ��Ӧ�ǳ����ִ�
//#define PRINT(format, ...) TRACE(__FILE__",%d: "format, __LINE__, ##__VA_ARGS__)

extern const UINT SLEEP_TIME_INTERVAL;
typedef unsigned short FNWORD;
typedef struct _COMM_TIME_DEF
{
	FNWORD					wYear;
	FNWORD					wMonth;//1-12
	FNWORD					wDayOfWeek;//0-6
	FNWORD					wDay;//1-31
	FNWORD					wHour;//0-23
	FNWORD					wMinute;//0-59
	FNWORD					wSecond;//0-59
	FNWORD					wMilliseconds;//0-999
}COMM_TIME_DEF;

class CSerialPort
{
public:
	CSerialPort(void);
	~CSerialPort(void);

public:

	/** ��ʼ�����ں���
	*
	*  @param:  UINT portNo ���ڱ��,Ĭ��ֵΪ1,��COM1,ע��,������Ҫ����9
	*  @param:  UINT baud   ������,Ĭ��Ϊ9600
	*  @param:  char parity �Ƿ������żУ��,'Y'��ʾ��Ҫ��żУ��,'N'��ʾ����Ҫ��żУ��
	*  @param:  UINT databits ����λ�ĸ���,Ĭ��ֵΪ8������λ
	*  @param:  UINT stopsbits ֹͣλʹ�ø�ʽ,Ĭ��ֵΪ1
	*  @param:  DWORD dwCommEvents Ĭ��ΪEV_RXCHAR,��ֻҪ�շ�����һ���ַ�,�����һ���¼�
	*  @return: bool  ��ʼ���Ƿ�ɹ�
	*  @note:   ��ʹ�����������ṩ�ĺ���ǰ,���ȵ��ñ��������д��ڵĳ�ʼ��
	*����������   \n�������ṩ��һЩ���õĴ��ڲ�������,����Ҫ����������ϸ��DCB����,��ʹ�����غ���
	*           \n������������ʱ���Զ��رմ���,�������ִ�йرմ���
	*  @see:
	*/
	bool InitPort(UINT  portNo = 1, UINT  baud = CBR_9600, char  parity = 'N', UINT  databits = 8, UINT  stopsbits = 1, DWORD dwCommEvents = EV_RXCHAR, HWND handleParent = NULL);

	/** ���ڳ�ʼ������
	*
	*  �������ṩֱ�Ӹ���DCB�������ô��ڲ���
	*  @param:  UINT portNo
	*  @param:  const LPDCB & plDCB
	*  @return: bool  ��ʼ���Ƿ�ɹ�
	*  @note:   �������ṩ�û��Զ���ش��ڳ�ʼ������
	*  @see:
	*/
	bool InitPort(UINT  portNo, const LPDCB& plDCB);

	/** ���������߳�
	*
	*  �������߳���ɶԴ������ݵļ���,�������յ������ݴ�ӡ����Ļ���
	*  @return: bool  �����Ƿ�ɹ�
	*  @note:   ���߳��Ѿ����ڿ���״̬ʱ,����flase
	*  @see:
	*/
	bool OpenListenThread();

	/** �رռ����߳�
	*
	*
	*  @return: bool  �����Ƿ�ɹ�
	*  @note:   ���ñ�������,�������ڵ��߳̽��ᱻ�ر�
	*  @see:
	*/
	bool CloseListenTread();

	/** �򴮿�д����
	*
	*  ���������е�����д�뵽����
	*  @param:  unsigned char * pData ָ����Ҫд�봮�ڵ����ݻ�����
	*  @param:  unsigned int length ��Ҫд������ݳ���
	*  @return: bool  �����Ƿ�ɹ�
	*  @note:   length��Ҫ����pData��ָ�򻺳����Ĵ�С
	*  @see:
	*/
	bool WriteData(unsigned char* pData, unsigned int length);

	/** ��ȡ���ڻ������е��ֽ���
	*
	*
	*  @return: UINT  �����Ƿ�ɹ�
	*  @note:   �����ڻ�������������ʱ,����0
	*  @see:
	*/
	UINT GetBytesInCOM();

	/** ��ȡ���ڽ��ջ�������һ���ֽڵ�����
	*
	*
	*  @param:  char & cRecved ��Ŷ�ȡ���ݵ��ַ�����
	*  @param:  UINT & length ��ȡ����
	*  @return: bool  ��ȡ�Ƿ�ɹ�
	*  @note:
	*  @see:
	*/
	bool ReadChar(char *cRecved, unsigned int length);


	bool ReadChar_Ba(char *cRecved);

	void WriteChar_Ba(char *cRecved, int len);

	bool isThreadRunning(){ return m_bThreadRunning; }


	/** �򿪴���
	*
	*
	*  @param:  UINT portNo �����豸��
	*  @return: bool  ���Ƿ�ɹ�
	*  @note:
	*  @see:
	*/
	bool openPort(UINT  portNo);
	/** �رմ���
	*
	*
	*  @return: void  �����Ƿ�ɹ�
	*  @note:
	*  @see:
	*/
	void ClosePort();
private:

	

	

	/** ���ڼ����߳�
	*
	*  �������Դ��ڵ����ݺ���Ϣ
	*  @param:  void * pParam �̲߳���
	*  @return: UINT WINAPI �̷߳���ֵ
	*  @note:
	*  @see:
	*/
	static UINT WINAPI ListenThread(void* pParam);

	void ListenThreadReceving();

	//��������
	int	ReceiveData(DWORD dwBytes);

public:
	BYTE* GetMemBuf(){ return m_btReceiveBuf; } 

	HWND GetParentHWND(){ return m_handleParent; }

	int GetComObjID(){ return m_iComOBJID; } //��ȡת����־
	void SetComObjID(int iID){ m_iComOBJID = iID!=0; } //����ת����־
private:

	/** ���ھ�� */
	HANDLE  m_hComm;

	COMMTIMEOUTS	m_CommTimeouts;						//��ʱ�ṹ
	DCB				m_DCB;								//�豸���ƿ�ṹ

	OVERLAPPED		m_ovRead;							//���ص��ṹ
	OVERLAPPED		m_ovWrite;							//д�ص��ṹ

	DWORD			m_dwRecvNum;//֡�����������ֽ���
	BYTE			m_btFrameReceiveBuf[SP_MAX_COMM_DATA_LEN];//ͨ��֡���ջ���
	BYTE			m_btReceiveBuf[SP_MAX_COMM_DATA_LEN];//ͨ�ſڻ���
	BYTE			m_btSendingBuf[SP_MAX_COMM_DATA_LEN];//ͨ�ſڻ���


	HANDLE			m_hWriteEvent;						//����
	HANDLE			m_hWriteShutdownEvent;				//�ر�д�߳��¼�
	HANDLE			m_hReadShutdownEvent;				//�رն��߳��¼�
	HANDLE			m_hReadEventArray[2];				//���߳��¼�����
	HANDLE			m_hWriteEventArray[2];				//д�߳��¼�����	


	/** �߳��˳���־���� */
	static bool s_bExit;

	/** �߳̾�� */
	volatile HANDLE    m_hListenThread;

	/** ͬ������,�ٽ������� */
	CRITICAL_SECTION   m_csCommunicationSync;       //!< �����������  

	bool m_bThreadRunning = false; 

	bool m_iComOBJID = 0;

	HWND m_handleParent = NULL;

};

#endif //SERIALPORT_H_ 