#include "dialogclient.h"
#include "ui_dialogclient.h"
#include <QIntValidator>
#include <QHostAddress>
#include <QMessageBox>
#include <QDebug>

DialogClient::DialogClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogClient)
{
    ui->setupUi(this);

    //端口验证
    QIntValidator *vali = new QIntValidator();
    vali->setRange(0, 65535);
    ui->lineEditPort->setValidator(vali);
    //
    ui->lineEditPort->setText("2000");
    ui->lineEditIP->setText("127.0.0.1");

    this->setLayout(ui->mainLayout);

    //创建客户端
    m_pTCPClient = new QTcpSocket();
    //关联信号
    connect(m_pTCPClient, SIGNAL(connected()), this, SLOT(OnSuccConnect()));
    connect(m_pTCPClient, SIGNAL(readyRead()), this, SLOT(ReadStructDataAndShow()));
}

DialogClient::~DialogClient()
{
    delete ui;
}

void DialogClient::on_pushButtonConnect_clicked()
{
    int nPort = ui->lineEditPort->text().toInt();
    QHostAddress address;
    bool bres = address.setAddress(ui->lineEditIP->text());
    //如果IP出错
    if( ! bres )
    {
        QMessageBox::warning(this, tr("IP address error"), tr("IP address is not correct!"));
        return;
    }
    //开始连接
    m_pTCPClient->connectToHost(address, nPort);

}

void DialogClient::OnSuccConnect()
{
    QMessageBox::information(this, tr("Connected"), tr("Connect successed."));
}

void DialogClient::ReadStructDataAndShow()
{
    QString strMsg;
    QByteArray dataArray = m_pTCPClient->readAll();
    if(dataArray.length() != sizeof(NetData))
    {
        //长度不对
        strMsg = tr("Receive error, struct length is %1 B, recv length is %2 B")
                .arg(sizeof(NetData)).arg(dataArray.length());

        qDebug()<<strMsg;
        //
        return;
    }
    //接收长度正确
    const NetData* pData = (NetData*)(dataArray.data());
    //显示
    QString strShow = tr("NetData: \r\n");
    strShow += tr("Length = %1\r\n").arg(pData->length);
    strShow += tr("n1 = %1\r\n").arg(pData->n1);
    strShow += tr("d1 = %1\r\n").arg(pData->d1);
    strShow += tr("d2 = %1\r\n").arg(pData->d2);
    strShow += tr("name = %1\r\n").arg(pData->name);
    //显示
    ui->labelShow->setText(strShow);

}
