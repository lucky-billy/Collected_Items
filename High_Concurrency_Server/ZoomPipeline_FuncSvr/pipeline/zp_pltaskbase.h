#ifndef ZP_PLTASKBASE_H
#define ZP_PLTASKBASE_H

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
namespace ZPTaskEngine{
	/**
	 * @brief the pure-base class for tasks , will be called by zp_plWorkingThread
	 * Users will imp your own int zp_plTaskBase::run() function.
	 */
	class zp_plTaskBase : public QObject
	{
		Q_OBJECT
	public:
		explicit zp_plTaskBase(QObject *parent = 0);

		/**
		 * @brief the virtual function called by zp_plWorkingThread
		 * this function will be called in thread pool
		 * @return int 0 means this object will be removed from pipeline-fifo. otherwise,
		 * when run() finished, the object will be re-injected into the tail of task-fifo.
		 */
		virtual int run() = 0;
		int addRef();
		int delRef();

		int ref();

		bool LockRun();
		void UnlockRun();

	private:
		int refCount;
		QMutex m_mutex_ref;
		QMutex m_mutex_run;
	signals:

	public slots:

	};
}
#endif // ZP_PLTASKBASE_H
