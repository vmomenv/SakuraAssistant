#include "passbook.h"
#include"aesencrypt.h"
#include<DWidget>
#include<DTitlebar>
#include<DMainWindow>
#include<QVBoxLayout>
#include<QFormLayout>
PassBook::PassBook(DWidget *parent)
{
    setFixedSize(861,490);
    titlebar()->setFixedHeight(0);
    setFocusPolicy(Qt::ClickFocus);
    installEventFilter(this);//安装事件过滤器

    //设置窗口透明
    setAttribute(Qt::WA_TranslucentBackground);
//    setWindowFlags(Qt::FramelessWindowHint);
    QPalette palette = QPalette();
        palette.setColor(QPalette::Background, QColor(0x00,0xFF,0x00,0x00));
    setPalette(palette);
    setEnableBlurWindow(true);//设置高斯模糊

    this->move(552,342);
    QWidget *titleWidget=new QWidget(this);
    titleWidget->resize(861,53);
    QPalette pal = titleWidget->palette();//设置widget背景色
    pal.setColor(QPalette::Background, QColor(49,53,62, 255));
    titleWidget->setPalette(pal);
    titleWidget->setAutoFillBackground(true);
    titleWidget->move(0,0);

    //创建主widget


    //创建搜索框widget并且将控件放入该widget
    QWidget *searchWidget = new QWidget(this);
    QHBoxLayout *searchLayout = new QHBoxLayout(searchWidget);
    QLabel *searchPicLabel=new QLabel(searchWidget);
    QPixmap searchPixmap(":/res/passbook/search.png");



    QLineEdit *searchEdit = new QLineEdit(searchWidget);
    searchLayout->addWidget(searchPicLabel);
    searchLayout->addWidget(searchEdit);
    searchWidget->setFixedSize(816, 53);
    searchWidget->setStyleSheet("background-color: #C3C9D3;border-radius: 12px;");//设置圆角
    searchWidget->move(22, 69);



    //密码详情
    QWidget *passWidget=new QWidget(this);
    passWidget->move(22,142);
    passWidget->resize(816,325);

    QLabel *targetNameLabel = new QLabel(passWidget);
    targetNameLabel->setText("目标选项");
    targetNameLabel->setFont(QFont("Arial", 15));
    targetNameLabel->setAlignment(Qt::AlignCenter);
    targetNameLabel->setStyleSheet("color: black;");

    QLabel *usernameLabel = new QLabel(passWidget);
    usernameLabel->setText("账号");
    usernameLabel->setFont(QFont("Arial", 15));
    usernameLabel->setAlignment(Qt::AlignCenter);
    usernameLabel->setStyleSheet("color: black;");
    usernameLabel->move(326,0);

    QLabel *passwordLabel = new QLabel(passWidget);
    passwordLabel->setText("密码");
    passwordLabel->setFont(QFont("Arial", 15));
    passwordLabel->setAlignment(Qt::AlignCenter);
    passwordLabel->setStyleSheet("color: black;");
    passwordLabel->move(536,0);

    // 创建添加密码按钮
//    LabelButton *addPassLabel;
//    QPixmap addPixmap(":/res/plus-circle.png");
//    addPassLabel->setPixmap(addPixmap);
//    addPassLabel->move(0,295);

//    connect(addPassLabel, &LabelButton::clicked, this, [=](){
//        QString targetName = targetNameLineEdit->text();
//        QString username = usernameLineEdit->text();
//        QString password = passwordLineEdit->text();
//        writeJson(targetName, username, password);
//    });
    readJson();


}

void PassBook::readJson()
{
    QFile file(":/res/passbook/data.json");
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"密码本文件占用";
        return;
    }
    QByteArray jsonData=file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(jsonData));
    QJsonArray jsonArray=doc.array();
    for(int i=0;i<jsonArray.size();i++){
        QJsonObject obj = jsonArray.at(i).toObject();
        QString targetName = obj.value("targetName").toString();
        QString username = obj.value("username").toString();
        QString password = obj.value("password").toString();

        // 在界面上显示读取到的数据
        addCredentialsWidget(targetName, username, password);
    }
    file.close();
}
void PassBook::writeJson(QString targetName, QString username, QString password)
{
    QFile file("data.json");
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qDebug() << "Open json file failed.";
        return;
    }

    QByteArray jsonData = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(jsonData));
    QJsonArray jsonArray = doc.array();

    QJsonObject obj;
    obj.insert("targetName", targetName);
    obj.insert("username", username);
    obj.insert("password", password);
    jsonArray.append(obj);

    doc.setArray(jsonArray);
    file.resize(0);
    file.write(doc.toJson());
    file.close();

    // 在界面上添加新的数据
    addCredentialsWidget(targetName, username, password);
}

// 创建一个新的QWidget并添加到credentialsWidget中
void PassBook::addCredentialsWidget(QString targetName, QString username, QString password)
{
//    // 创建输入框和标签
//    QWidget *credentialsWidget = new QWidget(passWidget);
//    QVBoxLayout *credentialsLayout = new QVBoxLayout(credentialsWidget);

//    QHBoxLayout *inputLayout = new QHBoxLayout;
//    QLineEdit *targetNameLineEdit = new QLineEdit;
//    targetNameLineEdit->setFixedSize(286, 34);
//    QLineEdit *usernameLineEdit = new QLineEdit;
//    usernameLineEdit->setFixedSize(192, 34);
//    QLineEdit *passwordLineEdit = new QLineEdit;
//    passwordLineEdit->setFixedSize(256, 34);
//    passwordLineEdit->setEchoMode(QLineEdit::Password); // 设置密码模式
//    inputLayout->addWidget(targetNameLineEdit);
//    inputLayout->addWidget(usernameLineEdit);
//    inputLayout->addWidget(passwordLineEdit);

//    credentialsLayout->addLayout(inputLayout);

    QWidget *credentialWidget = new QWidget(credentialsWidget);
    QVBoxLayout *credentialLayout = new QVBoxLayout(credentialWidget);

    QLabel *targetNameLabel = new QLabel(credentialWidget);
    targetNameLabel->setText(targetName);
    targetNameLabel->setFont(QFont("Arial", 15));
    targetNameLabel->setAlignment(Qt::AlignCenter);
    targetNameLabel->setStyleSheet("color: black;");

    QLabel *usernameLabel = new QLabel(credentialWidget);
    usernameLabel->setText(username);
    usernameLabel->setFont(QFont("Arial", 15));
    usernameLabel->setAlignment(Qt::AlignCenter);
    usernameLabel->setStyleSheet("color: black;");

    QLabel *passwordLabel = new QLabel(credentialWidget);
    passwordLabel->setText(password);
    passwordLabel->setFont(QFont("Arial", 15));
    passwordLabel->setAlignment(Qt::AlignCenter);
    passwordLabel->setStyleSheet("color: black;");

    credentialLayout->addWidget(targetNameLabel);
    credentialLayout->addWidget(usernameLabel);
    credentialLayout->addWidget(passwordLabel);

    credentialsLayout->addWidget(credentialWidget);
}
