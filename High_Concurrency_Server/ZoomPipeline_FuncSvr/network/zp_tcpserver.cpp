#include "zp_tcpserver.h"
#include <assert.h>
namespace ZPNetwork{
	ZP_TcpServer::ZP_TcpServer(QObject *parent )
		: QTcpServer(parent)
	{

	}

	void ZP_TcpServer::incomingConnection(qintptr socketDescriptor)
	{
		emit evt_NewClientArrived(socketDescriptor);
	}
}
