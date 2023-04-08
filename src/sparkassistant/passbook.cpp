#include "passbook.h"
#include"aesencrypt.h"
#include<DWidget>
#include<DTitlebar>
#include<DMainWindow>
PassBook::PassBook(DWidget *parent)
{
    setFixedSize(861,490);
    titlebar()->setFixedHeight(0);
    setFocusPolicy(Qt::ClickFocus);
    installEventFilter(this);//安装事件过滤器
    setWindowOpacity(0.7);//设置透明
//    setAttribute(Qt::WA_TranslucentBackground);//设置背景透明
    setEnableBlurWindow(true);//设置高斯模糊
    this->move(552,342);
    QWidget *titleWidget=new QWidget(this);
    titleWidget->resize(861,53);
    QPalette pal = titleWidget->palette();//设置widget背景色
    pal.setColor(QPalette::Background, QColor(49,53,62, 255));
    titleWidget->setPalette(pal);
    titleWidget->setAutoFillBackground(true);
    titleWidget->move(0,0);

    QWidget *searchWidget=new QWidget(this);
    searchWidget->resize(816,53);
//    QPixmap *searchPic







}
