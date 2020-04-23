#include "maindialog.h"
#include "ui_maindialog.h"
#include "../ZoomPipeline_FuncSvr/smartlink/st_message.h"
#include "../ZoomPipeline_FuncSvr/smartlink/st_msg_applayer.h"
#include <QSettings>
#include <time.h>
#include <QMessageBox>
#include <string.h>
#include <QVector>
using namespace ExampleServer;
MainDialog::MainDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::MainDialog)
{
	ui->setupUi(this);
	m_bLogedIn = false;
	m_bUUIDGot = false;
	nTimer = startTimer(100);
	ui->listView_msg->setModel(&model);
	client = new QGHTcpClient (this);
	connect(client, SIGNAL(readyRead()),this, SLOT(new_data_recieved()));
	connect(client, SIGNAL(connected()),this, SLOT(on_client_connected()));
	connect(client, SIGNAL(disconnected()),this,SLOT(on_client_disconnected()));
	connect(client, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(displayError(QAbstractSocket::SocketError)));
	connect(client, SIGNAL(bytesWritten(qint64)), this, SLOT(on_client_trasferred(qint64)));

	m_currentReadOffset = 0;
	m_currentMessageSize = 0;


	QSettings settings("goldenhawking club","FunctionalClientTest",this);
	ui->lineEdit_ip->setText(settings.value("settings/ip","localhost").toString());
	ui->lineEdit_Port->setText(settings.value("settings/port","23456").toString());
	ui->lineEdit_username->setText(settings.value("settings/client2svr_username","111").toString());
	ui->lineEdit_password->setText(settings.value("settings/client2svr_password","111").toString());
	ui->plainTextEdit_box_userids->setPlainText(settings.value("settings/box2svr_uploadid","0,").toString());
	ui->lineEdit_client_uuid->setText(settings.value("settings/client_uuid","112").toString());
}

MainDialog::~MainDialog()
{
	delete ui;
}
void MainDialog::on_client_trasferred(qint64 dtw)
{
	QGHTcpClient * pSock = qobject_cast<QGHTcpClient*>(sender());
	if (pSock)
	{
		displayMessage(QString("client %1 Transferrd %2 bytes.").arg((quintptr)pSock).arg(dtw));
	}

}
void MainDialog::on_client_connected()
{
	QGHTcpClient * pSock = qobject_cast<QGHTcpClient*>(sender());
	if (pSock)
	{
		displayMessage(QString("client %1 connected.").arg((quintptr)pSock));
		ui->pushButton_connect->setEnabled(false);

	}

}
void MainDialog::on_client_disconnected()
{
	m_bLogedIn = m_bUUIDGot = false;
	client = new QGHTcpClient (this);
	connect(client, SIGNAL(readyRead()),this, SLOT(new_data_recieved()));
	connect(client, SIGNAL(connected()),this, SLOT(on_client_connected()));
	connect(client, SIGNAL(disconnected()),this,SLOT(on_client_disconnected()));
	connect(client, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(displayError(QAbstractSocket::SocketError)));
	connect(client, SIGNAL(bytesWritten(qint64)), this, SLOT(on_client_trasferred(qint64)));

	QGHTcpClient * pSock = qobject_cast<QGHTcpClient*>(sender());
	if (pSock)
	{
		displayMessage(QString("client %1 disconnected.").arg((quintptr)pSock));
		ui->pushButton_connect->setEnabled(true);
		pSock->abort();
		pSock->deleteLater();
	}
}
void MainDialog::displayError(QAbstractSocket::SocketError /*err*/)
{
	QGHTcpClient * sock = qobject_cast<QGHTcpClient *> (sender());
	if (sock)
		displayMessage(QString("client %1 error msg:").arg((quintptr)sock)+sock->errorString());
}
void MainDialog::new_data_recieved()
{
	QTcpSocket * pSock = qobject_cast<QTcpSocket*>(sender());
	if (pSock)
	{
		QByteArray block =pSock->readAll();
		if (block.isEmpty()==false && block.size()>0)
		{

			do
			{
				m_currentReadOffset = filter_message(block,m_currentReadOffset);
			}
			while (m_currentReadOffset < block.size());
			m_currentReadOffset = 0;
		}
	}
}
void MainDialog::displayMessage(QString str)
{
	model.insertRow(0,new QStandardItem(str));
	while (model.rowCount()>=256)
		model.removeRow(model.rowCount()-1);
}
void MainDialog::saveIni()
{
	QSettings settings("goldenhawking club","FunctionalClientTest",this);
	settings.setValue("settings/ip", ui->lineEdit_ip->text());
	settings.setValue("settings/port", ui->lineEdit_Port->text());
	settings.setValue("settings/client2svr_username", ui->lineEdit_username->text());
	settings.setValue("settings/client2svr_password", ui->lineEdit_password->text());
	settings.setValue("settings/box2svr_uploadid", ui->plainTextEdit_box_userids->toPlainText());
	settings.setValue("settings/client_uuid", ui->lineEdit_client_uuid->text());

}

void MainDialog::timerEvent(QTimerEvent * evt)
{
	static int nCount = 0;
	if (evt->timerId()==nTimer)
	{
		nCount++;
		if (nCount % 200 == 0 && client->isOpen()==true)
		{
			//send heart-beating
			QByteArray array(sizeof(EXAMPLE_HEARTBEATING),0);
			char * ptr = array.data();
			EXAMPLE_HEARTBEATING * pMsg = (EXAMPLE_HEARTBEATING *)ptr;
			pMsg->Mark = 0xBEBE;
			pMsg->tmStamp = time(0)&0x00ffff;
			pMsg->source_id = 0;
			//3/10 possibility to send a data block to server
			client->SendData(array);

		}
	}
}
void MainDialog::on_pushButton_connect_clicked()
{
	saveIni();
	client->connectToHost(ui->lineEdit_ip->text(),ui->lineEdit_Port->text().toUShort());
}

void MainDialog::on_pushButton_clientLogin_clicked()
{
	if (client->isOpen()==false)
	{
		QMessageBox::warning(this,"Please connect first!","Connect to server and test funcs");
		return;
	}
	saveIni();
	quint32 userID = ui->lineEdit_username->text().toUInt();
	QString strPassWd = ui->lineEdit_password->text();
	std::string strStdPassword = strPassWd.toStdString();
	const char * pSrcPassword = strStdPassword.c_str();
	int nMaxLenPassword = strStdPassword.length();

	quint16 nMsgLen = sizeof(EXAMPLE_MSG_APP::tag_app_layer_header)
			+sizeof(stMsg_ClientLoginReq)+nMaxLenPassword;
	QByteArray array(sizeof(EXAMPLE_TRANS_MSG) + nMsgLen - 1,0);
	char * ptr = array.data();
	EXAMPLE_TRANS_MSG * pMsg = (EXAMPLE_TRANS_MSG *)ptr;
	EXAMPLE_MSG_APP * pApp = (EXAMPLE_MSG_APP *)(((unsigned char *)
													  (ptr))+sizeof(EXAMPLE_TRANS_MSG)-1
													 );
	pMsg->Mark = 0x55AA;
	pMsg->source_id = (quint32)((quint64)(0xffffffff) & 0xffffffff );

	pMsg->destin_id = (quint32)((quint64)(0x00000001) & 0xffffffff );;

	pMsg->data_length = nMsgLen;


	pApp->header.MsgType = 0x0001;

	pApp->MsgUnion.msg_ClientLoginReq.user_id = userID;

	for (int i=0;i<=nMaxLenPassword;i++)
		pApp->MsgUnion.msg_ClientLoginReq.Passwd[i] =
				i<nMaxLenPassword?pSrcPassword[i]:0;


	//3/10 possibility to send a data block to server
	client->SendData(array);
}

void MainDialog::on_pushButton_box_upload_uid_clicked()
{
	if (client->isOpen()==false)
	{
		QMessageBox::warning(this,"Please connect first!","Connect to server and test funcs");
		return;
	}
	if (m_bLogedIn==false)
	{
		QMessageBox::warning(this,"Please log in the host first!","get uuid and login");
		return;
	}
	saveIni();
	QStringList lst = ui->plainTextEdit_box_userids->toPlainText().split(",");
	QVector<quint32> vecInt;
	foreach (QString item,lst)
	{
		vecInt.push_back(item.toUInt());
	}

	quint16 nMsgLen = sizeof(EXAMPLE_MSG_APP::tag_app_layer_header)
			+sizeof(stMsg_UploadUserListReq)+ sizeof(quint32)*vecInt.size() - sizeof(quint32);
	QByteArray array(sizeof(EXAMPLE_TRANS_MSG) + nMsgLen - 1,0);
	char * ptr = array.data();
	EXAMPLE_TRANS_MSG * pMsg = (EXAMPLE_TRANS_MSG *)ptr;
	EXAMPLE_MSG_APP * pApp = (EXAMPLE_MSG_APP *)(((unsigned char *)
													  (ptr))+sizeof(EXAMPLE_TRANS_MSG)-1
													 );
	pMsg->Mark = 0x55AA;
	pMsg->source_id = (quint32)((quint64)(ui->lineEdit_username->text().toUInt()) & 0xffffffff );

	pMsg->destin_id = (quint32)((quint64)(0x00000001) & 0xffffffff );;

	pMsg->data_length = nMsgLen;


	pApp->header.MsgType = 0x1003;

	pApp->MsgUnion.msg_UploadUserListReq.UserNum = (quint16)(vecInt.size() & 0x00ffff);
	for (int i=0;i< vecInt.size();i++)
		pApp->MsgUnion.msg_UploadUserListReq.pUserIDList[i] = vecInt[i];

	//3/10 possibility to send a data block to server
	client->SendData(array);
}

void MainDialog::on_pushButton_box_download_uid_clicked()
{
	if (client->isOpen()==false)
	{
		QMessageBox::warning(this,"Please connect first!","Connect to server and test funcs");
		return;
	}
	if (m_bLogedIn==false)
	{
		QMessageBox::warning(this,"Please log in the host first!","get uuid and login");
		return;
	}
	saveIni();
	quint16 nMsgLen = sizeof(EXAMPLE_MSG_APP::tag_app_layer_header)
			/*+sizeof(stMsg_DownloadUserListReq)*/;
	QByteArray array(sizeof(EXAMPLE_TRANS_MSG) + nMsgLen - 1,0);
	char * ptr = array.data();
	EXAMPLE_TRANS_MSG * pMsg = (EXAMPLE_TRANS_MSG *)ptr;
	EXAMPLE_MSG_APP * pApp = (EXAMPLE_MSG_APP *)(((unsigned char *)
													  (ptr))+sizeof(EXAMPLE_TRANS_MSG)-1
													 );
	pMsg->Mark = 0x55AA;
	pMsg->source_id = (quint32)((quint64)(ui->lineEdit_username->text().toUInt()) & 0xffffffff );

	pMsg->destin_id = (quint32)((quint64)(0x00000001) & 0xffffffff );;

	pMsg->data_length = nMsgLen;

	pApp->header.MsgType = 0x1004;

	//3/10 possibility to send a data block to server
	client->SendData(array);
}

void  MainDialog::on_pushButton_clientLogout_clicked()
{
	if (client->isOpen()==false)
	{
		QMessageBox::warning(this,"Please connect first!","Connect to server and test funcs");
		return;
	}
	if (m_bLogedIn==false)
	{
		QMessageBox::warning(this,"Please log in the host first!","get uuid and login");
		return;
	}
	saveIni();
	quint16 nMsgLen = sizeof(EXAMPLE_MSG_APP::tag_app_layer_header)
			+sizeof(stMsg_ClientLogoutReq);
	QByteArray array(sizeof(EXAMPLE_TRANS_MSG) + nMsgLen - 1,0);
	char * ptr = array.data();
	EXAMPLE_TRANS_MSG * pMsg = (EXAMPLE_TRANS_MSG *)ptr;
	EXAMPLE_MSG_APP * pApp = (EXAMPLE_MSG_APP *)(((unsigned char *)
													  (ptr))+sizeof(EXAMPLE_TRANS_MSG)-1
													 );
	pMsg->Mark = 0x55AA;
	pMsg->source_id = (quint32)((quint64)(ui->lineEdit_username->text().toUInt()) & 0xffffffff );

	pMsg->destin_id = (quint32)((quint64)(0x00000001) & 0xffffffff );;

	pMsg->data_length = nMsgLen;


	pApp->header.MsgType = 0x1002;


	pApp->MsgUnion.msg_ClientLogoutReq.UserName[0] = 0;


	//3/10 possibility to send a data block to server
	client->SendData(array);
}

//!deal one message, affect m_currentRedOffset,m_currentMessageSize,m_currentHeader
//!return bytes Used.
int MainDialog::filter_message(QByteArray  block, int offset)
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
		//Heart Beating
		if (m_currentHeader.Mark == 0xBEBE)
		{
			while (m_currentMessageSize< sizeof(EXAMPLE_HEARTBEATING) && blocklen>offset )
			{
				m_currentBlock.push_back(dataptr[offset++]);
				m_currentMessageSize++;
			}
			if (m_currentMessageSize < sizeof(EXAMPLE_HEARTBEATING)) //Header not completed.
				continue;
			const EXAMPLE_HEARTBEATING * ptr = (const EXAMPLE_HEARTBEATING *)m_currentBlock.constData();
			displayMessage(tr("Recieved Heart-beating msg sended %1 sec(s) ago.").
						   arg((time(0)&0x00ffff)-(ptr->tmStamp)));
			//This Message is Over. Start a new one.
			m_currentMessageSize = 0;
			m_currentBlock = QByteArray();

			continue;
		}
		else if (m_currentHeader.Mark == 0x55AA)
			//Trans Message
		{
			while (m_currentMessageSize< sizeof(EXAMPLE_TRANS_MSG)-1 && blocklen>offset)
			{
				m_currentBlock.push_back(dataptr[offset++]);
				m_currentMessageSize++;
			}
			if (m_currentMessageSize < sizeof(EXAMPLE_TRANS_MSG)-1) //Header not completed.
				continue;
			else if (m_currentMessageSize == sizeof(EXAMPLE_TRANS_MSG)-1)//Header just  completed.
			{
				const char * headerptr = m_currentBlock.constData();
				memcpy((void *)&m_currentHeader,headerptr,sizeof(EXAMPLE_TRANS_MSG)-1);

				//continue reading if there is data left behind
				if (block.length()>offset)
				{
					qint32 bitLeft = m_currentHeader.data_length + sizeof(EXAMPLE_TRANS_MSG) - 1
							-m_currentMessageSize ;
					while (bitLeft>0 && blocklen>offset)
					{
						m_currentBlock.push_back(dataptr[offset++]);
						m_currentMessageSize++;
						bitLeft--;
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
					qint32 bitLeft = m_currentHeader.data_length + sizeof(EXAMPLE_TRANS_MSG) - 1
							-m_currentMessageSize ;
					while (bitLeft>0 && blocklen>offset)
					{
						m_currentBlock.push_back(dataptr[offset++]);
						m_currentMessageSize++;
						bitLeft--;
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
			displayMessage(tr("Client Send a unknown start Header %1 %2. Close client immediately.")
						   .arg((int)(ptrCurrData[0])).arg((int)(ptrCurrData[1])));
			m_currentMessageSize = 0;
			m_currentBlock = QByteArray();
			offset = blocklen;

			this->client->abort();
		}
	} // end while block len > offset

	return offset;
}
//!deal current message
int MainDialog::deal_current_message_block()
{
	//The bytes left to recieve.
	qint32 bytesLeft = m_currentHeader.data_length + sizeof(EXAMPLE_TRANS_MSG) - 1
			-m_currentMessageSize ;
	if (bytesLeft)
		return 0;


	const char * ptr = m_currentBlock.constData();

	EXAMPLE_MSG_APP * pApp = (EXAMPLE_MSG_APP *)(((unsigned char *)
													  (ptr))+sizeof(EXAMPLE_TRANS_MSG)-1
													 );

	if (pApp->header.MsgType==0x7FFE)
	{
		if (pApp->MsgUnion.msg_ClientLoginRsp.DoneCode==0)
		{
			m_bLogedIn = true;
			QMessageBox::information(this,tr("Succeed!"),tr("Log in succeed!"));
		}
		else if (pApp->MsgUnion.msg_ClientLoginRsp.DoneCode==1)
		{
			m_bLogedIn = true;
			QMessageBox::information(this,tr("Succeed!"),tr("But you can connect to another idle svr:%1:%2!")
									 .arg((const char *)pApp->MsgUnion.msg_ClientLoginRsp.Address_Redirect)
									 .arg(pApp->MsgUnion.msg_ClientLoginRsp.port_Redirect)
									 );
		}
		else
			QMessageBox::information(this,tr("Failed!"),tr("Log in failed!"));
		displayMessage(tr("Res = %1")
				   .arg(pApp->MsgUnion.msg_ClientLoginRsp.DoneCode)
				   );
	}
	else if (pApp->header.MsgType==0x7FFC)
	{
		if (pApp->MsgUnion.msg_UploadUserListRsp.DoneCode==0)
			QMessageBox::information(this,tr("Succeed!"),tr("upload succeed!"));
		else
			QMessageBox::information(this,tr("Failed!"),tr("upload in Failed!"));
		displayMessage(tr("Res = %1")
					   .arg(pApp->MsgUnion.msg_UploadUserListRsp.DoneCode)
					   );

	}
	else if (pApp->header.MsgType==0x7FFB)
	{
		if (pApp->MsgUnion.msg_DownloadUserListRsp.DoneCode==0)
		{
			QMessageBox::information(this,tr("Succeed!"),tr("download succeed!"));
			QString strRes;
			for (quint16 i = 0;i<pApp->MsgUnion.msg_DownloadUserListRsp.UserNum;i++)
			{
				strRes += QString("%1,").arg(pApp->MsgUnion.msg_DownloadUserListRsp.pUserIDList[i]);
			}
			ui->plainTextEdit_box_userids->setPlainText(strRes);
		}
		else
			QMessageBox::information(this,tr("Failed!"),tr("download in Failed!"));
		displayMessage(tr("Res = %1")
					   .arg(pApp->MsgUnion.msg_DownloadUserListRsp.DoneCode)
					   );

	}
	else if (pApp->header.MsgType==0x7FFD)
	{
		if (pApp->MsgUnion.msg_ClientLogoutRsp.DoneCode==0)
			QMessageBox::information(this,tr("Succeed!"),tr("log out succeed!"));
		else
			QMessageBox::information(this,tr("Failed!"),tr("download in Failed!"));
		displayMessage(tr("Res = %1")
					   .arg(pApp->MsgUnion.msg_ClientLogoutRsp.DoneCode)
					   );
		this->client->abort();
	}
	else
	{
		QString str;
		int nLen =  m_currentHeader.data_length;
		for (int i=0;i<nLen;i++)
		{
			str += ptr[i+ sizeof(EXAMPLE_TRANS_MSG) - 1];
		}
		ui->plainTextEdit_msg_recieved->setPlainText(str);

	}
	m_currentBlock = QByteArray();


	return 0;
}

void MainDialog::on_pushButton_sendToClient_clicked()
{
	if (client->isOpen()==false)
	{
		QMessageBox::warning(this,"Please connect first!","Connect to server and test funcs");
		return;
	}
	if (m_bLogedIn==false)
	{
		QMessageBox::warning(this,"Please log in the host first!","get uuid and login");
		return;
	}
	saveIni();
	QString strMsg = ui->plainTextEdit_msg_to_client->toPlainText();
	QByteArray arrMsg(strMsg.toStdString().c_str());
	QByteArray array(sizeof(EXAMPLE_TRANS_MSG) - 1,0);
	char * ptr = array.data();
	EXAMPLE_TRANS_MSG * pMsg = (EXAMPLE_TRANS_MSG *)ptr;

	pMsg->Mark = 0x55AA;
	pMsg->source_id = (quint32)((quint64)(ui->lineEdit_username->text().toUInt()) & 0xffffffff );
	pMsg->destin_id = (quint32)((quint64)(ui->lineEdit_client_uuid->text().toUInt()) & 0xffffffff );;

	pMsg->data_length = arrMsg.size();

	array.append(arrMsg);

	//3/10 possibility to send a data block to server
	client->SendData(array);
}
