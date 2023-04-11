#ifndef LABELBUTTON_H
#define LABELBUTTON_H

#include <QLabel>
#include <QMouseEvent>

class LabelButton : public QLabel

{
    Q_OBJECT
public:
    LabelButton(QWidget *parent = nullptr);
    ~LabelButton();

signals:
    void clicked(QMouseEvent *ev);

protected:
    void mousePressEvent(QMouseEvent *event);

};

#endif // LABELBUTTON_H
