#include "passbookpassword.h"

passbookPassword::passbookPassword()
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
    QByteArray jsonData = file.readAll();
    doc=QJsonDocument::fromJson(jsonData);
    QJsonObject jsonObj = doc.object();
    if (jsonObj.contains("credentials")) {
        qDebug() << "包含credentials字段，未加密";
        QDialog *enterPassDialog=new QDialog;
        enterPassDialog->setWindowTitle("密码本");
        enterPassDialog->setFixedSize(550,303);
        QLineEdit *setPassword=new QLineEdit(enterPassDialog);
        QLineEdit *confirmPassword=new QLineEdit(enterPassDialog);
        setPassword->setFixedSize(240,31);
        confirmPassword->setFixedSize(240,31);
        setPassword->move(155,100);
        confirmPassword->move(155,174);
        QPushButton *confirmButton = new QPushButton("确认",enterPassDialog);
        confirmButton->setFixedSize(100, 31);
        confirmButton->move(225,242);
        setPassword->setPlaceholderText("请设置密码");
        confirmPassword->setPlaceholderText("请再次输入密码");
        QLabel *warning=new QLabel(enterPassDialog);
        warning->setText("密码用于验证进入密码本并加解密数据，若忘记密码无法找回！");
        warning->setFixedSize(400,28);
        warning->move(75,210);
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


    } else {
        qDebug() << "不包含credentials已加密";

        QDialog *enterPassDialog=new QDialog;
        enterPassDialog->setWindowTitle("密码本");
        enterPassDialog->setFixedSize(550,303);
        QLineEdit *confirmPassword=new QLineEdit(enterPassDialog);
        confirmPassword->setFixedSize(240,31);
        confirmPassword->move(155,118);
        confirmPassword->setPlaceholderText("请输入密码");

        QPushButton *confirmButton = new QPushButton("确认",enterPassDialog);
        confirmButton->setFixedSize(100, 31);
        confirmButton->move(225,242);

        QObject::connect(confirmButton, &QPushButton::clicked, [=]{
            QByteArray ba = QCryptographicHash::hash(confirmPassword->text().toUtf8(), QCryptographicHash::Md5);
            QString password = QString(ba.toHex().left(16));

            QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB, QAESEncryption::PKCS7);
            QByteArray deBA = encryption.decode(jsonData, password.toUtf8());
           QByteArray newJsonData = QAESEncryption::RemovePadding(deBA, QAESEncryption::PKCS7);
           QJsonDocument newDoc=QJsonDocument::fromJson(newJsonData);
           QJsonObject newJsonObj = newDoc.object();
           qDebug()<<deBA;
            if (newJsonObj.contains("credentials")) {
                m_password = QString(ba.toHex().left(16));
                enterPassDialog->hide();
            } else {
                QMessageBox::warning(enterPassDialog, "提示", "密码错误！");

            }

        });
        enterPassDialog->exec();

    }

}

QString passbookPassword::getPassword()
{

    return m_password;
}
