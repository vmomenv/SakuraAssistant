#ifndef MONITORTHREAD_H
#define MONITORTHREAD_H

#include <DWidget>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>

DWIDGET_USE_NAMESPACE

class MonitorThread : public QWidget
{
    Q_OBJECT

public:
    explicit MonitorThread(QWidget *parent = nullptr);

signals:
    void cpuUsageUpdated(const QString &usage);
    void memUsageUpdated(const QString &usage);

private slots:
    void updateLabels();
    void setCpuUsage(const QString &usage);
    void setMemUsage(const QString &usage);

private:
    QHBoxLayout *hboxLayout;
    QWidget *labelWidget;
    QString m_cpuUsage;
    QString m_memUsage;

    void run();
};

#endif // MONITORTHREAD_H
