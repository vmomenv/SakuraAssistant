## 星火小助手开发文档
>github链接
>https://github.com/vmomenv/SparkAssistant

### 一、开发前的准备
#### 1.1具有dtk环境的系统
##### 方法1，安装deepin操作系统
##### 方法2，[在您的系统中编译与安装DTK](http://www.momen.world:8880/post/46)

#### 1.2构建依赖

1. Qt >= 5.6
2. debhelper (>= 9)
3. pkg-config
4. dpkg-dev
5. qt5-qmake
6. qtbase5-dev
7. qttools5-dev-tools

#### 1.3从源码上构建星火小助手
```bash
cd SparkAssistant
mkdir build
cd build
qmake qmake ../src/sparkassistant/sparkassistant.pro
make
```
### 二、开发星火小助手
#### 1.1开发之前，你可以先运行一个dtk demo来[熟悉dtk开发](http://www.momen.world:8880/post/63)。
#### 1.2查看dtk相关文档
deepin官方提供了三份dtk文档
[传送门 - DTK 文档与资源 | dtk (linuxdeepin.github.io)](https://linuxdeepin.github.io/dtk/README.zh_CN)

https://linuxdeepin.github.io/dtk-docs/index.html

https://deepin-opensource.gitee.io/dtk/class_dtk_1_1_widget_1_1_d_application.html


#### 1.3上手开发
##### 1.3.1概述
本项目主要功能有，系统中软件更新，天气预报，todo清单

##### 1.3.2 设计图
![image.png](http://www.momen.world:8880/static/img/4fa411c13b6db9f7370cef999e22d953.image.png)
##### 1.3.3功能设计
###### 窗口高斯模糊
本应用采用了dtk的高斯模糊功能
在mainwindow的构造函数中，我们先让背景透明，再设置高斯模糊
```c++
setAttribute(Qt::WA_TranslucentBackground);//设置背景透明
setEnableBlurWindow(true);//设置高斯模糊
```
[关于高斯模糊的dtk文档](https://linuxdeepin.github.io/dtkwidget/classDtk_1_1Widget_1_1DMainWindow.html#a4e932450731898588cfa47379936cfb7)
###### 点击窗口外区域关闭窗口
当我们想在之后的开发中暂时关闭此功能，你可以设置`isUpdating=true`来关闭该功能。
```c++
bool MainWindow::eventFilter(QObject *watched, QEvent *event)//感谢柚柚帮我解决了这个麻烦
/*如果发生ActivationChange事件，则检查当前活动窗口是否为此窗口。
 * 如果不是，则关闭该窗口。在这种情况下，如果isUpdating标志为false，则关闭该窗口。*/
{
    if (Q_NULLPTR == watched)
    {
        return false;
    }
    if (QEvent::ActivationChange == event->type())
    {
        if (QApplication::activeWindow() != this)
        {
            if(isUpdating==false){
                this->close();
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}
```

###### 软件更新功能

定义在MainWindow中
```c++
void MainWindow::updateUpdateButton(){
    sysUpdateButton =new DPushButton(this);
    sysUpdateButton->resize(97,34);
    sysUpdateButton->move(10,322);
    sysUpdateButton->setText("系统更新");
//    system("sudo apt update");
    appUpdateButton=new DPushButton(this);
    appUpdateButton->resize(97,34);
    appUpdateButton->move(115,322);
    appUpdateButton->setText("我是凑数的");
}
```
在构造函数中，监听按钮按下信号
```c++
connect(sysUpdateButton, SIGNAL(clicked()), this, SLOT(on_sysUpdateButton_clicked()));
```
当点击时执行
```c++
void MainWindow::on_sysUpdateButton_clicked()
```
MainWindow::on_sysUpdateButton_clicked()中
创建了系统更新进程，主要通过调用bash然后执行系统更新指令来完成更新
例:
```c++
processUpdate.start("bash", QStringList() << "-c" << "pkexec apt update");
```
`pkexec apt update`可以调用系统认证来进行更新
将系统更新信息放入updateResult中
```c++
    QString updateResult;
    updateResult = processList.readAllStandardOutput();
```


###### 天气预报功能

> （该功能的部分代码实现来源大轮明王讲qt：https://www.bilibili.com/video/BV1D841147zt/?spm_id_from=333.788）
weatherparse.h为天气解析类

WeatherParse::WeatherParse()中含有一个map表存放天气情况和天气图标
```c++
WeatherParse::WeatherParse(QObject *parent) : QObject(parent)
{

    qDebug()<<"weatherparse";
    //在map中导入键值
    mTypeMap.insert("暴雪",":/res/type/BaoXue.png");
    mTypeMap.insert("暴雨",":/res/type/BaoYu. png");
    mTypeMap.insert("暴雨到大暴雨",":/res/type/BaoYuDaoDaBaoYu.png");
    mTypeMap.insert("大暴雨",":/res/type/DaBaoYu.png");
    
    ···
```

连接信号槽，然后执行ipCityParse()
```c++
connect(mNetAccessManager,&QNetworkAccessManager::finished,this,&WeatherParse::requestServer);
ipCityParse();
```
在WeatherParse::ipCityParse()中有
```c++
mNetAccessManager->get(QNetworkRequest(url));
```
可以触发信号槽。
然后执行WeatherParse::requestServer()与天气服务商通信


在WeatherParse::requestServer(QNetworkReply *reply)中，若请求成功执行天气解析函数并传入天气情况相关数组
`weatherParseJson(byteArray);`
解析完成后，发送update()信号
该信号在MainWindow::MainWindow(DWidget *parent)中定义
```c++
    connect(weather,&WeatherParse::update,[=](){
            weatherCity->setText(weather->cityName);
            weatherTemperature->setText(weather->cityTemperature+"℃");
            QPixmap *cityPix=new QPixmap(weather->weatherType);
            weatherPic->setPixmap(*cityPix);
            weather->deleteLater();
    });
```

###### todo清单功能
入口函数在MainWindow::MainWindow(DWidget *parent)
中的`setToDo();`中执行。
在该方法中，我们new了todo对象，并在之后设置了todo的总体大小并移动到指定位置
```c++
void MainWindow::setToDo()
{
    TodoClassManager *todo =new TodoClassManager(this);
    todo->resize(220,238);
    todo->move(0,80);
}
```

TodoClassManager类中定义了todo清单的大部分功能
实现思路：
0. 设：一个复选框，一个输入框，一个删除按钮为一组条目。
1. 读取系统`~/.config/sparkassistant/todo.json`内容。若无该文件，则复制示例文件到这里。
2. 若json为空，则生成一组条目，用于增加样式，然后删除该条目
3. 若json不为空，则读取json并用for循环实例化多组条目，然后将json上的数据应用在条目上。
4. 当点击复选框/更改文本/点击删除按钮时，触发saveToJsonFile()，若删除一个条目，则在json中添加isDel:true做标记
5. 关闭窗口后，在析构函数中调用delJsonFile()来删除被标记为true的条目

实现方案：
1. 可以用ToDo类直接实例化一组条目。该组你需要设置样式：todoLayout
```c++
ToDo *todo=new ToDo(todoWidget);//实例化条目
todo->setLayout(todosLayout);//设置样式
todosVboxLayout->addLayout(todosLayout);
```
2. loadFromJsonFile()读取json
```c++
void TodoClassManager::loadFromJsonFile(){

    //创建文件路径
    QDir home = QDir::home();
    QString configPath = home.filePath(".config/sparkassistant");
    QDir dir(configPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QString path = dir.filePath("todo.json");
    QFile file;
    file.setFileName(path);
    //将qt中文件复制到~/.config/sparkassistant/
    if (!file.exists()) {
        QFile todoJson(":/res/todo.json");
        todoJson.copy(path);
    }
    //设置文件权限
    file.setPermissions(QFile::ReadUser | QFile::WriteUser | QFile::ReadGroup | QFile::WriteGroup | QFile::ReadOther | QFile::WriteOther);
    if(!file.open(QIODevice::ReadWrite)) {
        qDebug() << "File open error";
    } else {
        qDebug() <<"File open!";
    }
    qDebug()<<"file路径为"<<path;

    QByteArray data=file.readAll();//将内容加载到data中

    doc=QJsonDocument::fromJson(data);//将data转为json格式
    QJsonObject itemObj=doc.object();//将json格式的内容初始化
    itemArray=itemObj.value("items").toArray();//解析items数组,将json存放于itemArray

}
```
4. saveToJsonFile(bool completed,QString name,int i,bool isDel,bool isAdd)方法中
参数依次为：是否选中，输入框内容，该条目在json中为第i组,是否删除，是否增加一个条目

5. 在析构函数中调用delJsonFile()来删除被标记的条目
```c++
void TodoClassManager::delJsonFile()
{
    qDebug()<<doc;
    qDebug()<<"del"<<doc;


//    QJsonDocument fixedDoc;
//    fixedDoc=doc;
    QDir home = QDir::home();
    QString configPath = home.filePath(".config/sparkassistant");
    QDir dir(configPath);
    QString path = dir.filePath("todo.json");
    QFile file;
    file.setFileName(path);
    file.setPermissions(QFile::ReadUser | QFile::WriteUser | QFile::ReadGroup | QFile::WriteGroup | QFile::ReadOther | QFile::WriteOther);
    if(!file.open(QIODevice::ReadWrite)) {
        qDebug() << "File open error";
    } else {
        qDebug() <<"File open!";
    }
    //删除多余条目
    QJsonObject docObj =doc.object();
    QJsonArray docArray=docObj.value("items").toArray();
    qDebug()<<"del1"<<doc;
    for(int i=0;i<docArray.size();i++){
        QJsonObject item =docArray[i].toObject();
        qDebug()<<"item信息"<<item;
        if(item.value("isDel").toBool()==true){
            docArray.removeAt(i);
        }
    }
    docObj.insert("items", docArray);
    doc.setObject(docObj);
    qDebug()<<"del2"<<doc;
    file.resize(0);
    file.write(doc.toJson());
}
```





