#ifndef WEATHERPARSE_H
#define WEATHERPARSE_H

#include <QObject>
#include <QWidget>
#include<QNetworkAccessManager>
#include<QMap>
#include<QUrl>
#include<QNetworkAccessManager>
#include<QNetworkReply>
class WeatherParse : public QObject
{
    Q_OBJECT
public:
    explicit WeatherParse(QObject *parent = nullptr);
    QMap<QString,QString>mTypeMap;//天气类型map表
    QNetworkAccessManager* mNetAccessManager;
    void requestWeatherData(QNetworkReply *reply);//与天气服务商通信

signals:
public slots:


};

#endif // WEATHERPARSE_H
