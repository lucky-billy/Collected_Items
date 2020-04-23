#include "zp_net_threadpool.h"
#include <QCoreApplication>
#include <QStringList>

namespace ZPNetwork{
	/**
	 * @brief Construct a net thread pool object.
	 *
	 * @fn zp_net_Engine
	 * @param nPayLoad The data to be sent in buffer will be cutted into pieces, each pieces equals to nPayLoad bytes
	 * @param parent a Qt-style parent pointer. this object will be auto-deleted when parent is about to be destoryed.
	 */
	zp_net_Engine::zp_net_Engine(int nPayLoad,QObject *parent) :
		QObject(parent)
	{
		m_nPayLoad = nPayLoad;
		//The signals and slots will be reged.
		if (false==QMetaType::isRegistered(QMetaType::type("qintptr")))
			qRegisterMetaType<qintptr>("qintptr");
		if (false==QMetaType::isRegistered(QMetaType::type("QAbstractSocket::SocketError")))
			qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
		if (false==QMetaType::isRegistered(QMetaType::type("QHostAddress")))
			qRegisterMetaType<QHostAddress>("QHostAddress");

	}
	/**
	 * @brief return all of these listener names.
	 *
	 * @fn zp_net_Engine::ListenerNames
	 * @return QStringList listener names.
	 */
	QStringList zp_net_Engine::ListenerNames()
	{
		return m_map_netListenThreads.keys();
	}

	/**
	 * @brief Return how many threads is now actived for TCP transfer.
	 *
	 * @fn zp_net_Engine::TransThreadNum
	 * @return int 0 means no thread is active.
	 */
	int zp_net_Engine::TransThreadNum()
	{
		return m_vec_NetTransThreads.size();
	}
	/**
	 * @brief Return how many threads is now actived for SSL/Plain TCP transfer.
	 *
	 * @fn zp_net_Engine::TransThreadNum
	 * @param bSSL true means SSL Connection, false is Plain Connection
	 * @return int The transfer thread count for SSL connections.
	 */
	int zp_net_Engine::TransThreadNum(bool bSSL)
	{
		//m_mutex_trans.lock();
		int nsz = m_vec_NetTransThreads.size();
		int nCount = 0;
		for (int i=0;i<nsz;i++)
		{
			if (m_vec_NetTransThreads[i]->SSLConnection()==bSSL)
				nCount ++;
		}
		//m_mutex_trans.unlock();
		return nCount;
	}

	/**
	 * @brief This function returns how many clients managed by idxThread is now online.
	 *
	 * @fn zp_net_Engine::totalClients
	 * @param idxThread The thread index between 0 and TransThreadNum()-1
	 * @return int Clients which are currently online
	 */
	int zp_net_Engine::totalClients(int idxThread)
	{
		int nsz = m_vec_NetTransThreads.size();
		if (idxThread >=0 && idxThread<nsz)
			return m_vec_NetTransThreads[idxThread]->CurrentClients();
		else
			return 0;
	}

	/**
	 * @brief Begin a listening socket at special address and port. The socket will be activated as soon as possible
	 * The Wait-and-Accept approaches are all managed by Listening thread, instead of main-GUI thread.
	 * @fn zp_net_Engine::AddListeningAddress
	 * @param id The name of this listening address:port. You can later using RemoveListeningAddress(id) to close it.
	 * @param address Listening address. IPV6 is now not tested.
	 * @param nPort Listening Port. in Linux, this value should larger than 1024.
	 * @param bSSLConn True means this address using OpenSSL . False means plain TCP
	 */
	void zp_net_Engine::AddListeningAddress(QString  id,const QHostAddress & address , quint16 nPort,bool bSSLConn /*= true*/)
	{
		if (m_map_netListenThreads.find(id)==m_map_netListenThreads.end())
		{
			//Start Thread
			QThread * pThread = new QThread(this);
			zp_netListenThread * pListenObj = new zp_netListenThread(id,address,nPort,bSSLConn);
			pThread->start();
			//m_mutex_listen.lock();
			m_map_netInternalListenThreads[id] = pThread;
			m_map_netListenThreads[id] = pListenObj;
			//m_mutex_listen.unlock();
			//Bind Object to New thread
			connect(this,&zp_net_Engine::startListen,pListenObj,&zp_netListenThread::startListen,Qt::QueuedConnection);
			connect(this,&zp_net_Engine::stopListen,pListenObj,&zp_netListenThread::stopListen,Qt::QueuedConnection);
			connect(pListenObj,&zp_netListenThread::evt_Message,this,&zp_net_Engine::evt_Message,Qt::QueuedConnection);
			connect(pListenObj,&zp_netListenThread::evt_ListenClosed,this,&zp_net_Engine::on_ListenClosed,Qt::QueuedConnection);
			connect(pListenObj,&zp_netListenThread::evt_NewClientArrived,this,&zp_net_Engine::on_New_Arrived_Client,Qt::QueuedConnection);

			pListenObj->moveToThread(pThread);
			//Start Listen Immediately
			emit startListen(id);
		}
		else
			emit evt_Message(this,"Warning>"+QString(tr("This ID has been used.")));
	}

	/**
	 * @brief Remove a listening socket at special address and port.The socket will be deactivated as soon as possible
	 *
	 * @fn zp_net_Engine::RemoveListeningAddress
	 * @param id Listening-Address name specified by AddListeningAddress()
	 */
	void zp_net_Engine::RemoveListeningAddress(QString  id)
	{
		//m_mutex_listen.lock();
		if (m_map_netListenThreads.find(id)!=m_map_netListenThreads.end())
			emit stopListen(id);
		//m_mutex_listen.unlock();
	}
	/**
	 * @brief Cancel all addresses.
	 *
	 * @fn zp_net_Engine::RemoveAllAddresses
	 */
	void zp_net_Engine::RemoveAllAddresses()
	{
		//m_mutex_listen.lock();
		foreach (QString id,m_map_netListenThreads.keys())
			emit stopListen(id);
		//m_mutex_listen.unlock();
	}

	/**
	 * @brief This slot recieved incomming client socketDescriptor from listening threads.In this function, zp_net_Engine
	 * will make a balance control, choosing a trans-thread which currently holds least clients than others.
	 * @fn zp_net_Engine::on_New_Arrived_Client
	 * @param socketDescriptor the socket descriptor for incomming client.
	 */
	void zp_net_Engine::on_New_Arrived_Client(qintptr socketDescriptor)
	{
		zp_netListenThread * pSource = qobject_cast<zp_netListenThread *>(sender());
		if (!pSource)
		{
			emit evt_Message(this,"Warning>"+QString(tr("Non-zp_netListenThread type detected.")));
			return;
		}

		emit evt_Message(this,"Info>" +  QString(tr("Incomming client arriverd.")));
		//m_mutex_trans.lock();
		int nsz = m_vec_NetTransThreads.size();
		int nMinPay = 0x7fffffff;
		int nMinIdx = -1;

		for (int i=0;i<nsz && nMinPay!=0;i++)
		{
			if (m_vec_NetTransThreads[i]->isActive()==false ||
					m_vec_NetTransThreads[i]->SSLConnection()!=pSource->bSSLConn()
					)
				continue;
			int nPat = m_vec_NetTransThreads[i]->CurrentClients();

			if (nPat<nMinPay)
			{
				nMinPay = nPat;
				nMinIdx = i;
			}
			//qDebug()<<i<<" "<<nPat<<" "<<nMinIdx;
		}
		//remove old threads
		for (int i=0;i<nsz;i++)
			if (m_vec_NetTransThreads[i]->isActive()==false )
				TransThreadDel(m_vec_NetTransThreads[i]);

		if (nMinIdx>=0 && nMinIdx<nsz)
			emit evt_EstablishConnection(m_vec_NetTransThreads[nMinIdx],socketDescriptor);
		else
		{
			emit evt_Message(this,"Warning>"+QString(tr("Need Trans Thread Object for clients.")));
		}
		//m_mutex_trans.unlock();
	}
	/**
	 * @brief This is a slot function. when a listening thread is terminated,
	 * the thread itself will be deleted here. Some cleanning works is also needed.
	 * @fn zp_net_Engine::on_ListenClosed
	 * @param id the terminated thread id.
	 */
	void zp_net_Engine::on_ListenClosed(QString  id)
	{
		//m_mutex_listen.lock();
		if (m_map_netListenThreads.find(id)!=m_map_netListenThreads.end())
		{
			//Clean objects;
			zp_netListenThread * pListenObj = m_map_netListenThreads[id];
			QThread * pThread = m_map_netInternalListenThreads[id];
			m_map_netListenThreads.remove(id);
			m_map_netInternalListenThreads.remove(id);
			//disconnect signals;
			disconnect(this,&zp_net_Engine::startListen,pListenObj,&zp_netListenThread::startListen);
			disconnect(this,&zp_net_Engine::stopListen,pListenObj,&zp_netListenThread::stopListen);
			disconnect(pListenObj,&zp_netListenThread::evt_Message,this,&zp_net_Engine::evt_Message);
			disconnect(pListenObj,&zp_netListenThread::evt_ListenClosed,this,&zp_net_Engine::on_ListenClosed);
			disconnect(pListenObj,&zp_netListenThread::evt_NewClientArrived,this,&zp_net_Engine::on_New_Arrived_Client);
			pListenObj->deleteLater();
			pThread->quit();
			pThread->wait();
			pThread->deleteLater();

		}
		//m_mutex_listen.unlock();
	}
	//Add n client-Trans Thread(s).
	/**
	 * @brief Add some threads to hold transfer between server and client.
	 * The send and recieve approaches are all managed by these threads, instead of main-GUI thread.
	 * @fn zp_net_Engine::AddClientTransThreads
	 * @param nThreads
	 * @param bSSL
	 */
	void zp_net_Engine::AddClientTransThreads(int nThreads,bool bSSL)
	{
		if (nThreads>0 && nThreads<256)
		{
			for (int i=0;i<nThreads;i++)
			{
				zp_netTransThread * clientTH = new zp_netTransThread(this,m_nPayLoad);
				clientTH->SetSSLConnection(bSSL);
				QThread * pThread = new QThread(this);
				//m_mutex_trans.lock();
				m_vec_netInternalTransThreads.push_back(pThread);
				m_vec_NetTransThreads.push_back(clientTH);
				//m_mutex_trans.unlock();
				pThread->start();
				//Connect signals
				connect (clientTH,&zp_netTransThread::evt_ClientDisconnected,this,&zp_net_Engine::evt_ClientDisconnected,Qt::QueuedConnection);
				connect (clientTH,&zp_netTransThread::evt_Data_recieved,this,&zp_net_Engine::evt_Data_recieved,Qt::QueuedConnection);
				connect (clientTH,&zp_netTransThread::evt_Data_transferred,this,&zp_net_Engine::evt_Data_transferred,Qt::QueuedConnection);
				connect (clientTH,&zp_netTransThread::evt_NewClientConnected,this,&zp_net_Engine::evt_NewClientConnected,Qt::QueuedConnection);
				connect (clientTH,&zp_netTransThread::evt_ClientEncrypted,this,&zp_net_Engine::evt_ClientEncrypted,Qt::QueuedConnection);
				connect (clientTH,&zp_netTransThread::evt_SocketError,this,&zp_net_Engine::evt_SocketError,Qt::QueuedConnection);
				connect (clientTH,&zp_netTransThread::evt_Message,this,&zp_net_Engine::evt_Message,Qt::QueuedConnection);
				connect (this,&zp_net_Engine::evt_EstablishConnection,clientTH,&zp_netTransThread::incomingConnection,Qt::QueuedConnection);
				connect (this,&zp_net_Engine::evt_FireConnection,clientTH,&zp_netTransThread::startConnection,Qt::QueuedConnection);
				connect (this,&zp_net_Engine::evt_SendDataToClient,clientTH,&zp_netTransThread::SendDataToClient,Qt::QueuedConnection);
				connect (this,&zp_net_Engine::evt_KickAll,clientTH,&zp_netTransThread::KickAllClients,Qt::QueuedConnection);
				connect (this,&zp_net_Engine::evt_DeactivteImmediately,clientTH,&zp_netTransThread::DeactivateImmediately,Qt::QueuedConnection);
				connect (this,&zp_net_Engine::evt_KickClient,clientTH,&zp_netTransThread::KickClient,Qt::QueuedConnection);

				clientTH->moveToThread(pThread);
			}
		}
	}
	/**
	 * @brief This slot will be called, when an Trans-thread is about to close.
	 * Some cleanning works will be done ,all resources will be freeed.
	 * @fn zp_net_Engine::TransThreadDel
	 * @param pThreadObj The closing thread object.
	 * @return bool in normal situation, this slot is always return true.
	 */
	bool zp_net_Engine::TransThreadDel(zp_netTransThread * pThreadObj)
	{
		if (pThreadObj->CanExit()==false)
			return false;
		int nsz = m_vec_NetTransThreads.size();
		int idx = -1;
		for (int i=0;i<nsz && idx<0;i++)
		{
			if (m_vec_NetTransThreads[i]==pThreadObj)
				idx = i;
		}
		if (idx>=0 && idx <nsz)
		{
			zp_netTransThread * clientTH =  m_vec_NetTransThreads[idx];
			disconnect (clientTH,&zp_netTransThread::evt_ClientDisconnected,this,&zp_net_Engine::evt_ClientDisconnected);
			disconnect (clientTH,&zp_netTransThread::evt_Data_recieved,this,&zp_net_Engine::evt_Data_recieved);
			disconnect (clientTH,&zp_netTransThread::evt_Data_transferred,this,&zp_net_Engine::evt_Data_transferred);
			disconnect (clientTH,&zp_netTransThread::evt_NewClientConnected,this,&zp_net_Engine::evt_NewClientConnected);
			disconnect (clientTH,&zp_netTransThread::evt_ClientEncrypted,this,&zp_net_Engine::evt_ClientEncrypted);
			disconnect (clientTH,&zp_netTransThread::evt_SocketError,this,&zp_net_Engine::evt_SocketError);
			disconnect (clientTH,&zp_netTransThread::evt_Message,this,&zp_net_Engine::evt_Message);
			disconnect (this,&zp_net_Engine::evt_EstablishConnection,clientTH,&zp_netTransThread::incomingConnection);
			disconnect (this,&zp_net_Engine::evt_FireConnection,clientTH,&zp_netTransThread::startConnection);
			disconnect (this,&zp_net_Engine::evt_SendDataToClient,clientTH,&zp_netTransThread::SendDataToClient);
			disconnect (this,&zp_net_Engine::evt_KickAll,clientTH,&zp_netTransThread::KickAllClients);
			disconnect (this,&zp_net_Engine::evt_DeactivteImmediately,clientTH,&zp_netTransThread::DeactivateImmediately);
			disconnect (this,&zp_net_Engine::evt_KickClient,clientTH,&zp_netTransThread::KickClient);

			m_vec_netInternalTransThreads[idx]->quit();
			m_vec_netInternalTransThreads[idx]->wait();
			m_vec_netInternalTransThreads[idx]->deleteLater();
			m_vec_NetTransThreads[idx]->deleteLater();
			m_vec_netInternalTransThreads.remove(idx);
			m_vec_NetTransThreads.remove(idx);

		}
		return true;
	}
	/**
	 * @brief Kick all clients from server
	 * This Method is designed as an "Easy to compromise" method, which means this
	 * approach only "suggests" all these trans threads, that client should be kicked later.
	 * @fn zp_net_Engine::KickAllClients
	 */
	void zp_net_Engine::KickAllClients()
	{
		//m_mutex_trans.lock();
		int nsz = m_vec_NetTransThreads.size();
		for (int i=0;i<nsz;i++)
			emit evt_KickAll(m_vec_NetTransThreads[i]);
		//m_mutex_trans.unlock();
	}
	/**
	 * @brief :Deactive server immediately
	 *  This Method is designed as an "Mandatory" method, which means all clients will be sooner kicked out.
	 * @fn zp_net_Engine::DeactiveImmediately
	 */
	void zp_net_Engine::DeactiveImmediately()
	{
		//m_mutex_trans.lock();
		int nsz = m_vec_NetTransThreads.size();
		for (int i=0;i<nsz;i++)
			emit evt_DeactivteImmediately(m_vec_NetTransThreads[i]);
		//m_mutex_trans.unlock();
	}

	/**
	 * @brief Remove n Trans Thread(s)
	 * A thread marked as "removed" will be terminated after its last client socket exited.
	 * @fn zp_net_Engine::RemoveClientTransThreads
	 * @param nThreads how many threads will be marked as "removed"
	 * @param bSSL true means SSL threads, false means Plain
	 */
	void zp_net_Engine::RemoveClientTransThreads(int nThreads,bool bSSL)
	{

		//m_mutex_trans.lock();
		int nsz = m_vec_NetTransThreads.size();
		if (nThreads<0)
			nThreads = nsz;
		int nCount = 0;
		for (int i=0;i<nsz && nCount<nThreads;i++)
		{
			if (m_vec_NetTransThreads[i]->SSLConnection()==bSSL)
			{
				m_vec_NetTransThreads[i]->Deactivate();
				nCount ++;
			}
		}
		//Fire TransThreadDel Immediately
		this->CanExit();
		//m_mutex_trans.unlock();
	}

	/**
	 * @brief This slot send some data to a client.
	 * Because a client socket is actually belongs to a transfer-thread,
	 * this event will be re-fired as evt_SendDataToClient,  transfer-threads will
	 * @fn zp_net_Engine::SendDataToClient
	 * @param objClient The destin client
	 * @param dtarray data to be sent
	 */
	void zp_net_Engine::SendDataToClient(QObject * objClient,QByteArray   dtarray)
	{
		emit evt_SendDataToClient(objClient,dtarray);
	}

	/**
	 * @brief This slot kick client from server.
	 * @fn zp_net_Engine::KickClients
	 * @param object Client to be kicked.
	 */
	void zp_net_Engine::KickClients(QObject * object)
	{
		emit evt_KickClient(object);
	}


	/**
	 * @brief Test whether this threadpool can be safely closed.
	 * This function will return false as long as some clients is still active.
	 * @fn zp_net_Engine::CanExit
	 * @return bool true means can close, false mean can not close.
	 */
	bool zp_net_Engine::CanExit()
	{
		bool res = true;
		//m_mutex_trans.lock();
		int nsz = m_vec_NetTransThreads.size();
		for (int i=nsz-1;i>=0 && res==true;i--)
			res = TransThreadDel( m_vec_NetTransThreads[i]);
		//m_mutex_trans.unlock();

		//m_mutex_listen.lock();
		if (m_map_netListenThreads.size())
			res= false;
		//m_mutex_listen.unlock();
		return res;
	}

	/**
	 * @brief Positive connect to a server.
	 * In p2p connection, when the other side opens a listening address, this object can directly connect to the remote side.
	 * @fn zp_net_Engine::connectTo
	 * @param address the address to connect to
	 * @param nPort port to connect to
	 * @param bSSLConn if true, SSL connections will be used
	 * @return bool
	 */
	bool zp_net_Engine::connectTo (const QHostAddress & address , quint16 nPort,bool bSSLConn)
	{
		bool res= false;
		//m_mutex_trans.lock();
		int nsz = m_vec_NetTransThreads.size();
		int nMinPay = 0x7fffffff;
		int nMinIdx = -1;

		for (int i=0;i<nsz && nMinPay!=0;i++)
		{
			if (m_vec_NetTransThreads[i]->isActive()==false ||
					m_vec_NetTransThreads[i]->SSLConnection()!=bSSLConn
					)
				continue;
			int nPat = m_vec_NetTransThreads[i]->CurrentClients();

			if (nPat<nMinPay)
			{
				nMinPay = nPat;
				nMinIdx = i;
			}
			//qDebug()<<i<<" "<<nPat<<" "<<nMinIdx;
		}
		//remove old threads
		for (int i=0;i<nsz;i++)
			if (m_vec_NetTransThreads[i]->isActive()==false )
				TransThreadDel(m_vec_NetTransThreads[i]);

		if (nMinIdx>=0 && nMinIdx<nsz)
		{
			res = true;
			emit evt_FireConnection(m_vec_NetTransThreads[nMinIdx],address,nPort);
		}
		else
		{
			emit evt_Message(this,"Warning>"+QString(tr("Need Trans Thread Object for clients.")));
		}
		//m_mutex_trans.unlock();
		return res;
	}

}
