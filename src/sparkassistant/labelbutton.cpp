#include "labelbutton.h"

#include <QDebug>

LabelButton::LabelButton(QWidget *parent)
    : QLabel(parent)
{

}

LabelButton::~LabelButton()
{

}

void LabelButton::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "LabelButton::mousePressEvent";
    emit clicked(event);
}
