/************************************************************
**
** @Name: 文本
** @Description: 显示文本
** @Date: 2013/04/19 19:32
**
*************************************************************/

#ifndef QCCLOOKTEXT_H
#define QCCLOOKTEXT_H

#include <QGraphicsItem>
#include <QtWidgets>

#include "globaldefines.h"

class QCCLookText : public QGraphicsItem
{

public:
	QCCLookText(QString text, QRectF rect);
	~QCCLookText();

/// 接口
public:
	void setText(QString text)
	{
		m_szText = text;
		update();
	}

/// 虚函数重载
public:
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */);
	void advance(int phase);

private:
	QString m_szText;
	QRectF m_rect;
};

#endif // QCCLOOKTEXT_H
