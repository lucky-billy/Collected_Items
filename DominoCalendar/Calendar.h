///:
/*****************************************************************************
 **                                                                         **
 **                               .======.                                  **
 **                               | INRI |                                  **
 **                               |      |                                  **
 **                               |      |                                  **
 **                      .========'      '========.                         **
 **                      |   _      xxxx      _   |                         **
 **                      |  /_;-.__ / _\  _.-;_\  |                         **
 **                      |     `-._`'`_/'`.-'     |                         **
 **                      '========.`\   /`========'                         **
 **                               | |  / |                                  **
 **                               |/-.(  |                                  **
 **                               |\_._\ |                                  **
 **                               | \ \`;|                                  **
 **                               |  > |/|                                  **
 **                               | / // |                                  **
 **                               | |//  |                                  **
 **                               | \(\  |                                  **
 **                               |  ``  |                                  **
 **                               |      |                                  **
 **                               |      |                                  **
 **                               |      |                                  **
 **                               |      |                                  **
 **                   \\    _  _\\| \//  |//_   _ \// _                     **
 **                  ^ `^`^ ^`` `^ ^` ``^^`  `^^` `^ `^                     **
 **                                                                         **
 **                    Copyright © 1997-2013 by Tong G.                     **
 **                          ALL RIGHTS RESERVED.                           **
 **                                                                         **
 ****************************************************************************/

#pragma once
#include "main.h"
#include <QCalendarWidget>
#include <QWidget>
#include <QSettings>
#include <QCloseEvent>

class _AnalogClock;
class QPushButton;
class _DIYButton;
class QCalendarWidget;
class QCheckBox;
class QComboBox;
class QDate;
class QDateEdit;
class QGridLayout;
class QGroupBox;
class QLabel;
class QAction;

class _Calendar : public QWidget
    {
    Q_OBJECT

public:
    _Calendar();

protected slots:
    void closeEvent(QCloseEvent *_Event);
    void _WriteSettings();
    void _ReadSettings();

    void _ConfigureButtonClicked();
    void _LocaleChanged(int _Index);
    void _FirstDayChanged(int _Index);
    void _SelectionModeChanged(int _Index);
    void _HorizontalHeaderChanged(int _Index);
    void _VerticalHeaderChanged(int _Index);
    void _SelectedDateChanged();
    void _MinimumDateChanged(const QDate &_Date);
    void _MaximumDateChanged(const QDate &_Date);
    void _WeekdayFormatChanged();
    void _WeekendFormatChanged();
    void _ReformatHeaders();
    void _ReformatCalendarPage();

    void _ShowClockButtonClicked();
    void _GoToTongGsBirthday();
    void _ReturnToday();
    void _AboutButtonClicked();

protected:
    void _CreatePreviewGroupBox();          //..创建"预览"组
    void _CreateGeneralOptionGroupBox();    //..创建"常规选项"组
    void _CreateDatesGroupBox();            //..创建"日期"组
    void _CreateTextFormatGroupBox();       //..创建"文本格式"组
    QComboBox *_CreateColorComboBox();

    QDate m_Today;  //..m_Today记录着今天的日期

    QGroupBox*       m_PreviewGroupBox;
    QGridLayout*     m_PreviewLayout;
    QCalendarWidget* m_Calendar;
    QLabel*          m_TimeZoneLabel;
    _DIYButton*      m_ConfigureButton;
    _DIYButton*      m_ReturnToday;
    _DIYButton*      m_TimeZoneButton;
    _DIYButton*      m_AboutButton;
#if 0
    // TODO:  Context Menu
    void _CreateContextMenu();
    QAction*         m_ShowClockAction;
    QAction*         m_ReturnTodayAction;
    QAction*         m_ConfigureButtonAction;
#endif

    QGroupBox* m_GeneralOptionsGroupBox;
    QLabel*    m_LocaleLabel;
    QLabel*    m_WeekStartLabelBegin;
    QLabel*    m_WeekStartLabelEnd;
    QLabel*    m_SelectionModeLabel;
    QComboBox* m_LocaleComboBox;
    QComboBox* m_WeekStartComboBox;
    QComboBox* m_SelectionModeComboBox;
    QCheckBox* m_GridCheckBox;
    QCheckBox* m_NavigationBarCheckBox;
    QLabel*    m_HorizontalHeaderLabel;
    QLabel*    m_VerticalHeaderLabel;
    QComboBox* m_HorizontalHeaderComboBox;
    QComboBox* m_VerticalHeaderComboBox;
    _AnalogClock* m_AnalogClock;
    _DIYButton* m_ShowClock;

    QGroupBox* m_DatesGroupBox;
    QLabel*    m_MinimumDateLabel;
    QLabel*    m_CurrentDateLabel;
    QLabel*    m_MaximumDateLabel;
    QDateEdit* m_MinimumDateEdit;
    QDateEdit* m_CurrentDateEdit;
    QDateEdit* m_MaximumDateEdit;

    QGroupBox* m_TextFormatsGroupBox;
    QLabel*    m_WeekdayColorLabel;
    QLabel*    m_WeekendColorLabel;
    QLabel*    m_HeaderTextLabel;
    QComboBox* m_WeekdayColorComboBox;
    QComboBox* m_WeekendColorComboBox;
    QComboBox* m_HeaderTextComboBox;
    QCheckBox* m_FirstFridayInGreenCheckBox;
    QCheckBox* m_TongGsBirthdayInYellowCheckBox;
    QPushButton* m_GoToTongGsBirthday;
    };

 ////////////////////////////////////////////////////////////////////////////

 /***************************************************************************
 **                                                                        **
 **      _________                                      _______            **
 **     |___   ___|                                   / ______ \           **
 **         | |     _______   _______   _______      | /      |_|          **
 **         | |    ||     || ||     || ||     ||     | |    _ __           **
 **         | |    ||     || ||     || ||     ||     | |   |__  \          **
 **         | |    ||     || ||     || ||     ||     | \_ _ __| |  _       **
 **         |_|    ||_____|| ||     || ||_____||      \________/  |_|      **
 **                                           ||                           **
 **                                    ||_____||                           **
 **                                                                        **
 ***************************************************************************/
///:~
