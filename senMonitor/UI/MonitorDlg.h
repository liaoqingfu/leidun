/*!
  *@brief       测量对话框头文件
  *@author      yuenar
  *@date        16-06-24
  *@version     0.12
  *@note        @16-06-03 yuenar v0.10 初版，大致设计界面及及相关槽函数
  *@note        @16-06-23 yuenar v0.11 更新测量名并发送列表
  *@note        @16-06-24 yuenar v0.12 增加清空模型数据槽函数
  *@warning     关于布局的可编辑需要再分配
  *@copyright   leidun.co
  */
#ifndef MONITORDLG_H
#define MONITORDLG_H

#include <QWidget>

namespace Ui {
class MonitorDlg;
}

class MonitorDlg : public QWidget
{
    Q_OBJECT

public:
    explicit MonitorDlg(QWidget *parent = 0);
    ~MonitorDlg();
    QString curMtrName;//当前测量名
    QString curSrName;//当前传感器名
    QString curGrpName;//当前分组名
    QStringList grpList;//所有分组列表
    QStringList mtrList;//所有测量列表

private:
    Ui::MonitorDlg *ui;
    int grpId;//组号

signals:
    void addMtr(QStringList);//添加测量
    void addGrp(QStringList);//添加分组
    void setSrToPort(QString);//配置传感器串口相关
    void setMrInfo(QString curMtrName,QString curGrpName);

public slots:
    void UpdateSensor(QStringList srList);
    void UpdateMtrName(QString idName);
    void clearAllModel();//清除所有数据模型

private slots:
    void on_addBtn_clicked();//添加测量
    void on_cancelBtn_clicked();//取消添加测量

};

#endif // MONITORDLG_H
