#include "cclook.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTranslator ts;
	ts.load(":/other/cclook_zh.qm");
	a.installTranslator(&ts);

	CCLook w;
	w.show();
	QDesktopWidget* desktop = QApplication::desktop();
	w.move((desktop->width() - w.width())/2, (desktop->height() - w.height())/2);

	return a.exec();
}
