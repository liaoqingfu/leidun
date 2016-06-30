/**
  *@brief       主函数入口
  *@author      yuenar
  *@date        16-06-02
  *@version     0.12
  *@note        @16-04-06 yuenar v0.10 初版，添加日志系统及基本函数
  *@note        @16-05-30 yuenar v0.11 加入日志系统
  *@note        @16-06-02 yuenar v0.12 增加反馈登录用户槽函数
  *@copyright   leidun.co
  */

#include "UI/LoginDlg.h"
#include <QApplication>
#include "UI/LoginDlg.h"
#include "UI/MainWindow.h"
#include <log/QsLog.h>
#include <log/QsLogDest.h>
#include <QString>
#include <QObject>
#include <QDir>

#pragma execution_character_set("utf-8")

int main(int argc, char *argv[])
{
    /**
     * 以下为实例化日志系统类
*/
    using namespace QsLogging;
    // init the logging mechanism
    QsLogging::Logger& logger = QsLogging::Logger::instance();
    QApplication app(argc, argv);//日志系统在主程序之前
    logger.setLoggingLevel(QsLogging::TraceLevel);
    const QString sLogPath(QDir(app.applicationDirPath()).filePath("log.txt"));
    QsLogging::DestinationPtr fileDestination(
                QsLogging::DestinationFactory::MakeFileDestination(sLogPath) );
    QsLogging::DestinationPtr debugDestination(
                QsLogging::DestinationFactory::MakeDebugOutputDestination() );
    logger.addDestination(debugDestination);
    logger.addDestination(fileDestination);
    /**
     * 以上为实例化日志系统类
*/

    QLOG_INFO() << "Program started";
    LoginDlg login;//登陆窗口
    QLOG_INFO() << "请登录";
    login.show();
    MainWindow rootWin;

    QObject::connect(&login,&LoginDlg::curUser,&rootWin,&MainWindow::setUser);
    if(login.exec()==QDialog::Accepted){
        rootWin.show();//直接显示
        rootWin.move(0,0);//重置位置
        return app.exec();
    }
    return 0;
}
