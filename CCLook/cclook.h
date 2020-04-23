#ifndef CCLOOK_H
#define CCLOOK_H

#include <QMainWindow>
#include <QtWidgets>
#include <QtOpenGL>
#include "ui_cclook.h"
#include "qcclscene.h"
#include "qaboutdlg.h"
#include "qscoredlg.h"
#include "qcclstat.h"

class CCLook : public QMainWindow
{
	Q_OBJECT

public:
    CCLook(QWidget *parent = 0,  Qt::WindowFlags flags = 0);
	~CCLook();

/// 槽
public slots:
	// 显示关于对话框
	void showAbout();
	// 显示排行榜
	void showScore();
	// 提交分数
	void commitScore(int score);

/// 私有函数
private:
	// 初始化scene & view
	void initScene();
	// 初始化菜单，工具栏
	void initAction();

private:
	Ui::CCLookClass ui;
	QCCLScene* m_scene;
	QGraphicsView* m_view;
	QCCLStat m_stat;
	QList<QScoreStruct> m_scoreList;
};

#endif // CCLOOK_H
