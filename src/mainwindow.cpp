#include "mainwindow.h"
#include <QDebug>
#include<QFrame>
#include<DTitlebar>
#include<DWidget>
#include<QProcess>
#include<DDialog>
#include<DPasswordEdit>
#include <QInputDialog>
#include<QPixmap>
#include<QCheckBox>
#include<QListWidgetItem>
#include<QProgressDialog>
#include<DSpinner>
#include<QThread>
#include<weatherparse.h>
#include<todo.h>
#include<QButtonGroup>
#include<QWidget>
MainWindow::MainWindow(DWidget *parent)
{
    isUpdating=false;
    setFixedSize(230,376);
    titlebar()->setFixedHeight(0);
    qDebug()<<desktopWidget<<desktopHeight;
    this->move(desktopWidget-230,desktopHeight-376);
//    setWindowOpacity(0.5);//设置透明
    setAttribute(Qt::WA_TranslucentBackground);//设置背景透明
    setEnableBlurWindow(true);

    updateUpdateButton();

    connect(sysUpdateButton, SIGNAL(clicked()), this, SLOT(on_sysUpdateButton_clicked()));

    weather();
    WeatherParse *weather=new WeatherParse;
    connect(weather,&WeatherParse::update,[=](){
            weatherCity->setText(weather->cityName);
            weatherTemperature->setText(weather->cityTemperature+"℃");
            QPixmap *cityPix=new QPixmap(weather->weatherType);
            weatherPic->setPixmap(*cityPix);
            weather->deleteLater();
    });
    setToDo();
}

//失焦关闭窗口
void MainWindow::focusOutEvent(QFocusEvent *event){
    if(isUpdating==false){
        close();
    }

}

void MainWindow::updateUpdateButton(){
    sysUpdateButton =new DPushButton(this);
    sysUpdateButton->resize(97,34);
    sysUpdateButton->move(10,322);
    sysUpdateButton->setText("系统更新");
//    system("sudo apt update");
\
    appUpdateButton=new DPushButton(this);
    appUpdateButton->resize(97,34);
    appUpdateButton->move(115,322);
    appUpdateButton->setText("我是凑数的");


}

void MainWindow::weather(){
    //天气预报
    QFont tempFont;

    // 初始化城市
    weatherCity=new DLabel(this);
    weatherCity->move(70,12);
    weatherCity->resize(156,23);
    tempFont.setPointSize(10);
    weatherCity->setFont(tempFont);
    weatherCity->setText("正在获取城市中..");

    //初始化温度
    weatherTemperature=new DLabel(this);
    weatherTemperature->move(50,34);
    weatherTemperature->setIndent(20);
    tempFont.setPointSize(18);
    weatherTemperature->setFont(tempFont);
    weatherTemperature->setText("--℃");



    // 将weatherCity 和 weatherTemperature 垂直布局
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(weatherTemperature);
    layout->addWidget(weatherCity);
    layout->setAlignment(Qt::AlignLeft);
    this->setLayout(layout);
    weatherTemperature->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    weatherCity->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    //初始化天气图标
    QPixmap *cityPix=new QPixmap(":/res/type/Qing.png");
    weatherPic=new DLabel(this);
    weatherPic->move(22,26);
    weatherPic->setScaledContents(true);//设置自适应
    weatherPic->setMaximumSize(24,24);
    weatherPic->setPixmap(*cityPix);

    //解析天气




}

void MainWindow::setToDo()
{
    ToDo *todoList=new ToDo;
    todoList->loadFromJsonFile();//加载todo.json
    int radioButtonHigh=70;
    int radioButtonWeight=18;

    for(int i=0;i<todoList->itemArray.size();i++){
        isUpdating=true;
        QJsonObject item =todoList->itemArray[i].toObject();
        TodoItem todoItem;
        todoItem.name=item.value("name").toString();
        todoItem.completed=item.value("completed").toBool();
        QCheckBox *checkBox = new QCheckBox(this);
        checkBox->setChecked(todoItem.completed);
        checkBox->resize(250,40);
        checkBox->move(radioButtonWeight,radioButtonHigh);

        QLineEdit *lineEdit = new QLineEdit(checkBox);
        lineEdit->setVisible(true);
        lineEdit->setReadOnly(false);
        lineEdit->move(radioButtonWeight+10,0);
        lineEdit->setText(todoItem.name);
        QPushButton *pushbutton;

        connect(lineEdit,&QLineEdit::editingFinished,this,[=]{
            qDebug()<<1;

//            todoList->saveToJsonFile();
            isUpdating=false;
        });
        // 连接信号和槽
//        connect(checkBox, &QCheckBox::clicked, this,[=] {
//            isUpdating=true;

//            QPushButton *pushbutton=new QPushButton(checkBox);
//            lineEdit->setVisible(true);
//            lineEdit->setReadOnly(false);
//            lineEdit->move(radioButtonWeight+10,0);
//            pushbutton->setVisible(true);
//            pushbutton->resize(30,30);
//            pushbutton->move(170,5);
//            pushbutton->setText("√");
//            connect(pushbutton,&QPushButton::clicked,this,[=]{
//                pushbutton->hide();
//                checkBox->setText(lineEdit->text());
//            });

//        });




        radioButtonHigh+=40;
        qDebug()<<todoItem.name<<todoItem.completed;
    }


}
void MainWindow::on_sysUpdateButton_clicked(){

    isUpdating=true;//正在系统更新，取消失焦关闭动作


    QProcess processUpdate;//apt update
    QProcess processList;//apt list

    processUpdate.start("bash", QStringList() << "-c" << "pkexec apt update");
    processUpdate.waitForFinished();


    processList.start("bash", QStringList() << "-c" << "apt list --upgradable");
    processList.waitForFinished();

    QString updateResult;
    updateResult = processList.readAllStandardOutput();
//    QProcess::startDetached("pkexec apt-get update && apt-get upgrade");
    qDebug()<<updateResult;

    //将更新信息呈现到对话框中
    QDialog *resultDialog = new QDialog(this);//删除输入框
    resultDialog->setWindowTitle("待升级的应用");

    // 创建QListWidget并添加到对话框中
    QListWidget *listWidget = new QListWidget(resultDialog);
    QStringList lines = updateResult.split("\n");
    for (int i = 0; i < lines.size(); i++) {
        QString line = lines[i];
        if (line.startsWith("Listing"))
            continue;

        QStringList parts = line.split("/");
        if (parts.size() < 2)
            continue;

        QString packageName = parts[0];
        QListWidgetItem *item = new QListWidgetItem(packageName);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        listWidget->addItem(item);
    }

    //添加按钮
    QPushButton *updateButton = new QPushButton("Update", resultDialog);
    //布局
    QVBoxLayout *layout = new QVBoxLayout(resultDialog);
    layout->addWidget(listWidget);
    layout->addWidget(updateButton);
    resultDialog->setLayout(layout);

    //设置信号槽
    connect(updateButton, &QPushButton::clicked, this, [=](){
        for (int i = 0; i < listWidget->count(); i++) {
            QListWidgetItem *item = listWidget->item(i);
            if (item->checkState() == Qt::Checked) {
                QProcess process;
                process.start("bash", QStringList() << "-c" << "pkexec apt install -y " + item->text());
                process.waitForFinished();

            }
        }
    });
    resultDialog->exec();


    isUpdating=false;



}
MainWindow::~MainWindow(){

}
