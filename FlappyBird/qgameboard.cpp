#include "qgameboard.h"

QGameBoard::QGameBoard(QObject *parent) :
    QGameElement(parent)
{
}

void QGameBoard::draw(QPainter *painter)
{
    painter->drawPixmap(bindRect.topLeft(),pixmapList[0]);
}

void QGameBoard::logic()
{
    if (!enabled)
        return;
    bindRect.translate(0.0,dY);
    if (dY>0?bindRect.y()>destY:bindRect.y()<destY)
    {
        setEnabled(false);
        emit moveFinished();
    }
}
