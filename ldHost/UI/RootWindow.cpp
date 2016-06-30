/*!
  *@brief       系统控制面板声明文件
  *@author      yuenar
  *@date        16-06-01
  *@version     0.15
  *@note        @16-04-16 yuenar v0.11  初版，大致设计界面及鼠标事件
  *@note        @16-04-18 yuenar v0.12  自定义按钮悬停变化
  *@note        @16-04-20 yuenar v0.13  颜色变化设计
  *@note        @16-05-27 yuenar v0.14  修改解决BUG
  *@note        @16-06-01 yuenar v0.15  Qml重写为QWidget及相关槽函数
  *@warning     关于布局的可编辑需要再分配
  *@copyright   leidun.co
  */
#include "RootWindow.h"
#include <log/QsLog.h>
#include "ui_RootWindow.h"
#include <QGuiApplication>

const QString inFoucs="background-color:#345678;border-style: solid;font: 22pt '微软雅黑';";
const QString unFoucs="font: 22pt '微软雅黑';";

RootWindow::RootWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RootWindow),curUID(1)
{
    ui->setupUi(this);
    QLOG_INFO() << "加载主界面";
    this->ui->rootLay->setGeometry(QRect(0,0,1024,660));
    switch (curUID) {//根据当前的UID来切换布局中的widget部件
    case 1:
        this->ui->rootLay->addWidget(mainForm,0,0);
        break;
    default:
        this->ui->rootLay->addWidget(mainForm,0,0);
        break;
    }
    this->ui->mainBtn->setStyleSheet(inFoucs); //当前为焦点样式
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowTitleHint|Qt::Window);
}

RootWindow::~RootWindow()
{
    delete ui;
}
void RootWindow::on_exitBtn_clicked()
{
    qApp->exit();
}

void RootWindow::on_mainBtn_clicked()
{

    QLOG_INFO() << "加载主界面";
    mainForm->show();
    this->ui->mainBtn->setStyleSheet(inFoucs);
    this->ui->setBtn->setStyleSheet(unFoucs);
    this->ui->historyBtn->setStyleSheet(unFoucs);
    this->ui->sensorBtn->setStyleSheet(unFoucs);
    this->ui->logBtn->setStyleSheet(unFoucs);
    this->ui->helpBtn->setStyleSheet(unFoucs);
    this->ui->exitBtn->setStyleSheet(unFoucs);
    curUID=1;
}

void RootWindow::on_setBtn_clicked()
{

    QLOG_INFO() << "加载设置界面";
    mainForm->hide();
    this->ui->setBtn->setStyleSheet(inFoucs);
    this->ui->mainBtn->setStyleSheet(unFoucs);
    this->ui->historyBtn->setStyleSheet(unFoucs);
    this->ui->sensorBtn->setStyleSheet(unFoucs);
    this->ui->logBtn->setStyleSheet(unFoucs);
    this->ui->helpBtn->setStyleSheet(unFoucs);
    this->ui->exitBtn->setStyleSheet(unFoucs);
    curUID=2;
}

void RootWindow::on_historyBtn_clicked()
{

    QLOG_INFO() << "加载历史纪录";
    mainForm->hide();
    this->ui->historyBtn->setStyleSheet(inFoucs);
    this->ui->setBtn->setStyleSheet(unFoucs);
    this->ui->mainBtn->setStyleSheet(unFoucs);
    this->ui->sensorBtn->setStyleSheet(unFoucs);
    this->ui->logBtn->setStyleSheet(unFoucs);
    this->ui->helpBtn->setStyleSheet(unFoucs);
    this->ui->exitBtn->setStyleSheet(unFoucs);
    curUID=3;
}

void RootWindow::on_sensorBtn_clicked()
{

    QLOG_INFO() << "加载传感器校准界面";
    mainForm->hide();
    this->ui->sensorBtn->setStyleSheet(inFoucs);
    this->ui->setBtn->setStyleSheet(unFoucs);
    this->ui->historyBtn->setStyleSheet(unFoucs);
    this->ui->mainBtn->setStyleSheet(unFoucs);
    this->ui->logBtn->setStyleSheet(unFoucs);
    this->ui->helpBtn->setStyleSheet(unFoucs);
    this->ui->exitBtn->setStyleSheet(unFoucs);
    curUID=4;
}

void RootWindow::on_logBtn_clicked()
{
    QLOG_INFO() << "加载日志界面";
    mainForm->hide();
    this->ui->logBtn->setStyleSheet(inFoucs);
    this->ui->setBtn->setStyleSheet(unFoucs);
    this->ui->historyBtn->setStyleSheet(unFoucs);
    this->ui->sensorBtn->setStyleSheet(unFoucs);
    this->ui->mainBtn->setStyleSheet(unFoucs);
    this->ui->helpBtn->setStyleSheet(unFoucs);
    this->ui->exitBtn->setStyleSheet(unFoucs);
    curUID=5;
}

void RootWindow::on_helpBtn_clicked()
{
    QLOG_INFO() << "加载帮助与说明界面";
    mainForm->hide();
    this->ui->helpBtn->setStyleSheet(inFoucs);
    this->ui->setBtn->setStyleSheet(unFoucs);
    this->ui->historyBtn->setStyleSheet(unFoucs);
    this->ui->sensorBtn->setStyleSheet(unFoucs);
    this->ui->logBtn->setStyleSheet(unFoucs);
    this->ui->mainBtn->setStyleSheet(unFoucs);
    this->ui->exitBtn->setStyleSheet(unFoucs);
    curUID=6;
}
