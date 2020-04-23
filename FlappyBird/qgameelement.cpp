#include "qgameelement.h"

QGameElement::QGameElement(QObject *parent) :
    QObject(parent)
{
    frameCount=0;
    enabled=true;
}

void QGameElement::addFrame(QPixmap pixmap)
{
    if (pixmap.isNull())
        return;
    pixmapList.append(pixmap);
    frameCount++;
}

void QGameElement::clearAllFrame()
{
    pixmapList.clear();
    frameCount=0;
}

QRectF& QGameElement::getBindRect()
{
    return bindRect;
}

void QGameElement::setRect(qreal x, qreal y, qreal w, qreal h)
{
    bindRect.setRect(x,y,w,h);
}

void QGameElement::setRect(QRectF rect)
{
    bindRect=rect;
}

void QGameElement::setEnabled(bool _enabled)
{
    enabled=_enabled;
}

