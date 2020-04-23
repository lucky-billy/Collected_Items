#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStack>
const int INIT = 0;
const int DIGIT = 1;
const int OPERA = 2;
const int LEFT = 3;
const int RIGHT = 4;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void init();
    int getLevel(const QChar& oper);//得到运算符等级
    void toPostfix();//转换成后缀表达式
    void evaluation();//求值
    void digitBtn(char ch);
signals:
    void whichBtn(int type);
private slots:
    void enableDigit(int type);
    void enableOp(int type);
    void enableLeft(int type);
    void enableRight(int type);
    void enableEqual(int type);

    void on_btn0_clicked();
    void on_btn1_clicked();
    void on_btn2_clicked();
    void on_btn3_clicked();
    void on_btn4_clicked();
    void on_btn5_clicked();
    void on_btn6_clicked();
    void on_btn7_clicked();
    void on_btn8_clicked();
    void on_btn9_clicked();

    void on_btnDot_clicked();
    void on_btnAdd_clicked();
    void on_btnSub_clicked();
    void on_btnMuti_clicked();
    void on_btnDivi_clicked();
    void on_btnEqual_clicked();
    void on_btnLeft_clicked();
    void on_btnRight_clicked();
    void on_btnClear_clicked();

    void on_btnBack_clicked();

private:
    Ui::MainWindow *ui;
    QStack<QChar> expStack;//后缀表达式栈
    QStack<QChar> opStack;//运算符栈
    QString postfix;//存储后缀表达式
    bool complete;//表达式是否计算完成
    int lefts;
};

#endif // MAINWINDOW_H
