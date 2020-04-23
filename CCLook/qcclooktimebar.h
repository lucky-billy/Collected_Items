/************************************************************
**
** @Name: 倒计时
** @Description: 时间进度条的显示及控制
** @Date: 2013/04/19 19:32
**
*************************************************************/

#ifndef QCCLOOKTIMEBAR_H
#define QCCLOOKTIMEBAR_H

#include <QGraphicsItem>
#include <QtWidgets>
#include "globaldefines.h"

class QCCLookTimeBar : public QGraphicsItem
{

public:
	enum status {INIT, START, FINISHED};
	QCCLookTimeBar(QRectF rect, int timecount);
	~QCCLookTimeBar();

/// 接口
public:
	// 开始计时
	void start();
	// 时间是否耗尽
	bool isFinished();
	// 重置
	void reset();
	// 获取剩余时间
	int getLastTime()
	{
		return m_iTimeCount;
	}

/// 虚函数重载
public:
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */);
	void advance(int phase);

private:
	QRectF m_rect;				// 显示区域
	int m_iOriTime;				// 总时间
	int m_iTimeCount;			// 剩余时间
	QTime m_lastTime;			// 用于计时
	int m_iStatus;				// 标识计时是否结束
};

#endif // QCCLOOKTIMEBAR_H
