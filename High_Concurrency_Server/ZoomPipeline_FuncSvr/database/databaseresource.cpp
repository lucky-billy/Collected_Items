#include "databaseresource.h"
#include <QThread>
#include <QMutexLocker>
#include <QSqlError>
#include <QSqlQuery>
namespace ZPDatabase{


	DatabaseResource::DatabaseResource(QObject *parent) :
		QThread(parent)
	{
		bTerm = false;
	}

	/**
	 * @brief Get an database connection belong to current thread.
	 * if database does not exist, it will be added using dbtype
	 * @fn DatabaseResource::databse
	 * @param strDBName the database name
	 * @return QSqlDatabase return the database object
	 */
	QSqlDatabase  DatabaseResource::databse(QString  strDBName)
	{
		QMutexLocker locker(&m_mutex_reg);
		if (false==QSqlDatabase::contains(strDBName))
		{
			QString msg =  "Database:"+tr(" Connection name ")+strDBName+ tr(" does not exist.");
			emit evt_Message(this,msg);
			return QSqlDatabase();
		}
		return  QSqlDatabase::database(strDBName);
	}
	void DatabaseResource::remove_connections()
	{
		QMap<QString,tagConnectionPara> sets;
		{
			QMutexLocker locker(&m_mutex_reg);
			sets = currentDatabaseConnections();
		}

		foreach (QString name, sets.keys())
			this->remove_connection(name);
	}

	//!Remove Database
	void DatabaseResource::remove_connection(QString  strDBName)
	{
		QMutexLocker locker(&m_mutex_reg);
		if (true==QSqlDatabase::contains(strDBName))
		{
			QSqlDatabase db = QSqlDatabase::database(strDBName);
			if (db.isOpen()==true)
				db.close();
			QSqlDatabase::removeDatabase(strDBName);
			QString msg = "Database:"+tr(" Connection removed ")+strDBName+ tr(" .");
			emit evt_Message(this,msg);

		}
		else
		{
			QString msg = "Database:"+tr(" Connection name ")+strDBName+ tr(" does not exist.");
			emit evt_Message(this,msg);
		}
		m_dbNames.remove(strDBName) ;

	}
	/**
	 * @brief add a database connection resource
	 *
	 * @fn DatabaseResource::addConnection
	 * @param connName the user-specified name standing for this connection
	 * @param type the Qt-Sql database driver name, QPSQL, QMYSQL, etc.
	 * @param HostAddr the host address will connect to.
	 * @param port the port will connect to.
	 * @param dbName the database schema name
	 * @param User username
	 * @param Pass password
	 * @param ExtraOptions some extra options.
	 * @param testSQL if this para is not empty, confirmConnection will call this SQL to confirm the db is OK,
	 * for example, select 1+1 , will return 2 if db is ok.
	 * @return bool succeed : true
	 */
	bool DatabaseResource::addConnection(
			QString  connName,
			QString  type,
			QString  HostAddr,
			int port,
			QString  dbName,
			QString  User,
			QString  Pass,
			QString  ExtraOptions,
			QString  testSQL
			)
	{
		QMutexLocker locker(&m_mutex_reg);
		tagConnectionPara para;
		para.connName = connName;
		para.type = type;
		para.HostAddr = HostAddr;
		para.port = port;
		para.dbName = dbName;
		para.User = User;
		para.Pass = Pass;
		para.status = true;
		para.testSQL = testSQL;
		para.ExtraOptions = ExtraOptions;

		if (true==QSqlDatabase::contains(connName))
		{
			QSqlDatabase db = QSqlDatabase::database(connName);
			if (db.isOpen()==true)
				db.close();
			QSqlDatabase::removeDatabase(connName);
			QString msg = "Database:"+tr(" Connection removed ")+connName+ tr(" .");
			emit evt_Message(this,msg);
		}

		m_dbNames[connName] = para;
		QSqlDatabase db = QSqlDatabase::addDatabase(type,connName);
		db.setHostName(HostAddr);
		db.setPort(port);
		db.setDatabaseName(dbName);
		db.setUserName(User);
		db.setPassword(Pass);
		db.setConnectOptions(ExtraOptions);
		if (db.open()==true)
		{
			QString msg ="Database:"+ tr(" Connection  ")+connName+ tr(" Established.");
			emit evt_Message(this,msg);
			return true;
		}
		QString msg = "Database:"+tr(" Connection  ")+connName+ tr(" Can't be opened. MSG=");
		msg += db.lastError().text();
		emit evt_Message(this,msg);
		QSqlDatabase::removeDatabase(connName);
		m_dbNames.remove(connName) ;
		return false;
	}
	/**
	 * @brief this method runs in a special guarding thread
	 * to confirm the connection resource is still OK
	 * see  DatabaseResource::run()
	 * @fn DatabaseResource::confirmConnection
	 * @param connName the connection name which to be tested.
	 * @return bool the check result.
	 */
	bool DatabaseResource::confirmConnection (QString  connName)
	{
		QMutexLocker locker(&m_mutex_reg);
		if (false==m_dbNames.contains(connName))
		{
			QString msg = "Database:"+tr(" Connection ")+connName+ tr(" has not been added.");
			emit evt_Message(this,msg);
			return false;
		}
		tagConnectionPara & para = m_dbNames[connName];
		if (true==QSqlDatabase::contains(connName)  )
		{
			QSqlDatabase db = QSqlDatabase::database(connName);
			if (db.isOpen()==true)
			{
				bool bNeedDisconnect = false;
				if (para.testSQL.length())
				{
					QSqlQuery query(db);
					query.exec(para.testSQL);
					if (query.lastError().type()!=QSqlError::NoError)
					{
						QString msg = "Database:"+tr(" Connection  ")+connName+ tr(" confirm failed. MSG=");
						msg += query.lastError().text();
						emit evt_Message(this,msg);
						bNeedDisconnect = true;
					}
				}
				if (bNeedDisconnect==true)
				{
					db.close();
					QSqlDatabase::removeDatabase(connName);
					return false;
				}
				else
					return true;
			}
			QString msg = "Database:"+tr(" Connection ")+connName+ tr(" has not been opened.");
			emit evt_Message(this,msg);
			db = QSqlDatabase::addDatabase(para.type,para.connName);
			db.setHostName(para.HostAddr);
			db.setPort(para.port);
			db.setDatabaseName(para.dbName);
			db.setUserName(para.User);
			db.setPassword(para.Pass);
			db.setConnectOptions(para.ExtraOptions);
			if (db.open()==true)
			{
				para.status = true;
				para.lastError = "";
				msg = "Database:"+tr(" Connection  ")+connName+ tr(" Re-Established.");
				emit evt_Message(this,msg);
				return true;
			}
			QSqlDatabase::removeDatabase(connName);
			msg ="Database:"+ tr(" Connection  ")+connName+ tr(" Can't be opened. MSG=");
			msg += db.lastError().text();
			emit evt_Message(this,msg);
			para.status = false;
			para.lastError = db.lastError().text();
			return false;
		}

		QSqlDatabase db = QSqlDatabase::addDatabase(para.type,para.connName);
		db.setHostName(para.HostAddr);
		db.setPort(para.port);
		db.setDatabaseName(para.dbName);
		db.setUserName(para.User);
		db.setPassword(para.Pass);
		db.setConnectOptions(para.ExtraOptions);
		if (db.open()==true)
		{
			para.status = true;
			para.lastError = "";
			QString msg ="Database:"+ tr(" Connection  ")+connName+ tr(" Re-Established.");
			emit evt_Message(this,msg);
			return true;
		}
		QString msg ="Database:"+ tr(" Connection  ")+connName+ tr(" Can't be opened. MSG=");
		msg += db.lastError().text();
		emit evt_Message(this,msg);
		QSqlDatabase::removeDatabase(connName);
		para.status = false;
		para.lastError = db.lastError().text();
		return false;
	}

	void DatabaseResource::run()
	{
		while(bTerm==false)
		{
			QMap<QString,tagConnectionPara> sets;
			{
				QMutexLocker locker(&m_mutex_reg);
				sets = currentDatabaseConnections();
			}

			foreach (QString name, sets.keys())
			{
				confirmConnection(name) ;
				if (bTerm==true)
					break;
			}
			if (bTerm==false)
				QThread::currentThread()->msleep(30000);
		}

	}

};
