#include "todo.h"
#include<QPushButton>
#include<QDebug>
ToDo::ToDo()
{
}


void ToDo::addItem(const TodoItem &item) {
    m_items.append(item);
  }
void ToDo::loadFromJsonFile(){
    QFile file(":/res/todo.json");
    if(!file.open(QIODevice::ReadOnly)){
        return;
    }
    QByteArray data=file.readAll();//将内容加载到data中
    file.close();
    QJsonDocument doc=QJsonDocument::fromJson(data);//将data转为json格式
    qDebug()<<doc;
    QJsonObject itemObj=doc.object();//将json格式的内容初始化
    itemArray=itemObj.value("items").toArray();//解析items数组,将json存放于itemArray
    file.close();
}
void ToDo::saveToJsonFile(const QString &fileName){
    QFile file(":/res/todo.json");
    if(!file.open(QIODevice::WriteOnly)){
        return;
    }
    QJsonArray array;
    for(const TodoItem &item : m_items){
        QJsonObject obj;
        obj["name"]=item.name;
        obj["completed"]=item.completed;
        array.append(obj);
    }
    QJsonDocument doc(array);
    file.write(doc.toJson());
    file.close();
}
