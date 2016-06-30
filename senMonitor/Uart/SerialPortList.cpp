/**
  *@brief       主界面类声明文件
  *@author      yuenar
  *@date        16-06-04
  *@version     0.10
  *@note        @16-06-04 yuenar v0.10 用自带串口写扫描槽函数,仅作检测可用串口
  *@copyright   leidun.co
  */
#include "serialportlist.h"
#include <log/QsLog.h>

#pragma execution_character_set("utf-8")

SerialPortList::SerialPortList()
{
    timer = new QTimer;
    scanCycleMs = 200;
    oldPortStringList.clear();
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
}

SerialPortList::SerialPortList(quint16 CycleMs)
{ //自定义扫描周期构造
    timer = new QTimer;
    scanCycleMs = CycleMs;
    oldPortStringList.clear();
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
}

SerialPortList::~SerialPortList()
{
    delete timer;
}

void SerialPortList::ScanStart()
{//开始扫描
    timer->stop();
    timer->start(scanCycleMs);
}

void SerialPortList::ScanStop()
{  //停止扫描
    timer->stop();
}

void SerialPortList::onTimeOut()
{//周期扫描服务
    QStringList newPortStringList;
    //搜索串口
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
        //        QLOG_INFO() << "串口名: " << info.portName();
        //        QLOG_INFO() << "描述: " << info.description();
        //        QLOG_INFO() << "厂商: " << info.manufacturer();
        newPortStringList += info.portName();
    }

    if(newPortStringList.size() != oldPortStringList.size())
    {    //更新旧的串口列表
        oldPortStringList = newPortStringList;
        emit onNewSerialPort(oldPortStringList);
    }
}
