#include "todo.h"
#include<QPushButton>
#include<QDebug>
ToDo::ToDo()
{
    loadFromJsonFile();
}


void ToDo::addItem(const TodoItem &item) {
    m_items.append(item);
  }
void ToDo::loadFromJsonFile(){
    QFile file(":/res/todo.json");
    if(!file.open(QIODevice::ReadOnly)){
        return;
    }
    QByteArray data=file.readAll();
    file.close();
    QJsonDocument doc=QJsonDocument::fromJson(data);
    qDebug()<<doc;
    QJsonObject item=doc.object();
    QJsonArray itemArray=item.value("items").toArray();//解析items数组
    for(int i=0;i<itemArray.size();i++){
        QJsonObject item =itemArray[i].toObject();
        TodoItem todoItem;
        todoItem.name=item.value("name").toString();
        todoItem.completed=item.value("completed").toBool();
        qDebug()<<todoItem.name<<todoItem.completed;
        m_items.append(todoItem);
    }
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
