#include "ZP_TcpClient.h"
#include <assert.h>

ZP_TcpClient::ZP_TcpClient(QObject *parent,int nPayLoad)
	: QTcpSocket(parent),
	m_nPayLoad(nPayLoad)

{
	assert(m_nPayLoad>=256 && m_nPayLoad<=16*1024*1024);
	connect(this, SIGNAL(bytesWritten(qint64)), this, SLOT(some_data_sended(qint64)));

}

ZP_TcpClient::~ZP_TcpClient()
{

}
void ZP_TcpClient::some_data_sended(qint64 wsended)
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

void ZP_TcpClient::SendData(QByteArray dtarray)
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
