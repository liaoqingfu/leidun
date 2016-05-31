#include "DstSrConfig.h"
#include "ui_DstSrConfig.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>
#include <QTimer>
#include <QMessageBox>
#include <QDebug>

DstSrConfig::DstSrConfig(QWidget *parent) :
    QWidget(parent),ui(new Ui::DstSrConfig)
{
    ui->setupUi(this);
    loaddstConfig();
    this->resize(170,350);//重新窗口设置大小，源始大小为170*340
    connect(ui->applyBtn,&QPushButton::clicked,this,&DstSrConfig::saveTempConfig);
    connect(ui->delBtn,&QPushButton::clicked,this,&DstSrConfig::deldstConfig);
    connect(ui->nameCbBox,&QComboBox::editTextChanged,this,&DstSrConfig::addDefaultConfig);
    connect(ui->nameCbBox,&QComboBox::currentTextChanged,this,&DstSrConfig::setdstSensor);
}
DstSrConfig::~DstSrConfig()
{
    delete ui;
    savedstConfig();//退出时保存所有配置
}
void DstSrConfig::loaddstConfig()
{
    QString fileName = "DstSrConfig.xml";
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
                if((reader.name()=="dst")&&(reader.attributes().hasAttribute("ID")))
                {
                    QStringRef Temp=reader.attributes().value("ID");
                    dstName=Temp.toString();
                    if((false==dstList.contains(dstName))&&(false==dstName.isEmpty()))
                    {//可能有重名的传感器不重复添加,传感器名为空也不添加
                        dstNameMap[dstName]=dstName;
                        dstList.append(dstName);
                        this->ui->nameCbBox->addItem(dstName);
                        qDebug()<<dstName<<"传感器配置中...";
                    }else
                        qDebug()<<dstName<<"传感器已存在...";
                }else if(reader.name()=="name")
                {
                    dstName=reader.readElementText();
                    if((false==dstList.contains(dstName))&&(false==dstName.isEmpty()))
                    {//可能有重名的传感器不重复添加,传感器名为空也不添加
                        dstNameMap[dstName]=dstName;
                        dstList.append(dstName);
                        this->ui->nameCbBox->addItem(dstName);
                        qDebug()<<dstName<<"传感器配置中...";
                    }else
                        qDebug()<<dstName<<"传感器已配置完成...";
                }
                else if(reader.name()=="portId")
                {
                    portId=reader.readElementText();
                    portIdMap[dstName]=portId;
                    if((false==portIdList.contains(portId))&&(false==portId.isEmpty()))
                    {//可能有有复用同一端口的传感器不重复添加,为空也不添加
                        portIdList.append(portId);
                        this->ui->portIdCbBox->addItem(portId);
                    }else
                        qDebug()<<"已添加过端口"<<portId;
                }else if(reader.name()=="baudRate")
                {
                    baudRate=reader.readElementText();
                    baudRateMap[dstName]=baudRate;
                }else if(reader.name()== "dataBits")
                {
                    dataBits=reader.readElementText();
                    dataBitsMap[dstName]=dataBits;
                }else if(reader.name()=="parity")
                {
                    parity=reader.readElementText();
                    parityMap[dstName]=parity;
                }else if(reader.name()=="stopBits")
                {
                    stopBits=reader.readElementText();
                    stopBitsMap[dstName]=stopBits;
                }else if(reader.name()=="rwDelay")
                {
                    rwDelay=reader.readElementText();
                    rwDelayMap[dstName]=rwDelay;
                }else if(reader.name()=="dstId")
                {
                    dstId=reader.readElementText();
                    dstIdMap[dstName]=dstId;
                }else if(reader.name()=="rwItval")
                {
                    rwItval=reader.readElementText();
                    rwItvalMap[dstName]=rwItval;
                }else if(reader.name()=="rwAds")
                {
                    rwAds=reader.readElementText();
                    rwAdsMap[dstName]=rwAds;
                }
                else
                {
                    qDebug()<<"本地配置读取进行中...";
                }
            }
        }
        qDebug()<<"本地配置读取完成";
        setdstSensor(dstName);//配置界面为最后添加的传感器
        file.close();
    }
}
void DstSrConfig::savedstConfig()
{
    QString fileName = "DstSrConfig.xml";
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
        writer.writeStartElement("DstSensorConfig_List");
        for (int i = 0; i <dstList.count(); i++)
        {
            QString dId=dstList.at(i);
            //先取出对应键值
            dstName=dstNameMap[dId];
            portId=portIdMap[dId];
            baudRate=baudRateMap[dId];
            parity=parityMap[dId];
            dataBits=dataBitsMap[dId];
            stopBits=stopBitsMap[dId];
            dstId=dstIdMap[dId];
            rwDelay=rwDelayMap[dId];
            rwAds=rwAdsMap[dId];
            rwItval=rwItvalMap[dId];
            qDebug()<<dstId<<portId<<baudRate<<parity<<dataBits<<stopBits<<rwDelay<<rwItval<<rwAds;
            //再写入保存
            writer.writeStartElement("dst");
            writer.writeAttribute("Id", dId);
            writer.writeTextElement("name", dstName);
            writer.writeTextElement("dstId",dstId);
            writer.writeTextElement("portId",portId);
            writer.writeTextElement("baudRate",baudRate);
            writer.writeTextElement("dataBits",dataBits);
            writer.writeTextElement("parity",parity);
            writer.writeTextElement("stopBits",stopBits);
            writer.writeTextElement("rwDelay",rwDelay);
            writer.writeTextElement("rwItval",rwItval);
            writer.writeTextElement("rwAds",rwAds);
            writer.writeEndElement();
        }
        writer.writeEndDocument();
        qDebug()<<"本地配置保存完成";
        file.close();
    }
}
void DstSrConfig::setdstSensor(const QString& dId){

    //先取出对应键值
    dstName=dstNameMap[dId];
    portId=portIdMap[dId];
    baudRate=baudRateMap[dId];
    parity=parityMap[dId];
    dataBits=dataBitsMap[dId];
    stopBits=stopBitsMap[dId];
    dstId=dstIdMap[dId];
    rwAds=rwAdsMap[dId];
    rwDelay=rwDelayMap[dId];
    rwItval=rwItvalMap[dId];
    qDebug()<<dstName<<"更新配置界面...";
    //设置对应参数设置框
    this->ui->nameCbBox->setCurrentText(dstName);
    this->ui->portIdCbBox->setCurrentText(portId);
    this->ui->baudRateCbBox->setCurrentText(baudRate);
    this->ui->dataBitsCbBox->setCurrentText(dataBits);
    this->ui->parityCbBox->setCurrentText(parity);
    this->ui->stopBitsCbBox->setCurrentText(stopBits);
    this->ui->rwDelaySpBox->setValue(rwDelay.toInt());
    this->ui->dstIdLineEdit->setText(dstId);
    this->ui->rwAdsSpBox->setValue(rwAds.toInt());
    this->ui->rwItvalSpBox->setValue(rwItval.toInt());
}
void DstSrConfig::saveTempConfig()
{
    dstName=this->ui->nameCbBox->currentText();
    dstNameMap[dstName]=dstName;
    baudRate=this->ui->baudRateCbBox->currentText();
    baudRateMap[dstName]=baudRate;
    dataBits=this->ui->dataBitsCbBox->currentText();
    dataBitsMap[dstName]=dataBits;
    parity=this->ui->parityCbBox->currentText();
    parityMap[dstName]=parity;
    stopBits=this->ui->stopBitsCbBox->currentText();
    stopBitsMap[dstName]=stopBits;
    quint32 dl=this->ui->rwDelaySpBox->value();
    rwDelay= QString("%1").arg(dl);//进行简单的转型
    rwDelayMap[dstName]=rwDelay;
    dstId=this->ui->dstIdLineEdit->text();
    dstIdMap[dstName]=dstId;
    quint32 itVal=this->ui->rwItvalSpBox->value();
    rwItval=QString("%1").arg(itVal);
    rwItvalMap[dstName]=rwItval;
    quint32 dAds=this->ui->rwAdsSpBox->value();
    rwAds=QString("%1").arg(dAds);
    rwAdsMap[dstName]=rwAds;
    portId=this->ui->portIdCbBox->currentText();
    portIdMap[dstName]=portId;
    if(false==portIdList.contains(portId))
    {//检测当前的端口是否新添加的
        portIdList.append(portId);
        this->ui->portIdCbBox->addItem(portId);
        qDebug()<<portId<<"端口添加成功，已保存临时配置！";
    }
    qDebug()<<"保存"<<dstName<<"传感器临时配置"<<dstId<<portId<<baudRate<<parity<<dataBits<<stopBits<<rwDelay<<rwItval<<rwAds;
}
void DstSrConfig::addDefaultConfig()
{
    dstName=this->ui->nameCbBox->currentText();
    while(false==dstList.contains(dstName)&&(dstName!="dst-")&&(false==dstName.isEmpty()))
    {//简单判断输入
        if(QMessageBox::Yes==QMessageBox::question(this,"请选择",QString("是否添加%1传感器？").arg(dstName),QMessageBox::Yes|QMessageBox::No|QMessageBox::Yes))
        {
            this->ui->nameCbBox->addItem(dstName);
            dstList.append(dstName);
            dstNameMap[dstName]=dstName;
            portId=this->ui->portIdCbBox->currentText();
            portIdMap[dstName]=portId;
            baudRate=this->ui->baudRateCbBox->currentText();
            baudRateMap[dstName]=baudRate;
            dataBits=this->ui->dataBitsCbBox->currentText();
            dataBitsMap[dstName]=dataBits;
            parity=this->ui->parityCbBox->currentText();
            parityMap[dstName]=parity;
            stopBits=this->ui->stopBitsCbBox->currentText();
            stopBitsMap[dstName]=stopBits;
            quint32 dl=this->ui->rwDelaySpBox->value();
            rwDelay= QString("%1").arg(dl);//进行简单的转型
            rwDelayMap[dstName]=rwDelay;
            dstId=this->ui->dstIdLineEdit->text();
            dstIdMap[dstName]=dstId;
            quint32 dAds=this->ui->rwAdsSpBox->value();
            rwAds=QString("%1").arg(dAds);
            rwAdsMap[dstName]=rwAds;
            quint32 itVal=this->ui->rwItvalSpBox->value();
            rwItval=QString("%1").arg(itVal);
            rwItvalMap[dstName]=rwItval;
            qDebug()<<"添加新的传感器"<<dstName<<"加载默认配置"<<dstId<<portId<<baudRate<<parity<<dataBits<<stopBits<<rwDelay<<rwItval<<rwAds;
            return;
        }else
            qDebug()<<"已放弃添加";
        return;
    }
}
void DstSrConfig::deldstConfig()
{//获取当前传感器名字及位置
    dstName=this->ui->nameCbBox->currentText();
    int index=this->ui->nameCbBox->currentIndex();
    while(true==dstList.contains(dstName))
    {//简单判断输入
        if(QMessageBox::Yes==QMessageBox::question(this,"请选择",QString("是否删除%1传感器？").arg(dstName),QMessageBox::Yes|QMessageBox::No|QMessageBox::Yes))
        {//清除所有dst传感器参数
            dstList.removeAt(index);
            dstNameMap[dstName].clear();
            portIdMap[dstName].clear();
            baudRateMap[dstName].clear();
            dataBitsMap[dstName].clear();
            parityMap[dstName].clear();
            stopBitsMap[dstName].clear();
            rwDelayMap[dstName].clear();
            dstIdMap[dstName].clear();
            rwAdsMap[dstName].clear();
            rwItvalMap[dstName].clear();
            this->ui->nameCbBox->removeItem(index);
            return;
        }else
            qDebug()<<"已放弃删除"<<dstName<<"传感器";
        return;
    }
}
