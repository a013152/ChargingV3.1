#ifndef H_COMMON
#define H_COMMON
 

extern char g_AppPath[256];  //exe路径 

#define  MAIN_WINDOW_TITLE "未读取标题"
#define  SETING_WINDOW_TITLE "系统参数设置"

#define DBHOSTNAME "119.23.48.100"
#define CHARGINGOPTIONPROCESS  "chargingOption.exe"  //设置程序名称（修改电池电压……的进程）
#define CANDEVICETRANSMITION "canDeviceTransmition.exe" //can 设备通讯进程
#define PIPE_NAME "\\\\.\\Pipe\\canDevicePipe" //管道名称
#define S2C "S2C" //进程方向： 服务端向客户端
#define C2S "C2S" //进程方向： 客户端向服务端
#define SPILTE_CHAR ','  //分割字符','
enum enCANDevieErrorCode{	//CAN进程返回的错误码
	Success = 0,
	DeviceNotOpen,
	DetailError,
};
#define MAX_BUF_SIZE 1024 //缓冲区

#define TIMER_INTERVAL 100  //关闭定时器时间间隔 100毫秒

#define ONE_CLOSET_SCAN_TIME 3 * 1000  //单元间隔时间  
#define ONE_SUBMIT_TIME 15000			//网络提交间隔
#define CHARGING_LIMIT_TIME 360		//充电时限，超过则主动停止，分钟为单位 add20180521
#define DEFAUT_OVERHEATTEMPERATURE 45 //过热温度
#define MAX_CLOSET  10  //最大柜子数量
#define MAX_CHARGER 48  //每个柜子最大充电器数量

 
#define MAX_BATTERY 15  //每个柜子最大电池数量

#define ONE_PAGE_CLOSET 5  //每页柜子数量
#define ONE_CLOSET_SCAN_COMMAND 14 //每柜子扫描命令数量 3个M 8个G 3个D 一共14个

#define MAX_RELAY_TO_LOOP 6  //一个继电器对应最多的回路
#define BEGIN_RELAY_ID 25  //一个继电器开始ID


//错误信息
#define ERROR_DONT_OPEN_SERIAL -1;   //串口未放置。
#define ERROR_NO_BATTERY -2;   //电池未放置。
#define ERROR_NO_CHARGER -3;   //充电器未在线
#define ERROR_BATTERY_CHARGING -4;   //其他电池正在放电电。
 
#define ERROR_CONFIG   -5; //未匹配信息
#define ERROR_CONFIG_CLOSET -6;   //未匹配柜子。
#define ERROR_CONFIG_BATTERY -7;   //未匹配电池。
#define ERROR_CONFIG_CHARGER -8;   //未匹配充电器。
#define ERROR_CONFIG_RELAY -9;   //未匹配继电器。
#define ERROR_CONFIG_BATTERY_MODEL -10;   //未匹配电池型号。
#define ERROR_OVER_HEAT -11;   //充电器过热

#define ERROR_DONT_OPEN_CAN -12;   //CAN设备未打开。


#define STATE_OFFLINE "充电器不在线"
#define STATE_FREE "充电器闲置"
#define STATE_CHARGING "充电中"
#define STATE_DISCHARGING "放电中"
#define STATE_APPLYCHARGING "预充"
#define STATE_OVER_HEAT "高温过热"

//调试信息
#define DEBUG_LOG_FLAG  1
#endif // !H_COMMON
