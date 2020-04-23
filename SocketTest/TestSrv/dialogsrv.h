#ifndef DIALOGSRV_H
#define DIALOGSRV_H

#include <QDialog>
#include <QtNetwork>
#include <QStringList>
#include <QList>
#include "clientjobs.h"

namespace Ui {
class DialogSrv;
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


class DialogSrv : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSrv(QWidget *parent = 0);
    ~DialogSrv();

private slots:
    void on_pushButtonListen_clicked();
    //两个信号的槽函数
    void onNewConnection();
    //
    void DeleteOneClient(QString strIPAndPort);


    void on_pushButtonStop_clicked();

    void on_pushButtonSend_clicked();

private:
    Ui::DialogSrv *ui;
    //server
    QTcpServer *m_pTCPSrv;

    //保存客户列表
    QStringList m_listIPAndPorts;  //IP_Port
    QList<ClientJobs*> m_listClients;//客户端


};

#endif // DIALOGSRV_H
