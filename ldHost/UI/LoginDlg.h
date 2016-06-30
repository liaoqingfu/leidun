/**
  *@brief       登录界面类头文件
  *@author      yuenar
  *@date        16-05-30
  *@version     0.14
  *@note        @16-04-16 yuenar v0.1  初版，大致设计界面及数据结构
  *@note        @16-04-17 yuenar v0.11 用户模式配置及验证
  *@note        @16-04-18 yuenar v0.12 更换ini为xml保存用户配置
  *@note        @16-05-07 yuenar v0.13 验证及测试BUG
  *@note        @16-05-30 yuenar v0.14 加入日志系统
  *@warning     调用时，注意合理布局
  *@copyright   leidun.co
  */
#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QDialog>
#include <QMap>
#include <QString>

namespace Ui {
class LoginDlg;
}

/**
 * @class       登录窗口类
 * @brief       操作工通过该窗口进行登录及验证之后进入系统界面
 * @author      yuenar
 * @date        16-04-16
 */
class LoginDlg : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDlg(QWidget *parent = 0);
    ~LoginDlg();


    QString curUserType;//当前输入的用户名
    QString curPassWord;//当前输入的密码
    bool isLogin;//登录状态标志位
    QMap<QString,QString>  userModeMap;//储存用户模式信息

signals:
    /**
    *@brief		当前用户类型信号量
    *@remark	用于传值设定相应权限
    *@param[in]	userType输入的登录名
    */
    void curUser(const QString &userType);
private slots:
    void on_quitBtn_clicked();//退出按钮触发事件
    void loadUserMode();//加载本地用户模式配置
    void on_loginBtn_clicked();//登录按钮触发事件

private:
    Ui::LoginDlg *ui;
};

#endif // LOGINDLG_H
