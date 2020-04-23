#ifndef QGUIDE_H
#define QGUIDE_H

#include "qgameelement.h"

class QGuide : public QGameElement
{
    Q_OBJECT
private:
    int curFrame;
    int elapsedFrame;
public:
    explicit QGuide(QObject *parent = 0);
    void draw(QPainter *);
    void logic();
signals:

public slots:

};

#endif // QGUIDE_H
