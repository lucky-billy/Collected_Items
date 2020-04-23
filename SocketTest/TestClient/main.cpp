#include "dialogclient.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DialogClient w;
    w.show();

    return a.exec();
}
