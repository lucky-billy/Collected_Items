#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QListWidget>
#include <QQueue>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QSerialPort *CurrentSerialPort;
    quint64 ReceiveNumber;
    quint64 RightNumber;
    quint8  PackLossRate;
    QListWidgetItem *receitem;
    QListWidgetItem *rightitem;
    QQueue<int> ReceiveBuffer;
    quint16 crc_byte(quint16 crc, quint8 b);
    quint16 crc_packet(QByteArray data, int len);
    quint8  GetHighByte(quint16);
    quint8  GetLowByte(quint16);
    quint8  CRC_result_uint8[2];
    quint16 CRC_result_uint16;

signals:
    void ReceiveBufferReady();

private slots:
    void openSerialPort();
    void closeSerialPort();
    void test();
    void ReadData();
    void ClearData();
    void OutPutData();

};

#endif // MAINWINDOW_H
