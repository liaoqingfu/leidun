/**
  *@brief       主界面类头文件
  *@author      yuenar
  *@date        16-06-27
  *@version     0.14
  *@note        @16-06-02 yuenar v0.10 初版，大致设计布局
  *@note        @16-06-03 yuenar v0.11 添加测量控制相关槽函数
  *@note        @16-06-04 yuenar v0.12 添加串口相关库和槽函数
  *@note        @16-06-24 yuenar v0.13 改写表格模型初始化函数及清空统计信号
  *@note        @16-06-27 yuenar v0.14 解析数据更新到UI及打印异常
  *@warning     调用时，注意合理布局
  *@copyright   leidun.co
  */
#ifndef MAINFORM_H
#define MAINFORM_H

#include <QWidget>
#include <QTimer>
#include <QStandardItemModel>
#include "UI/MonitorDlg.h"
#include "Uart/SerialPortList.h"

namespace Ui {
class MainForm;
}

class MainForm : public QWidget
{
    Q_OBJECT

public:
    explicit MainForm(QWidget *parent = 0);
    ~MainForm();

    QStandardItemModel *model;

    QTimer *systimer;//运行时间计数器
    QTimer *timer;//检测时间计数器
    QString curUser;//当前用户名
    QString basicStr;//基准值,注意转型为float
    float avgNum;//标准值
    float okNum;//Ok计数器
    float ngNum;//Ng计数器
    float allNum;//总数
    float precent;//百分比

    MonitorDlg *mtDlg;//测量控制对话框

    SerialPortList *spListScan;//已扫描到的可用串口列表
    QStringList srList;//初始传感器列表
    QStringList newList;//更新后的传感器列表
    QStringList srGrpList;//初始分组列表
    QStringList newGrpList;//更新后的分组列表
    int num;//取测量列表的值
    bool running;//检测统计是否在运行

signals:
    void clearModel();//清空所有数据模型
    void countData(float okNum,float ngNum,float allNum,float precent);//统计数据信号
private:
    Ui::MainForm *ui;
    qint64 duration;
    //    int pId;//当前产品号

private slots:
    void runPause();//暂停或继续检测产品槽函数
    //    void on_addBtn_clicked();

    void upCountData(float okNum,float ngNum,float allNum,float precent);//更新统计数据到界面

    /**
    *@brief		添加测量信息到表格界面
    *@param[in]	传进来的所有传感器列表
    */
    void addMt(QStringList curMtrList);

    /**
    *@brief		添加分组信息到表格界面
    *@param[in]	传进来的已分组信息列表
    */
    void addGp(QStringList curGrpList);

    /**
    *@brief		更新测量信息到表格界面
    *@param[in]	传进来的传感器测量数据
    */
    void flushMtrData(QStringList dIdList,QStringList mtrDataList);

    /**
    *@brief		选中测量值为基准
    *@param[in]	传进来是光标位于表格视图中的位置
    */
    void on_resultTab_pressed(const QModelIndex &index);
    void on_cleanBtn_clicked();//清空所有统计
    void resetTabview();//初始化或重置模型
};

#endif // MAINFORM_H
