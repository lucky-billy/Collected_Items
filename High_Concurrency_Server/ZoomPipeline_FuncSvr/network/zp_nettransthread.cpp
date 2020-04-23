#include "zp_nettransthread.h"
#include <QTcpSocket>
#include <QSslSocket>
#include <assert.h>
#include <QDebug>
#include <QCoreApplication>
#include <QHostAddress>
#include "zp_net_threadpool.h"
#include <QMutex>
//Trans st
QMutex g_mutex_sta;
quint64 g_bytesRecieved = 0;
quint64 g_bytesSent = 0;
quint64 g_secRecieved = 0;
quint64 g_secSent = 0;

namespace ZPNetwork{
	zp_netTransThread::zp_netTransThread(zp_net_Engine *pThreadPool,int nPayLoad,QObject *parent) :
		QObject(parent)
	  ,m_pThreadPool(pThreadPool)
	{
		m_nPayLoad = nPayLoad;
		m_bActivated = true;
		m_bSSLConnection = true;
		assert(m_nPayLoad>=256 && m_nPayLoad<=16*1024*1024);
	}

	bool zp_netTransThread::isActive()
	{
		return m_bActivated;
	}
	bool zp_netTransThread::SSLConnection()
	{
		return m_bSSLConnection ;
	}
	void zp_netTransThread::SetSSLConnection(bool bssl)
	{
		m_bSSLConnection = bssl;
	}
	void zp_netTransThread::Deactivate()
	{
		m_bActivated = false;
	}
	/**
	 * @brief return a list contains all of online-clients objects.
	 *
	 * @return QList<QObject *> Online clients
	 */
	QList <QObject *> zp_netTransThread::clientsList()
	{
		QList <QObject *> lsts ;
		m_mutex_protect.lock();
		lsts = m_clientList.values();
		m_mutex_protect.unlock();
		return lsts;
	}
	/**
	 * @brief return howmany clients holded by this zp_netTransThread
	 *
	 * @return int the clients count
	 */
	int zp_netTransThread::CurrentClients()
	{
		int nres = 0;
		m_mutex_protect.lock();
		nres = m_clientList.size();
		m_mutex_protect.unlock();
		return nres;
	}
	/**
	 * @brief this slot recieves evt_DeactivateImmediately message,
	 * if this message is sent to this object, a "KickAllClients"
	 * method will be called.
	 *
	 * @param ptr
	 */
	void zp_netTransThread::DeactivateImmediately(zp_netTransThread * ptr)
	{
		if (ptr!=this)
			return;
		m_bActivated = false;
		this->KickAllClients(ptr);
	}

	/**
	 * @brief set the payload of this object. data large than nPayload will be
	 * cutted into pieces, each piece is nPayload bytes. the last piece may be
	 * shorter than nPayload.
	 *
	 * @param nPayload bytes a signal block contains .
	 */
	void zp_netTransThread::SetPayload(int nPayload)
	{
		m_nPayLoad = nPayload;
		assert(m_nPayLoad>=256 && m_nPayLoad<=16*1024*1024);
	}

	/**
	 * @brief This slot dealing with multi-thread client socket accept.
	 * accepy works start from zp_netListenThread::m_tcpserver, end with this method.
	 * the socketDescriptor is delivered from zp_netListenThread(a Listening thread)
	 *  to zp_net_Engine(Normally in main-gui thread), and then zp_netTransThread.
	 *
	 * @param threadid if threadid is not equal to this object, this message is just omitted.
	 * @param socketDescriptor socketDescriptor for incomming client.
	 */
	void zp_netTransThread::incomingConnection(QObject * threadid,qintptr socketDescriptor)
	{
		if (threadid!=this)
			return;
		QTcpSocket * sock_client = 0;
		if (m_bSSLConnection)
			sock_client =  new QSslSocket(this);
		else
			sock_client =  new QTcpSocket(this);
		if (sock_client)
		{
			//Initial content
			if (true ==sock_client->setSocketDescriptor(socketDescriptor))
			{
				connect(sock_client, &QTcpSocket::readyRead,this, &zp_netTransThread::new_data_recieved,Qt::QueuedConnection);
				connect(sock_client, &QTcpSocket::disconnected,this,&zp_netTransThread::client_closed,Qt::QueuedConnection);
				connect(sock_client, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(displayError(QAbstractSocket::SocketError)),Qt::QueuedConnection);
				connect(sock_client, &QTcpSocket::bytesWritten, this, &zp_netTransThread::some_data_sended,Qt::QueuedConnection);
				m_mutex_protect.lock();
				m_clientList.insert(sock_client);
				m_mutex_protect.unlock();
				if (m_bSSLConnection)
				{
					QSslSocket * psslsock = qobject_cast<QSslSocket *>(sock_client);
					assert(psslsock!=NULL);
					QString strCerPath =  QCoreApplication::applicationDirPath() + "/svr_cert.pem";
					QString strPkPath =  QCoreApplication::applicationDirPath() + "/svr_privkey.pem";
					psslsock->setLocalCertificate(strCerPath);
					psslsock->setPrivateKey(strPkPath);
					connect(psslsock, &QSslSocket::encrypted,this, &zp_netTransThread::on_encrypted,Qt::QueuedConnection);
					psslsock->startServerEncryption();
				}
				emit evt_NewClientConnected(sock_client);
				emit evt_Message(sock_client,"Info>" +  QString(tr("Client Accepted.")));
			}
			else
				sock_client->deleteLater();
		}

	}

	/**
	 * @brief This method will create a socket, and directly connect to destion address.
	 *
	 * @param threadid if threadid is not equal to this object, this message is just omitted.
	 * @param addr address to which the socket should be connected.
	 * @param port port to which the socket should be connected.
	 */
	void zp_netTransThread::startConnection(QObject * threadid,const QHostAddress & addr, quint16 port)
	{
		if (threadid!=this)
			return;
		QTcpSocket * sock_client = 0;
		if (m_bSSLConnection)
			sock_client =  new QSslSocket(this);
		else
			sock_client =  new QTcpSocket(this);
		if (sock_client)
		{
			if (m_bSSLConnection==true)
			{
				QSslSocket * psslsock = qobject_cast<QSslSocket *>(sock_client);
				assert(psslsock!=NULL);
				QString strCerPath =  QCoreApplication::applicationDirPath() + "/ca_cert.pem";
				QList<QSslCertificate> lstCas = QSslCertificate::fromPath(strCerPath);
				psslsock->setCaCertificates(lstCas);

				connect(sock_client, &QTcpSocket::readyRead,this,&zp_netTransThread::new_data_recieved,Qt::QueuedConnection);
				connect(sock_client, &QTcpSocket::disconnected,this,&zp_netTransThread::client_closed,Qt::QueuedConnection);
				connect(sock_client, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(displayError(QAbstractSocket::SocketError)),Qt::QueuedConnection);
				connect(sock_client, &QTcpSocket::bytesWritten, this,&zp_netTransThread::some_data_sended,Qt::QueuedConnection);
				connect(sock_client, &QTcpSocket::connected,this, &zp_netTransThread::on_connected,Qt::QueuedConnection);
				connect(psslsock, &QSslSocket::encrypted,this, &zp_netTransThread::on_encrypted,Qt::QueuedConnection);
				m_mutex_protect.lock();
				m_clientList.insert(sock_client);
				m_mutex_protect.unlock();

				psslsock->connectToHostEncrypted(addr.toString(),port);
			}
			else
			{
				connect(sock_client, &QTcpSocket::readyRead,this, &zp_netTransThread::new_data_recieved,Qt::QueuedConnection);
				connect(sock_client, &QTcpSocket::disconnected,this,&zp_netTransThread::client_closed,Qt::QueuedConnection);
				connect(sock_client, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(displayError(QAbstractSocket::SocketError)),Qt::QueuedConnection);
				connect(sock_client, &QTcpSocket::bytesWritten, this,&zp_netTransThread::some_data_sended,Qt::QueuedConnection);
				connect(sock_client, &QTcpSocket::connected,this, &zp_netTransThread::on_connected,Qt::QueuedConnection);
				m_mutex_protect.lock();
				m_clientList.insert(sock_client);
				m_mutex_protect.unlock();
				sock_client->connectToHost(addr,port);

			}
		}
		else
			assert(false);
	}
	void zp_netTransThread::on_connected()
	{
		QTcpSocket * pSock = qobject_cast<QTcpSocket*>(sender());
		emit evt_NewClientConnected(pSock);
		emit evt_Message(pSock,"Info>" +  QString(tr("Client connected.")));
	}

	void zp_netTransThread::on_encrypted()
	{
		QTcpSocket * pSock = qobject_cast<QTcpSocket*>(sender());
		emit evt_ClientEncrypted(pSock);
		emit evt_Message(pSock,"Info>" +  QString(tr("Client Encrypted.")));
	}

	void zp_netTransThread::client_closed()
	{
		QTcpSocket * pSock = qobject_cast<QTcpSocket*>(sender());
		if (pSock)
		{
			if (m_bSSLConnection)
			{
				QSslSocket * psslsock = qobject_cast<QSslSocket *>(pSock);
				if (psslsock)
					disconnect(psslsock, &QSslSocket::encrypted,this, &zp_netTransThread::on_encrypted);
			}
			disconnect(pSock, &QTcpSocket::readyRead,this, &zp_netTransThread::new_data_recieved);
			disconnect(pSock, &QTcpSocket::disconnected,this,&zp_netTransThread::client_closed);
			disconnect(pSock, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(displayError(QAbstractSocket::SocketError)));
			disconnect(pSock, &QTcpSocket::bytesWritten, this, &zp_netTransThread::some_data_sended);
			disconnect(pSock, &QTcpSocket::connected,this, &zp_netTransThread::on_connected);

			m_buffer_sending.erase(pSock);
			m_buffer_sending_offset.erase(pSock);
			m_mutex_protect.lock();
			m_clientList.remove(pSock);
			m_mutex_protect.unlock();
			pSock->deleteLater();
			emit evt_ClientDisconnected(pSock);
			emit evt_Message(pSock,"Info>" +  QString(tr("Client Closed.")));
		}
	}
	void zp_netTransThread::new_data_recieved()
	{
		QTcpSocket * pSock = qobject_cast<QTcpSocket*>(sender());
		if (pSock)
		{
			QByteArray array = pSock->readAll();
			int sz = array.size();
			g_mutex_sta.lock();
			g_bytesRecieved +=sz;
			g_secRecieved += sz;
			g_mutex_sta.unlock();
			emit evt_Data_recieved(pSock,array);
		}
	}
	/**
	 * @brief this slot will be called when internal socket successfully
	 * sent some data. in this method, zp_netTransThread object will check
	 * the sending-queue, and send more data to buffer.
	 *
	 * @param wsended
	 */
	void zp_netTransThread::some_data_sended(qint64 wsended)
	{
		g_mutex_sta.lock();
		g_bytesSent +=wsended;
		g_secSent += wsended;
		g_mutex_sta.unlock();
		QTcpSocket * pSock = qobject_cast<QTcpSocket*>(sender());
		if (pSock)
		{
			emit evt_Data_transferred(pSock,wsended);
			QList<QByteArray> & list_sock_data = m_buffer_sending[pSock];
			QList<qint64> & list_offset = m_buffer_sending_offset[pSock];
			while (list_sock_data.empty()==false)
			{
				QByteArray & arraySending = *list_sock_data.begin();
				qint64 & currentOffset = *list_offset.begin();
				qint64 nTotalBytes = arraySending.size();
				assert(nTotalBytes>=currentOffset);
				qint64 nBytesWritten = pSock->write(arraySending.constData()+currentOffset,qMin((int)(nTotalBytes-currentOffset),m_nPayLoad));
				currentOffset += nBytesWritten;
				if (currentOffset>=nTotalBytes)
				{
					list_offset.pop_front();
					list_sock_data.pop_front();
				}
				else
					break;
			}
		}
	}
	void zp_netTransThread::displayError(QAbstractSocket::SocketError socketError)
	{
		QTcpSocket * pSock = qobject_cast<QTcpSocket*>(sender());
		if (pSock)
		{
			emit evt_SocketError(pSock,socketError);
			emit evt_Message(pSock,"Debug:" + pSock->errorString());
			if (m_bSSLConnection)
			{
				QSslSocket * psslsock = qobject_cast<QSslSocket *>(pSock);
				if (psslsock)
					disconnect(psslsock, &QSslSocket::encrypted,this, &zp_netTransThread::on_encrypted);
			}
			disconnect(pSock, &QTcpSocket::readyRead,this, &zp_netTransThread::new_data_recieved);
			disconnect(pSock, &QTcpSocket::disconnected,this,&zp_netTransThread::client_closed);
			disconnect(pSock, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(displayError(QAbstractSocket::SocketError)));
			disconnect(pSock, &QTcpSocket::bytesWritten, this, &zp_netTransThread::some_data_sended);
			disconnect(pSock, &QTcpSocket::connected,this, &zp_netTransThread::on_connected);

			m_buffer_sending.erase(pSock);
			m_buffer_sending_offset.erase(pSock);
			m_mutex_protect.lock();
			m_clientList.remove(pSock);
			m_mutex_protect.unlock();
			pSock->abort();
			pSock->deleteLater();
			emit evt_ClientDisconnected(pSock);
			emit evt_Message(pSock,"Info>" +  QString(tr("Client Closed.")));

		}
	}

	void zp_netTransThread::SendDataToClient(QObject * objClient,QByteArray   dtarray)
	{
		m_mutex_protect.lock();
		if (m_clientList.contains(objClient)==false)
		{
			m_mutex_protect.unlock();
			return;
		}
		m_mutex_protect.unlock();
		QTcpSocket * pSock = qobject_cast<QTcpSocket*>(objClient);
		if (pSock&&dtarray.size())
		{
			QList<QByteArray> & list_sock_data = m_buffer_sending[pSock];
			QList<qint64> & list_offset = m_buffer_sending_offset[pSock];
			if (list_sock_data.empty()==true)
			{
				qint64 bytesWritten = pSock->write(dtarray.constData(),qMin(dtarray.size(),m_nPayLoad));
				if (bytesWritten < dtarray.size())
				{
					list_sock_data.push_back(dtarray);
					list_offset.push_back(bytesWritten);
				}
			}
			else
			{
				list_sock_data.push_back(dtarray);
				list_offset.push_back(0);
			}
		}
	}

	void zp_netTransThread::KickAllClients(zp_netTransThread * ptr)
	{
		if (ptr!=this)
			return;
		m_mutex_protect.lock();
		QList<QObject *> clientList = m_clientList.values();

		foreach(QObject * obj,clientList)
		{
			QTcpSocket * pSock = qobject_cast<QTcpSocket*>(obj);
			if (pSock)
			{
				QSslSocket * pSSl = qobject_cast<QSslSocket*>(pSock);
				if (pSSl==NULL)
					pSock->abort();
				else
					pSock->disconnectFromHost();

			}

		}
		m_mutex_protect.unlock();
	}

	void zp_netTransThread::KickClient(QObject * objClient)
	{
		m_mutex_protect.lock();
		if (m_clientList.contains(objClient)==false)
		{
			m_mutex_protect.unlock();
			return;
		}
		m_mutex_protect.unlock();
		QTcpSocket * pSock = qobject_cast<QTcpSocket*>(objClient);

		if (pSock)
		{
			QSslSocket * pSSl = qobject_cast<QSslSocket*>(pSock);
			if (pSSl==NULL)
				pSock->abort();
			else
				pSock->disconnectFromHost();
		}
	}

	bool zp_netTransThread::CanExit()
	{
		if (m_bActivated==true)
			return false;
		if (CurrentClients())
			return false;
		return true;
	}
}
