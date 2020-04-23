/************************************************************
**
** @Name: 分数
** @Description: 分数统计及显示
** @Date: 2013/04/19 19:30
**
*************************************************************/

#ifndef QCCLOOKSCORE_H
#define QCCLOOKSCORE_H

#include <QGraphicsItem>
#include <QtWidgets>
#include "globaldefines.h"

class QCCLookScore : public QGraphicsItem
{

public:
	QCCLookScore(QRectF rect);
	~QCCLookScore();

/// 接口
public:
	// 增加分数
	void addScore(int delta)
	{
		m_iScore = (m_iScore + delta > 0) ? m_iScore + delta : 0;
		update();
	}
	// 获取分数
	int getScore() const
	{
		return m_iScore;
	}
	// 重置分数
	int reset()
	{
		m_iScore = 0;
		update();
	}

/// 虚函数重载
public:
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */);
	void advance(int phase);

/// 私有函数
private:
	// 通过数字获取图片指针
	void getPixmapFromNum(int num, QPixmap*& first, QPixmap*& second);

private:
	QRectF m_rect;
	int m_iScore;			// 分数
	QPixmap m_numPic[10];	// 数字图片
};

#endif // QCCLOOKSCORE_H
