#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    // 初始化最高分为0
    highScore = 0;
    // 读取存储最高分的文件
    QFile file("score.j");
    if (file.open(QIODevice::ReadOnly))
    {
        file.read((char *)&highScore, sizeof(highScore));
        file.close();
    }

    // 构造一个游戏部件对象
    gameWidget = new GameWidget(this);
    gameWidget->setGeometry(2, 200, 400, 400);
    connect(gameWidget, SIGNAL(ScoreInc(int)), this, SLOT(onScoreInc(int)));
    connect(gameWidget, SIGNAL(GameOver()), this, SLOT(onGameOver()));
    connect(gameWidget, SIGNAL(win()), this, SLOT(onWin()));

    // 构造一个字体对象
    QFont font;
    font.setFamily("Consolas");
    font.setBold(true);
    font.setPixelSize(25);

    // 构造一个按钮对象
    restartBtn = new QPushButton("Restart", this);
    restartBtn->setGeometry(100, 120, 200, 50);
    restartBtn->setFont(font);
    restartBtn->setStyleSheet("QPushButton {color: blue}");
    connect(restartBtn, SIGNAL(clicked()), gameWidget, SLOT(restart()));

    // 构造一个标签对象
    highScoreLbl = new QLabel(QString("High Score:\n %1").arg(highScore),this);
    highScoreLbl->setGeometry(220, 30, 150, 50);
    highScoreLbl->setFont(font);
    highScoreLbl->setStyleSheet("QLabel {color: red}");

    // 构造一个标签对象
    scoreLbl = new QLabel("Score:\n 0", this);
    scoreLbl->setGeometry(30, 30, 150, 50);
    scoreLbl->setFont(font);
    scoreLbl->setStyleSheet("QLabel {color: red}");

    // 重置窗口大小
    resize(404, 606);
}

Widget::~Widget()
{
    // 释放相关对象
    delete restartBtn;
    delete scoreLbl;
    delete highScoreLbl;
    delete gameWidget;
}

void Widget::onScoreInc(int score)
{
    // 更新分数显示
    scoreLbl->setText(QString("Score:\n %1").arg(score));
    // 如果当前分数高于最高分
    if (score > highScore)
    {
        // 更新最高分
        highScore = score;
        highScoreLbl->setText(QString("High Score:\n %1").arg(highScore));

        // 将新的最高分存入文件
        QFile file("score.j");
        file.open(QIODevice::WriteOnly);
        file.write((char *)&highScore, sizeof(highScore));
        file.close();
    }
}

void Widget::onGameOver()
{
    QMessageBox::information(this, "GameOver", "You lost !");
}

void Widget::onWin()
{
    QMessageBox::information(this, "Congratulation", "You win !");
}

void Widget::resizeEvent(QResizeEvent *)
{
    // 计算宽度和高度的缩放比例
    ratioW = width() / 404.0f;
    ratioH = height() / 606.0f;
    // 重置子部件大小和位置
    gameWidget->setGeometry(2 * ratioW, 200 * ratioH, 400 * ratioW, 400 * ratioH);
    restartBtn->setGeometry(100 * ratioW, 120 * ratioH, 200 * ratioW, 50 * ratioH);
    highScoreLbl->setGeometry(220 * ratioW, 30 * ratioH, 150 * ratioW, 50 * ratioH);
    scoreLbl->setGeometry(30 * ratioW, 30 * ratioH, 150 * ratioW, 50 * ratioH);
}
