#ifndef QPIPE_H
#define QPIPE_H

#include "qgameelement.h"

class QPipe : public QGameElement
{
    Q_OBJECT
private:
    static int lastRandom;
    int pipeType;
    bool emited;
    const qreal randomY[2][6]={{0.0,-30.0,-100.0,-160.0,-50.0,-120.0},
                               {358.0,328.0,258.0,198.0,308.0,238.0}};
public:
    explicit QPipe(QObject *parent = 0);
    void draw(QPainter *);
    void logic();
    void setPipeType(int _type){pipeType=_type;}
signals:
    void scoreAdd();
public slots:

};

#endif // QPIPE_H
