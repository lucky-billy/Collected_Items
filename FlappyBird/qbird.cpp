#include "qbird.h"

QBird::QBird(QList<QGameElement *>* lpList, QObject *parent) :
    QGameElement(parent)
{
    elementList=lpList;
    curFrame=0;
    elapsedFrame=0;
    vy=2.0;
}

void QBird::setVy(qreal _vy)
{
    vy=_vy;
}

void QBird::birdUp()
{
    vy=-7.0;
}

void QBird::draw(QPainter *painter)
{
    painter->save();
    painter->translate(bindRect.center());
    painter->rotate(qAtan2(vy,6)*180.0/3.14159);
    painter->translate(-bindRect.center());
    painter->drawPixmap(bindRect.topLeft(),pixmapList[curFrame]);
    painter->restore();
}

void QBird::logic()
{
    if (!enabled)
        return;
    bindRect.translate(0.0,vy);
    vy+=0.5;
    if (bindRect.y()<2.0)
        emit birdDead();
    for (QGameElement *e:*elementList)
        if (e!=this)
            if (bindRect.intersects(e->getBindRect()))
                emit birdDead();
    elapsedFrame++;
    if (elapsedFrame%3==0)
        curFrame++;
    if (curFrame==frameCount)
    {
        curFrame=0;
        elapsedFrame=0;
    }
}
