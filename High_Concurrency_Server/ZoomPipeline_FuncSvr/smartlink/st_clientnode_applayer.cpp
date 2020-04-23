#include "st_clientnode_applayer.h"
#include "st_client_table.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
namespace ExampleServer{
	st_clientNodeAppLayer::st_clientNodeAppLayer(st_client_table * pClientTable, QObject * pClientSock ,QObject *parent) :
		st_clientNode_baseTrans(pClientTable,pClientSock,parent)
	{

		m_bLoggedIn= false;
		memset(&m_current_app_header,0,sizeof(EXAMPLE_MSG_APP));
	}

	bool st_clientNodeAppLayer::loadRelations()
	{
		QSqlDatabase db = m_pClientTable->dbRes()->databse(m_pClientTable->Database_UserAcct());
		if (db.isValid()==true && db.isOpen()==true )
		{
			QString sql = "select friend_id from relations where user_id = ?;";

			QSqlQuery query(db);
			query.prepare(sql);
			query.addBindValue((quint32)m_uuid);
			if (false== query.exec())
			{
				emit evt_Message(this,tr("try to get relations Failed! ")+ query.lastError().text());
				return false;
			}

			m_matched_nodes.clear();
			while (query.next())
			{
				quint32 val = query.value(0).toUInt();
				m_matched_nodes.insert(val);
			}
			return true;
		}
		else
		{
			//Server db is currently not accessable, wait.
			emit evt_Message(this,"Server Not Accessable Now.");
		}
		return false;
	}

	bool st_clientNodeAppLayer::saveRelations()
	{
		QSqlDatabase db = m_pClientTable->dbRes()->databse(m_pClientTable->Database_UserAcct());
		if (db.isValid()==true && db.isOpen()==true )
		{
			QSqlQuery query(db);
			QString sql = "delete from relations where user_id = ?;";

			query.prepare(sql);
			query.addBindValue((quint32)m_uuid);
			if (false== query.exec())
			{
				emit evt_Message(this,tr("try to del old relations Failed! ")+ query.lastError().text());
				return false;
			}



			sql = "insert into relations (user_id,friend_id) values (?,?);";
			//the forigen key can automatic avoid non-existing values.
			foreach(quint32 nodeid,m_matched_nodes)
			{

				QSqlQuery query(db);
				if (bIsValidUserId(nodeid)==false)
					continue;
				query.prepare(sql);
				query.addBindValue((quint32)m_uuid);
				query.addBindValue(nodeid);

				if (false== query.exec())
				{
					emit evt_Message(this,tr("try to insert new relations Failed! ")+ query.lastError().text());
					return false;
				}
			}
			return true;
		}
		else
		{
			//Server db is currently not accessable, wait.
			emit evt_Message(this,"Server Not Accessable Now.");
		}
		return false;
	}

	//!deal current message
	int st_clientNodeAppLayer::deal_current_message_block()
	{
		//then , Start deal to-server messages
		//Uncomment these codes to enable debug msg dump.
//		if (m_currentBlock.length()>=64)
//			emit evt_Message(this,"Debug:" + m_currentBlock.toHex().left(64) + "..." + m_currentBlock.toHex().right(64));
//		else
//			emit evt_Message(this,"Debug:" + m_currentBlock.toHex());
		if (m_currentHeader.destin_id==0x00000001)
		{
			if (this->m_bLoggedIn==false || this->m_bUUIDRecieved==false)
			{
				//To-Server Messages does not wait for message-block completes
				if (false==Deal_ToServer_Handshakes())
				{
					m_currentBlock = QByteArray();
					emit evt_Message(this,tr("To-server Message Failed."));
					emit evt_close_client(this->sock());
				}
			}
			else
			{
				if (bIsValidUserId(m_currentHeader.source_id) )
				{
					//Deal Client->Svr Msgs
					if (false==Deal_Node2Svr_Msgs())
					{
						m_currentBlock = QByteArray();
						emit evt_Message(this,tr("Client To Server Message Failed."));
						emit evt_close_client(this->sock());
					}
				}
				else if (m_currentHeader.source_id==0xFFFFFFFF)
				{
					m_currentBlock = QByteArray();
					emit evt_Message(this,tr("warning, UUID 0xFFFFFFFF.ignore"));

				}
				else
				{
					m_currentBlock = QByteArray();
					emit evt_Message(this,tr("Bad UUID %1. Client Kicked out").arg(m_currentHeader.source_id));
					emit evt_close_client(this->sock());
				}
			}

		}
		else
		{
			//find Destin Client using Hash.
			st_clientNode_baseTrans * destin_node = m_pClientTable->clientNodeFromUUID(m_currentHeader.destin_id);
			if (destin_node==NULL)
			{
				//need server-to-server channels to re-post this message.
				QString svr = m_pClientTable->cross_svr_find_uuid(m_currentHeader.destin_id);
				if (svr.length()<=0)
					emit evt_Message(this,tr("Destin ID ") + QString("%1").arg(m_currentHeader.destin_id) + tr(" is not currently logged in."));
				else
					m_pClientTable->cross_svr_send_data(svr,m_currentBlock);


			}
			else
			{
				emit evt_SendDataToClient(destin_node->sock(),m_currentBlock);
				m_currentBlock = QByteArray();

			}

		}
		if (bytesLeft()==0)
			m_current_app_header.header.MsgType = 0x00;
		return     st_clientNode_baseTrans::deal_current_message_block();
	}

	//deal to-server handshakes
	bool st_clientNodeAppLayer::Deal_ToServer_Handshakes()
	{
		bool res = true;
		//qDebug()<<m_currentHeader.data_length<<"\n";
		//qDebug()<<this->m_currentBlock.toHex()<<"\n";
		if (m_currentHeader.data_length < sizeof (EXAMPLE_MSG_APP::tag_app_layer_header))
			return false;
		if (m_currentMessageSize < sizeof(EXAMPLE_TRANS_MSG) - 1 + sizeof (EXAMPLE_MSG_APP::tag_app_layer_header))
		{
			// header is not complete, return
			return true;
		}
		//Catch the header
		if (m_current_app_header.header.MsgType==0x00)
			memcpy((void *)&this->m_current_app_header,
				   ((unsigned char *)this->m_currentBlock.constData()) + sizeof(EXAMPLE_TRANS_MSG) - 1,
				   sizeof (EXAMPLE_MSG_APP::tag_app_layer_header)
				   );
		//qDebug()<<m_current_app_header.header.MsgType<<"\n";
		switch (m_current_app_header.header.MsgType)
		{
		case 0x0001:
			if (bytesLeft()>0)
				// message is not complete, return
				return true;
			if (m_currentMessageSize>
					sizeof(EXAMPLE_TRANS_MSG) - 1
					+ sizeof (EXAMPLE_MSG_APP::tag_app_layer_header)
					+ sizeof (stMsg_ClientLoginReq)+66)
			{
				emit evt_Message(this,tr("Broken Message stMsg_ClientLoginReq, size not correct."));
				res = false;
			}
			else
				res = this->LoginClient();
			break;
		default:
			emit evt_Message(this,tr("Message type not supported."));
			res = false;
			break;
		}

		m_currentBlock.clear();


		return res;
	}

	//Deal Box2Svr Msgs
	bool st_clientNodeAppLayer::Deal_Node2Svr_Msgs()
	{
		bool res = true;

		if (m_currentHeader.data_length < sizeof (EXAMPLE_MSG_APP::tag_app_layer_header))
			return false;
		if (m_currentMessageSize < sizeof(EXAMPLE_TRANS_MSG) - 1 + sizeof (EXAMPLE_MSG_APP::tag_app_layer_header))
		{
			// header is not complete, return
			return true;
		}
		//Catch the header
		if (m_current_app_header.header.MsgType==0x00)
			memcpy((void *)&this->m_current_app_header,
				   ((unsigned char *)this->m_currentBlock.constData()) + sizeof(EXAMPLE_TRANS_MSG) - 1,
				   sizeof (EXAMPLE_MSG_APP::tag_app_layer_header)
				   );
		//do
		switch (m_current_app_header.header.MsgType)
		{

		case 0x1003:
			if (bytesLeft()>0)
				return true;

			if (m_currentMessageSize<
					sizeof(EXAMPLE_TRANS_MSG) - 1
					+ sizeof (EXAMPLE_MSG_APP::tag_app_layer_header)
					+ sizeof (stMsg_UploadUserListReq) - sizeof(quint32))
			{
				emit evt_Message(this,tr("Broken Message stMsg_UploadUserListReq, size not correct."));
				res = false;
			}
			else
				res = this->Box2Svr_UploadUserTable();
			break;
		case 0x1004:
			if (bytesLeft()>0)
				// message is not complete, return
				return true;
			if (m_currentMessageSize!=
					sizeof(EXAMPLE_TRANS_MSG) - 1
					+ sizeof (EXAMPLE_MSG_APP::tag_app_layer_header)
					/*+ sizeof (stMsg_DownloadUserListReq)*/)
			{
				emit evt_Message(this,tr("Broken Message stMsg_DownloadUserListReq, size not correct."));
				res = false;
			}
			else
				res = this->Box2Svr_DownloadUserTable();
			break;
		case 0x1002:
			if (bytesLeft()>0)
				// message is not complete, return
				return true;
			if (m_currentMessageSize!=
					sizeof(EXAMPLE_TRANS_MSG) - 1
					+ sizeof (EXAMPLE_MSG_APP::tag_app_layer_header)
					+ sizeof (stMsg_ClientLogoutReq))
			{
				emit evt_Message(this,tr("Broken Message stMsg_ClientLogoutReq, size not correct."));
				res = false;
			}
			else
				res = this->ClientLogout();
			break;

		default:
			emit evt_Message(this,tr("Message type not supported."));
			res = false;
			break;
		}

		m_currentBlock.clear();


		return res;
	}

}
