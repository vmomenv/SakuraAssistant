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
    //创建左上角key的装饰图标
    //创建置顶按钮
    QLabel *keyLabel=new QLabel(titleWidget);
    keyLabel->setPixmap(QPixmap(":/res/passbook/key.png"));
    keyLabel->move(22,7);
    QPushButton *setTopButton = new QPushButton(titleWidget);
    setTopButton->setFixedSize(29,29);
    setTopButton->move(809,12);
    setTopButton->setIcon(QIcon(":/res/passbook/unPin.png"));
    setTopButton->setIconSize(QSize(31,31));
    setTopButton->setStyleSheet("border:none; background-color:transparent;");
    connect(setTopButton, &QPushButton::clicked, [=] {
    if (isUpdating == false) {
        setTopButton->setIcon(QIcon(":/res/passbook/pin.png"));
        this->isUpdating=true;
        setWindowFlag(Qt::WindowStaysOnTopHint);
        this->show();


    } else {
        setTopButton->setIcon(QIcon(":/res/passbook/unPin.png"));
        this->isUpdating=false;
    }
    });


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


    //文字
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


    int y = 0;//高，显示主界面
    QScrollArea *scrollArea = new QScrollArea(passWidget);
    scrollArea->setFixedSize(836, 266);
    scrollArea->move(0,32);
    scrollArea->setWidgetResizable(true);
    QWidget *allCredentialsWidget=new QWidget(scrollArea);
    allCredentialsWidget->setFixedSize(816, 290);
    scrollArea->setWidget(allCredentialsWidget);


    //循环遍历json并创建多个qwidget
    QFile file(":/res/passbook/data.json");
    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << "密码本文件占用";
        return;
    }
    QByteArray jsonData = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(jsonData));
    QJsonObject jsonObj = doc.object();
    QJsonArray credentialsArray = jsonObj["credentials"].toArray();
    if(credentialsArray.size()!=0){
        for(int i=0;i<credentialsArray.size();i++){
            QJsonObject credentialObj = credentialsArray[i].toObject();
            QString targetName = credentialObj["targetName"].toString();
            QString username = credentialObj["username"].toString();
            QString password = credentialObj["password"].toString();
            qDebug()<<targetName<<username<<password;

            // 创建 QLineEdit 控件
            QWidget *credentialWidget=new QWidget(allCredentialsWidget);
            credentialWidget->resize(816,53);


            QLineEdit *targetNameLineEdit = new QLineEdit(credentialWidget);
            targetNameLineEdit->setFont(QFont("Arial", 13));
            targetNameLineEdit->setAlignment(Qt::AlignCenter);
            targetNameLineEdit->setStyleSheet("background-color: #C3C9D3;border-radius: 6px;");
            targetNameLineEdit->setFixedSize(286, 40);
            targetNameLineEdit->setText(targetName);
            targetNameLineEdit->move(0,0);

            QLineEdit *usernameLineEdit = new QLineEdit(credentialWidget);
            usernameLineEdit->setFont(QFont("Arial", 13));
            usernameLineEdit->setAlignment(Qt::AlignCenter);
            usernameLineEdit->setStyleSheet("background-color: #C3C9D3;border-radius: 6px;");
            usernameLineEdit->setFixedSize(192, 40);
            usernameLineEdit->setText(username);
            usernameLineEdit->move(305,0);

            QLineEdit *passwordLineEdit = new QLineEdit(credentialWidget);
            passwordLineEdit->setFont(QFont("Arial", 13));
            passwordLineEdit->setAlignment(Qt::AlignCenter);
            passwordLineEdit->setStyleSheet("background-color: #C3C9D3;border-radius: 6px;");
            passwordLineEdit->setEchoMode(QLineEdit::Password);
            passwordLineEdit->setFixedSize(256, 40);
            passwordLineEdit->setTextMargins(40, 0, 40, 0);
            passwordLineEdit->setText(password);
            passwordLineEdit->move(515,0);

            // 创建QPushButton控件用于切换明文和暗文
            QPushButton *showPasswordButton = new QPushButton(credentialWidget);
            showPasswordButton->setFixedSize(31, 31);
            showPasswordButton->move(520, 5);
            showPasswordButton->setIcon(QIcon(":/res/passbook/showPass.png"));
            showPasswordButton->setIconSize(QSize(31,31));
            showPasswordButton->setStyleSheet("border:none; background-color:transparent;");

            // 创建QPushButton控件用于复制内容
            QPushButton *copyButton = new QPushButton(credentialWidget);
            copyButton->setFixedSize(30, 30);
            copyButton->move(738, 5);
            copyButton->setIcon(QIcon(":/res/passbook/copy.png"));
            copyButton->setIconSize(QSize(30, 30));
            copyButton->setStyleSheet("border:none; background-color:transparent;");
            //创建删除按钮
            LabelButton *delButton=new LabelButton(credentialWidget);
            QPixmap delPix(":/res/passbook/delete.png");
            delButton->setPixmap(delPix);
            delButton->setFixedSize(38,38);
            delButton->move(779,0);

            // 连接showPasswordButton的点击事件
            connect(showPasswordButton, &QPushButton::clicked, [=] {
            if (passwordLineEdit->echoMode() == QLineEdit::Password) {

            passwordLineEdit->setEchoMode(QLineEdit::Normal);
            } else {
            passwordLineEdit->setEchoMode(QLineEdit::Password);
            }
            });

            // 连接copyButton的点击事件
            connect(copyButton, &QPushButton::clicked, [=] {
            QApplication::clipboard()->setText(passwordLineEdit->text());
            });

            credentialWidget->move(0, y);
            y += 50;

        }
    }


//    readJson();


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
            if(this->isUpdating==false){
                this->hide();
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}




