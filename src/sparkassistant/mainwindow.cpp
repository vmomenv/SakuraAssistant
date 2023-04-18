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
#include<QScrollArea>
#include<QStandardItemModel>
#include<QAbstractItemModel>
#include<QTableView>
#include<monitorthread.h>
#include "todoclassmanager.h"
#include<QPalette>
MainWindow::MainWindow(DWidget *parent)
{
//    isUpdating=true;
    setFixedSize(230,376);
    titlebar()->setFixedHeight(0);
    setFocusPolicy(Qt::ClickFocus);
    installEventFilter(this);//安装事件过滤器
    qDebug()<<desktopWidget<<desktopHeight;
    this->move(desktopWidget-230,desktopHeight-376);
//    setWindowOpacity(0.5);//设置透明
    setAttribute(Qt::WA_TranslucentBackground);//设置背景透明
    setEnableBlurWindow(true);//设置高斯模糊

    updateUpdateButton();

    connect(sysUpdateButton, SIGNAL(clicked()), this, SLOT(on_sysUpdateButton_clicked()));
    connect(passbookButton, SIGNAL(clicked()), this, SLOT(on_passbookButton_clicked()));


//    //设置背景色
//    QLinearGradient gradient(0, 0, 0, height());
//    gradient.setColorAt(0, QColor("#fce38a"));
//    gradient.setColorAt(1, QColor("#f38181"));
//    QBrush brush(gradient);
//    QPalette palette;
//    palette.setBrush(QPalette::Background, brush);
//    this->setPalette(palette);

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
    setMonitor();


}

////失焦关闭窗口 --已替换为eventFilter：失焦判断更精确，单击文本框不会判定为失焦
//void MainWindow::focusOutEvent(QFocusEvent *event){
//    if(isUpdating==false){
//        close();
//    }

//}

void MainWindow::updateUpdateButton(){
    sysUpdateButton =new DPushButton(this);
    sysUpdateButton->resize(97,34);
    sysUpdateButton->move(10,322);
    sysUpdateButton->setText("系统更新");
//    system("sudo apt update");

    passbookButton=new DPushButton(this);
    passbookButton->resize(97,34);
    passbookButton->move(115,322);
    passbookButton->setText("密码本");


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





}

void MainWindow::setToDo()
{

    TodoClassManager *todo =new TodoClassManager(this);
    todo->resize(220,238);
    todo->move(0,80);

}

void MainWindow::setMonitor()
{
//    MonitorThread *monitor=new MonitorThread(this);
////    monitor->move(293,58);
//    monitor->resize(130,30);
//    monitor->move(50,280);

}
void MainWindow::on_sysUpdateButton_clicked(){

    isUpdating=true;//正在系统更新，取消失焦关闭动作


    QProcess processUpdate;//apt update
    QProcess processList;//apt list


    processUpdate.start("bash", QStringList() << "-c" << "pkexec apt update");
    processUpdate.waitForFinished();


    processList.start("bash", QStringList() << "-c" << "apt list --upgradable");
    processList.waitForFinished();

    //将系统更新信息放入updateResult中
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
    QPushButton *updateButton = new QPushButton("更新", resultDialog);
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
                QString updatedResult;
                updatedResult = process.readAllStandardOutput();//更新后信息输出
                qDebug()<<updatedResult;
                item->setHidden(true);
            }
        }
    });
    resultDialog->exec();


    isUpdating=false;



}

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

                this->hide();
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}


void MainWindow::destructToDo()
{
    //Delete the created widgets
    QList<QCheckBox*> checkBoxList = this->findChildren<QCheckBox*>();
    for (int i = 0; i < checkBoxList.size(); i++) {
        QCheckBox* checkBox = checkBoxList.at(i);
        delete(checkBox);
    }

    QList<QLineEdit*> lineEditList = this->findChildren<QLineEdit*>();
    for (int i = 0; i < lineEditList.size(); i++) {
        QLineEdit* lineEdit = lineEditList.at(i);
        lineEdit->deleteLater();
    }

    QList<QPushButton*> pushButtonList = this->findChildren<QPushButton*>();
    for (int i = 0; i < pushButtonList.size(); i++) {
        QPushButton* pushButton = pushButtonList.at(i);
        pushButton->deleteLater();
    }
}

void MainWindow::on_passbookButton_clicked(){
    passbookPassword *pbp=new passbookPassword;
    QString password=pbp->getPassword();
    qDebug()<<"main"<<password;
    PassBook *passBook=new PassBook(password);
    passBook->show();
}
MainWindow::~MainWindow(){

}
