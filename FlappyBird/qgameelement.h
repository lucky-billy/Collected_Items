#ifndef QGAMEELEMENT_H
#define QGAMEELEMENT_H

#include <QObject>
#include <QVector>
#include <QPainter>
#include <QPixmap>

class QGameElement : public QObject
{
    Q_OBJECT
public:
    explicit QGameElement(QObject *parent = 0);
    virtual void draw(QPainter *)=0;//绘制函数
    virtual void logic()=0;//逻辑函数,控制游戏元素的移动和显示的图片
    void setRect(QRectF);//设置矩形区域
    void setRect(qreal,qreal,qreal,qreal);
    QRectF& getBindRect();//获取矩形区域
    void addFrame(QPixmap);//添加帧
    void clearAllFrame();//清理所有帧
    void setEnabled(bool);//设置元素可用状态
protected:
    QRectF bindRect;//绑定的矩形区域
    int frameCount;//帧的总数
    QVector<QPixmap> pixmapList;//帧图像列表
    bool enabled;//是否可用
signals:

public slots:

};

#endif // QGAMEELEMENT_H
