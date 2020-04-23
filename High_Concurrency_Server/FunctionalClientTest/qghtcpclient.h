#ifndef QGHTCPCLIENT_H
#define QGHTCPCLIENT_H

#include <QTcpSocket>
#include <QList>
class QGHTcpClient : public QTcpSocket
{
	Q_OBJECT

public:
	QGHTcpClient(QObject *parent,int nPayLoad = 4096);
	~QGHTcpClient();

private:
	int m_nPayLoad;
	QList<QByteArray> m_buffer_sending;
	QList<qint64> m_buffer_sending_offset;
public slots:
	void some_data_sended(qint64);
	void SendData(QByteArray dtarray);

};

#endif // QGHTCPCLIENT_H
