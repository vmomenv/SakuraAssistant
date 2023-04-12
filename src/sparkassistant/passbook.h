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
#include<QScrollArea>
#include<QClipboard>
#include<QMessageBox>
#include "labelbutton.h"
DWIDGET_USE_NAMESPACE

class PassBook : public DMainWindow
{
    Q_OBJECT
public:
    explicit PassBook(DWidget *parent = nullptr);
    bool eventFilter(QObject *watched, QEvent *event);

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
    QLabel *keyLabel;
    QLineEdit *targetNameLineEdit;
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QWidget *credentialWidget;//一条凭据
    QHBoxLayout *credentialLayout;
    QWidget *allCredentialsWidget;//所有凭据
    QHBoxLayout *allCredentialsLayout;
    LabelButton *delButton;
    QScrollArea *scrollArea;
    QPushButton *showPasswordButton;
    QPushButton *copyButton;
    QPushButton *setTopButton;
    bool isUpdating;


signals:

public slots:
};

#endif // PASSBOOK_H
