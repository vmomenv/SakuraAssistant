#ifndef WEATHERPARSE_H
#define WEATHERPARSE_H

#include <QWidget>
#include<QNetworkAccessManager>
#include<QMap>
class WeatherParse:public QWidget
{
public:
    WeatherParse();
    void requestWeatherData(QNetworkReply *reply);//与天气服务商通信

private:
    QMap<QString,QString>mTypeMap;//天气类型map表
    QNetworkAccessManager* mNetAccessManager;

};

#endif // WEATHERPARSE_H
