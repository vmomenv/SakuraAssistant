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
    label1->setText("Label 1");

    DLabel *label2 = new DLabel(labelWidget);
    label2->setText("Label 2");
    QHBoxLayout *hboxLayout =new QHBoxLayout(labelWidget);

//     将两个 QLabel 对象添加到布局中
    hboxLayout->addWidget(label1);
    hboxLayout->addWidget(label2);
    labelWidget->move(10,10);

}

void MonitorThread::run()
{
    while (true) {
        QProcess process;
        process.start("top -b -n 1");
        process.waitForFinished();
        QString output = process.readAllStandardOutput();

        QTextStream stream(&output);
        QString line = stream.readLine();
        while (!line.startsWith("Cpu(s)")) {
            line = stream.readLine();
        }

        QRegularExpression re("[0-9]+\\.[0-9]+");
        QRegularExpressionMatch match = re.match(line);

        double cpu = match.captured(0).toDouble();
//        match = match.next();
        double mem = match.captured(0).toDouble();
        qDebug()<<cpu<<mem;
        emit updateData(cpu, mem);


    }
}

