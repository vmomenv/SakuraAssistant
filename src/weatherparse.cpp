#include "weatherparse.h"
#include<QMessageBox>
#include<mainwindow.h>


WeatherParse::WeatherParse(QObject *parent) : QObject(parent)
{

    qDebug()<<"weatherparse";
    //在map中导入键值
    mTypeMap.insert("暴雪",":/res/type/BaoXue.png");
    mTypeMap.insert("暴雨",":/res/type/BaoYu. png");
    mTypeMap.insert("暴雨到大暴雨",":/res/type/BaoYuDaoDaBaoYu.png");
    mTypeMap.insert("大暴雨",":/res/type/DaBaoYu.png");
    mTypeMap.insert("大暴雨到特大暴雨",":/res/type/DaBaoYuDaoTeDaBaoYu.png");
    mTypeMap.insert("大到暴雪",":/res/type/DaDaoBaoXue.png");
    mTypeMap.insert("大雪",":/res/type/DaXue.png");
    mTypeMap.insert("大雨",":/res/type/DaYu.png");
    mTypeMap.insert("冻雨",":/res/type/DongYu.png");
    mTypeMap.insert("多云",":/res/type/DuoYun.png");
    mTypeMap.insert("浮沉",":/res/type/FuChen.png");
    mTypeMap.insert("雷阵雨",":/res/type/LeiZhenYu.png");
    mTypeMap.insert("雷阵雨伴有冰雹",":/res/type/LeiZhenYuBanYouBingBao.png");
    mTypeMap.insert("霾",":/res/type/Mai.png");
    mTypeMap.insert("强沙尘暴",":/res/type/QiangShaChenBao.png");
    mTypeMap.insert("晴",":/res/type/Qing.png");
    mTypeMap.insert("沙尘暴",":/res/type/ShaChenBao.png");
    mTypeMap.insert("特大暴雨",":/res/type/TeDaBaoYu.png");
    mTypeMap.insert("我也不知道啥天气",":/res/type/undefined.png");
    mTypeMap.insert("雾",":/res/type/Wu.png");
    mTypeMap.insert("小到中雪",":/res/type/XiaoDaoZhongXue.png");
    mTypeMap.insert("小到中雨",":/res/type/XiaoDaoZhongYu.png");
    mTypeMap.insert("小雪",":/res/type/XiaoXue.png");
    mTypeMap.insert("小雨",":/res/type/XiaoYu.png");
    mTypeMap.insert("雪",":/res/type/Xue.png");
    mTypeMap.insert("扬沙",":/res/type/YangSha.png");
    mTypeMap.insert("阴",":/res/type/Yin.png");
    mTypeMap.insert("雨",":/res/type/Yu.png");
    mTypeMap.insert("雨夹雪",":/res/type/YuJiaXue.png");
    mTypeMap.insert("阵雪",":/res/type/ZhenXue.png");
    mTypeMap.insert("阵雨",":/res/type/ZhenYu.png");
    mTypeMap.insert("中到大雪",":/res/type/ZhongDaoDaXue.png");
    mTypeMap.insert("中到大雨",":/res/type/ZhongDaoDaYu.png");
    mTypeMap.insert("中雪",":/res/type/ZhongXue.png");
    mTypeMap.insert("中雨",":/res/type/ZhongYu.png");

    mNetAccessManager = new QNetworkAccessManager(this);//实现qt网络模块

//    connect(mNetAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestWeatherData(QNetworkReply*)));
    connect(mNetAccessManager,&QNetworkAccessManager::finished,this,&WeatherParse::requestServer);
    QUrl url("http://ip-api.com/json");
    mNetAccessManager->get(QNetworkRequest(url));
}



void WeatherParse::requestServer(QNetworkReply *reply){
    qDebug()<<"请求服务商数据";
    int serverCode =reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug()<<"operation reply:"<<reply->operation();
    qDebug()<<"status code:"<<serverCode;
    qDebug()<<"url:"<<reply->url();
    qDebug()<<"raw header"<<reply->rawHeaderList();//响应头
    if(reply->error()!=QNetworkReply::NoError||serverCode!=200){
        qDebug()<<reply->errorString().toLatin1().data();
        qDebug()<<"请求失败，请检查网络";

    }else{
        QByteArray byteArray=reply->readAll();
        qDebug()<<"请求成功！"<<byteArray.data();
        if (reply->url() == QUrl("http://ip-api.com/json")) {
            ipCityParseJson(byteArray);
        }
        else{
            weatherParseJson(byteArray);
        }
    }
    reply->deleteLater();

}

void WeatherParse::weatherParseJson(QByteArray &byteArray)
{
    QJsonParseError err;
    QJsonDocument doc=QJsonDocument::fromJson(byteArray,&err);
    if(err.error!=QJsonParseError::NoError){
        qDebug()<<"解析失败";
    }
    QJsonObject rootObj=doc.object();
    qDebug()<<rootObj.value("message").toString();

}

void WeatherParse::ipCityParseJson(QByteArray &byteArray)
{
    QJsonParseError err;
    QJsonDocument doc=QJsonDocument::fromJson(byteArray,&err);
    if(err.error!=QJsonParseError::NoError){
        qDebug()<<"解析失败";
    }
        QJsonObject rootObj=doc.object();
        CityName=rootObj.value("city").toString();
        qDebug()<<CityName;
        getCityCode(CityName);
//        QUrl url("http://t.weather.itboy.net/api/weather/city/101010100");
        //        mNetAccessManager->get(QNetworkRequest(url));
}


QString WeatherParse::getCityCode(QString CityName){

//    if(CityName.isEmpty()){
        initCityMap();
//    }
}
//初始化城市map表
void WeatherParse::initCityMap(){
    qDebug()<<1;
    //1.读取文件
    QFile file(":/res/citycode.json");
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



