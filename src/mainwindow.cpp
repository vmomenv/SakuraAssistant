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
MainWindow::MainWindow(DWidget *parent)
{
    setFixedSize(230,376);
    titlebar()->setFixedHeight(0);
    qDebug()<<desktopWidget<<desktopHeight;
    this->move(desktopWidget-230,desktopHeight-376);
//    setWindowOpacity(0.5);//设置透明
    setAttribute(Qt::WA_TranslucentBackground);//设置背景透明
    setEnableBlurWindow(true);

    updateUpdateButton();
    connect(sysUpdateButton, SIGNAL(clicked()), this, SLOT(on_sysUpdateButton_clicked()));

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
    weatherCity=new DLabel(this);
    weatherPic=new DLabel(this);
    WeatherTemperature=new DLabel(this);

//    weatherCity->setText("正在获取城市中..");
//    weatherPic->setPixmap(":/res/type/Qing.png");

}
void MainWindow::on_sysUpdateButton_clicked(){

    //等待特效
    DSpinner *waitDspinner =new DSpinner(MainWindow::sysUpdateButton);
    waitDspinner->start();
    waitDspinner->isPlaying();
    waitDspinner->show();

    isUpdating=true;//正在系统更新，取消失焦关闭动作
    QProcess processUpdate;//apt update
    QProcess processList;//apt list
    QProcess processDspinner;



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

                QProgressDialog *progressDialog = new QProgressDialog(this);
                progressDialog->setLabelText("Updating, please wait...");
                progressDialog->setCancelButton(0);
                progressDialog->setRange(0, 0);
                progressDialog->setModal(true);
                progressDialog->show();

                process.start("bash", QStringList() << "-c" << "pkexec apt install -y " + item->text());
                process.waitForFinished();

                progressDialog->hide();
            }
        }
    });
    resultDialog->exec();

    isUpdating=false;



}
MainWindow::~MainWindow(){

}
