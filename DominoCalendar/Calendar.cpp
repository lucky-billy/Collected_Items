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

#include "Calendar.h"
#include <QIcon>
#include <QCheckBox>
#include <QComboBox>
#include <QDate>
#include <QDateEdit>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QCalendarWidget>
#include <QList>
#include <QTextCharFormat>
#include <Clock.h>
#include <QPushButton>
#include <QToolButton>
#include "SettingButton.h"
#include <QMessageBox>
#include <QAction>
//.._Calendar类实现

    /* 构造函数实现 */
    _Calendar::_Calendar()
        {
        _CreatePreviewGroupBox();
        _CreateGeneralOptionGroupBox();
        _CreateDatesGroupBox();
        _CreateTextFormatGroupBox();

        QGridLayout* _MainLayout = new QGridLayout;
        _MainLayout->addWidget(m_PreviewGroupBox, 0, 0);
        _MainLayout->addWidget(m_GeneralOptionsGroupBox, 0, 1);
        _MainLayout->addWidget(m_DatesGroupBox, 1, 0);
        _MainLayout->addWidget(m_TextFormatsGroupBox, 1, 1);
        _MainLayout->setSizeConstraint(QLayout::SetFixedSize);

        m_PreviewLayout->setRowMinimumHeight(
                    0, m_Calendar->sizeHint().height());
        m_PreviewLayout->setColumnMinimumWidth(
                    0, m_Calendar->sizeHint().width());

        m_Today = QDate(m_Calendar->selectedDate());
        setLayout(_MainLayout);
        setWindowTitle(tr("Domino Calendar"));
        setWindowIcon(QIcon(":/Images/Icon.ico"));
        _ReadSettings();
        }

    /////////////////////////////////////////////////////////////////
    //..protected部分

    /* _CreatePreviewGroupBox()函数实现 */
    void _Calendar::_CreatePreviewGroupBox()
        {
        m_PreviewGroupBox = new QGroupBox(tr("日历"));
        m_Calendar = new QCalendarWidget;
        m_Calendar->setMinimumDate(QDate(1900, 1, 1));
        m_Calendar->setMaximumDate(QDate(3000, 1, 1));
        m_Calendar->setGridVisible(true);
        connect(m_Calendar, SIGNAL(currentPageChanged(int, int)),
                this, SLOT(_ReformatCalendarPage()));

        m_ReturnToday = new _DIYButton;
        m_ReturnToday->setIcon(QIcon(":/Images/ShowToday.ico"));
        m_ReturnToday->setToolTip(tr("回到今天"));
        connect(m_ReturnToday, SIGNAL(clicked()),
                this, SLOT(_ReturnToday()));

        m_ShowClock = new _DIYButton;
        m_ShowClock->setIcon(QIcon(":/Images/Clock.ico"));
        m_ShowClock->setToolTip(tr("时钟"));

        connect(m_ShowClock, SIGNAL(clicked()),
                this, SLOT(_ShowClockButtonClicked()));

        m_ConfigureButton = new _DIYButton;
        m_ConfigureButton->setText(QStringLiteral("☞"));
        m_ConfigureButton->setIcon(QIcon(":/Images/Configure.ico"));
        m_ConfigureButton->setToolTip(tr("显示设置"));
        connect(m_ConfigureButton, SIGNAL(clicked()),
                this, SLOT(_ConfigureButtonClicked()));

        m_AboutButton = new _DIYButton;
        m_AboutButton->setIcon(QIcon(":/Images/Help.ico"));
        m_AboutButton->setToolTip(QStringLiteral("关于产品"));

        m_TimeZoneLabel = new QLabel(QStringLiteral("时区："));
        m_TimeZoneButton = new _DIYButton;
        m_TimeZoneButton->setIcon(QIcon(":/Images/flag-cn.ico"));
        connect(m_AboutButton, SIGNAL(clicked()),
                this, SLOT(_AboutButtonClicked()));
#if 0
        // TODO:  Context Menu
        m_ReturnTodayAction = new QAction(tr("回到今天"), this);
        m_ReturnTodayAction->setIcon(QIcon(":/Images/ShowToday.ico"));
        m_ReturnTodayAction->setShortcut(tr("F9"));
        m_ShowClockAction = new QAction(tr("显示时钟"), this);
        m_ShowClockAction->setIcon(QIcon(":/Images/Clock.ico"));
        m_ShowClockAction->setShortcut(tr("F10"));
        m_ConfigureButtonAction = new QAction(tr("设置"), this);
        m_ConfigureButtonAction->setIcon(QIcon(":/Images/Configure.ico"));
        m_ConfigureButtonAction->setShortcut(tr("Ctrl+G"));
        connect(m_ReturnTodayAction, SIGNAL(triggered()),
                this, SLOT(_ReturnToday()));
        connect(m_ShowClockAction, SIGNAL(triggered()),
                SLOT(_ShowClockButtonClicked()));
        connect(m_ConfigureButtonAction, SIGNAL(clicked()),
                this, SLOT(_ConfigureButtonClicked()));
#endif
        /////////////////////////////////////////////////////////////

        m_PreviewLayout = new QGridLayout;
        m_PreviewLayout->addWidget(m_Calendar, 0, 0, Qt::AlignCenter);

        QHBoxLayout* _ShowLayout = new QHBoxLayout;
        _ShowLayout->addWidget(m_TimeZoneLabel);
        _ShowLayout->addWidget(m_TimeZoneButton);
        _ShowLayout->addStretch();
        _ShowLayout->addWidget(m_ReturnToday);
        _ShowLayout->addWidget(m_ShowClock);
        _ShowLayout->addWidget(m_ConfigureButton);
        _ShowLayout->addWidget(m_AboutButton);

        QVBoxLayout* _MainLayout = new QVBoxLayout;
        _MainLayout->addLayout(m_PreviewLayout);
        _MainLayout->addLayout(_ShowLayout);
        m_PreviewGroupBox->setLayout(_MainLayout);
        }

    /* _CreateGeneralOptionsGroupBox()函数实现 */
    void _Calendar::_CreateGeneralOptionGroupBox()
        {
        m_GeneralOptionsGroupBox = new QGroupBox(QStringLiteral("常规选项"));

        //..创建并设置m_LocaleComboBox和m_LocaleLabel
        m_LocaleComboBox = new QComboBox;
        int _CurLocaleIndex = -1;
        int _Index = 0;
        for (int _Lang = QLocale::C; _Lang <= QLocale::LastLanguage; ++_Lang)
            {
            QLocale::Language Lang = static_cast<QLocale::Language>(_Lang);
            QList<QLocale::Country> _Countries
                    = QLocale::countriesForLanguage(Lang);
            for (int _I = 0; _I < _Countries.count(); ++_I)
                {
                QLocale::Country _Country = _Countries.at(_I);
                QString _Label = QLocale::languageToString(Lang);
                _Label += QLatin1Char('/');
                _Label += QLocale::countryToString(_Country);
                QLocale _Locale(Lang, _Country);
                if (this->locale().language() == Lang &&
                        this->locale().country() == _Country)
                    _CurLocaleIndex = _Index;
                m_LocaleComboBox->addItem(_Label, _Locale);
                ++_Index;
                }
            }
        if (_CurLocaleIndex != -1)
            m_LocaleComboBox->setCurrentIndex(_CurLocaleIndex);
        m_LocaleLabel = new QLabel(QStringLiteral("语言/地区(&L)："));
        m_LocaleLabel->setBuddy(m_LocaleComboBox);
        connect(m_LocaleComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(_LocaleChanged(int)));

        /////////////////////////////////////////////////////////////

        //..创建并设置m_SelectionModeLabel和m_SelectionModeLabel
        m_SelectionModeComboBox = new QComboBox;
        m_SelectionModeComboBox->addItem(QStringLiteral("可单选"),
                                         QCalendarWidget::SingleSelection);
        m_SelectionModeComboBox->addItem(QStringLiteral("不可选"),
                                         QCalendarWidget::NoSelection);

        m_SelectionModeLabel = new QLabel(QStringLiteral("选择模式(&S)："));
        m_SelectionModeLabel->setBuddy(m_SelectionModeComboBox);
        connect(m_SelectionModeComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(_SelectionModeChanged(int)));

        /////////////////////////////////////////////////////////////

        //..创建并设置m_GridCheckBox和m_NavigationBarCheckBar
        m_GridCheckBox = new QCheckBox(QStringLiteral("显示网格(&G)"));
        m_GridCheckBox->setChecked(true);
        connect(m_GridCheckBox, SIGNAL(toggled(bool)),
                m_Calendar, SLOT(setGridVisible(bool)));

        m_NavigationBarCheckBox = new QCheckBox(QStringLiteral("显示导航栏(&N)"));
        m_NavigationBarCheckBox->setChecked(true);
        connect(m_NavigationBarCheckBox, SIGNAL(toggled(bool)),
                m_Calendar, SLOT(setNavigationBarVisible(bool)));

        /////////////////////////////////////////////////////////////

        //..创建并设置m_HorizontalHeaderComboBox和m_HorizontalHeaderLabel
        m_HorizontalHeaderComboBox = new QComboBox;
        m_HorizontalHeaderComboBox->addItem(QStringLiteral("简写"),
                                         QCalendarWidget::SingleLetterDayNames);
        m_HorizontalHeaderComboBox->addItem(QStringLiteral("完整"),
                                            QCalendarWidget::ShortDayNames);
        m_HorizontalHeaderComboBox->addItem(QStringLiteral("隐藏"),
                                  QCalendarWidget::NoHorizontalHeader);
        m_HorizontalHeaderComboBox->setCurrentIndex(1);

        m_HorizontalHeaderLabel = new QLabel(QStringLiteral("水平标头(&H)："));
        m_HorizontalHeaderLabel->setBuddy(m_HorizontalHeaderComboBox);
        connect(m_HorizontalHeaderComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(_HorizontalHeaderChanged(int)));

        /////////////////////////////////////////////////////////////

        //..创建并设置m_VerticalHeaderComboBox和m_VerticalHeaderLabel
        m_VerticalHeaderComboBox = new QComboBox;
        m_VerticalHeaderComboBox->addItem(QStringLiteral("显示ISO星期代码"),
                                          QCalendarWidget::ISOWeekNumbers);
        m_VerticalHeaderComboBox->addItem(QStringLiteral("隐藏"),
                                          QCalendarWidget::NoVerticalHeader);
        m_VerticalHeaderComboBox->setCurrentIndex(0);
        m_VerticalHeaderLabel = new QLabel(QStringLiteral("垂直标头："));
        m_VerticalHeaderLabel->setBuddy(m_VerticalHeaderComboBox);
        connect(m_VerticalHeaderComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(_VerticalHeaderChanged(int)));

        /////////////////////////////////////////////////////////////

        //..创建并设置m_WeekStaryComboBox和m_WeekStartLabel
        m_WeekStartComboBox = new QComboBox;
        m_WeekStartComboBox->addItem(QStringLiteral("星期日"), Qt::Sunday);
        m_WeekStartComboBox->addItem(QStringLiteral("星期一"), Qt::Monday);
        m_WeekStartComboBox->addItem(QStringLiteral("星期二"), Qt::Tuesday);
        m_WeekStartComboBox->addItem(QStringLiteral("星期三"), Qt::Wednesday);
        m_WeekStartComboBox->addItem(QStringLiteral("星期四"), Qt::Thursday);
        m_WeekStartComboBox->addItem(QStringLiteral("星期五"), Qt::Friday);
        m_WeekStartComboBox->addItem(QStringLiteral("星期六"), Qt::Saturday);

        m_WeekStartLabelBegin = new QLabel(QStringLiteral("每星期以"));
        m_WeekStartLabelEnd = new QLabel(QStringLiteral("开始"));
        connect(m_WeekStartComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(_FirstDayChanged(int)));
#if 1
        QHBoxLayout* _CheckBoxLayout = new QHBoxLayout;
        _CheckBoxLayout->addWidget(m_GridCheckBox);
        _CheckBoxLayout->addStretch();
        _CheckBoxLayout->addWidget(m_NavigationBarCheckBox);

        QHBoxLayout* _WeekStartsLayout = new QHBoxLayout;
        _WeekStartsLayout->addWidget(m_WeekStartLabelBegin);
        _WeekStartsLayout->addWidget(m_WeekStartComboBox);
        _WeekStartsLayout->addWidget(m_WeekStartLabelEnd);

        QGridLayout* _GeneralOptionsLayout = new QGridLayout;
        _GeneralOptionsLayout->addWidget(m_LocaleLabel, 0, 0);
        _GeneralOptionsLayout->addWidget(m_LocaleComboBox, 0, 1);
        _GeneralOptionsLayout->addWidget(m_SelectionModeLabel, 1, 0);
        _GeneralOptionsLayout->addWidget(m_SelectionModeComboBox, 1, 1);
        _GeneralOptionsLayout->addLayout(_CheckBoxLayout, 2, 0, 1, 2);
        _GeneralOptionsLayout->addLayout(_WeekStartsLayout, 3, 0);
        _GeneralOptionsLayout->addWidget(m_HorizontalHeaderLabel, 4, 0);
        _GeneralOptionsLayout->addWidget(m_HorizontalHeaderComboBox, 4, 1);
        _GeneralOptionsLayout->addWidget(m_VerticalHeaderLabel, 5, 0);
        _GeneralOptionsLayout->addWidget(m_VerticalHeaderComboBox, 5, 1);
#endif

#if 0
        QHBoxLayout* _LocaleLayout = new QHBoxLayout;
        _LocaleLayout->addWidget(m_LocaleLabel);
        _LocaleLayout->addWidget(m_LocaleComboBox);

        QHBoxLayout* _SelectionModeLayout = new QHBoxLayout;
        _SelectionModeLayout->addWidget(m_SelectionModeLabel);
        _SelectionModeLayout->addWidget(m_SelectionModeComboBox);

        QHBoxLayout* _CheckBoxesLayout = new QHBoxLayout;
        _CheckBoxesLayout->addWidget(m_GridCheckBox);
        _CheckBoxesLayout->addWidget(m_NavigationBarCheckBox);

        QHBoxLayout* _HorizontalHeaderLayout = new QHBoxLayout;
        _HorizontalHeaderLayout->addWidget(m_HorizontalHeaderLabel);
        _HorizontalHeaderLayout->addWidget(m_HorizontalHeaderComboBox);

        QHBoxLayout* _VerticalHeaderLayout = new QHBoxLayout;
        _VerticalHeaderLayout->addWidget(m_VerticalHeaderLabel);
        _VerticalHeaderLayout->addWidget(m_VerticalHeaderComboBox);

        QVBoxLayout* _LeftLayout = new QVBoxLayout;
        _LeftLayout->addLayout(_LocaleLayout);
        _LeftLayout->addLayout(_SelectionModeLayout);
        _LeftLayout->addLayout(_CheckBoxesLayout);
        _LeftLayout->addLayout(_HorizontalHeaderLayout);
        _LeftLayout->addLayout(_VerticalHeaderLayout);

        QHBoxLayout* _MainLayout = new QHBoxLayout;
        _MainLayout->addLayout(_LeftLayout);
        _MainLayout->addWidget(m_AnalogClock);
#endif
        m_GeneralOptionsGroupBox->setLayout(_GeneralOptionsLayout);
        m_GeneralOptionsGroupBox->setHidden(true);

        _FirstDayChanged(m_WeekStartComboBox->currentIndex());
        _SelectionModeChanged(m_SelectionModeComboBox->currentIndex());
        _HorizontalHeaderChanged(m_HorizontalHeaderComboBox->currentIndex());
        _VerticalHeaderChanged(m_VerticalHeaderComboBox->currentIndex());
        }

    /* _CreateDatesGroupBox()函数实现 */
    void _Calendar::_CreateDatesGroupBox()
        {
        m_DatesGroupBox = new QGroupBox(QStringLiteral("日期"));

        m_GoToTongGsBirthday
                = new QPushButton(QStringLiteral("前往桐哥生日"));
        connect(m_GoToTongGsBirthday, SIGNAL(clicked()),
                this, SLOT(_GoToTongGsBirthday()));

        //..m_MinimumDateEdit和m_MinimumDateLabel的创建与设置
        m_MinimumDateEdit = new QDateEdit;
        m_MinimumDateEdit->setDisplayFormat("MMM d yyyy");
        m_MinimumDateEdit->setDateRange(m_Calendar->minimumDate(),
                                        m_Calendar->maximumDate());
        m_MinimumDateEdit->setDate(m_Calendar->minimumDate());

        m_MinimumDateLabel = new QLabel(QStringLiteral("最小日期(&M)："));
        m_MinimumDateLabel->setBuddy(m_MinimumDateEdit);
        connect(m_MinimumDateEdit, SIGNAL(dateChanged(QDate)),
                this, SLOT(_MinimumDateChanged(QDate)));

        //..m_CurrentDateEdit和m_CurrentDateLabel的创建与设置
        m_CurrentDateEdit = new QDateEdit;
        m_CurrentDateEdit->setDisplayFormat("MMM d yyyy");
        m_CurrentDateEdit->setDate(m_Calendar->selectedDate());
        m_CurrentDateEdit->setDateRange(m_Calendar->minimumDate(),
                                        m_Calendar->maximumDate());
        m_CurrentDateLabel = new QLabel(QStringLiteral("当前日期(&C)："));
        m_CurrentDateLabel->setBuddy(m_CurrentDateEdit);
        connect(m_CurrentDateEdit, SIGNAL(dateChanged(QDate)),
                m_Calendar, SLOT(setSelectedDate(QDate)));

        //..m_MaximumDateEdit和m_MaximumDateLabel的创建与设置
        m_MaximumDateEdit = new QDateEdit;
        m_MaximumDateEdit->setDisplayFormat("MMM d yyyy");
        m_MaximumDateEdit->setDate(m_Calendar->maximumDate());
        m_MaximumDateEdit->setDateRange(m_Calendar->minimumDate(),
                                        m_Calendar->maximumDate());
        m_MaximumDateLabel = new QLabel(QStringLiteral("最大日期(&X)："));
        m_MaximumDateLabel->setBuddy(m_MaximumDateEdit);
        connect(m_MaximumDateEdit, SIGNAL(dateChanged(QDate)),
                this, SLOT(_MaximumDateChanged(QDate)));

        /////////////////////////////////////////////////////////////

        connect(m_Calendar, SIGNAL(selectionChanged()),
                this, SLOT(_SelectedDateChanged()));

        QGridLayout* _DatesGroupBoxLayout = new QGridLayout;
        _DatesGroupBoxLayout->addWidget(m_GoToTongGsBirthday, 0, 0);
        _DatesGroupBoxLayout->addWidget(m_MinimumDateLabel, 1, 0);
        _DatesGroupBoxLayout->addWidget(m_MinimumDateEdit, 1, 1);
        _DatesGroupBoxLayout->addWidget(m_CurrentDateLabel, 2, 0);
        _DatesGroupBoxLayout->addWidget(m_CurrentDateEdit, 2, 1);
        _DatesGroupBoxLayout->addWidget(m_MaximumDateLabel, 3, 0);
        _DatesGroupBoxLayout->addWidget(m_MaximumDateEdit, 3, 1);

        m_DatesGroupBox->setLayout(_DatesGroupBoxLayout);
        m_DatesGroupBox->setHidden(true);
        }

    /* _CreateTextFormatsGroupBox()函数实现 */
    void _Calendar::_CreateTextFormatGroupBox()
        {
        m_TextFormatsGroupBox = new QGroupBox(QStringLiteral("格式设置"));

        //..m_WeekdayColorComboBox和m_WeekdayColorLabel的创建与设置
        m_WeekdayColorComboBox = _CreateColorComboBox();
        m_WeekdayColorComboBox->setCurrentIndex(
                    m_WeekdayColorComboBox->findText(QStringLiteral("黑色")));
        m_WeekdayColorLabel = new QLabel(QStringLiteral("星期一至星期五的颜色(&W)："));
        m_WeekdayColorLabel->setBuddy(m_WeekdayColorComboBox);

        //..m_WeekendColorComboBox和m_WeekendColorLabel的创建与设置
        m_WeekendColorComboBox = _CreateColorComboBox();
        m_WeekendColorComboBox->setCurrentIndex(
                    m_WeekendColorComboBox->findText(QStringLiteral("红色")));
        m_WeekendColorLabel = new QLabel(QStringLiteral("周末的颜色(&E)："));
        m_WeekendColorLabel->setBuddy(m_WeekendColorComboBox);

        connect(m_WeekdayColorComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(_WeekdayFormatChanged()));
        connect(m_WeekdayColorComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(_ReformatCalendarPage()));
        connect(m_WeekendColorComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(_WeekendFormatChanged()));
        connect(m_WeekendColorComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(_ReformatCalendarPage()));

        //..m_HeaderTextComboBox和m_HeaderTextLabel的创建与设置
        m_HeaderTextComboBox = new QComboBox;
        m_HeaderTextComboBox->addItem(QStringLiteral("黑体"));
        m_HeaderTextComboBox->addItem(QStringLiteral("斜体"));
        m_HeaderTextComboBox->addItem(QStringLiteral("普通"));
        m_HeaderTextLabel = new QLabel(QStringLiteral("标头字体(&H)："));
        m_HeaderTextLabel->setBuddy(m_HeaderTextComboBox);
        connect(m_HeaderTextComboBox, SIGNAL(currentIndexChanged(QString)),
                this, SLOT(_ReformatHeaders()));

        m_FirstFridayInGreenCheckBox
                = new QCheckBox(QStringLiteral("每月第一个星期五标记为绿色(&F)"));
        connect(m_FirstFridayInGreenCheckBox, SIGNAL(toggled(bool)),
                this, SLOT(_ReformatCalendarPage()));

        m_TongGsBirthdayInYellowCheckBox
                = new QCheckBox(QStringLiteral("桐哥生日标记为黄色(&Y)"));
        connect(m_TongGsBirthdayInYellowCheckBox, SIGNAL(toggled(bool)),
                this, SLOT(_ReformatCalendarPage()));

        /////////////////////////////////////////////////////////////

        QVBoxLayout* _CheckBoxLayout = new QVBoxLayout;
        _CheckBoxLayout->addWidget(m_FirstFridayInGreenCheckBox);
        _CheckBoxLayout->addStretch();
        _CheckBoxLayout->addWidget(m_TongGsBirthdayInYellowCheckBox);

        QGridLayout* _TextFormatsLayout = new QGridLayout;
        _TextFormatsLayout->addWidget(m_WeekdayColorLabel, 0, 0);
        _TextFormatsLayout->addWidget(m_WeekdayColorComboBox, 0, 1);
        _TextFormatsLayout->addWidget(m_WeekendColorLabel, 1, 0);
        _TextFormatsLayout->addWidget(m_WeekendColorComboBox, 1, 1);
        _TextFormatsLayout->addWidget(m_HeaderTextLabel, 2, 0);
        _TextFormatsLayout->addWidget(m_HeaderTextComboBox, 2, 1);
        _TextFormatsLayout->addLayout(_CheckBoxLayout, 3, 0);

        m_TextFormatsGroupBox->setLayout(_TextFormatsLayout);
        m_TextFormatsGroupBox->setHidden(true);

        _WeekdayFormatChanged();
        _WeekendFormatChanged();
        _ReformatHeaders();
        _ReformatCalendarPage();
        }
#if 0
    /* _CreateContextMenu()函数实现 */
    void _Calendar::_CreateContextMenu()
        {
        m_PreviewGroupBox->addAction(m_ShowClockAction);
        m_PreviewGroupBox->addAction(m_ReturnTodayAction);
        m_PreviewGroupBox->addAction(m_ConfigureButtonAction);
        m_PreviewGroupBox->setContextMenuPolicy(Qt::ActionsContextMenu);
        }
#endif
    /* _CreateColorComboBox()函数实现 */
    QComboBox* _Calendar::_CreateColorComboBox()
        {
        QComboBox* _ComboBox = new QComboBox;
        _ComboBox->addItem(QStringLiteral("黑色"), QColor(Qt::black));
        _ComboBox->addItem(QStringLiteral("红色"), QColor(Qt::red));
        _ComboBox->addItem(QStringLiteral("蓝色"), QColor(Qt::blue));
        _ComboBox->addItem(QStringLiteral("品红"), QColor(Qt::magenta));
        _ComboBox->addItem(QStringLiteral("绿色"), QColor(Qt::green));
        _ComboBox->addItem(QStringLiteral("灰色"), QColor(Qt::gray));

        return _ComboBox;
        }

    //..protected槽实现

    /* closeEvent()函数重写 */
    void _Calendar::closeEvent(QCloseEvent *_Event)
        {
        _WriteSettings();
        _Event->accept();
        }

    /* _WriteSettings()槽实现 */
    void _Calendar::_WriteSettings()
        {
        QSettings _Settings("Domino Inc. & Tong G.", "Domino Calendar");

        _Settings.setValue("Language", m_LocaleComboBox->currentIndex());
        _Settings.setValue("WeekStart", m_WeekStartComboBox->currentIndex());
        _Settings.setValue("Selection Mode",
                           m_SelectionModeComboBox->currentIndex());
        _Settings.setValue("Show Grid", m_GridCheckBox->isChecked());
        _Settings.setValue("Show Navigation Bar",
                           m_NavigationBarCheckBox->isChecked());
        _Settings.setValue("Horizontal Header",
                           m_HorizontalHeaderComboBox->currentIndex());
        _Settings.setValue("Vertival Header",
                           m_VerticalHeaderComboBox->currentIndex());
        _Settings.setValue("WeekdayColor",
                           m_WeekdayColorComboBox->currentIndex());
        _Settings.setValue("WeekendColor",
                           m_WeekendColorComboBox->currentIndex());
        _Settings.setValue("Header Font", m_HeaderTextComboBox->currentIndex());
        _Settings.setValue("First Friday",
                           m_FirstFridayInGreenCheckBox->isChecked());
        _Settings.setValue("Tong G.'s Birthday",
                           m_TongGsBirthdayInYellowCheckBox->isChecked());
        // _Settings.setValue("Clock Geometry", m_AnalogClock->saveGeometry());
        /* Bug */
        }

    /* _ReadSettings()槽实现 */
    void _Calendar::_ReadSettings()
        {
        QSettings _Settings("Domino Inc. & Tong G.", "Domino Calendar");

        m_LocaleComboBox->setCurrentIndex(
                    _Settings.value("Language").toInt());
        m_WeekStartComboBox->setCurrentIndex(
                    _Settings.value("WeekStart").toInt());
        m_SelectionModeComboBox->setCurrentIndex(
                    _Settings.value("Selection Mode").toInt());
        m_GridCheckBox->setChecked(
                    _Settings.value("Show Grid").toBool());
        m_NavigationBarCheckBox->setChecked(
                    _Settings.value("Show Navigation Bar").toBool());
        m_HorizontalHeaderComboBox->setCurrentIndex(
                    _Settings.value("Horizontal Header").toInt());
        m_VerticalHeaderComboBox->setCurrentIndex(
                    _Settings.value("Vertival Header").toInt());
        m_WeekdayColorComboBox->setCurrentIndex(
                    _Settings.value("WeekdayColor").toInt());
        m_WeekendColorComboBox->setCurrentIndex(
                    _Settings.value("WeekendColor").toInt());
        m_HeaderTextComboBox->setCurrentIndex(
                    _Settings.value("Header Font").toInt());
        m_FirstFridayInGreenCheckBox->setChecked(
                    _Settings.value("First Friday").toBool());
        m_TongGsBirthdayInYellowCheckBox->setChecked(
                    _Settings.value("Tong G.'s Birthday").toBool());
        /* m_AnalogClock->restoreGeometry(
                    _Settings.value("Clock Geometry").toByteArray());
         Bug */
        }

    /* _ConfigureButtonClicked()槽实现 */
    void _Calendar::_ConfigureButtonClicked()
        {
        if (m_ConfigureButton->text() == QStringLiteral("☞"))
            {
            m_GeneralOptionsGroupBox->setHidden(false);
            m_DatesGroupBox->setHidden(false);
            m_TextFormatsGroupBox->setHidden(false);
            m_ConfigureButton->setText(QStringLiteral("☜"));
            m_ConfigureButton->setToolTip(QStringLiteral("隐藏设置"));

            m_PreviewGroupBox->setTitle(QStringLiteral("预览"));

            /* 隐藏和显示群组架时会出现很操蛋的情况, 不得不出此下策, 见笑 */
            m_GridCheckBox->setChecked(!m_GridCheckBox->isChecked());
            m_GridCheckBox->setChecked(!m_GridCheckBox->isChecked());
            m_NavigationBarCheckBox->setChecked(
                        !m_NavigationBarCheckBox->isChecked());
            m_NavigationBarCheckBox->setChecked(
                        !m_NavigationBarCheckBox->isChecked());
            } else if (m_ConfigureButton->text() == QStringLiteral("☜"))
                {
                m_GeneralOptionsGroupBox->setHidden(true);
                m_DatesGroupBox->setHidden(true);
                m_TextFormatsGroupBox->setHidden(true);
                m_ConfigureButton->setText(QStringLiteral("☞"));
                m_ConfigureButton->setToolTip(QStringLiteral("显示设置"));

                /* 啊哈, 和上边内个原理一样 */
                m_Calendar->setGridVisible(!m_Calendar->isGridVisible());
                m_Calendar->setGridVisible(!m_Calendar->isGridVisible());
                m_Calendar->setNavigationBarVisible(
                            !m_Calendar->isNavigationBarVisible());
                m_Calendar->setNavigationBarVisible(
                            !m_Calendar->isNavigationBarVisible());
                m_PreviewGroupBox->setTitle(QStringLiteral("日历"));
                }
        }

    /* _LocaleChanged()槽实现 */
    void _Calendar::_LocaleChanged(int _Index)
        {
        const QLocale _NewLocale(m_LocaleComboBox->itemData(_Index).toLocale());
        m_Calendar->setLocale(_NewLocale);
        int _NewLocaleFirstDayIndex
                = m_WeekdayColorComboBox->findData(_NewLocale.firstDayOfWeek());
        m_WeekdayColorComboBox->setCurrentIndex(_NewLocaleFirstDayIndex);
        }

    /* _FirstDayChanged()槽实现
     * 在_Calendar::_CreateGeneralOptionGroupBox()函数中调用
     * 与currentIndexChanged()信号连接
     */
    void _Calendar::_FirstDayChanged(int _Index)
        {
        /* 通过QComboBox::itemData()获取UserData并将其转换为Qt::DayOfWeek枚举类型 */
        m_Calendar->setFirstDayOfWeek(
                    Qt::DayOfWeek(
                        m_WeekStartComboBox->itemData(_Index).toInt()));
        }/* Bug已解决: 2013/4/7 19:42
          * _Calendar::_CreateGeneralOptionGroupBox()函数中
          * 与m_WeekStartComboBox()的信号连接错误,
          * m_WeekStartComboBox()误打成m_WeekColorComboBox()
          */

    /* _SelectionModeChanged()槽实现
     * 该槽在_Calendar::_CreateGeneralOpetionGroupBox()函数中调用
     * 与currentIndexChanged()信号连接
     */
    void _Calendar::_SelectionModeChanged(int _Index)
        {
        m_Calendar->setSelectionMode(
                    QCalendarWidget::SelectionMode(
                        m_SelectionModeComboBox->itemData(_Index).toInt()));
        }

    /* _HorizontalHeaderChanged()槽实现
     * 该槽在_Calendar::_CreateGeneralOpetionGroupBox()函数中调用
     * 与currentIndexChanged()信号连接
     */
    void _Calendar::_HorizontalHeaderChanged(int _Index)
        {
        m_Calendar->setHorizontalHeaderFormat(
                    QCalendarWidget::HorizontalHeaderFormat(
                        m_HorizontalHeaderComboBox->itemData(_Index).toInt()));
        }

    /* _VerticalHeaderChanged()槽实现
     * 该槽在_Calendar::_CreateGeneralOpetionGroupBox()函数中调用
     * 与currentIndexChanged()信号连接
     */
    void _Calendar::_VerticalHeaderChanged(int _Index)
        {
        m_Calendar->setVerticalHeaderFormat(
                    QCalendarWidget::VerticalHeaderFormat(
                        m_VerticalHeaderComboBox->itemData(_Index).toInt()));
        }

    /* _SelectedDateChanged()槽实现
     * 该槽在_Calendar::_CreateDatesGroupBox()函数中调用
     * 与selectionChanged()信号连接
     */
    void _Calendar::_SelectedDateChanged()
        {
        m_CurrentDateEdit->setDate(m_Calendar->selectedDate());
        }

    /* _MinimumDateChanged()槽实现
     * 该槽在_Calendar::_CreateDatesGroupBox()函数中调用
     * 与dateChanged()信号连接
     */
    void _Calendar::_MinimumDateChanged(const QDate &_Date)
        {
        m_Calendar->setMinimumDate(_Date);
        m_MaximumDateEdit->setDate(m_Calendar->maximumDate());
        }

    /* _MaximumDateChanged()槽实现
     * 该槽在_Calendar::_CreateDatesGroupBox()函数中调用
     * 与_MinimumDateChanged()信号连接
     */
    void _Calendar::_MaximumDateChanged(const QDate &_Date)
        {
        m_Calendar->setMaximumDate(_Date);
        m_MinimumDateEdit->setDate(m_Calendar->minimumDate());
        }

    /* _WeekdayFormatChanged()槽实现
     * 该槽在_Calendar::_CreateDatesGroupBox()函数中调用
     * 与_MaximumDateChanged()信号连接
     */
    void _Calendar::_WeekdayFormatChanged()
        {
        QTextCharFormat _Format;

        _Format.setForeground(
                    qvariant_cast<QColor>(
                        m_WeekdayColorComboBox->itemData(
                            m_WeekdayColorComboBox->currentIndex())));
        m_Calendar->setWeekdayTextFormat(Qt::Monday, _Format);
        m_Calendar->setWeekdayTextFormat(Qt::Tuesday, _Format);
        m_Calendar->setWeekdayTextFormat(Qt::Wednesday, _Format);
        m_Calendar->setWeekdayTextFormat(Qt::Thursday, _Format);
        m_Calendar->setWeekdayTextFormat(Qt::Friday, _Format);
        }

    /* _WeekendFormatChanged()槽实现
     * 该槽在_Calendar::_CreateTextFormatGroupBox()函数中调用
     * 与currentIndexChanged()信号连接
     */
    void _Calendar::_WeekendFormatChanged()
        {
        QTextCharFormat _Format;

        _Format.setForeground(
                    qvariant_cast<QColor>(
                        m_WeekendColorComboBox->itemData(
                            m_WeekendColorComboBox->currentIndex())));
        m_Calendar->setWeekdayTextFormat(Qt::Saturday, _Format);
        m_Calendar->setWeekdayTextFormat(Qt::Sunday, _Format);
        }

    /* _ReformHeaders()槽实现
     * 该槽在_Calendar::_CreateTextFormatGroupBox()函数中调用
     * 与currentIndexChanged()信号连接
     */
    void _Calendar::_ReformatHeaders()
        {
        QString _Text = m_HeaderTextComboBox->currentText();
        QTextCharFormat _Format;

        if (_Text == QStringLiteral("黑体"))
            _Format.setFontWeight(QFont::Bold);
        else if (_Text == QStringLiteral("斜体"))
            _Format.setFontItalic(true);
        else if (_Text == QStringLiteral("普通"))
            _Format.setFontWeight(QFont::Normal);

        m_Calendar->setHeaderTextFormat(_Format);
        }

    /* _ReformatCalendarPage()槽实现 */
    void _Calendar::_ReformatCalendarPage()
        {
        QTextCharFormat _TongGsBirthdayFormat;
        const QDate _TongGsBirthday(m_Calendar->yearShown(), 1, 28);

        QTextCharFormat _FirstFridayFormat;
        QDate _FirstFriday(
                    m_Calendar->yearShown(), m_Calendar->monthShown(), 1);
        while (_FirstFriday.dayOfWeek() != Qt::Friday)
            _FirstFriday = _FirstFriday.addDays(1);

        if (m_FirstFridayInGreenCheckBox->isChecked())
            /* 如果将"每月第一个星期五标记为绿色"复选框被选中... */
            _FirstFridayFormat.setForeground(Qt::darkGreen);
        else
            {
            Qt::DayOfWeek _DayOfWeek(
                        static_cast<Qt::DayOfWeek>(_FirstFriday.dayOfWeek()));
            _FirstFridayFormat.setForeground(
                        m_Calendar->weekdayTextFormat(_DayOfWeek).foreground());
            }

        m_Calendar->setDateTextFormat(_FirstFriday, _FirstFridayFormat);

        if (m_TongGsBirthdayInYellowCheckBox->isChecked())
            _TongGsBirthdayFormat.setForeground(Qt::darkYellow);
        else if (!m_FirstFridayInGreenCheckBox->isChecked()
                    || _FirstFriday != _TongGsBirthday)
            {
            Qt::DayOfWeek _DayOfWeek(
                        static_cast<Qt::DayOfWeek>(
                            _TongGsBirthday.dayOfWeek()));

            m_Calendar->setDateTextFormat(
                        _TongGsBirthday,
                        m_Calendar->weekdayTextFormat(_DayOfWeek));
            }

        m_Calendar->setDateTextFormat(_TongGsBirthday, _TongGsBirthdayFormat);
        }

    /* _ShowClockButtonClicked()槽实现 */
    void _Calendar::_ShowClockButtonClicked()
        {
        if (!m_AnalogClock)
            {
            m_AnalogClock = new _AnalogClock;
            }

        m_AnalogClock->exec();
        }

    /* _GoToTongGsBirthday()槽实现 */
    void _Calendar::_GoToTongGsBirthday()
        {
        if (m_GoToTongGsBirthday->text() == QStringLiteral("前往桐哥生日"))
            {
            m_Calendar->setSelectedDate(
                        QDate(m_Calendar->yearShown(), 1, 28));
            m_GoToTongGsBirthday->setText(QStringLiteral("回到今天"));
            } else if (m_GoToTongGsBirthday->text() == QStringLiteral("回到今天"))
                {
                _ReturnToday();
                m_GoToTongGsBirthday->setText(QStringLiteral("前往桐哥生日"));
                }
        }

    /* _ReturnToday()槽实现 */
    void _Calendar::_ReturnToday()
        {
        m_Calendar->setSelectedDate(m_Today);
        if (m_GoToTongGsBirthday->text() != QStringLiteral("前往桐哥生日"))
            m_GoToTongGsBirthday->setText(QStringLiteral("前往桐哥生日"));
        }

    /* _AboutButtonClicked()槽实现 */
    void _Calendar::_AboutButtonClicked()
        {
        QMessageBox::about(this, QStringLiteral("About Domino Calendar"),
             QStringLiteral("<h1>Domino Calendar 2013</h1>"

             "<p><h4>Release 1.0</h4>"
             "<p>Copyright &copy; 1997-2013 Domino Inc. & Tong G. "
             "All rights reserved."
             "<p>版权所有 © 1997-2013 Domino Inc. & 郭桐。 保留所有权利。"
             "<p>警告：本计算机程序受著作权法和国际公约的保护，未经授权擅自复制或传播本程序"
             "的部分或全部，可能受到严厉的民事及刑事制裁，并将在法律许可的范围内受到最大"
             "可能的起诉。"
             ));
        }

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
