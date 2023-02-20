#include "todoclassmanager.h"
#include<QDir>
#include<QFile>
#include<QScrollArea>

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
                this->saveToJsonFile(false,"",i,false,false);

            });

        }
        todoWidget->setLayout(todosVboxLayout);
        todosVboxLayout->setAlignment(Qt::AlignTop);
    }else{

    }


//    QVBoxLayout *vbox =new QVBoxLayout(todoWidget);
//    vbox->addLayout(todosVboxLayout);
//    vbox->addItem(spacer);


//    vbox->addWidget(addBtn);
    static int index;
    index=itemArray.size();
    connect(addBtn,&QPushButton::clicked,this,[&](){
        qDebug()<<index;
        QHBoxLayout *todosLayout =new QHBoxLayout();
        ToDo *todo=new ToDo();
        todo->checkBox->setChecked(false);
        todo->line->setText("");
        todo->delBtn->setIcon(QPixmap(":/res/delete.png"));

        todo->setLayout(todosLayout);
        todosVboxLayout->addLayout(todosLayout);
        this->saveToJsonFile(false," ",itemArray.size(),false,true);
        connect(todo->checkBox, &QCheckBox::stateChanged, this,[=](){
            this->saveToJsonFile(todo->checkBox->checkState(),todo->line->text(),index,false,false);
        });

        connect(todo->line, &QLineEdit::editingFinished, this, [=](){
            this->saveToJsonFile(todo->checkBox->checkState(),todo->line->text(),index,false,false);
        });
        connect(todo->delBtn, &QPushButton::clicked, this, [=](){
            todo->checkBox->deleteLater();
            todo->checkBox->setParent(nullptr);
            todo->line->deleteLater();
            todo->line->setParent(nullptr);
            todo->delBtn->deleteLater();
            todo->delBtn->setParent(nullptr);
            this->saveToJsonFile(false,"",index,false,false);

        });
        index+=1;
    });


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

//    QJsonDocument
    QJsonObject itemObj = doc.object();
    qDebug()<<"第"<<i<<"组保存前"<<itemObj;
    QJsonArray itemArr = itemObj["items"].toArray();
    if(name.size()==0){
        smallObj.insert("isDel", true);
        itemArr[i]=smallObj;

    }else if(isAdd==true){//新增一条
        itemArr.append(smallObj);
    }else{//保存该条
        itemArr[i]=smallObj;
    }
    if(isDel==true){
        itemArr.removeAt(i);//删除第i组
    }

    itemObj.insert("items", itemArr);

    doc.setObject(itemObj);
    qDebug()<<"第"<<i<<"组保存后"<<itemObj;
    itemArray=itemObj.value("items").toArray();

//    file.resize(0);
//    file.write(doc.toJson());

}

void TodoClassManager::delJsonFile()
{
    qDebug()<<"del"<<doc;

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
    QJsonObject item =doc.object();
    QJsonObject itemObj=doc.object();
    qDebug()<<"del"<<doc;
    QJsonArray itemArray=itemObj.value("items").toArray();
    for(int i=0;i<itemArray.size();i++){
        QJsonObject item =itemArray[i].toObject();
        TodoItem todoItem;
        if(item.value("isDel").toBool()==true){
            saveToJsonFile(false,"",i,true,false);
            qDebug()<<"正在删除第"<<i<<"数据";
        }

    }
//    qDebug()<<"del"<<doc;
    file.resize(0);
    file.write(doc.toJson());
}

TodoClassManager::~TodoClassManager() {
    delJsonFile();
}
