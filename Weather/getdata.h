#ifndef GETDATA_H
#define GETDATA_H
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include <QMap>
#include <QQmlPropertyMap>
#include <QList>
class Getdata:public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString city_data READ re_city  NOTIFY updateData)//让访问city_data时执行re_city函数，返回城市名
    Q_PROPERTY(QString data_y_data READ re_date_y  NOTIFY updateData)
    Q_PROPERTY(QString week_data READ re_week  NOTIFY updateData)
    Q_PROPERTY(QString temp1_data READ re_temp1  NOTIFY updateData)
    Q_PROPERTY(QString weather1_data READ re_weather1  NOTIFY updateData)
    Q_PROPERTY(QString wind1_data READ re_wind1  NOTIFY updateData)
    Q_PROPERTY(QString index_d_data READ re_index_d  NOTIFY updateData)
    Q_PROPERTY(QString temp2_data  READ re_temp2  NOTIFY updateData)
    Q_PROPERTY(QString weather2_data READ re_weather2  NOTIFY updateData)
    Q_PROPERTY(QString wind2_data READ re_wind2  NOTIFY updateData)


public:
    explicit Getdata();


    Q_INVOKABLE void Separatedata();
    Q_INVOKABLE QString re_city();//return city name
    Q_INVOKABLE QString re_date_y();
    Q_INVOKABLE QString re_week();
    Q_INVOKABLE QString re_temp1();
    Q_INVOKABLE QString re_weather1();
    Q_INVOKABLE QString re_wind1();
    Q_INVOKABLE QString re_index_d();
    Q_INVOKABLE QString re_temp2();
    Q_INVOKABLE QString re_weather2();
    Q_INVOKABLE QString re_wind2();

    Q_INVOKABLE QString findCityID(QString text);


    //今天的天气信息
    QString city;//城市
    QString date_y;//日期
    QString week;//星期
    QString temp1;//温度
    QString weather1;//天气
    QString wind1;//风
    QString index_d;//建议
    //第二天天气
    QString temp2;//温度
    QString weather2;//天气
    QString wind2;//风

    QString currentProvinceID;//城市ID

    QQmlPropertyMap ownerData;
signals:
    void updateData(void);
public slots:
    void replyFinished(QNetworkReply*);
    void achivedata(QString id);


private:
    QNetworkAccessManager *manager;
    QVariantMap weatherMap;

};


#endif // GETDATA_H
