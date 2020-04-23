#include "cclook.h"

CCLook::CCLook(QWidget *parent, Qt::WindowFlags flags)
	: QMainWindow(parent, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowCloseButtonHint)
	, m_stat(this)
{
	ui.setupUi(this);

	initScene();

	initAction();

	m_scoreList = m_stat.getUserScore();
	connect(m_scene, SIGNAL(emitScore(int)), this, SLOT(commitScore(int)));

	m_scene->gameStart();
}

CCLook::~CCLook()
{
}

/// 槽
void CCLook::showAbout()
{
	QAboutDlg dlg;
	dlg.exec();
}

void CCLook::showScore()
{
	QScoreDlg dlg;
	for (int i = 0; i != m_scoreList.count(); i++)
	{
		qDebug() << m_scoreList[i].m_userName << m_scoreList[i].m_userScore;
		dlg.addItem(m_scoreList[i].m_userName, m_scoreList[i].m_userScore);
	}
	dlg.exec();
}

void CCLook::commitScore(int score)
{
	QString name = tr("Anonymous");

	QInputDialog dlg(this);
	dlg.setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowCloseButtonHint);
	dlg.setWindowTitle(tr("You Win!"));
	dlg.setLabelText(tr("Your Name:"));
	dlg.setOkButtonText(tr("Ok"));
	dlg.exec();
	if (dlg.textValue() != "")
		name = dlg.textValue();

	QScoreStruct scoreStruct = {name, score};
	for (int i = 0; i != m_scoreList.count(); i++)
	{
		if (m_scoreList[i].m_userScore < scoreStruct.m_userScore)
		{
			m_scoreList.insert(i, scoreStruct);
			m_stat.setUserScore(m_scoreList);
			return;
		}
	}
	m_scoreList.append(scoreStruct);

	m_stat.setUserScore(m_scoreList);
}

/// 私有函数
void CCLook::initScene()
{
	m_scene = new QCCLScene(this);
	m_view = new QGraphicsView(this);
	m_view->setScene(m_scene);
	m_view->setViewport(new QGLWidget(QGLFormat(QGL::DoubleBuffer)));

	QHBoxLayout* layout = new QHBoxLayout;
	layout->setContentsMargins(0, 0, 0, 0);
	ui.centralWidget->setLayout(layout);
	m_view->setMinimumSize(Width + 2 * Margin, Height + 2 * Margin);
	m_view->setMaximumSize(Width + 2 * Margin, Width + 2 * Margin);
	m_view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	layout->addWidget(m_view);

	m_scene->setSceneRect(-Width / 2, -Height / 2, Width, Height);
	QDir qPicDir(":/img");
	QList<QFileInfo> qPicList = qPicDir.entryInfoList();

	QStringList qPicNames;
	for (int i = 0; i != qPicList.count(); i++)
	{
		qPicNames << qPicList[i].absoluteFilePath();
	}

	m_scene->setPicSets(qPicNames);
}

void CCLook::initAction()
{
	// 开始， 提示， 排行榜， 关于
	QAction* startAction = new QAction(QIcon(":/ico/Game.png"), tr("&New Game"), this);
	QAction* tipAction = new QAction(QIcon(":/ico/Tip.png"), tr("&Tip Me"), this);
	QAction* rankAction = new QAction(QIcon(":/ico/Rank.png"), tr("&Rank"), this);
	QAction* aboutAction = new QAction(QIcon(":/ico/About.png"), tr("&About"), this);

	startAction->setShortcut(QKeySequence("Ctrl+N"));
	tipAction->setShortcut(QKeySequence("Ctrl+T"));
	rankAction->setShortcut(QKeySequence("Ctrl+R"));
	aboutAction->setShortcut(QKeySequence("Ctrl+A"));

	connect(startAction, SIGNAL(triggered()), m_scene, SLOT(gameStart()));
	connect(tipAction, SIGNAL(triggered()), m_scene, SLOT(getTip()));
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAbout()));
	connect(rankAction, SIGNAL(triggered()), this, SLOT(showScore()));

	QMenu* gameMenu = new QMenu(tr("Game"), this);
	QMenu* helpMenu = new QMenu(tr("Help"), this);
	gameMenu->addAction(startAction);
	gameMenu->addAction(tipAction);
	helpMenu->addAction(rankAction);
	helpMenu->addAction(aboutAction);

	ui.menuBar->addMenu(gameMenu);
	ui.menuBar->addMenu(helpMenu);

	ui.mainToolBar->addAction(startAction);
	ui.mainToolBar->addAction(tipAction);
	ui.mainToolBar->addAction(rankAction);
	ui.mainToolBar->addAction(aboutAction);
}
