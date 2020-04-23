#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QMouseEvent>
#include <QLCDNumber>
#include <QPushButton>
#include "qbird.h"
#include "qguide.h"
#include "qground.h"
#include "qpipe.h"
#include "qgameboard.h"

#define GAME_PLAYING 0x1
#define GAME_PAUSED  0x2
#define GAME_OVER    0x4

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QBird *bird;//鸟
    QGuide *guide;//引导界面
    QGround *ground;//地面
    QPipe *pipe[4];//管道
    QGameBoard *gameover;//游戏结束板
    QGameBoard *scoreBoard;//分数板
    QList<QGameElement*> Elementlist;//游戏元素列表
    QTimer timer;//计时器
    QLCDNumber *lcdScore;//显示分数
    QPushButton *btnReplay;//重新开始按钮
    QPushButton *btnRank;//排名按钮
    QPixmap pixmapBkgnd;//游戏背景
    int score;//游戏分数
    uint gameStatus;//游戏状态
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
public slots:
    void gameOver();//游戏结束
    void gameScoreAdd();//游戏加分
    void board1Stop();
    void board2Stop();
    void replay();
    void rank();
};

#endif // MAINWINDOW_H
