/************************************************************
**
** @Name: “关于”对话框
** @Description: 显示操作说明
** @Date: 2013/04/19 19:27
**
*************************************************************/

#ifndef QABOUTDLG_H
#define QABOUTDLG_H

#include <QDialog>
#include "ui_qaboutdlg.h"

class QAboutDlg : public QDialog
{
	Q_OBJECT

public:
	QAboutDlg(QWidget *parent = 0);
	~QAboutDlg();

private:
	Ui::QAboutDlg ui;
};

#endif // QABOUTDLG_H
