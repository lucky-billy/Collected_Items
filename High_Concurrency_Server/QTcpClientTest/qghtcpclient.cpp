#include "qghtcpclient.h"
#include <assert.h>
#include <QFile>
#include <QThread>
QGHSslClient::QGHSslClient(QObject *parent,int nPayLoad)
	: QSslSocket(parent),
	  m_nPayLoad(nPayLoad)

{
	assert(m_nPayLoad>=256 && m_nPayLoad<=16*1024*1024);
	connect(this, SIGNAL(bytesWritten(qint64)), this, SLOT(some_data_sended(qint64)));
	m_uuid = 0;
}

QGHSslClient::~QGHSslClient()
{

}
quint32 QGHSslClient::uuid()
{
	return m_uuid;
}
void QGHSslClient::geneGlobalUUID(QString  globalUuidFile)
{
	QFile file(globalUuidFile);
	int ctp = 0;
	while (file.open(QIODevice::ReadWrite)==false)
	{
		ctp++;
		if (ctp>=100)
		{
			this->disconnectFromHost();
			return ;
		}
		QThread::currentThread()->sleep(20);
	}
	file.seek(0);
	quint32 oldUUID =1;
	if (file.size()>=sizeof(oldUUID))
		file.read((char *)&oldUUID,sizeof(oldUUID));
	oldUUID ++;
	this->m_uuid = oldUUID;
	file.seek(0);
	file.write((char *)&oldUUID,sizeof(oldUUID));
	file.flush();
	file.close();

}

void QGHSslClient::some_data_sended(qint64 wsended)
{
	while (m_buffer_sending.empty()==false)
	{
		QByteArray & arraySending = *m_buffer_sending.begin();
		qint64 & currentOffset = *m_buffer_sending_offset.begin();
		qint64 nTotalBytes = arraySending.size();
		assert(nTotalBytes>=currentOffset);
		qint64 nBytesWritten = write(arraySending.constData()+currentOffset,qMin((int)(nTotalBytes-currentOffset),m_nPayLoad));
		currentOffset += nBytesWritten;
		if (currentOffset>=nTotalBytes)
		{
			m_buffer_sending.pop_front();
			m_buffer_sending_offset.pop_front();
		}
		else
			break;
	}
}

void QGHSslClient::SendData(QByteArray dtarray)
{
	if (dtarray.size())
	{
		if (m_buffer_sending.empty()==true)
		{
			qint64 bytesWritten = write(dtarray.constData(),qMin(dtarray.size(),m_nPayLoad));
			if (bytesWritten < dtarray.size())
			{
				m_buffer_sending.push_back(dtarray);
				m_buffer_sending_offset.push_back(bytesWritten);
			}
		}
		else
		{
			m_buffer_sending.push_back(dtarray);
			m_buffer_sending_offset.push_back(0);
		}
	}
}
QGHTcpClient::QGHTcpClient(QObject *parent,int nPayLoad)
	: QTcpSocket(parent),
	  m_nPayLoad(nPayLoad)

{
	assert(m_nPayLoad>=256 && m_nPayLoad<=16*1024*1024);
	connect(this, SIGNAL(bytesWritten(qint64)), this, SLOT(some_data_sended(qint64)));
	m_uuid = 0;
}
quint32 QGHTcpClient::uuid()
{
	return m_uuid;
}
void QGHTcpClient::geneGlobalUUID(QString  globalUuidFile)
{
	QFile file(globalUuidFile);
	int ctp = 0;
	while (file.open(QIODevice::ReadWrite)==false)
	{
		ctp++;
		if (ctp>=100)
		{
			this->disconnectFromHost();
			return ;
		}
		QThread::currentThread()->sleep(20);
	}
	file.seek(0);
	quint32 oldUUID =1;
	if (file.size()>=sizeof(oldUUID))
		file.read((char *)&oldUUID,sizeof(oldUUID));
	oldUUID ++;
	this->m_uuid = oldUUID;
	file.seek(0);
	file.write((char *)&oldUUID,sizeof(oldUUID));
	file.flush();
	file.close();
}
QGHTcpClient::~QGHTcpClient()
{

}
void QGHTcpClient::some_data_sended(qint64 wsended)
{
	while (m_buffer_sending.empty()==false)
	{
		QByteArray & arraySending = *m_buffer_sending.begin();
		qint64 & currentOffset = *m_buffer_sending_offset.begin();
		qint64 nTotalBytes = arraySending.size();
		assert(nTotalBytes>=currentOffset);
		qint64 nBytesWritten = write(arraySending.constData()+currentOffset,qMin((int)(nTotalBytes-currentOffset),m_nPayLoad));
		currentOffset += nBytesWritten;
		if (currentOffset>=nTotalBytes)
		{
			m_buffer_sending.pop_front();
			m_buffer_sending_offset.pop_front();
		}
		else
			break;
	}
}

void QGHTcpClient::SendData(QByteArray dtarray)
{
	if (dtarray.size())
	{
		if (m_buffer_sending.empty()==true)
		{
			qint64 bytesWritten = write(dtarray.constData(),qMin(dtarray.size(),m_nPayLoad));
			if (bytesWritten < dtarray.size())
			{
				m_buffer_sending.push_back(dtarray);
				m_buffer_sending_offset.push_back(bytesWritten);
			}
		}
		else
		{
			m_buffer_sending.push_back(dtarray);
			m_buffer_sending_offset.push_back(0);
		}
	}
}
