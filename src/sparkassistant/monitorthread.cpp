#include "monitorthread.h"
#include<QProcess>
#include<QTextStream>
#include<QRegularExpression>
#include<QDebug>
#include<DLabel>
MonitorThread::MonitorThread(QWidget *parent):QWidget(parent)
{
    QWidget *labelWidget=new QWidget(this);
    DLabel *label1 = new DLabel(labelWidget);
    label1->setText("CPU:");

    DLabel *label2 = new DLabel(labelWidget);
    label2->setText("MEM:");
    QHBoxLayout *hboxLayout =new QHBoxLayout(labelWidget);

//     将两个 QLabel 对象添加到布局中
    hboxLayout->addWidget(label1);
    hboxLayout->addWidget(label2);

}




