#include "qguide.h"

QGuide::QGuide(QObject *parent) :
    QGameElement(parent)
{
    curFrame=0;
    elapsedFrame=0;
}

void QGuide::draw(QPainter *painter)
{
    painter->drawPixmap(bindRect.topLeft(),pixmapList[curFrame]);
}

void QGuide::logic()
{
    if (!enabled)
        return;
    elapsedFrame++;
    if (elapsedFrame%3==0)
        curFrame++;
    if (curFrame==frameCount)
    {
        curFrame=0;
        elapsedFrame=0;
    }
}
