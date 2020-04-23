#ifndef QSCOREDLG_H
#define QSCOREDLG_H

#include <QtWidgets>
#include "ui_qscoredlg.h"

class QScoreDlg : public QDialog
{
	Q_OBJECT

public:
	QScoreDlg(QWidget *parent = 0);
	~QScoreDlg();

/// ½Ó¿Ú
public:
	void clearTable()
	{
		ui.tableWidget->clear();
	}
	void addItem(QString name, int score)
	{
		QTableWidgetItem* firstItem = new QTableWidgetItem(name), *secondItem = new QTableWidgetItem(tr("%1").arg(score));
		firstItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		secondItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		firstItem->setTextAlignment(Qt::AlignCenter);
		secondItem->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget->insertRow(ui.tableWidget->rowCount());
		ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1, 0, firstItem);
		ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1, 1, secondItem);
	}

private:
	Ui::QScoreDlg ui;
};

#endif // QSCOREDLG_H
