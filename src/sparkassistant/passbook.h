#ifndef PASSBOOK_H
#define PASSBOOK_H
#include <DWidget>
#include <DMainWindow>
#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include<QLineEdit>
#include<QHBoxLayout>
#include<QLabel>
DWIDGET_USE_NAMESPACE

class PassBook : public DMainWindow
{
    Q_OBJECT
public:
    explicit PassBook(DWidget *parent = nullptr);
private:
    QWidget *titleWidget;
    QWidget *searchWidget;
    QLineEdit *searchEdit;
    QHBoxLayout *searchEditLayout;
    QLabel *searchPicLabel;

signals:

public slots:
};

#endif // PASSBOOK_H
