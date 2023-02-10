#include "todo.h"
#include<QPushButton>
#include<QDebug>
#include<QDir>
ToDo::ToDo()
{
}


void ToDo::addItem(const TodoItem &item) {
    m_items.append(item);
  }
void ToDo::loadFromJsonFile(){


    QDir home = QDir::home();
    QString configPath = home.filePath(".config/sparkassistant");
    QDir dir(configPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QString path = dir.filePath("todo.json");
    QFile file(path);

    if (!file.exists()) {
        QFile todoJson(":/res/todo.json");
        todoJson.copy(path);
    }
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
void ToDo::saveToJsonFile(QString name,bool completed,int i){
    qDebug()<<name<<completed<<i;
    QDir home = QDir::home();
    QString configPath = home.filePath(".config/sparkassistant");
    QDir dir(configPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QString path = dir.filePath("todo.json");
    QFile file(path);

    if (!file.exists()) {
        QFile todoJson(":/res/todo.json");
        todoJson.copy(path);
    }
    file.open(QIODevice::ReadWrite);

      if(!file.open(QIODevice::ReadWrite)) {
          qDebug() << "File open error";
      } else {
          qDebug() <<"File open!";
      }

      QByteArray data = file.readAll();
      QJsonObject smallObj;
      smallObj.insert("completed", completed);
      smallObj.insert("name", name);

      QJsonDocument doc = QJsonDocument::fromJson(data);
      QJsonObject itemObj = doc.object();
      QJsonArray itemArr = itemObj["items"].toArray();
      if(name.size()==0){
          itemArr.removeAt(i);//删除第i组
      }else if(i==-1){//新增一条
          itemArr.append(smallObj);
      }else{//保存该条
          itemArr[i]=smallObj;
      }

      itemObj.insert("items", itemArr);

      doc.setObject(itemObj);
      file.resize(0);
      file.write(doc.toJson());
      file.close();

}


