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
    // 创建系统托盘图标
    QSystemTrayIcon trayIcon;
    trayIcon.setIcon(QIcon(":/res/sakura.png")); // 设置图标
    trayIcon.setToolTip("樱花助手"); // 设置提示信息

    // 创建菜单
    QMenu *menu = new QMenu();
    QAction *quitAction = new QAction("退出", menu);
    menu->addAction(quitAction);

    // 将菜单添加到系统托盘图标中
    trayIcon.setContextMenu(menu);

    // 单击托盘图标时显示主程序
    QObject::connect(&trayIcon, &QSystemTrayIcon::activated, [&](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::Trigger) {
            w.show();
        }
    });
    // 单击退出动作时退出应用程序
    QObject::connect(quitAction, &QAction::triggered, &w, &QApplication::quit);

    // 显示系统托盘图标
    trayIcon.show();


    return a.exec();
}
