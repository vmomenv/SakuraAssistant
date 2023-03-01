#ifndef MONITORTHREAD_H
#define MONITORTHREAD_H

#include <QWidget>

class MonitorThread : public QWidget
{
    Q_OBJECT
public:
    explicit MonitorThread(QWidget *parent = nullptr);
    void run();

signals:
    void updateData(double cpu, double mem);
public slots:
};

#endif // MONITORTHREAD_H
