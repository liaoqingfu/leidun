/**
  *@brief       主界面类声明文件
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
#include "MainForm.h"
#include "ui_MainForm.h"
#include <QDateTime>
#include <QTime>
#include <QVBoxLayout>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QValueAxis>
#include <log/QsLog.h>

MainForm::MainForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainForm),duration(0),pId(0),running(true)
{
    ui->setupUi(this);
    /*以下为样板线条*/
    QLineSeries *series = new QLineSeries();
    series->setName(QString("Line %1").arg(pId));
    series->append(0, 0);
    series->append(.25, .25);
    series->append(.5, .5);
    series->append(.75, .75);
    series->append(1, 1);
    series->append(1.25, 1.25);
    *series << QPointF(1.5, 1.5) << QPointF(1.75, 1.75) << QPointF(2, 2);

    chart = new QChart();
    chart->addSeries(series);
    //    chart->setAnimationOptions(QChart::SeriesAnimations);//设置曲线呈动画显示

    QValueAxis *axisX = new QValueAxis; //定义X轴
    axisX->setRange(0, 2); //设置范围
    axisX->setLabelFormat("%.1f"); //设置刻度的格式
    axisX->setTitleText("距离（mm）"); //设置X轴的标题
    axisX->setGridLineVisible(true); //设置是否显示网格线
    axisX->setMinorTickCount(2); //设置小刻度线的数目,最小为2
    axisX->setLabelsVisible(true); //设置刻度是否显示

    QValueAxis *axisY = new QValueAxis;//定义Y轴
    axisY->setRange(0, 2);
    axisY->setTitleText("压力（N）");
    axisY->setLabelFormat("%.2f");
    axisY->setMinorTickCount(2);
    axisY->setGridLineVisible(true);

    chart->setAxisX(axisX, series);
    chart->setAxisY(axisY, series);
    chart->legend()->hide();

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    this->ui->vLay->setGeometry(QRect(50,195,600,600));
    this->ui->vLay->addWidget(chartView,0,0);
    /*以上为样板线条*/

     /*以下为系统时间计数器*/
    systimer = new QTimer(this);
    systimer->setInterval(100);
    connect(systimer,&QTimer::timeout,[=]()
    {
        duration+=1000;
        QDateTime dateTime = QDateTime::currentDateTime();
        QString Str;
        QTime currentTime((duration/3600000)%60,(duration/60000)%60,duration/1000%60);
        Str = currentTime.toString("hh:mm:ss");
        ui->curtTimeLb->setText(dateTime.toString("yyyy-MM-dd hh:mm:ss ddd"));
        ui->lcdTime->display(Str);
    });
    systimer->start(1000); //1000ms触发一次
     /*以上为系统时间计数器*/

     /*以下为检测时间计数器*/
    timer = new QTimer(this);
    timer->setInterval(100);
    connect(ui->runPauseBtn,&QPushButton::clicked,this,runPause);
    connect(timer,&QTimer::timeout,[=]()
    {
        updateLines(pId);
        QLOG_INFO() << "正在检测产品："<<pId;
        chart->setTitle(QString("过程力曲线【当前产品-%1】").arg(pId));
        pId++;
    });
    timer->start(1000); //1000ms触发一次
     /*以上为检测时间计数器*/

    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowTitleHint|Qt::SubWindow);//去除边框
}

MainForm::~MainForm()
{
    delete ui;
}
void MainForm::updateLines(int productId)//最好传来一组数据进行解析更新图表
{//以下是写死的数据,正常应该是采集的数据反馈
    chartView->clearFocus();
    chart->removeAllSeries();//清空之前所有线
    QLineSeries *series = new QLineSeries();
    series->setName(QString("Line %1").arg(productId));
    series->append(0, 0);
    //     series->append(.25, .25);
    series->append(.5, .5);
    //     series->append(.75, .75);
    series->append(1, 1);
    //     series->append(1.25, 1.25);
    *series << QPointF(1.5, 1.5) << QPointF(1.75, 1.75) << QPointF(2, 2);
    chart->addSeries(series);
}
void MainForm::runPause()
{//暂停/继续运行槽函数
    if(this->running)
    {
        ui->runPauseBtn->setText("开始");
        this->running=false;
        this->timer->stop();
        QLOG_WARN() << "已暂停检测产品！";
    }else
    {
        ui->runPauseBtn->setText("暂停");
        this->running=true;
        this->timer->start(1000);
        QLOG_WARN() << "已继续检测产品！";
    }
}
