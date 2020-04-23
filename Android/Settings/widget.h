#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QSlider>
#include <QAndroidJniObject>
#include <QRadioButton>
#include <QButtonGroup>
#include <QVBoxLayout>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected slots:
    void onVibrate();
    void onScreenOnChecked(bool checked);
    void onBrightnessChanged(int value);
    void onScreenOrientation();
    void onRingerModeClicked(int mode);

private:
    void initRingerMode(QVBoxLayout *layout);
    int getCurrentBrightness();

private:
    QCheckBox *m_screenOn;
    QSlider *m_volume;
    QString m_ringstone;
    QPushButton *m_playPause;
    QSlider *m_screenBrightness;
    QPushButton *m_vibrate;
    QAndroidJniObject m_wakeLock;
    bool m_lastChecked;
    QPushButton *m_screenOrient;
    QButtonGroup *m_ringerModeGroup;
};

#endif // WIDGET_H
