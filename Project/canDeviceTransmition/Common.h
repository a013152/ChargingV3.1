#pragma once

#include "stdafx.h" 
#include <stdint.h> 
#include <string>
#include <windows.h>
#define MAX_BUF_SIZE 1024 //������

#define TIMER_WATCHUI  1
#define APP_TITLE "���ܳ���2.1"
extern BYTE g_CAN_ID_Default[2];  //��ǰ��CAN ID 
extern BYTE g_CAN_ID_Common[2];
extern BYTE g_DefaultKey; //Ĭ���ܳ� 0xff
extern char g_AppPath[256];

typedef void(*TYPEcallbackPrintf)(int nType, bool bDisplayOption);
 
