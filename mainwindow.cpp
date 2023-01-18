#include "mainwindow.h"
#include <QDebug>
#include<QFrame>
#include<DTitlebar>
#include<DWidget>
#include<QProcess>
#include<DDialog>
#include<DPasswordEdit>
#include <QInputDialog>
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

}
void MainWindow::on_sysUpdateButton_clicked(){
    isUpdating=true;//正在系统更新，取消失焦关闭动作
    QProcess process;
    process.start("bash", QStringList() << "-c" << "pkexec apt update && apt list --upgradable");

    process.waitForFinished();
    QString updateResult;
    updateResult = process.readAllStandardOutput();
//    QProcess::startDetached("pkexec apt-get update && apt-get upgrade");
    qDebug()<<updateResult;

//    //更新信息编写
//    QStringList updateInfo=updateResult.split("\n");
//    QString packageName;
//    QString currentVersion;
//    QString newVersion;
//    for (const QString& line : updateInfo) {
//        if (line.startsWith("Package:")) {
//            packageName = line.mid(8).trimmed();

//        } else if (line.startsWith("Current Version:")) {
//            currentVersion = line.mid(15).trimmed();
//        } else if (line.startsWith("New Version:")) {
//            newVersion = line.mid(12).trimmed();
//        }
//    }

//    qDebug()<<packageName;
    QDialog *resultDialog = new QDialog(this);
    resultDialog->setWindowTitle("Update Result");
    QTextEdit *textEdit = new QTextEdit(resultDialog);
    textEdit->setText(updateResult);
    QVBoxLayout *layout = new QVBoxLayout(resultDialog);
    layout->addWidget(textEdit);
    resultDialog->exec();
    isUpdating=false;



}
MainWindow::~MainWindow(){

}
