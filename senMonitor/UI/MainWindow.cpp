/*!
  *@brief       系统控制面板声明文件
  *@author      yuenar
  *@date        16-06-02
  *@version     0.10
  *@note        @16-06-02 yuenar v0.10  初版，大致设计界面及及相关槽函数
  *@warning     关于布局的可编辑需要再分配
  *@copyright   leidun.co
  */
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <log/QsLog.h>
#include <QGuiApplication>
#include <QDebug>

#pragma execution_character_set("utf-8")

const QString inFoucs="background-color:#345678;border-style: solid;font: 22pt '微软雅黑';";
const QString unFoucs="font: 22pt '微软雅黑';";
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),curUID(1)
{
    ui->setupUi(this);
    QLOG_INFO() << "加载主界面";
    this->ui->mainLay->setGeometry(QRect(0,0,1024,660));
    switch (curUID) {//根据当前的UID来切换布局中的widget部件
    case 1:
        this->ui->mainLay->addWidget(mainForm,0,0);
        break;
    default:
        this->ui->mainLay->addWidget(mainForm,0,0);
        break;
    }
    this->ui->mainBtn->setStyleSheet(inFoucs); //当前为焦点样式
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowTitleHint|Qt::Window);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_exitBtn_clicked()
{
    qApp->exit();
}

void MainWindow::setUser(QString userType)
{
    mainForm->curUser=userType;
    QLOG_INFO() << "当前登录用户为"<<userType;
}

void MainWindow::on_mainBtn_clicked()
{

    QLOG_INFO() << "加载主界面";
    mainForm->show();
    this->ui->mainBtn->setStyleSheet(inFoucs);
    this->ui->historyBtn->setStyleSheet(unFoucs);
    this->ui->setBtn->setStyleSheet(unFoucs);
    this->ui->logBtn->setStyleSheet(unFoucs);
    this->ui->helpBtn->setStyleSheet(unFoucs);
    this->ui->exitBtn->setStyleSheet(unFoucs);
    curUID=1;
}

void MainWindow::on_historyBtn_clicked()
{

    QLOG_INFO() << "加载历史纪录";
    mainForm->hide();
    this->ui->historyBtn->setStyleSheet(inFoucs);
    this->ui->mainBtn->setStyleSheet(unFoucs);
    this->ui->setBtn->setStyleSheet(unFoucs);
    this->ui->logBtn->setStyleSheet(unFoucs);
    this->ui->helpBtn->setStyleSheet(unFoucs);
    this->ui->exitBtn->setStyleSheet(unFoucs);
    curUID=2;
}

void MainWindow::on_logBtn_clicked()
{
    QLOG_INFO() << "加载日志界面";
    mainForm->hide();
    this->ui->logBtn->setStyleSheet(inFoucs);
    this->ui->historyBtn->setStyleSheet(unFoucs);
    this->ui->setBtn->setStyleSheet(unFoucs);
    this->ui->mainBtn->setStyleSheet(unFoucs);
    this->ui->helpBtn->setStyleSheet(unFoucs);
    this->ui->exitBtn->setStyleSheet(unFoucs);
    curUID=3;
}

void MainWindow::on_helpBtn_clicked()
{
    QLOG_INFO() << "加载帮助与说明界面";
    mainForm->hide();
    this->ui->helpBtn->setStyleSheet(inFoucs);
    this->ui->historyBtn->setStyleSheet(unFoucs);
    this->ui->setBtn->setStyleSheet(unFoucs);
    this->ui->logBtn->setStyleSheet(unFoucs);
    this->ui->mainBtn->setStyleSheet(unFoucs);
    this->ui->exitBtn->setStyleSheet(unFoucs);
    curUID=4;
}

void MainWindow::on_setBtn_clicked()
{
    QLOG_INFO() << "加载系统设置界面";
    mainForm->hide();
    this->ui->helpBtn->setStyleSheet(unFoucs);
    this->ui->historyBtn->setStyleSheet(unFoucs);
    this->ui->setBtn->setStyleSheet(inFoucs);
    this->ui->logBtn->setStyleSheet(unFoucs);
    this->ui->mainBtn->setStyleSheet(unFoucs);
    this->ui->exitBtn->setStyleSheet(unFoucs);
    curUID=5;
}
