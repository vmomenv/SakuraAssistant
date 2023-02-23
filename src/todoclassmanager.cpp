#include "todoclassmanager.h"
#include<QDir>
#include<QFile>
#include<QScrollArea>
#include<QProcess>
TodoClassManager::TodoClassManager(QWidget *parent) : QWidget(parent)
  , todos()
  ,todosVboxLayout(new QVBoxLayout)
  , spacer(new QSpacerItem(10,10, QSizePolicy::Minimum, QSizePolicy::Expanding))

{

    QWidget *todoWidget=new QWidget(this);
    QScrollArea *scrollArea=new QScrollArea(this);
    scrollArea->setWidget(todoWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    this->setStyleSheet("QScrollArea,QWidget#viewport,.QWidget\
    {\
        background-color:transparent;\
        margin:0px;\
    }\
    ");

    //重新载入文档
    this->loadFromJsonFile();
    QJsonObject item =doc.object();
    QJsonObject itemObj=doc.object();
    itemArray=itemObj.value("items").toArray();
    //添加控件
    QPushButton *addBtn=new QPushButton(this);
    addBtn->resize(30,30);
    addBtn->setText("+");
    addBtn->move(20,200);

    if(this->itemArray.size()!=0){
        for(int i=0;i<this->itemArray.size();i++){
            QJsonObject item =this->itemArray[i].toObject();
            TodoItem todoItem;
            todoItem.name=item.value("name").toString();
            todoItem.completed=item.value("completed").toBool();

            if(item.value("isDel").toBool()==true){
                continue;
            }

            QHBoxLayout *todosLayout =new QHBoxLayout();

            ToDo *todo=new ToDo(todoWidget);
            scrollArea->setFixedSize(208, 200);
            todo->checkBox->setChecked(todoItem.completed);
            todo->checkBox->setFixedHeight(16);
            todo->line->setText(todoItem.name);
            todo->line->setFixedHeight(40);
            todo->delBtn->setIcon(QPixmap(":/res/delete.png"));
            todo->delBtn->setFixedHeight(40);

            todo->setLayout(todosLayout);
            todosVboxLayout->addLayout(todosLayout);



            connect(todo->checkBox, &QCheckBox::stateChanged, this,[=](){
                this->saveToJsonFile(todo->checkBox->checkState(),todo->line->text(),i,false,false);
            });

            connect(todo->line, &QLineEdit::editingFinished, this, [=](){
                this->saveToJsonFile(todo->checkBox->checkState(),todo->line->text(),i,false,false);
            });
            connect(todo->delBtn, &QPushButton::clicked, this, [=](){
    //            todo->checkBox->deleteLater();
                todo->checkBox->setParent(nullptr);
    //            todo->line->deleteLater();
                todo->line->setParent(nullptr);
    //            todo->delBtn->deleteLater();
                todo->delBtn->setParent(nullptr);
                qDebug()<<"正在删除第"<<i<<"组件";
                this->saveToJsonFile(false,"",i,true,false);

            });

        }

    }else{

    }


//    QVBoxLayout *vbox =new QVBoxLayout(todoWidget);
//    vbox->addLayout(todosVboxLayout);
//    vbox->addItem(spacer);


//    vbox->addWidget(addBtn);
    static int index;
    index=itemArray.size();
    connect(addBtn,&QPushButton::clicked,this,[&](){
        int *addIndex=new int();
        *addIndex=index;
        if(index==0){
            index=1;
        }
        qDebug()<<index;
        QHBoxLayout *todosLayout =new QHBoxLayout();
        ToDo *todo=new ToDo();
        todo->checkBox->setChecked(false);
        todo->line->setText("");
        todo->delBtn->setIcon(QPixmap(":/res/delete.png"));

        todo->setLayout(todosLayout);
        todosVboxLayout->addLayout(todosLayout);
        this->saveToJsonFile(false," ",*addIndex,false,true);//新增一条


        connect(todo->checkBox, &QCheckBox::stateChanged, this,[=](){
            this->saveToJsonFile(todo->checkBox->checkState(),todo->line->text(),*addIndex,false,false);
        });

        connect(todo->line, &QLineEdit::editingFinished, this, [=](){
            this->saveToJsonFile(todo->checkBox->checkState(),todo->line->text(),*addIndex,false,false);
        });
        connect(todo->delBtn, &QPushButton::clicked, this, [=](){
            todo->checkBox->deleteLater();
            todo->checkBox->setParent(nullptr);
            todo->line->deleteLater();
            todo->line->setParent(nullptr);
            todo->delBtn->deleteLater();
            todo->delBtn->setParent(nullptr);
            this->saveToJsonFile(false,"",*addIndex,true,false);

        });
        index+=1;
    });

    todoWidget->setLayout(todosVboxLayout);
    todosVboxLayout->setAlignment(Qt::AlignTop);
}



void TodoClassManager::loadFromJsonFile(){


    //创建文件路径
    QDir home = QDir::home();
    QString configPath = home.filePath(".config/sparkassistant");
    QDir dir(configPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QString path = dir.filePath("todo.json");
    QFile file;
    file.setFileName(path);
    //将qt中文件复制到~/.config/sparkassistant/
    if (!file.exists()) {
        QFile todoJson(":/res/todo.json");
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

    QByteArray data=file.readAll();//将内容加载到data中

    doc=QJsonDocument::fromJson(data);//将data转为json格式
    QJsonObject itemObj=doc.object();//将json格式的内容初始化
    itemArray=itemObj.value("items").toArray();//解析items数组,将json存放于itemArray

}


void TodoClassManager::saveToJsonFile(bool completed,QString name,int i,bool isDel,bool isAdd){

    QJsonObject smallObj;
    smallObj.insert("completed", completed);
    smallObj.insert("name", name);



    QJsonObject itemObj = doc.object();
    qDebug()<<"保存时大json"<<itemObj;
    QJsonArray itemArr = itemObj["items"].toArray();
    if(isAdd==true){//新增一条

        itemArr.append(smallObj);
        qDebug()<<"new"<<itemArr;
    }

    if(isDel==true){
        smallObj.insert("isDel", true);
        itemArr[i]=smallObj;
    }
    qDebug()<<"若保存修改"<<i;
    itemArr[i]=smallObj;

    itemObj.insert("items", itemArr);
    doc.setObject(itemObj);
    qDebug()<<"保存时doc"<<doc;



}

void TodoClassManager::delJsonFile()
{
    qDebug()<<doc;
    qDebug()<<"del"<<doc;


//    QJsonDocument fixedDoc;
//    fixedDoc=doc;
    QDir home = QDir::home();
    QString configPath = home.filePath(".config/sparkassistant");
    QDir dir(configPath);
    QString path = dir.filePath("todo.json");
    QFile file;
    file.setFileName(path);
    file.setPermissions(QFile::ReadUser | QFile::WriteUser | QFile::ReadGroup | QFile::WriteGroup | QFile::ReadOther | QFile::WriteOther);
    if(!file.open(QIODevice::ReadWrite)) {
        qDebug() << "File open error";
    } else {
        qDebug() <<"File open!";
    }
    //删除多余条目
    QJsonObject docObj =doc.object();
    QJsonArray docArray=docObj.value("items").toArray();
    qDebug()<<"del1"<<doc;
    for(int i=0;i<docArray.size();i++){
        QJsonObject item =docArray[i].toObject();
        qDebug()<<"item信息"<<item;
        if(item.value("isDel").toBool()==true){
            docArray.removeAt(i);
        }
    }
    docObj.insert("items", docArray);
    doc.setObject(docObj);
    qDebug()<<"del2"<<doc;
    file.resize(0);
    file.write(doc.toJson());
}

TodoClassManager::~TodoClassManager() {
delJsonFile();
}
