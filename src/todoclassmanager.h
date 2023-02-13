#ifndef TODOCLASSMANAGER_H
#define TODOCLASSMANAGER_H
#include "todo.h"
#include <QWidget>
#include <QSlider>
class TodoClassManager : public QWidget
{
    Q_OBJECT
public:
    explicit TodoClassManager(QWidget *parent = nullptr);
    ~TodoClassManager();
    //从json中加载todo项目
    void loadFromJsonFile();
    void saveToJsonFile(bool completed,QString name,int i);

private:
    QList<ToDo*>todos;//根据todo类，创建Qlist

    QVBoxLayout todosVboxLayout;
    QSpacerItem spacer;//QSpacerItem?
    QPushButton addBtn;

    QJsonArray itemArray;


signals:

public slots:
};

#endif // TODOCLASSMANAGER_H
