/**
  *@brief       主界面类声明文件
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

#include "MainForm.h"
#include "ui_MainForm.h"
#include "UI/DstSrConfig.h"
#include "UI/PlcSrConfig.h"
#include <QDateTime>
#include <QLibrary>
#include <QTime>
#include <QVBoxLayout>
#include <log/QsLog.h>
#include <QMessageBox>

#pragma execution_character_set("utf-8")

MainForm::MainForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainForm),duration(0),running(true),num(1),basicStr("0"),avgNum(20.0),okNum(0),ngNum(0),allNum(0),precent(0.00)
{
    ui->setupUi(this);

    DstSrConfig *dstwidget=new DstSrConfig(this);
    PlcSrConfig *plcwidget=new PlcSrConfig(this);
    spListScan=new SerialPortList(200);
    mtDlg=new MonitorDlg(this);
    mtDlg->setGeometry(200,200,235,265);
    spListScan->ScanStart();//开启自动检测串口

    /**
    \* 以下为测量统计表格
     */
    resetTabview();
    /**
     * 以上为测量统计表格
     * \
      */

    connect(spListScan, &SerialPortList::onNewSerialPort,dstwidget,&DstSrConfig::onNewPortList);//更新传感器对应界面的串口数据
    connect(spListScan, &SerialPortList::onNewSerialPort,plcwidget,&PlcSrConfig::onNewPortList);
    connect(ui->ctrlBtn,&QPushButton::clicked,mtDlg,&MonitorDlg::show);
    connect(mtDlg,&MonitorDlg::setSrToPort,dstwidget,&DstSrConfig::openSrToPort);//打开对应传感器设定中的串口
    connect(dstwidget,&DstSrConfig::updateMtrName,mtDlg,&MonitorDlg::UpdateMtrName);//打开对应传感器设定中的串口

    dstwidget->resize(170,380);
    plcwidget->resize(170,380);
    dstwidget->move(680,300);
    plcwidget->move(860,300);
    connect(mtDlg,&MonitorDlg::addMtr,this,&MainForm::addMt);
    connect(mtDlg,&MonitorDlg::addGrp,this,&MainForm::addGp);
    connect(dstwidget,&DstSrConfig::flushMtrData,this,&MainForm::flushMtrData);
    connect(this,&MainForm::clearModel,dstwidget,&DstSrConfig::clearAllModel);
    connect(this,&MainForm::clearModel,mtDlg,&MonitorDlg::clearAllModel);


    /**
    \* 以下为系统时间计数器
     */
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
        ui->nameLb->setText(curUser);
        newList=dstwidget->dstList+plcwidget->plcList;
        if(newList!=srList)
        {
            mtDlg->UpdateSensor(newList);
            QLOG_INFO() << "已更新传感器列表！";
            srList=newList;
        }
    });
    systimer->start(1000); //1000ms触发一次
    /*以上为系统时间计数器*/

    /*以下为检测时间计数器*/
    timer = new QTimer(this);
    timer->setInterval(100);
    connect(ui->runPauseBtn,&QPushButton::clicked,this,&MainForm::runPause);
    connect(timer,&QTimer::timeout,[=]()
    {
        dstwidget->readMyCom();//读取数据
    });
    timer->start(1000); //1000ms触发一次
    /*以上为检测时间计数器*/

    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowTitleHint|Qt::SubWindow);//去除边框

    connect(this,&MainForm::countData,this,&MainForm::upCountData);//更新数据
}

MainForm::~MainForm()
{
    delete ui;
}

void MainForm::runPause()
{//暂停/继续运行槽函数
    if(true==this->running)
    {
        this->ui->runPauseBtn->setText("开始统计");
        this->running=false;
        this->timer->stop();
        QLOG_WARN() << "已暂停测量！";
    }else
    {
        ui->runPauseBtn->setText("暂停统计");
        this->running=true;
        this->timer->start(1000);
        QLOG_WARN() << "已继续测量！";
    }
}

void MainForm::addMt(QStringList curMtrList)
{//添加测量
    do{//追加行数
        QModelIndex rowAppend = model->index(curMtrList.count(), 0, QModelIndex());
        model->insertRow(num,rowAppend);
        num=curMtrList.count();
    }while(curMtrList.count()>num);
    QString did;
    if(false==curMtrList.isEmpty()){
        int row=0;
        foreach(QString i,curMtrList){
            QModelIndex index = model->index(row, 0, QModelIndex());
            did=i.left(2);
            model->setData(index, i);
            row++;
        }
    }else
        QLOG_WARN() << "添加的测量列表为空！";
}

void MainForm::addGp(QStringList curGrpList)
{//添加分组
    do{//追加行数
        QModelIndex rowAppend = model->index(curGrpList.count(), 1, QModelIndex());
        model->insertRow(num,rowAppend);
        num=curGrpList.count();
    }while(curGrpList.count()>num);
    QListIterator<QString> g(curGrpList);
    while (g.hasNext()) {
        for (int row = 0; row < curGrpList.count(); ++row) {
            QModelIndex index = model->index(row, 1, QModelIndex());
            model->setData(index, g.next());
        }
    }
}

void MainForm::flushMtrData(QStringList dIdList,QStringList mtrDataList)
{//更新收到的数据，先判定数据是否为不正常的数据，然后对比差值来判断OK/NG
    foreach (QString tData, mtrDataList) {
        QString id=tData.left(2);//id号
        QString data=tData.remove(0,2);//源始数据主体
        bool ok;
        float sNum=data.toFloat(&ok);//源始数据转浮点型
        float basNum=basicStr.toFloat(&ok);//基准值浮点型
        float ctNum=sNum-basNum;//差值是带有正负号的
        float absNum=sNum>basNum?(sNum-basNum):(basNum-sNum);//绝对为正的差值
        int count;
        if(data=="+EEE.EEEE") {
            QLOG_WARN() << "传感器放大器处于错误状态！";
            continue;
        }
        else if(data=="+999.9999")
        {
            QLOG_WARN() << "检测值超过上限！";
            continue;
        }
        else if(data=="-999.9999")
        {
            QLOG_WARN() << "检测值低于下限！";
            continue;
        }
        else if(data=="-999.9998")
        {
            QLOG_WARN() << "传感器读取数据越界！";
            continue;
        }
        else if(dIdList.contains(id))
        {
            QLOG_INFO() << "待更新到界面的传感器Id为:"<<id;
            count=dIdList.indexOf(id);
            QModelIndex index = model->index(count, 2, QModelIndex());
            model->setData(index, data);
            QModelIndex index2 = model->index(count, 3, QModelIndex());
            QModelIndex index3 = model->index(count, 4, QModelIndex());
            model->setData(index2, ctNum);
            if(absNum<avgNum)
            {
                okNum++;
                allNum++;
                precent=100*okNum/allNum;
                model->setData(index3, "OK");
                emit countData(okNum,ngNum,allNum,precent);
                QLOG_INFO() << "该传感器传回数据正常！";
            }else
            {

                ngNum++;
                allNum++;
                precent=100-100*ngNum/allNum;
                model->setData(index3, "NG");
                emit countData(okNum,ngNum,allNum,precent);
                QLOG_INFO() << "该传感器传回数据异常！";
            }
        }
    }
    QLOG_INFO() << "待更新到界面的数据为:"<<dIdList<<mtrDataList;
}

void MainForm::on_resultTab_pressed(const QModelIndex &index)
{//选为基准
    QModelIndex curMtr = model->index(index.row(), 0, QModelIndex());//当前测量名的位置
    QString curMtrName=model->data(curMtr).toString();//当前测量名
    if((index.column()==2)&&(false==curMtrName.isEmpty()))
    {
        QString curReadData=model->data(index).toString();
        QLOG_INFO() << "当前选中值:"<<curReadData;
        if(QMessageBox::Yes==QMessageBox::question(this,"请选择",QString("是否将%1的基准值设为%2？").arg(curMtrName).arg(curReadData),QMessageBox::Yes|QMessageBox::No|QMessageBox::Yes))
        {
            basicStr=curReadData;
            QLOG_INFO() << "已更新"<<curMtrName<<"基准值为:"<<curReadData;
        }else
        {
            QLOG_INFO() << "已取消更新基准值！";
        }
    }
}

void MainForm::on_cleanBtn_clicked()
{//清除所有测量
    this->running=false;
    this->timer->stop();
    this->ui->runPauseBtn->setText("开始统计");
    /**
     * 清除当前数据模型
     * */
    model->clear();
    emit clearModel();
    okNum=0;
    ngNum=0;
    allNum=0;
    precent=0.00;
    emit countData(okNum,ngNum,allNum,precent);
    /**
     * 清除当前数据模型
     * */
    num=1;//初始化表格只显示一横行
    resetTabview();
}
void MainForm::resetTabview()
{//重置模型数据
    model = new QStandardItemModel(num,5);
    model->setHeaderData(0, Qt::Horizontal, tr("测量名称"));
    model->setHeaderData(1, Qt::Horizontal, tr("分组状态"));
    model->setHeaderData(2, Qt::Horizontal, tr("实时读数"));
    model->setHeaderData(3, Qt::Horizontal, tr("测量结果"));
    model->setHeaderData(4, Qt::Horizontal, tr("判定结果"));
    ui->resultTab->setModel(model);
}
void MainForm::upCountData(float okNum, float ngNum, float allNum, float precent)
{
    QString pct=QString("%1").arg(precent)+QString("%");
    this->ui->countLb->setText(QString("%1").arg(allNum));
    this->ui->hegeLb->setText(QString("%1").arg(okNum));
    this->ui->buhegeLb->setText(QString("%1").arg(ngNum));
    this->ui->pctlvLb->setText(pct);
}
