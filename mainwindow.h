#ifndef WIDGET_H
#define WIDGET_H
#include <DWidget>
#include <DMainWindow>
#include <QDesktopWidget>
#include<DApplication>
DWIDGET_USE_NAMESPACE
class MainWindow :public DMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    //读取屏幕信息
    QDesktopWidget *desktop=DApplication::desktop();
    int desktopWidget =desktop->width();
    int desktopHeight = desktop->height();

protected:
    void focusOutEvent(QFocusEvent *event);

};
#endif // DMAINWINDOW_H
