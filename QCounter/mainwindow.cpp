#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    QDesktopWidget * desktop = QApplication::desktop();
    move((desktop->width() - this->width())/2,(desktop->height() - this->height())/2);
    ui->setupUi(this);
    ui->lineEdit->setEnabled(false);
    ui->lineEdit->setAlignment(Qt::AlignRight);//设置显示居右
    ui->lineEdit->setStyleSheet("font-size:18px");//设置字体大小为18px
    ui->lineEdit->setText("0");//设置初试文本为0

    lefts=0;
    //fist connect,next init
    connect(this,&MainWindow::whichBtn,&MainWindow::enableOp);
    connect(this,&MainWindow::whichBtn,&MainWindow::enableLeft);
    connect(this,&MainWindow::whichBtn,&MainWindow::enableEqual);
    connect(this,&MainWindow::whichBtn,&MainWindow::enableRight);
    connect(this,&MainWindow::whichBtn,&MainWindow::enableDigit);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}
/*
 *public function
 */
//初始化操作.不仅开头调用，每次计算完结果都会调用。
void MainWindow::init()
{
    if(!expStack.isEmpty())
        expStack.clear();
    if(!opStack.isEmpty())
        opStack.clear();
    opStack.push('#');
    complete=true;
    emit whichBtn(INIT);
}
void MainWindow::digitBtn(char ch)
{
    QString s = ui->lineEdit->text();
    if(!complete)
        ui->lineEdit->setText(s+ch);
    else
    {
        ui->lineEdit->setText(QString(ch));
        complete = false;
    }
    emit whichBtn(DIGIT);
}
/*
 *SLOT function
 */
void MainWindow::enableDigit(int type)
{
    bool enable=true;
    if(RIGHT==type)
        enable=false;
    ui->btn0->setEnabled(enable);
    ui->btn1->setEnabled(enable);
    ui->btn2->setEnabled(enable);
    ui->btn3->setEnabled(enable);
    ui->btn4->setEnabled(enable);
    ui->btn5->setEnabled(enable);
    ui->btn6->setEnabled(enable);
    ui->btn7->setEnabled(enable);
    ui->btn8->setEnabled(enable);
    ui->btn9->setEnabled(enable);
    ui->btnDot->setEnabled(enable);
}
void MainWindow::enableOp(int type)
{
    bool enable = false;
    switch(type)
    {
    case RIGHT:
    case DIGIT:enable = true;
        break;
    case INIT:
    case OPERA:
    case LEFT:enable = false;
        break;
    default:return;
    }
    ui->btnMuti->setEnabled(enable);
    ui->btnDivi->setEnabled(enable);
    ui->btnAdd->setEnabled(enable);
    ui->btnSub->setEnabled(enable);
}
void MainWindow::enableLeft(int type)
{
    bool enable = false;
    switch(type)
    {
    case RIGHT:
    case DIGIT:
        break;
    case INIT:
    case LEFT:
    case OPERA:enable = true;
        break;
    default:return;
    }
    ui->btnLeft->setEnabled(enable);
}
void MainWindow::enableRight(int type)
{
    bool enable = false;
    switch(type)
    {
    case INIT:
    case OPERA:
        break;
    case LEFT:enable = true;
    case RIGHT:
    case DIGIT:
        if(lefts)
            enable = true;
        break;
    default:return;
    }
    ui->btnRight->setEnabled(enable);
}
void MainWindow::enableEqual(int type)
{
    bool enable = false;
    switch(type)
    {
    case OPERA:
    case LEFT:
        break;
    case RIGHT:
    case DIGIT:
        enable = true;
        break;
    default:return;
    }
    ui->btnEqual->setEnabled(enable);
}
////////////////////////////////////
void MainWindow::on_btn0_clicked()
{
    digitBtn('0');
}

void MainWindow::on_btn1_clicked()
{
    digitBtn('1');

}

void MainWindow::on_btn2_clicked()
{
    digitBtn('2');
}

void MainWindow::on_btn3_clicked()
{
    digitBtn('3');
}

void MainWindow::on_btn4_clicked()
{
    digitBtn('4');
}

void MainWindow::on_btn5_clicked()
{
    digitBtn('5');
}

void MainWindow::on_btn6_clicked()
{
    digitBtn('6');
}

void MainWindow::on_btn7_clicked()
{
    digitBtn('7');
}

void MainWindow::on_btn8_clicked()
{
    digitBtn('8');
}

void MainWindow::on_btn9_clicked()
{
    digitBtn('9');
}

void MainWindow::on_btnDot_clicked()
{
    //如果是小数点,那么不能以小数点打头
    QString s = ui->lineEdit->text();
    if(!complete)
        ui->lineEdit->setText(s+".");
}
///////////////////////////////////////////////////
//加号和减号应做特殊处理。因为也可以理解为正负号
///////////////////////////////////////////////////
void MainWindow::on_btnAdd_clicked()
{
    QString s = ui->lineEdit->text();
    ui->lineEdit->setText(s+"+");
    emit whichBtn(OPERA);
}

void MainWindow::on_btnSub_clicked()
{
    QString s = ui->lineEdit->text();
    ui->lineEdit->setText(s+"-");
    emit whichBtn(OPERA);
}
//////////////////////////////////////////////////////
void MainWindow::on_btnMuti_clicked()
{
    QString s = ui->lineEdit->text();
    ui->lineEdit->setText(s+"*");
    emit whichBtn(OPERA);
}

void MainWindow::on_btnDivi_clicked()
{
    QString s = ui->lineEdit->text();
    ui->lineEdit->setText(s+"/");
    emit whichBtn(OPERA);
}
////////////////////////////////////////////
void MainWindow::on_btnLeft_clicked()
{
    QString s = ui->lineEdit->text();
    if(!complete)
        ui->lineEdit->setText(s+"(");
    else
    {
        ui->lineEdit->setText("(");
        complete = false;
    }
    emit whichBtn(LEFT);
    lefts++;
}

void MainWindow::on_btnRight_clicked()
{
    QString s = ui->lineEdit->text();
    if(!complete)
        ui->lineEdit->setText(s+")");
    lefts--;
    emit whichBtn(RIGHT);
}
//得到运算符的优先级
int MainWindow::getLevel(const QChar &oper)
{
    switch(oper.cell())
    {
    case '#':
    case '(':return 0;
    case '+':
    case '-':return 1;
    case '*':
    case '/':
    case '%':return 2;
    }
    return 0;
}
//转换为后缀表达式，实现最主要功能
void MainWindow::toPostfix()
{
    QString exp = ui->lineEdit->text();
    //QString exp = "0.3/(5*2+1)",postfix;

    int j=0;
    qDebug()<<j;
    for(int i=0;i<exp.length();i++)
    {qDebug()<<i<<exp[i];
        if(exp[i].isDigit()||exp[i]=='.')
        {
            postfix.push_back(exp[i]);
        }
        else if(exp[i]=='(')
        {
            opStack.push(exp[i]);
        }
        else if(exp[i]==')')
        {
            postfix.push_back(' ');
            while(opStack.top()!='(')
            {
                postfix.push_back(opStack.pop());qDebug()<<postfix;
            }
            opStack.pop();
        }
        else if(getLevel(exp[i])>getLevel(opStack.top()))
        {
            postfix.push_back(' ');qDebug()<<"postfix";
            opStack.push(exp[i]);
        }
        else
        {
            postfix.push_back(' ');qDebug()<<postfix;
            while(getLevel(exp[i])<=getLevel(opStack.top()))
                postfix.push_back(opStack.pop());
            opStack.push(exp[i]);
        }
    }
    while(opStack.top()!='#')
    {
        QChar c = opStack.pop();
        postfix.push_back(' ');
        postfix.push_back(c);
    }
    qDebug()<<postfix;
}
//
void MainWindow::evaluation()
{
    QString tem;
    QStack<double> ans;
    for(int i=0;i<postfix.size();i++)
    {
        qDebug()<<postfix[i]<<i;
        if(postfix[i].isDigit()||postfix[i]=='.')
            tem.push_back(postfix[i]);
        else if(postfix[i]==' ')
        {
            if(!tem.isEmpty())
            {
                ans.push(tem.toDouble());
                tem.clear();
            }
            qDebug()<<ans.top()<<tem.isEmpty();
        }
        else
        {
            double a,b;
            a=ans.pop();qDebug()<<a<<"a";
            b=ans.pop();qDebug()<<b<<"b";
            switch(postfix[i].cell())
            {
            case '+':ans.push(b+a);break;
            case '-':ans.push(b-a);break;
            case '*':ans.push(b*a);break;
            case '/':ans.push(b/a);break;
            case '%':ans.push((int)a%(int)b);break;
            }
            qDebug()<<ans.top()<<"top";
        }

    }
    //qDebug()<<ans.top();
    ui->lineEdit->setText(QString::number(ans.top()));
}
//等于号的槽函数。
void MainWindow::on_btnEqual_clicked()
{
    //如果还没输入，那么等于号不起作用
    if(ui->lineEdit->text()=="0")
        return;

    toPostfix();
    evaluation();
    init();
}


void MainWindow::on_btnClear_clicked()
{
    init();
    ui->lineEdit->setText("0");
}


void MainWindow::on_btnBack_clicked()
{
    QString s = ui->lineEdit->text();
    s=s.mid(0,s.size()-1);
    ui->lineEdit->setText(s);
}
