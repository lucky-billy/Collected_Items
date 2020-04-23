#include <QApplication>
#include <QTextCodec>
#include "Keyboard.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("gb18030"));
    Widget w;
    w.show();
    
    return a.exec();
}
