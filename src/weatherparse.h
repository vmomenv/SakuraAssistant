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
#include <QTextCodec>
#include <QRegularExpression>
class WeatherParse : public QObject
{
    Q_OBJECT
public:
    explicit WeatherParse(QObject *parent = nullptr);
    QMap<QString,QString>mTypeMap;//天气类型map表
    QNetworkAccessManager* mNetAccessManager;
    void requestServer(QNetworkReply *reply);//与天气和ip服务商通信
    void weatherParseJson(QByteArray &byteArray);//天气服务json
    void ipCityParse();//ip转城市
    QString cityName;//所在城市city
    QString cityTemperature;
    QString weatherType;


private:




signals:
    void update();
public slots:


};

#endif // WEATHERPARSE_H
