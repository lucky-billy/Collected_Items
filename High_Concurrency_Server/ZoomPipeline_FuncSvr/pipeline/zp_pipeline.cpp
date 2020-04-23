#include "zp_pipeline.h"
namespace ZPTaskEngine{
	zp_pipeline::zp_pipeline(QObject *parent) :
		QObject(parent)
	{
		m_nExistingThreads = 0;
	}
	bool zp_pipeline::canClose()
	{
		return m_nExistingThreads==0?true:false;
	}
	/**
	 * @brief Add nThreads to the thread pool
	 *
	 * @fn zp_pipeline::addThreads
	 * @param nThreads how many threads you want to add.
	 * @return int current threads count after add.
	 */
	int zp_pipeline::addThreads(int nThreads)
	{
		if (nThreads>=1 && nThreads <=128)
		{
			for (int i=0;i<nThreads;i++)
			{
				zp_plWorkingThread * thread = new zp_plWorkingThread(this);
				m_vec_workingThreads.push_back(thread);
				QThread * pTh = new QThread(this);
				m_vec_InternalworkingThreads.push_back(pTh);
				thread->moveToThread(pTh);
				connect (this,&zp_pipeline::evt_start_work,thread,&zp_plWorkingThread::FetchNewTask,Qt::QueuedConnection);
				connect (this,&zp_pipeline::evt_stop_work,thread,&zp_plWorkingThread::setStopMark,Qt::QueuedConnection);
				connect (thread,&zp_plWorkingThread::taskFinished,this,&zp_pipeline::on_finished_task,Qt::QueuedConnection);
				pTh->start();
				m_mutex_protect.lock();
				m_nExistingThreads++;
				m_mutex_protect.unlock();

			}

		}
		return m_vec_workingThreads.size();
	}

	/**
	 * @brief remove n threads and kill them.nthreads=-1 means kill all.
	 *
	 * @fn zp_pipeline::removeThreads
	 * @param nThreads how many threads to kill
	 * @return int the threads count after del
	 */
	int zp_pipeline::removeThreads(int nThreads)
	{
		int nsz =  m_vec_workingThreads.size();
		if (nThreads<0 || nThreads>nsz)
			nThreads = nsz;

		for (int i=0;i<nThreads;i++ )
		{
			emit evt_stop_work( m_vec_workingThreads.last());
			m_vec_workingThreads.pop_back();
			m_vec_InternalworkingThreads.pop_back();
		}
		return m_vec_workingThreads.size();
	}

	/**
	 * @brief  Threads call this function to get next task, task will be popped from list.
	 *
	 * @fn zp_pipeline::popTask
	 * @param bValid return whether this task is valid
	 * @return zp_plTaskBase the top-task in queue
	 */
	zp_plTaskBase * zp_pipeline::popTask( bool * bValid)
	{
		*bValid = false;
		zp_plTaskBase * funcres = 0;
		m_mutex_protect.lock();
		if (m_list_tasks.empty()==false)
		{
			funcres =* m_list_tasks.begin();
			m_list_tasks.pop_front();
			*bValid = true;
		}
		m_mutex_protect.unlock();
		return funcres;
	}

	/**
	 * @brief Call this function to insert an task into queue.
	 * task will be put into the tail of the fifo
	 * @fn zp_pipeline::pushTask
	 * @param task the pointer to this task object
	 * @param bFire if true, will fire an deal-event immediately
	 */
	void zp_pipeline::pushTask(zp_plTaskBase * task,bool bFire )
	{
		m_mutex_protect.lock();
		m_list_tasks.push_back(task);
		task->addRef();
		m_mutex_protect.unlock();

		int nsz =  m_vec_workingThreads.size();
		if (bFire==true)
			for (int i=0;i<nsz;i++ )
			{
				if (m_vec_workingThreads[i]->m_bBusy==false)
				{
					on_finished_task (m_vec_workingThreads[i]);
					break;
				}
			}

	}

	int zp_pipeline::threadsCount()
	{
		return m_vec_workingThreads.size();
	}

	int  zp_pipeline::payload()
	{
		int res = 0;
		m_mutex_protect.lock();
		res = m_list_tasks.size();
		m_mutex_protect.unlock();

		return res;
	}
	int  zp_pipeline::idleThreads()
	{
		int idle = 0;
		int nsz =  m_vec_workingThreads.size();
		for (int i=0;i<nsz;i++ )
		{
			if (m_vec_workingThreads[i]->m_bBusy==false)
				idle++;
		}
		return idle;
	}

	/**
	 * @brief this slot will be called when a task is finished,
	 * in this method, it will check whether the queue is still not empty
	 * and fire the event to deal with next job.
	 * @fn zp_pipeline::on_finished_task
	 * @param task
	 */
	void  zp_pipeline::on_finished_task (zp_plWorkingThread * task)
	{
		int res = 0;
		m_mutex_protect.lock();
		res = m_list_tasks.size();
		m_mutex_protect.unlock();
		if (res)
			emit evt_start_work(task );
	}
}
