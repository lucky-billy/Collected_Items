#ifndef ZP_CLUSTERTERM_H
#define ZP_CLUSTERTERM_H

#include <QObject>
#include <QHostAddress>
#include <QList>
#include <QMutex>
#include <QMap>
#include "../network/zp_net_threadpool.h"
#include "../pipeline/zp_pipeline.h"
#include "../pipeline/zp_pltaskbase.h"
#include <functional>
namespace ZP_Cluster{
	class zp_ClusterNode;
	//!this class enable server processes can
	//! communicate with each other.
	class zp_ClusterTerm : public QObject
	{
		Q_OBJECT
	public:
		explicit zp_ClusterTerm(QString  name,QObject *parent = 0);


		/**
		 * The factory enables user-defined sub-classes inherits from zp_ClusterNode
		 * Using SetNodeFactory , set your own allocate method.
		 */
		void SetNodeFactory(std::function<
							zp_ClusterNode * (
								zp_ClusterTerm * /*pTerm*/,
								QObject * /*psock*/,
								QObject * /*parent*/)>
							);

		//cluster status
		ZPNetwork::zp_net_Engine * netEng();
		ZPTaskEngine::zp_pipeline * taskEng();
		bool canExit();

		//Server Group Mapping
	public:
		zp_ClusterNode * SvrNodeFromName(QString );
		zp_ClusterNode * SvrNodeFromSocket(QObject *);
		QStringList SvrNames();
		quint32 remoteClientNums(QString  name);

		QHostAddress SvrLANAddr(QString  name);
		int SvrLANPort(QString  name);
		QHostAddress SvrPubAddr(QString  name);
		int SvrPubPort(QString  name);


		//Client Num set, for cross-svr balance
		void setClientNums(quint32 nnum);
		quint32 clientNums();
		//Propties
	public:
		//properties.
		QString setName(QString  s);
		QString name();

		//LAN Address, for other servers
		QHostAddress LANAddr();
		int LANPort();
		QHostAddress setLANAddr(QHostAddress addr);
		int setLANPort(int port);

		//Publish Address, for clients.
		QHostAddress PublishAddr();
		int PublishPort();
		QHostAddress setPublishAddr(QHostAddress addr);
		int setPublishPort(int port);

		//Re-Direct Test.
		QString minPayloadServer(quint8 bufAddresses[/*64*/],quint16 * pnPort);

		int heartBeatingThrdHold() ;
		void setHeartBeatingThrd(const int n);
		bool regisitNewServer(zp_ClusterNode *);
		void BroadcastServers();
		void SendHeartBeatings();
	protected:
		std::function<zp_ClusterNode * (
				zp_ClusterTerm * /*pTerm*/,
				QObject * /*psock*/,
				QObject * /*parent*/)> m_factory;
		zp_ClusterNode * default_factory(
						zp_ClusterTerm * /*pTerm*/,
						QObject * /*psock*/,
						QObject * /*parent*/);
	protected:

		int m_nHeartBeatingTime;
		quint32 m_nClientNums; //the clients this server now connected.
		QString m_strTermName;//the Terminal's name
		QHostAddress m_addrLAN;	//The LAN address for other server-terms to connect to
		int m_nPortLAN;//The LAN port for other server-terms to connect to
		QHostAddress m_addrPub;	//The Publish address for clients to connect to
		int m_nPortPub;//The Publish port for clients to connect to
		ZPNetwork::zp_net_Engine * m_pClusterNet;
		ZPTaskEngine::zp_pipeline * m_pClusterEng;
		//This list hold dead nodes that still in task queue,avoiding crash
		QList<zp_ClusterNode *> m_nodeToBeDel;
		//important hashes. server name to socket, socket to server name
		QMutex m_hash_mutex;
		QMap<QString , zp_ClusterNode *> m_hash_Name2node;
		QMap<QObject *,zp_ClusterNode *> m_hash_sock2node;
	signals:

		void evt_Message(QObject * ,QString );
		//The socket error message
		void evt_SocketError(QObject * senderSock ,QAbstractSocket::SocketError socketError);
		//this event indicates new svr successfully hand-shaked.
		void evt_NewSvrConnected(QString /*svrHandle*/);
		//this event indicates a client disconnected.
		void evt_NewSvrDisconnected(QString /*svrHandle*/);
		//some data arrival
		void evt_RemoteData_recieved(QString /*svrHandle*/,QByteArray  /*svrHandle*/ );
		//a block of data has been successfuly sent
		void evt_RemoteData_transferred(QObject *  /*svrHandle*/,qint64 /*bytes sent*/);

	protected slots:
		//this event indicates new client connected.
		void on_evt_NewClientConnected(QObject * /*clientHandle*/);
		//this event indicates new client encrypted.
		void on_evt_ClientEncrypted(QObject * /*clientHandle*/);
		//this event indicates a client disconnected.
		void on_evt_ClientDisconnected(QObject * /*clientHandle*/);
		//some data arrival
		void on_evt_Data_recieved(QObject *  /*clientHandle*/,QByteArray  /*datablock*/ );
		//a block of data has been successfuly sent
		void on_evt_Data_transferred(QObject *   /*clientHandle*/,qint64 /*bytes sent*/);

	public slots:
		//!Start listen, this term can be connected by newly joined terms in future.
		void StartListen(const QHostAddress &addr, int nPort);
		//!Join cluster, using existing term (addr:nPort)
		//!as soon as connection established, more existing terms will be sent to this term,
		//!an p2p connection will start
		bool JoinCluster(const QHostAddress &addr, int nPort,bool bSSL=false);
		void  KickDeadClients();

		//Send Data to Server
		void SendDataToRemoteServer(QString  svrName,QByteArray );

	};
}
#endif // ZP_CLUSTERTERM_H
