#include "getdata.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QUrl>
#include <QtDebug>
#include <QQmlPropertyMap>
#include <QList>

#include <QSqlQuery>

Getdata::Getdata():QObject()
{

    manager = new QNetworkAccessManager();
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

    //添加暴露给qml的数据
    ownerData.insert("hello", QString("你好"));
    ownerData.insert("city", city);//天气的所在城市
    ownerData.insert("date_y", date_y);//date_y是日期

}

void Getdata::achivedata(QString id)
{

    QUrl url(QString("http://m.weather.com.cn/data/").append(id).append(".html"));

    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::UserAgentHeader,
                      "Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.1; Trident/6.0)");
    manager->get(request);

}

void Getdata::replyFinished(QNetworkReply* replay)
{
    if(replay->error() == QNetworkReply::NoError)
    {
        QByteArray data = replay->readAll();
        QJsonParseError jsonError;
        QJsonDocument json = QJsonDocument::fromJson(data, &jsonError);
        if(jsonError.error == QJsonParseError::NoError)
        {
            if(json.isObject())
            {
                QJsonObject obj = json.object();
                if(obj.contains("weatherinfo"))
                {
                    if(obj["weatherinfo"].isObject())
                    {
                        QJsonObject weatherObj = obj["weatherinfo"].toObject();
                        weatherMap = weatherObj.toVariantMap();
                        //qDebug() << weatherMap;
                        Separatedata();//分离数据
                        return;
                    }
                }
            }
        }
    }
}


void Getdata::Separatedata()//分离数据
{
    //今天
     city=weatherMap.value("city").toString();
     date_y=weatherMap.value("date_y").toString();
     week=weatherMap.value("week").toString();
     temp1=weatherMap.value("temp1").toString();
     weather1=weatherMap.value("weather1").toString();
     wind1=weatherMap.value("wind1").toString();
     index_d=weatherMap.value("index_d").toString();
     //明天
     temp2=weatherMap.value("temp2").toString();
     weather2=weatherMap.value("weather2").toString();
     wind2=weatherMap.value("wind2").toString();


     //今天
      qDebug()<<city;
      qDebug()<<date_y;
      qDebug()<<week;
      qDebug()<<temp1;
      qDebug()<<weather1;
      qDebug()<<wind1;
      qDebug()<<index_d;
      //明天
      qDebug()<<temp2;
      qDebug()<<weather2;
      qDebug()<<wind2;

      emit updateData();//发送信号

}

QString Getdata::re_city()
{
    return city;
}

QString Getdata::re_date_y()
{
    return date_y;
}

QString Getdata::re_week()
{
    return week;
}

QString Getdata::re_temp1()
{
    return temp1;
}

QString Getdata::re_weather1()
{
    return weather1;
}

QString Getdata::re_wind1()
{
    return wind1;
}

QString Getdata::re_index_d()
{
    return index_d;
}

QString Getdata::re_temp2()
{
    return temp2;
}

QString Getdata::re_weather2()
{
    return weather2;
}

QString Getdata::re_wind2()
{
    return wind2;
}

QString Getdata::findCityID(QString text)
{
    QSqlQuery query;

    query.prepare("SELECT city_num FROM cities WHERE name = ?;");
    query.addBindValue(text);
    query.exec();
    while(query.next())
    {
        currentProvinceID = query.value(0).toString();
        break;
    }
    query.clear();
    query.prepare("SELECT country_num FROM countries WHERE name = ?;");
    query.addBindValue(text);
    query.exec();
    while(query.next())
    {
        currentProvinceID = query.value(0).toString();
        break;
    }

    if(!currentProvinceID.isEmpty())
    {
      //qDebug()<<currentProvinceID;
      return currentProvinceID;
    }

    return "101010100";


}
