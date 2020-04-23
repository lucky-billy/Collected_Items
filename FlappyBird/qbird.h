#ifndef QBIRD_H
#define QBIRD_H

#include "qgameelement.h"
#include <qmath.h>
#include <QList>

class QBird : public QGameElement
{
    Q_OBJECT
private:
    int curFrame;
    int elapsedFrame;
    qreal vy;
    QList<QGameElement*> *elementList;
public:
    explicit QBird(QList<QGameElement*>*,QObject *parent = 0);
    void draw(QPainter *);
    void logic();
    void birdUp();
    void setVy(qreal);
signals:
    void birdDead();
public slots:

};

#endif // QBIRD_H
