#ifndef QGHTCPCLIENT_H
#define QGHTCPCLIENT_H

#include <QTcpSocket>
#include <QList>
class ZP_TcpClient : public QTcpSocket
{
	Q_OBJECT

public:
    ZP_TcpClient(QObject *parent,int nPayLoad = 4096);
    ~ZP_TcpClient();

private:
	int m_nPayLoad;
	QList<QByteArray> m_buffer_sending;
	QList<qint64> m_buffer_sending_offset;
public slots:
	void some_data_sended(qint64);
	void SendData(QByteArray dtarray);

};

#endif // QGHTCPCLIENT_H
