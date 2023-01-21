#include <DApplication>
#include <DMainWindow>
#include <DWidgetUtil>
#include <DApplicationSettings>
#include <DTitlebar>
#include <DProgressBar>
#include <DFontSizeManager>

#include <QPropertyAnimation>
#include <QDate>
#include <QLayout>
#include <QMenu>


#include "mainwindow.h"
DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    DApplication a(argc, argv);
//    a.setOrganizationName("deepin");
//    a.setApplicationName("dtk-application");
//    a.setApplicationVersion("1.0");
//    a.setProductIcon(QIcon(":/images/logo.svg"));
//    a.setProductName("Dtk Application");
//    a.setApplicationDescription("This is a dtk template application.");

//    a.loadTranslator();
//    a.setApplicationDisplayName(QCoreApplication::translate("Main", "DTK Application"));

//    // 保存程序的窗口主题设置
//    DApplicationSettings as;
//    Q_UNUSED(as)

    MainWindow w;
    w.show();


    return a.exec();
}
