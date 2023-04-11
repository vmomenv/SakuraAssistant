#include "passbook.h"
#include"aesencrypt.h"
#include<DWidget>
#include<DTitlebar>
#include<DMainWindow>
#include<QVBoxLayout>
#include<QFormLayout>
PassBook::PassBook(DWidget *parent)
{
    isUpdating=false;
    setFixedSize(861,490);
    titlebar()->setFixedHeight(0);
    setFocusPolicy(Qt::ClickFocus);
    installEventFilter(this);//安装事件过滤器

    //设置窗口透明
    setAttribute(Qt::WA_TranslucentBackground);
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
    searchPicLabel->setPixmap(searchPixmap);


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



    // 创建添加密码按钮
    LabelButton *addPassLabel = new LabelButton(passWidget);



    QPixmap addPixmap(":/res/plus-circle.png");
    addPassLabel->setPixmap(addPixmap);
    addPassLabel->move(0,295);


    QWidget *allCredentialsWidget=new QWidget(passWidget);
    allCredentialsWidget->resize(816,266);
    allCredentialsWidget->move(0,30);


    QWidget *credentialWidget=new QWidget(allCredentialsWidget);
    credentialWidget->resize(816,53);
    QLabel *targetNameLabel = new QLabel(passWidget);
    targetNameLabel->setText("目标选项");
    targetNameLabel->setFont(QFont("Arial", 15));
    targetNameLabel->setAlignment(Qt::AlignCenter);
    targetNameLabel->setStyleSheet("color: black;");
    targetNameLabel->move(0,0);

    QLabel *usernameLabel = new QLabel(passWidget);
    usernameLabel->setText("账号");
    usernameLabel->setFont(QFont("Arial", 15));
    usernameLabel->setAlignment(Qt::AlignCenter);
    usernameLabel->setStyleSheet("color: black;");
    usernameLabel->move(305,0);

    QLabel *passwordLabel = new QLabel(passWidget);
    passwordLabel->setText("密码");
    passwordLabel->setFont(QFont("Arial", 15));
    passwordLabel->setAlignment(Qt::AlignCenter);
    passwordLabel->setStyleSheet("color: black;");
    passwordLabel->move(515,0);

    // 创建 QLineEdit 控件
    QLineEdit *targetNameLineEdit = new QLineEdit(credentialWidget);
    targetNameLineEdit->setFont(QFont("Arial", 13));
    targetNameLineEdit->setAlignment(Qt::AlignCenter);
    targetNameLineEdit->setStyleSheet("background-color: #D9D9D9;border-radius: 6px;");
    targetNameLineEdit->setFixedSize(286, 40);
    targetNameLineEdit->move(0,0);

    QLineEdit *usernameLineEdit = new QLineEdit(credentialWidget);
    usernameLineEdit->setFont(QFont("Arial", 13));
    usernameLineEdit->setAlignment(Qt::AlignCenter);
    usernameLineEdit->setStyleSheet("background-color: #D9D9D9;border-radius: 6px;");
    usernameLineEdit->setFixedSize(192, 40);
    usernameLineEdit->move(305,0);

    QLineEdit *passwordLineEdit = new QLineEdit(credentialWidget);
    passwordLineEdit->setFont(QFont("Arial", 13));
    passwordLineEdit->setAlignment(Qt::AlignCenter);
    passwordLineEdit->setStyleSheet("background-color: #D9D9D9;border-radius: 6px;");
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordLineEdit->setFixedSize(256, 40);
    passwordLineEdit->move(515,0);

    LabelButton *delButton=new LabelButton(credentialWidget);
    QPixmap delPix(":/res/passbook/delete.png");
    delButton->setPixmap(delPix);
    delButton->setFixedSize(38,38);
    delButton->move(779,0);
    readJson();


//    connect(addPassLabel, &LabelButton::clicked, this, [=](){
//        QString targetName = targetNameLineEdit->text();
//        QString username = usernameLineEdit->text();
//        QString password = passwordLineEdit->text();
//        writeJson(targetName, username, password);
//    });
}
bool PassBook::eventFilter(QObject *watched, QEvent *event)//失焦关闭窗口
{
    if (Q_NULLPTR == watched)
    {
        return false;
    }
    if (QEvent::ActivationChange == event->type())
    {
        if (QApplication::activeWindow() != this)
        {
            if(isUpdating==false){
                this->close();
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}

void PassBook::readJson()
{
    QFile file(":/res/passbook/data.json");
    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << "密码本文件占用";
        return;
    }
    QByteArray jsonData = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(jsonData));
    QJsonObject json = doc.object();
    qDebug() << QJsonDocument(json).toJson(QJsonDocument::Indented);//.toJson(QJsonDocument::Indented) 方法可以将 JSON 对象转换为具有缩进格式的字符串。
//    if(jsonData.size()!=0){
//        for(int i=0;i<this->)
//    }
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
}

// 创建一个新的QWidget并添加到credentialsWidget中
void PassBook::addCredentialsWidget(const QString& targetName, const QString& username, const QString& password)
{
    // 创建密码详情控件
    displayCredentials(targetName, username, password);

    // 创建删除密码按钮
    LabelButton *deletePassLabel = new LabelButton(passWidget);
    QPixmap deletePixmap(":/res/delete-circle.png");
    deletePassLabel->setPixmap(deletePixmap);
    deletePassLabel->move(790, passWidget->height() - 35);

    // 连接删除密码按钮的槽函数
    connect(deletePassLabel, &LabelButton::clicked, [=](){
        // 删除密码详情控件和删除密码按钮
        delete deletePassLabel;
        delete passWidget->layout()->takeAt(passWidget->layout()->count()-1)->widget();
        delete passWidget->layout()->takeAt(passWidget->layout()->count()-1)->widget();
        delete passWidget->layout()->takeAt(passWidget->layout()->count()-1)->widget();
        delete passWidget->layout()->takeAt(passWidget->layout()->count()-1)->widget();
    });
}

void PassBook::displayCredentials(const QString& targetName, const QString& username, const QString& password)
{



}
