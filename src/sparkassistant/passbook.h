#ifndef PASSBOOK_H
#define PASSBOOK_H
#include <DWidget>
#include <DMainWindow>
#include <QMainWindow>
#include <QObject>
#include <QWidget>
DWIDGET_USE_NAMESPACE

class PassBook : public DMainWindow
{
    Q_OBJECT
public:
    explicit PassBook(DWidget *parent = nullptr);
private:
    QWidget *titleWidget;
    QWidget *searchWidget;
signals:

public slots:
};

#endif // PASSBOOK_H
