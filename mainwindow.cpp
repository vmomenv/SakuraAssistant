#include "mainwindow.h"
#include <QDebug>
#include<QFrame>
#include<DTitlebar>
#include<QMouseEvent>
#include<DWidget>
MainWindow::MainWindow(DWidget *parent)
{
    setFixedSize(230,376);
    titlebar()->setFixedHeight(0);
//    void MainWindow::mousePressEvent(QMouseEvent *event)
//    {
//        if (event->y() < 0) {
//            close();
//        }
//        DMainWindow::mousePressEvent(event);
//    }


}
MainWindow::~MainWindow(){

}
