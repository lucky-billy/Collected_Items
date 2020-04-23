#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"
#include <QTextCodec>
#include <getdata.h>
#include <QQmlPropertyMap>
#include <QQmlContext>
#include <QtDebug>
#include <QSqlDatabase>

bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("db_weather.db");
    if(!db.open())
    {
        return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);

    if(!createConnection())
    {
        return 1;
    }

    Getdata data;//获取天气数据的类



    QtQuick2ApplicationViewer viewer;

    QQmlContext *ctxt = viewer.rootContext();
    ctxt->setContextProperty("owner", &data);//这里加后在qml中可以该问owner/


    viewer.setMainQmlFile(QStringLiteral("qml/mywhether/main.qml"));
    viewer.showExpanded();
    return app.exec();
}
