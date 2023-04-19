#include "passbookpassword.h"

passbookPassword::passbookPassword()
{
    QByteArray *jsonData;
    if(verifyEncryption()){
        login();
    }else{
        setPassword();
    }

}

QString passbookPassword::getPassword()
{

    return m_password;
}

void passbookPassword::setPassword()
{
    QDialog *enterPassDialog=new QDialog;

    enterPassDialog->setWindowTitle("设置密码");
    enterPassDialog->setFixedSize(861,490);

    QLineEdit *setPassword=new QLineEdit(enterPassDialog);
    setPassword->setFont(QFont("Arial", 13));
    setPassword->setAlignment(Qt::AlignLeft| Qt::AlignVCenter);
    setPassword->setStyleSheet("background-color: #C3C9D3;border-radius: 6px;");
    setPassword->setEchoMode(QLineEdit::Password);
    setPassword->setFixedSize(256, 40);
    setPassword->setTextMargins(40, 3, 40, 0);
    setPassword->setPlaceholderText("请设置密码");
    setPassword->setFixedSize(508,53);
    setPassword->move(177,166);
    QLineEdit *confirmPassword=new QLineEdit(enterPassDialog);
    confirmPassword->setFont(QFont("Arial", 13));
    confirmPassword->setAlignment(Qt::AlignLeft| Qt::AlignVCenter);
    confirmPassword->setStyleSheet("background-color: #C3C9D3;border-radius: 6px;");
    confirmPassword->setEchoMode(QLineEdit::Password);
    confirmPassword->setFixedSize(256, 40);
    confirmPassword->setTextMargins(40, 3, 40, 0);
    confirmPassword->setPlaceholderText("请再次输入密码");
    confirmPassword->setFixedSize(508,53);
    confirmPassword->move(177,273);
    QPushButton *confirmButton = new QPushButton("确认",enterPassDialog);
    confirmButton->setFixedSize(127, 53);
    confirmButton->move(558,384);
    QLabel *warning=new QLabel(enterPassDialog);
    warning->setText("密码用于验证进入密码本并加解密数据，若忘记密码无法找回！");
    warning->setFixedSize(400,28);
    warning->move(196,332);
    QObject::connect(confirmButton, &QPushButton::clicked, [=]{
        if (setPassword->text() == confirmPassword->text()) {
        // 密码验证通过，关闭窗口
            QByteArray ba = QCryptographicHash::hash(confirmPassword->text().toUtf8(), QCryptographicHash::Md5);
            QString a=QString(ba.toHex().left(16));
            m_password=a;
            enterPassDialog->hide();

        } else {
        // 密码不一致，弹出提示框
        QMessageBox::warning(enterPassDialog, "提示", "两次输入的密码不一致！");
        }
    });
    enterPassDialog->exec();
}



bool passbookPassword::verifyEncryption()
{
    QDir home = QDir::home();
    QString dataPath = home.filePath(".config/sakuraassistant");
    QDir dir(dataPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    QString path = dir.filePath("data.json");
    QFile file;
    file.setFileName(path);
    if (!file.exists()) {
        QFile todoJson(":/res/passbook/data.json");
        todoJson.copy(path);
    }
    //设置文件权限
    file.setPermissions(QFile::ReadUser | QFile::WriteUser | QFile::ReadGroup | QFile::WriteGroup | QFile::ReadOther | QFile::WriteOther);
    if(!file.open(QIODevice::ReadWrite)) {
        qDebug() << "File open error";
    } else {
        qDebug() <<"File open!";
    }
    qDebug()<<"file路径为"<<path;

    //循环遍历json并创建多个qwidget
    QJsonDocument doc;
    jsonData= file.readAll();
    doc=QJsonDocument::fromJson(jsonData);
    QJsonObject jsonObj = doc.object();
    if (jsonObj.contains("credentials")) {
        qDebug() << "含credentials字段，未加密";

        return false;
    }else{
        qDebug() << "不包含credentials字段，已加密";
        qDebug()<<jsonData;
        return true;
    }
}

void passbookPassword::login()
{
    QDialog *enterPassDialog=new QDialog;
    enterPassDialog->setWindowTitle("密码本");
    enterPassDialog->setFixedSize(861,490);
    QWidget *loginWidget=new QWidget(enterPassDialog);
    QLineEdit *confirmPasswordEdit=new QLineEdit(loginWidget);
    confirmPasswordEdit->setFont(QFont("Arial", 13));
    confirmPasswordEdit->setAlignment(Qt::AlignLeft| Qt::AlignVCenter);
    confirmPasswordEdit->setStyleSheet("background-color: #C3C9D3;border-radius: 6px;");
    confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    confirmPasswordEdit->setFixedSize(256, 40);
    confirmPasswordEdit->setTextMargins(40, 3, 40, 0);
    confirmPasswordEdit->setPlaceholderText("请输入密码");
    confirmPasswordEdit->setFixedSize(508,53);
    confirmPasswordEdit->move(177,191);

    QPushButton *changeButton = new QPushButton("更改密码",loginWidget);
    changeButton->setFixedSize(127,53);
    changeButton->move(177,268);

    QPushButton *confirmButton = new QPushButton("确认",loginWidget);
    confirmButton->setFixedSize(127,53);
    confirmButton->move(558,268);

    QObject::connect(confirmButton, &QPushButton::clicked, [=]{
        QString tmppassword=confirmPasswordEdit->text();
       QByteArray newJsonData = decode(tmppassword);
       QJsonDocument newDoc=QJsonDocument::fromJson(newJsonData);
       QJsonObject newJsonObj = newDoc.object();
        if (newJsonObj.contains("credentials")) {
            enterPassDialog->hide();
        } else {
            QMessageBox::warning(enterPassDialog, "提示", "密码错误！");

        }

    });
    QObject::connect(changeButton, &QPushButton::clicked, [=]{
        loginWidget->hide();
        QWidget *changePasswordWidget=new QWidget(enterPassDialog);

        QLineEdit *oldPassword=new QLineEdit(changePasswordWidget);
        oldPassword->setFont(QFont("Arial", 13));
        oldPassword->setAlignment(Qt::AlignLeft| Qt::AlignVCenter);
        oldPassword->setStyleSheet("background-color: #C3C9D3;border-radius: 6px;");
        oldPassword->setEchoMode(QLineEdit::Password);
        oldPassword->setFixedSize(256, 40);
        oldPassword->setTextMargins(40, 3, 40, 0);
        oldPassword->setPlaceholderText("请输入旧密码");
        oldPassword->setFixedSize(508,53);
        oldPassword->move(177,166);
        QLineEdit *newPassword=new QLineEdit(changePasswordWidget);
        newPassword->setFont(QFont("Arial", 13));
        newPassword->setAlignment(Qt::AlignLeft| Qt::AlignVCenter);
        newPassword->setStyleSheet("background-color: #C3C9D3;border-radius: 6px;");
        newPassword->setEchoMode(QLineEdit::Password);
        newPassword->setFixedSize(256, 40);
        newPassword->setTextMargins(40, 3, 40, 0);
        newPassword->setPlaceholderText("请输入新密码");
        newPassword->setFixedSize(508,53);
        newPassword->move(177,273);
        QPushButton *confirmButton = new QPushButton("确认",changePasswordWidget);
        confirmButton->setFixedSize(127, 53);
        confirmButton->move(558,384);
        QLabel *warning=new QLabel(changePasswordWidget);
        warning->setText("密码用于验证进入密码本并加解密数据，若忘记密码无法找回！");
        warning->setFixedSize(400,28);
        warning->move(196,332);
        changePasswordWidget->show();
        QObject::connect(confirmButton, &QPushButton::clicked, [=]{
            QString tmppassword=oldPassword->text();
           QByteArray newJsonData = decode(tmppassword);
           QJsonDocument newDoc=QJsonDocument::fromJson(newJsonData);
           QJsonObject newJsonObj = newDoc.object();

            if (newJsonObj.contains("credentials")) {
                QString tmpNewPassword=newPassword->text();
                QByteArray ba = QCryptographicHash::hash(tmpNewPassword.toUtf8(), QCryptographicHash::Md5);
                QString password = QString(ba.toHex().left(16));
                //加密
                QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB, QAESEncryption::PKCS7);
                QByteArray utf8Data = newDoc.toJson(QJsonDocument::Compact);
                QString str = QString::fromUtf8(utf8Data.constData(), utf8Data.size());
                QByteArray enBA = encryption.encode(str.toUtf8(), password.toUtf8());
                QDir home = QDir::home();
                 QString dataPath = home.filePath(".config/sakuraassistant");
                 QDir dir(dataPath);
                 if (!dir.exists()) {
                     dir.mkpath(".");
                 }
                 QString path = dir.filePath("data.json");
                 QFile file;
                 file.setFileName(path);
                 if (!file.exists()) {
                     QFile todoJson(":/res/passbook/data.json");
                     todoJson.copy(path);
                 }
                 //设置文件权限
                 file.setPermissions(QFile::ReadUser | QFile::WriteUser | QFile::ReadGroup | QFile::WriteGroup | QFile::ReadOther | QFile::WriteOther);
                 if(!file.open(QIODevice::ReadWrite)) {
                     qDebug() << "File open error";
                 } else {
                     qDebug() <<"File open!";
                 }
                 qDebug()<<"file路径为"<<path;
                 file.resize(0);
                 file.write(enBA);
                 file.close();
                 m_password=password;
                 enterPassDialog->hide();
            } else {
                QMessageBox::warning(enterPassDialog, "提示", "旧密码错误！");

            }
        });
        enterPassDialog->exec();

    });
    enterPassDialog->exec();
}

QByteArray passbookPassword::decode(QString confirmPassword)//普通密码
{
    QByteArray ba = QCryptographicHash::hash(confirmPassword.toUtf8(), QCryptographicHash::Md5);
    QString password = QString(ba.toHex().left(16));

    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB, QAESEncryption::PKCS7);
    QByteArray deBA = encryption.decode(jsonData, password.toUtf8());
   QByteArray newJsonData = QAESEncryption::RemovePadding(deBA, QAESEncryption::PKCS7);
   QJsonDocument newDoc=QJsonDocument::fromJson(newJsonData);
   QJsonObject newJsonObj = newDoc.object();
   qDebug()<<deBA;
    if (newJsonObj.contains("credentials")) {
        m_password=password;
        return newJsonData;
    }
}
