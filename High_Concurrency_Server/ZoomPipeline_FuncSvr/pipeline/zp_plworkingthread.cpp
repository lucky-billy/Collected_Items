#include "zp_plworkingthread.h"
#include <assert.h>
#include "zp_pipeline.h"
namespace ZPTaskEngine{
	zp_plWorkingThread::zp_plWorkingThread(zp_pipeline * pipl,QObject *parent) :
		QObject(parent)
	{
		m_bRuning = true;
		m_pipeline = pipl;
		assert(m_pipeline != NULL);
		m_bBusy = false;
	}

	/**
	 * @brief This slot mark this zp_plWorkingThread object
	 * as stopped. when tasks belong to this zp_plWorkingThread has been
	 * finished, it will quit.
	 *
	 * @fn zp_plWorkingThread::setStopMark
	 * @param obj the zp_plWorkingThread object recieved by signal-slot system.
	 * this method will omit zp_plWorkingThread objs except for it self.
	 */
	void zp_plWorkingThread::setStopMark(zp_plWorkingThread * obj)
	{
		if (obj != this)
			return;
		m_bRuning = false;
		m_pipeline->m_mutex_protect.lock();
		m_pipeline->m_nExistingThreads--;
		m_pipeline->m_mutex_protect.unlock();
		this->deleteLater();
		QThread::currentThread()->quit();
	}

	/**
	 * @brief Call zp_plTaskBase::popTask to fetch new tasks.
	 *
	 * @fn zp_plWorkingThread::FetchNewTask
	 * @param obj the zp_plWorkingThread object recieved by signal-slot system.
	 * this method will omit zp_plWorkingThread objs except for it self.
	 */
	void zp_plWorkingThread::FetchNewTask(zp_plWorkingThread * obj)
	{


		if (obj != this)
			return;
		if (m_bRuning)
		{

			bool bValid = false;
			zp_plTaskBase * ptr = this->m_pipeline->popTask(&bValid);

			if (bValid==true && ptr!=NULL)
			{
				m_bBusy = true;
				if (ptr->LockRun()==true)
				{
					int res = ptr->run();
					ptr->delRef();
					if (res!=0 )
						this->m_pipeline->pushTask(ptr,false);
					ptr->UnlockRun();
				}
				else
				{
					ptr->delRef();
					this->m_pipeline->pushTask(ptr,false);
				}
				m_bBusy = false;

			}

			emit taskFinished(this);

		}

	}

}
