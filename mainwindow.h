#ifndef WIDGET_H
#define WIDGET_H

#include <DWidget>
DWIDGET_USE_NAMESPACE
DWIDGET_USE_NAMESPACE
#include <DMainWindow>
class MainWindow :public DMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:

protected:
    void focusOutEvent(QFocusEvent *event);
};
#endif // DMAINWINDOW_H
