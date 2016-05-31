/**
  *@brief       更新PLC传感器配置界面
  *@author      yuenar
  *@date        16-04-01
  *@version     0.13
  *@note        @16-03-28 yuenar v0.1  初版，大致设计布局,初始参数及数据结构
  *@note        @16-03-29 yuenar v0.11 增加载入、保存、设定函数实现
  *@note        @16-03-31 yuenar v0.12 增加添加及删除传感器、串口实现
  *@warning     调用时，注意合理布局
  *@copyright   leidun.co
  */

#ifndef PLCSRCONFIG_H
#define PLCSRCONFIG_H

#include "plcsrconfig_global.h"
#include <QWidget>
#include <QString>
#include <QMap>

namespace Ui {
class PlcSrConfig;//UI设计师布局类
}
/**
 * @class       PLC传感器配置窗口类
 * @brief       操作工通过该窗口进行PLC传感器设定，读取配置、保存配置、添加、删除新的传感器及串口等操作
 * @author      yuenar
 * @date        16-03-28
 */

class PLCSRCONFIGSHARED_EXPORT PlcSrConfig: public QWidget
{

public:
    PlcSrConfig(QWidget *parent = 0);
    ~PlcSrConfig();
public slots:
    void saveTempConfig();//保存临时配置
signals:
    void UIStatusChanged();//留坑，界面更新的信号，预计扩展传参数给下位机
private:
    Ui::PlcSrConfig *ui;
    int plcSpId;//留坑，Plc传感器特有ID标识，具体情况再说
    QStringList plcList;//读取配置文件的所有可用PLC列表
    QStringList portIdList;//读取配置文件的可用端口列表
    QString plcName;//PLC型号
    QString portId;//端口号
    QString baudRate;//波特率
    QString parity;//校检位
    QString dataBits;//数据位
    QString stopBits;//
    QString type;//PLC类型
    QString rwDelay;//读写延迟时间
    QString rwLen;//读写长度
    QMap<QString,QString>  plcNameMap;
    QMap<QString,QString>  portIdMap;
    QMap<QString,QString>  baudRateMap;
    QMap<QString,QString>  dataBitsMap;
    QMap<QString,QString>  parityMap;
    QMap<QString,QString>  stopBitsMap;
    QMap<QString,QString>  rwDelayMap;
    QMap<QString,QString>  typeMap;
    QMap<QString,QString>  rwLenMap;
private slots:
    void loadPlcConfig();//载入配置文件
    void savePlcConfig();//保存配置文件
    void addDefaultConfig();//添加新的传感器调用
    void delPlcConfig();//删除选中传感器或端口

    /**
    *@brief		设定布局显示名为pid的传感器配置
    *@remark	条码规则的指针和内存共用
    *@param[in]	PLC传感器名称pid
    *@retval	配置该PLC传感器的参数更新到界面
    */
    void setPlcSensor(const QString& pId);
};

#endif // PLCSRCONFIG_H
