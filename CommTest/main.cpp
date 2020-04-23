/*
This is a demo to show how to receive the packet from USRP in QT5, using the non-blocking IO operation.
most part are just similar as the serial port example terminal, but fixed the packet synchronization problem,
using the separator(int(126)). Please see the slot OutPutData();
In tinyos Sending packet protocol let 125 -> 125 93 126 -> 125 94 ,because the 126 is the separator.

The number is decimal

CRC check is not used
*/

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}

