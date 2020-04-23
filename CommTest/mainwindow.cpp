#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QtCore>
#include <QtGui>
#include <QDebug>

// The explaination is in main.cpp
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    ui->plainTextEdit_Defalut_Setting->setEnabled(false);

    CurrentSerialPort = new QSerialPort(this);

    // (1)Set Up the Combobox_SerialPort
    foreach(const QSerialPortInfo &SerialPortInfo,QSerialPortInfo ::availablePorts())
    {
        ui->comboBox_SerialPort->addItem(SerialPortInfo.portName());
    }
    // (1)Set Up the Combobox_SerialPort

    connect(ui->pushButton_OpenCom,SIGNAL(clicked()),this,SLOT(openSerialPort()));
    connect(ui->pushButton_CloseCom,SIGNAL(clicked()),this,SLOT(closeSerialPort()));
    connect(CurrentSerialPort,SIGNAL(readyRead()),this,SLOT(ReadData()));
    connect(ui->pushButton_Clear_Data,SIGNAL(clicked()),this,SLOT(ClearData()));
    connect(this,SIGNAL(ReceiveBufferReady()),this,SLOT(OutPutData()));
    // Initialize the parameters
    ReceiveNumber = 0 ;
    RightNumber = 0 ;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openSerialPort()
{
    // (2) Set the priate object CurrentSerialPort as the ComboBox Choose One
    CurrentSerialPort->setPortName(ui->comboBox_SerialPort->currentText());

    if(CurrentSerialPort->isOpen())
    {
        qDebug () <<CurrentSerialPort->portName() << " is Already Open";
    }

    // Set  parameters of CurrentCom ,and Open it.
    if(CurrentSerialPort->open(QIODevice ::ReadWrite))
    {
        if(CurrentSerialPort->setBaudRate(QSerialPort ::Baud57600)
                &&CurrentSerialPort->setDataBits(QSerialPort ::Data8)
                &&CurrentSerialPort->setDataErrorPolicy(QSerialPort ::IgnorePolicy)
                &&CurrentSerialPort->setFlowControl(QSerialPort ::HardwareControl)
                &&CurrentSerialPort->setParity(QSerialPort ::NoParity)
                &&CurrentSerialPort->setStopBits(QSerialPort ::UnknownStopBits))
        {
            ui->statusBar->showMessage("Successful open "+CurrentSerialPort->portName());
            qDebug() << "Successful to open " << CurrentSerialPort->portName();
        }
        else
        {
            qDebug() << "Error to set the status of " << CurrentSerialPort->portName() ;
            ui->statusBar->showMessage("Successful open "+CurrentSerialPort->portName());
        }
    }
    else
    {
        qDebug() << "Error to open the " << CurrentSerialPort->portName();
    }
}


void MainWindow::closeSerialPort()
{
    CurrentSerialPort->setPortName(ui->comboBox_SerialPort->currentText());
    CurrentSerialPort->close();
    if(!CurrentSerialPort->isOpen())
    {
        qDebug() << "Successful to Close " << CurrentSerialPort->portName();
    }
    else
    {
        qDebug() << "Error to Close the " << CurrentSerialPort->portName();
    }
}

void MainWindow::test()
{
    qDebug() << "This is a test";
}


void MainWindow::ReadData()
{
    qDebug() << "Read Data Now";
    QByteArray data = CurrentSerialPort->readLine();

    foreach(int number ,data )
    {
        ReceiveBuffer.enqueue(number);
    }

    if(ReceiveBuffer.size()>15)
    {
        emit ReceiveBufferReady();
    }

}

void MainWindow::ClearData()
{
    qDebug() << "Clear Data";
    ui->plainTextEdit_Result->clear();
    RightNumber = 0;
    ReceiveNumber = 0;
    receitem = ui->listWidget->item(0);
    rightitem = ui->listWidget->item(1);
    receitem->setText("ReceiveNumber: " + QString ::number(ReceiveNumber));
    rightitem->setText("Right Receive: " + QString ::number(RightNumber));
    ReceiveBuffer.clear();
}

// Fix the synchronization Problem
void MainWindow::OutPutData()
{
    ReceiveNumber ++;
    QByteArray OutPutArray;
    while(ReceiveBuffer[0]!=126)
    {
        ReceiveBuffer.dequeue();
    }

    OutPutArray.append(ReceiveBuffer.dequeue());
    while(ReceiveBuffer[0]!=126)
    {
        if(ReceiveBuffer[0] == 125)
        {
            qDebug() << ReceiveBuffer.dequeue();
            if(ReceiveBuffer[0]==93)
            {
                OutPutArray.append(125);
                qDebug() <<  ReceiveBuffer.dequeue();
            }
            if (ReceiveBuffer[0] == 94)
            {
                OutPutArray.append(126);
                qDebug() <<  ReceiveBuffer.dequeue();
            }
        }
        else
        {
            OutPutArray.append(ReceiveBuffer.dequeue());
        }
    }
    OutPutArray.append(ReceiveBuffer.dequeue());

    // ui->plainTextEdit_Result->insertPlainText("\n");
    if(OutPutArray.size() >= 15)
    {
        ui->plainTextEdit_Result->insertPlainText("\n");
        foreach(quint8 number,OutPutArray)
        {
            ui->plainTextEdit_Result->insertPlainText(" "+QString ::number(number));
        }
    }
    // simple use the length to confirm the receive packet,
    // should plus CRC later

    CRC_result_uint16 = crc_packet(OutPutArray,15);
    CRC_result_uint8[0] = GetHighByte(CRC_result_uint16);
    CRC_result_uint8[1] = GetLowByte(CRC_result_uint16);

    if(OutPutArray.size() >= 15)
    {
        RightNumber++ ;
    }

    // Showing the Statistic Result
    receitem = ui->listWidget->item(0);
    rightitem = ui->listWidget->item(1);
    receitem->setText("ReceiveNumber: " + QString ::number(ReceiveNumber));
    rightitem->setText("Right Receive: " + QString ::number(RightNumber));
}

quint16 MainWindow::crc_byte(quint16 crc, quint8 b)
{
    quint8 i;
    crc = crc ^ b << 8;
    i = 8;
    do
        if (crc & 0x8000)
            crc = crc << 1 ^ 0x1021;
        else
            crc = crc << 1;
    while (--i);

    return crc;
}

quint16 MainWindow::crc_packet(QByteArray data, int len)
{
    int i = 0;
    quint16 crc = 0;

    while (len-- > 0)
        crc = crc_byte(crc, data[i++]);

    return crc;
}

quint8 MainWindow::GetHighByte(quint16 crc)
{
    return quint8(crc >> 8);
}

quint8 MainWindow::GetLowByte(quint16 crc)
{
    return quint8(crc&0x00FF);
}
