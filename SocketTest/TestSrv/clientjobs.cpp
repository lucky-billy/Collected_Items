#include "clientjobs.h"

ClientJobs::ClientJobs(QObject *parent, QTcpSocket *client, QString strIPPort) :
    QObject(parent)
{
    m_pClientSock = client;
    m_strIPPort = strIPPort;
    //关联信号
    connect(m_pClientSock, SIGNAL(disconnected()), this, SLOT(LostConnection()));
    //可以关联接收数据信号readyRead()，这里不管

}

void ClientJobs::SendBytes(QByteArray &data)
{
    m_pClientSock->write(data);
}

void ClientJobs::LostConnection()
{
    //通知主窗口删除自己
    emit CallMainWindowDeleteClient(m_strIPPort);
}
