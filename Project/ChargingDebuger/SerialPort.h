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

/** 串口通信类
*
*  本类实现了对串口的基本操作
*  例如监听发到指定串口的数据、发送指定数据到串口
*/

#define SP_MAX_COMM_DATA_LEN								256
#define SP_SPECIAL_BAUDRATE									19200  //波特率
const UINT SLEEP_TIME_INTERVAL = 10;
 

int  SUM(int a, int b);  
 
class  CSerialPort
{
	
public:	
	CSerialPort();
	/*获取类实例
	
	*/
	static CSerialPort* GetInstance();

	/*析构函数
	 
	*/
	~CSerialPort();

	/*获取串口信息  
	* @param:  char    输出串口的缓冲区,（格式如：COM1,COM2,COM3,...)
	* @param:  iError   错误码 ，0无错 -1 打开注册表失败
	*/
	void getComInfo(char* szOut, int iError);

	/** 初始化串口函数
	*
	*  @param:  UINT portNo 串口编号,默认值为1,即COM1, 
	*  @param:  UINT baud   波特率,默认为9600
	*  @param:  char parity 是否进行奇偶校验,'Y'表示需要奇偶校验,'N'表示不需要奇偶校验
	*  @param:  UINT databits 数据位的个数,默认值为8个数据位
	*  @param:  UINT stopsbits 停止位使用格式,默认值为1
	*  @param:  DWORD dwCommEvents 默认为EV_RXCHAR,即只要收发任意一个字符,则产生一个事件
	*  @return: bool  初始化是否成功
	*  @note:   在使用其他本类提供的函数前,请先调用本函数进行串口的初始化
	*　　　　　   \n本函数提供了一些常用的串口参数设置,若需要自行设置详细的DCB参数,可使用重载函数
	*           \n本串口类析构时会自动关闭串口,无需额外执行关闭串口
	*  @see:
	*/
	bool openPort(UINT  portNo = 1, UINT  baud = CBR_19200, char  parity = 'N', UINT  databits = 8, UINT  stopsbits = 1, DWORD dwCommEvents = EV_RXCHAR);

	/** 关闭串口
	*
	*
	*  @return: void  操作是否成功
	*  @note:
	*  @see:
	*/
	void ClosePort();

	/** 开启监听线程
	*
	*  本监听线程完成对串口数据的监听,并将接收到的数据打印到屏幕输出
	*  @return: bool  操作是否成功
	*  @note:   当线程已经处于开启状态时,返回flase
	*  @see:
	*/
	bool OpenListenThread();

	/** 关闭监听线程
	*
	*
	*  @return: bool  操作是否成功
	*  @note:   调用本函数后,监听串口的线程将会被关闭
	*  @see:
	*/
	bool CloseListenTread();

	/** 向串口写数据
	*
	*  将缓冲区中的数据写入到串口
	*  @param:  unsigned char * pData 指向需要写入串口的数据缓冲区
	*  @param:  unsigned int length 需要写入的数据长度
	*  @return: bool  操作是否成功
	*  @note:   length不要大于pData所指向缓冲区的大小
	*  @see:
	*/
	bool WriteData(unsigned char* pData, unsigned int length);



	/** 读取串口接收缓冲区中的数据
	*  @param:  char & cRecved 存放读取数据的字符变量	
	*  @return: bool  读取是否成功
	*  @note:
	*  @see:
	*/ 

	bool ReadData(char *cRecved);

	
	/** 线程是否正在运行
	*  @return: bool  true正在运行线程
	*  @note:
	*  @see:
	*/
	bool isThreadRunning(){ return m_bThreadRunning; }


private:
	/** 获取串口缓冲区中的字节数
	*
	*
	*  @return: UINT  操作是否成功
	*  @note:   当串口缓冲区中无数据时,返回0
	*  @see:
	*/
	UINT GetBytesInCOM();

	/** 串口监听线程
	*
	*  监听来自串口的数据和信息
	*  @param:  void * pParam 线程参数
	*  @return: UINT WINAPI 线程返回值
	*  @note:
	*  @see:
	*/
	static UINT WINAPI ListenThread(void* pParam);

	void ListenThreadReceving();

	//接收数据
	int	ReceiveData(DWORD dwBytes);

	/** 打开串口
	*
	*  @param:  UINT openPortFile 串口设备号
	*  @return: bool  打开是否成功
	*  @note:
	*  @see:
	*/
	bool openPortFile(UINT  portNo);
private:
	/** 串口句柄 */
	HANDLE  m_hComm;

	COMMTIMEOUTS	m_CommTimeouts;						//超时结构
	DCB				m_DCB;								//设备控制块结构

	OVERLAPPED		m_ovRead;							//读重叠结构
	OVERLAPPED		m_ovWrite;							//写重叠结构

	DWORD			m_dwRecvNum;//帧缓冲区接收字节数
	BYTE			m_btFrameReceiveBuf[SP_MAX_COMM_DATA_LEN];//通信帧接收缓冲
	BYTE			m_btReceiveBuf[SP_MAX_COMM_DATA_LEN];//通信口缓冲
	BYTE			m_btSendingBuf[SP_MAX_COMM_DATA_LEN];//通信口缓冲
	DWORD           m_dwRecvlen;  //历史接收长度


	HANDLE			m_hWriteEvent;						//发送
	HANDLE			m_hWriteShutdownEvent;				//关闭写线程事件
	HANDLE			m_hReadShutdownEvent;				//关闭读线程事件
	HANDLE			m_hReadEventArray[2];				//读线程事件数组
	HANDLE			m_hWriteEventArray[2];				//写线程事件数组	


	/** 线程退出标志变量 */
	static bool s_bExit;

	/** 线程句柄 */
	volatile HANDLE    m_hListenThread;

	/** 同步互斥,临界区保护 */
	CRITICAL_SECTION   m_csCommunicationSync;       //!< 互斥操作串口  

	bool m_bThreadRunning = false;
};

