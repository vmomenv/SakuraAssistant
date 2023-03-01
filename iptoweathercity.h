#ifndef IPTOWEATHERCITY_H
#define IPTOWEATHERCITY_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include<QFile>
#include<QMap>
class IpToWeatherCity : public QObject
{
    Q_OBJECT
public:
    explicit IpToWeatherCity(QObject *parent = nullptr);
    QString getCityCode(QString CityName);
    void initCityMap();
    QMap<QString,QString>cityMap;//城市对照表citycode.json

signals:

public slots:
};

#endif // IPTOWEATHERCITY_H
