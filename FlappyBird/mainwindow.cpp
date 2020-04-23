#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    score=0;
    gameStatus=GAME_OVER;
    setFixedSize(384,512);

    pixmapBkgnd.load(":/Images/bg.png");

    lcdScore=new QLCDNumber(3,this);
    lcdScore->setGeometry(152,40,80,50);
    lcdScore->setLineWidth(0);
    lcdScore->setStyleSheet("QLCDNumber{color: white}");
    lcdScore->setVisible(false);

    btnReplay=new QPushButton(this);
    btnReplay->setGeometry(42,400,140,78);
    btnReplay->setStyleSheet("QPushButton{border-image:url(:/Images/replay.png);}"
                             "QPushButton:pressed{margin: 2px 2px 2px 2px;}");
    btnReplay->setVisible(false);

    btnRank=new QPushButton(this);
    btnRank->setGeometry(202,400,140,78);
    btnRank->setStyleSheet("QPushButton{border-image:url(:/Images/rank.png);}"
                             "QPushButton:pressed{margin: 2px 2px 2px 2px;}");
    btnRank->setVisible(false);

    bird=new QBird(&Elementlist);
    bird->addFrame(QPixmap(":/Images/bird1.png"));
    bird->addFrame(QPixmap(":/Images/bird2.png"));
    bird->addFrame(QPixmap(":/Images/bird3.png"));

    guide=new QGuide;
    guide->addFrame(QPixmap(":/Images/guid1.png"));
    guide->addFrame(QPixmap(":/Images/guid2.png"));
    guide->addFrame(QPixmap(":/Images/guid3.png"));
    guide->setRect(0.0,0.0,0.0,0.0);

    ground=new QGround;
    ground->addFrame(QPixmap(":/Images/ground.png"));
    ground->addFrame(QPixmap(":/Images/ground.png"));
    ground->setRect(0.0,448.0,384.0,64.0);

    pipe[0]=new QPipe;
    pipe[0]->addFrame(QPixmap(":/Images/pipe1.png"));
    pipe[1]=new QPipe;
    pipe[1]->addFrame(QPixmap(":/Images/pipe2.png"));
    pipe[1]->setPipeType(1);
    pipe[2]=new QPipe;
    pipe[2]->addFrame(QPixmap(":/Images/pipe1.png"));
    pipe[3]=new QPipe;
    pipe[3]->addFrame(QPixmap(":/Images/pipe2.png"));
    pipe[3]->setPipeType(1);

    gameover=new QGameBoard;
    gameover->addFrame(QPixmap(":/Images/gameover.png"));
    gameover->setDestAndDelta(100.0,4.0);

    scoreBoard=new QGameBoard;
    scoreBoard->setDestAndDelta(200.0,-8.0);

    Elementlist.append(guide);
    Elementlist.append(ground);

    connect(bird,SIGNAL(birdDead()),this,SLOT(gameOver()));
    connect(pipe[0],SIGNAL(scoreAdd()),this,SLOT(gameScoreAdd()));
    connect(pipe[2],SIGNAL(scoreAdd()),this,SLOT(gameScoreAdd()));
    connect(gameover,SIGNAL(moveFinished()),this,SLOT(board1Stop()));
    connect(scoreBoard,SIGNAL(moveFinished()),this,SLOT(board2Stop()));
    connect(btnReplay,SIGNAL(clicked()),this,SLOT(replay()));
    connect(btnRank,SIGNAL(clicked()),this,SLOT(rank()));
    connect(&timer,SIGNAL(timeout()),this,SLOT(update()));
    timer.start(30);
}

MainWindow::~MainWindow()
{
}

void MainWindow::gameOver()
{
    gameStatus=GAME_PAUSED;
    lcdScore->setVisible(false);
    for (QGameElement* e:Elementlist)
        e->setEnabled(false);
    gameover->setRect(64.0,-50.0,256.0,57.0);
    gameover->setEnabled(true);
    Elementlist.append(gameover);
}

void MainWindow::gameScoreAdd()
{
    score++;
    lcdScore->display(QString("%1").arg(score,3,10,QChar('0')));
}

void MainWindow::board1Stop()
{
    scoreBoard->clearAllFrame();
    QPixmap pixmap(":/Images/scoreboard.png");
    QPixmap pixmap2;
    QPainter painter(&pixmap);
    if (score<10)
        pixmap2.load(":/Images/medal2.png");
    else if (score<30)
        pixmap2.load(":/Images/medal1.png");
    else
        pixmap2.load(":/Images/medal0.png");
    painter.drawPixmap(30,50,pixmap2);
    painter.setFont(QFont("Arial",15,20));
    painter.setPen(Qt::white);
    painter.drawText(231,65,QString("%1").arg(score,3,10,QChar('0')));
    painter.drawText(238,122,"INF");
    scoreBoard->addFrame(pixmap);
    scoreBoard->setRect(42.0,500.0,300.0,153.0);
    scoreBoard->setEnabled(true);
    Elementlist.append(scoreBoard);
}

void MainWindow::board2Stop()
{
    btnReplay->setVisible(true);
    btnRank->setVisible(true);
}

void MainWindow::replay()
{
    btnReplay->setVisible(false);
    btnRank->setVisible(false);
    gameStatus=GAME_OVER;
    Elementlist.clear();
    Elementlist.append(guide);
    Elementlist.append(ground);
    ground->setEnabled(true);
}

void MainWindow::rank()
{
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button()==Qt::LeftButton)
    {
        if (gameStatus&GAME_OVER)
        {
            lcdScore->setVisible(true);
            lcdScore->display("000");
            bird->setRect(100.0,60.0,30.0,30.0);
            bird->setVy(2.0);
            pipe[0]->setRect(350.0,-160,70.0,250.0);
            pipe[1]->setRect(350.0,198.0,70.0,250.0);
            pipe[2]->setRect(600.0,-160,70.0,250.0);
            pipe[3]->setRect(600.0,198.0,70.0,250.0);
            Elementlist.clear();
            Elementlist.append(bird);
            Elementlist.append(pipe[0]);
            Elementlist.append(pipe[1]);
            Elementlist.append(pipe[2]);
            Elementlist.append(pipe[3]);
            Elementlist.append(ground);
            for (QGameElement *e:Elementlist)
                e->setEnabled(true);
            score=0;
            gameStatus=GAME_PLAYING;
        }
        else if (gameStatus&GAME_PLAYING)
            bird->birdUp();
    }
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if (!(gameStatus&GAME_OVER))
        painter.drawPixmap(0,0,pixmapBkgnd);
    for (QGameElement* e:Elementlist)
    {
        e->logic();
        e->draw(&painter);
    }
}
