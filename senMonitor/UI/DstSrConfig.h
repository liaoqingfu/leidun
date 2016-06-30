/**
  *@brief       距离传感器配置头文件
  *@author      yuenar
  *@date        16-06-27
  *@version     0.21
  *@note        @16-03-28 yuenar v0.1  初版，大致设计布局,初始参数及数据结构
  *@note        @16-03-29 yuenar v0.11 增加载入、保存、设定函数实现
  *@note        @16-03-31 yuenar v0.12 增加添加及删除传感器、串口实现
  *@note        @16-06-04 yuenar v0.13 添加ext串口相关库和槽函数
  *@note        @16-06-12 yuenar v0.14 增加解析串口数据函数原型
  *@note        @16-06-16 yuenar v0.15 格式化指令
  *@note        @16-06-20 yuenar v0.16 模拟测试串口，更新串口配置函数
  *@note        @16-06-21 yuenar v0.17 模拟解析响应函数，正常响应处理
  *@note        @16-06-23 yuenar v0.19 尝试匹配数据更新到UI界面
  *@note        @16-06-24 yuenar v0.20 增加清空模型数据槽函数
  *@note        @16-06-27 yuenar v0.21 解析数据更新到UI及打印异常
  *@warning     调用时，注意合理布局
  *@copyright   leidun.co
  */
#ifndef DSTSRCONFIG_H
#define DSTSRCONFIG_H


#include <QWidget>
#include <QString>
#include "Uart/qextserialport.h"
#include <QMap>
#include <QList>
#include <QSet>

namespace Ui {
class DstSrConfig;
}
/**
 * @class       dst传感器配置窗口类
 * @brief       操作工通过该窗口进行距离传感器设定，读取配置、保存配置、添加、删除新的传感器及串口等操作
 * @author      yuenar
 * @date        16-03-28
 */

class DstSrConfig: public QWidget
{
    Q_OBJECT

public:
    explicit DstSrConfig(QWidget *parent = 0);
    ~DstSrConfig();

    QStringList dstList;//读取配置文件的所有可用dst列表
    QStringList portIdList;//读取配置文件的可用端口列表
    QStringList dIdList;//已添加测量的传感器id列表，更新数据要用的

signals:
    void flushMtrData(QStringList,QStringList);//刷新检测数据
    void updateMtrName(QString);//更新检测名字，在后面加上（dstId）
private:
    Ui::DstSrConfig *ui;
    int dstSpId;//留坑，距离传感器特有ID标识，具体情况再说
    int num;//已打开串口总数

    QList <QextSerialPort *>myComs;//存放串口容器
    QextSerialPort *myCom;//串口指针
    QStringList myComList;//已打开的串口名称列表，为了方便查找取出已经打开的串口
    QStringList mtrDataList;//解析过的测量数据传给UI部分

    /*
     * 响应及异常处理思路
     *
     * 先取响应的头部看是正常响应或者异常
     * 然后将尾部信息进行解析及处理
     * 将处理完的信息反馈到UI界面上
     * 具体内容参见手册
    */
    QString replyHead;//响应头部数据
    QString replyRear;//响应尾部数据
    QString errHead;//异常头部信息
    QString errRear;//异常尾部信息
    QString replyId;//ID编号，占位2字节
    QString replyDataId;//数据编号，占位3字节
    QString replyData;//响应数据
    QString errData;//异常数据

    /*
     * 传感器配置相关数据结构
    */
    QString dstName;//dst型号
    QString portId;//端口号
    QString baudRate;//波特率
    QString parity;//校检位
    QString dataBits;//数据位
    QString stopBits;//停止位
    QString dstId;//ID号设定
    QString rwDelay;//读写超时时间
    QString rwItval;//读写间隔时间
    QString rwAds;//读写地址
    QMap<QString,QString>  dstNameMap;
    QMap<QString,QString>  portIdMap;
    QMap<QString,QString>  baudRateMap;
    QMap<QString,QString>  dataBitsMap;
    QMap<QString,QString>  parityMap;
    QMap<QString,QString>  stopBitsMap;
    QMap<QString,QString>  rwDelayMap;
    QMap<QString,QString>  rwItvalMap;
    QMap<QString,QString>  rwAdsMap;
    QMap<QString,QString>  dstIdMap;

public slots:
    void saveTempConfig();//保存临时配置

    /**
    *@brief		打开串口
    *@param[in]	传进来是传感器名字
    *@retval	打开该距离传感器对应的串口
    */
    void openSrToPort(QString dstName);

    /**
    *@brief		更新端口列表到界面
    *@param[in]	传进来的所有端口列表
    *@retval	配置该距离传感器的参数更新到界面
    */
    void onNewPortList(QStringList portNames);

    /**
    *@brief		发送第一段指令
    *@param[in]	格式化的指令
    *@retval	正常响应或者异常
    */
    void sendFirstCmd(QString cmd);

    /*以下为解析数据处理部分*/
    /**
    *@brief		发送数据
    *@param[in]	传进来指令
    *@retval	向串口发送数据
    */
    void sendToCom(QString cmd);
    void clearAllModel();//清除所有数据模型
    void readMyCom();//读数据

    /**
    *@brief		解析串口读出的响应
    *@param[in]	传进来的是串口全部响应数据
    *@retval	将响应数据主体分情况处理
    */
    void parseData(QString rdata);

    /**
    *@brief		解析串口读出的异常
    *@param[in]	传进来的是去除“ER”开头的异常主体信息
    *@retval	将响应数据主体分情况处理
    */
    void parseError(QString rdata);

    /**
    *@brief		解析串口读出的响应数据主体
    *@param[in]	传进来的是SR命令的响应数据主体
    *@retval	解析数据反馈到UI界面
    */
    void parseSrReply(QString replyBody);

    /**
    *@brief		解析串口读出的响应数据主体
    *@param[in]	传进来的是M0命令的响应数据主体
    *@retval	解析数据反馈到UI界面
    */
    void parseM0Reply(QString replyBody);

    /**
    *@brief		解析串口读出的响应数据主体
    *@param[in]	传进来的是MS命令的响应数据主体
    *@retval	解析数据反馈到UI界面
    */
    void parseMsReply(QString replyBody);

    /**
    *@brief		解析串口读出的响应数据主体
    *@param[in]	传进来的是SW命令的响应数据主体
    *@retval	解析数据反馈到UI界面
    */
    void parseSwReply(QString replyBody);

    /**
    *@brief		解析串口读出的响应数据主体
    *@param[in]	传进来的是AW命令的响应数据主体
    *@retval	解析数据反馈到UI界面
    */
    void parseAwReply(QString replyBody);

    /**
    *@brief		解析串口读出的响应数据主体
    *@param[in]	传进来的是DR命令的响应数据主体
    *@retval	解析数据反馈到UI界面
    */
    void parseDrReply(QString replyBody);
    /*以上为解析数据处理部分*/

private slots:
    void loaddstConfig();//载入配置文件
    void savedstConfig();//保存配置文件
    void addDefaultConfig();//添加新的传感器调用
    void deldstConfig();//删除选中传感器或端口


    /**
    *@brief		设定布局显示名为did的传感器配置
    *@remark	条码规则的指针和内存共用
    *@param[in]	距离传感器名称did
    *@retval	配置该距离传感器的参数更新到界面并尝试打开串口
    */
    void setdstSensor(const QString& dId);
};

#endif // DSTSRCONFIG_H
