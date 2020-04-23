#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPalette>
#include <QButtonGroup>
#include <QByteArray>
#include <string.h>
#include <QStack>

class calculator : public QWidget
{
    Q_OBJECT

public:
    calculator(QWidget *parent = 0);
    ~calculator();

    // 把中缀表达式转换成后缀表达式
    void mid2end(const char* mid, char* end);

    // 计算后缀表达式的值并返回
    int calculate(const char* end);

    // 设置操作键盘
    void operatEnable(bool);

    // 设置数字键盘
    void numberEnable(bool);

public slots:
    void euqalBtn();

    void backspcBtn();

    void whichBtnClicked(int);

signals:
    void buttonReleased(QString);

private:
    QLineEdit       *inputline;
    QLabel          *lableresult;
    QPushButton     *buttons[24];
    QButtonGroup    *buttongroup;

    //声明两个字符数组用来保存中缀表达式和后缀表达式
    char mid[50], end[50];

    //保存结果
    int result;
};

#endif // CALCULATOR_H
