#include "st_clientnode_applayer.h"
#include "st_client_table.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <assert.h>
#include <QCoreApplication>
#include <QMutexLocker>
#include <QSettings>
#include <QSqlError>
#include <QSet>
namespace ExampleServer{

	bool st_clientNodeAppLayer::LoginClient()
	{
		const EXAMPLE_MSG_APP * pAppLayer =
				(const EXAMPLE_MSG_APP *)(
					((const char *)(m_currentBlock.constData()))
					+sizeof(EXAMPLE_TRANS_MSG)-1);
		int nAppLen = m_currentBlock.length()- (sizeof(EXAMPLE_TRANS_MSG)-1)- sizeof(tag_example_app_layer::tag_app_layer_header) - sizeof (quint32);
		QString strPasswd ;
		quint32 UserID = pAppLayer->MsgUnion.msg_ClientLoginReq.user_id;

		int nSwim = 0;
		while (  nSwim < 65 && nSwim <nAppLen && pAppLayer->MsgUnion.msg_ClientLoginReq.Passwd[nSwim]!=0 )
			strPasswd+= pAppLayer->MsgUnion.msg_ClientLoginReq.Passwd[nSwim++];


		//form Msgs
		quint16 nMsgLen = sizeof(EXAMPLE_MSG_APP::tag_app_layer_header)
				+sizeof(stMsg_ClientLoginRsp);
		QByteArray array(sizeof(EXAMPLE_TRANS_MSG) + nMsgLen - 1,0);
		char * ptr = array.data();
		EXAMPLE_TRANS_MSG * pMsg = (EXAMPLE_TRANS_MSG *)ptr;
		EXAMPLE_MSG_APP * pApp = (EXAMPLE_MSG_APP *)(((unsigned char *)
														  (ptr))+sizeof(EXAMPLE_TRANS_MSG)-1
														 );
		pMsg->Mark = 0x55AA;
		pMsg->source_id = (quint32)((quint64)(m_currentHeader.destin_id) & 0xffffffff );

		pMsg->destin_id = (quint32)((quint64)(m_currentHeader.source_id) & 0xffffffff );;

		pMsg->data_length = nMsgLen;

		pApp->header.MsgType = 0x7FFE;

		stMsg_ClientLoginRsp & reply = pApp->MsgUnion.msg_ClientLoginRsp;

		//Check the database, find current equipment info
		QSqlDatabase db = m_pClientTable->dbRes()->databse(m_pClientTable->Database_UserAcct());

		reply.DoneCode = 3;
		//strcpy(reply.TextInfo,"Unknown error");
		if (db.isValid()==true && db.isOpen()==true )
		{
			QSqlQuery query(db);

			QString sql = "select user_id,password from users where user_id = ? and password = ?;";
			query.prepare(sql);
			query.addBindValue(UserID);
			query.addBindValue(strPasswd);

			if (true==query.exec())
			{
				if (query.next())
				{
					bool bOk = false;
					quint32 ncurrid = query.value(1).toUInt(&bOk);
					if (bOk==true)
					{
						if (this->bIsValidUserId(ncurrid))
						{

							reply.DoneCode = 0;
							reply.UserID = ncurrid;
							m_bLoggedIn = true;
							m_bUUIDRecieved = true;
							m_uuid = ncurrid;
							m_pClientTable->regisitClientUUID(this);
							if (false == loadRelations())
							{
								reply.DoneCode = 3;
							}
							//Cluster-Balance.
							if (m_pClientTable->NeedRedirect(reply.Address_Redirect,&reply.port_Redirect))
							{
								reply.DoneCode = 1;
							}
						}
						// else
						// strcpy(reply.TextInfo,"UserID Is Invalid.Accunt locked by svr");
					}
					//else
					// strcpy(reply.TextInfo,"UserID Is Invalid.Accunt locked by svr");
				}
				else
				{
					// No such device
					//strcpy(reply.TextInfo,"No such user or password.");
				}
			}
			else
			{
				//strcpy(reply.TextInfo,"Server Access Error.");
				emit evt_Message(this,tr("Database Access Error :")+query.lastError().text());
			}
		}
		else
		{
			//Server db is currently not accessable, wait.
			//strcpy(reply.TextInfo,"Server Not Accessable Now.");
		}


		//Send back
		emit evt_SendDataToClient(this->sock(),array);



		return reply.DoneCode==3?false:true;
	}
	bool st_clientNodeAppLayer::Box2Svr_UploadUserTable()
	{
		const EXAMPLE_MSG_APP * pAppLayer =
				(const EXAMPLE_MSG_APP *)(
					((const char *)(m_currentBlock.constData()))
					+sizeof(EXAMPLE_TRANS_MSG)-1);

		if (m_currentMessageSize!=sizeof(EXAMPLE_TRANS_MSG) - 1
				+ sizeof (EXAMPLE_MSG_APP::tag_app_layer_header)
				+ sizeof (stMsg_UploadUserListReq) - sizeof(quint32)
				+ sizeof (quint32) * (pAppLayer->MsgUnion.msg_UploadUserListReq.UserNum)
				)
		{
			emit evt_Message(this,tr("Broken Message stMsg_UploadUserListReq, size not correct."));
			return false;
		}

		//form Msgs
		quint16 nMsgLen = sizeof(EXAMPLE_MSG_APP::tag_app_layer_header)
				+sizeof(stMsg_UploadUserListRsp);
		QByteArray array(sizeof(EXAMPLE_TRANS_MSG) + nMsgLen - 1,0);
		char * ptr = array.data();
		EXAMPLE_TRANS_MSG * pMsg = (EXAMPLE_TRANS_MSG *)ptr;
		EXAMPLE_MSG_APP * pApp = (EXAMPLE_MSG_APP *)(((unsigned char *)
														  (ptr))+sizeof(EXAMPLE_TRANS_MSG)-1
														 );
		pMsg->Mark = 0x55AA;
		pMsg->source_id = (quint32)((quint64)(m_currentHeader.destin_id) & 0xffffffff );

		pMsg->destin_id = (quint32)((quint64)(m_currentHeader.source_id) & 0xffffffff );;

		pMsg->data_length = nMsgLen;

		pApp->header.MsgType = 0x7FFC;

		stMsg_UploadUserListRsp & reply = pApp->MsgUnion.msg_UploadUserListRsp;

		reply.DoneCode = 1;
		//strcpy(reply.TextInfo,"Unknown error");
		if (loadRelations()==true )
		{
			for (quint32 i = 0; i <pAppLayer->MsgUnion.msg_UploadUserListReq.UserNum && i<4096;i++)
			{
				if (bIsValidUserId(pAppLayer->MsgUnion.msg_UploadUserListReq.pUserIDList[i])==false)
					continue;
				m_matched_nodes.insert(pAppLayer->MsgUnion.msg_UploadUserListReq.pUserIDList[i]);
			}
			if (true == saveRelations())
			{
				reply.DoneCode = 0;
				//strcpy(reply.TextInfo,"Succeeded.");
			}
		}
		else
		{
			//Server db is currently not accessable, wait.
			//strcpy(reply.TextInfo,"Failed to load current relations from db.");
		}


		//Send back
		emit evt_SendDataToClient(this->sock(),array);



		return reply.DoneCode==0?true:false;
	}
	bool st_clientNodeAppLayer::Box2Svr_DownloadUserTable()
	{
		bool res = true;
		//form Msgs
		quint16 nMsgLen = sizeof(EXAMPLE_MSG_APP::tag_app_layer_header)
				+sizeof(stMsg_DownloadUserListRsp) - sizeof(quint32);
		int nSz = 0;
		if (loadRelations()==true )
		{
			nSz = m_matched_nodes.size();
			nMsgLen += nSz * sizeof(quint32);
		}
		else
			res = false;

		//form Msgs
		QByteArray array(sizeof(EXAMPLE_TRANS_MSG) + nMsgLen - 1,0);
		char * ptr = array.data();
		EXAMPLE_TRANS_MSG * pMsg = (EXAMPLE_TRANS_MSG *)ptr;
		EXAMPLE_MSG_APP * pApp = (EXAMPLE_MSG_APP *)(((unsigned char *)
														  (ptr))+sizeof(EXAMPLE_TRANS_MSG)-1
														 );
		pMsg->Mark = 0x55AA;
		pMsg->source_id = (quint32)((quint64)(m_currentHeader.destin_id) & 0xffffffff );

		pMsg->destin_id = (quint32)((quint64)(m_currentHeader.source_id) & 0xffffffff );;

		pMsg->data_length = nMsgLen;

		pApp->header.MsgType = 0x7FFB;

		stMsg_DownloadUserListRsp & reply = pApp->MsgUnion.msg_DownloadUserListRsp;

		reply.DoneCode = res==true?0:1;
		//if (res==false)
		//strcpy(reply.TextInfo,"load Relation failed");
		int ii = 0;
		foreach (quint32 it, m_matched_nodes)
		{
			if (ii < nSz && ii<32768)
			{
				reply.pUserIDList[ii] = it;
				reply.UserNum = (quint16) ii+1;
			}
			++ii;
		}


		//Send back
		emit evt_SendDataToClient(this->sock(),array);



		return reply.DoneCode==0?true:false;
	}
	bool st_clientNodeAppLayer::ClientLogout()
	{
		bool res = true;
		//form Msgs
		quint16 nMsgLen = sizeof(EXAMPLE_MSG_APP::tag_app_layer_header)
				+sizeof(stMsg_ClientLogoutRsp);
		//int nSz = 0;
		//form Msgs
		QByteArray array(sizeof(EXAMPLE_TRANS_MSG) + nMsgLen - 1,0);
		char * ptr = array.data();
		EXAMPLE_TRANS_MSG * pMsg = (EXAMPLE_TRANS_MSG *)ptr;
		EXAMPLE_MSG_APP * pApp = (EXAMPLE_MSG_APP *)(((unsigned char *)
														  (ptr))+sizeof(EXAMPLE_TRANS_MSG)-1
														 );
		pMsg->Mark = 0x55AA;
		pMsg->source_id = (quint32)((quint64)(m_currentHeader.destin_id) & 0xffffffff );

		pMsg->destin_id = (quint32)((quint64)(m_currentHeader.source_id) & 0xffffffff );;

		pMsg->data_length = nMsgLen;


		pApp->header.MsgType = 0x7FFD;

		stMsg_ClientLogoutRsp & reply = pApp->MsgUnion.msg_ClientLogoutRsp;

		reply.DoneCode = res==true?0:1;
		//if (res==false)
		//strcpy(reply.TextInfo,"load Relation failed");
		//Send back
		emit evt_SendDataToClient(this->sock(),array);
		return reply.DoneCode==0?true:false;
	}
}
