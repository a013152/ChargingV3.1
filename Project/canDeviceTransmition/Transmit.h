
#include "Common.h"
#include "ControlCAN.h"
#pragma comment(lib, "ControlCAN.lib")

struct stCAN_DevData;
class CTransmit
{
public:
	
	//函数：GetInstance
	//功能：获取类实例
	//参数：
	//返回值：CTransmit 单实例
	static CTransmit* GetInstance();

	//析构函数
	~CTransmit();
	 
	//函数：openCanDev
	//功能：打开设备
	//参数：  
	//返回值：0 成功， -1 打开失败，-2初始化失败， -3 启动失败
	int openCanDev();

	//函数：closeCanDev
	//功能：关闭设备
	//参数：  
	//返回值：0成功， -1 关闭失败
	int closeCanDev();

	//函数：isOpenCanDev
	//功能：是否打开设备
	//参数：  
	//返回值：true已经打开， false未打开
	bool isOpenCanDev(){ return m_bOpen; }

	//函数：CAN_Receive
	//功能：分析CAN接收的数据
	//参数：  
	//返回值： 
	void CAN_Receive();

	//函数：sendCanData
	//功能：整合VCI数据，发送到CAN协议器
	//参数：dataObj引用数据， canID 设备的ID 例如0x333
	//返回值：0 成功， -1 设备未打开，-2 发送失败。
	int sendCanData(stCAN_DevData &dataObj, UINT canID);
	
	//函数：setWnd
	//功能：设置窗口句柄，用于通知窗口显示接收到的数据
	//参数：HWND wnd窗口句柄
	//返回值：
	void setWnd(HWND wnd){ m_AppWnd = wnd; }

	//函数：getDebugData
	//功能：获取接收到的数据，用于通知窗口显示接收到的数据
	//参数：   
	//返回值：std::string
	std::string getDebugData(){ return m_strDebugData; }

	//函数：m_pPrintfFun
	//功能：打印数据
	//参数： 
	//返回值：
	TYPEcallbackPrintf m_pPrintfFun = NULL;

public:
	HANDLE			m_hReadEventArray[2];				//读线程事件数组 0 退出， 1读取
private:
	CTransmit();
	bool m_bOpen = false;
	std::string m_strDebugData, str1, str;
	char szTemp[256];
	HANDLE  m_hThread = NULL; //线程句柄
	HWND    m_AppWnd = NULL;  //窗口
	BYTE m_bufRecive[MAX_BUF_SIZE]; //接收的buf 
	int m_DevType = VCI_USBCAN2;
	int m_DevIndex = 0;
};

