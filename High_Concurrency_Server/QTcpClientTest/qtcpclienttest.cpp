#include "qtcpclienttest.h"
#include <QSettings>
#include <QCoreApplication>
#include <QFileDialog>
#include "../ZoomPipeline_FuncSvr/smartlink/st_message.h"
using namespace ExampleServer;
QTcpClientTest::QTcpClientTest(QWidget *parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	//Paramenters
	QString iniFile = QCoreApplication::applicationFilePath()+".ini";
	QSettings settings(iniFile,QSettings::IniFormat);
	ui.lineEdit_ip->setText(settings.value("ip","localhost").toString());
	ui.lineEdit_Port->setText(settings.value("port","23457").toString());
	ui.dial->setValue(settings.value("clientNum","32").toInt());
	ui.lcdNumber->display(settings.value("clientNum","32").toInt());
	ui.horizontalSlider->setValue(settings.value("Payload","2048").toInt());
	ui.label_load->setText(QString("Payload = %1").arg(settings.value("Payload","2048").toInt()));
	ui.checkBox_SSL->setChecked(settings.value("SSL",false).toBool());
	ui.lineEdit_globalFile->setText(settings.value("globalFile","/zpserver_syn.debug").toString());
	ui.listView_msg->setModel(&model);
	m_maxUUID = 2;
	m_minUUID = 0xffffffff;
}

QTcpClientTest::~QTcpClientTest()
{

}
void QTcpClientTest::on_horizontalSlider_valueChanged(int value)
{
	ui.label_load->setText(QString("Payload = %1").arg(value));
}
void QTcpClientTest::on_action_Connect_triggered(bool bConn)
{
	//connect to the server
	QString iniFile = QCoreApplication::applicationFilePath()+".ini";
	QSettings settings(iniFile,QSettings::IniFormat);
	settings.setValue("ip",ui.lineEdit_ip->text());
	settings.setValue("port",ui.lineEdit_Port->text());
	settings.setValue("clientNum",ui.dial->value());
	settings.setValue("Payload",ui.horizontalSlider->value());
	settings.setValue("SSL",ui.checkBox_SSL->isChecked());
	settings.setValue("globalFile",ui.lineEdit_globalFile->text());
	if (bConn==true)
	{
		nTimer = startTimer(100);
		ui.dockWidget->setEnabled(false);
	}
	else
	{
		killTimer(nTimer);
		ui.dockWidget->setEnabled(true);
		QList<QTcpSocket*> listObj = m_clients.keys();
		foreach(QTcpSocket * sock,listObj)
			sock->disconnectFromHost();

	}
}
void QTcpClientTest::on_client_trasferred(qint64 dtw)
{
	if (dtw <= sizeof(EXAMPLE_HEARTBEATING))
		return;
	QTcpSocket * pSock = qobject_cast<QTcpSocket*>(sender());
	if (pSock)
	{
		QGHSslClient * pSockSsl = qobject_cast<QGHSslClient*>(sender());
		QGHTcpClient * pSockTcp = qobject_cast<QGHTcpClient*>(sender());
		if (pSockSsl)
			displayMessage(QString("client %1 Transferrd %2 bytes.").arg(pSockSsl->uuid()).arg(dtw));
		else if (pSockTcp)
			displayMessage(QString("client %1 Transferrd %2 bytes.").arg(pSockTcp->uuid()).arg(dtw));
	}

}
void QTcpClientTest::on_client_connected()
{
	QGHSslClient * pSockSsl = qobject_cast<QGHSslClient*>(sender());
	QGHTcpClient * pSockTcp = qobject_cast<QGHTcpClient*>(sender());

	if (pSockSsl)
	{
		pSockSsl->geneGlobalUUID(ui.lineEdit_globalFile->text());
		if (pSockSsl->uuid()>m_maxUUID)
			m_maxUUID = pSockSsl->uuid();
		if (pSockSsl->uuid()<m_minUUID)
			m_minUUID = pSockSsl->uuid();

		displayMessage(QString("client %1 connected.").arg(pSockSsl->uuid()));
		QByteArray array(sizeof(EXAMPLE_HEARTBEATING),0);
		char * ptr = array.data();
		EXAMPLE_HEARTBEATING * pMsg = (EXAMPLE_HEARTBEATING *)ptr;
		pMsg->Mark = 0xBEBE;
		pMsg->source_id = pSockSsl->uuid();
		pMsg->tmStamp = 0;
		(pSockSsl)->SendData(array);
	}
	else if (pSockTcp)
	{
		pSockTcp->geneGlobalUUID(ui.lineEdit_globalFile->text());
		if (pSockTcp->uuid()>m_maxUUID)
			m_maxUUID = pSockTcp->uuid();
		if (pSockTcp->uuid()<m_minUUID)
			m_minUUID = pSockTcp->uuid();

		displayMessage(QString("client %1 connected.").arg(pSockTcp->uuid()));
		QByteArray array(sizeof(EXAMPLE_HEARTBEATING),0);
		char * ptr = array.data();
		EXAMPLE_HEARTBEATING * pMsg = (EXAMPLE_HEARTBEATING *)ptr;
		pMsg->Mark = 0xBEBE;
		pMsg->source_id = pSockTcp->uuid();
		pMsg->tmStamp = 0;
		(pSockTcp)->SendData(array);
	}

}
void QTcpClientTest::on_client_disconnected()
{
	QGHSslClient * pSockSsl = qobject_cast<QGHSslClient*>(sender());
	QGHTcpClient * pSockTcp = qobject_cast<QGHTcpClient*>(sender());
	if (pSockSsl)
	{
		displayMessage(QString("client %1 disconnected.").arg(pSockSsl->uuid()));
		//disconnect the signal immediately so that the system resource could be freed.
		disconnect(pSockSsl, SIGNAL(readyRead()),this, SLOT(new_data_recieved()));
		disconnect(pSockSsl, SIGNAL(encrypted()),this, SLOT(on_client_connected()));
		disconnect(pSockSsl, SIGNAL(disconnected()),this,SLOT(on_client_disconnected()));
		disconnect(pSockSsl, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(displayError(QAbstractSocket::SocketError)));
		disconnect(pSockSsl, SIGNAL(bytesWritten(qint64)), this, SLOT(on_client_trasferred(qint64)));
		m_clients.remove(pSockSsl);
		pSockSsl->deleteLater();
	}
	else if (pSockTcp)
	{
		displayMessage(QString("client %1 disconnected.").arg(pSockTcp->uuid()));
		//disconnect the signal immediately so that the system resource could be freed.
		disconnect(pSockTcp, SIGNAL(readyRead()),this, SLOT(new_data_recieved()));
		disconnect(pSockTcp, SIGNAL(connected()),this, SLOT(on_client_connected()));
		disconnect(pSockTcp, SIGNAL(disconnected()),this,SLOT(on_client_disconnected()));
		disconnect(pSockTcp, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(displayError(QAbstractSocket::SocketError)));
		disconnect(pSockTcp, SIGNAL(bytesWritten(qint64)), this, SLOT(on_client_trasferred(qint64)));
		m_clients.remove(pSockTcp);
		pSockTcp->deleteLater();
	}
}
void QTcpClientTest::displayError(QAbstractSocket::SocketError /*err*/)
{
	QTcpSocket * sock = qobject_cast<QTcpSocket *> (sender());
	if (sock)
	{
		QGHSslClient * pSockSsl = qobject_cast<QGHSslClient*>(sock);
		QGHTcpClient * pSockTcp = qobject_cast<QGHTcpClient*>(sock);
		if (pSockSsl)
			displayMessage(QString("client %1 error msg:").arg(pSockSsl->uuid())+sock->errorString());
		else if (pSockTcp)
			displayMessage(QString("client %1 error msg:").arg(pSockTcp->uuid())+sock->errorString());
	}
}
void QTcpClientTest::new_data_recieved()
{
	QTcpSocket * pSock = qobject_cast<QTcpSocket*>(sender());
	if (pSock)
	{
		QByteArray array =pSock->readAll();
		if (array.size() <= sizeof(EXAMPLE_HEARTBEATING))
			return;
		//in this example, we just do nothing but to display the byte size.
		QGHSslClient * pSockSsl = qobject_cast<QGHSslClient*>(pSock);
		QGHTcpClient * pSockTcp = qobject_cast<QGHTcpClient*>(pSock);
		if (pSockSsl)
			displayMessage(QString("client %1 Recieved %2 bytes.").arg(pSockSsl->uuid()).arg(array.size()));
		else if (pSockTcp)
			displayMessage(QString("client %1 Recieved %2 bytes.").arg(pSockTcp->uuid()).arg(array.size()));

	}
}

void QTcpClientTest::timerEvent(QTimerEvent * evt)
{
	static int nCount = 0;
	if (evt->timerId()==nTimer)
	{
		int nTotalClients = ui.dial->value();
		int nPayload = ui.horizontalSlider->value();
		QList<QTcpSocket*> listObj = m_clients.keys();
		nCount++;
		if (nCount % 100 == 0)
		{
			//send heart-beating
			foreach(QTcpSocket * pSock,listObj)
			{
				quint32 uuid = 0;
				QGHSslClient * pSockSsl = qobject_cast<QGHSslClient*>(pSock);
				QGHTcpClient * pSockTcp = qobject_cast<QGHTcpClient*>(pSock);
				if (pSockSsl)
					uuid = pSockSsl->uuid();
				else if (pSockTcp)
					uuid = pSockTcp->uuid();
				else
					continue;

				QByteArray array(sizeof(EXAMPLE_HEARTBEATING),0);
				char * ptr = array.data();
				EXAMPLE_HEARTBEATING * pMsg = (EXAMPLE_HEARTBEATING *)ptr;
				pMsg->Mark = 0xBEBE;
				pMsg->source_id = uuid;
				pMsg->tmStamp = 0;
				if (pSockSsl)
					pSockSsl->SendData(array);
				else if (pSockTcp)
					pSockTcp->SendData(array);
			}
		}
		foreach(QTcpSocket * sock,listObj)
		{
			if (rand()%1000<5)
			{
				quint32 uuid = 0;
				QGHSslClient * pSockSsl = qobject_cast<QGHSslClient*>(sock);
				QGHTcpClient * pSockTcp = qobject_cast<QGHTcpClient*>(sock);
				if (pSockSsl)
					uuid = pSockSsl->uuid();
				else if (pSockTcp)
					uuid = pSockTcp->uuid();
				else
					continue;
				quint16 nMsgLen = qrand()%(32)+nPayload-32-sizeof(EXAMPLE_TRANS_MSG);
				QByteArray array(sizeof(EXAMPLE_TRANS_MSG) + nMsgLen - 1,0);
				char * ptr = array.data();
				EXAMPLE_TRANS_MSG * pMsg = (EXAMPLE_TRANS_MSG *)ptr;
				pMsg->Mark = 0x55AA;
				pMsg->source_id = uuid;

				pMsg->destin_id = getRadomUUIDDestin();

				pMsg->data_length = nMsgLen;
				for (int i=0;i<nMsgLen;i++)
					pMsg->data[i] = '0' + i%10;

				if (pSockSsl)
					pSockSsl->SendData(array);
				else if (pSockTcp)
					pSockTcp->SendData(array);
			}
		}
		//
		if (rand()%100 <1)
			//if (m_clients.size()==0)
		{
			//1/10 chance to make new connections.
			if (m_clients.size()>nTotalClients)
			{
				int nDel = m_clients.size()-nTotalClients;
				QList<QTcpSocket*> listObj = m_clients.keys();
				for (int i=0;i<nDel;i++)
				{
					listObj.at(i)->abort();
				}
			}
			if (ui.checkBox_SSL->isChecked()==true)
			{
				QGHSslClient * client = new QGHSslClient(this,ui.horizontalSlider->value());
				//client->connectToHost(ui.lineEdit_ip->text(),ui.lineEdit_Port->text().toUShort());
				m_clients[client] = QDateTime::currentDateTime();
				connect(client, SIGNAL(readyRead()),this, SLOT(new_data_recieved()));
				//connect(client, SIGNAL(connected()),this, SLOT(on_client_connected()));
				connect(client, SIGNAL(disconnected()),this,SLOT(on_client_disconnected()));
				connect(client, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(displayError(QAbstractSocket::SocketError)));
				connect(client, SIGNAL(bytesWritten(qint64)), this, SLOT(on_client_trasferred(qint64)));
				connect(client, SIGNAL(encrypted()), this, SLOT(on_client_connected()));
				QString strCerPath =  QCoreApplication::applicationDirPath() + "/ca_cert.pem";
				QList<QSslCertificate> lstCas = QSslCertificate::fromPath(strCerPath);
				client->setCaCertificates(lstCas);
				client->connectToHostEncrypted(ui.lineEdit_ip->text(),ui.lineEdit_Port->text().toUShort());
			}
			else
			{
				QGHTcpClient * client = new QGHTcpClient(this,ui.horizontalSlider->value());
				m_clients[client] = QDateTime::currentDateTime();
				connect(client, SIGNAL(readyRead()),this, SLOT(new_data_recieved()));
				connect(client, SIGNAL(connected()),this, SLOT(on_client_connected()));
				connect(client, SIGNAL(disconnected()),this,SLOT(on_client_disconnected()));
				connect(client, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(displayError(QAbstractSocket::SocketError)));
				connect(client, SIGNAL(bytesWritten(qint64)), this, SLOT(on_client_trasferred(qint64)));
				client->connectToHost(ui.lineEdit_ip->text(),ui.lineEdit_Port->text().toUShort());
			}
		}
	}
}
void QTcpClientTest::displayMessage(QString str)
{
	model.insertRow(0,new QStandardItem(str));
	while (model.rowCount()>=128)
		model.removeRow(model.rowCount()-1);
}
quint32 QTcpClientTest::getRadomUUIDDestin()
{
	int nTotalClients = ui.dial->value();
	int nMinID = ((m_maxUUID - m_minUUID) < nTotalClients) ? m_minUUID:(m_maxUUID - nTotalClients);
	int nSpan = (m_maxUUID - nMinID + 1);
	int nChoose = rand() % nSpan;

	return nMinID + nChoose ;
}
void QTcpClientTest::on_toolButton_brFile_clicked()
{
	QString file = QFileDialog::getOpenFileName(this,tr("Open UUID global Syn File"));
	if (file.length()>0)
		ui.lineEdit_globalFile->setText(file);
}
