#include "todoclassmanager.h"
#include<QDir>
#include<QFile>
#include<QScrollArea>
TodoClassManager::TodoClassManager(QWidget *parent) : QWidget(parent)
  ,todosVboxLayout(new QVBoxLayout)
  , todos()
  , spacer(new QSpacerItem(10,10, QSizePolicy::Minimum, QSizePolicy::Expanding))

{
    this->loadFromJsonFile();
    QPushButton *addBtn=new QPushButton(this);
    addBtn->resize(30,30);
    addBtn->setText("+");
    addBtn->move(20,200);
    for(int i=0;i<this->itemArray.size();i++){
        QJsonObject item =this->itemArray[i].toObject();
        TodoItem todoItem;
        todoItem.name=item.value("name").toString();
        todoItem.completed=item.value("completed").toBool();

        QHBoxLayout *todosLayout =new QHBoxLayout();
        ToDo *todo=new ToDo();

        todo->checkBox->setChecked(todoItem.completed);
        todo->line->setText(todoItem.name);
        todo->delBtn->setIcon(QPixmap(":/res/delete.png"));

        todo->setLayout(todosLayout);
        todosVboxLayout->addLayout(todosLayout);



        connect(todo->checkBox, &QCheckBox::stateChanged, this,[=](){
            this->saveToJsonFile(todo->checkBox->checkState(),todo->line->text(),i,false);
        });

        connect(todo->line, &QLineEdit::editingFinished, this, [=](){
            this->saveToJsonFile(todo->checkBox->checkState(),todo->line->text(),i,false);
        });
        connect(todo->delBtn, &QPushButton::clicked, this, [=](){
            todo->checkBox->deleteLater();
            todo->checkBox->setParent(nullptr);
            todo->line->deleteLater();
            todo->line->setParent(nullptr);
            todo->delBtn->deleteLater();
            todo->delBtn->setParent(nullptr);
            this->saveToJsonFile(false,"",i,false);

        });

    }

    QVBoxLayout *vbox =new QVBoxLayout(this);
    vbox->addLayout(todosVboxLayout);
    vbox->addItem(spacer);




//    vbox->addWidget(addBtn);
    static int index;
    index=this->itemArray.size()-1;//将计数器调到最后一位
    connect(addBtn,&QPushButton::clicked,this,[&](){
        QHBoxLayout *todosLayout =new QHBoxLayout();
        ToDo *todo=new ToDo();
        todo->checkBox->setChecked(false);
        todo->line->setText("");
        todo->delBtn->setIcon(QPixmap(":/res/delete.png"));

        todo->setLayout(todosLayout);
        todosVboxLayout->addLayout(todosLayout);
        this->saveToJsonFile(false," ",-1,false);
        connect(todo->checkBox, &QCheckBox::stateChanged, this,[=](){
            this->saveToJsonFile(todo->checkBox->checkState(),todo->line->text(),index,false);
        });

        connect(todo->line, &QLineEdit::editingFinished, this, [=](){
            this->saveToJsonFile(todo->checkBox->checkState(),todo->line->text(),index,false);
        });
        connect(todo->delBtn, &QPushButton::clicked, this, [=](){
            todo->checkBox->deleteLater();
            todo->checkBox->setParent(nullptr);
            todo->line->deleteLater();
            todo->line->setParent(nullptr);
            todo->delBtn->deleteLater();
            todo->delBtn->setParent(nullptr);
            this->saveToJsonFile(false,"",index,false);

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
    QFile file(path);
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

    QJsonDocument doc=QJsonDocument::fromJson(data);//将data转为json格式
    qDebug()<<doc;
    QJsonObject itemObj=doc.object();//将json格式的内容初始化
    itemArray=itemObj.value("items").toArray();//解析items数组,将json存放于itemArray
    file.close();
}


void TodoClassManager::saveToJsonFile(bool completed,QString name,int i,bool isDel){
    //创建文件路径
    QDir home = QDir::home();
    QString configPath = home.filePath(".config/sparkassistant");
    QDir dir(configPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QString path = dir.filePath("todo.json");
    QFile file(path);
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
    QJsonObject smallObj;
    smallObj.insert("completed", completed);
    smallObj.insert("name", name);

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject itemObj = doc.object();
    QJsonArray itemArr = itemObj["items"].toArray();
    if(name.size()==0){
        smallObj.insert("isDel", true);
        itemArr[i]=smallObj;

    }else if(i==-1){//新增一条
        itemArr.append(smallObj);
    }else{//保存该条
        itemArr[i]=smallObj;
    }
    if(isDel==true){
        itemArr.removeAt(i);//删除第i组
    }

    itemObj.insert("items", itemArr);

    doc.setObject(itemObj);
    file.resize(0);
    file.write(doc.toJson());
    file.close();

}

TodoClassManager::~TodoClassManager() {

}
