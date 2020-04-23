#include "dialogaddressinput.h"
#include "ui_dialogaddressinput.h"

DialogAddressInput::DialogAddressInput(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DialogAddressInput)
{
	ui->setupUi(this);
}

DialogAddressInput::~DialogAddressInput()
{
	delete ui;
}
void DialogAddressInput::SetAddr(QString addr, QString port)
{
	ui->lineEdit_addr->setText(addr);
	ui->lineEdit_port->setText(port);
}

QString DialogAddressInput::addr()
{
	return ui->lineEdit_addr->text();
}

QString DialogAddressInput::port()
{
	return ui->lineEdit_port->text();
}
