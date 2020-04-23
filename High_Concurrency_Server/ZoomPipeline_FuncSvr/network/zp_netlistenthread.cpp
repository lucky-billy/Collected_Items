#include "zp_netlistenthread.h"
namespace ZPNetwork{
	zp_netListenThread::zp_netListenThread(QString  id, QHostAddress address ,quint16 port,bool bSSL,QObject *parent)
		:QObject(parent)
		,m_tcpServer(0)
		,m_id(id)
		,m_address(address)
		,m_port(port)
		,m_bSSLConn (bSSL)
	{

	}

	bool zp_netListenThread::bSSLConn()
	{
		return m_bSSLConn;
	}

	/**
	 * @brief The slot recieves startListen command from its parent
	 * zp_net_Engine object. the Qt's signal-and-slots system
	 * likes a broadcast system, so, a string id should be used to
	 * tell each listening zp_netListenThread, only this id should be
	 * notified.
	 *
	 * @param id the Listening ID that zp_net_Engine really
	 * want to operate.
	 */
	void zp_netListenThread::startListen(QString  id)
	{
		if (id==m_id)
		{
			if (!m_tcpServer)
			{
				m_tcpServer = new ZP_TcpServer(this);
				connect (m_tcpServer,&ZP_TcpServer::evt_NewClientArrived,this,&zp_netListenThread::evt_NewClientArrived,Qt::QueuedConnection);
				if (false==m_tcpServer->listen(m_address,m_port))
				{
					disconnect (m_tcpServer,&ZP_TcpServer::evt_NewClientArrived,this,&zp_netListenThread::evt_NewClientArrived);
					emit evt_Message(this,"Error>"+QString(tr("Can not start listen!")));
					m_tcpServer->deleteLater();
					m_tcpServer = 0;
					//Close this thread.
					emit evt_ListenClosed(m_id);
				}
			}
		}
	}

	/**
	 * @brief this method is very similar to zp_netListenThread::startListen(QString  id)
	 *
	 * @param id the Listening ID that zp_net_Engine really want to operate.
	 */
	void zp_netListenThread::stopListen(QString  id)
	{
		if (id==m_id)
		{
			if (m_tcpServer)
			{
				disconnect (m_tcpServer,&ZP_TcpServer::evt_NewClientArrived,this,&zp_netListenThread::evt_NewClientArrived);
				m_tcpServer->close();
				m_tcpServer->deleteLater();
				m_tcpServer = 0;
				//Close this thread.
				emit evt_Message(this,"Info>"+QString(tr("Listen Closed!")));
				emit evt_ListenClosed(m_id);
			}
		}
	}
}
