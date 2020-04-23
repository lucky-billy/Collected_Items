#ifndef RTSPTHREAD_H
#define RTSPTHREAD_H

#include <QThread>
#include "qffmpeg.h"

class RtspThread : public QThread
{
    Q_OBJECT
public:
    explicit RtspThread(QObject *parent = 0);

    void run();    
    void setffmpeg(QFFmpeg *f){ffmpeg=f;}

private:    
    QFFmpeg * ffmpeg;

signals:

public slots:

};

#endif // RTSPTHREAD_H
