#include "qaboutdlg.h"

QAboutDlg::QAboutDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowCloseButtonHint);
}

QAboutDlg::~QAboutDlg()
{

}
