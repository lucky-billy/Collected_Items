#ifndef ZP_TcpServer_H
#define ZP_TcpServer_H

#include <QTcpServer>
namespace ZPNetwork{
	class ZP_TcpServer : public QTcpServer
	{
		Q_OBJECT

	public:
		ZP_TcpServer(QObject *parent);
	protected:
		void incomingConnection(qintptr socketDescriptor);
	signals:
		void evt_NewClientArrived(qintptr socketDescriptor);
	};
}
#endif // ZP_TcpServer_H
