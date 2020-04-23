#include "widget.h"
#include <QApplication>
#include "../../qt/qDebug2Logcat.h"

int main(int argc, char *argv[])
{
    installLogcatMessageHandler("QtSettings");
    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
