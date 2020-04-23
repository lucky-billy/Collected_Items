#include "qpipe.h"

int QPipe::lastRandom=0;

QPipe::QPipe(QObject *parent) :
    QGameElement(parent)
{
    pipeType=0;
    emited=false;
}

void QPipe::draw(QPainter *painter)
{
    painter->drawPixmap(bindRect.topLeft(),pixmapList[0]);
}

void QPipe::logic()
{
    if (!enabled)
        return;
    bindRect.translate(-6.0,0.0);
    if (bindRect.x()<70)
        if (!emited)
        {
            emit scoreAdd();
            emited=true;
        }
    if (bindRect.x()<-70)
    {
        bindRect.translate(500.0,randomY[pipeType][pipeType==0?lastRandom=qrand()%6:lastRandom]-bindRect.y());
        emited=false;
    }
}
