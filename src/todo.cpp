#include "todo.h"
#include<QPushButton>
#include<QDebug>
#include<QDir>
ToDo::ToDo(QWidget *parent) : QWidget(parent)
  , checkBox(new QCheckBox)
  , line(new QLineEdit)
  , delBtn(new QPushButton())
  , isDel(false)
{


}

void ToDo::setLayout(QHBoxLayout *hbox)//设置该组样式
{
    hbox->addWidget(checkBox);
    hbox->addWidget(line);
    hbox->addWidget(delBtn);
}





void ToDo::addItem(const TodoItem &item) {
    m_items.append(item);
  }



