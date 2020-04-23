#ifndef ZP_CLUSTERNODE_H
#define ZP_CLUSTERNODE_H

#include <QObject>
#include <QHostAddress>
#include <QDateTime>
#include "../pipeline/zp_pltaskbase.h"
#include "cross_svr_messages.h"
namespace ZP_Cluster{
	class zp_ClusterTerm;
	/**
	 * @brief This class stand for a remote server.
	 * when local server establish a connection between itself and remote svr,
	 * a zp_ClusterNode object will be created.
	 */
	class zp_ClusterNode : public ZPTaskEngine::zp_plTaskBase
	{
		Q_OBJECT
	public:
		explicit zp_ClusterNode(zp_ClusterTerm * pTerm, QObject * psock,QObject *parent = 0);
		int run();
		bool bTermSet;
		//push new binary data into queue
		int push_new_data(const  QByteArray &  dtarray);
		void CheckHeartBeating();

	protected:
		//!deal at most m_nMessageBlockSize messages per deal_message();
		static const int m_nMessageBlockSize = 8;
		//!deal one message, affect m_currentRedOffset,m_currentMessageSize,m_currentHeader
		//!return bytes Used.
		int filter_message(QByteArray , int offset);
		//!in Trans-Layer, it does nothing.
		int deal_current_message_block();
		//!virtual functions, dealing with the user-defined operations.
		virtual bool deal_user_data(QByteArray );

		QDateTime lastActiveTime();

	public:
		QString termName();
		QHostAddress addrLAN();
		int portLAN() ;
		QHostAddress addrPub();
		int portPub() ;
		QObject * sock() ;

		//!Messages
	public:
		void SendHelloPackage();
		quint32 clientNums();
	protected:
		zp_ClusterTerm * m_pTerm;
		//Client socket handle of this connection
		QObject * m_pSock;
		//the data members.
		QString m_strTermName;		//the Terminal's name
		QHostAddress m_addrLAN;	//The LAN address for other terms to connect to
		int m_nPortLAN;			//The LAN port for other terms to connect to
		QHostAddress m_addrPub;	//The Publish address for clients to connect to
		int m_nPortPub;			//The Publish port for clients to connect to
		//Data Process
		//The raw data queue and its mutex
		QList<QByteArray> m_list_RawData;
		QMutex m_mutex_rawData;
		//The current Read Offset, from m_list_RawData's beginning
		int m_currentReadOffset;
		//Current Message Offset, according to m_currentHeader
		int m_currentMessageSize;
		//Current un-procssed message block.for large blocks,
		//this array will be re-setted as soon as some part of data has been
		//dealed, eg, send a 200MB block, the 200MB data will be splitted into pieces
		QByteArray m_currentBlock;

		CROSS_SVR_MSG::tag_header m_currentHeader;

		QDateTime m_last_Report;

		quint32 m_nRemoteClientNums;
	signals:
		void evt_SendDataToClient(QObject * objClient,QByteArray   dtarray);
		void evt_close_client(QObject * objClient);
		void evt_connect_to(const QHostAddress & address , quint16 nPort,bool bSSLConn);
		void evt_Message (QObject * psource,QString );

		//Notify Messages
		void evt_NewSvrConnected(QString  /*svrHandle*/);
		//some data arrival
		void evt_RemoteData_recieved(QString   /*svrHandle*/,QByteArray  /*svrHandle*/ );
	};
}
#endif // ZP_CLUSTERNODE_H
