#include "monitorthread.h"
#include <QThread>
#include <QDebug>
#include <QTimer>
#include<DLabel>
#include<QProcess>
MonitorThread::MonitorThread(QWidget *parent)
    : QWidget(parent)
{
    labelWidget = new QWidget(this);
    DLabel *label1 = new DLabel(labelWidget);
    label1->setText("CPU:");

    DLabel *label2 = new DLabel(labelWidget);
    label2->setText("MEM:");
    hboxLayout = new QHBoxLayout(labelWidget);

    // 将两个 QLabel 对象添加到布局中
    hboxLayout->addWidget(label1);
    hboxLayout->addWidget(label2);

    // 设置布局为 labelWidget 的主布局
    labelWidget->setLayout(hboxLayout);

    // 将 labelWidget 添加到 MonitorThread 的主布局中
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(labelWidget);

    // 创建并启动定时器
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MonitorThread::updateLabels);
    timer->start(1000);  // 每秒更新一次

    // 启动线程
    QThread *thread = new QThread(this);
    connect(thread, &QThread::started, this, &MonitorThread::run);
    this->moveToThread(thread);
    thread->start();
}

void MonitorThread::run()
{
    while (true) {
        // 获取 CPU 和内存占用情况
        QProcess process;
        process.start("top -n 1");
        process.waitForFinished();
        QString output = process.readAllStandardOutput();
        QString cpuUsage = output.split("\n").value(2).split(",").value(0).trimmed().remove("Cpu(s): ");
        QString memUsage = output.split("\n").value(3).split(",").value(1).trimmed().remove("Mem : ");

        // 发送信号更新标签文本
        emit cpuUsageUpdated(cpuUsage);
        emit memUsageUpdated(memUsage);

        // 等待一段时间再继续获取占用情况
        QThread::msleep(5000);
    }
}

void MonitorThread::updateLabels()
{
    // 获取最新的占用情况并更新标签文本
    labelWidget->findChild<DLabel*>("cpuLabel")->setText(QString("CPU: %1").arg(m_cpuUsage));
    labelWidget->findChild<DLabel*>("memLabel")->setText(QString("MEM: %1").arg(m_memUsage));
}

void MonitorThread::setCpuUsage(const QString &usage)
{
    m_cpuUsage = usage;
}

void MonitorThread::setMemUsage(const QString &usage)
{
    m_memUsage = usage;
}
