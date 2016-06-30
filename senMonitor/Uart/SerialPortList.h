/**
  *@brief       主界面类头文件
  *@author      yuenar
  *@date        16-06-04
  *@version     0.10
  *@note        @16-06-04 yuenar v0.10 用自带串口写扫描槽函数,仅作检测可用串口
  *@copyright   leidun.co
  */
#ifndef SERIALPORTLIST_H
#define SERIALPORTLIST_H

#include <QTimer>
#include <QDebug>
#include <QSerialPort>
#include <QStringList>
#include <QSerialPortInfo>

class SerialPortList : public QObject
{
    Q_OBJECT

public:
    SerialPortList();
    SerialPortList(quint16);
    ~SerialPortList();

    quint16 scanCycleMs;
    QTimer *timer;
    QStringList oldPortStringList;//临时串口列表

    void ScanStart();//开启扫描
    void ScanStop();//停止扫描

public slots:
    void onTimeOut();

signals:
    void onNewSerialPort(QStringList);//串口已更新的信号
};

#endif // SERIALPORTLIST_H
