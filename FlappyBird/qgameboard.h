#ifndef QGAMEBOARD_H
#define QGAMEBOARD_H

#include "qgameelement.h"

class QGameBoard : public QGameElement
{
    Q_OBJECT
private:
    qreal destY;
    qreal dY;
public:
    explicit QGameBoard(QObject *parent = 0);
    void draw(QPainter *);
    void logic();
    void setDestAndDelta(qreal _destY,qreal _dY){destY=_destY,dY=_dY;}
signals:
    void moveFinished();
public slots:

};

#endif // QGAMEBOARD_H
