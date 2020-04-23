#include "st_client_table.h"
#include "st_clientnode_applayer.h"
#include <assert.h>
#include "st_cross_svr_node.h"
#include "st_cross_svr_msg.h"
#include <functional>
#include <QList>
namespace ExampleServer{
	using namespace std::placeholders;
	st_client_table::st_client_table(
			ZPNetwork::zp_net_Engine * NetEngine,
			ZPTaskEngine::zp_pipeline * taskeng,
			ZPDatabase::DatabaseResource * pDb,
			ZP_Cluster::zp_ClusterTerm * pCluster,
			QObject *parent) :
		QObject(parent)
	  ,m_pThreadEngine(NetEngine)
	  ,m_pTaskEngine(taskeng)
	  ,m_pDatabaseRes(pDb)
	  ,m_pCluster(pCluster)
	  ,m_nBalanceMax(1024)
	{

		m_nHeartBeatingDeadThrd = 180;
		connect (m_pThreadEngine,&ZPNetwork::zp_net_Engine::evt_NewClientConnected,this,&st_client_table::on_evt_NewClientConnected,Qt::QueuedConnection);
		connect (m_pThreadEngine,&ZPNetwork::zp_net_Engine::evt_ClientEncrypted,this,&st_client_table::on_evt_ClientEncrypted,Qt::QueuedConnection);
		connect (m_pThreadEngine,&ZPNetwork::zp_net_Engine::evt_ClientDisconnected,this,&st_client_table::on_evt_ClientDisconnected,Qt::QueuedConnection);
		connect (m_pThreadEngine,&ZPNetwork::zp_net_Engine::evt_Data_recieved,this,&st_client_table::on_evt_Data_recieved,Qt::QueuedConnection);
		connect (m_pThreadEngine,&ZPNetwork::zp_net_Engine::evt_Data_transferred,this,&st_client_table::on_evt_Data_transferred,Qt::QueuedConnection);

		connect (m_pCluster,&ZP_Cluster::zp_ClusterTerm::evt_NewSvrConnected,this,&st_client_table::on_evt_NewSvrConnected,Qt::QueuedConnection);
		connect (m_pCluster,&ZP_Cluster::zp_ClusterTerm::evt_NewSvrDisconnected,this,&st_client_table::on_evt_NewSvrDisconnected,Qt::QueuedConnection);
		connect (m_pCluster,&ZP_Cluster::zp_ClusterTerm::evt_RemoteData_recieved,this,&st_client_table::on_evt_RemoteData_recieved,Qt::QueuedConnection);
		connect (m_pCluster,&ZP_Cluster::zp_ClusterTerm::evt_RemoteData_transferred,this,&st_client_table::on_evt_RemoteData_transferred,Qt::QueuedConnection);
		m_pCluster->SetNodeFactory(
					std::bind(&st_client_table::cross_svr_node_factory,
							  this,
							  _1,_2,_3)
					);
	}
	void st_client_table::setBalanceMax(int nmax)
	{
		m_nBalanceMax = nmax;
	}

	int st_client_table::balanceMax()
	{
		return m_nBalanceMax;
	}


	int st_client_table::heartBeatingThrd()
	{
		return m_nHeartBeatingDeadThrd;
	}
	void st_client_table::setHeartBeatingThrd(int h)
	{
		m_nHeartBeatingDeadThrd = h;
	}

	//Database and disk resources
	QString st_client_table::Database_UserAcct()
	{
		return m_strDBName_useraccount;
	}
	void st_client_table::setDatabase_UserAcct(QString  s)
	{
		m_strDBName_useraccount = s;
	}
	QString st_client_table::Database_Event()
	{
		return m_strDBName_event;
	}
	void st_client_table::setDatabase_Event(QString  s)
	{
		m_strDBName_event = s;
	}
	QString st_client_table::largeFileFolder()
	{
		return m_largeFileFolder;
	}
	void st_client_table::setLargeFileFolder(QString  s)
	{
		m_largeFileFolder = s;
	}

	ZPDatabase::DatabaseResource * st_client_table::dbRes()
	{
		return m_pDatabaseRes;
	}

	st_client_table::~st_client_table()
	{
	}
	void  st_client_table::KickDeadClients()
	{
		m_hash_mutex.lock();
		for (std::unordered_map<QObject *,st_clientNode_baseTrans *>::iterator p =m_hash_sock2node.begin();
			 p!=m_hash_sock2node.end();p++)
		{
			(*p).second->CheckHeartBeating();
		}
		m_hash_mutex.unlock();
	}
	bool st_client_table::regisitClientUUID(st_clientNode_baseTrans * c)
	{
		if (c->uuidValid()==false)
			return false;
		m_hash_mutex.lock();
		m_hash_uuid2node[c->uuid()] = c;
		m_hash_mutex.unlock();
		broadcast_client_uuid(c->uuid(),true);
		return true;
	}

	st_clientNode_baseTrans *  st_client_table::clientNodeFromUUID(quint32 uuid)
	{
		m_hash_mutex.lock();
		if (m_hash_uuid2node.find(uuid)!=m_hash_uuid2node.end())
		{
			m_hash_mutex.unlock();
			return m_hash_uuid2node[uuid];
		}
		m_hash_mutex.unlock();

		return NULL;
	}

	st_clientNode_baseTrans *  st_client_table::clientNodeFromSocket(QObject * sock)
	{
		m_hash_mutex.lock();
		if (m_hash_sock2node.find(sock)!=m_hash_sock2node.end())
		{
			m_hash_mutex.unlock();
			return m_hash_sock2node[sock];
		}
		m_hash_mutex.unlock();
		return NULL;

	}
	//this event indicates new client encrypted.
	void st_client_table::on_evt_ClientEncrypted(QObject * clientHandle)
	{
		bool nHashContains = false;
		st_clientNode_baseTrans * pClientNode = 0;
		m_hash_mutex.lock();
		nHashContains = (m_hash_sock2node.find(clientHandle)!=m_hash_sock2node.end())?true:false;
		if (false==nHashContains)
		{
			st_clientNode_baseTrans * pnode = new st_clientNodeAppLayer(this,clientHandle,0);
			//using queued connection of send and revieve;
			connect (pnode,&st_clientNode_baseTrans::evt_SendDataToClient,m_pThreadEngine,&ZPNetwork::zp_net_Engine::SendDataToClient,Qt::QueuedConnection);
			connect (pnode,&st_clientNode_baseTrans::evt_close_client,m_pThreadEngine,&ZPNetwork::zp_net_Engine::KickClients,Qt::QueuedConnection);
			connect (pnode,&st_clientNode_baseTrans::evt_Message,this,&st_client_table::evt_Message,Qt::QueuedConnection);
			m_hash_sock2node[clientHandle] = pnode;
			nHashContains = true;
			pClientNode = pnode;
		}
		else
		{
			pClientNode =  m_hash_sock2node[clientHandle];
		}
		m_hash_mutex.unlock();
		assert(nHashContains!=0 && pClientNode !=0);
	}

	//this event indicates new client connected.
	void  st_client_table::on_evt_NewClientConnected(QObject * clientHandle)
	{
		bool nHashContains = false;
		st_clientNode_baseTrans * pClientNode = 0;
		m_hash_mutex.lock();
		nHashContains = (m_hash_sock2node.find(clientHandle)!=m_hash_sock2node.end())?true:false;
		if (false==nHashContains)
		{
			st_clientNode_baseTrans * pnode = new st_clientNodeAppLayer(this,clientHandle,0);
			//using queued connection of send and revieve;
			connect (pnode,&st_clientNode_baseTrans::evt_SendDataToClient,m_pThreadEngine,&ZPNetwork::zp_net_Engine::SendDataToClient,Qt::QueuedConnection);
			connect (pnode,&st_clientNode_baseTrans::evt_close_client,m_pThreadEngine,&ZPNetwork::zp_net_Engine::KickClients,Qt::QueuedConnection);
			connect (pnode,&st_clientNode_baseTrans::evt_Message,this,&st_client_table::evt_Message,Qt::QueuedConnection);
			m_hash_sock2node[clientHandle] = pnode;
			nHashContains = true;
			pClientNode = pnode;
		}
		else
		{
			pClientNode =  m_hash_sock2node[clientHandle];
		}
		m_hash_mutex.unlock();
		assert(nHashContains!=0 && pClientNode !=0);
	}

	//this event indicates a client disconnected.
	void  st_client_table::on_evt_ClientDisconnected(QObject * clientHandle)
	{
		bool nHashContains  = false;
		st_clientNode_baseTrans * pClientNode = 0;
		m_hash_mutex.lock();
		nHashContains = (m_hash_sock2node.find(clientHandle)!=m_hash_sock2node.end())?true:false;
		if (nHashContains)
			pClientNode =  m_hash_sock2node[clientHandle];
		if (pClientNode)
		{
			m_hash_sock2node.erase(clientHandle);
			if (pClientNode->uuidValid())
			{
				//This is important. some time m_hash_sock2node and m_hash_uuid2node, same uuid has different socket.
				if (m_hash_uuid2node.find(pClientNode->uuid())!=m_hash_uuid2node.end())
					if (m_hash_uuid2node[pClientNode->uuid()]==pClientNode)
					{
						m_hash_uuid2node.erase(pClientNode->uuid());
						broadcast_client_uuid(pClientNode->uuid(),false);
					}
			}

			pClientNode->bTermSet = true;
			disconnect (pClientNode,&st_clientNode_baseTrans::evt_SendDataToClient,m_pThreadEngine,&ZPNetwork::zp_net_Engine::SendDataToClient);
			disconnect (pClientNode,&st_clientNode_baseTrans::evt_close_client,m_pThreadEngine,&ZPNetwork::zp_net_Engine::KickClients);
			disconnect (pClientNode,&st_clientNode_baseTrans::evt_Message,this,&st_client_table::evt_Message);

			m_nodeToBeDel.push_back(pClientNode);
			//qDebug()<<QString("%1(ref %2) Node Push in queue.\n").arg((unsigned int)pClientNode).arg(pClientNode->ref());
		}
		m_hash_mutex.unlock();

		//Try to delete objects
		QList <st_clientNode_baseTrans *> toBedel;
		foreach(st_clientNode_baseTrans * pdelobj,m_nodeToBeDel)
		{
			if (pdelobj->ref() ==0)
				toBedel.push_back(pdelobj);
			else
			{
				//qDebug()<<QString("%1(ref %2) Waiting in del queue.\n").arg((unsigned int)pdelobj).arg(pdelobj->ref());
			}
		}
		foreach(st_clientNode_baseTrans * pdelobj,toBedel)
		{
			m_nodeToBeDel.removeAll(pdelobj);

			//qDebug()<<QString("%1(ref %2) deleting.\n").arg((unsigned int)pdelobj).arg(pdelobj->ref());
			pdelobj->deleteLater();

		}

	}

	//some data arrival
	void  st_client_table::on_evt_Data_recieved(QObject *  clientHandle,QByteArray  datablock )
	{
		//Push Clients to nodes if it is not exist
		bool nHashContains = false;
		st_clientNode_baseTrans * pClientNode = 0;
		m_hash_mutex.lock();
		nHashContains = (m_hash_sock2node.find(clientHandle)!=m_hash_sock2node.end())?true:false;
		if (false==nHashContains)
		{
			st_clientNode_baseTrans * pnode = new st_clientNodeAppLayer(this,clientHandle,0);
			//using queued connection of send and revieve;
			connect (pnode,&st_clientNode_baseTrans::evt_SendDataToClient,m_pThreadEngine,&ZPNetwork::zp_net_Engine::SendDataToClient,Qt::QueuedConnection);
			connect (pnode,&st_clientNode_baseTrans::evt_close_client,m_pThreadEngine,&ZPNetwork::zp_net_Engine::KickClients,Qt::QueuedConnection);
			connect (pnode,&st_clientNode_baseTrans::evt_Message,this,&st_client_table::evt_Message,Qt::QueuedConnection);
			m_hash_sock2node[clientHandle] = pnode;
			nHashContains = true;
			pClientNode = pnode;
		}
		else
		{
			pClientNode =  m_hash_sock2node[clientHandle];
		}

		assert(nHashContains!=0 && pClientNode !=0);


		int nblocks =  pClientNode->push_new_data(datablock);
		if (nblocks<=1)
			m_pTaskEngine->pushTask(pClientNode);
		m_hash_mutex.unlock();

	}

	//a block of data has been successfuly sent
	void  st_client_table::on_evt_Data_transferred(QObject *   /*clientHandle*/,qint64 /*bytes sent*/)
	{

	}

	//this event indicates new svr successfully hand-shaked.
	void st_client_table::on_evt_NewSvrConnected(QString  svrHandle)
	{
		//Send All Client UUIDs to new Svr
		m_hash_mutex.lock();
		QList<quint32> uuids;
		for(std::unordered_map<quint32,st_clientNode_baseTrans *>::iterator p =  m_hash_uuid2node.begin();
			p!=m_hash_uuid2node.end();p++)
			uuids.push_back((*p).first);
		int nNodeSz = uuids.size();
		if (nNodeSz>0)
		{
			int nMsgLen = sizeof(EXAMPLE_CROSSSVR_MSG::tag_msgHearder) +  nNodeSz * sizeof(quint32);
			QByteArray array(nMsgLen,0);
			EXAMPLE_CROSSSVR_MSG * pMsg = (EXAMPLE_CROSSSVR_MSG *) array.data();
			pMsg->header.Mark = 0x4567;
			pMsg->header.version = 1;
			pMsg->header.messageLen = nNodeSz * sizeof(quint32);
			pMsg->header.mesageType = 0x01;
			int ct = -1;
			foreach (quint32 uuid,uuids)
				pMsg->payload.uuids[++ct] = uuid;
			m_pCluster->SendDataToRemoteServer(svrHandle,array);
		}
		m_hash_mutex.unlock();
		emit evt_Message(this,tr("Send Initial UUIDs to Remote Svr:") + svrHandle);
	}
	void st_client_table::broadcast_client_uuid(quint32 uuid, bool bActive)
	{
		QStringList svrs = m_pCluster->SvrNames();
		if (svrs.empty()==false)
		{
			int nMsgLen = sizeof(EXAMPLE_CROSSSVR_MSG::tag_msgHearder) +  sizeof(quint32);
			QByteArray array(nMsgLen,0);
			EXAMPLE_CROSSSVR_MSG * pMsg = (EXAMPLE_CROSSSVR_MSG *) array.data();
			pMsg->header.Mark = 0x4567;
			pMsg->header.version = 1;
			pMsg->header.messageLen = sizeof(quint32);
			pMsg->header.mesageType = bActive==true?0x01:0x02;
			pMsg->payload.uuids[0] = uuid;
			foreach (QString svr,svrs)
				m_pCluster->SendDataToRemoteServer(svr,array);
		}
	}
	bool st_client_table::SendToNode(quint32 uuid, QByteArray  msg)
	{
		bool bres = false;
		m_hash_mutex.lock();
		if (m_hash_uuid2node.find(uuid)!=m_hash_uuid2node.end())
		{
			st_clientNode_baseTrans * pAppLayer = qobject_cast<st_clientNode_baseTrans *>(m_hash_uuid2node[uuid]);
			if (pAppLayer)
			{
				this->m_pThreadEngine->SendDataToClient(pAppLayer->sock(),msg);
				bres = true;
			}
		}
		m_hash_mutex.unlock();
		//emit evt_Message(this,tr("Recieved remote user-data to uuid:%1,DATA:%2").arg(uuid).arg(QString("HEX")+QString(msg.toHex())));
		return bres;
	}

	//this event indicates a client disconnected.
	void st_client_table::on_evt_NewSvrDisconnected(QString  svrHandle)
	{
		//remove all client-maps belongs to this server.
		this->cross_svr_del_uuids(svrHandle,NULL,0);
		emit evt_Message(this,"Svr DisConnected. " + svrHandle);
	}

	//some data arrival
	void st_client_table::on_evt_RemoteData_recieved(QString  svrHandle,QByteArray  array )
	{
		emit evt_Message(this,tr("Recieved %1 bytes Msg from ").arg(array.length()) + svrHandle);
	}

	//a block of data has been successfuly sent
	void st_client_table::on_evt_RemoteData_transferred(QObject *  /*svrHandle*/,qint64 /*bytes sent*/)
	{

	}
	ZP_Cluster::zp_ClusterNode * st_client_table::cross_svr_node_factory(
			ZP_Cluster::zp_ClusterTerm * pTerm,
			QObject * psock,
			QObject * parent)
	{
		st_cross_svr_node * pNode = new st_cross_svr_node(pTerm,psock,parent);
		pNode->setClientTable(this);
		//connect (pNode,&st_cross_svr_node::evt_SendToNode,this,&st_client_table::SendToNode,Qt::QueuedConnection);
		return pNode;
	}
	//reg new uuids in m_hash_remoteClient2SvrName
	void st_client_table::cross_svr_add_uuids(QString  svrname,quint32 * pUUIDs, int nUUIDs)
	{
		m_mutex_cross_svr_map.lock();
		for (int i=0;i<nUUIDs;i++)
			m_hash_remoteClient2SvrName[pUUIDs[i]] = svrname;
		m_mutex_cross_svr_map.unlock();
		emit evt_Message(this,tr("Recieved remote %1 client uuid(s) from svr ").arg(nUUIDs) + svrname);
	}

	//del uuids in m_hash_remoteClient2SvrName, pUUIDs =0 means del all uuids belong to svrname
	void st_client_table::cross_svr_del_uuids(QString  svrname,quint32 * pUUIDs , int nUUIDs)
	{
		m_mutex_cross_svr_map.lock();
		if (pUUIDs==NULL)
		{
			QList<quint32> keys;
			for(std::unordered_map<quint32,QString>::iterator p =
				m_hash_remoteClient2SvrName.begin();
				p!=m_hash_remoteClient2SvrName.end();p++)
			{
				if ((*p).second == svrname )
					keys.push_back((*p).first);
			}
			foreach (quint32 key, keys)
			{
				m_hash_remoteClient2SvrName.erase(key);
			}
		}
		else
		{
			for (int i=0;i<nUUIDs;i++)
				m_hash_remoteClient2SvrName.erase(pUUIDs[i]);
		}
		m_mutex_cross_svr_map.unlock();
		emit evt_Message(this,tr("Removed remote %1 client uuid(s) from svr ").arg(nUUIDs) + svrname);
	}
	void st_client_table::cross_svr_send_data(QString  svrname,QByteArray  arr)
	{
		int nMsgLen = sizeof(EXAMPLE_CROSSSVR_MSG::tag_msgHearder);
		QByteArray array(nMsgLen,0);
		EXAMPLE_CROSSSVR_MSG * pMsg = (EXAMPLE_CROSSSVR_MSG *) array.data();
		pMsg->header.Mark = 0x4567;
		pMsg->header.version = 1;
		pMsg->header.messageLen = arr.size();
		pMsg->header.mesageType = 0x03;
		array.append(arr);
		m_pCluster->SendDataToRemoteServer(svrname,array);
	}

	QString  st_client_table::cross_svr_find_uuid(quint32 uuid)
	{
		QString svr = "";
		m_mutex_cross_svr_map.lock();
		if (m_hash_remoteClient2SvrName.find(uuid)!=m_hash_remoteClient2SvrName.end())
			svr = m_hash_remoteClient2SvrName[uuid];
		m_mutex_cross_svr_map.unlock();
		return svr;
	}

	bool st_client_table::NeedRedirect(quint8 bufAddresses[/*64*/],quint16 * pnPort)
	{
		if (m_pCluster->clientNums()<m_nBalanceMax)
			return false;
		QString strServerName = m_pCluster->minPayloadServer(bufAddresses,pnPort);
		if (strServerName==m_pCluster->name())
			return false;
		return true;
	}

}

