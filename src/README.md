## 这是星火助手的文档

部分功能采用dtk，如按钮，背景高斯模糊

[传送门 - DTK 文档与资源 | dtk (linuxdeepin.github.io)](https://linuxdeepin.github.io/dtk/README.zh_CN)

https://deepin-opensource.gitee.io/dtk/class_dtk_1_1_widget_1_1_d_application.html

### mainwindow主界面

本应用采用了dtk的高斯模糊功能

在mainwindow.cpp中。在mainwindow的构造函数中，我们使用setAttribute(Qt::WA_TranslucentBackground);//设置背景透明

setEnableBlurWindow(true);//设置高斯模糊

https://linuxdeepin.github.io/dtkwidget/classDtk_1_1Widget_1_1DMainWindow.html#a4e932450731898588cfa47379936cfb7



### 系统更新功能

在Mainwindow中updateUpdateButton();实现按钮显示


 connect(sysUpdateButton, SIGNAL(clicked()), this, SLOT(on_sysUpdateButton_clicked()));来监听按钮


MainWindow::on_sysUpdateButton_clicked()来实现系统更新功能



### 天气更新功能

> （该功能的部分代码实现来源大轮明王讲qt：https://www.bilibili.com/video/BV1D841147zt/?spm_id_from=333.788）

weatherparse.h为天气解析类

WeatherParse::WeatherParse()中含有一个map表存放天气情况和天气图标

WeatherParse::requestServer()用于请求ip地址服务器和天气服务器，如果reply为http://ip-api.com/json，则执行ipCityParseJson(byteArray)来请求ip地址，否则执行weatherParseJson(byteArray);来获取天气

### todo功能

在todo.h中声明了类ToDo，里面包含一个复选框，输入框，按钮。我们之后会把这个称为一个组。在todo的构造函数中，我们初始化了这三个按钮，并且写了水平布局样式方法。

```c++
void ToDo::setLayout(QHBoxLayout *hbox)//设置该组样式
{
    hbox->addWidget(checkBox);
    hbox->addWidget(line);
    hbox->addWidget(delBtn);
}
```

在todoclassmanager.h中，我们将要完成  

`void loadFromJsonFile();`加载文件

 `void saveToJsonFile(bool completed,QString name,int i,bool isDel);`保存文件

bool completed :复选框状态

QString name:文本框内容

int i:第i组

bool isDel:是否删除 

多个控件组用QVBoxLayout *todosVboxLayout;进行垂直布局。与QScrollArea一起放入一个QWidget中。

   
