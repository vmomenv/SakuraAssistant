#include "mainwindow.h"
#include <QDebug>
#include<QFrame>
#include<DTitlebar>
#include<DWidget>

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

}

//失焦关闭窗口
void MainWindow::focusOutEvent(QFocusEvent *event){
    close();
}

void MainWindow::updateUpdateButton(){
    sysUpdateButton =new DPushButton(this);
    sysUpdateButton->resize(97,34);
    sysUpdateButton->move(10,322);
    sysUpdateButton->setText("系统更新");


    appUpdateButton=new DPushButton(this);
    appUpdateButton->resize(97,34);
    appUpdateButton->move(115,322);
    appUpdateButton->setText("应用更新");

}

void MainWindow::weather(){

}
MainWindow::~MainWindow(){

}
