#include "zpmainframe.h"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
int main(int argc, char *argv[])
{
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

	ZPMainFrame w;
	//Command Line Args
	if (argc>1)
	{

	}
	else
	{

	}
	w.show();
	int pp = app.exec();
	return pp;
}
