#include "frmmain.h"
#include "ui_frmmain.h"
#include "qffmpeg.h"
#include "rtspthread.h"
#include <QDebug>
#include <qpainter.h>
#include <QMouseEvent>
#include <qpropertyanimation.h>

#define BORDER_TOP 55
#define BORDER_RIGHT 26
#define BORDER_BOTTOM 23
#define BORDER_LEFT 23

frmMain::frmMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmMain)
{
    ui->setupUi(this);

    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(1000);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();

    //设置为无边框窗体
    this->setWindowFlags(Qt::FramelessWindowHint);
    //设置为背景透明
    this->setAttribute(Qt::WA_TranslucentBackground);
    //设置界面背景图
    m_pixmapBg.load(":/image/bg_main.png");
}

frmMain::~frmMain()
{
    delete ui;
}

void frmMain::paintEvent(QPaintEvent *e)
{    
    QPainter painter(this);
    //九宫格
    //画左上角
    painter.drawPixmap(QPoint(0, 0), m_pixmapBg, QRect(0, 0, BORDER_LEFT, BORDER_TOP));
    //画顶部
    painter.drawPixmap(QRect(BORDER_LEFT, 0, width() - BORDER_RIGHT - BORDER_LEFT, BORDER_TOP),
                       m_pixmapBg,
                       QRect(BORDER_LEFT, 0, m_pixmapBg.width() - BORDER_RIGHT - BORDER_LEFT, BORDER_TOP));
    //画右上部分
    painter.drawPixmap(QPoint(width() - BORDER_RIGHT, 0), m_pixmapBg,
                       QRect(m_pixmapBg.width() - BORDER_RIGHT, 0, BORDER_RIGHT, BORDER_TOP));
    //画左下部分
    painter.drawPixmap(QPoint(0, height() - BORDER_BOTTOM), m_pixmapBg,
                       QRect(0, m_pixmapBg.height() - BORDER_BOTTOM, BORDER_LEFT, BORDER_BOTTOM));
    //画底部分
    painter.drawPixmap(QRect(BORDER_LEFT, height() - BORDER_BOTTOM, width() - BORDER_RIGHT - BORDER_LEFT, BORDER_BOTTOM),
                       m_pixmapBg,
                       QRect(BORDER_LEFT, m_pixmapBg.height() - BORDER_BOTTOM, m_pixmapBg.width() - BORDER_RIGHT - BORDER_LEFT, BORDER_BOTTOM));
    //画右下部分
    painter.drawPixmap(QPoint(width() - BORDER_RIGHT, height() - BORDER_BOTTOM),
                       m_pixmapBg,
                       QRect(m_pixmapBg.width() - BORDER_RIGHT, m_pixmapBg.height() - BORDER_BOTTOM, BORDER_RIGHT, BORDER_BOTTOM));
    //画左部分
    painter.drawPixmap(QRect(0, BORDER_TOP, BORDER_LEFT, height() - BORDER_BOTTOM - BORDER_TOP),
                       m_pixmapBg,
                       QRect(0, BORDER_TOP, BORDER_LEFT, m_pixmapBg.height() - BORDER_BOTTOM - BORDER_TOP));
    //画右部分
    painter.drawPixmap(QRect(width() - BORDER_RIGHT, BORDER_TOP, BORDER_LEFT, height() - BORDER_BOTTOM - BORDER_TOP),
                       m_pixmapBg,
                       QRect(m_pixmapBg.width() - BORDER_RIGHT, BORDER_TOP, BORDER_LEFT, m_pixmapBg.height() - BORDER_BOTTOM - BORDER_TOP));
    //画中间部分
    painter.drawPixmap(QRect(BORDER_LEFT, BORDER_TOP, width() - BORDER_RIGHT - BORDER_LEFT, height() - BORDER_BOTTOM - BORDER_TOP),
                       m_pixmapBg,
                       QRect(BORDER_LEFT, BORDER_TOP, m_pixmapBg.width() - BORDER_RIGHT - BORDER_LEFT, m_pixmapBg.height() - BORDER_BOTTOM - BORDER_TOP));
}

void frmMain::mouseMoveEvent(QMouseEvent *e)
{
    this->move(e->globalPos() - m_pointStart);
}

void frmMain::mousePressEvent(QMouseEvent *e)
{
    m_pointPress = e->globalPos();
    m_pointStart = m_pointPress - this->pos();
}

void frmMain::on_btnOpen_clicked()
{
    QStringList tempURL = ui->txtUrl->text().split(";");
    int count = tempURL.count();

    if (count > 0 && tempURL[0].length() > 0) {
        ffmpeg1 = new QFFmpeg(this);
        connect(ffmpeg1, SIGNAL(GetImage(QImage)), this, SLOT(SetImage1(QImage)));
        ffmpeg1->SetUrl(tempURL[0]);

        if (ffmpeg1->Init()) {
            RtspThread *rtsp = new RtspThread(this);
            rtsp->setffmpeg(ffmpeg1);
            rtsp->start();
        }
    }

    if (count > 1 && tempURL[1].length() > 0) {
        ffmpeg2 = new QFFmpeg(this);
        connect(ffmpeg2, SIGNAL(GetImage(QImage)), this, SLOT(SetImage2(QImage)));
        ffmpeg2->SetUrl(tempURL[1]);

        if (ffmpeg2->Init()) {
            RtspThread *rtsp = new RtspThread(this);
            rtsp->setffmpeg(ffmpeg2);
            rtsp->start();
        }
    }
}

int tempWidth = 480;
int tempHeight = 320;
void frmMain::SetImage1(const QImage &image)
{
    tempWidth = ui->labVideo1->geometry().width();
    tempHeight = ui->labVideo1->geometry().height();
    if (image.height() > 0) {
        QPixmap pix = QPixmap::fromImage(image.scaled(tempWidth, tempHeight));
        ui->labVideo1->setPixmap(pix);
    }
}

void frmMain::SetImage2(const QImage &image)
{
    tempWidth = ui->labVideo2->geometry().width();
    tempHeight = ui->labVideo2->geometry().height();
    if (image.height() > 0) {
        QPixmap pix = QPixmap::fromImage(image.scaled(tempWidth, tempHeight));
        ui->labVideo2->setPixmap(pix);
    }
}

void frmMain::removelayout1()
{
    ui->lay1->removeWidget(ui->labVideo1);
    ui->lay1->removeWidget(ui->labVideo2);
    ui->lay1->removeWidget(ui->labVideo3);
    ui->lay1->removeWidget(ui->labVideo4);

    ui->labVideo1->setVisible(false);
    ui->labVideo2->setVisible(false);
    ui->labVideo3->setVisible(false);
    ui->labVideo4->setVisible(false);
}

void frmMain::removelayout2()
{
    ui->lay2->removeWidget(ui->labVideo5);
    ui->lay2->removeWidget(ui->labVideo6);
    ui->lay2->removeWidget(ui->labVideo7);
    ui->lay2->removeWidget(ui->labVideo8);

    ui->labVideo5->setVisible(false);
    ui->labVideo6->setVisible(false);
    ui->labVideo7->setVisible(false);
    ui->labVideo8->setVisible(false);
}

void frmMain::removelayout3()
{
    ui->lay3->removeWidget(ui->labVideo9);
    ui->lay3->removeWidget(ui->labVideo10);
    ui->lay3->removeWidget(ui->labVideo11);
    ui->lay3->removeWidget(ui->labVideo12);

    ui->labVideo9->setVisible(false);
    ui->labVideo10->setVisible(false);
    ui->labVideo11->setVisible(false);
    ui->labVideo12->setVisible(false);
}

void frmMain::removelayout4()
{
    ui->lay4->removeWidget(ui->labVideo13);
    ui->lay4->removeWidget(ui->labVideo14);
    ui->lay4->removeWidget(ui->labVideo15);
    ui->lay4->removeWidget(ui->labVideo16);

    ui->labVideo13->setVisible(false);
    ui->labVideo14->setVisible(false);
    ui->labVideo15->setVisible(false);
    ui->labVideo16->setVisible(false);
}

void frmMain::on_btn1_clicked()
{
    removelayout1();
    removelayout2();
    removelayout3();
    removelayout4();

    ui->lay1->addWidget(ui->labVideo1);
    ui->labVideo1->setVisible(true);
}

void frmMain::on_btn4_clicked()
{
    removelayout1();
    removelayout2();
    removelayout3();
    removelayout4();

    ui->lay1->addWidget(ui->labVideo1);
    ui->labVideo1->setVisible(true);

    ui->lay1->addWidget(ui->labVideo2);
    ui->labVideo2->setVisible(true);

    ui->lay2->addWidget(ui->labVideo3);
    ui->labVideo3->setVisible(true);

    ui->lay2->addWidget(ui->labVideo4);
    ui->labVideo4->setVisible(true);
}

void frmMain::on_btn9_clicked()
{
    removelayout1();
    removelayout2();
    removelayout3();
    removelayout4();

    ui->lay1->addWidget(ui->labVideo1);
    ui->labVideo1->setVisible(true);
    ui->lay1->addWidget(ui->labVideo2);
    ui->labVideo2->setVisible(true);
    ui->lay1->addWidget(ui->labVideo3);
    ui->labVideo3->setVisible(true);

    ui->lay2->addWidget(ui->labVideo4);
    ui->labVideo4->setVisible(true);
    ui->lay2->addWidget(ui->labVideo5);
    ui->labVideo5->setVisible(true);
    ui->lay2->addWidget(ui->labVideo6);
    ui->labVideo6->setVisible(true);

    ui->lay3->addWidget(ui->labVideo7);
    ui->labVideo7->setVisible(true);
    ui->lay3->addWidget(ui->labVideo8);
    ui->labVideo8->setVisible(true);
    ui->lay3->addWidget(ui->labVideo9);
    ui->labVideo9->setVisible(true);
}

void frmMain::on_btn16_clicked()
{
    removelayout1();
    removelayout2();
    removelayout3();
    removelayout4();

    ui->lay1->addWidget(ui->labVideo1);
    ui->labVideo1->setVisible(true);
    ui->lay1->addWidget(ui->labVideo2);
    ui->labVideo2->setVisible(true);
    ui->lay1->addWidget(ui->labVideo3);
    ui->labVideo3->setVisible(true);
    ui->lay1->addWidget(ui->labVideo4);
    ui->labVideo4->setVisible(true);

    ui->lay2->addWidget(ui->labVideo5);
    ui->labVideo5->setVisible(true);
    ui->lay2->addWidget(ui->labVideo6);
    ui->labVideo6->setVisible(true);
    ui->lay2->addWidget(ui->labVideo7);
    ui->labVideo7->setVisible(true);
    ui->lay2->addWidget(ui->labVideo8);
    ui->labVideo8->setVisible(true);

    ui->lay3->addWidget(ui->labVideo9);
    ui->labVideo9->setVisible(true);
    ui->lay3->addWidget(ui->labVideo10);
    ui->labVideo10->setVisible(true);
    ui->lay3->addWidget(ui->labVideo11);
    ui->labVideo11->setVisible(true);
    ui->lay3->addWidget(ui->labVideo12);
    ui->labVideo12->setVisible(true);

    ui->lay4->addWidget(ui->labVideo13);
    ui->labVideo13->setVisible(true);
    ui->lay4->addWidget(ui->labVideo14);
    ui->labVideo14->setVisible(true);
    ui->lay4->addWidget(ui->labVideo15);
    ui->labVideo15->setVisible(true);
    ui->lay4->addWidget(ui->labVideo16);
    ui->labVideo16->setVisible(true);
}

void frmMain::on_btn_min_clicked()
{
    if (!this->isMinimized()) {
        this->showMinimized();
    }
}

void frmMain::on_btn_max_clicked()
{
    if (this->isMaximized()) {
        this->showNormal();
    } else {
        this->showMaximized();
    }
}

void frmMain::on_btn_close_clicked()
{
    qApp->exit();
}
