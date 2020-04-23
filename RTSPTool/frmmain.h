#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QWidget>
#include "qffmpeg.h"

namespace Ui {
class frmMain;
}

class frmMain : public QWidget
{
    Q_OBJECT

public:
    explicit frmMain(QWidget *parent = 0);
    ~frmMain();

protected slots:
    void paintEvent(QPaintEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);

private slots:
    void SetImage1(const QImage &image);
    void SetImage2(const QImage &image);

    void on_btnOpen_clicked();

    void on_btn1_clicked();

    void on_btn4_clicked();

    void on_btn9_clicked();

    void on_btn16_clicked();

    void on_btn_min_clicked();

    void on_btn_max_clicked();

    void on_btn_close_clicked();

private:
    Ui::frmMain *ui;

    QPixmap m_pixmapBg;
    QPoint m_pointStart;
    QPoint m_pointPress;

    QFFmpeg *ffmpeg1;
    QFFmpeg *ffmpeg2;

    void removelayout1();
    void removelayout2();
    void removelayout3();
    void removelayout4();
};

#endif // FRMMAIN_H
