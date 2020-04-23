#include "qtcpclienttest.h"
#include <QApplication>
#include <time.h>
#include <stdlib.h>
#include <QTranslator>
#include <QLibraryInfo>
int main(int argc, char *argv[])
{
	srand(time(0));
	QApplication app(argc, argv);
	QTranslator qtTranslator;
	qtTranslator.load("qt_" + QLocale::system().name(),
					  QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	app.installTranslator(&qtTranslator);

	QTranslator appTranslator;
	QString strTransLocalFile =
			QCoreApplication::applicationDirPath()+"/" +
			QCoreApplication::applicationName()+"_"+
			QLocale::system().name()+".qm";
	appTranslator.load(strTransLocalFile );
	app.installTranslator(&appTranslator);

	QTcpClientTest w;
	w.show();
	return app.exec();
}
