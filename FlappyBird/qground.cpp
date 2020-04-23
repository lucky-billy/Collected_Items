#include "qground.h"

QGround::QGround(QObject *parent) :
    QGameElement(parent)
{
    xFrame1=-1;
    xFrame2=xMax;
}

void QGround::draw(QPainter *painter)
{
    painter->drawPixmap(xFrame1,yMax,pixmapList[0]);
    painter->drawPixmap(xFrame2,yMax,pixmapList[1]);
}

void QGround::logic()
{
    if (!enabled)
        return;
    xFrame1+=dX;
    xFrame2+=dX;
    if (xFrame1<-xMax)
        xFrame1=xMax;
    if (xFrame2<-xMax)
        xFrame2=xMax;
}

