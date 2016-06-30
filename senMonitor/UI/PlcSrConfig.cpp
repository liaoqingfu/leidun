#include "PlcSrConfig.h"
#include "ui_PlcSrConfig.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>
#include <QTimer>
#include <QMessageBox>
#include <log/QsLog.h>

#pragma execution_character_set("utf-8")

PlcSrConfig::PlcSrConfig(QWidget *parent) :
    QWidget(parent),ui(new Ui::PlcSrConfig)
{
    ui->setupUi(this);
    loadPlcConfig();
//    this->resize(200,380);//重新窗口设置大小，源始大小为170*340
    connect(ui->applyBtn,&QPushButton::clicked,this,&PlcSrConfig::saveTempConfig);
    connect(ui->delBtn,&QPushButton::clicked,this,&PlcSrConfig::delPlcConfig);
    connect(ui->nameCbBox,&QComboBox::editTextChanged,this,&PlcSrConfig::addDefaultConfig);
    connect(ui->nameCbBox,&QComboBox::currentTextChanged,this,&PlcSrConfig::setPlcSensor);
}

PlcSrConfig::~PlcSrConfig()
{
    delete ui;
    savePlcConfig();//退出时保存所有配置
}

void PlcSrConfig::loadPlcConfig()
{
    QString fileName = "PlcSrConfig.xml";
    QFile file(fileName);
    if (false==file.open(QFile::ReadOnly | QFile::Text))
    {
        return;
    }
    else
    {
        QXmlStreamReader reader;
        reader.setDevice(&file);
        while (false==reader.atEnd())
        {
            reader.readNext();
            if(reader.isStartElement())
            {
                if((reader.name()=="plc")&&(reader.attributes().hasAttribute("ID")))
                {
                    QStringRef Temp=reader.attributes().value("ID");
                    plcName=Temp.toString();
                    if((false==plcList.contains(plcName))&&(false==plcName.isEmpty()))
                    {//可能有重名的传感器不重复添加,传感器名为空也不添加
                        plcNameMap[plcName]=plcName;
                        plcList.append(plcName);
                        this->ui->nameCbBox->addItem(plcName);
                        QLOG_INFO()<<plcName<<"传感器配置中...";
                    }else
                        QLOG_INFO()<<plcName<<"传感器已存在...";
                }else if(reader.name()=="name")
                {
                    plcName=reader.readElementText();
                    if((false==plcList.contains(plcName))&&(false==plcName.isEmpty()))
                    {//可能有重名的传感器不重复添加,传感器名为空也不添加
                        plcNameMap[plcName]=plcName;
                        plcList.append(plcName);
                        this->ui->nameCbBox->addItem(plcName);
                        QLOG_INFO()<<plcName<<"传感器配置中...";
                    }else
                        QLOG_INFO()<<plcName<<"传感器已配置完成...";
                }
                else if(reader.name()=="portId")
                {
                    portId=reader.readElementText();
                    portIdMap[plcName]=portId;
                    if((false==portIdList.contains(portId))&&(false==portId.isEmpty()))
                    {//可能有有复用同一端口的传感器不重复添加,为空也不添加
                        portIdList.append(portId);
                        this->ui->portIdCbBox->addItem(portId);
                    }else
                        QLOG_INFO()<<"已添加过端口"<<portId;
                }else if(reader.name()=="baudRate")
                {
                    baudRate=reader.readElementText();
                    baudRateMap[plcName]=baudRate;
                }else if(reader.name()== "dataBits")
                {
                    dataBits=reader.readElementText();
                    dataBitsMap[plcName]=dataBits;
                }else if(reader.name()=="parity")
                {
                    parity=reader.readElementText();
                    parityMap[plcName]=parity;
                }else if(reader.name()=="stopBits")
                {
                    stopBits=reader.readElementText();
                    stopBitsMap[plcName]=stopBits;
                }else if(reader.name()=="rwDelay")
                {
                    rwDelay=reader.readElementText();
                    rwDelayMap[plcName]=rwDelay;
                }else if(reader.name()=="type")
                {
                    type=reader.readElementText();
                    typeMap[plcName]=type;
                }else if(reader.name()=="rwLen")
                {
                    rwLen=reader.readElementText();
                    rwLenMap[plcName]=rwLen;
                }
                else
                {
                    QLOG_INFO()<<"本地配置读取进行中...";
                }
            }
        }
    }
    QLOG_INFO()<<"本地配置读取完成";
    setPlcSensor(plcName);//配置界面为最后添加的传感器
    file.close();
}

void PlcSrConfig::savePlcConfig()
{
    QString fileName = "PlcSrConfig.xml";
    QFile file(fileName);
    if(false==file.open(QIODevice::ReadWrite|QIODevice::Truncate))
    {
        return;
    }
    else
    {
        QXmlStreamWriter writer(&file);
        writer.setAutoFormatting(true);
        writer.writeStartDocument();// 写文档头
        writer.writeStartElement("PlcSensorConfig_List");
        for (int i = 0; i <plcList.count(); i++)
        {
            QString pid=plcList.at(i);
            //先取出对应键值
            plcName=plcNameMap[pid];
            portId=portIdMap[pid];
            baudRate=baudRateMap[pid];
            parity=parityMap[pid];
            dataBits=dataBitsMap[pid];
            stopBits=stopBitsMap[pid];
            type=typeMap[pid];
            rwDelay=rwDelayMap[pid];
            rwLen=rwLenMap[pid];
            QLOG_INFO()<<portId<<baudRate<<parity<<dataBits<<stopBits<<type<<rwDelay<<rwLen;
            //再写入保存
            writer.writeStartElement("plc");
            writer.writeAttribute("ID", pid);
            writer.writeTextElement("name", plcName);
            writer.writeTextElement("portId",portId);
            writer.writeTextElement("baudRate",baudRate);
            writer.writeTextElement("dataBits",dataBits);
            writer.writeTextElement("parity",parity);
            writer.writeTextElement("stopBits",stopBits);
            writer.writeTextElement("rwDelay",rwDelay);
            writer.writeTextElement("type",type);
            writer.writeTextElement("rwLen",rwLen);
            writer.writeEndElement();
        }
        writer.writeEndDocument();
        QLOG_INFO()<<"本地配置保存完成";
        file.close();
    }
}

void PlcSrConfig::setPlcSensor(const QString& pId)
{//设定传感器
    //先取出对应键值
    plcName=plcNameMap[pId];
    portId=portIdMap[pId];
    baudRate=baudRateMap[pId];
    parity=parityMap[pId];
    dataBits=dataBitsMap[pId];
    stopBits=stopBitsMap[pId];
    type=typeMap[pId];
    rwDelay=rwDelayMap[pId];
    rwLen=rwLenMap[pId];
    QLOG_INFO()<<plcName<<"更新配置界面...";
    //设置对应参数设置框
    this->ui->nameCbBox->setCurrentText(plcName);
    this->ui->portIdCbBox->setCurrentText(portId);
    this->ui->baudRateCbBox->setCurrentText(baudRate);
    this->ui->dataBitsCbBox->setCurrentText(dataBits);
    this->ui->parityCbBox->setCurrentText(parity);
    this->ui->stopBitsCbBox->setCurrentText(stopBits);
    this->ui->delaySpBox->setValue(rwDelay.toInt());
    this->ui->typeCbBox->setCurrentText(type);
    this->ui->rwLenSpBox->setValue(rwLen.toInt());
}

void PlcSrConfig::saveTempConfig()
{//每次更改后临时保存
    plcName=this->ui->nameCbBox->currentText();
    plcNameMap[plcName]=plcName;
    baudRate=this->ui->baudRateCbBox->currentText();
    baudRateMap[plcName]=baudRate;
    dataBits=this->ui->dataBitsCbBox->currentText();
    dataBitsMap[plcName]=dataBits;
    parity=this->ui->parityCbBox->currentText();
    parityMap[plcName]=parity;
    stopBits=this->ui->stopBitsCbBox->currentText();
    stopBitsMap[plcName]=stopBits;
    quint32 dl=this->ui->delaySpBox->value();
    rwDelay= QString("%1").arg(dl);//进行简单的转型
    rwDelayMap[plcName]=rwDelay;
    type=this->ui->typeCbBox->currentText();
    typeMap[plcName]=type;
    quint32 Lth=this->ui->rwLenSpBox->value();
    rwLen=QString("%1").arg(Lth);
    rwLenMap[plcName]=rwLen;
    portId=this->ui->portIdCbBox->currentText();
    portIdMap[plcName]=portId;
    if(false==portIdList.contains(portId))
    {//检测当前的端口是否新添加的
        portIdList.append(portId);
        this->ui->portIdCbBox->addItem(portId);
        QLOG_INFO()<<portId<<"端口添加成功，已保存临时配置！";
    }
    QLOG_INFO()<<"保存"<<plcName<<"传感器临时配置"<<portId<<baudRate<<parity<<dataBits<<stopBits<<type<<rwDelay<<rwLen;
}

void PlcSrConfig::addDefaultConfig()
{
    plcName=this->ui->nameCbBox->currentText();
    while(false==plcList.contains(plcName)&&(plcName!="PLC-")&&(false==plcName.isEmpty()))
    {//简单判断输入
        if(QMessageBox::Yes==QMessageBox::question(this,"请选择",QString("是否添加%1传感器？").arg(plcName),QMessageBox::Yes|QMessageBox::No|QMessageBox::Yes))
        {
            this->ui->nameCbBox->addItem(plcName);
            plcList.append(plcName);
            plcNameMap[plcName]=plcName;
            portId=this->ui->portIdCbBox->currentText();
            portIdMap[plcName]=portId;
            baudRate=this->ui->baudRateCbBox->currentText();
            baudRateMap[plcName]=baudRate;
            dataBits=this->ui->dataBitsCbBox->currentText();
            dataBitsMap[plcName]=dataBits;
            parity=this->ui->parityCbBox->currentText();
            parityMap[plcName]=parity;
            stopBits=this->ui->stopBitsCbBox->currentText();
            stopBitsMap[plcName]=stopBits;
            quint32 dl=this->ui->delaySpBox->value();
            rwDelay= QString("%1").arg(dl);//进行简单的转型
            rwDelayMap[plcName]=rwDelay;
            type=this->ui->typeCbBox->currentText();
            typeMap[plcName]=type;
            quint32 Lth=this->ui->rwLenSpBox->value();
            rwLen=QString("%1").arg(Lth);
            rwLenMap[plcName]=rwLen;
            QLOG_INFO()<<"添加新的传感器"<<plcName<<"加载默认配置"<<portId<<baudRate<<parity<<dataBits<<stopBits<<type<<rwDelay<<rwLen;
            return;
        }else
            QLOG_INFO()<<"已放弃添加";
        return;
    }
}

void PlcSrConfig::delPlcConfig()
{//获取当前传感器名字及位置
    plcName=this->ui->nameCbBox->currentText();
    int index=this->ui->nameCbBox->currentIndex();
    while(true==plcList.contains(plcName))
    {//简单判断输入
        if(QMessageBox::Yes==QMessageBox::question(this,"请选择",QString("是否删除%1传感器？").arg(plcName),QMessageBox::Yes|QMessageBox::No|QMessageBox::Yes))
        {//清除所有PLC传感器参数
            plcList.removeAt(index);
            plcNameMap[plcName].clear();
            portIdMap[plcName].clear();
            baudRateMap[plcName].clear();
            dataBitsMap[plcName].clear();
            parityMap[plcName].clear();
            stopBitsMap[plcName].clear();
            rwDelayMap[plcName].clear();
            typeMap[plcName].clear();
            rwLenMap[plcName].clear();
            this->ui->nameCbBox->removeItem(index);
            return;
        }else
            QLOG_INFO()<<"已放弃删除"<<plcName<<"传感器";
        return;
    }
}

void PlcSrConfig::onNewPortList(QStringList portNames)
{//更新系统已检测到的串口列表
    this->ui->portIdCbBox->clear();
    this->ui->portIdCbBox->addItems(portNames);
}
