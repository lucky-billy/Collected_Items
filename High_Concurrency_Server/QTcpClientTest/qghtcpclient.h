#ifndef QGHTCPCLIENT_H
#define QGHTCPCLIENT_H

#include <QSslSocket>
#include <QList>
class QGHSslClient : public QSslSocket
{
	Q_OBJECT

public:
	QGHSslClient(QObject *parent,int nPayLoad = 4096);
	~QGHSslClient();
	quint32 uuid();
	void geneGlobalUUID(QString  globalUuidFile);
private:

	quint32 m_uuid;
	int m_nPayLoad;
	QList<QByteArray> m_buffer_sending;
	QList<qint64> m_buffer_sending_offset;
public slots:
	void some_data_sended(qint64);
	void SendData(QByteArray dtarray);

};

class QGHTcpClient : public QTcpSocket
{
	Q_OBJECT

public:
	QGHTcpClient(QObject *parent,int nPayLoad = 4096);
	~QGHTcpClient();
	quint32 uuid();
	void geneGlobalUUID(QString  globalUuidFile);
private:

	quint32 m_uuid;	int m_nPayLoad;
	QList<QByteArray> m_buffer_sending;
	QList<qint64> m_buffer_sending_offset;
public slots:
	void some_data_sended(qint64);
	void SendData(QByteArray dtarray);

};

#endif // QGHTCPCLIENT_H
