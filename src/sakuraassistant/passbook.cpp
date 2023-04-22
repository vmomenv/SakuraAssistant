#include "passbook.h"
#include"aesencrypt.h"
#include<DWidget>
#include<DTitlebar>
#include<DMainWindow>
#include<QVBoxLayout>
#include<QFormLayout>
PassBook::PassBook(QString accountPassword, DWidget *parent): m_accountPassword(accountPassword)
{

    isUpdating=false;
    setFixedSize(861,490);
    titlebar()->setFixedHeight(0);
    setFocusPolicy(Qt::ClickFocus);
    installEventFilter(this);//安装事件过滤器

//    //设置窗口透明
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
    passWidget->resize(830,330);



    // 创建添加密码按钮
    QPushButton *addPassButton = new QPushButton(passWidget);
    QPixmap addPixmap(":/res/plus-circle.png");
    addPassButton->setIcon(addPixmap);
    addPassButton->setIconSize(QSize(31,31));
    addPassButton->setStyleSheet("border:none; background-color:transparent;");
    addPassButton->move(0,301);
    //创建导入导出按钮
    QPushButton *importButton =new QPushButton(passWidget);
    QPixmap importPixmap(":/res/passbook/import.png");
    importButton->setIcon(importPixmap);
    importButton->setIconSize(QSize(31,31));
    importButton->setStyleSheet("border:none; background-color:transparent;");
    importButton->move(739,301);

    QPushButton *exportButton =new QPushButton(passWidget);
    QPixmap exportPixmap(":/res/passbook/export.png");
    exportButton->setIcon(exportPixmap);
    exportButton->setIconSize(QSize(31,31));
    exportButton->setStyleSheet("border:none; background-color:transparent;");
    exportButton->move(787,301);


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
    scrollArea->setFixedSize(830, 266);
    scrollArea->move(0,32);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);


    QWidget *allCredentialsWidget=new QWidget(scrollArea);
    QVBoxLayout *allCredentialsLayout=new QVBoxLayout(allCredentialsWidget);//设置样式
    scrollArea->setWidget(allCredentialsWidget);//设置滚动
    scrollArea->setWidgetResizable(true);


    // 初始化布局
    QWidget *credentialWidget=new QWidget(allCredentialsWidget);
    credentialWidget->resize(816,53);
    credentialWidget->deleteLater();
    credentialWidget->setParent(nullptr);

    //创建文件路径
    QDir home = QDir::home();
    QString dataPath = home.filePath(".config/sakuraassistant");
    QDir dir(dataPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
//    //转储文件
//    QString tmpDataFile = dataPath+ "/tmpdata.json";
//    QFile tmp(tmpDataFile);
//    if (!tmp.exists()) {
//        qDebug() << "tmp.json文件不存在";
//    }
    //打开文件
    QString path = dir.filePath("data.json");
    QFile file;
    file.setFileName(path);

//    if (file.exists() && !file.remove()) {
//        qDebug() << "无法删除data.json文件";
//    }

//    // 将tmp.json文件移动到data.json
//    if (!tmp.copy(path)|| !tmp.remove()) {
//        qDebug() << "无法将tmp.json文件移动到data.json";
//    }
//    if (file.exists() && !file.remove()) {
//        qDebug() << "无法删除data.json文件";
//    }
    //将qt中文件复制到~/.config/sparkassistant/
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
    QByteArray jsonData = file.readAll();
    doc=QJsonDocument::fromJson(jsonData);
    QJsonObject jsonObj = doc.object();
    if (jsonObj.contains("credentials")) {
        qDebug() << "包含credentials字段，未加密";
    } else {
        qDebug() << "不包含credentials已加密";
        doc=QJsonDocument::fromJson(decryptJsonFile(jsonData,accountPassword));
        jsonObj = doc.object();
        qDebug()<<"jsonobj"<<jsonObj;
    }
    qDebug()<<"jsonobj"<<jsonObj;

    QJsonArray credentialsArray = jsonObj["credentials"].toArray();
    qDebug()<<"显示数据";

    if(credentialsArray.size()!=0){
        for(int i=0;i<credentialsArray.size();i++){
            QJsonObject credentialObj = credentialsArray[i].toObject();
            QString targetName = credentialObj["targetName"].toString();
            QString username = credentialObj["username"].toString();
            QString password = credentialObj["password"].toString();
            qDebug()<<targetName<<username<<password;
            if(credentialObj.value("isDel").toBool()==true){
                continue;
            }

            // 创建 QLineEdit 控件
            QWidget *credentialWidget=new QWidget(allCredentialsWidget);
            credentialWidget->setFixedSize(816,53);


            QLineEdit *targetNameLineEdit = new QLineEdit(credentialWidget);
            targetNameLineEdit->setFont(QFont("Arial", 13));
            targetNameLineEdit->setAlignment(Qt::AlignCenter);
            targetNameLineEdit->setStyleSheet("background-color: #C3C9D3;border-radius: 6px;");
            targetNameLineEdit->setFixedSize(286, 40);
            targetNameLineEdit->setText(targetName);
            targetNameLineEdit->setPlaceholderText("网址/app名称/服务器地址");
            targetNameLineEdit->move(0,0);

            QLineEdit *usernameLineEdit = new QLineEdit(credentialWidget);
            usernameLineEdit->setFont(QFont("Arial", 13));
            usernameLineEdit->setAlignment(Qt::AlignCenter);
            usernameLineEdit->setStyleSheet("background-color: #C3C9D3;border-radius: 6px;");
            usernameLineEdit->setFixedSize(192, 40);
            usernameLineEdit->setText(username);
            usernameLineEdit->setPlaceholderText("请输入您的账户");
            usernameLineEdit->move(305,0);

            QLineEdit *passwordLineEdit = new QLineEdit(credentialWidget);
            passwordLineEdit->setFont(QFont("Arial", 13));
            passwordLineEdit->setAlignment(Qt::AlignCenter);
            passwordLineEdit->setStyleSheet("background-color: #C3C9D3;border-radius: 6px;");
            passwordLineEdit->setEchoMode(QLineEdit::Password);
            passwordLineEdit->setFixedSize(256, 40);
            passwordLineEdit->setTextMargins(40, 0, 40, 0);
            passwordLineEdit->setText(password);
            passwordLineEdit->setPlaceholderText("请输入您的密码");

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
            QPushButton *delButton=new QPushButton(credentialWidget);
            QPixmap delPix(":/res/passbook/delete.png");
            delButton->setIcon(delPix);
            delButton->setFixedSize(38,38);
            delButton->move(779,0);
            delButton->setIconSize(QSize(31,31));
            delButton->setStyleSheet("border:none; background-color:transparent;");


            //对一条内容进行布局
            allCredentialsLayout->addWidget(credentialWidget);
            // 连接showPasswordButton的点击事件
            connect(showPasswordButton, &QPushButton::clicked, [=] {
                if(passwordLineEdit->text().isEmpty()){
                    passwordLineEdit->setText(generatePassword());
                }
            if (passwordLineEdit->echoMode() == QLineEdit::Password) {

            passwordLineEdit->setEchoMode(QLineEdit::Normal);
            } else {
            passwordLineEdit->setEchoMode(QLineEdit::Password);
            }
            });
            //修改完成进行保存
            connect(targetNameLineEdit, &QLineEdit::editingFinished, this, [=](){
                this->saveToJsonFile(targetNameLineEdit->text(),usernameLineEdit->text(),passwordLineEdit->text(),i,false,false);
            });
            connect(usernameLineEdit, &QLineEdit::editingFinished, this, [=](){
                this->saveToJsonFile(targetNameLineEdit->text(),usernameLineEdit->text(),passwordLineEdit->text(),i,false,false);

            });
            connect(passwordLineEdit, &QLineEdit::editingFinished, this, [=](){
                this->saveToJsonFile(targetNameLineEdit->text(),usernameLineEdit->text(),passwordLineEdit->text(),i,false,false);

            });



            // 连接copyButton的点击事件
            connect(copyButton, &QPushButton::clicked, [=] {
            QApplication::clipboard()->setText(passwordLineEdit->text());
            });

            connect(delButton ,&QPushButton::clicked, this, [=](){
                credentialWidget->setParent(nullptr);
                qDebug()<<"正在删除第"<<i<<"组件";
                this->saveToJsonFile(targetNameLineEdit->text(),usernameLineEdit->text(),passwordLineEdit->text(),i,true,false);

            });
        }
    }
    static int index=0;
    index=credentialsArray.size();
    connect(addPassButton ,&QPushButton::clicked, this, [=](){
        int *addIndex=new int();
        *addIndex=index;
        qDebug()<<index;
        QWidget *credentialWidget=new QWidget(allCredentialsWidget);
        credentialWidget->setFixedSize(816,53);
        saveToJsonFile("","","",*addIndex,false,true);//新增一条
        index++;
        QLineEdit *targetNameLineEdit = new QLineEdit(credentialWidget);
        targetNameLineEdit->setFont(QFont("Arial", 13));
        targetNameLineEdit->setAlignment(Qt::AlignCenter);
        targetNameLineEdit->setStyleSheet("background-color: #C3C9D3;border-radius: 6px;");
        targetNameLineEdit->setFixedSize(286, 40);
        targetNameLineEdit->setPlaceholderText("网址/app名称/服务器地址");
        targetNameLineEdit->move(0,0);

        QLineEdit *usernameLineEdit = new QLineEdit(credentialWidget);
        usernameLineEdit->setFont(QFont("Arial", 13));
        usernameLineEdit->setAlignment(Qt::AlignCenter);
        usernameLineEdit->setStyleSheet("background-color: #C3C9D3;border-radius: 6px;");
        usernameLineEdit->setFixedSize(192, 40);
        usernameLineEdit->setPlaceholderText("请输入您的账户");
        usernameLineEdit->move(305,0);

        QLineEdit *passwordLineEdit = new QLineEdit(credentialWidget);
        passwordLineEdit->setFont(QFont("Arial", 13));
        passwordLineEdit->setAlignment(Qt::AlignCenter);
        passwordLineEdit->setStyleSheet("background-color: #C3C9D3;border-radius: 6px;");
        passwordLineEdit->setEchoMode(QLineEdit::Password);
        passwordLineEdit->setFixedSize(256, 40);
        passwordLineEdit->setTextMargins(40, 0, 40, 0);
        passwordLineEdit->setPlaceholderText("请输入您的密码");
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
        QPushButton *delButton=new QPushButton(credentialWidget);
        QPixmap delPix(":/res/passbook/delete.png");
        delButton->setIcon(delPix);
        delButton->setFixedSize(38,38);
        delButton->move(779,0);
        delButton->setIconSize(QSize(31,31));
        delButton->setStyleSheet("border:none; background-color:transparent;");
        //将credentialswidget插入布局
        allCredentialsLayout->insertWidget(allCredentialsLayout->count()-1,credentialWidget);


        // 连接showPasswordButton的点击事件
        connect(showPasswordButton, &QPushButton::clicked, [=] {
            if(passwordLineEdit->text().isEmpty()){
                passwordLineEdit->setText(generatePassword());
            }
        if (passwordLineEdit->echoMode() == QLineEdit::Password) {

        passwordLineEdit->setEchoMode(QLineEdit::Normal);
        } else {
        passwordLineEdit->setEchoMode(QLineEdit::Password);
        }
        });



        //修改完成进行保存
        connect(targetNameLineEdit, &QLineEdit::editingFinished, this, [=](){
            this->saveToJsonFile(targetNameLineEdit->text(),usernameLineEdit->text(),passwordLineEdit->text(),*addIndex,false,false);
        });
        connect(usernameLineEdit, &QLineEdit::editingFinished, this, [=](){
            this->saveToJsonFile(targetNameLineEdit->text(),usernameLineEdit->text(),passwordLineEdit->text(),*addIndex,false,false);

        });
        connect(passwordLineEdit, &QLineEdit::editingFinished, this, [=](){
            this->saveToJsonFile(targetNameLineEdit->text(),usernameLineEdit->text(),passwordLineEdit->text(),*addIndex,false,false);

        });



        // 连接copyButton的点击事件
        connect(copyButton, &QPushButton::clicked, [=] {
        QApplication::clipboard()->setText(passwordLineEdit->text());
        });

        connect(delButton ,&QPushButton::clicked, this, [=](){
            credentialWidget->setParent(nullptr);
            this->saveToJsonFile(targetNameLineEdit->text(),usernameLineEdit->text(),passwordLineEdit->text(),*addIndex,true,false);

        });


    });


    //设置密码条目底部弹簧
    QSpacerItem* passbookSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    //将弹簧加入布局
    allCredentialsLayout->addItem(passbookSpacer);
    allCredentialsWidget->setLayout(allCredentialsLayout);


    connect(searchEdit, &QLineEdit::returnPressed, this,[=]{
        disconnect(addPassButton ,&QPushButton::clicked, this, nullptr);

        QJsonArray search_resultArray;//放置搜索出来的json数据
        QString searchKeyword = searchEdit->text().toLower();
        QJsonObject searchJsonObj = doc.object();
        QJsonArray search_credentialsArray = searchJsonObj["credentials"].toArray();
        for (int i=0;i<search_credentialsArray.size();i++) {
           QJsonObject search_credentialObj = search_credentialsArray[i].toObject();
           QString targetName = search_credentialObj["targetName"].toString().toLower();
           if (targetName.contains(searchKeyword)) {
//            将符合搜索条件的凭据添加到新JSON对象中
           QJsonObject newCredential;
           if(search_credentialObj.value("isDel").toBool()==true){
               continue;
           }
           newCredential["targetName"] = search_credentialObj["targetName"].toString();
           newCredential["username"] = search_credentialObj["username"].toString();
           newCredential["password"] = search_credentialObj["password"].toString();
           newCredential["index"]=i;
           search_resultArray.append(newCredential);
          }
        }
        qDebug()<<"new"<<search_resultArray;
        // 将结果转换为标准的JSON格式

        QWidget *allCredentialsWidget=new QWidget(scrollArea);
        QVBoxLayout *allCredentialsLayout=new QVBoxLayout(allCredentialsWidget);//设置样式
        scrollArea->setWidget(allCredentialsWidget);//设置滚动
        // 初始化布局
        QWidget *credentialWidget=new QWidget(allCredentialsWidget);
        credentialWidget->resize(816,53);
        credentialWidget->deleteLater();
        credentialWidget->setParent(nullptr);
        if(search_resultArray.size()!=0){
            for(int i=0;i<search_resultArray.size();i++){
                QJsonObject search_credentialObj = search_resultArray[i].toObject();
                QString search_targetName = search_credentialObj["targetName"].toString();
                QString search_username = search_credentialObj["username"].toString();
                QString search_password = search_credentialObj["password"].toString();
                int search_index=search_credentialObj["index"].toInt();
                qDebug()<<search_targetName<<search_username;
                if(search_credentialObj.value("isDel").toBool()==true){
                    continue;
                }

                // 创建 QLineEdit 控件
                QWidget *credentialWidget=new QWidget(allCredentialsWidget);
                credentialWidget->setFixedSize(816,53);


                QLineEdit *targetNameLineEdit = new QLineEdit(credentialWidget);
                targetNameLineEdit->setFont(QFont("Arial", 13));
                targetNameLineEdit->setAlignment(Qt::AlignCenter);
                targetNameLineEdit->setStyleSheet("background-color: #C3C9D3;border-radius: 6px;");
                targetNameLineEdit->setFixedSize(286, 40);
                targetNameLineEdit->setText(search_targetName);
                targetNameLineEdit->setPlaceholderText("网址/app名称/服务器地址");
                targetNameLineEdit->move(0,0);

                QLineEdit *usernameLineEdit = new QLineEdit(credentialWidget);
                usernameLineEdit->setFont(QFont("Arial", 13));
                usernameLineEdit->setAlignment(Qt::AlignCenter);
                usernameLineEdit->setStyleSheet("background-color: #C3C9D3;border-radius: 6px;");
                usernameLineEdit->setFixedSize(192, 40);
                usernameLineEdit->setText(search_username);
                usernameLineEdit->setPlaceholderText("请输入您的账户");
                usernameLineEdit->move(305,0);

                QLineEdit *passwordLineEdit = new QLineEdit(credentialWidget);
                passwordLineEdit->setFont(QFont("Arial", 13));
                passwordLineEdit->setAlignment(Qt::AlignCenter);
                passwordLineEdit->setStyleSheet("background-color: #C3C9D3;border-radius: 6px;");
                passwordLineEdit->setEchoMode(QLineEdit::Password);
                passwordLineEdit->setFixedSize(256, 40);
                passwordLineEdit->setTextMargins(40, 0, 40, 0);
                passwordLineEdit->setText(search_password);
                passwordLineEdit->setPlaceholderText("请输入您的密码");
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
                QPushButton *delButton=new QPushButton(credentialWidget);
                QPixmap delPix(":/res/passbook/delete.png");
                delButton->setIcon(delPix);
                delButton->setFixedSize(38,38);
                delButton->move(779,0);
                delButton->setIconSize(QSize(31,31));
                delButton->setStyleSheet("border:none; background-color:transparent;");


                //对一条内容进行布局
                allCredentialsLayout->addWidget(credentialWidget);

                // 连接showPasswordButton的点击事件
                connect(showPasswordButton, &QPushButton::clicked, [=] {
                    if(passwordLineEdit->text().isEmpty()){
                        passwordLineEdit->setText(generatePassword());
                    }
                if (passwordLineEdit->echoMode() == QLineEdit::Password) {

                passwordLineEdit->setEchoMode(QLineEdit::Normal);
                } else {
                passwordLineEdit->setEchoMode(QLineEdit::Password);
                }
                });
                //修改完成进行保存
                connect(targetNameLineEdit, &QLineEdit::editingFinished, this, [=](){
                    this->saveToJsonFile(targetNameLineEdit->text(),usernameLineEdit->text(),passwordLineEdit->text(),search_index,false,false);
                });
                connect(usernameLineEdit, &QLineEdit::editingFinished, this, [=](){
                    this->saveToJsonFile(targetNameLineEdit->text(),usernameLineEdit->text(),passwordLineEdit->text(),search_index,false,false);

                });
                connect(passwordLineEdit, &QLineEdit::editingFinished, this, [=](){
                    this->saveToJsonFile(targetNameLineEdit->text(),usernameLineEdit->text(),passwordLineEdit->text(),search_index,false,false);

                });



                // 连接copyButton的点击事件
                connect(copyButton, &QPushButton::clicked, [=] {
                QApplication::clipboard()->setText(passwordLineEdit->text());
                });

                connect(delButton ,&QPushButton::clicked, this, [=](){
                    credentialWidget->setParent(nullptr);
                    qDebug()<<"正在删除第"<<i<<"组件";
                    this->saveToJsonFile(targetNameLineEdit->text(),usernameLineEdit->text(),passwordLineEdit->text(),search_index,true,false);

                });

            }
        }
        connect(addPassButton ,&QPushButton::clicked, this, [=](){
            int *addIndex=new int();
            *addIndex=index;
            qDebug()<<index;
            QWidget *credentialWidget=new QWidget(allCredentialsWidget);
            credentialWidget->setFixedSize(816,53);
            saveToJsonFile("","","",*addIndex,false,true);//新增一条
            index++;
            QLineEdit *targetNameLineEdit = new QLineEdit(credentialWidget);
            targetNameLineEdit->setFont(QFont("Arial", 13));
            targetNameLineEdit->setAlignment(Qt::AlignCenter);
            targetNameLineEdit->setStyleSheet("background-color: #C3C9D3;border-radius: 6px;");
            targetNameLineEdit->setFixedSize(286, 40);
            targetNameLineEdit->setPlaceholderText("网址/app名称/服务器地址");
            targetNameLineEdit->move(0,0);

            QLineEdit *usernameLineEdit = new QLineEdit(credentialWidget);
            usernameLineEdit->setFont(QFont("Arial", 13));
            usernameLineEdit->setAlignment(Qt::AlignCenter);
            usernameLineEdit->setStyleSheet("background-color: #C3C9D3;border-radius: 6px;");
            usernameLineEdit->setFixedSize(192, 40);
            usernameLineEdit->setPlaceholderText("请输入您的账户");
            usernameLineEdit->move(305,0);

            QLineEdit *passwordLineEdit = new QLineEdit(credentialWidget);
            passwordLineEdit->setFont(QFont("Arial", 13));
            passwordLineEdit->setAlignment(Qt::AlignCenter);
            passwordLineEdit->setStyleSheet("background-color: #C3C9D3;border-radius: 6px;");
            passwordLineEdit->setEchoMode(QLineEdit::Password);
            passwordLineEdit->setFixedSize(256, 40);
            passwordLineEdit->setTextMargins(40, 0, 40, 0);
            passwordLineEdit->setPlaceholderText("请输入您的密码");

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
            QPushButton *delButton=new QPushButton(credentialWidget);
            QPixmap delPix(":/res/passbook/delete.png");
            delButton->setIcon(delPix);
            delButton->setFixedSize(38,38);
            delButton->move(779,0);
            delButton->setIconSize(QSize(31,31));
            delButton->setStyleSheet("border:none; background-color:transparent;");
            //将credentialswidget插入布局
            allCredentialsLayout->insertWidget(allCredentialsLayout->count()-1,credentialWidget);
            // 连接showPasswordButton的点击事件
            connect(showPasswordButton, &QPushButton::clicked, [=] {
                if(passwordLineEdit->text().isEmpty()){
                    passwordLineEdit->setText(generatePassword());
                }
            if (passwordLineEdit->echoMode() == QLineEdit::Password) {

            passwordLineEdit->setEchoMode(QLineEdit::Normal);
            } else {
            passwordLineEdit->setEchoMode(QLineEdit::Password);
            }
            });



            //修改完成进行保存
            connect(targetNameLineEdit, &QLineEdit::editingFinished, this, [=](){
                this->saveToJsonFile(targetNameLineEdit->text(),usernameLineEdit->text(),passwordLineEdit->text(),*addIndex,false,false);
            });
            connect(usernameLineEdit, &QLineEdit::editingFinished, this, [=](){
                this->saveToJsonFile(targetNameLineEdit->text(),usernameLineEdit->text(),passwordLineEdit->text(),*addIndex,false,false);

            });
            connect(passwordLineEdit, &QLineEdit::editingFinished, this, [=](){
                this->saveToJsonFile(targetNameLineEdit->text(),usernameLineEdit->text(),passwordLineEdit->text(),*addIndex,false,false);

            });



            // 连接copyButton的点击事件
            connect(copyButton, &QPushButton::clicked, [=] {
            QApplication::clipboard()->setText(passwordLineEdit->text());
            });

            connect(delButton ,&QPushButton::clicked, this, [=](){
                credentialWidget->setParent(nullptr);
                this->saveToJsonFile(targetNameLineEdit->text(),usernameLineEdit->text(),passwordLineEdit->text(),*addIndex,true,false);

            });

        });
        //设置密码条目底部弹簧
        QSpacerItem* passbookSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
        //将弹簧加入布局
        allCredentialsLayout->addItem(passbookSpacer);
        allCredentialsWidget->setLayout(allCredentialsLayout);
        scrollArea->update();

      });

    connect(exportButton,&QPushButton::clicked,this,[=]{
        this->close();
        QDir home = QDir::home();
        QString configPath = home.filePath(".config/sakuraassistant");
        QString desktopPath = home.filePath("Desktop");
        QString testPath = desktopPath + "/樱花助手备份";

        // 如果不存在，则复制configPath到testPath
        QDir dir(configPath);
        if (dir.exists()) {
            QDir().mkpath(testPath);
            QDir(testPath).removeRecursively();  // 先删除目标文件夹，避免复制失败
            if (!QDir().mkdir(testPath)) {
                qDebug() << "Failed to create directory" << testPath;
                return;
            }
            QDir().mkpath(testPath);
            QDir().setNameFilters(QStringList("*"));
            QDir().setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden);
            QDirIterator it(configPath, QDirIterator::Subdirectories);
            while (it.hasNext()) {
                QString srcPath = it.next();
                QString dstPath = testPath + "/" + it.fileName();
                if (QFileInfo(srcPath).isDir()) {
                    QDir().mkpath(dstPath);
                }
                else {
                    QFile::copy(srcPath, dstPath);
                }
            }
        }
        qDebug()<<"复制完成";
        QMessageBox::information(this, "复制成功", "请在桌面查看！");

    });
    connect(importButton,&QPushButton::clicked,this,[=]{
        QDir home = QDir::home();
        QString configPath = home.filePath(".config/sakuraassistant");
        QDesktopServices::openUrl(QUrl::fromLocalFile(configPath));
    });
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
                delJsonFile(m_accountPassword);
                this->close();
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}

void PassBook::saveToJsonFile(QString targetName, QString username, QString password, int i,bool isDel,bool isAdd)
{

    QJsonObject CredentialsObj;
    CredentialsObj.insert("targetName",targetName);
    CredentialsObj.insert("username",username);
    CredentialsObj.insert("password",password);

    QJsonObject allCredentialsObj=doc.object();

    QJsonArray CredentialsArr = allCredentialsObj["credentials"].toArray();

    if(isAdd==true){
        CredentialsArr.append(CredentialsObj);
    }
    if(isDel==true){
        CredentialsObj.insert("isDel", true);
        CredentialsArr[i]=CredentialsObj;
    }
    qDebug()<<"保存"<<i;

    CredentialsArr[i]=CredentialsObj;
    qDebug()<<CredentialsArr;

    allCredentialsObj.insert("credentials", CredentialsArr);
    doc.setObject(allCredentialsObj);
}


void PassBook::delJsonFile(QString accountPassword)
{


//    QJsonDocument fixedDoc;
//    fixedDoc=doc;
    QDir home = QDir::home();
    QString configPath = home.filePath(".config/sakuraassistant");
    QDir dir(configPath);
    QString path = dir.filePath("data.json");
    QFile file(path);
    if (!file.exists()) {
        if (file.open(QIODevice::WriteOnly)) {
            file.write("[]");
        }
    }
    //删除多余条目
    QJsonObject docObj =doc.object();
    QJsonArray docArray=docObj.value("credentials").toArray();
    qDebug()<<"del1"<<doc;
    for(int i=0;i<docArray.size();i++){
        QJsonObject item =docArray[i].toObject();
        qDebug()<<"item信息"<<item;
        if(item.value("isDel").toBool()==true){
            docArray.removeAt(i);
        }
    }
    docObj.insert("credentials", docArray);
    doc.setObject(docObj);
    qDebug()<<"del2"<<doc;
    file.setPermissions(QFile::ReadUser | QFile::WriteUser | QFile::ReadGroup | QFile::WriteGroup | QFile::ReadOther | QFile::WriteOther);
    if(!file.open(QIODevice::ReadWrite)) {
        qDebug() << "File open error";
    } else {
        qDebug() <<"File open!";
    }
    file.resize(0);
    qDebug()<<"加密时秘钥"<<accountPassword;
    file.write(encryptJsonFile(doc,accountPassword));
    file.close();
}



QByteArray PassBook::encryptJsonFile(QJsonDocument doc,const QString accountPassword)
{
    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB, QAESEncryption::PKCS7);
    QByteArray utf8Data = doc.toJson(QJsonDocument::Compact);
    QString str = QString::fromUtf8(utf8Data.constData(), utf8Data.size());
    QByteArray enBA = encryption.encode(str.toUtf8(), accountPassword.toUtf8());
    QByteArray enBABase64 = enBA.toBase64();
    qDebug()<<"enBABase64"<<enBABase64;
    enBA = QByteArray::fromBase64(enBABase64);
    qDebug()<<"enBA"<<enBABase64;

    return enBA;


}

QByteArray PassBook::decryptJsonFile(QByteArray jsonData, const QString accountPassword)
{
    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB, QAESEncryption::PKCS7);
    QByteArray deBA = encryption.decode(jsonData, accountPassword.toUtf8());
    qDebug()<<deBA;
    return QAESEncryption::RemovePadding(deBA, QAESEncryption::PKCS7);



}

void PassBook::passMainWindow()
{

}

QString PassBook::generatePassword()
{
    QString generatePassword;
    const int passwordLength = 10;
    const QString symbols = "~!@#$%^&*()_+`-=[]\\{}|;':\",./<>?";

    // 添加数字
    for (int i = 0; i < 3; ++i) {
        generatePassword.append(QString::number(qrand() % 10));
    }

    // 添加大小写字母
    for (int i = 0; i < 3; ++i) {
        generatePassword.append(QChar('a' + qrand() % 26));
        generatePassword.append(QChar('A' + qrand() % 26));
    }

    // 添加符号
    for (int i = 0; i < 1; ++i) {
        generatePassword.append(symbols[qrand() % symbols.length()]);
    }

    // 随机排序
    std::random_shuffle(generatePassword.begin(), generatePassword.end());

    return generatePassword;
}

PassBook::~PassBook() {
}


