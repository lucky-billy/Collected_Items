#include "qcclookline.h"
#include <QGraphicsScene>

QCCLookLine::QCCLookLine(QPainterPath path)
	: QGraphicsItem(0)
	, m_path(path)
	, m_iStatus(FADING)
	, m_iAlpha(0)
{
	m_lastTime = GetTickCount();
}

QCCLookLine::~QCCLookLine()
{

}

/// Ðéº¯ÊýÖØÔØ
QRectF QCCLookLine::boundingRect() const
{
	return m_path.boundingRect();
}

void QCCLookLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */)
{
	painter->setPen(QPen(QBrush(QColor(0, 0, 0, m_iAlpha)), 4));
	painter->drawPath(m_path);
}

void QCCLookLine::advance(int phase)
{
	if (m_iStatus == FADING)
	{
		int delta = GetTickCount() - m_lastTime;
		if (delta <= AnimationTime)
		{
			m_iAlpha = 255 - 255 * delta / AnimationTime ;
			update();
		}
		else
		{
			m_iStatus = UNVISIABLE;
			scene()->removeItem(this);
		}
	}
}
