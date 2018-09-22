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
//设置窗口置顶
void SetWindowTop(char * _szAppName){
	HWND hForeWnd = NULL;
	HWND hWnd = FindWindowA(NULL, _szAppName);
	DWORD dwForeID;
	DWORD dwCurID;

	hForeWnd = GetForegroundWindow();  //获取当前最前窗口句柄
	dwCurID = GetCurrentThreadId();
	dwForeID = GetWindowThreadProcessId(hForeWnd, NULL);
	AttachThreadInput(dwCurID, dwForeID, TRUE);  //键盘输入焦点AttachThreadInput
	ShowWindow(hWnd, SW_SHOWNORMAL);
	SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);  //.调整窗口的层次，SetWindowPos使之最上
	SetForegroundWindow(hWnd);
	AttachThreadInput(dwCurID, dwForeID, FALSE);
}

BOOL IsAlreadyRunning(char *_szAppName)
{
	BOOL bRet = FALSE;
	HANDLE hMutex = ::CreateMutexA(NULL, TRUE, _szAppName);  //全局互斥 

	if (GetLastError() == ERROR_ALREADY_EXISTS) //已经存在互斥
	{
		bRet = TRUE;
// 		HWND hWnd = FindWindowA(/*"Qt5QWindowIcon"*/NULL,_szAppName);
// 		if (hWnd)
// 		{
// 			//SetForegroundWindow(hWnd);//尝试激活窗口
// 			// 主窗口已最小化，则恢复其大小 
// 			if (::IsIconic(hWnd))
// 				::ShowWindow(hWnd, SW_RESTORE);
// 
// 			BOOL flag = ::ShowWindow(hWnd, 1);
// 			// 将主窗激活 
// 			flag = ::SetForegroundWindow(hWnd);
// 
// 			// 将主窗的对话框激活 
// 			flag = ::SetForegroundWindow(
// 				::GetLastActivePopup(hWnd));
// 		}
		DWORD dwTimeout = -1;
		SystemParametersInfo(SPI_GETFOREGROUNDLOCKTIMEOUT, 0, (LPVOID)&dwTimeout, 0);
		if (dwTimeout >= 100) {
			SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, (LPVOID)0, SPIF_SENDCHANGE | SPIF_UPDATEINIFILE);
		}
		//设置窗口置顶
		SetWindowTop(_szAppName);
		
	}
	if (hMutex)
	{
		::ReleaseMutex(hMutex);
	}
	return bRet;
}