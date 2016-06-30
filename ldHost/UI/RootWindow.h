/*!
  *@brief       系统控制面板头文件
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
#ifndef ROOTWINDOW_H
#define ROOTWINDOW_H

#include <QMainWindow>
#include "MainForm.h"

namespace Ui {
class RootWindow;
}

class RootWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RootWindow(QWidget *parent = 0);
    ~RootWindow();

private:
    Ui::RootWindow *ui;
    MainForm *mainForm=new MainForm();
    int curUID;
private slots:
    void on_exitBtn_clicked();//退出
    void on_mainBtn_clicked();//显示主窗体
    void on_setBtn_clicked();//显示设置
    void on_historyBtn_clicked();//显示历史
    void on_sensorBtn_clicked();//传感器校准
    void on_logBtn_clicked();//显示日志
    void on_helpBtn_clicked();//显示帮助
};

#endif // ROOTWINDOW_H
