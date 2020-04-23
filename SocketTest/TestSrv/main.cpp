#include "dialogsrv.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DialogSrv w;
    w.show();

    return a.exec();
}
