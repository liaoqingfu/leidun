/**
  *@brief       主界面类头文件
  *@author      yuenar
  *@date        16-05-30
  *@version     0.18
  *@note        @16-04-21 yuenar v0.11 初版，大致设计布局
  *@note        @16-04-22 yuenar v0.12 添加计时器信息
  *@note        @16-04-25 yuenar v0.13 添加其他布局
  *@note        @16-04-28 yuenar v0.14 添加图表及模型数据
  *@note        @16-05-08 yuenar v0.15 添加线型表更新函数
  *@note        @16-05-27 yuenar v0.16 完善及测试线表更新函数
  *@note        @16-05-30 yuenar v0.17 添加日志系统及相关函数
  *@note        @16-05-31 yuenar v0.18 添加暂停/继续检测按钮及函数
  *@warning     调用时，注意合理布局
  *@copyright   leidun.co
  */
#ifndef MAINFORM_H
#define MAINFORM_H

#include <QWidget>
#include <QTimer>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

/*include chart start */
QT_CHARTS_USE_NAMESPACE
/*include chart end */

namespace Ui {
class MainForm;
}

class MainForm : public QWidget
{
    Q_OBJECT

public:
    explicit MainForm(QWidget *parent = 0);
    ~MainForm();
    QTimer *systimer;//运行时间计数器
    QTimer *timer;//检测时间计数器
    bool running;//是否在运行

private:
    Ui::MainForm *ui;
    qint64 duration;
    int pId;//当前产品号

    QChart *chart;
    QChartView *chartView;
private slots:
    /**
     * @class       线型表更新类
     * @brief       更新线数据槽函数,productId代表产品号对应当前产品号
     * @author      yuenar
     * @date        16-05-08
     */
    void updateLines(int productId);
    void runPause();//暂停或继续检测产品槽函数
};

#endif // MAINFORM_H
