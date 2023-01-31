#include "iptoweathercity.h"

IpToWeatherCity::IpToWeatherCity(QObject *parent) : QObject(parent)
{

}

QString IpToWeatherCity::getCityCode(QString CityName)
{

    if(CityName.isEmpty()){
        initCityMap();
    }

}

void IpToWeatherCity::initCityMap()
{


//        QMap<QString,QString> *cityMap=new QMap<QString,QString>;
    qDebug()<<1;
    //1.读取文件
    QFile file(":/res/citycode.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file: " << file.errorString();
        return;
    }
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray json = file.readAll();
    file.close();

    //2.解析并写入map
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(json,&err);
    if(err.error != QJsonParseError::NoError){
        return;
    }
    if(!doc.isArray()){//如果不是数组
        return;
    }
    QJsonArray cities = doc.array();

    //导入map表
    for(int i=0;i<cities.size();i++){
        QString ENcity_name = cities[i].toObject().value("ENcity_name").toString();
        QString cityCode =cities[i].toObject().value("city_code").toString();
        if(cityCode.size()>0)
        {
            cityMap.insert(ENcity_name,cityCode);//导入map表
        }
    }
}
