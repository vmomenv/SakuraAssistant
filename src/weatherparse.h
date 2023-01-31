#ifndef WEATHERPARSE_H
#define WEATHERPARSE_H

#include <QObject>
#include <QWidget>
#include<QNetworkAccessManager>
#include<QMap>
#include<QUrl>
#include<QNetworkAccessManager>
#include<QNetworkReply>
#include<QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
class WeatherParse : public QObject
{
    Q_OBJECT
public:
    explicit WeatherParse(QObject *parent = nullptr);
    QMap<QString,QString>mTypeMap;//天气类型map表
    QNetworkAccessManager* mNetAccessManager;
    void requestServer(QNetworkReply *reply);//与天气和ip服务商通信
    void weatherParseJson(QByteArray &byteArray);//天气服务json
    void ipCityParseJson(QByteArray &byteArray);//ip转城市服务json


private:
    QString CityName;//所在城市city




signals:
public slots:


};

#endif // WEATHERPARSE_H
