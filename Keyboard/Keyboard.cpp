#include "Keyboard.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    this->resize(520,250);
   /* QPalette palette = this->palette();
    palette.setBrush(QPalette::Background,QBrush(Qt::blue));
    this->setPalette(palette);
    //this->setAutoFillbackground(true);*/

    init();
    setName_en();
    setConnect();
    setPlace();
}

Widget::~Widget()
{
    
}

void Widget::init()
{
    m_file = new QFile(":/zk.txt");
    m_in   = new QTextStream(m_file);
    if(!m_file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "can't open this zk.txt!!";
        return;
    }

    m_hanziEdit = new QLineEdit(this);
    m_spellEdit = new QLineEdit(this);

    m_leftBtn = new QToolButton(this);
    m_rightBtn = new QToolButton(this);
    m_ziBtn0 = new QToolButton(this);
    m_ziBtn1 = new QToolButton(this);
    m_ziBtn2 = new QToolButton(this);
    m_ziBtn3 = new QToolButton(this);
    m_ziBtn4 = new QToolButton(this);
    m_ziBtn5 = new QToolButton(this);
    m_ziBtn6 = new QToolButton(this);
    m_ziBtn7 = new QToolButton(this);

    m_QBtn = new QToolButton(this);
    m_WBtn = new QToolButton(this);
    m_EBtn = new QToolButton(this);
    m_RBtn = new QToolButton(this);
    m_TBtn = new QToolButton(this);
    m_YBtn = new QToolButton(this);
    m_UBtn = new QToolButton(this);
    m_IBtn = new QToolButton(this);
    m_OBtn = new QToolButton(this);
    m_PBtn = new QToolButton(this);
    m_ABtn = new QToolButton(this);
    m_SBtn = new QToolButton(this);
    m_DBtn = new QToolButton(this);
    m_FBtn = new QToolButton(this);
    m_GBtn = new QToolButton(this);
    m_HBtn = new QToolButton(this);
    m_JBtn = new QToolButton(this);
    m_KBtn = new QToolButton(this);
    m_LBtn = new QToolButton(this);
    m_ZBtn = new QToolButton(this);
    m_XBtn = new QToolButton(this);
    m_CBtn = new QToolButton(this);
    m_VBtn = new QToolButton(this);
    m_BBtn = new QToolButton(this);
    m_NBtn = new QToolButton(this);
    m_MBtn = new QToolButton(this);

    m_capsBtn = new QToolButton(this);
    m_deleteBtn = new QToolButton(this);
    m_signBtn = new QToolButton(this);
    m_changeBtn = new QToolButton(this);
    m_spaceBtn = new QToolButton(this);
    m_sureBtn = new QToolButton(this);
    m_cancelBtn = new QToolButton(this);

    m_leftBtn ->setText("<");
    m_rightBtn ->setText(">");
    m_capsBtn ->setText("Caps");
    m_deleteBtn ->setText("<--");
    m_signBtn ->setText("CN");
    m_changeBtn ->setText(QStringLiteral("切换"));
    m_spaceBtn ->setText(QStringLiteral("空格"));
    m_sureBtn ->setText(QStringLiteral("确定"));
    m_cancelBtn ->setText(QStringLiteral("取消"));

    m_stateFlag = 0;
    m_capsFlag = true;
    m_storeCount = 0;
    m_selectFlag = 1;

}
void Widget::setName_EN()
{

    m_QBtn ->setText("Q");
    m_WBtn ->setText("W");
    m_EBtn ->setText("E");
    m_RBtn ->setText("R");
    m_TBtn ->setText("T");
    m_YBtn ->setText("Y");
    m_UBtn ->setText("U");
    m_IBtn ->setText("I");
    m_OBtn ->setText("O");
    m_PBtn ->setText("P");
    m_ABtn ->setText("A");
    m_SBtn ->setText("S");
    m_DBtn ->setText("D");
    m_FBtn ->setText("F");
    m_GBtn ->setText("G");
    m_HBtn ->setText("H");
    m_JBtn ->setText("J");
    m_KBtn ->setText("K");
    m_LBtn ->setText("L");
    m_ZBtn ->setText("Z");
    m_XBtn ->setText("X");
    m_CBtn ->setText("C");
    m_VBtn ->setText("V");
    m_BBtn ->setText("B");
    m_NBtn ->setText("N");
    m_MBtn ->setText("M");
}
void Widget::setName_en()
{
    m_QBtn ->setText("q");
    m_WBtn ->setText("w");
    m_EBtn ->setText("e");
    m_RBtn ->setText("r");
    m_TBtn ->setText("t");
    m_YBtn ->setText("y");
    m_UBtn ->setText("u");
    m_IBtn ->setText("i");
    m_OBtn ->setText("o");
    m_PBtn ->setText("p");
    m_ABtn ->setText("a");
    m_SBtn ->setText("s");
    m_DBtn ->setText("d");
    m_FBtn ->setText("f");
    m_GBtn ->setText("g");
    m_HBtn ->setText("h");
    m_JBtn ->setText("j");
    m_KBtn ->setText("k");
    m_LBtn ->setText("l");
    m_ZBtn ->setText("z");
    m_XBtn ->setText("x");
    m_CBtn ->setText("c");
    m_VBtn ->setText("v");
    m_BBtn ->setText("b");
    m_NBtn ->setText("n");
    m_MBtn ->setText("m");
}
void Widget::setName_sn()
{
    m_QBtn ->setText("1");
    m_WBtn ->setText("2");
    m_EBtn ->setText("3");
    m_RBtn ->setText("4");
    m_TBtn ->setText("5");
    m_YBtn ->setText("6");
    m_UBtn ->setText("7");
    m_IBtn ->setText("8");
    m_OBtn ->setText("9");
    m_PBtn ->setText("0");
    m_ABtn ->setText(",");
    m_SBtn ->setText("。");
    m_DBtn ->setText("、");
    m_FBtn ->setText("；");
    m_GBtn ->setText("：");
    m_HBtn ->setText("？");
    m_JBtn ->setText("！");
    m_KBtn ->setText("·");
    m_LBtn ->setText("＂");
    m_ZBtn ->setText("〔");
    m_XBtn ->setText("〕");
    m_CBtn ->setText("●");
    m_VBtn ->setText("※");
    m_BBtn ->setText("＃");
    m_NBtn ->setText("＠");
    m_MBtn ->setText("‰");
}
void Widget::setPlace()
{
    m_hanziEdit ->setGeometry(10,10,110,30);
    m_spellEdit ->setGeometry(120,10,40,30);

    m_leftBtn ->setGeometry(165,10,19,30);
    m_rightBtn ->setGeometry(488,10,19,30);

    m_ziBtn0 ->setGeometry(184,10,38,30);
    m_ziBtn1 ->setGeometry(222,10,38,30);
    m_ziBtn2 ->setGeometry(260,10,38,30);
    m_ziBtn3 ->setGeometry(298,10,38,30);
    m_ziBtn4 ->setGeometry(336,10,38,30);
    m_ziBtn5 ->setGeometry(374,10,38,30);
    m_ziBtn6 ->setGeometry(412,10,38,30);
    m_ziBtn7 ->setGeometry(450,10,38,30);

    m_QBtn ->setGeometry(10,50,40,40);
    m_WBtn ->setGeometry(60,50,40,40);
    m_EBtn ->setGeometry(110,50,40,40);
    m_RBtn ->setGeometry(160,50,40,40);
    m_TBtn ->setGeometry(210,50,40,40);
    m_YBtn ->setGeometry(260,50,40,40);
    m_UBtn ->setGeometry(310,50,40,40);
    m_IBtn ->setGeometry(360,50,40,40);
    m_OBtn ->setGeometry(410,50,40,40);
    m_PBtn ->setGeometry(460,50,40,40);
    m_ABtn ->setGeometry(36,100,40,40);
    m_SBtn ->setGeometry(86,100,40,40);
    m_DBtn ->setGeometry(136,100,40,40);
    m_FBtn ->setGeometry(186,100,40,40);
    m_GBtn ->setGeometry(236,100,40,40);
    m_HBtn ->setGeometry(286,100,40,40);
    m_JBtn ->setGeometry(336,100,40,40);
    m_KBtn ->setGeometry(386,100,40,40);
    m_LBtn ->setGeometry(436,100,40,40);

    m_ZBtn ->setGeometry(77,150,40,40);
    m_XBtn ->setGeometry(127,150,40,40);
    m_CBtn ->setGeometry(177,150,40,40);
    m_VBtn ->setGeometry(227,150,40,40);
    m_BBtn ->setGeometry(277,150,40,40);
    m_NBtn ->setGeometry(327,150,40,40);
    m_MBtn ->setGeometry(377,150,40,40);

    m_capsBtn ->setGeometry(20,150,50,40);
    m_deleteBtn ->setGeometry(427,150,70,40);
    m_signBtn ->setGeometry(15,200,60,40);
    m_changeBtn ->setGeometry(80,200,60,40);
    m_spaceBtn ->setGeometry(150,200,220,40);
    m_sureBtn ->setGeometry(380,200,60,40);
    m_cancelBtn ->setGeometry(445,200,60,40);
}

void Widget::setConnect()
{
    connect(m_leftBtn,SIGNAL(clicked()),this,SLOT(leftFun()));
    connect(m_rightBtn,SIGNAL(clicked()),this,SLOT(rightFun()));

    connect(m_ziBtn0,SIGNAL(clicked()),this,SLOT(ziFun0()));
    connect(m_ziBtn1,SIGNAL(clicked()),this,SLOT(ziFun1()));
    connect(m_ziBtn2,SIGNAL(clicked()),this,SLOT(ziFun2()));
    connect(m_ziBtn3,SIGNAL(clicked()),this,SLOT(ziFun3()));
    connect(m_ziBtn4,SIGNAL(clicked()),this,SLOT(ziFun4()));
    connect(m_ziBtn5,SIGNAL(clicked()),this,SLOT(ziFun5()));
    connect(m_ziBtn6,SIGNAL(clicked()),this,SLOT(ziFun6()));
    connect(m_ziBtn7,SIGNAL(clicked()),this,SLOT(ziFun7()));

    connect(m_QBtn,SIGNAL(clicked()),this,SLOT(qFun()));
    connect(m_WBtn,SIGNAL(clicked()),this,SLOT(wFun()));
    connect(m_EBtn,SIGNAL(clicked()),this,SLOT(eFun()));
    connect(m_RBtn,SIGNAL(clicked()),this,SLOT(rFun()));
    connect(m_TBtn,SIGNAL(clicked()),this,SLOT(tFun()));
    connect(m_YBtn,SIGNAL(clicked()),this,SLOT(yFun()));
    connect(m_UBtn,SIGNAL(clicked()),this,SLOT(uFun()));
    connect(m_IBtn,SIGNAL(clicked()),this,SLOT(iFun()));
    connect(m_OBtn,SIGNAL(clicked()),this,SLOT(oFun()));
    connect(m_PBtn,SIGNAL(clicked()),this,SLOT(pFun()));
    connect(m_ABtn,SIGNAL(clicked()),this,SLOT(aFun()));
    connect(m_SBtn,SIGNAL(clicked()),this,SLOT(sFun()));
    connect(m_DBtn,SIGNAL(clicked()),this,SLOT(dFun()));
    connect(m_FBtn,SIGNAL(clicked()),this,SLOT(fFun()));
    connect(m_GBtn,SIGNAL(clicked()),this,SLOT(gFun()));
    connect(m_HBtn,SIGNAL(clicked()),this,SLOT(hFun()));
    connect(m_JBtn,SIGNAL(clicked()),this,SLOT(jFun()));
    connect(m_KBtn,SIGNAL(clicked()),this,SLOT(kFun()));
    connect(m_LBtn,SIGNAL(clicked()),this,SLOT(lFun()));
    connect(m_ZBtn,SIGNAL(clicked()),this,SLOT(zFun()));
    connect(m_XBtn,SIGNAL(clicked()),this,SLOT(xFun()));
    connect(m_CBtn,SIGNAL(clicked()),this,SLOT(cFun()));
    connect(m_VBtn,SIGNAL(clicked()),this,SLOT(vFun()));
    connect(m_BBtn,SIGNAL(clicked()),this,SLOT(bFun()));
    connect(m_NBtn,SIGNAL(clicked()),this,SLOT(nFun()));
    connect(m_MBtn,SIGNAL(clicked()),this,SLOT(mFun()));

    connect(m_capsBtn,SIGNAL(clicked()),this,SLOT(capsFun()));
    connect(m_deleteBtn,SIGNAL(clicked()),this,SLOT(deleteFun()));
    connect(m_changeBtn,SIGNAL(clicked()),this,SLOT(changeFun()));
    connect(m_spaceBtn,SIGNAL(clicked()),this,SLOT(spaceFun()));
    connect(m_sureBtn,SIGNAL(clicked()),this,SLOT(sureFun()));
    connect(m_cancelBtn,SIGNAL(clicked()),this,SLOT(cancelFun()));

    connect(m_spellEdit,SIGNAL(textChanged(QString)),this,SLOT(spellFun(QString)));

}
void Widget::showHanzi()
{
  //  m_selectFlag = flag;
    if(m_storeCount>=m_selectFlag*8)
    {
        m_ziBtn0 ->setText(m_storeHanziStr.at(m_selectFlag*8-8));
        m_ziBtn1 ->setText(m_storeHanziStr.at(m_selectFlag*8-7));
        m_ziBtn2 ->setText(m_storeHanziStr.at(m_selectFlag*8-6));
        m_ziBtn3 ->setText(m_storeHanziStr.at(m_selectFlag*8-5));
        m_ziBtn4 ->setText(m_storeHanziStr.at(m_selectFlag*8-4));
        m_ziBtn5 ->setText(m_storeHanziStr.at(m_selectFlag*8-3));
        m_ziBtn6 ->setText(m_storeHanziStr.at(m_selectFlag*8-2));
        m_ziBtn7 ->setText(m_storeHanziStr.at(m_selectFlag*8-1));
    }
    else
    {
        int extraNum = m_selectFlag*8 - m_storeCount;
        switch(extraNum)
        {
            case 1: m_ziBtn0 ->setText(m_storeHanziStr.at(m_selectFlag*8-8));
                    m_ziBtn1 ->setText(m_storeHanziStr.at(m_selectFlag*8-7));
                    m_ziBtn2 ->setText(m_storeHanziStr.at(m_selectFlag*8-6));
                    m_ziBtn3 ->setText(m_storeHanziStr.at(m_selectFlag*8-5));
                    m_ziBtn4 ->setText(m_storeHanziStr.at(m_selectFlag*8-4));
                    m_ziBtn5 ->setText(m_storeHanziStr.at(m_selectFlag*8-3));
                    m_ziBtn6 ->setText(m_storeHanziStr.at(m_selectFlag*8-2));
                    m_ziBtn7 ->setText("");break;
            case 2: m_ziBtn0 ->setText(m_storeHanziStr.at(m_selectFlag*8-8));
                    m_ziBtn1 ->setText(m_storeHanziStr.at(m_selectFlag*8-7));
                    m_ziBtn2 ->setText(m_storeHanziStr.at(m_selectFlag*8-6));
                    m_ziBtn3 ->setText(m_storeHanziStr.at(m_selectFlag*8-5));
                    m_ziBtn4 ->setText(m_storeHanziStr.at(m_selectFlag*8-4));
                    m_ziBtn5 ->setText(m_storeHanziStr.at(m_selectFlag*8-3));
                    m_ziBtn6 ->setText("");
                    m_ziBtn7 ->setText("");break;
            case 3: m_ziBtn0 ->setText(m_storeHanziStr.at(m_selectFlag*8-8));
                    m_ziBtn1 ->setText(m_storeHanziStr.at(m_selectFlag*8-7));
                    m_ziBtn2 ->setText(m_storeHanziStr.at(m_selectFlag*8-6));
                    m_ziBtn3 ->setText(m_storeHanziStr.at(m_selectFlag*8-5));
                    m_ziBtn4 ->setText(m_storeHanziStr.at(m_selectFlag*8-4));
                    m_ziBtn5 ->setText("");
                    m_ziBtn6 ->setText("");
                    m_ziBtn7 ->setText("");break;
            case 4: m_ziBtn0 ->setText(m_storeHanziStr.at(m_selectFlag*8-8));
                    m_ziBtn1 ->setText(m_storeHanziStr.at(m_selectFlag*8-7));
                    m_ziBtn2 ->setText(m_storeHanziStr.at(m_selectFlag*8-6));
                    m_ziBtn3 ->setText(m_storeHanziStr.at(m_selectFlag*8-5));
                    m_ziBtn4 ->setText("");
                    m_ziBtn5 ->setText("");
                    m_ziBtn6 ->setText("");
                    m_ziBtn7 ->setText("");break;
            case 5: m_ziBtn0 ->setText(m_storeHanziStr.at(m_selectFlag*8-8));
                    m_ziBtn1 ->setText(m_storeHanziStr.at(m_selectFlag*8-7));
                    m_ziBtn2 ->setText(m_storeHanziStr.at(m_selectFlag*8-6));
                    m_ziBtn3 ->setText("");
                    m_ziBtn4 ->setText("");
                    m_ziBtn5 ->setText("");
                    m_ziBtn6 ->setText("");
                    m_ziBtn7 ->setText("");break;
            case 6: m_ziBtn0 ->setText(m_storeHanziStr.at(m_selectFlag*8-8));
                    m_ziBtn1 ->setText(m_storeHanziStr.at(m_selectFlag*8-7));
                    m_ziBtn2 ->setText("");
                    m_ziBtn3 ->setText("");
                    m_ziBtn4 ->setText("");
                    m_ziBtn5 ->setText("");
                    m_ziBtn6 ->setText("");
                    m_ziBtn7 ->setText("");break;
            case 7: m_ziBtn0 ->setText(m_storeHanziStr.at(m_selectFlag*8-8));
                    m_ziBtn1 ->setText("");
                    m_ziBtn2 ->setText("");
                    m_ziBtn3 ->setText("");
                    m_ziBtn4 ->setText("");
                    m_ziBtn5 ->setText("");
                    m_ziBtn6 ->setText("");
                    m_ziBtn7 ->setText("");break;

        }
    }

}

void Widget::leftFun()
{
    m_selectFlag --;
    if(m_selectFlag<1)
        m_selectFlag =1;
    showHanzi();
}

void Widget::rightFun()
{
    m_selectFlag ++;
    if(m_selectFlag>(m_storeCount/8+1))
        m_selectFlag =m_storeCount/8+1;
    showHanzi();
}
void Widget::ziFun0()
{
    if(m_stateFlag==0)
        if(m_ziBtn0->text()!=NULL){
            m_hanziEdit->insert(m_ziBtn0->text());
            m_spellEdit->clear();
        }
}

void Widget::ziFun1()
{
    if(m_stateFlag==0)
        if(m_ziBtn1->text()!=NULL){
            m_hanziEdit->insert(m_ziBtn1->text());
            m_spellEdit->clear();
        }
}

void Widget::ziFun2()
{
    if(m_stateFlag==0)
        if(m_ziBtn2->text()!=NULL){
            m_hanziEdit->insert(m_ziBtn2->text());
            m_spellEdit->clear();
        }
}

void Widget::ziFun3()
{
    if(m_stateFlag==0)
        if(m_ziBtn3->text()!=NULL){
            m_hanziEdit->insert(m_ziBtn3->text());
            m_spellEdit->clear();
        }
}

void Widget::ziFun4()
{
    if(m_stateFlag==0)
        if(m_ziBtn4->text()!=NULL){
            m_hanziEdit->insert(m_ziBtn4->text());
            m_spellEdit->clear();
        }
}

void Widget::ziFun5()
{
    if(m_stateFlag==0)
        if(m_ziBtn5->text()!=NULL){
            m_hanziEdit->insert(m_ziBtn5->text());
            m_spellEdit->clear();
        }
}

void Widget::ziFun6()
{
    if(m_stateFlag==0)
        if(m_ziBtn6->text()!=NULL){
            m_hanziEdit->insert(m_ziBtn6->text());
            m_spellEdit->clear();
        }
}

void Widget::ziFun7()
{
    if(m_stateFlag==0)
        if(m_ziBtn7->text()!=NULL){
            m_hanziEdit->insert(m_ziBtn7->text());
            m_spellEdit->clear();
        }
}

void Widget::qFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("q");break;
    case 1:if(m_capsFlag)
                m_hanziEdit->insert("Q");
            else
                m_hanziEdit->insert("q");
            break;
     case 2:m_hanziEdit->insert("1");
     default:break;
    }
}

void Widget::wFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("w");break;
    case 1:if(m_capsFlag)
                m_hanziEdit->insert("W");
            else
                m_hanziEdit->insert("w");
            break;
     case 2:m_hanziEdit->insert("2");
     default:break;
    }
}

void Widget::eFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("e");break;
    case 1:if(m_capsFlag)
                m_hanziEdit->insert("E");
            else
                m_hanziEdit->insert("e");
            break;
     case 2:m_hanziEdit->insert("3");
     default:break;
    }
}

void Widget::rFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("r");break;
    case 1:if(m_capsFlag)
                m_hanziEdit->insert("R");
            else
                m_hanziEdit->insert("r");
            break;
     case 2:m_hanziEdit->insert("4");
     default:break;
    }
}

void Widget::tFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("t");break;
    case 1:if(m_capsFlag)
                m_hanziEdit->insert("T");
            else
                m_hanziEdit->insert("t");
            break;
     case 2:m_hanziEdit->insert("5");
     default:break;
    }
}

void Widget::yFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("y");break;
    case 1:if(m_capsFlag)
                m_hanziEdit->insert("Y");
            else
                m_hanziEdit->insert("y");
            break;
     case 2:m_hanziEdit->insert("6");
     default:break;
    }
}

void Widget::uFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("u");break;
    case 1:if(m_capsFlag)
                m_hanziEdit->insert("U");
            else
                m_hanziEdit->insert("u");
            break;
     case 2:m_hanziEdit->insert("7");
     default:break;
    }
}

void Widget::iFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("i");break;
    case 1:if(m_capsFlag)
                m_hanziEdit->insert("U");
            else
                m_hanziEdit->insert("i");
            break;
     case 2:m_hanziEdit->insert("8");
     default:break;
    }
}

void Widget::oFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("o");break;
    case 1:if(m_capsFlag)
                m_hanziEdit->insert("O");
            else
                m_hanziEdit->insert("o");
            break;
     case 2:m_hanziEdit->insert("9");
     default:break;
    }
}

void Widget::pFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("p");break;
    case 1:if(m_capsFlag)
                m_hanziEdit->insert("P");
            else
                m_hanziEdit->insert("p");
            break;
     case 2:m_hanziEdit->insert("0");
     default:break;
    }
}

void Widget::aFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("a");break;
    case 1:if(m_capsFlag)
                m_hanziEdit->insert("A");
            else
                m_hanziEdit->insert("a");
            break;
     case 2:m_hanziEdit->insert(",");
     default:break;
    }
}

void Widget::sFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("s");break;
    case 1:if(m_capsFlag)
                m_hanziEdit->insert("S");
            else
                m_hanziEdit->insert("s");
            break;
     case 2:m_hanziEdit->insert("。");
     default:break;
    }
}

void Widget::dFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("d");break;
    case 1:if(m_capsFlag)
                m_hanziEdit->insert("D");
            else
                m_hanziEdit->insert("d");
            break;
     case 2:m_hanziEdit->insert("、");
     default:break;
    }
}

void Widget::fFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("f");break;
    case 1:if(m_capsFlag)
                m_hanziEdit->insert("F");
            else
                m_hanziEdit->insert("f");
            break;
     case 2:m_hanziEdit->insert("；");
     default:break;
    }
}

void Widget::gFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("g");break;
    case 1:if(m_capsFlag)
                m_hanziEdit->insert("G");
            else
                m_hanziEdit->insert("g");
            break;
     case 2:m_hanziEdit->insert("：");
     default:break;
    }
}

void Widget::hFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("h");break;
    case 1:if(m_capsFlag)
                m_hanziEdit->insert("H");
            else
                m_hanziEdit->insert("h");
            break;
     case 2:m_hanziEdit->insert("？");
     default:break;
    }
}

void Widget::jFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("j");break;
    case 1:if(m_capsFlag)
                m_hanziEdit->insert("J");
            else
                m_hanziEdit->insert("j");
            break;
     case 2:m_hanziEdit->insert("！");
     default:break;
    }
}

void Widget::kFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("k");break;
    case 1:if(m_capsFlag)
                m_hanziEdit->insert("K");
            else
                m_hanziEdit->insert("k");
            break;
     case 2:m_hanziEdit->insert("·");
     default:break;
    }
}

void Widget::lFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("l");break;
    case 1:if(m_capsFlag)
                m_hanziEdit->insert("L");
            else
                m_hanziEdit->insert("l");
            break;
     case 2:m_hanziEdit->insert("＂");
     default:break;
    }
}

void Widget::zFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("z");break;
    case 1:if(m_capsFlag)
                m_hanziEdit->insert("Z");
            else
                m_hanziEdit->insert("z");
            break;
     case 2:m_hanziEdit->insert("〔");
     default:break;
    }
}

void Widget::xFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("x");break;
    case 1:if(m_capsFlag)
                m_hanziEdit->insert("X");
            else
                m_hanziEdit->insert("x");
            break;
     case 2:m_hanziEdit->insert("〕");
     default:break;
    }
}

void Widget::cFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("c");break;
    case 1:if(m_capsFlag)
                m_hanziEdit->insert("C");
            else
                m_hanziEdit->insert("c");
            break;
     case 2:m_hanziEdit->insert("●");
     default:break;
    }
}

void Widget::vFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("v");break;
    case 1:if(m_capsFlag)
                m_hanziEdit->insert("V");
            else
                m_hanziEdit->insert("v");
            break;
     case 2:m_hanziEdit->insert("※");
     default:break;
    }
}

void Widget::bFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("b");break;
    case 1:if(m_capsFlag)
                m_hanziEdit->insert("B");
            else
                m_hanziEdit->insert("b");
            break;
     case 2:m_hanziEdit->insert("＃");
     default:break;
    }
}

void Widget::nFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("n");break;
    case 1:if(m_capsFlag)
                m_hanziEdit->insert("N");
            else
                m_hanziEdit->insert("n");
            break;
     case 2:m_hanziEdit->insert("＠");
     default:break;
    }
}

void Widget::mFun()
{
    switch(m_stateFlag)
    {case 0:m_spellEdit->insert("m");break;
    case 1:if(m_capsFlag)
                m_hanziEdit->insert("M");
            else
                m_hanziEdit->insert("m");
            break;
     case 2:m_hanziEdit->insert("‰");
     default:break;
    }
}

void Widget::capsFun()
{
    if(m_stateFlag==1)
    {
        if(m_capsFlag)
        {
            setName_en();
            m_signBtn ->setText("en");
            m_capsFlag = false;
        }
        else
        {
            setName_EN();
            m_signBtn ->setText("EN");
            m_capsFlag = true;
        }
    }
}

void Widget::deleteFun()
{
    if(m_stateFlag!=0)
    {
        if((m_hanziEdit -> text()) != NULL){
           m_hanziEdit -> backspace();
        }
    }
    else
    {
        if((m_spellEdit -> text()) != NULL){
           m_spellEdit -> backspace();
        }
        else
        {
            if((m_hanziEdit -> text()) != NULL){
               m_hanziEdit -> backspace();
            }
        }
    }
}
void Widget::changeFun()
{
    m_stateFlag ++;
    if(m_stateFlag==3)
        m_stateFlag=0;
    switch(m_stateFlag)
    {
    case 0:setName_en();m_signBtn ->setText("CN");break;
    case 1:setName_EN();m_signBtn ->setText("EN");break;
    case 2:setName_sn();m_signBtn ->setText("*/123");break;
    default:break;
    }
    m_storeHanziStr = "";
    m_storeCount =0;
    m_ziBtn0 ->setText("");
    m_ziBtn1 ->setText("");
    m_ziBtn2 ->setText("");
    m_ziBtn3 ->setText("");
    m_ziBtn4 ->setText("");
    m_ziBtn5 ->setText("");
    m_ziBtn6 ->setText("");
    m_ziBtn7 ->setText("");
}

void Widget::spaceFun()
{

}

void Widget::sureFun()
{

}

void Widget::cancelFun()
{
    this ->close();
}

void Widget::spellFun(QString text)
{
    qDebug()<<"spell text = "<<text;
    QString str ,stringLine;
    int ret=0;
    m_in -> seek(0);
    if(text!=NULL)
    {
        while(!m_in->atEnd())
        {
            stringLine = m_in -> readLine();
            str = stringLine.left(text.length());
            ret = QString::compare(str, text);
            if(!ret)
            {
                QStringList strlist = stringLine.split(" ");
                stringLine = strlist[1];
                m_storeHanziStr = stringLine;
                m_storeCount = stringLine.count();
                m_selectFlag = 1;
                qDebug()<<"m_storeHanziStr="<<m_storeHanziStr<<"count ="<<m_storeCount;
                break;
            }
        }
        qDebug()<<"";
        showHanzi();
    }
}
