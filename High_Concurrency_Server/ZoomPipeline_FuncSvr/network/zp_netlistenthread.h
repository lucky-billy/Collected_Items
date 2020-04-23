/**
  * @breif zp_netListenThread is a controller class, running in a independance thread.
*/
#ifndef ZP_NETLISTENTHREAD_H
#define ZP_NETLISTENTHREAD_H

#include <QObject>
#include <QHostAddress>
#include "zp_tcpserver.h"
namespace ZPNetwork{
	/**
	 * @brief this class manages a listening-address.The object of this class will
	 * be moved into a thread. this thread  hold the message-queue for
	 * zp_netListenThread object. Wait and Accept method will run in
	 * its message-queue thread.
	 */
	class zp_netListenThread : public QObject
	{
		Q_OBJECT
	protected:
		ZP_TcpServer * m_tcpServer;
		QString m_id;   //The listen ID
		QHostAddress m_address;
		quint16 m_port;
		bool m_bSSLConn;
	public:
		explicit zp_netListenThread(QString  id, QHostAddress address ,quint16 port,bool bSSL = true,QObject *parent = 0);
		bool bSSLConn();
	signals:
		void evt_Message(QObject *,QString );
		void evt_ListenClosed(QString );
		//This message will tell thread pool, a new incoming connection has arrived.
		void evt_NewClientArrived(qintptr socketDescriptor);
	public slots:
		void startListen(QString  id);
		void stopListen(QString  id);
	};
}
#endif // ZP_NETLISTENTHREAD_H
