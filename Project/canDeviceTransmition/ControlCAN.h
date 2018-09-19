#ifndef CONTROLCAN_H
#define CONTROLCAN_H

////�ļ��汾��v2.00 20150920
//#include <cvidef.h>	//ʹ��CVIƽ̨��������ʹ�ø���䡣

//�ӿڿ����Ͷ���

#define VCI_USBCAN1		3
#define VCI_USBCAN2		4
#define VCI_USBCAN2A		4

#define VCI_USBCAN_E_U 		20
#define VCI_USBCAN_2E_U 	21



//�������÷���״ֵ̬
#define	STATUS_OK					1
#define STATUS_ERR					0
	
/*------------------------------------------------����ZLG�ĺ�������������------------------------------------------------*/
//1.ZLGCANϵ�нӿڿ���Ϣ���������͡�
typedef  struct  _VCI_BOARD_INFO{
		USHORT	hw_Version;
		USHORT	fw_Version;
		USHORT	dr_Version;
		USHORT	in_Version;
		USHORT	irq_Num;
		BYTE	can_Num;
		CHAR	str_Serial_Num[20];
		CHAR	str_hw_Type[40];
		USHORT	Reserved[4];
} VCI_BOARD_INFO,*PVCI_BOARD_INFO; 

//2.����CAN��Ϣ֡���������͡�
typedef  struct  _VCI_CAN_OBJ{
	UINT	ID;
	UINT	TimeStamp;
	BYTE	TimeFlag;
	BYTE	SendType;
	BYTE	RemoteFlag;//�Ƿ���Զ��֡
	BYTE	ExternFlag;//�Ƿ�����չ֡
	BYTE	DataLen;
	BYTE	Data[8];
	BYTE	Reserved[3];
}VCI_CAN_OBJ,*PVCI_CAN_OBJ;

//3.�����ʼ��CAN����������
typedef struct _VCI_INIT_CONFIG{
	DWORD	AccCode;
	DWORD	AccMask;
	DWORD	Reserved;
	UCHAR	Filter;
	UCHAR	Timing0;	
	UCHAR	Timing1;	
	UCHAR	Mode;
}VCI_INIT_CONFIG,*PVCI_INIT_CONFIG;

///////// new add struct for filter /////////
typedef struct _VCI_FILTER_RECORD{
	DWORD ExtFrame;	//�Ƿ�Ϊ��չ֡
	DWORD Start;
	DWORD End;
}VCI_FILTER_RECORD,*PVCI_FILTER_RECORD;
 
#define EXTERNC		extern "C"
#ifdef VCI_EXPORTS
#define VCI_API  __declspec(dllexport)
#else
#define VCI_API  __declspec(dllimport)
#endif

EXTERNC VCI_API DWORD __stdcall VCI_OpenDevice(DWORD DeviceType, DWORD DeviceInd, DWORD Reserved);
EXTERNC VCI_API DWORD __stdcall VCI_CloseDevice(DWORD DeviceType, DWORD DeviceInd);
EXTERNC VCI_API DWORD __stdcall VCI_InitCAN(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, PVCI_INIT_CONFIG pInitConfig);

EXTERNC VCI_API DWORD __stdcall VCI_ReadBoardInfo(DWORD DeviceType, DWORD DeviceInd, PVCI_BOARD_INFO pInfo);

EXTERNC VCI_API DWORD __stdcall VCI_SetReference(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, DWORD RefType, PVOID pData);

EXTERNC VCI_API ULONG __stdcall VCI_GetReceiveNum(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd);
EXTERNC VCI_API DWORD __stdcall VCI_ClearBuffer(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd);

EXTERNC VCI_API DWORD __stdcall VCI_StartCAN(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd);
EXTERNC VCI_API DWORD __stdcall VCI_ResetCAN(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd);

EXTERNC VCI_API ULONG __stdcall VCI_Transmit(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, PVCI_CAN_OBJ pSend, ULONG Len);
EXTERNC VCI_API ULONG __stdcall VCI_Receive(DWORD DeviceType, DWORD DeviceInd, DWORD CANInd, PVCI_CAN_OBJ pReceive, ULONG Len, INT WaitTime);


/*------------------------------------------------�������亯�������ݽṹ����------------------------------------------------*/

//USB-CAN�����������忨��Ϣ����������1��������ΪVCI_FindUsbDevice�����ķ��ز�����
typedef  struct  _VCI_BOARD_INFO1{
	USHORT	hw_Version;
	USHORT	fw_Version;
	USHORT	dr_Version;
	USHORT	in_Version;
	USHORT	irq_Num;
	BYTE	can_Num;
	BYTE	Reserved;
	CHAR	str_Serial_Num[8];
	CHAR	str_hw_Type[16];
	CHAR	str_Usb_Serial[4][4];
} VCI_BOARD_INFO1,*PVCI_BOARD_INFO1;

//USB-CAN�����������忨��Ϣ����������2��������ΪVCI_FindUsbDevice�����ķ��ز�����Ϊ��չ������豸
typedef  struct  _VCI_BOARD_INFO2{
	USHORT	hw_Version;
	USHORT	fw_Version;
	USHORT	dr_Version;
	USHORT	in_Version;
	USHORT	irq_Num;
	BYTE	can_Num;
	BYTE	Reserved;
	CHAR	str_Serial_Num[8];
	CHAR	str_hw_Type[16];
	CHAR	str_Usb_Serial[10][4];
} VCI_BOARD_INFO2,*PVCI_BOARD_INFO2;


#define EXTERNC		extern "C"

EXTERNC VCI_API  DWORD __stdcall VCI_GetReference2(DWORD DevType, DWORD DevIndex, DWORD CANIndex, DWORD Reserved, BYTE *pData);
EXTERNC VCI_API   DWORD __stdcall VCI_SetReference2(DWORD DevType, DWORD DevIndex, DWORD CANIndex, DWORD RefType, BYTE *pData);


EXTERNC VCI_API DWORD __stdcall VCI_ConnectDevice(DWORD DevType, DWORD DevIndex);
EXTERNC VCI_API DWORD __stdcall VCI_UsbDeviceReset(DWORD DevType, DWORD DevIndex, DWORD Reserved);
EXTERNC VCI_API DWORD __stdcall VCI_FindUsbDevice(PVCI_BOARD_INFO1 pInfo);
EXTERNC VCI_API DWORD __stdcall VCI_FindUsbDevice2(PVCI_BOARD_INFO2 pInfo);



#endif
