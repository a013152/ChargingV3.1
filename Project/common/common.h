#ifndef H_COMMON
#define H_COMMON
 

extern char g_AppPath[256];  //exe·�� 

#define  MAIN_WINDOW_TITLE "δ��ȡ����"
#define  SETING_WINDOW_TITLE "ϵͳ��������"

#define DBHOSTNAME "119.23.48.100"
#define CHARGINGOPTIONPROCESS  "chargingOption.exe"  //���ó������ƣ��޸ĵ�ص�ѹ�����Ľ��̣�
#define CANDEVICETRANSMITION "canDeviceTransmition.exe" //can �豸ͨѶ����
#define PIPE_NAME "\\\\.\\Pipe\\canDevicePipe" //�ܵ�����
#define S2C "S2C" //���̷��� �������ͻ���
#define C2S "C2S" //���̷��� �ͻ���������
#define SPILTE_CHAR ','  //�ָ��ַ�','
enum enCANDevieErrorCode{	//CAN���̷��صĴ�����
	Success = 0,
	DeviceNotOpen,
	DetailError,
};
#define MAX_BUF_SIZE 1024 //������

#define TIMER_INTERVAL 100  //�رն�ʱ��ʱ���� 100����

#define ONE_CLOSET_SCAN_TIME 3 * 1000  //��Ԫ���ʱ��  
#define ONE_SUBMIT_TIME 15000			//�����ύ���
#define CHARGING_LIMIT_TIME 360		//���ʱ�ޣ�����������ֹͣ������Ϊ��λ add20180521
#define DEFAUT_OVERHEATTEMPERATURE 45 //�����¶�
#define MAX_CLOSET  10  //����������
#define MAX_CHARGER 48  //ÿ�����������������

 
#define MAX_BATTERY 15  //ÿ���������������

#define ONE_PAGE_CLOSET 5  //ÿҳ��������
#define ONE_CLOSET_SCAN_COMMAND 14 //ÿ����ɨ���������� 3��M 8��G 3��D һ��14��

#define MAX_RELAY_TO_LOOP 6  //һ���̵�����Ӧ���Ļ�·
#define BEGIN_RELAY_ID 25  //һ���̵�����ʼID


//������Ϣ
#define ERROR_DONT_OPEN_SERIAL -1;   //����δ���á�
#define ERROR_NO_BATTERY -2;   //���δ���á�
#define ERROR_NO_CHARGER -3;   //�����δ����
#define ERROR_BATTERY_CHARGING -4;   //����������ڷŵ�硣
 
#define ERROR_CONFIG   -5; //δƥ����Ϣ
#define ERROR_CONFIG_CLOSET -6;   //δƥ����ӡ�
#define ERROR_CONFIG_BATTERY -7;   //δƥ���ء�
#define ERROR_CONFIG_CHARGER -8;   //δƥ��������
#define ERROR_CONFIG_RELAY -9;   //δƥ��̵�����
#define ERROR_CONFIG_BATTERY_MODEL -10;   //δƥ�����ͺš�
#define ERROR_OVER_HEAT -11;   //���������

#define ERROR_DONT_OPEN_CAN -12;   //CAN�豸δ�򿪡�


#define STATE_OFFLINE "�����������"
#define STATE_FREE "���������"
#define STATE_CHARGING "�����"
#define STATE_DISCHARGING "�ŵ���"
#define STATE_APPLYCHARGING "Ԥ��"
#define STATE_OVER_HEAT "���¹���"

//������Ϣ
#define DEBUG_LOG_FLAG  1
#endif // !H_COMMON
