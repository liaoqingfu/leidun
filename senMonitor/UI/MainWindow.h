/*!
  *@brief       系统控制面板头文件
  *@author      yuenar
  *@date        16-06-02
  *@version     0.10
  *@note        @16-06-02 yuenar v0.10  初版，大致设计界面及及相关槽函数
  *@warning     关于布局的可编辑需要再分配
  *@copyright   leidun.co
  */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "MainForm.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    MainForm *mainForm=new MainForm();
    int curUID;//子窗口ID号用于切换界面

public slots:
    void setUser(QString userType);//设定用户类型

private slots:
    void on_exitBtn_clicked();//退出
    void on_mainBtn_clicked();//显示主窗体
    void on_historyBtn_clicked();//显示历史
    void on_logBtn_clicked();//显示日志
    void on_helpBtn_clicked();//显示帮助
    void on_setBtn_clicked();
};

#endif // MAINWINDOW_H
