/************************************************************
**
** @Name: 方块
** @Description: 用于连连看的方块的显示
** @Date: 2013/04/19 19:28
**
*************************************************************/

#ifndef QCCLOOKITEM_H
#define QCCLOOKITEM_H

#include <QGraphicsItem>
#include <QtWidgets>

#include "globaldefines.h"
#include "qcclscene.h"

class QCCLScene;

class QCCLookItem : public QGraphicsItem
{

public:
	// 正常，悬停，按下，消失中，不可见，寻路状态
	enum status{NORMAL, HOVER, TOGGLE, TIP, FADING, UNVISIABLE, TESTING};
	enum {BLOCK = UserType + 1};

public:
	QCCLookItem(QString fileName);
	~QCCLookItem();
	
/// 虚函数重载
public:
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */);
	void advance(int phase);
	int type () const 
	{
		return BLOCK;
	}

/// 事件
public:
	void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);

/// 接口
public:	
	void setToggle(bool status);
	bool isToggle() const;
	void setStatus(status eStatus)
	{
		m_iStatus = eStatus;
		// 记录时间
		if (m_iStatus == FADING)
			m_lastTime = GetTickCount();
		update();
	}
	int getStatus() const
	{
		return m_iStatus;
	}
	int getID() const
	{
		return m_iId;
	}
	void setID(int id)
	{
		m_iId = id;
	}
	void setIndex(int index)
	{
		m_index = index;
	}
	int getIndex() const
	{
		return m_index;
	}

private:
	// 画item
	void drawBlock(QPainter* painter, QColor color);
	
private:
	int m_iStatus;				// 普通，按下，悬停三种状态
	QPixmap m_pixmap;			// 方块上显示的图像
	int m_iAlpha;				// 用于填充背景色使图片逐渐消失
	int m_iId;					// 用于表示方块类别
	int m_index;				// 存储在m_pBlocks中的索引
	int m_lastTime;				// 时间记录，用于动画显示
};

#endif // QCCLOOKITEM_H
