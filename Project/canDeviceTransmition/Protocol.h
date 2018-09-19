#pragma once


//CProtocol 协议类
//用于处理大疆充电槽的协议
//author：chenlin
//date：20180712

#include "stdafx.h"
#include "Common.h"
extern "C"{
#include "aes256.h"
#include "crc.h"
}

struct stCAN_DevData;
union UnionCRC;

UINT  Uint8ToUint16(uint8_t * canID);



//电池 信息
struct stBatteryInfo
{
	bool isOline_;
	uint8_t  ChargingMode; //1 打开充电， 2关闭充电，0xff自动充电（低于40%会充到60%)
	float vol_;
	float tempterator;
	float current_;
	SYSTEMTIME   protime_;
	int loop_;  //循环次数
	int capacity_; //容量
	int life_;   //寿命百分比
	char sn_[14];  //SN
	stBatteryInfo() :isOline_(false), ChargingMode(0xff), vol_(0), \
		current_(0), tempterator(0), loop_(0), capacity_(0), life_(0)
	{
			memset(&protime_, 0, sizeof(protime_));
			memset(sn_, 0, sizeof(sn_));
		}
	stBatteryInfo(const stBatteryInfo& other_)
	{
		this->isOline_ = other_.isOline_;
		this->ChargingMode = other_.ChargingMode;
		this->vol_ = other_.vol_;
		this->tempterator = other_.tempterator;
		this->current_ = other_.current_;
		this->loop_ = other_.loop_;
		this->capacity_ = other_.capacity_;
		this->life_ = other_.life_;
		memcpy(&this->protime_, &other_.protime_, sizeof(protime_));
		memcpy(this->sn_, &other_.sn_, sizeof(sn_));
	}
};

//协议类
class CProtocol
{
	CProtocol();
public:
	//函数：GetInstance
	//功能：获取类实例
	//参数：
	//返回值：CTransmit 单实例
	static CProtocol* GetInstance();

	//析构函数
	~CProtocol();

	//函数：setWnd
	//功能：设置窗口句柄，用于通知窗口显示接收到的数据
	//参数：HWND wnd窗口句柄
	//返回值：
	void setWnd(HWND wnd){ m_AppWnd = wnd; }

	//函数：getKey
	//功能：获取密匙
	//参数：  
	//返回值：
	uint8_t* getKey(){
		return m_szKeyDefault;
	};

	//函数：getCommandReadCanId
	//功能：获取读CAN id 的命令
	//参数： dataObj引用 
	//返回值：
	void getCommandReadCanId(stCAN_DevData& dataObj);

	//函数：getCommandReadCanId
	//功能：获取设置CAN id 的命令
	//参数： dataObj引用,  canID要设置的id
	//返回值：
	void getCommandWriteCanId(stCAN_DevData& dataObj, BYTE* canID);

	//函数：getCommandVerify
	//功能：获取认证的命令
	//参数： dataObj引用,  
	//返回值：
	void getCommandVerify(stCAN_DevData& dataObj);

	//函数：getCommandBeginMode
	//功能：获取起始模式的命令
	//参数： dataObj引用,  
	//返回值：
	void getCommandBeginMode(stCAN_DevData& dataObj);

	//函数：getCommandMaxCharge
	//功能：获取最大充电数的命令
	//参数： dataObj引用,  
	//返回值：
	void getCommandMaxCharge(stCAN_DevData& dataObj);

	//函数：getCommandCharge
	//功能：充电的命令
	//参数： dataObj引用, bReadOrWrite 是否充电 false 读取充电状态 true设置充电; 
	//         当bReadOrWrite为true时： changedId 改变状态的id 停止则开启充电，正在充电则停止。
	//返回值：
	void getCommandCharge(stCAN_DevData& dataObj, bool bReadOrWrite, int changedId );
	
	//函数：getCommandLoadAppInfo
	//功能：获取充电数的命令
	//参数： dataObj引用,  
	//返回值：
	void getCommandLoadAppInfo(stCAN_DevData& dataObj );

	//函数：getCommandDataStatic
	//功能：获取静态数据的命令
	//参数： dataObj引用,  
	//返回值：
	void getCommandStaticData(stCAN_DevData& dataObj);

	//函数：getCommandDynaData
	//功能：获取动态数据的命令
	//参数： dataObj引用,  
	//返回值：
	void getCommandDynaData(stCAN_DevData& dataObj);
	 
	//函数：getCommandAutoDCharge
	//功能：获取自动放电时间的命令
	//参数： dataObj引用,  
	//返回值：
	void getCommandAutoDCharge(stCAN_DevData& dataObj);

	//函数：getCommandBlueLED
	//功能：获取满电LED闪烁的命令
	//参数： dataObj引用,  
	//返回值：
	void getCommandLED(stCAN_DevData& dataObj);

	//函数：getCommandDisC
	//功能：充电数的命令
	//参数： dataObj引用, bReadOrWrite 是否放电 false 读取放电状态 true设置放电; 
	//         当bReadOrWrite为true时： changedId 改变状态的id 停止则开启充电，正在充电则停止。
	//返回值：
	void getCommandDisCharge(stCAN_DevData& dataObj, bool bReadOrWrite, int changedId);



	//函数：analyzeReceiveData
	//功能：分析接收的数据
	//参数：  szData 数据内容， length 数据长度
	//返回值：
	void analyzeReceiveData(BYTE* szData, int Length);

	//函数：getDebugData
	//功能：获取接收到的数据，用于通知窗口显示接收到的数据
	//参数：   
	//返回值：std::string
	std::string getDebugData(){ return m_strDebugData; }

protected:
	//函数：calulataCRC8
	//功能：计算CRC校验值
	//参数： dataObj CAN设备的数据包
	//返回值：uint8_t CRC值
	uint8_t calulataCRC8(stCAN_DevData& dataObj);

	//函数：calulataCRC16
	//功能：计算CRC校验值
	//参数： dataObj CAN设备的数据包
	//返回值：UnionCRC CRC的联合体
	UnionCRC calulataCRC16(stCAN_DevData& dataObj);

	//函数：asc256_enCode
	//功能：加密数据 
	//参数： szKey 是指32字节秘钥， szData 指要加密的数据buf， length 要加密数据的长度，要求16的倍数
	//返回值： 0 成功 ，-1 长度错误
	int asc256_enCode( uint8_t*szKey, uint8_t* szData, int length);

	//函数：asc256_deCode
	//功能：加密解密 
	//参数： szKey 是指32字节秘钥， szData 要解密的数据buf， length 要解密数据的长度 要求16的倍数
	//返回值： 0 成功，-1 长度错误
	int asc256_deCode(uint8_t*szKey, uint8_t* szData, int length);

	//函数：verifyStepOne
	//功能：认证第一步
	//参数： dataObj数据的引用，内含下位机生成的32字节密匙
	//返回值：  
	void verifyStepOne(stCAN_DevData& dataObj);

	//函数：verifyStepTwo
	//功能：认证第二步
	//参数： dataObj数据的引用，判断是否成功
	//返回值：  
	void verifyStepTwo(stCAN_DevData& dataObj);
public:
	//函数：getBatteryArray
	//功能：获取电池队列
	//参数： 
	//返回值：stBatteryInfo* 不可修改
	stBatteryInfo* getBatteryArray() { return m_BatteryArray; }

	//函数：getVerifyFlag
	//功能：获取电池队列
	//参数： 
	//返回值：bool
	bool isVerifyFlag() { return m_bVerify; }

	//函数：m_pPrintfFun
	//功能：打印数据
	//参数： 
	//返回值：
	TYPEcallbackPrintf m_pPrintfFun = NULL;
private:
	aes256_context m_ctx;

	uint8_t m_szKeyDefault[32];
	HWND    m_AppWnd = NULL;  //窗口
	std::string m_strDebugData, str1, str;
	char szTemp[256]; ;
	bool m_bVerify = false; //认证标志
	bool m_bReadChargeState = false;  //读取充电状态标志 0x07
	bool m_bReadOnlineState = false;  //读取电池在位标志 0x09 0x0a
	stBatteryInfo m_BatteryArray[30];
};

//结构体：can 设备的数据包
struct stCAN_DevData   
{
	BYTE Header;  //头，固定是0x55
	BYTE LEN[2];  //整体长度，LSB在前（小端在前）
	BYTE CMD_;    //命令码
	BYTE Enc;     //加密标志：用于表示Data域是否加密，附录加密算法
	BYTE Seq_;    //数据序列号,应答时与发送一致
	BYTE CRC8_;   //帧头校验码，是Header到Seq的crc-8 校验值,附录crc计算C程序注
	BYTE Data_[MAX_BUF_SIZE];//数据(不一定有MAX_BUF_SIZE这么长)
	BYTE CRC16_[2]; //帧校验码，是Header到Seq的crc-16 校验值,附录crc计算C程序注：LSB在前，MSB在后
	stCAN_DevData() :
		Header(0), CMD_(0), Enc(0), Seq_(0), CRC8_(0){
		memset(LEN, 0, 2);
		memset(CRC16_, 0, 2);
		memset(Data_, 0, MAX_BUF_SIZE);
	}
	stCAN_DevData(const stCAN_DevData& other_)
	{
		this->Header = other_.Header;
		this->CMD_ = other_.CMD_;
		this->Enc = other_.Enc;
		
		this->Seq_ = other_.Seq_;
		this->CRC8_ = other_.CRC8_;
		memcpy(this->LEN, other_.LEN, 2);

		memcpy(this->CRC16_, other_.CRC16_, 2);
		memcpy(this->Data_, other_.Data_, MAX_BUF_SIZE);
	}
	unsigned short getLen(){
		unsigned short length_ = LEN[1] << 8 | LEN[0];
		return length_;
	}
	void getHeaderToSeq(BYTE* pBuf){
		pBuf[0] = Header;
		pBuf[1] = LEN[0];
		pBuf[2] = LEN[1];
		pBuf[3] = CMD_;
		pBuf[4] = Enc;
		pBuf[5] = Seq_;
	}
	void packetUp(BYTE * pData, int Len){
		unsigned short dataLen = Len - 7; //数据起步
		for (int i = 0; i < Len; i++){
			if (i == 0){
				Header = pData[i];
				continue;
			}
			if (i == 1){
				LEN[0] = pData[i];
				continue;
			}
			if (i == 2){
				LEN[1] = pData[i];
				continue;
			}
			if (i == 3){
				CMD_ = pData[i];
				continue;
			}
			if (i == 4){
				Enc = pData[i];
				continue;
			}
			if (i == 5){
				Seq_ = pData[i];
				continue;
			}
			if (i == 6){
				CRC8_ = pData[i];
				continue;
			}	
			if (i >= 7 && dataLen > 2 && i < Len - 2) //数据部分
			{
				Data_[i - 7] = pData[i];
				continue;
			}
			if (i >= Len - 2)
			{
				if (i == Len - 2){
					CRC16_[0] = pData[i] ;
					continue;
				}
				if (i == Len - 1){
					CRC16_[1] = pData[i];
					continue;
				}				
			}
		}
	}
	void packetDown(BYTE * pData ){
		unsigned short Len = getLen();
		unsigned short dataLen = Len - 7; //数据起步

		for (int i = 0; i < Len; i++){
			if (i == 0){
				pData[i] = Header;
				continue;
			}
			if (i == 1){
				pData[i] = LEN[0];
				continue;
			}
			if (i == 2){
				pData[i] = LEN[1];
				continue;
			}
			if (i == 3){
				pData[i] = CMD_;
				continue;
			}
			if (i == 4){
				pData[i] = Enc;
				continue;
			}
			if (i == 5){
				pData[i] = Seq_;
				continue;
			}
			if (i == 6){
				pData[i] = CRC8_;
				continue;
			}
			if (i >= 7 && dataLen > 2 && i < Len - 2) //数据部分
			{
				pData[i] = Data_[i - 7];
				continue;
			}
			if (i >= Len - 2)
			{
				if (i == Len - 2){
					pData[i] = CRC16_[0];
					continue;
				}
				if (i == Len - 1){
					pData[i] = CRC16_[1];
					continue;
				}
			}
		}
	}
};

//结构体：DJI充电槽的电池动态数据
struct stDJI_DanyBatteryData{
	uint8_t position_;	//位置
	uint16_t vol_;		//电压mv(充电中才有电压)
	uint16_t temperater;   //温度 0.1°C
	uint8_t capacityPerecent_;//容量百分比
	uint8_t  state_;     //电池状态  0 满电 1 充电中 2放电中 3静默
	uint16_t volDetail_[4]; //每节电压
	uint8_t  chargeTime_;  //预计时间 分钟
	uint8_t  autoDChargeDay;  //自动放电时间
	uint16_t alarmReason;  //报警原因
	stDJI_DanyBatteryData() :position_(0), vol_(0), temperater(0),
		capacityPerecent_(0), state_(0), chargeTime_(0), autoDChargeDay(0), alarmReason(0){
		memset(volDetail_, 0, 4);
	}
	stDJI_DanyBatteryData(const stDJI_DanyBatteryData& other_){
		this->position_ = other_.position_;
		this->vol_ = other_.vol_;
		this->temperater = other_.temperater;
		this->state_ = other_.state_;
		this->capacityPerecent_ = other_.capacityPerecent_;
		memcpy(this->volDetail_, other_.volDetail_, 4);
		this->chargeTime_ = other_.chargeTime_;
		this->autoDChargeDay = other_.autoDChargeDay;
		this->alarmReason = other_.alarmReason;
	}
	
	void packetUp(BYTE * pData, int dataLen){
		for (int i = 0; i < dataLen; i++){
			if (i == 0){ position_ = pData[i];		continue; }
			if (i == 1){ vol_ = Uint8ToUint16(&pData[i]);		continue; }
			if (i == 3){ temperater = Uint8ToUint16(&pData[i]);		continue; }
			if (i == 5){ capacityPerecent_ = pData[i];		continue; }
			if (i == 6){ state_ = pData[i];		continue; }
			if (i == 7){ volDetail_ [0]= Uint8ToUint16(&pData[i]);		continue; }
			if (i == 9){ volDetail_[1] = Uint8ToUint16(&pData[i]);		continue; }
			if (i == 11){ volDetail_[2] = Uint8ToUint16(&pData[i]);		continue; }
			if (i == 13){ volDetail_[3] = Uint8ToUint16(&pData[i]);		continue; }
			if (i == 15){ chargeTime_ = pData[i];		continue; }
			if (i == 16){ autoDChargeDay = pData[i];		continue; }
			if (i == 17){ alarmReason = Uint8ToUint16(&pData[i]);		continue; }
		} 
	}
	char* getBatteryState(){
		//std::string str;
		switch (state_)
		{
		case 0x00: return "满电";	break;
		case 0x01: return "充电中 "; break;
		case 0x02: return "放电中 "; break;
		case 0x03: return "静默   "; break;
		default:
			break;
		}
		return "";
	}
};

//CRC 校验包
union UnionCRC
{
	unsigned short crc16_;
	uint8_t crcArray[2];
};

