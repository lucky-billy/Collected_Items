#include "zp_pltaskbase.h"
namespace ZPTaskEngine{
	zp_plTaskBase::zp_plTaskBase(QObject *parent) :
		QObject(parent)
	{
		refCount = 0;
	}
	int zp_plTaskBase::addRef()
	{
		QMutexLocker locker(&m_mutex_ref);
		refCount++;

		return refCount;
	}
	int zp_plTaskBase::delRef()
	{
		QMutexLocker locker(&m_mutex_ref);
		refCount--;

		return refCount;
	}

	int zp_plTaskBase::ref()
	{
		QMutexLocker locker(&m_mutex_ref);
		return refCount;
	}

	bool zp_plTaskBase::LockRun()
	{
		return m_mutex_run.tryLock();
	}

	void zp_plTaskBase::UnlockRun()
	{
		return m_mutex_run.unlock();
	}
}
