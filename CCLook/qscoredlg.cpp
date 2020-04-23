#include "qscoredlg.h"

QScoreDlg::QScoreDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowCloseButtonHint);
}

QScoreDlg::~QScoreDlg()
{

}
