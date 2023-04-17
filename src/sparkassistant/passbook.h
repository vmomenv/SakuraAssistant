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
#include<QSpacerItem>
#include<QDir>
#include<QVBoxLayout>
#include "aesencrypt.h"
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QVector>
#include <QDebug>
#include "qaesencryption.h"
#include "labelbutton.h"
DWIDGET_USE_NAMESPACE

class PassBook : public DMainWindow
{
    Q_OBJECT
public:
    explicit PassBook(DWidget *parent = nullptr);
    bool eventFilter(QObject *watched, QEvent *event);
    void saveToJsonFile(QString targetName,QString username,QString password,int i,bool isDel,bool isAdd);
    void delJsonFile();
    QByteArray encryptJsonFile(QJsonDocument doc,const QString& password);
    QByteArray decryptJsonFile(QByteArray jsonData, const QString& password);

    ~PassBook();


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
    QVBoxLayout *allCredentialsLayout;
    QPushButton *delButton;
    QScrollArea *scrollArea;
    QPushButton *showPasswordButton;
    QPushButton *copyButton;
    QPushButton *setTopButton;
    QSpacerItem *spacer;
    QPushButton *addPassButton;
    QSpacerItem *passbookSpacer;//底部弹簧
    QJsonDocument doc;
    bool isUpdating;
    int *addIndex;


signals:

public slots:
};

#endif // PASSBOOK_H
