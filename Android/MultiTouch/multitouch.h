#ifndef MULTITOUCH_H
#define MULTITOUCH_H

#include <QMainWindow>
#include <QTouchEvent>
#include <QVector>
#include <QLineF>

namespace Ui {
class MultiTouch;
}

class MultiTouch : public QMainWindow
{
    Q_OBJECT

public:
    explicit MultiTouch(QWidget *parent = 0);
    ~MultiTouch();

    bool event(QEvent *e);
    void paintEvent(QPaintEvent *e);

private slots:
    void on_pushButton_clicked();

private:
    void addLine(QTouchEvent *e);

private:
    Ui::MultiTouch *ui;

    QVector<QLineF> lines;
};

#endif // MULTITOUCH_H
