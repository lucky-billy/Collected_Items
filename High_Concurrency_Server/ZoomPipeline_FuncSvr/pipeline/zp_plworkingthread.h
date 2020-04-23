#ifndef ZP_PLWORKINGTHREAD_H
#define ZP_PLWORKINGTHREAD_H

#include <QObject>
#include "zp_pltaskbase.h"
namespace ZPTaskEngine{

	class zp_pipeline;
	/**
	 * @brief /Working thread, reading functions from queue,
	 * call the tasks' "run" method, doing actual works.
	 * @class zp_plWorkingThread zp_plworkingthread.h "ZoomPipeline_FuncSvr/pipeline/zp_plworkingthread.h"
	 */
	class zp_plWorkingThread : public QObject
	{
		Q_OBJECT
	public:
		explicit zp_plWorkingThread(zp_pipeline * pipl,QObject *parent = 0);
		bool m_bBusy;
	protected:
		zp_pipeline * m_pipeline;

		bool m_bRuning;


	public slots:
		void setStopMark(zp_plWorkingThread *);

		void FetchNewTask(zp_plWorkingThread *);

	signals:

		void taskFinished(zp_plWorkingThread *);
	};
}
#endif // ZP_PLWORKINGTHREAD_H
