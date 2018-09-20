#include "stdafx.h"
#include "Protocol.h"
#include "Transmit.h"
#define GET_T CTransmit::GetInstance()

BYTE g_CAN_ID_Default[2] = { 0x01, 0x05 };//��ʱ��CAN ID д�����ȡ;  //��ǰ��CAN ID  0x111
BYTE g_CAN_ID_Common[2] = { 0xff, 0x07 };//ͨ�õ�CAN ID ����DJI ���۵�can idʱʹ��

BYTE g_DefaultKey = 0xff; //Ĭ���ܳ� 0xff
char g_AppPath[256] = { 0 };

CProtocol * pThis = nullptr;
UINT  Uint8ToUint16(uint8_t * canID)
{
	UINT canId = canID[1] << 8 | canID[0];
	return canId;
}

CProtocol::CProtocol()
{
	for (int i = 0; i <32; i++)
		m_szKeyDefault[i] = g_DefaultKey; //Ĭ���ܳ�
	memset(szTemp, 0, 256);
}


CProtocol* CProtocol::GetInstance()
{
	if (nullptr == pThis){
		pThis = new CProtocol();
	}
	return pThis;
}

CProtocol::~CProtocol()
{
}

void CProtocol::getCommandReadCanId(stCAN_DevData& dataObj)
{ 
	dataObj.Header = 0x55;
	dataObj.LEN[0] = 9;
	dataObj.CMD_ = 0x01;  //��can id
	dataObj.Enc = 0  ;  //������
	dataObj.Seq_ = 1;
	dataObj.CRC8_ = calulataCRC8(dataObj);

	UnionCRC unionObj = calulataCRC16(dataObj);
	dataObj.CRC16_[0] = unionObj.crcArray[0];
	dataObj.CRC16_[1] = unionObj.crcArray[1];
}

void CProtocol::getCommandWriteCanId(stCAN_DevData& dataObj, BYTE* canID)
{
	dataObj.Header = 0x55;
	dataObj.LEN[0] = 9 + 16;
	dataObj.CMD_ = 0;   //����can id
	dataObj.Enc = 1;	//����
	dataObj.Seq_ = 0;
	dataObj.CRC8_ = calulataCRC8(dataObj);
	
	BYTE byteCANID[16] = { 0 };
	byteCANID[0] = canID[0];
	byteCANID[1] = canID[1]; 
	
	aes256_context ctx;
	uint8_t key[32];
	for (int i = 0; i < sizeof(key); i++) key[i] = g_DefaultKey; //Ĭ���ܳ�
	aes256_init(&ctx, key);
	aes256_encrypt_ecb(&ctx, byteCANID);

	BYTE byteCANID_1[16] = { 0 };
	memcpy(byteCANID_1, byteCANID, 16);
	aes256_decrypt_ecb(&ctx, byteCANID_1);

	memcpy(dataObj.Data_, byteCANID, 16);

	UnionCRC unionObj = calulataCRC16(dataObj);
	dataObj.CRC16_[0] = unionObj.crcArray[0];
	dataObj.CRC16_[1] = unionObj.crcArray[1];
}

void CProtocol::getCommandVerify(stCAN_DevData& dataObj)
{
	dataObj.Header = 0x55;
	dataObj.LEN[0] = 7 + 1 + 2 ;
	dataObj.CMD_ = 4;
	dataObj.Enc = 0;
	dataObj.Seq_ = 4;
	dataObj.CRC8_ = calulataCRC8(dataObj);

	UnionCRC unionObj = calulataCRC16(dataObj);
	dataObj.CRC16_[0] = unionObj.crcArray[0];
	dataObj.CRC16_[1] = unionObj.crcArray[1];

}

void CProtocol::getCommandBeginMode(stCAN_DevData& dataObj)
{
	dataObj.Header = 0x55;
	dataObj.LEN[0] = 7 + 1 + 2;
	dataObj.LEN[1] = 0x00;
	dataObj.CMD_ = 0x05;
	dataObj.Enc = 0;
	dataObj.Seq_ = 5;
	dataObj.CRC8_ = calulataCRC8(dataObj);

	dataObj.Data_[0] = 0x00;
	//dataObj.Data_[1] = 0x01;

	UnionCRC unionObj = calulataCRC16(dataObj);
	dataObj.CRC16_[0] = unionObj.crcArray[0];
	dataObj.CRC16_[1] = unionObj.crcArray[1];
	//dataObj.Data_[0] = 0x00; // 0x0 ��ȡ�� 0x01 ����
	////dataObj.Data_[1] = 0x01; //0x01 50%�����ʼģʽ��0xff 100%�����ʼģʽ
}

void CProtocol::getCommandMaxCharge(stCAN_DevData& dataObj)
{
	dataObj.Header = 0x55;
	dataObj.LEN[0] = 7 + 1 + 2;
	dataObj.CMD_ = 6;
	dataObj.Enc = 0;
	dataObj.Seq_ = 6;
	dataObj.CRC8_ = calulataCRC8(dataObj);

	dataObj.Data_[0] = 0x00;
	//dataObj.Data_[1] = 15;

	UnionCRC unionObj = calulataCRC16(dataObj);
	dataObj.CRC16_[0] = unionObj.crcArray[0];
	dataObj.CRC16_[1] = unionObj.crcArray[1];
	//dataObj.Data_[0] = 0x00; // 0x0 ��ȡ�� 0x01 ����
	//dataObj.Data_[1] = 0x0f; // 1~15 ��������
}

void CProtocol::getCommandCharge(stCAN_DevData& dataObj, bool bReadOrWrite, int changedId)
{
	dataObj.Header = 0x55;
	if (false == bReadOrWrite)
		dataObj.LEN[0] = 7 + 1 + 2;
	else
		dataObj.LEN[0] = 7 + 16  + 2;

	dataObj.CMD_ = 7;
	dataObj.Enc = 0;
	dataObj.Seq_ = 7;
	dataObj.CRC8_ = calulataCRC8(dataObj);

	
	if (false == bReadOrWrite)
		// 0x0 ��ȡ�� 
		dataObj.Data_[0] = 0x00; 
	else
	{
		// 0x01 ����
		//���޸�: ��Ҫ����UI�����ֵ�������
		if (changedId >= 0 && changedId <= 15)
		{
			dataObj.Data_[0] = 0x01; // 0x01 ����
			for (int i = 0; i < 15; i++){
				if (m_BatteryArray[i].isOline_){
					if (changedId - 1 == i){
						uint8_t chargingMode = m_BatteryArray[i].ChargingMode != 1 ? 1 : 2;
						dataObj.Data_[i + 1] = chargingMode;//(m_BatteryArray[i].ChargingMode != 1 ? 1 : 2);//1 ��ʼ�䣬2ֹͣ
						m_BatteryArray[i].ChargingMode = chargingMode;
					}
					else{
						dataObj.Data_[i + 1] = m_BatteryArray[i].ChargingMode;// ���� ;
					}
				}
				else{
					dataObj.Data_[i + 1] = 0xff;
				}
			}
		}
	}

	UnionCRC unionObj = calulataCRC16(dataObj);
	dataObj.CRC16_[0] = unionObj.crcArray[0];
	dataObj.CRC16_[1] = unionObj.crcArray[1];
}

void CProtocol::getCommandLoadAppInfo(stCAN_DevData& dataObj)
{
	dataObj.Header = 0x55;
	dataObj.LEN[0] = 7 + 2 ;
	dataObj.CMD_ = 8;
	dataObj.Enc = 0;
	dataObj.Seq_ = 8;
	dataObj.CRC8_ = calulataCRC8(dataObj);

	UnionCRC unionObj = calulataCRC16(dataObj);
	dataObj.CRC16_[0] = unionObj.crcArray[0];
	dataObj.CRC16_[1] = unionObj.crcArray[1];

}

void CProtocol::getCommandStaticData(stCAN_DevData& dataObj)
{
	dataObj.Header = 0x55;
	dataObj.LEN[0] = 7 + 1 + 2;
	dataObj.CMD_ = 9;
	dataObj.Enc = 0;
	dataObj.Seq_ = 9;
	dataObj.CRC8_ = calulataCRC8(dataObj);

	dataObj.Data_[0] = 0x01;

	UnionCRC unionObj = calulataCRC16(dataObj);
	dataObj.CRC16_[0] = unionObj.crcArray[0];
	dataObj.CRC16_[1] = unionObj.crcArray[1];
}

void CProtocol::getCommandDynaData(stCAN_DevData& dataObj)
{
	dataObj.Header = 0x55;
	dataObj.LEN[0] = 7 + 1 + 2;
	dataObj.CMD_ = 0x0a;
	dataObj.Enc = 0;
	dataObj.Seq_ = 0x0a;
	dataObj.CRC8_ = calulataCRC8(dataObj);
	dataObj.Data_[0] = 0x01;  //����һ��
	UnionCRC unionObj = calulataCRC16(dataObj);
	dataObj.CRC16_[0] = unionObj.crcArray[0];
	dataObj.CRC16_[1] = unionObj.crcArray[1];
	
}

void CProtocol::getCommandAutoDCharge(stCAN_DevData& dataObj)
{
	//dataObj.Header = 0x55;
	//dataObj.LEN[0] = 8 + 17;
	//dataObj.CMD_ = 0x0b;
	//dataObj.Enc = 0;
	//dataObj.Seq_ = 0x0b;
	//UnionCRC unionObj = calulataCRC(dataObj);
	//dataObj.CRC16_[0] = unionObj.crcArray[0];
	//dataObj.CRC16_[1] = unionObj.crcArray[1];
	//dataObj.Data_[0] = 0xff;
	//dataObj.Data_[1] = 0x7f;
	//for (INT i = 2; i < 17; i++)dataObj.Data_[i] = 9;//9���Զ��ŵ�
}

void CProtocol::getCommandLED(stCAN_DevData& dataObj)
{
	//dataObj.Header = 0x55;
	//dataObj.LEN[0] = 8 + 2;
	//dataObj.CMD_ = 0x0c;
	//dataObj.Enc = 0;
	//dataObj.Seq_ = 0x0c;
	//UnionCRC unionObj = calulataCRC(dataObj);
	//dataObj.CRC16_[0] = unionObj.crcArray[0];
	//dataObj.CRC16_[1] = unionObj.crcArray[1];
	//dataObj.Data_[0] = 0x7f;  //�Ƶ����
	//dataObj.Data_[1] = 0x01;  //1 ���ƣ� 2ȡ������
}

void CProtocol::getCommandDisCharge(stCAN_DevData& dataObj, bool bReadOrWrite, int changedId)
{
	dataObj.Header = 0x55;
	if (false == bReadOrWrite)
		dataObj.LEN[0] = 7 + 1 + 2;
	else
		dataObj.LEN[0] = 7 + 16 + 2;

	dataObj.CMD_ = 0x0d;
	dataObj.Enc = 0;
	dataObj.Seq_ = 0x0c;
	dataObj.CRC8_ = calulataCRC8(dataObj);

	if (false == bReadOrWrite)
	{
		//��ȡ
		dataObj.Data_[0] = 0x00;
	}
	else
	{
		//����
		// ���޸ģ��ŵ���Ҫ����UI�����ֵ 
		dataObj.Data_[0] = 0x01;  //
		for (int i = 1; i < 16; i++)
		{
			if (changedId == i-1)
				dataObj.Data_[i] = 0x01;  //1 �򿪷ŵ磬 0 �رշŵ�
			else
				dataObj.Data_[i] = 0x00;
		}
	}


	UnionCRC unionObj = calulataCRC16(dataObj);
	dataObj.CRC16_[0] = unionObj.crcArray[0];
	dataObj.CRC16_[1] = unionObj.crcArray[1];

}

void CProtocol::analyzeReceiveData(BYTE* szData, int Length)
{
	if (Length == 0)
		return;
	//ƴװ����
	int i = 0, Len = 0;stCAN_DevData dataObj;
	dataObj.packetUp(szData, Length);
	uint16_t datalen = dataObj.getLen();
	if (dataObj.Data_[0] != 0x00 && dataObj.CMD_ != 0x00 && dataObj.CMD_ != 0x01){
		if (dataObj.Data_[0] == 0x01){ m_strDebugData = "����һ����������ԡ�"; }
		if (dataObj.Data_[0] == 0x02){ m_strDebugData = "����Flash�����������ԡ�"; }
		if (dataObj.Data_[0] == 0x03){ m_strDebugData = "����Flashдʧ�ܣ������ԡ�"; }
		if (dataObj.Data_[0] == 0x04){ m_strDebugData = "����Flash �������������ԡ�"; }
		if (dataObj.Data_[0] == 0x05){ m_strDebugData = "��������У����������ԡ�"; }
		if (dataObj.Data_[0] == 0x06){ m_strDebugData = "�����̼�������������ԡ�"; }
		if (dataObj.Data_[0] == 0xD0){ m_strDebugData = "δ��֤��������֤���ٲ�����"; }
		if (dataObj.Data_[0] == 0xD1){ m_strDebugData = "��֤ʧ�ܡ�"; }
		if (dataObj.Data_[0] == 0xD2){ m_strDebugData = "��ز���λ������ʧ�ܡ�"; }
		if (dataObj.Data_[0] == 0xD3){ m_strDebugData = "������쳣�������쳣�鿴�ܶ�̬���ݡ�"; }
		if (dataObj.Data_[0] == 0xD4){ m_strDebugData = "����Ԫ�¶ȹ��ߣ�����ʧ�ܡ�"; }
		if (dataObj.Data_[0] == 0xD5){ m_strDebugData = "�������ͬʱ������ø���"; }
		if (dataObj.Data_[0] == 0xD6){ m_strDebugData = "����ѳ���"; }
		if (dataObj.Data_[0] == 0xD7){ m_strDebugData = "д������Ƿ�"; }
		if (dataObj.Data_[0] == 0xD8){ m_strDebugData = "Flash дʧ��"; }
		if (dataObj.Data_[0] == 0xD9){ m_strDebugData = "δ���磬��������ָʾ�ơ�"; }
		if (m_pPrintfFun) m_pPrintfFun(1, true);
		return;
	}
	
	if (dataObj.CMD_ == 0x00){
		//����can id
		Len = dataObj.getLen() - 7;
		if (dataObj.Data_[0] == 0x00){
			m_strDebugData = "����Can ID�ɹ�\n";
		}
		else{
			sprintf_s(szTemp, 256, "����Can IDʧ�ܣ������룺%02X", dataObj.Data_[0]);
			m_strDebugData = szTemp;
		}
		if (m_pPrintfFun) m_pPrintfFun(1, true);
	}
	if (dataObj.CMD_ == 0x01){
		//��ȡcan id
		if (dataObj.Enc = 0x01){
			//����
			Len = dataObj.getLen() - 7 - 2;
			asc256_deCode(getKey(), dataObj.Data_, Len);
			str = "";
			for (i = 0; i < Len; i++)	//������Ϣ
			{
				sprintf_s(szTemp, 256, "%02X ", dataObj.Data_[i]); 
				str += szTemp;
			}
			m_strDebugData = "��ȡ��CAN ID:" + str;
			if (m_pPrintfFun) m_pPrintfFun(1, true);
		}
	}
	
 	if (dataObj.CMD_ == 0x04){
		//��֤���� 0 ��ȡ�����Կ�� 1��ȡ��֤�����
		if (dataObj.Data_[0] == 0x00){
			if (0x00 == dataObj.Data_[1]){
				verifyStepOne(dataObj);
			}
			else if (0x01 == dataObj.Data_[1]){
				verifyStepTwo(dataObj);
			}
		}
			
	}
	if (dataObj.CMD_ == 0x05){
		//��ʼģʽ 0 ��ȡ���� ��1 ���÷���
		if (dataObj.Data_[0] == 0x00){
			if (0x00 == dataObj.Data_[1]){
				sprintf_s(szTemp, 256, "��ȡ����ʼģʽ:%s", (dataObj.Data_[2] == 0x01 ? "50%�洢ģʽ" : "100%�洢ģʽ"));
				m_strDebugData = szTemp;

				if (m_pPrintfFun){
					m_pPrintfFun(1, true);
				}
			}
			else if (0x01 == dataObj.Data_[1]){
				if (dataObj.Data_[0] == 0)
					m_strDebugData = "������ʼģʽ�ɹ�\n";
				else
					m_strDebugData = "������ʼģʽʧ��\n";
				if (m_pPrintfFun){	m_pPrintfFun(1, true);	}
			}
		}
	}
	if (dataObj.CMD_ == 0x06){
		//��������� 0 ��ȡ���� ��1 ���÷���
		if (dataObj.Data_[0] == 0x00){
			if (0x00 == dataObj.Data_[1]){
				sprintf_s(szTemp, 256, "��ȡ�����������:%d", dataObj.Data_[2]);
				m_strDebugData = szTemp;
				if (m_pPrintfFun){ m_pPrintfFun(1, true); }
				 
			}
			else if (0x01 == dataObj.Data_[1]){				
					m_strDebugData = "���������������ɡ�";	
				if (m_pPrintfFun){ m_pPrintfFun(1, true); }
			}
		}
	}	
	if (dataObj.CMD_ == 0x07){
		//��� dataObj.Data_[1]  == 0 ��ȡ���� ��1 ���÷���
		if (0x00 == dataObj.Data_[1] && 0x00 == dataObj.Data_[0]){
			m_bReadChargeState = true;
			m_strDebugData = "��ȡ���״̬��\n";
			for (int i = 2; i < 17; i++){
				m_BatteryArray[i-2].ChargingMode = dataObj.Data_[i];
				
				if(dataObj.Data_[i]==0x01)
					sprintf_s(szTemp, 256, " %02d:%02X ��\n",i - 1, dataObj.Data_[i]);
				else if(dataObj.Data_[i] == 0x02)
					sprintf_s(szTemp, 256, " %02d:%02X �ر�\n", i - 1, dataObj.Data_[i]);
				else if (dataObj.Data_[i] == 0xff)
					sprintf_s(szTemp, 256, " %02d:%02X �Զ�\n", i - 1, dataObj.Data_[i]);
				m_strDebugData += szTemp;
				 
			}
			if (m_pPrintfFun){ m_pPrintfFun(1, true); }
		}
		if (0x01 == dataObj.Data_[1] ){
			sprintf_s(szTemp, 256, "���ó�������������:%02X", dataObj.Data_[0]);
			m_strDebugData = szTemp;
			if (m_pPrintfFun){ m_pPrintfFun(1, true); }
		}
	}
	if (dataObj.CMD_ == 0x08){
		//��ȡ Loader�汾
		if (0x00 == dataObj.Data_[0]){
			sprintf_s(szTemp, 256, "Loader�汾%d.%d.%d.%d%d%d\n", Uint8ToUint16(&dataObj.Data_[1]),
				Uint8ToUint16(&dataObj.Data_[3]), Uint8ToUint16(&dataObj.Data_[5]), Uint8ToUint16(&dataObj.Data_[7]),
				dataObj.Data_[9], dataObj.Data_[10]); str = szTemp;
			
			sprintf_s(szTemp, 256, "App�汾%d.%d.%d.%d%d%d\n", Uint8ToUint16(&dataObj.Data_[11]),
				Uint8ToUint16(&dataObj.Data_[13]), Uint8ToUint16(&dataObj.Data_[15]), Uint8ToUint16(&dataObj.Data_[17]),
				dataObj.Data_[19], dataObj.Data_[20]); str1 = szTemp;
			m_strDebugData = str1; m_strDebugData += str;
			if(dataObj.Data_[21] == 0x00)
				m_strDebugData += "P4ϵ�е��\n"; m_strDebugData += "��Ʒ���:";
			for (int i = 22; i < 36;i++)
			{
				sprintf_s(szTemp, 256, "%c", dataObj.Data_[i]);
				str1 = szTemp;;
				m_strDebugData += str1;
			}
			if (m_pPrintfFun){ m_pPrintfFun(1, true); }
			//if (m_AppWnd){ ::PostMessage(m_AppWnd, WM_MSG_DISPLAY_DATA, 0, 1); }
		}
	}
	if (dataObj.CMD_ == 0x09){
		//��ȡ ��̬����
		if (0x00 == dataObj.Data_[0]){
			//����������
			uint16_t batteryOnline = Uint8ToUint16(&dataObj.Data_[2]);
			uint16_t temp1 = 0x01;
			for (int i = 0; i < 15; i++)  
			{
				m_BatteryArray[i].isOline_ = (((temp1 << i) & batteryOnline)>>i == 0x01);
			}
			//���������
			//�����


			if (m_pPrintfFun){ m_pPrintfFun(1, true); }
		}
	}
	if (dataObj.CMD_ == 0x0a){
		//��ض�̬����
		if (0x00 == dataObj.Data_[0]){
			m_strDebugData = "";
			//��ع�Ԫ������Ϣ
			uint16_t temp1 = 0x01, temp3 = 0, temp4 = 0, temp5 = 0; uint8_t temp2[4] = { 0 };
			for (int i = 0; i < 4; i++)	{
				temp2[i] = ((temp1 << i)&dataObj.Data_[2])>>i	;
			}
			if (temp2[0])
				m_strDebugData = "���������ϡ�";
			if (temp2[1])
				m_strDebugData = "��ع�Ԫ�¶ȹ��ߡ�";
			if (temp2[2])
				m_strDebugData = "��ع�Ԫ�¶ȹ��͡�";
			if (temp2[3])
				m_strDebugData = "��ع�Ԫ�����쳣��";
			//if (m_AppWnd && dataObj.Data_[2] != 0x00) { ::PostMessage(m_AppWnd, WM_MSG_DISPLAY_DATA, 0, 1); }
			
			//��ȡ��صĶ�̬���ݣ�һ�����һ�����ݣ�һ�����ݳ�19bytes����λ����ţ���ѹ���¶�
			int  dataDynaLength = dataObj.getLen() - 7 - 5 -2;
			if (dataDynaLength >0 && dataDynaLength % 19 ==0){
				int loop = dataDynaLength / 19;
				for (int i = 0; i < loop; i++){
					static float nPreVol;
					uint8_t szTempData[20] = { 0 };
					memcpy(szTempData, &dataObj.Data_[5 + i*19] , 19);
					stDJI_DanyBatteryData stObj;
					stObj.packetUp(szTempData, 19);
					//�Ľڵ�صĵ�ѹ
					int volTemp = stObj.volDetail_[0];		volTemp += stObj.volDetail_[1];	
					volTemp += stObj.volDetail_[2];			volTemp += stObj.volDetail_[3];
					nPreVol = m_BatteryArray[stObj.position_ - 1].vol_;
					m_BatteryArray[stObj.position_ - 1].vol_ = volTemp / 4000.0f;
					m_BatteryArray[stObj.position_ - 1].tempterator = stObj.temperater / 10.0f;
					//����0.2V����ui
				//	if(fabs(nPreVol - m_BatteryArray[stObj.position_ - 1].vol_) > 0.2 && m_AppWnd)
					//	::PostMessage(m_AppWnd, WM_MSG_REFRESH_BATTERY, stObj.position_ - 1, 0);
					sprintf_s(szTemp, 256, "��غţ�%d , ״̬��%s,��ѹ:%3.1fV, �¶ȣ�%3.1f��\n",\
						stObj.position_,  stObj.getBatteryState(), volTemp / 4000.0f, stObj.temperater / 10.0f);
					m_strDebugData += szTemp;
				}
			} 

			//����������
			uint16_t batteryOnline = Uint8ToUint16(&dataObj.Data_[3]);
			temp1 = 0x01;
			for (int i = 0; i < 15; i++)
			{
				static bool isPreOnline = false;
				isPreOnline = m_BatteryArray[i].isOline_;
				temp3 = (temp1 << i);
				temp4 = (temp3 & batteryOnline);
				m_BatteryArray[i].isOline_ = (temp4 >> i == 0x01);
				if (false == m_BatteryArray[i].isOline_){
					//��ز����ߣ��¶ȵ�ѹ����
					m_BatteryArray[i].vol_ = 0;
					m_BatteryArray[i].tempterator = 0;
				}
			}
			if (m_pPrintfFun)m_pPrintfFun(1, true);
		}
	}
	if (dataObj.CMD_ == 0x0d){
		if (0x00 == dataObj.Data_[1] && 0x00 == dataObj.Data_[0]){
			m_bReadChargeState = true;
			m_strDebugData = "��ȡ�ŵ�״̬��\n";
			for (int i = 2; i < 17; i++){ 
				if (dataObj.Data_[i] == 0x00)
					sprintf_s(szTemp, 256, " %02d:%02X �ر�\n", i - 1, dataObj.Data_[i]);
				else if (dataObj.Data_[i] == 0x01)
					sprintf_s(szTemp, 256, " %02d:%02X ��\n", i - 1, dataObj.Data_[i]);
				m_strDebugData += szTemp;
			}
		}
		if (0x01 == dataObj.Data_[1]){
			sprintf_s(szTemp, 256, "���÷ŵ������������:%02X", dataObj.Data_[0]);
			m_strDebugData = szTemp;
		}
		if (m_pPrintfFun)m_pPrintfFun(1, true);
	}
}
 
uint8_t CProtocol::calulataCRC8(stCAN_DevData& dataObj)
{
	BYTE szHeaderToSeq[8] = { 0 };
	dataObj.getHeaderToSeq(szHeaderToSeq);
	BYTE crc8_ = crc8(szHeaderToSeq, 6);
	return crc8_;
}
 

UnionCRC CProtocol::calulataCRC16(stCAN_DevData& dataObj)
{
	uint16_t len = dataObj.getLen() - 2;  //crc16�Ƕ�����֡����crc16�������ֽڣ�ǰ�������ֽڵļ���
	BYTE* szTemp = new BYTE[len +1] ;
	memset(szTemp, 0, len + 1);
	memcpy(szTemp, &dataObj, len);
	UnionCRC unionObj; unionObj.crc16_ = crc16(szTemp, len);
	delete[]szTemp;
	return unionObj;
}

int CProtocol::asc256_enCode(uint8_t*szKey, uint8_t* szData, int length)
{
	if (0 != length % 16)
		return -1;
	aes256_init(&m_ctx, szKey);
	for (int i = 0; i < length; i += 16){
		aes256_encrypt_ecb(&m_ctx, &szData[i]);
	}
	return 0;
}

int CProtocol::asc256_deCode(uint8_t*szKey, uint8_t* szData, int length)
{
	if (0 != length % 16)
		return -1;
	aes256_init(&m_ctx, szKey);
	for (int i = 0; i < length; i += 16){
		aes256_decrypt_ecb(&m_ctx, &szData[i]);
	}
	return 0;
}

void CProtocol::verifyStepOne(stCAN_DevData& dataObj)
{
	BYTE byteA[42] = { 0 }, keyA[32] = { 0 }, bytePlaintText[128] = { 0 }, byteEncodeText[128] = { 0 }, byteVerify[272] = { 0 };
	
	//��ȡ �ܳ�A	
	memcpy(keyA, &(dataObj.Data_[2]), 32);
	str = "";
	for (INT i = 0; i < 32; i++)	//������Ϣ
	{
		sprintf_s(szTemp,256, "%02X ", keyA[i]);
		str += szTemp;
	}
	 
	//�����������
	for (int i = 0; i < 128; i++){
		bytePlaintText[i] = i;
		byteEncodeText[i] = i;
	}

	//��������
	asc256_enCode( keyA, byteEncodeText, 128); 

	//ƴװ���ݰ�
	dataObj.Header = 0x55;
	uint16_t len_ = 7 + 272 + 2;
	dataObj.LEN[0] = len_ & 0xff;
	dataObj.LEN[1] = len_ >> 8;
	dataObj.CMD_ = 4;
	dataObj.Enc = 1;
	dataObj.Seq_ = 4;
	dataObj.CRC8_ = calulataCRC8(dataObj);  
	 
	memset(dataObj.Data_, 0, MAX_BUF_SIZE);
	byteVerify[0] = 0x01;
	memcpy(&byteVerify[1], bytePlaintText, 128);
	memcpy(&byteVerify[129], byteEncodeText, 128);//ƴװ256�ֽ���֤����
	
	str = "";
	for (INT i = 0; i < 272; i++)	//������Ϣ
	{
		//str1.Format("%02X ", byteVerify[i]);
		sprintf_s(szTemp, 256, "%02X ", byteVerify[i]);
		str += szTemp;
	}
	//�������ݶ�
	uint8_t key[32];
	for (int i = 0; i < sizeof(key); i++) key[i] = g_DefaultKey; //Ĭ���ܳ�
	asc256_enCode( key, byteVerify, 272); 
	 
	memcpy(dataObj.Data_, byteVerify, 272);

	UnionCRC unionObj = calulataCRC16(dataObj);
	dataObj.CRC16_[0] = unionObj.crcArray[0];
	dataObj.CRC16_[1] = unionObj.crcArray[1];

	str = "";
	for (INT i = 0; i < 272; i++)	//������Ϣ
	{
		//str1.Format("%02X ", byteVerify[i]);
		sprintf_s(szTemp, 256, "%02X ", byteVerify[i]);
		str += szTemp;
	}
	UINT canID = Uint8ToUint16(g_CAN_ID_Default);
	canID |= 0x400;   //v1.3can id��Ҫ����0x400
	GET_T->sendCanData(dataObj, canID);//ʹ��Ĭ�� can id

}

void CProtocol::verifyStepTwo(stCAN_DevData& dataObj)
{
	if (dataObj.Data_[0] == 0){
		m_strDebugData = "��֤�ɹ���\n";
		m_bVerify = true;
	}
	else{
		sprintf_s(szTemp, 256, "��֤ʧ�ܡ�������:%02X\n ", dataObj.Data_[0]);
		m_strDebugData = szTemp;
	}
	if (m_pPrintfFun)m_pPrintfFun(1, true);
}
