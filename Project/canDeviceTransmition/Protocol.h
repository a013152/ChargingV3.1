#pragma once


//CProtocol Э����
//���ڴ���󽮳��۵�Э��
//author��chenlin
//date��20180712

#include "stdafx.h"
#include "Common.h"
extern "C"{
#include "aes256.h"
#include "crc.h"
}

struct stCAN_DevData;
union UnionCRC;

UINT  Uint8ToUint16(uint8_t * canID);



//��� ��Ϣ
struct stBatteryInfo
{
	bool isOline_;
	uint8_t  ChargingMode; //1 �򿪳�磬 2�رճ�磬0xff�Զ���磨����40%��䵽60%)
	float vol_;
	float tempterator;
	float current_;
	SYSTEMTIME   protime_;
	int loop_;  //ѭ������
	int capacity_; //����
	int life_;   //�����ٷֱ�
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

//Э����
class CProtocol
{
	CProtocol();
public:
	//������GetInstance
	//���ܣ���ȡ��ʵ��
	//������
	//����ֵ��CTransmit ��ʵ��
	static CProtocol* GetInstance();

	//��������
	~CProtocol();

	//������setWnd
	//���ܣ����ô��ھ��������֪ͨ������ʾ���յ�������
	//������HWND wnd���ھ��
	//����ֵ��
	void setWnd(HWND wnd){ m_AppWnd = wnd; }

	//������getKey
	//���ܣ���ȡ�ܳ�
	//������  
	//����ֵ��
	uint8_t* getKey(){
		return m_szKeyDefault;
	};

	//������getCommandReadCanId
	//���ܣ���ȡ��CAN id ������
	//������ dataObj���� 
	//����ֵ��
	void getCommandReadCanId(stCAN_DevData& dataObj);

	//������getCommandReadCanId
	//���ܣ���ȡ����CAN id ������
	//������ dataObj����,  canIDҪ���õ�id
	//����ֵ��
	void getCommandWriteCanId(stCAN_DevData& dataObj, BYTE* canID);

	//������getCommandVerify
	//���ܣ���ȡ��֤������
	//������ dataObj����,  
	//����ֵ��
	void getCommandVerify(stCAN_DevData& dataObj);

	//������getCommandBeginMode
	//���ܣ���ȡ��ʼģʽ������
	//������ dataObj����,  
	//����ֵ��
	void getCommandBeginMode(stCAN_DevData& dataObj);

	//������getCommandMaxCharge
	//���ܣ���ȡ�������������
	//������ dataObj����,  
	//����ֵ��
	void getCommandMaxCharge(stCAN_DevData& dataObj);

	//������getCommandCharge
	//���ܣ���������
	//������ dataObj����, bReadOrWrite �Ƿ��� false ��ȡ���״̬ true���ó��; 
	//         ��bReadOrWriteΪtrueʱ�� changedId �ı�״̬��id ֹͣ������磬���ڳ����ֹͣ��
	//����ֵ��
	void getCommandCharge(stCAN_DevData& dataObj, bool bReadOrWrite, int changedId );
	
	//������getCommandLoadAppInfo
	//���ܣ���ȡ�����������
	//������ dataObj����,  
	//����ֵ��
	void getCommandLoadAppInfo(stCAN_DevData& dataObj );

	//������getCommandDataStatic
	//���ܣ���ȡ��̬���ݵ�����
	//������ dataObj����,  
	//����ֵ��
	void getCommandStaticData(stCAN_DevData& dataObj);

	//������getCommandDynaData
	//���ܣ���ȡ��̬���ݵ�����
	//������ dataObj����,  
	//����ֵ��
	void getCommandDynaData(stCAN_DevData& dataObj);
	 
	//������getCommandAutoDCharge
	//���ܣ���ȡ�Զ��ŵ�ʱ�������
	//������ dataObj����,  
	//����ֵ��
	void getCommandAutoDCharge(stCAN_DevData& dataObj);

	//������getCommandBlueLED
	//���ܣ���ȡ����LED��˸������
	//������ dataObj����,  
	//����ֵ��
	void getCommandLED(stCAN_DevData& dataObj);

	//������getCommandDisC
	//���ܣ������������
	//������ dataObj����, bReadOrWrite �Ƿ�ŵ� false ��ȡ�ŵ�״̬ true���÷ŵ�; 
	//         ��bReadOrWriteΪtrueʱ�� changedId �ı�״̬��id ֹͣ������磬���ڳ����ֹͣ��
	//����ֵ��
	void getCommandDisCharge(stCAN_DevData& dataObj, bool bReadOrWrite, int changedId);



	//������analyzeReceiveData
	//���ܣ��������յ�����
	//������  szData �������ݣ� length ���ݳ���
	//����ֵ��
	void analyzeReceiveData(BYTE* szData, int Length);

	//������getDebugData
	//���ܣ���ȡ���յ������ݣ�����֪ͨ������ʾ���յ�������
	//������   
	//����ֵ��std::string
	std::string getDebugData(){ return m_strDebugData; }

protected:
	//������calulataCRC8
	//���ܣ�����CRCУ��ֵ
	//������ dataObj CAN�豸�����ݰ�
	//����ֵ��uint8_t CRCֵ
	uint8_t calulataCRC8(stCAN_DevData& dataObj);

	//������calulataCRC16
	//���ܣ�����CRCУ��ֵ
	//������ dataObj CAN�豸�����ݰ�
	//����ֵ��UnionCRC CRC��������
	UnionCRC calulataCRC16(stCAN_DevData& dataObj);

	//������asc256_enCode
	//���ܣ��������� 
	//������ szKey ��ָ32�ֽ���Կ�� szData ָҪ���ܵ�����buf�� length Ҫ�������ݵĳ��ȣ�Ҫ��16�ı���
	//����ֵ�� 0 �ɹ� ��-1 ���ȴ���
	int asc256_enCode( uint8_t*szKey, uint8_t* szData, int length);

	//������asc256_deCode
	//���ܣ����ܽ��� 
	//������ szKey ��ָ32�ֽ���Կ�� szData Ҫ���ܵ�����buf�� length Ҫ�������ݵĳ��� Ҫ��16�ı���
	//����ֵ�� 0 �ɹ���-1 ���ȴ���
	int asc256_deCode(uint8_t*szKey, uint8_t* szData, int length);

	//������verifyStepOne
	//���ܣ���֤��һ��
	//������ dataObj���ݵ����ã��ں���λ�����ɵ�32�ֽ��ܳ�
	//����ֵ��  
	void verifyStepOne(stCAN_DevData& dataObj);

	//������verifyStepTwo
	//���ܣ���֤�ڶ���
	//������ dataObj���ݵ����ã��ж��Ƿ�ɹ�
	//����ֵ��  
	void verifyStepTwo(stCAN_DevData& dataObj);
public:
	//������getBatteryArray
	//���ܣ���ȡ��ض���
	//������ 
	//����ֵ��stBatteryInfo* �����޸�
	stBatteryInfo* getBatteryArray() { return m_BatteryArray; }

	//������getVerifyFlag
	//���ܣ���ȡ��ض���
	//������ 
	//����ֵ��bool
	bool isVerifyFlag() { return m_bVerify; }

	//������m_pPrintfFun
	//���ܣ���ӡ����
	//������ 
	//����ֵ��
	TYPEcallbackPrintf m_pPrintfFun = NULL;
private:
	aes256_context m_ctx;

	uint8_t m_szKeyDefault[32];
	HWND    m_AppWnd = NULL;  //����
	std::string m_strDebugData, str1, str;
	char szTemp[256]; ;
	bool m_bVerify = false; //��֤��־
	bool m_bReadChargeState = false;  //��ȡ���״̬��־ 0x07
	bool m_bReadOnlineState = false;  //��ȡ�����λ��־ 0x09 0x0a
	stBatteryInfo m_BatteryArray[30];
};

//�ṹ�壺can �豸�����ݰ�
struct stCAN_DevData   
{
	BYTE Header;  //ͷ���̶���0x55
	BYTE LEN[2];  //���峤�ȣ�LSB��ǰ��С����ǰ��
	BYTE CMD_;    //������
	BYTE Enc;     //���ܱ�־�����ڱ�ʾData���Ƿ���ܣ���¼�����㷨
	BYTE Seq_;    //�������к�,Ӧ��ʱ�뷢��һ��
	BYTE CRC8_;   //֡ͷУ���룬��Header��Seq��crc-8 У��ֵ,��¼crc����C����ע
	BYTE Data_[MAX_BUF_SIZE];//����(��һ����MAX_BUF_SIZE��ô��)
	BYTE CRC16_[2]; //֡У���룬��Header��Seq��crc-16 У��ֵ,��¼crc����C����ע��LSB��ǰ��MSB�ں�
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
		unsigned short dataLen = Len - 7; //������
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
			if (i >= 7 && dataLen > 2 && i < Len - 2) //���ݲ���
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
		unsigned short dataLen = Len - 7; //������

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
			if (i >= 7 && dataLen > 2 && i < Len - 2) //���ݲ���
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

//�ṹ�壺DJI���۵ĵ�ض�̬����
struct stDJI_DanyBatteryData{
	uint8_t position_;	//λ��
	uint16_t vol_;		//��ѹmv(����в��е�ѹ)
	uint16_t temperater;   //�¶� 0.1��C
	uint8_t capacityPerecent_;//�����ٷֱ�
	uint8_t  state_;     //���״̬  0 ���� 1 ����� 2�ŵ��� 3��Ĭ
	uint16_t volDetail_[4]; //ÿ�ڵ�ѹ
	uint8_t  chargeTime_;  //Ԥ��ʱ�� ����
	uint8_t  autoDChargeDay;  //�Զ��ŵ�ʱ��
	uint16_t alarmReason;  //����ԭ��
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
		case 0x00: return "����";	break;
		case 0x01: return "����� "; break;
		case 0x02: return "�ŵ��� "; break;
		case 0x03: return "��Ĭ   "; break;
		default:
			break;
		}
		return "";
	}
};

//CRC У���
union UnionCRC
{
	unsigned short crc16_;
	uint8_t crcArray[2];
};

