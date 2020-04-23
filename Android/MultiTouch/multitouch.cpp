#include "multitouch.h"
#include "ui_multitouch.h"

#include <QTouchEvent>
#include <QLineF>
#include <QPainter>

MultiTouch::MultiTouch(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MultiTouch)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_AcceptTouchEvents, true);
}

MultiTouch::~MultiTouch()
{
    delete ui;
}

bool MultiTouch::event(QEvent *e)
{
    switch (e->type()) {
    case QEvent::TouchBegin:    return true;
    case QEvent::TouchUpdate:
    case QEvent::TouchEnd:
    {
        QTouchEvent *te = static_cast<QTouchEvent *>(e);
        this->addLine(te);
        return true;
    }
    default: ;
    }

    return QMainWindow::event(e);
}

void MultiTouch::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawLines(this->lines);
    painter.end();

    return QMainWindow::paintEvent(e);
}

void MultiTouch::addLine(QTouchEvent *e)
{
    QLineF line;
    QList<QTouchEvent::TouchPoint> touchPoints = e->touchPoints();
    for (QTouchEvent::TouchPoint i : touchPoints){
        line.setP1(i.lastPos());
        line.setP2(i.pos());
        this->lines.push_back(line);
    }

    this->update();
}

void MultiTouch::on_pushButton_clicked()
{
    this->lines.clear();
    this->update();
}
