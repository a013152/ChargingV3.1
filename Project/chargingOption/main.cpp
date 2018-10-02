#include "chargingoption.h"
#include <QtWidgets/QApplication>
extern char  g_AppPath[256] = { 0 };
int main(int argc, char *argv[])
{
	QApplication::addLibraryPath("./plugins");
	QApplication::addLibraryPath("./platforms");
	QApplication::addLibraryPath(".");
	QApplication::addLibraryPath("./plugins/platforms");

	QApplication a(argc, argv);
	QByteArray ba = a.applicationDirPath().toLatin1();
	strcpy_s(g_AppPath, 256, ba.data());
	chargingOption w;
	w.show();
	return a.exec();
}
