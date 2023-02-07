#ifndef WIDGET_H
#define WIDGET_H
#include <DWidget>
#include <DMainWindow>
#include <QDesktopWidget>
#include<DApplication>
#include<DPushButton>
#include<DLabel>
#include<QPixmap>
#include<QTextEdit>
#include<QProcess>
#include<DSpinner>
DWIDGET_USE_NAMESPACE
class MainWindow :public DMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void destructToDo();
private:
    //读取屏幕信息
    QDesktopWidget *desktop=DApplication::desktop();
    int desktopWidget =desktop->width();
    int desktopHeight = desktop->height();

    DPushButton *sysUpdateButton;
    DPushButton *appUpdateButton;

    DLabel *weatherCity;
    DLabel *weatherTemperature;
    DLabel *weatherPic;

    bool isUpdating;

    QThread *spinnerThread;

    



protected:
//    void focusOutEvent(QFocusEvent *event);
    void updateUpdateButton();
    void weather();
    void setToDo();

private slots:
    void on_sysUpdateButton_clicked();
    bool eventFilter(QObject *,QEvent *);


};
#endif // DMAINWINDOW_H
