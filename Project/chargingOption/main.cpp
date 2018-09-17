#include "chargingoption.h"
#include <QtWidgets/QApplication>
extern QString g_AppPath;
int main(int argc, char *argv[])
{
	QApplication::addLibraryPath("./plugins");
	QApplication::addLibraryPath("./platforms");
	QApplication::addLibraryPath(".");
	QApplication::addLibraryPath("./plugins/platforms");

	QApplication a(argc, argv);
	g_AppPath = a.applicationDirPath();
	chargingOption w;
	w.show();
	return a.exec();
}
