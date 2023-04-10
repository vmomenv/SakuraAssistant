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
#include<QGraphicsOpacityEffect>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include<QPushButton>
//#include "labelbutton.h"
DWIDGET_USE_NAMESPACE

class PassBook : public DMainWindow
{
    Q_OBJECT
public:
    explicit PassBook(DWidget *parent = nullptr);
    void readJson();
    void writeJson(QString targetName, QString username, QString password);
    void addCredentialsWidget(QString targetName, QString username, QString password);
private:
    QWidget *titleWidget;
    QWidget *searchWidget;
    QLineEdit *searchEdit;
    QHBoxLayout *searchEditLayout;
    QLabel *searchPicLabel;
    QWidget *passWidget;
    QLabel *label;
    QLabel *passwordLabel;
    QLabel *usernameLabel;
    QLabel *targetNameLabel;
    QWidget *credentialsWidget;
    QLineEdit *targetNameLineEdit;
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QHBoxLayout *credentialsLayout;


signals:

public slots:
};

#endif // PASSBOOK_H
