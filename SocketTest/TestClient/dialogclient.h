#ifndef DIALOGCLIENT_H
#define DIALOGCLIENT_H

#include <QDialog>
#include <QtNetwork>

namespace Ui {
class DialogClient;
}


//结构体
struct NetData
{
    unsigned int length;
    unsigned int n1;
    double d1;
    double d2;
    char name[32];  //这里数组可以，注意如果是指针指向堆空间，堆里数据要自己copy
};


class DialogClient : public QDialog
{
    Q_OBJECT

public:
    explicit DialogClient(QWidget *parent = 0);
    ~DialogClient();

private slots:
    void on_pushButtonConnect_clicked();

    //连接成功
    void OnSuccConnect();
    //接收数据
    void ReadStructDataAndShow();


private:
    Ui::DialogClient *ui;
    QTcpSocket *m_pTCPClient;
};

#endif // DIALOGCLIENT_H
