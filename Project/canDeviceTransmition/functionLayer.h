#pragma once
/*******************
//description�������㣬�������ϡ�
//
//author�� chenlin
//date��20180923
*/

#include "Common.h"
#include "../common/common.h"
#include "Protocol.h"
#include "Transmit.h" 
#define GET_T CTransmit::GetInstance()
#define GET_P CProtocol::GetInstance()
#include <string>
#include <vector>

#define VT_STR std::vector<std::string>&


//��CAN�豸
void openCAN(char * resultString);

//�ر�CAN�豸
void closeCAN(char * resultString);

//��дCAN ID
bool readOrWriteCANID(VT_STR vtStrCommand, char* resultString);

//��֤�豸
bool verifyDevice(VT_STR vtStrCommand, char* resultString);

//��֤�豸
bool readOrWriteBeginMode(VT_STR vtStrCommand, char* resultString);

