/**
  *@brief       更新距离传感器配置界面
  *@author      yuenar
  *@date        16-04-01
  *@version     0.13
  *@note        @16-03-28 yuenar v0.1  初版，大致设计布局,初始参数及数据结构
  *@note        @16-03-29 yuenar v0.11 增加载入、保存、设定函数实现
  *@note        @16-03-31 yuenar v0.12 增加添加及删除传感器、串口实现
  *@warning     调用时，注意合理布局
  *@copyright   leidun.co
  */
#ifndef DSTSRCONFIG_H
#define DSTSRCONFIG_H

#include "dstsrconfig_global.h"
#include <QWidget>
#include <QString>
#include <QMap>

namespace Ui {
class DstSrConfig;
}
/**
 * @class       dst传感器配置窗口类
 * @brief       操作工通过该窗口进行距离传感器设定，读取配置、保存配置、添加、删除新的传感器及串口等操作
 * @author      yuenar
 * @date        16-03-28
 */

class DSTSRCONFIGSHARED_EXPORT DstSrConfig: public QWidget
{

public:
    DstSrConfig(QWidget *parent = 0);
    ~DstSrConfig();
public slots:
    void saveTempConfig();//保存临时配置
signals:
    void UIStatusChanged();//留坑，界面更新的信号，预计扩展传参数给下位机
private:
    Ui::DstSrConfig *ui;
    int dstSpId;//留坑，距离传感器特有ID标识，具体情况再说
    QStringList dstList;//读取配置文件的所有可用dst列表
    QStringList portIdList;//读取配置文件的可用端口列表
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

private slots:
    void loaddstConfig();//载入配置文件
    void savedstConfig();//保存配置文件
    void addDefaultConfig();//添加新的传感器调用
    void deldstConfig();//删除选中传感器或端口

    /**
    *@brief		设定布局显示名为did的传感器配置
    *@remark	条码规则的指针和内存共用
    *@param[in]	距离传感器名称did
    *@retval	配置该距离传感器的参数更新到界面
    */
    void setdstSensor(const QString& dId);
};

#endif // DSTSRCONFIG_H
