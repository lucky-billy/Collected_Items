#include "calculator.h"
#include <QDebug>

QStack <char> mystack;

calculator::calculator(QWidget *parent) : QWidget(parent)
{
    this->result = 0;

    setFixedSize(300,270);
    setWindowTitle("Calculator");

    inputline = new QLineEdit(this);
    inputline->setText("0");
    inputline->setReadOnly(true);
    inputline->setFixedHeight(40);
    inputline->setFont(QFont("Monospace",14,QFont::Normal,false));
    inputline->setAlignment(Qt::AlignRight);

    lableresult = new QLabel(this);
    lableresult->setFixedHeight(18);
    lableresult->setFont(QFont("Monospace",10,QFont::Light,false));
    lableresult->setAlignment(Qt::AlignRight);
    lableresult->setMargin(0);

    QHBoxLayout *hBox = new QHBoxLayout;
    hBox->addWidget(inputline);

    QGridLayout *grid = new QGridLayout;

    // virtual class buttons container BUTTERGRIUP
    // 这样就可以用按键的id 来判断是那一个按键按下了 ！@important这里没有 this
    buttongroup = new QButtonGroup; // no (this)

    for(int i = 0;i<23;i++){
        buttons[i] = new QPushButton(this);
        buttons[i]->setFont(QFont("Monospace", 14, QFont::Normal,false));
        buttons[i]->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        buttons[i]->setCursor(Qt::PointingHandCursor);
        // 把buttons[i]添加到按钮组
        buttongroup->addButton(buttons[i],i);
        // button released 触发whichBtnClicked 该函数-->用Emit 给button 发信号
        // connect(this,SIGNAL(buttonReleased(QString)),inputline,SLOT(setText(QString)));
    }


    grid->addWidget(buttons[0],0,0);
    buttons[0]->setIcon(QIcon(":/icon/icon/xx.png"));   // x^2
    buttons[0]->setIconSize(QSize(20,20));
    buttons[0]->setCursor(Qt::PointingHandCursor);

    buttons[1]->setIcon(QIcon(":/icon/icon/genhao.png"));   // radical sign
    grid->addWidget(buttons[1],0,1);
    buttons[2]->setText(QString("("));
    buttons[2]->setShortcut(QKeySequence(Qt::SHIFT+Qt::Key_9));
    grid->addWidget(buttons[2],0,2);
    buttons[3]->setText(QString(")"));
    buttons[3]->setShortcut(QKeySequence(Qt::SHIFT+Qt::Key_0));
    grid->addWidget(buttons[3],0,3);

    buttons[4]->setIcon(QIcon(":/icon/icon/cc.png"));   // clean
    buttons[4]->setShortcut(QKeySequence(Qt::Key_C));
    grid->addWidget(buttons[4],0,4);

    buttons[5]->setText(QString("7"));
    buttons[5]->setShortcut(QKeySequence(Qt::Key_7));
    grid->addWidget(buttons[5],1,0);
    buttons[6]->setText(QString("8"));
    buttons[6]->setShortcut(QKeySequence(Qt::Key_8));
    grid->addWidget(buttons[6],1,1);
    buttons[7]->setText(QString("9"));
    buttons[7]->setShortcut(QKeySequence(Qt::Key_9));
    grid->addWidget(buttons[7],1,2);
    buttons[8]->setText(QString("+"));
    buttons[8]->setShortcut(QKeySequence(Qt::Key_Plus));
    buttons[8]->setFont(QFont("Monospace",14,QFont::Bold,false));
    grid->addWidget(buttons[8],1,3);
    buttons[9]->setText(QString(""));
    buttons[9]->setIcon(QIcon(":/icon/icon/arrow.png"));    // backspace
    buttons[9]->setShortcut(QKeySequence(Qt::Key_Backspace));
    grid->addWidget(buttons[9],1,4);

    buttons[10]->setText(QString("4"));
    buttons[10]->setShortcut(QKeySequence(Qt::Key_4));
    grid->addWidget(buttons[10],2,0);
    buttons[11]->setText(QString("5"));
    buttons[11]->setShortcut(QKeySequence(Qt::Key_5));
    grid->addWidget(buttons[11],2,1);
    buttons[12]->setText(QString("6"));
    buttons[12]->setShortcut(QKeySequence(Qt::Key_6));
    grid->addWidget(buttons[12],2,2);
    buttons[13]->setText(QString("-"));
    buttons[13]->setShortcut(QKeySequence(Qt::Key_Minus));
    buttons[13]->setFont(QFont("Monospace",14,QFont::Bold,false));
    grid->addWidget(buttons[13],2,3);
    buttons[14]->setText(QString("%"));
    buttons[14]->setShortcut(QKeySequence(Qt::Key_Percent));
    grid->addWidget(buttons[14],2,4);

    buttons[15]->setText(QString("1"));
    buttons[15]->setShortcut(QKeySequence(Qt::Key_1));
    grid->addWidget(buttons[15],3,0);
    buttons[16]->setText(QString("2"));
    buttons[16]->setShortcut(QKeySequence(Qt::Key_2));
    grid->addWidget(buttons[16],3,1);
    buttons[17]->setText(QString("3"));
    buttons[17]->setShortcut(QKeySequence(Qt::Key_3));
    grid->addWidget(buttons[17],3,2);
    buttons[18]->setText(QString("*"));
    buttons[18]->setShortcut(QKeySequence(Qt::Key_Asterisk));
//    buttons[18]->setIcon(QIcon(":/icon/icon/sub.png"));
    buttons[18]->setFont(QFont("Monospace",14,QFont::Black,false));
    grid->addWidget(buttons[18],3,3);

    buttons[19]->setIcon(QIcon(":/icon/icon/equl.png"));    // show result
    buttons[19]->setIconSize(QSize(20,20));
    buttons[19]->setStyleSheet("background-color:#ffa500;border-radius: 6px;");
    buttons[19]->setShortcut(QKeySequence(Qt::Key_Return));

    grid->addWidget(buttons[19],3,4,2,1);

    buttons[20]->setText(QString("0"));
    buttons[20]->setShortcut(QKeySequence(Qt::Key_0));
    grid->addWidget(buttons[20],4,0,1,2);
    buttons[21]->setText(QString("."));
    buttons[21]->setShortcut(QKeySequence("."));
    grid->addWidget(buttons[21],4,2);
    buttons[22]->setIcon(QIcon(":/icon/icon/div.png")); // div
    buttons[22]->setShortcut(QKeySequence(Qt::Key_Slash));
    grid->addWidget(buttons[22],4,3);

    QVBoxLayout *vBox = new QVBoxLayout;

    vBox->addLayout(hBox);
    vBox->addWidget(lableresult);
    vBox->addLayout(grid);

    setLayout(vBox);
    // 判断按键，输入文本
    connect(buttongroup,SIGNAL(buttonClicked(int)),this,SLOT(whichBtnClicked(int)));
    // result
    connect(buttons[19],SIGNAL(clicked()),this,SLOT(euqalBtn()));
    // backspace
    connect(buttons[9],SIGNAL(clicked()),this,SLOT(backspcBtn()));
}

calculator::~calculator()
{

}

// 中缀表达式转为后缀表达式的实现
void calculator::mid2end(const char* mid,char* end)
{
    int i=0;
    int j=0;
    while(mid[i]){
        if(mid[i] == '.'||mid[i] == '%')continue;
        // mystack.push('#');
        // 遇到数字直接输出，
        if(mid[i]>='0'&&mid[i]<='9'){
            end[j++] = mid[i];

        }else if(mid[i]=='-'||mid[i]=='+'){
            // 加号和减号的操作
            if(!mystack.isEmpty()){
                while((!mystack.isEmpty()) && (mystack.top()=='+'||mystack.top()=='-'||mystack.top()=='*'||mystack.top()=='/')){
                    // 只要是加号或者减号 栈中的无论是什么，出干净
                    end[j++]=mystack.pop();
                }
            }
            mystack.push(mid[i]);

        }else if(mid[i]=='*'||mid[i]=='/'){
            if(!mystack.isEmpty()){
                while((!mystack.isEmpty()) && (mystack.top()=='*'||mystack.top()=='/')){
                    end[j++]=mystack.pop();
                }
            }
            mystack.push(mid[i]);

        }else if('('==mid[i]){
            mystack.push(mid[i]);
        }else if(')'==mid[i]){
            // 遇到）将（之前的所有数据输出之后将（出栈
            if(!mystack.isEmpty()){
                while((!mystack.isEmpty()) && mystack.top()!='('){
                    end[j++]=mystack.pop();
                }
            }// 当前的）直接输出
            mystack.pop();
        }
        i++;
    }
    // 扫描结束将所有的操作符出栈
    while(!mystack.isEmpty())end[j++]=mystack.pop();
}

// 计算表达式的和
int calculator::calculate(const char* end){
    int i=0;
    int a;
    int b;
    while(end[i])
    {
        switch(end[i]){
        case '+':
            b = mystack.pop();
            a = mystack.pop();
            mystack.push(a+b);
            qDebug()<<a<<"+"<<b<<"="<<mystack.top()<<endl;
            break;
        case '-':
            b = mystack.pop();
            a = mystack.pop();
            mystack.push(a-b);
            qDebug()<<a<<"-"<<b<<"="<<mystack.top()<<endl;
            break;
        case '*':
            b = mystack.pop();
            a = mystack.pop();
            mystack.push(a*b);
            qDebug()<<a<<"*"<<b<<"="<<mystack.top()<<endl;
            break;
        case '/':
            b = mystack.pop();
            a = mystack.pop();
            mystack.push(a/b);
            qDebug()<<a<<"/"<<b<<"="<<mystack.top()<<endl;
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            mystack.push(end[i]-'0');
        }
        i++;
    }
    return mystack.top();
}

void calculator::euqalBtn(){
    // 获取字符串QString 转换成 char*
    QByteArray ba = inputline->text().toLatin1();
    strcpy_s(mid, ba.data());

    // end 清空
    memset(end, 0, sizeof(end));

    // 调用中缀表达式转为后缀表达式的计算
    mid2end(mid, end);

    qDebug() << "the end: " << end;

    // 调用计算函数
    result = calculate(end);
    qDebug() << "result: " << result;
    lableresult->setNum(result);
}

void calculator::backspcBtn(){
    QString text = inputline->text();
    text.chop(1);
    if (text.isEmpty())
        text = '0';
    inputline->setText(text);
}

// 输入操作
void calculator::whichBtnClicked(int buttonID){
    // 设置flag来确定是不是
    qDebug()<<"a button clicked";
    if(inputline->text().size() == 1&& inputline->text().toInt() == 0){
        if(( inputline->text()== "."||inputline->text()=="+" \
             ||inputline->text()=="-"||inputline->text()=="*"||\
             inputline->text()=="/")){
            ;
        }else{
            inputline->setText("");
        }
    }
    switch (buttonID)
    {
    case 2:
        inputline->setText(inputline->text()+QString("("));
        break;
    case 3:
        inputline->setText(inputline->text()+QString(")"));
        break;
    case 4:
        inputline->setText("0");
        lableresult->setText("");
        buttons[19]->setEnabled(false);
        numberEnable(true);
        break;
    case 5:
        inputline->setText(inputline->text()+QString("7"));
        numberEnable(false);
        operatEnable(true);
        break;
    case 6:
        inputline->setText(inputline->text()+QString("8"));
        numberEnable(false);
        operatEnable(true);
        break;
    case 7:
        inputline->setText(inputline->text()+QString("9"));
        numberEnable(false);
        operatEnable(true);
        break;
    case 8:
        inputline->setText(inputline->text()+QString("+"));
        numberEnable(true);
        operatEnable(false);
        break;
    case 10:
        inputline->setText(inputline->text()+QString("4"));
        numberEnable(false);
        operatEnable(true);
        break;
    case 11:
        inputline->setText(inputline->text()+QString("5"));
        numberEnable(false);
        operatEnable(true);
        break;
    case 12:
        inputline->setText(inputline->text()+QString("6"));
        numberEnable(false);
        operatEnable(true);
        break;
    case 13:
        inputline->setText(inputline->text()+QString("-"));
        operatEnable(false);
        numberEnable(true);
        break;
    case 14:
        inputline->setText(inputline->text()+QString("%"));
        break;
    case 15:
        inputline->setText(inputline->text()+QString("1"));
        numberEnable(false);
        operatEnable(true);
        break;
    case 16:
        inputline->setText(inputline->text()+QString("2"));
        numberEnable(false);
        operatEnable(true);
        break;
    case 17:
        inputline->setText(inputline->text()+QString("3"));
        numberEnable(false);
        operatEnable(true);
        break;
    case 18:
        inputline->setText(inputline->text()+QString("*"));
        operatEnable(false);
        numberEnable(true);
        break;
    case 20:
        inputline->setText(inputline->text()+QString("0"));
        numberEnable(false);
        operatEnable(true);
        break;
    case 21:
        inputline->setText(inputline->text()+QString("."));
        break;
    case 22:
        inputline->setText(inputline->text()+QString("/"));
        operatEnable(false);
        numberEnable(true);
        break;

    default: break;
    }
}

// 设置操作键盘
void  calculator::operatEnable(bool status){
    //8 13 18 22
    buttons[8]->setEnabled(status);
    buttons[18]->setEnabled(status);
    buttons[13]->setEnabled(status);
    buttons[22]->setEnabled(status);
    buttons[19]->setEnabled(true);
}

// 设置数字键盘
void calculator::numberEnable(bool status){
    //5 6 7 10 11 12 15 16 17
    buttons[5]->setEnabled(status);
    buttons[6]->setEnabled(status);
    buttons[7]->setEnabled(status);
    buttons[10]->setEnabled(status);
    buttons[11]->setEnabled(status);
    buttons[12]->setEnabled(status);
    buttons[15]->setEnabled(status);
    buttons[16]->setEnabled(status);
    buttons[17]->setEnabled(status);
    buttons[20]->setEnabled(status);

}
