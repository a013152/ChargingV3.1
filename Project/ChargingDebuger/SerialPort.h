#pragma once
#ifdef DLL_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

#include <string.h>
#include <stdio.h>
#include <windows.h>          // for HANDLE  
#include <process.h>          // for _beginthread()  

/** ����ͨ����
*
*  ����ʵ���˶Դ��ڵĻ�������
*  �����������ָ�����ڵ����ݡ�����ָ�����ݵ�����
*/

#define SP_MAX_COMM_DATA_LEN								256
#define SP_SPECIAL_BAUDRATE									19200  //������
const UINT SLEEP_TIME_INTERVAL = 10;
 

int  SUM(int a, int b);  
 
class  CSerialPort
{
	
public:	
	CSerialPort();
	/*��ȡ��ʵ��
	
	*/
	static CSerialPort* GetInstance();

	/*��������
	 
	*/
	~CSerialPort();

	/*��ȡ������Ϣ  
	* @param:  char    ������ڵĻ�����,����ʽ�磺COM1,COM2,COM3,...)
	* @param:  iError   ������ ��0�޴� -1 ��ע���ʧ��
	*/
	void getComInfo(char* szOut, int iError);

	/** ��ʼ�����ں���
	*
	*  @param:  UINT portNo ���ڱ��,Ĭ��ֵΪ1,��COM1, 
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
	bool openPort(UINT  portNo = 1, UINT  baud = CBR_19200, char  parity = 'N', UINT  databits = 8, UINT  stopsbits = 1, DWORD dwCommEvents = EV_RXCHAR);

	/** �رմ���
	*
	*
	*  @return: void  �����Ƿ�ɹ�
	*  @note:
	*  @see:
	*/
	void ClosePort();

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



	/** ��ȡ���ڽ��ջ������е�����
	*  @param:  char & cRecved ��Ŷ�ȡ���ݵ��ַ�����	
	*  @return: bool  ��ȡ�Ƿ�ɹ�
	*  @note:
	*  @see:
	*/ 

	bool ReadData(char *cRecved);

	
	/** �߳��Ƿ���������
	*  @return: bool  true���������߳�
	*  @note:
	*  @see:
	*/
	bool isThreadRunning(){ return m_bThreadRunning; }


private:
	/** ��ȡ���ڻ������е��ֽ���
	*
	*
	*  @return: UINT  �����Ƿ�ɹ�
	*  @note:   �����ڻ�������������ʱ,����0
	*  @see:
	*/
	UINT GetBytesInCOM();

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

	/** �򿪴���
	*
	*  @param:  UINT openPortFile �����豸��
	*  @return: bool  ���Ƿ�ɹ�
	*  @note:
	*  @see:
	*/
	bool openPortFile(UINT  portNo);
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
	DWORD           m_dwRecvlen;  //��ʷ���ճ���


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
};

