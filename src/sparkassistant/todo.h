#ifndef TODO_H
#define TODO_H
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QObject>
#include <QPushButton>
#include <QWidget>
struct TodoItem{
    QString name;
    bool completed;
};

class ToDo: public QWidget
{
public:

    explicit ToDo(QWidget *parent = nullptr);
    void setLayout(QHBoxLayout *hbox);//一组按钮样式

    void addItem(const TodoItem &item);

    QList<TodoItem> items() const {
      return m_items;
    }

    //checkbox line delbtn为一组
    QCheckBox *checkBox;
    QLineEdit *line;
    QPushButton *delBtn;
    bool isDel;
private:
  QList<TodoItem> m_items;


signals:
//    void statusChanged();
};

#endif // TODO_H
