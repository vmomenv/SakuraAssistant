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
    setWindowOpacity(0.5);//设置透明

}

//失焦关闭窗口
void MainWindow::focusOutEvent(QFocusEvent *event){
    close();
}


MainWindow::~MainWindow(){

}
