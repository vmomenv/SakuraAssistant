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
    system("sudo apt update");
\
    appUpdateButton=new DPushButton(this);
    appUpdateButton->resize(97,34);
    appUpdateButton->move(115,322);
    appUpdateButton->setText("应用更新");


}

void MainWindow::weather(){
    //天气预报
    //初始化温度
    weatherTemperature=new DLabel(this);
    weatherTemperature->move(56,34);
    weatherTemperature->setText("0℃");
    weatherTemperature->setIndent(20);
    QFont tempFont;
    tempFont.setPointSize(18);
    weatherTemperature->setFont(tempFont);
    QFont cityFont;
    weatherTemperature->setFont(cityFont);
    // weatherTemperature->setAlignment(Qt::AlignLeft);

    // 初始化城市
    weatherCity=new DLabel(this);
    weatherCity->move(56,12);
    cityFont.setPointSize(12);
    weatherCity->setText("正在获取城市中..");

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
