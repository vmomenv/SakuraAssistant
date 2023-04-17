#include "passbookpassword.h"

passbookPassword::passbookPassword()
{
    QString *password=new QString;
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
        QWidget *enterPassWidget=new QWidget;
        enterPassWidget->setWindowTitle("密码本");
        enterPassWidget->setFixedSize(550,303);
        QLineEdit *setPassword=new QLineEdit(enterPassWidget);
        QLineEdit *confirmPassword=new QLineEdit(enterPassWidget);
        setPassword->setFixedSize(240,31);
        confirmPassword->setFixedSize(240,31);
        setPassword->move(155,100);
        confirmPassword->move(155,174);
        QPushButton *confirmButton = new QPushButton("确认",enterPassWidget);
        confirmButton->setFixedSize(100, 31);
        confirmButton->move(225,242);
        setPassword->setPlaceholderText("请设置密码");
        confirmPassword->setPlaceholderText("请再次输入密码");
        QLabel *warning=new QLabel(enterPassWidget);
        warning->setText("密码用于验证进入密码本并加解密数据，若忘记密码无法找回！");
        warning->setFixedSize(400,28);
        warning->move(75,210);
        QObject::connect(confirmButton, &QPushButton::clicked, [=]{
            if (setPassword->text() == confirmPassword->text()) {
            // 密码验证通过，关闭窗口
                QByteArray ba = QCryptographicHash::hash(confirmPassword->text().toUtf8(), QCryptographicHash::Md5);
                QString a=QString(ba.toHex().left(16));
                *password=a;
                qDebug()<<*password;
                enterPassWidget->hide();

            } else {
            // 密码不一致，弹出提示框
            QMessageBox::warning(enterPassWidget, "提示", "两次输入的密码不一致！");
            }
        });
        enterPassWidget->show();


    } else {
        qDebug() << "不包含credentials已加密";

        QWidget *enterPassWidget=new QWidget;
        enterPassWidget->setWindowTitle("密码本");
        enterPassWidget->setFixedSize(550,303);
        QLineEdit *confirmPassword=new QLineEdit(enterPassWidget);
        confirmPassword->setFixedSize(240,31);
        confirmPassword->move(155,118);
        confirmPassword->setPlaceholderText("请输入密码");

        QPushButton *confirmButton = new QPushButton("确认",enterPassWidget);
        confirmButton->setFixedSize(100, 31);
        confirmButton->move(225,242);

        QObject::connect(confirmButton, &QPushButton::clicked, [=]{
            QByteArray ba = QCryptographicHash::hash(confirmPassword->text().toUtf8(), QCryptographicHash::Md5);
//            qDebug()<<QJsonDocument::fromJson(PassBook::decryptJsonFile(jsonData,QString(ba.toHex().left(16))));
            if (jsonObj.contains("credentials")) {
                *password = QString(ba.toHex().left(16));
            } else {
                QMessageBox::warning(enterPassWidget, "提示", "密码错误！");

            }

        });
        enterPassWidget->show();

    }

}
