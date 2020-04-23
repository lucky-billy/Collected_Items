#include "zp_clusternode.h"
#include "zp_clusterterm.h"
#include <assert.h>
namespace ZP_Cluster{
	zp_ClusterNode::zp_ClusterNode(zp_ClusterTerm * pTerm, QObject * psock,QObject *parent) :
		ZPTaskEngine::zp_plTaskBase(parent)
	  ,m_pTerm(pTerm)
	  ,m_pSock(psock)
	  ,bTermSet(false)
	{
		m_currentReadOffset = 0;
		m_currentMessageSize = 0;
		m_nPortLAN = m_nPortPub = 0;
		m_last_Report = QDateTime::currentDateTime();
		m_nRemoteClientNums = 0;
	}
	quint32 zp_ClusterNode::clientNums()
	{
		return m_nRemoteClientNums;
	}

	QDateTime zp_ClusterNode::lastActiveTime()
	{
		return m_last_Report;
	}
	QString zp_ClusterNode::termName()
	{
		return m_strTermName;
	}
	QHostAddress zp_ClusterNode::addrLAN()
	{
		return m_addrLAN;
	}
	int zp_ClusterNode::portLAN()
	{
		return m_nPortLAN;
	}

	QHostAddress zp_ClusterNode::addrPub()
	{
		return m_addrPub;
	}
	int zp_ClusterNode::portPub()
	{
		return m_nPortPub;
	}

	QObject * zp_ClusterNode::sock()
	{
		return m_pSock;
	}

	int zp_ClusterNode::run()
	{
		if (bTermSet==true)
		{
			//qDebug()<<QString("%1(%2) Node Martked Deleted, return.\n").arg((unsigned int)this).arg(ref());
			return 0;
		}
		int nCurrSz = -1;
		int nMessage = m_nMessageBlockSize;
		while (--nMessage>=0 && nCurrSz!=0  )
		{
			QByteArray block;
			m_mutex_rawData.lock();
			if (m_list_RawData.size())
				block =  *m_list_RawData.begin();
			m_mutex_rawData.unlock();
			if (block.isEmpty()==false && block.isNull()==false)
			{
				m_currentReadOffset = filter_message(block,m_currentReadOffset);
				if (m_currentReadOffset >= block.size())
				{
					m_mutex_rawData.lock();
					if (m_list_RawData.empty()==false)
						m_list_RawData.pop_front();
					else
						assert(false);
					m_currentReadOffset = 0;
					m_mutex_rawData.unlock();
				}
			}
			else
			{
				m_mutex_rawData.lock();
				//pop empty cabs
				if (m_list_RawData.empty()==false)
					m_list_RawData.pop_front();
				m_mutex_rawData.unlock();
			}
			m_mutex_rawData.lock();
			nCurrSz = m_list_RawData.size();
			m_mutex_rawData.unlock();
		}
		m_mutex_rawData.lock();
		nCurrSz = m_list_RawData.size();
		m_mutex_rawData.unlock();
		if (nCurrSz==0)
			return 0;
		return -1;
	}
	//push new binary data into queue
	int zp_ClusterNode::push_new_data(const  QByteArray &  dtarray)
	{
		int res = 0;
		m_mutex_rawData.lock();

		m_list_RawData.push_back(dtarray);
		res = m_list_RawData.size();
		m_mutex_rawData.unlock();
		m_last_Report = QDateTime::currentDateTime();
		return res;
	}

	//!deal one message, affect m_currentRedOffset,m_currentMessageSize,m_currentHeader
	//!return bytes Used.
	int zp_ClusterNode::filter_message(QByteArray  block, int offset)
	{
		const int blocklen = block.length();
		while (blocklen>offset)
		{
			const char * dataptr = block.constData();

			//Recieve First 2 byte
			while (m_currentMessageSize<2 && blocklen>offset )
			{
				m_currentBlock.push_back(dataptr[offset++]);
				m_currentMessageSize++;
			}
			if (m_currentMessageSize < 2) //First 2 byte not complete
				continue;

			if (m_currentMessageSize==2)
			{
				const char * headerptr = m_currentBlock.constData();
				memcpy((void *)&m_currentHeader,headerptr,2);
			}

			const char * ptrCurrData = m_currentBlock.constData();
			if (m_currentHeader.Mark == 0x1234)
				//Valid Message
			{
				//while (m_currentMessageSize< sizeof(CROSS_SVR_MSG::tag_header) && blocklen>offset)
				if (m_currentMessageSize< sizeof(CROSS_SVR_MSG::tag_header) && blocklen>offset)
				{
					int nCpy = sizeof(CROSS_SVR_MSG::tag_header) - m_currentMessageSize;
					if (nCpy > blocklen - offset)
						nCpy = blocklen - offset;
					QByteArray arrCpy(dataptr+offset,nCpy);
					m_currentBlock.push_back(arrCpy);
					//m_currentBlock.push_back(dataptr[offset++]);
					//m_currentMessageSize++;
					offset += nCpy;
					m_currentMessageSize += nCpy;
				}
				if (m_currentMessageSize < sizeof(CROSS_SVR_MSG::tag_header)) //Header not completed.
					continue;
				else if (m_currentMessageSize == sizeof(CROSS_SVR_MSG::tag_header))//Header just  completed.
				{
					const char * headerptr = m_currentBlock.constData();
					memcpy((void *)&m_currentHeader,headerptr,sizeof(CROSS_SVR_MSG::tag_header));

					//continue reading if there is data left behind
					if (block.length()>offset)
					{
						qint32 bitLeft = m_currentHeader.data_length + sizeof(CROSS_SVR_MSG::tag_header)
								-m_currentMessageSize ;
						//while (bitLeft>0 && blocklen>offset)
						if (bitLeft>0 && blocklen>offset)
						{
							int nCpy = bitLeft;
							if (nCpy > blocklen - offset)
								nCpy = blocklen - offset;
							QByteArray arrCpy(dataptr+offset,nCpy);
							m_currentBlock.push_back(arrCpy);
							offset += nCpy;
							m_currentMessageSize += nCpy;
							bitLeft -= nCpy;
							//m_currentBlock.push_back(dataptr[offset++]);
							//m_currentMessageSize++;
							//bitLeft--;
						}
						//deal block, may be send data as soon as possible;
						deal_current_message_block();
						if (bitLeft>0)
							continue;
						//This Message is Over. Start a new one.
						m_currentMessageSize = 0;
						m_currentBlock = QByteArray();
						continue;
					}
				}
				else
				{
					if (block.length()>offset)
					{
						qint32 bitLeft = m_currentHeader.data_length + sizeof(CROSS_SVR_MSG::tag_header)
								-m_currentMessageSize ;
						//while (bitLeft>0 && blocklen>offset)
						if (bitLeft>0 && blocklen>offset)
						{
							int nCpy = bitLeft;
							if (nCpy > blocklen - offset)
								nCpy = blocklen - offset;
							QByteArray arrCpy(dataptr+offset,nCpy);
							m_currentBlock.push_back(arrCpy);
							offset += nCpy;
							m_currentMessageSize += nCpy;
							bitLeft -= nCpy;

							//m_currentBlock.push_back(dataptr[offset++]);
							//m_currentMessageSize++;
							//bitLeft--;
						}
						//deal block, may be processed as soon as possible;
						deal_current_message_block();
						if (bitLeft>0)
							continue;
						//This Message is Over. Start a new one.
						m_currentMessageSize = 0;
						m_currentBlock = QByteArray();
						continue;
					}
				} // end if there is more bytes to append
			} //end deal trans message
			else
			{
				emit evt_Message(this,tr("Client Send a unknown start Header %1 %2. Close client immediately.")
								 .arg((int)(ptrCurrData[0])).arg((int)(ptrCurrData[1])));
				m_currentMessageSize = 0;
				m_currentBlock = QByteArray();
				offset = blocklen;
				emit evt_close_client(this->sock());
			}
		} // end while block len > offset

		return offset;
	}
	//in Trans-Level, do nothing.
	int zp_ClusterNode::deal_current_message_block()
	{
		qint32 bytesLeft = m_currentHeader.data_length + sizeof(CROSS_SVR_MSG::tag_header)
				-m_currentMessageSize ;
		const CROSS_SVR_MSG * pMsg =(const CROSS_SVR_MSG *) m_currentBlock.constData();
		switch(m_currentHeader.messagetype)
		{
		case 0x00://Heart Beating
			if (bytesLeft==0)
			{
				m_nRemoteClientNums = pMsg->payload.heartBeating.nClients;
			}
			break;
		case 0x01://basicInfo, when connection established, this message should be used
//			UnComment code below, will generate debug output.
//			if (m_currentBlock.length()>=64)
//				emit evt_Message(this,"Debug:" + m_currentBlock.toHex().left(64) + "..." + m_currentBlock.toHex().right(64));
//			else
//				emit evt_Message(this,"Debug:" + m_currentBlock.toHex());

			if (bytesLeft==0)
			{
				QString strName ((const char *)pMsg->payload.basicInfo.name);
				if (strName != m_pTerm->name())
				{
					this->m_strTermName = strName;
					m_nPortLAN = pMsg->payload.basicInfo.port_LAN;
					m_addrLAN = QHostAddress((const char *)pMsg->payload.basicInfo.Address_LAN);
					m_nPortPub = pMsg->payload.basicInfo.port_Pub;
					m_addrPub = QHostAddress((const char *)pMsg->payload.basicInfo.Address_Pub);
					if (false==m_pTerm->regisitNewServer(this))
					{
						this->m_strTermName.clear();
						emit evt_Message(this,tr("Info: New Svr already regisited. Ignored.")+strName);
						emit evt_close_client(this->sock());
					}
					else
					{
						emit evt_NewSvrConnected(this->termName());
						m_pTerm->BroadcastServers();
					}
				}
				else
				{
					emit evt_Message(this,tr("Can not connect to it-self, Loopback connections is forbidden."));
					emit evt_close_client(this->sock());
				}
			}
			break;
		case 0x02: //Server - broadcast messages
//			UnComment code below, will generate debug output.
//			if (m_currentBlock.length()>=64)
//				emit evt_Message(this,"Debug:" + m_currentBlock.toHex().left(64) + "..." + m_currentBlock.toHex().right(64));
//			else
//				emit evt_Message(this,"Debug:" + m_currentBlock.toHex());

			if (bytesLeft==0)
			{
				int nSvrs = pMsg->hearder.data_length / sizeof(CROSS_SVR_MSG::uni_payload::tag_CSM_Broadcast);
				for (int i=0;i<nSvrs;i++)
				{
					QString strName ((const char *)pMsg->payload.broadcastMsg[i].name);
					if (strName != m_pTerm->name() && m_pTerm->SvrNodeFromName(strName)==NULL)
					{
						QHostAddress addrToConnectTo((const char *)pMsg->payload.broadcastMsg[i].Address_LAN);
						quint16 PortToConnectTo = pMsg->payload.broadcastMsg[i].port_LAN;
						//because cross-connection is not good, we just want the low Addr:port connect to max Addr:Port.
						//Connect to New Servers
						if (strName > m_pTerm->name())
							emit evt_connect_to(addrToConnectTo,PortToConnectTo,false);
						else
							emit evt_Message(this,tr("Name %1 <= %2, omitted.").arg(strName).arg(m_pTerm->name()));
					}
				}
			}
			break;
		case 0x03:
			//			UnComment code below, will generate debug output.
//			if (m_currentBlock.length()>=64)
//				emit evt_Message(this,"Debug:" + m_currentBlock.toHex().left(64) + "..." + m_currentBlock.toHex().right(64));
//			else
//				emit evt_Message(this,"Debug:" + m_currentBlock.toHex());

			if (m_currentMessageSize==m_currentBlock.size())
			{
				QByteArray arraySend ((const char *)(pMsg) + sizeof(CROSS_SVR_MSG::tag_header),
									  m_currentMessageSize - sizeof(CROSS_SVR_MSG::tag_header));
				if (deal_user_data(arraySend)==true)
					m_currentBlock = QByteArray();
			}
			else
			{
				if (deal_user_data(m_currentBlock)==true)
					m_currentBlock = QByteArray();
			}
			break;
		default:
			emit evt_Message(this,tr("Info:Unknown Msg Type got."));
			emit evt_close_client(this->sock());
			break;
		};

		return 0;
	}

	bool zp_ClusterNode::deal_user_data(QByteArray  data)
	{
		emit evt_RemoteData_recieved(this->termName(),data);
		return true;
	}

	void zp_ClusterNode::CheckHeartBeating()
	{
		QDateTime dtm = QDateTime::currentDateTime();
		qint64 usc = this->m_last_Report.secsTo(dtm);
		int nThredHold = m_pTerm->heartBeatingThrdHold();
		if (usc >= nThredHold)
		{
			emit evt_Message(this,tr("Client ") + QString("%1").arg((unsigned int)((quint64)this)) + tr(" is dead, kick out."));
			emit evt_close_client(this->sock());
		}
	}
	void zp_ClusterNode::SendHelloPackage()
	{
		int nMsgLen = sizeof(CROSS_SVR_MSG::tag_header) + sizeof (CROSS_SVR_MSG::uni_payload::tag_CSM_BasicInfo);
		QByteArray array(nMsgLen,0);
		CROSS_SVR_MSG * pMsg =(CROSS_SVR_MSG *) array.data();
		pMsg->hearder.Mark = 0x1234;
		pMsg->hearder.data_length = sizeof (CROSS_SVR_MSG::uni_payload::tag_CSM_BasicInfo);
		pMsg->hearder.messagetype = 0x01;
		strncpy((char *)(pMsg->payload.basicInfo.name),
				m_pTerm->name().toStdString().c_str(),
				sizeof(pMsg->payload.basicInfo.name)-1);
		strncpy((char *)(pMsg->payload.basicInfo.Address_LAN),
				m_pTerm->LANAddr().toString().toStdString().c_str(),
				sizeof(pMsg->payload.basicInfo.Address_LAN)-1);

		pMsg->payload.basicInfo.port_LAN = m_pTerm->LANPort();

		strncpy((char *)(pMsg->payload.basicInfo.Address_Pub),
				m_pTerm->PublishAddr().toString().toStdString().c_str(),
				sizeof(pMsg->payload.basicInfo.Address_Pub)-1);

		pMsg->payload.basicInfo.port_Pub = m_pTerm->PublishPort();


		emit evt_SendDataToClient(sock(),array);
	}
}
