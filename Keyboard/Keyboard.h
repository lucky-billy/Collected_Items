#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QWidget>
#include <QToolButton>
#include <QLineEdit>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QPalette>
class Widget : public QWidget
{
    Q_OBJECT
    
public:
    Widget(QWidget *parent = 0);
    ~Widget();

    void init();
    void setName_EN();
    void setName_en();
    void setName_sn();
    void setPlace();
    void setConnect();
    void showHanzi();
private:
    QLineEdit *m_hanziEdit;
    QLineEdit *m_spellEdit;

    QToolButton *m_leftBtn;
    QToolButton *m_rightBtn;

    QToolButton *m_ziBtn0;
    QToolButton *m_ziBtn1;
    QToolButton *m_ziBtn2;
    QToolButton *m_ziBtn3;
    QToolButton *m_ziBtn4;
    QToolButton *m_ziBtn5;
    QToolButton *m_ziBtn6;
    QToolButton *m_ziBtn7;

    QToolButton *m_QBtn;
    QToolButton *m_WBtn;
    QToolButton *m_EBtn;
    QToolButton *m_RBtn;
    QToolButton *m_TBtn;
    QToolButton *m_YBtn;
    QToolButton *m_UBtn;
    QToolButton *m_IBtn;
    QToolButton *m_OBtn;
    QToolButton *m_PBtn;
    QToolButton *m_ABtn;
    QToolButton *m_SBtn;
    QToolButton *m_DBtn;
    QToolButton *m_FBtn;
    QToolButton *m_GBtn;
    QToolButton *m_HBtn;
    QToolButton *m_JBtn;
    QToolButton *m_KBtn;
    QToolButton *m_LBtn;
    QToolButton *m_ZBtn;
    QToolButton *m_XBtn;
    QToolButton *m_CBtn;
    QToolButton *m_VBtn;
    QToolButton *m_BBtn;
    QToolButton *m_NBtn;
    QToolButton *m_MBtn;

    QToolButton *m_capsBtn;
    QToolButton *m_deleteBtn;
    QToolButton *m_signBtn;
    QToolButton *m_changeBtn;
    QToolButton *m_spaceBtn;
    QToolButton *m_sureBtn;
    QToolButton *m_cancelBtn;

    QFile		*m_file;
    QTextStream	*m_in;
    QString m_storeHanziStr;
    int m_storeCount;
    int m_stateFlag;
    int m_selectFlag;
    bool m_capsFlag;
private slots:
    void leftFun();
    void rightFun();

    void ziFun0();
    void ziFun1();
    void ziFun2();
    void ziFun3();
    void ziFun4();
    void ziFun5();
    void ziFun6();
    void ziFun7();

    void qFun();
    void wFun();
    void eFun();
    void rFun();
    void tFun();
    void yFun();
    void uFun();
    void iFun();
    void oFun();
    void pFun();
    void aFun();
    void sFun();
    void dFun();
    void fFun();
    void gFun();
    void hFun();
    void jFun();
    void kFun();
    void lFun();
    void zFun();
    void xFun();
    void cFun();
    void vFun();
    void bFun();
    void nFun();
    void mFun();

    void capsFun();
    void deleteFun();
    void changeFun();
    void spaceFun();
    void sureFun();
    void cancelFun();

    void spellFun(QString);
};

#endif // KEYBOARD_H
