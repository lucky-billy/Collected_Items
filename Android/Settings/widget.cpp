#include "widget.h"
#include <QHBoxLayout>
#include <QtAndroid>
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include <QLabel>
#include <QDebug>

using namespace QtAndroid;

#define CHECK_EXCEPTION() \
    if(env->ExceptionCheck())\
    {\
        qDebug() << "exception occured";\
        env->ExceptionDescribe();\
        env->ExceptionClear();\
    }

#define RINGER_MODE_NORMAL  2
#define RINGER_MODE_SILENT  0
#define RINGER_MODE_VIBRATE 1

Widget::Widget(QWidget *parent)
    : QWidget(parent), m_lastChecked(false)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    m_vibrate = new QPushButton("Vibrate");
    connect(m_vibrate, SIGNAL(clicked()), this, SLOT(onVibrate()));
    layout->addWidget(m_vibrate);
    layout->addSpacing(12);

    m_screenOrient = new QPushButton("ScreenOrientation");
    layout->addWidget(m_screenOrient);
    connect(m_screenOrient, SIGNAL(clicked()), this, SLOT(onScreenOrientation()));
    layout->addSpacing(12);

    initRingerMode(layout);

    m_screenOn = new QCheckBox("Keep Screen On");
    connect(m_screenOn, SIGNAL(clicked(bool)), this, SLOT(onScreenOnChecked(bool)));
    layout->addWidget(m_screenOn);
    layout->addSpacing(12);

    QHBoxLayout *rowLayout = new QHBoxLayout();
    layout->addLayout(rowLayout);
    rowLayout->addWidget(new QLabel("Brightness(App)"));
    m_screenBrightness = new QSlider(Qt::Horizontal);
    rowLayout->addWidget(m_screenBrightness, 1);
    m_screenBrightness->setMaximum(0);
    m_screenBrightness->setMaximum(255);
    m_screenBrightness->setValue(getCurrentBrightness());
    connect(m_screenBrightness, SIGNAL(valueChanged(int)), this, SLOT(onBrightnessChanged(int)));

    layout->addStretch(1);
}

Widget::~Widget()
{

}
//
// android.os.Vibrator
//
void Widget::onVibrate()
{
    QAndroidJniEnvironment env;
    QAndroidJniObject activity = androidActivity();
    QAndroidJniObject name = QAndroidJniObject::getStaticObjectField(
                "android/content/Context",
                "VIBRATOR_SERVICE",
                "Ljava/lang/String;"
                );
    CHECK_EXCEPTION();
    QAndroidJniObject vibrateService = activity.callObjectMethod(
                "getSystemService",
                "(Ljava/lang/String;)Ljava/lang/Object;",
                name.object<jstring>());
    CHECK_EXCEPTION();
    jlong duration = 200;
    vibrateService.callMethod<void>("vibrate", "(J)V", duration);
    CHECK_EXCEPTION();
}

// 有两种方法。
// 1.
//   getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
//   必须放在 Actvity 的 setContentView 调用前…… Qt 中很难操作
// 2.通过 PowerManager 来实现：
// android/os/PowerManager
//    WakeLock newWakeLock(int , String tag);
// android.os.PowerManager.WakeLock
//      void acquire();
//      void release();
//
void Widget::onScreenOnChecked(bool checked)
{
    QAndroidJniEnvironment env;
    QAndroidJniObject activity = androidActivity();

    if(m_lastChecked)
    {
        if(m_wakeLock.isValid())
        {
            m_wakeLock.callMethod<void>("release");
            CHECK_EXCEPTION();
        }
        m_lastChecked = false;
        return;
    }

    QAndroidJniObject name = QAndroidJniObject::getStaticObjectField(
                "android/content/Context",
                "POWER_SERVICE",
                "Ljava/lang/String;"
                );
    CHECK_EXCEPTION();
    QAndroidJniObject powerService = activity.callObjectMethod(
                "getSystemService",
                "(Ljava/lang/String;)Ljava/lang/Object;",
                name.object<jstring>());
    CHECK_EXCEPTION();
    QAndroidJniObject tag = QAndroidJniObject::fromString("QtJniWakeLock");
    // 10 == SCREEN_BRIGHT_WAKE_LOCK
    //  6 == SCREEN_DIM_WAKE_LOCK
    // getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
    //  before setContentView() was called.
    m_wakeLock = powerService.callObjectMethod(
                "newWakeLock",
                "(ILjava/lang/String;)Landroid/os/PowerManager$WakeLock;",
                10,
                tag.object<jstring>()
                );
    CHECK_EXCEPTION();
    qDebug() << "wakeLock isValid() - " << m_wakeLock.isValid();
    if(m_wakeLock.isValid())
    {
        m_wakeLock.callMethod<void>("acquire");
        CHECK_EXCEPTION();
    }
    m_lastChecked = true;
}
//
// 方法1（改变当前Activity的亮度）：
//      WindowManager.LayoutParams lp = window.getAttributes();
//      lp.screenBrightness = brightness;
//      window.setAttributes(lp);
//    必须在 Android Activity 的 UI 线程中调用， Qt主线程与Java Activity不是一个线程，报错
// 方法2（改变系统设置）：
// android/provider/Settings.System
//   static boolean putInt(ContentResolver, String key, int value);
//   static int getInt(ContentResolver, String key);
//
//  SCREEN_BRIGHTNESS = "screen_brightness"
//      brightness: 0--255
//  SCREEN_BRIGHTNESS_MODE == "screen_brightness_mode"
//  SCREEN_BRIGHTNESS_MODE_MANUAL == 0
//  SCREEN_BRIGHTNESS_MODE_AUTOMATIC == 1
// 会持久生效……

void Widget::onBrightnessChanged(int value)
{
    QAndroidJniEnvironment env;
    QAndroidJniObject activity = androidActivity();
    QAndroidJniObject contentResolver = activity.callObjectMethod(
                "getContentResolver",
                "()Landroid/content/ContentResolver;"
                );
    CHECK_EXCEPTION();

    //set brightness mode to MANUAL
    QAndroidJniObject brightnessTag = QAndroidJniObject::fromString("screen_brightness");
    QAndroidJniObject brightnessModeTag = QAndroidJniObject::fromString("screen_brightness_mode");
    bool ok = QAndroidJniObject::callStaticMethod<jboolean>(
                "android/provider/Settings$System",
                "putInt",
                "(Landroid/content/ContentResolver;Ljava/lang/String;I)Z",
                contentResolver.object<jobject>(),
                brightnessModeTag.object<jstring>(),
                0
                );
    CHECK_EXCEPTION();
    qDebug() << "set brightness mode to MANUAL - " << ok;

    //set brightness to value
    ok = QAndroidJniObject::callStaticMethod<jboolean>(
                "android/provider/Settings$System",
                "putInt",
                "(Landroid/content/ContentResolver;Ljava/lang/String;I)Z",
                contentResolver.object<jobject>(),
                brightnessTag.object<jstring>(),
                value
                );
    CHECK_EXCEPTION();
    qDebug() << "set brightness to " << value << " result - " << ok;

#if 0
    QAndroidJniObject window = activity.callObjectMethod(
                "getWindow",
                "()Landroid/view/Window;"
                );
    CHECK_EXCEPTION();
    /*
    QAndroidJniObject *runnable = new QAndroidJniObject(
                "an/qt/Settings/BrightnessRunnable",
                "(Landroid/view/Window;I)V",
                window.object<jobject>(),
                value);
                */
    QAndroidJniObject *runnable = new QAndroidJniObject(
                "an/qt/Settings/BrightnessRunnable",
                "(Landroid/app/Activity;I)V",
                activity.object<jobject>(),
                value);
    CHECK_EXCEPTION();
    qDebug() << "runnable isValid() - " << runnable->isValid();
    activity.callObjectMethod(
                "runOnUiThread",
                "(Ljava/lang/Runnable;)V",
                runnable->object<jobject>()
                );
    CHECK_EXCEPTION();
    qDebug() << "postRunnable to ui thread";
    /*
    qDebug() << "adjust brightness, window isValid() - " << window.isValid();
    QAndroidJniObject attrs = window.callObjectMethod(
                "getAttributes",
                "()Landroid/view/WindowManager$LayoutParams;"
                );
    CHECK_EXCEPTION();
    qDebug() << "adjust brightness, attrs isValid() - " << attrs.isValid();
    attrs.setField<jfloat>("screenBrightness", brightness);
    CHECK_EXCEPTION();
    qDebug() << "adjust brightness, setField";
    window.callObjectMethod(
                "setAttributes",
                "(Landroid/view/WindowManager$LayoutParams;)V",
                attrs.object<jobject>()
                );
    CHECK_EXCEPTION();
    qDebug() << "adjust brightness, setAttributes";
    */
#endif
}
// android/content/pm/ActivityInfo这个类定义了屏幕方向相关的静态常量，
// 比如 SCREEN_ORIENTATION_LANDSCAPE 是 0， SCREEN_ORIENTATION_PORTRAIT 是 1
//
// Activity 有下面的方法：
//int getRequestedOrientation()
//void setRequestedOrientation(int requestedOrientation)
//
void Widget::onScreenOrientation()
{
    QAndroidJniEnvironment env;
    QAndroidJniObject activity = androidActivity();
    jint orient = activity.callMethod<jint>(
                "getRequestedOrientation"
                );
    CHECK_EXCEPTION();
    if(orient == 1)
    {
        orient = 0;
    }
    else
    {
        orient = 1;
    }
    activity.callMethod<void>(
                "setRequestedOrientation",
                "(I)V", orient);
    CHECK_EXCEPTION();
}

// Context.AUDIO_SERVICE
// AudioManager
//   int getRingerMode()
//   void setRingerMode(int)
//
void Widget::onRingerModeClicked(int mode)
{
    QAndroidJniEnvironment env;
    QAndroidJniObject activity = androidActivity();
    QAndroidJniObject name = QAndroidJniObject::getStaticObjectField(
                "android/content/Context",
                "AUDIO_SERVICE",
                "Ljava/lang/String;"
                );
    CHECK_EXCEPTION();
    QAndroidJniObject audioService = activity.callObjectMethod(
                "getSystemService",
                "(Ljava/lang/String;)Ljava/lang/Object;",
                name.object<jstring>());
    CHECK_EXCEPTION();

    audioService.callMethod<void>(
                "setRingerMode", "(I)V", mode
                );
    CHECK_EXCEPTION();
}

void Widget::initRingerMode(QVBoxLayout *layout)
{
    QAndroidJniEnvironment env;
    QAndroidJniObject activity = androidActivity();
    QAndroidJniObject name = QAndroidJniObject::getStaticObjectField(
                "android/content/Context",
                "AUDIO_SERVICE",
                "Ljava/lang/String;"
                );
    CHECK_EXCEPTION();
    QAndroidJniObject audioService = activity.callObjectMethod(
                "getSystemService",
                "(Ljava/lang/String;)Ljava/lang/Object;",
                name.object<jstring>());
    CHECK_EXCEPTION();

    int mode = audioService.callMethod<jint>(
                "getRingerMode",
                "()I"
                );
    CHECK_EXCEPTION();
    layout->addWidget(new QLabel("Ringer Mode:"));
    QHBoxLayout *rowLayout = new QHBoxLayout();
    layout->addLayout(rowLayout);
    rowLayout->addSpacing(30);
    m_ringerModeGroup = new QButtonGroup(this);
    QRadioButton *normal = new QRadioButton("Normal");
    m_ringerModeGroup->addButton(normal, RINGER_MODE_NORMAL);
    rowLayout->addWidget(normal);
    QRadioButton *silent = new QRadioButton("Silent");
    m_ringerModeGroup->addButton(silent, RINGER_MODE_SILENT);
    rowLayout->addWidget(silent);
    QRadioButton *vibrate = new QRadioButton("Vibrate");
    m_ringerModeGroup->addButton(vibrate, RINGER_MODE_VIBRATE);
    rowLayout->addWidget(vibrate);

    switch(mode)
    {
    case RINGER_MODE_NORMAL:
        normal->setChecked(true);
        break;
    case RINGER_MODE_SILENT:
        silent->setChecked(true);
        break;
    case RINGER_MODE_VIBRATE:
        vibrate->setChecked(true);
        break;
    }

    connect(m_ringerModeGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(onRingerModeClicked(int)));
}

int Widget::getCurrentBrightness()
{
    QAndroidJniEnvironment env;
    QAndroidJniObject activity = androidActivity();
    QAndroidJniObject contentResolver = activity.callObjectMethod(
                "getContentResolver",
                "()Landroid/content/ContentResolver;"
                );
    CHECK_EXCEPTION();

    QAndroidJniObject name = QAndroidJniObject::getStaticObjectField(
                "android/provider/Settings$System",
                "SCREEN_BRIGHTNESS",
                "Ljava/lang/String;"
                );
    CHECK_EXCEPTION();

    int brightness = QAndroidJniObject::callStaticMethod<jint>(
                "android/provider/Settings$System",
                "getInt",
                "(Landroid/content/ContentResolver;Ljava/lang/String;)I",
                contentResolver.object<jobject>(),
                name.object<jstring>()
                );
    CHECK_EXCEPTION();
    qDebug() << "current brightness - " << brightness;
    return brightness;
}
