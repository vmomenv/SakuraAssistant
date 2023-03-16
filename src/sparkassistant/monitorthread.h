#ifndef MONITORTHREAD_H
#define MONITORTHREAD_H
#include <DWidget>
#include <DMainWindow>
#include <QWidget>
#include <QHBoxLayout>
DWIDGET_USE_NAMESPACE
class MonitorThread : public QWidget{

    Q_OBJECT
public:
    explicit MonitorThread(QWidget *parent = nullptr);
private:
    QHBoxLayout *hboxLayout;
    QWidget *labelWidget;

signals:
public slots:
};

#endif // MONITORTHREAD_H
