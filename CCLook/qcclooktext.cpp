#include "qcclooktext.h"

QCCLookText::QCCLookText(QString text, QRectF rect)
	: QGraphicsItem(0)
	, m_szText(text)
	, m_rect(rect)
{

}

QCCLookText::~QCCLookText()
{

}

/// Ðéº¯ÊýÖØÔØ
QRectF QCCLookText::boundingRect() const
{
	return m_rect;
}

void QCCLookText::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */)
{
	QFont font(QObject::tr("Impact"), 50);
	font.setStyleStrategy(QFont::PreferAntialias);
	painter->setFont(font);
	painter->drawText(m_rect, Qt::AlignCenter, m_szText);
}

void QCCLookText::advance(int phase)
{

}