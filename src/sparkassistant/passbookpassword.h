#ifndef PASSBOOKPASSWORD_H
#define PASSBOOKPASSWORD_H
#include "passbook.h"
#include<QDialog>
#include<QPixmap>
#include<QByteArray>
DWIDGET_USE_NAMESPACE
class passbookPassword
{
public:
    passbookPassword();
    QString getPassword();
    void setPassword();
    bool verifyEncryption();
    void login();
    QByteArray decode(QString confirmPassword);
private:
    QString m_password;
    QJsonDocument doc;
    QByteArray jsonData;
    QJsonObject jsonObj;

};

#endif // PASSBOOKPASSWORD_H
