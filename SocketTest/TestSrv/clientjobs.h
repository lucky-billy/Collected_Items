#ifndef CLIENTJOBS_H
#define CLIENTJOBS_H

#include <QObject>
#include <QtNetwork>
#include "clientjobs.h"


class ClientJobs : public QObject
{
    Q_OBJECT
public:
    explicit ClientJobs(QObject *parent, QTcpSocket *client, QString strIPPort);
    //TCP连接
    QTcpSocket *m_pClientSock;
    QString m_strIPPort;//IP_Port


signals:
    void CallMainWindowDeleteClient(QString m_strIPPort);

public slots:
    void SendBytes(QByteArray &data);
    void LostConnection();


};

#endif // CLIENTJOBS_H
