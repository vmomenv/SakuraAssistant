#include "mainwindow.h"
#include <QDebug>
#include<QFrame>
#include<DTitlebar>

#include<DWidget>
MainWindow::MainWindow(DWidget *parent)
{
    setFixedSize(230,376);
    titlebar()->setFixedHeight(0);
}
void MainWindow::focusOutEvent(QFocusEvent *event){
    close();
}

MainWindow::~MainWindow(){

}
