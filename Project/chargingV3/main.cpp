#include "charging.h" 
#include <QtWidgets/QApplication>
#include <QFileInfo>
#include "SingleApplication.h"
#include <windows.h> 


extern char g_AppPath[256] = { 0 };
QString g_winTitle ;
BOOL IsAlreadyRunning(char *_szAppName);
int main(int argc, char *argv[])
{

	QApplication::addLibraryPath("./plugins");
	QApplication::addLibraryPath("./platforms");
	QApplication::addLibraryPath(".");
	QApplication::addLibraryPath("./plugins/platforms");	 

	QApplication a(argc, argv);
	QByteArray ba = a.applicationDirPath().toLatin1();
	strcpy_s(g_AppPath, 256, ba.data());

	COperatorFile::GetInstance()->setAppPath(QString(g_AppPath));
	int iError = 0;
	g_winTitle = CReadIniFile::getInstance()->readProfileInfo("SET", "windowTitle", QString(g_AppPath) + "\\set.ini", &iError);
	if (iError != 0)
		g_winTitle = MAIN_WINDOW_TITLE;
	char szName[256] = { 0 }; sprintf_s(szName, "%s", (g_winTitle.toLocal8Bit()).data());
	if (!IsAlreadyRunning(szName))
	{
		
		charging w;
		w.readConfig();
		w.init_now();
		w.show();
		return a.exec();
	}
	return 0;
}
//���ô����ö�
void SetWindowTop(char * _szAppName){
	HWND hForeWnd = NULL;
	HWND hWnd = FindWindowA(NULL, _szAppName);
	DWORD dwForeID;
	DWORD dwCurID;

	hForeWnd = GetForegroundWindow();  //��ȡ��ǰ��ǰ���ھ��
	dwCurID = GetCurrentThreadId();
	dwForeID = GetWindowThreadProcessId(hForeWnd, NULL);
	AttachThreadInput(dwCurID, dwForeID, TRUE);  //�������뽹��AttachThreadInput
	ShowWindow(hWnd, SW_SHOWNORMAL);
	SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);  //.�������ڵĲ�Σ�SetWindowPosʹ֮����
	SetForegroundWindow(hWnd);
	AttachThreadInput(dwCurID, dwForeID, FALSE);
}

BOOL IsAlreadyRunning(char *_szAppName)
{
	BOOL bRet = FALSE;
	HANDLE hMutex = ::CreateMutexA(NULL, TRUE, _szAppName);  //ȫ�ֻ��� 

	if (GetLastError() == ERROR_ALREADY_EXISTS) //�Ѿ����ڻ���
	{
		bRet = TRUE;
// 		HWND hWnd = FindWindowA(/*"Qt5QWindowIcon"*/NULL,_szAppName);
// 		if (hWnd)
// 		{
// 			//SetForegroundWindow(hWnd);//���Լ����
// 			// ����������С������ָ����С 
// 			if (::IsIconic(hWnd))
// 				::ShowWindow(hWnd, SW_RESTORE);
// 
// 			BOOL flag = ::ShowWindow(hWnd, 1);
// 			// ���������� 
// 			flag = ::SetForegroundWindow(hWnd);
// 
// 			// �������ĶԻ��򼤻� 
// 			flag = ::SetForegroundWindow(
// 				::GetLastActivePopup(hWnd));
// 		}
		DWORD dwTimeout = -1;
		SystemParametersInfo(SPI_GETFOREGROUNDLOCKTIMEOUT, 0, (LPVOID)&dwTimeout, 0);
		if (dwTimeout >= 100) {
			SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, (LPVOID)0, SPIF_SENDCHANGE | SPIF_UPDATEINIFILE);
		}
		//���ô����ö�
		SetWindowTop(_szAppName);
		
	}
	if (hMutex)
	{
		::ReleaseMutex(hMutex);
	}
	return bRet;
}