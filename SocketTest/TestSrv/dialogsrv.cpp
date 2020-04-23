#include "dialogsrv.h"
#include "ui_dialogsrv.h"
#include <QMessageBox>
#include <QIntValidator>    //验证端口号
#include <QHostAddress>
#include <QByteArray>
#include <QDebug>

DialogSrv::DialogSrv(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSrv)
{
    ui->setupUi(this);

    QIntValidator *vali = new QIntValidator();
    vali->setRange(0, 65535);
    ui->lineEditPort->setValidator(vali);   //限制端口范围
    ui->lineEditPort->setText("2000");  //默认监听端口
    this->setLayout(ui->mainLayout);    //设置界面排布

    //构建TCP服务器
    m_pTCPSrv = new QTcpServer(this);
    //关联成功接收连接的信号
    connect(m_pTCPSrv, SIGNAL(newConnection()), this, SLOT(onNewConnection()));

}

DialogSrv::~DialogSrv()
{
    delete m_pTCPSrv;   m_pTCPSrv = NULL;
    delete ui;
}

void DialogSrv::on_pushButtonListen_clicked()
{
    if( m_pTCPSrv->isListening() )
    {
        //已经在监听，啥都不干
        return;
    }
    //需要开启监听
    //获取端口
    int nPort = ui->lineEditPort->text().toInt();

    bool bres = m_pTCPSrv->listen(QHostAddress::Any, nPort);
    if(bres)
    {
        //成功
        QMessageBox::information(this, tr("Listen"), tr("Listen successed."));
        return;
    }
    else
    {
        //失败
        QMessageBox::warning(this, tr("Listen"), tr("Listen failed."));
        return;
    }
}

void DialogSrv::onNewConnection()
{
    //接收新连接
    QTcpSocket *sockclient = m_pTCPSrv->nextPendingConnection();
    //获取远端IP和端口
    QString strIPAndPort = sockclient->peerAddress().toString() + tr(" __ %1").arg(sockclient->peerPort());
    //添加到列表
    ui->listWidget->addItem(strIPAndPort);
    //构建客户端对象
    ClientJobs *pClientJob = new ClientJobs(this, sockclient, strIPAndPort);
    //IP-port列表
    m_listIPAndPorts.append(strIPAndPort);
    //客户端列表
    m_listClients.append(pClientJob);
    //
    connect(pClientJob, SIGNAL(CallMainWindowDeleteClient(QString)), this, SLOT(DeleteOneClient(QString)));

}


void DialogSrv::on_pushButtonStop_clicked()
{
    if(m_pTCPSrv->isListening())
    {
        m_pTCPSrv->close();
    }
    else
    {
        return;
    }

}


//发送测试数据
void DialogSrv::on_pushButtonSend_clicked()
{
    //判断有无客户端接入
    if( m_listClients.count() < 1 )
    {
        return;
    }
    //判断选择的是哪个客户端
    int nIndex = ui->listWidget->currentRow();
    if(nIndex < 0)
    {
        //没选择客户端发送
        return;
    }
    //使用向nIndex号客户端发包
    //构造结构体
    NetData ssData;
    //填充数据
    ssData.length = sizeof(NetData);
    ssData.n1 = 1;
    ssData.d1 = 2.0;
    ssData.d2 = 4.0;
    char name[32] = "Say Hello.";
    strncpy(ssData.name, name, 32);
    //转为QByteArray发送
    QByteArray dataArray = QByteArray::fromRawData((char*)&ssData, sizeof(NetData));
    m_listClients[nIndex]->SendBytes(dataArray);
    //
    qDebug()<<"Sendto:  "<<m_listIPAndPorts[nIndex]<<" Bytes: "<<sizeof(NetData);

}

void DialogSrv::DeleteOneClient(QString strIPAndPort)
{
    int nIndex = m_listIPAndPorts.indexOf(strIPAndPort);
    if(nIndex < 0)
    {
        //序号出错
        return;
    }
    //删除对应客户端
    ui->listWidget->takeItem(nIndex);   //删除UI显示的那一个IP条目
    m_listIPAndPorts.removeAt(nIndex); //删除IP字符串列表对应条目
    m_listClients.removeAt(nIndex);//删除客户端

}







