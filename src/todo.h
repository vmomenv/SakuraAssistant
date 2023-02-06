#ifndef TODO_H
#define TODO_H
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QList>
#include <QTextStream>
#include <QObject>
#include<QString>
struct TodoItem{
    QString name;
    bool completed;
};

class ToDo
{
public:
    ToDo();
    void addItem(const TodoItem &item);
    //从json中加载todo项目
    void loadFromJsonFile();
    void saveToJsonFile();
    QList<TodoItem> items() const {
      return m_items;
    }
    QJsonArray itemArray;
private:
  QList<TodoItem> m_items;
};

#endif // TODO_H
