#include "qcclooktimebar.h"

QCCLookTimeBar::QCCLookTimeBar(QRectF rect, int timecount)
	: QGraphicsItem(0)
	, m_rect(rect)
	, m_iOriTime(timecount)
	, m_iTimeCount(timecount)
	, m_iStatus(INIT)
{

}

QCCLookTimeBar::~QCCLookTimeBar()
{

}

/// 接口
void QCCLookTimeBar::start()
{
	m_iStatus = START;
	m_lastTime = QTime::currentTime();
}

bool QCCLookTimeBar::isFinished()
{
	return ((m_iStatus == FINISHED) ? true : false);
}

void QCCLookTimeBar::reset()
{
	m_iStatus = INIT;
	m_iTimeCount = m_iOriTime;
}

/// 虚函数重载
QRectF QCCLookTimeBar::boundingRect() const
{
	return m_rect;
}

void QCCLookTimeBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */)
{
	QLinearGradient linearGrad(QPointF(0, m_rect.top()), QPointF(0, m_rect.bottom()));
	linearGrad.setColorAt(0, QColor(0, 255, 0, 100));
	linearGrad.setColorAt(0.5, QColor(0, 255, 0, 255));
	linearGrad.setColorAt(1, QColor(0, 255, 0, 100));
	painter->fillRect(m_rect, QBrush(Qt::darkGray));
	painter->fillRect(m_rect.left(), m_rect.top(), m_rect.width() * m_iTimeCount / m_iOriTime, m_rect.height(), QBrush(linearGrad));
	painter->setPen(QPen(QBrush(QColor(200, 200, 200, 255)), 1));
	painter->drawRect(m_rect);

}

void QCCLookTimeBar::advance(int phase)
{
	if ((m_iStatus == START) && ((QTime::currentTime().second() - m_lastTime.second() + 60) % 60 == 1))
	{
		m_lastTime = QTime::currentTime();
		if (!m_iTimeCount--)
			m_iStatus = FINISHED;
		update();
	}
}
