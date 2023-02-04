## 这是星火助手的文档

部分功能采用dtk，如按钮，背景高斯模糊

### 系统更新功能

在Mainwindow中updateUpdateButton();实现按钮显示


 connect(sysUpdateButton, SIGNAL(clicked()), this, SLOT(on_sysUpdateButton_clicked()));来监听按钮


MainWindow::on_sysUpdateButton_clicked()来实现系统更新功能



### 天气更新功能

> （该功能的部分代码实现来源大轮明王讲qt：https://www.bilibili.com/video/BV1D841147zt/?spm_id_from=333.788）

weatherparse.h为天气解析类

WeatherParse::WeatherParse()中含有一个map表存放天气情况和天气图标

WeatherParse::requestServer()用于请求ip地址服务器和天气服务器，如果reply为http://ip-api.com/json，则执行ipCityParseJson(byteArray)来请求ip地址，否则执行weatherParseJson(byteArray);来获取天气
