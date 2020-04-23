/**
  * @brief This is the Zoom-Pipeline Network engine by goldenhawking, 2013,2014
  */
#ifndef ZP_NET_THREADPOOL_H
#define ZP_NET_THREADPOOL_H

#include <QObject>
#include <QThread>
#include <QMap>
#include <QVector>
#include <QMutex>
#include <QThread>
#include <QSslCertificate>
#include <QSslKey>
#include <QFile>
#include "zp_netlistenthread.h"
#include "zp_nettransthread.h"

namespace ZPNetwork{

	/**
	  * @brief zp_net_Engine provides a  multi-thread listening\transfer network
	  * framework. SSL and Plain sockets are both supported.
	  */
	class zp_net_Engine : public QObject
	{
		Q_OBJECT
	public:
		explicit zp_net_Engine(int nPayLoad = 4096,QObject *parent = 0);

		//Listening Ctrl
		//Begin a listening socket at special address and port. The socket will be activated as soon as possible
		void AddListeningAddress(QString  id,const QHostAddress & address , quint16 nPort,bool bSSLConn = true);
		//Remove a listening socket at special address and port.The socket will be deactivated as soon as possible
		void RemoveListeningAddress(QString  id);
		//Remove all listening sockets
		void RemoveAllAddresses();


		//Trans Control
		//Add n client-Trans Thread(s).
		void AddClientTransThreads(int nThreads,bool bSSL=true);
		//Remove n client-Trans Thread(s).a thread marked reomved will be terminated after its last client socket exited.
		void RemoveClientTransThreads(int nThreads,bool bSSL=true);
		//Kick All Clients
		void KickAllClients();
		//Deactive Immediately
		void DeactiveImmediately();


		//when Program exit, wait for close;
		bool CanExit();

		//The status
		QStringList ListenerNames();
		int TransThreadNum();
		int TransThreadNum(bool bSSL);
		int totalClients(int idxThread);

	protected:
		int m_nPayLoad;
		//QMutex m_mutex_listen;
		//QMutex m_mutex_trans;
		//This map stores listenThreadObjects
		QMap<QString,zp_netListenThread *> m_map_netListenThreads;
		//Internal Threads to hold each listenThreadObjects' message Queue
		QMap<QString,QThread *> m_map_netInternalListenThreads;

		//This map stores ClientTransThreadObjects
		QVector<zp_netTransThread *> m_vec_NetTransThreads;
		//Internal Threads to hold each m_map_NetTransThreads' message Queue
		QVector<QThread *> m_vec_netInternalTransThreads;
		bool TransThreadDel(zp_netTransThread * pThreadObj);
	signals:

		void evt_Message(QObject * pSource,QString );
		//The socket error message
		void evt_SocketError(QObject * senderSock ,QAbstractSocket::SocketError socketError);
		//this event indicates new client connected.
		void evt_NewClientConnected(QObject * /*clientHandle*/);
		//SSL Connections OK
		void evt_ClientEncrypted(QObject * client);
		//this event indicates a client disconnected.
		void evt_ClientDisconnected(QObject * /*clientHandle*/);
		//some data arrival
		void evt_Data_recieved(QObject *  /*clientHandle*/,QByteArray  /*datablock*/ );
		//a block of data has been successfuly sent
		void evt_Data_transferred(QObject *   /*clientHandle*/,qint64 /*bytes sent*/);

		//Internal Message for ctrl.------------------------------------
		//Listen Control
		void startListen(QString  id);
		void stopListen(QString  id);
		void evt_EstablishConnection(QObject * threadid,qintptr socketDescriptor);
		void evt_FireConnection(QObject * threadid,const QHostAddress & hostAddr, quint16 port);
		//Trans Control,for intenal thread usage
		void evt_SendDataToClient(QObject * objClient,QByteArray   dtarray);
		void evt_KickClient(QObject *);
		void evt_DeactivteImmediately(zp_netTransThread *);
		void evt_KickAll(zp_netTransThread *);

	protected slots:
		void on_New_Arrived_Client(qintptr socketDescriptor);
		void on_ListenClosed(QString  id);
	public slots:
		//Call this function to send data to client
		void SendDataToClient(QObject * objClient,QByteArray   dtarray);
		//Close client Immediatele
		void KickClients(QObject * object);

		//Possive Connection Methods
		bool connectTo (const QHostAddress & address , quint16 nPort,bool bSSLConn);

	};
}
#endif // ZP_NET_THREADPOOL_H
