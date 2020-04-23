#include "qcclookscore.h"

QCCLookScore::QCCLookScore(QRectF rect)
	: QGraphicsItem(0)
	, m_rect(rect)
	, m_iScore(0)
{
	for (int i = 0; i != 10; i++)
	{
		m_numPic[i].load(QString(":/digit/%1.png").arg(i));
	}
}

QCCLookScore::~QCCLookScore()
{

}

/// 虚函数重载
QRectF QCCLookScore::boundingRect() const
{
	return m_rect;
}

void QCCLookScore::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	QRectF m_firstRect = QRectF(m_rect.left() + 30, m_rect.top(), m_rect.width() / 2 - 35, m_rect.height());
	QRectF m_secondRect = QRectF(m_rect.left() + m_rect.width() / 2, m_rect.top(), m_rect.width() / 2 - 35, m_rect.height());

	QPixmap* firstPixmap = NULL, *secondPixmap = NULL;
	getPixmapFromNum(m_iScore, firstPixmap, secondPixmap);

	painter->setRenderHint(QPainter::Antialiasing);
	painter->drawPixmap(m_firstRect, *firstPixmap, firstPixmap->rect());
	painter->drawPixmap(m_secondRect, *secondPixmap, secondPixmap->rect());
}

void QCCLookScore::advance(int phase)
{

}

/// 私有函数
void QCCLookScore::getPixmapFromNum(int num, QPixmap*& first, QPixmap*& second)
{
	first = m_numPic + num / 10;
	second = m_numPic + (num - num / 10 * 10);
}