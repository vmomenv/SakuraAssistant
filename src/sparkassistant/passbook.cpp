#include "passbook.h"
#include"aesencrypt.h"
#include<DWidget>
#include<DTitlebar>
#include<DMainWindow>
#include<QVBoxLayout>
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

    //创建搜索框widget并且将控件放入该widget
    QWidget *searchWidget = new QWidget(this);
    QHBoxLayout *searchLayout = new QHBoxLayout(searchWidget);

    QLabel *searchPicLabel=new QLabel(searchWidget);

    QPixmap searchPixmap(":/res/passbook/search.png");
    searchPicLabel->setPixmap(searchPixmap);

    QLineEdit *searchEdit = new QLineEdit(searchWidget);
    searchLayout->addWidget(searchPicLabel);
    searchLayout->addWidget(searchEdit);
    searchWidget->setFixedSize(816, 53);
    searchWidget->setStyleSheet("background-color: #C3C9D3;border-radius: 12px;");
    searchWidget->move(22, 69);
//    QPixmap *searchPic








}
