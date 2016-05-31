/**
  *@brief       主函数入口
  *@author      yuenar
  *@date        16-04-28
  *@version     0.14
  *@note        @16-04-06 yuenar v0.11 初版，设置qml界面加载
  *@note        @16-04-17 yuenar v0.11 添加登录框LoginDlg加载
  *@note        @16-04-23 yuenar v0.12 添加主控窗口MainForm加载
  *@note        @16-04-28 yuenar v0.13 混合编译测试，做出相应改动
  *@note        @16-05-30 yuenar v0.14 添加日志系统及相关函数
  *@copyright   leidun.co
  */
#include <QApplication>
#include <QQmlApplicationEngine>
#include "LoginDlg.h"
#include "MainForm.h"
#include <log/QsLog.h>
#include <log/QsLogDest.h>
#include <QString>
#include <QDir>
#include <QQmlContext>
#include <QQmlComponent>


int main(int argc, char *argv[])
{
    using namespace QsLogging;
    // init the logging mechanism
    QsLogging::Logger& logger = QsLogging::Logger::instance();
    QApplication app(argc, argv);

    // set minimum log level and file name
    logger.setLoggingLevel(QsLogging::TraceLevel);
    const QString sLogPath(QDir(app.applicationDirPath()).filePath("log.txt"));
    // Create log destinations
    QsLogging::DestinationPtr fileDestination(
                QsLogging::DestinationFactory::MakeFileDestination(sLogPath) );
    QsLogging::DestinationPtr debugDestination(
                QsLogging::DestinationFactory::MakeDebugOutputDestination() );
//    // set log destinations on the logger
    logger.addDestination(debugDestination);
    logger.addDestination(fileDestination);

    // write an info message
    QLOG_INFO() << "Program started";

    LoginDlg login;
    QLOG_INFO() << "请登录";
    login.show();
    if(login.exec()==QDialog::Accepted){
        QQmlApplicationEngine engine;
        engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));
        MainForm form;
        engine.rootContext()->setContextProperty("mainUI",&form);//放到QML
        form.show();//直接显示
        form.move(0,0);//重置位置
        engine.rootContext()->setContextProperty("loginDlg",&login);
        return app.exec();
    }
    return 0;
}
