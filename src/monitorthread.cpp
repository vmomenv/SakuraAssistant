#include "monitorthread.h"
#include<QProcess>
#include<QTextStream>
#include<QRegularExpression>
#include<QDebug>
MonitorThread::MonitorThread(QWidget *parent) : QWidget(parent)
{

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

//        msleep(1000);
    }
}
