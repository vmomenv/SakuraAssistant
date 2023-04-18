#ifndef PASSBOOKPASSWORD_H
#define PASSBOOKPASSWORD_H
#include "passbook.h"
#include<QDialog>
class passbookPassword
{
public:
    passbookPassword();
    QString getPassword();
private:
    QString m_password;
};

#endif // PASSBOOKPASSWORD_H
