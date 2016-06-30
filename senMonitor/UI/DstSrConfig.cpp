/**
  *@brief       距离传感器配置声明文件
  *@author      yuenar
  *@date        16-06-27
  *@version     0.20
  *@note        @16-03-28 yuenar v0.10 初版，大致设计布局,初始参数及数据结构
  *@note        @16-03-29 yuenar v0.11 增加载入、保存、设定函数实现
  *@note        @16-03-31 yuenar v0.12 增加添加及删除传感器、串口实现
  *@note        @16-06-04 yuenar v0.13 添加ext串口相关库和槽函数
  *@note        @16-06-12 yuenar v0.14 增加解析串口数据函数原型
  *@note        @16-06-16 yuenar v0.15 格式化指令
  *@note        @16-06-20 yuenar v0.16 模拟测试串口，更新串口配置函数
  *@note        @16-06-21 yuenar v0.17 模拟解析响应函数，正常响应处理
  *@note        @16-06-23 yuenar v0.18 尝试匹配数据更新到UI界面
  *@note        @16-06-24 yuenar v0.19 增加清空模型数据槽函数
  *@note        @16-06-27 yuenar v0.20 解析数据更新到UI及打印异常
  *@warning     调用时，注意合理布局
  *@copyright   leidun.co
  */

#include "DstSrConfig.h"
#include "ui_DstSrConfig.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>
#include <QTimer>
#include <QMessageBox>
#include <log/QsLog.h>

#pragma execution_character_set("utf-8")

const int z_Num=199;//整数部分
const int p_Num=9999;//小数部分
/**
 * 格式化指令开始
*/
/*
 * @brief 从特定的传感器放大器中读取[SR指令]
 * @param 参数%1：ID编号（2个数字，ASCII字符）
 * @param 参数%2：数据编号（3个数字，ASCII字符）
 */
const QString SrCmd = "SR,%1,%2CRLF";

/*
 * @brief 从所有的传感器放大器中读取所有数据[M0指令]
 */
const QByteArray M0Cmd = "M0CRLF";

/*
 * @brief 从所有的传感器放大器中读取状态和数据[MS指令]
 */
const QByteArray MsCmd = "MSCRLF";


/*
 * @brief 写入特定的传感器放大器[SW指令]
 * @param 参数%1：ID编号（2个数字，ASCII字符）
 * @param 参数%2：数据编号（3个数字，ASCII字符）
 * @param 参数%3：设置数据（N个数字，根据被写入的数据的长度不同，数据长度也不同，ASCII字符）
 */
const QString SwCmd = "SW,%1,%2,%3,CRLF";


/*
 * @brief 写入所有传感器放大器[AW指令]
 * @param 参数%1：数据编号（3个数字，ASCII字符）
 * @param 参数%2：设置数据（N个数字，根据被写入的数据的长度不同，数据长度也不同，ASCII字符）
 */
const QString AwCmd = "AW,%1,%2,CRLF";
/**
 * 格式化指令完毕
*/

DstSrConfig::DstSrConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DstSrConfig),num(0),replyHead(""),replyRear("")
{
    ui->setupUi(this);
    loaddstConfig();
    //    this->resize(200,380);//重新窗口设置大小，源始大小为170*340
    connect(ui->applyBtn,&QPushButton::clicked,this,&DstSrConfig::saveTempConfig);
    connect(ui->delBtn,&QPushButton::clicked,this,&DstSrConfig::deldstConfig);
    connect(ui->nameCbBox,&QComboBox::editTextChanged,this,&DstSrConfig::addDefaultConfig);
    connect(ui->nameCbBox,&QComboBox::currentTextChanged,this,&DstSrConfig::setdstSensor);
}

DstSrConfig::~DstSrConfig()
{
    delete ui;
    foreach (QextSerialPort *com, myComs) {
        while(com->isOpen()){
            com->close();
        }
        delete com;//删除串口
    }
    myComs.clear();
    QLOG_INFO()<<"已关闭所有串口";
    savedstConfig();//退出时保存所有配置
}

void DstSrConfig::loaddstConfig()
{//读取配置
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
                        QLOG_INFO()<<dstName<<"传感器配置中...";
                    }else
                        QLOG_INFO()<<dstName<<"传感器已存在...";
                }else if(reader.name()=="name")
                {
                    dstName=reader.readElementText();
                    if((false==dstList.contains(dstName))&&(false==dstName.isEmpty()))
                    {//可能有重名的传感器不重复添加,传感器名为空也不添加
                        dstNameMap[dstName]=dstName;
                        dstList.append(dstName);
                        this->ui->nameCbBox->addItem(dstName);
                        QLOG_INFO()<<dstName<<"传感器配置中...";
                    }else
                        QLOG_INFO()<<dstName<<"传感器已配置完成...";
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
                        QLOG_INFO()<<"已添加过端口"<<portId;
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
                    QLOG_INFO()<<"本地配置读取进行中...";
                }
            }
        }
        QLOG_INFO()<<"本地配置读取完成";
        setdstSensor(dstName);//配置界面为最后添加的传感器并打开对应的串口
        file.close();
    }
}

void DstSrConfig::savedstConfig()
{//保存配置
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
            QLOG_INFO()<<dstId<<portId<<baudRate<<parity<<dataBits<<stopBits<<rwDelay<<rwItval<<rwAds;
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
        QLOG_INFO()<<"本地配置保存完成";
        file.close();
    }
}

void DstSrConfig::setdstSensor(const QString& dId)
{//设定传感器
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
    QLOG_INFO()<<dstName<<"更新配置界面...";
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
    /**以下为打开串口及设定部分*/
#ifdef Q_OS_LINUX
    myCom = new QextSerialPort("/dev/" + portId);
#elif defined (Q_OS_WIN)
    myCom = new QextSerialPort(portId);
#endif
    connect(myCom, &QextSerialPort::readyRead, this,&DstSrConfig::readMyCom);
    if(false==myComList.contains(portId))
    {
        myComs.append(myCom);
        myComList.append(portId);
        QLOG_INFO()<<"打开并添加"<<portId<<"串口";

        //设置波特率
        myCom->setBaudRate((BaudRateType)baudRate.toInt());

        //设置数据位
        myCom->setDataBits((DataBitsType)dataBits.toInt());

        //设置校验
        switch(ui->parityCbBox->currentIndex()){
        case 0:
            myCom->setParity(PAR_NONE);
            break;
        case 1:
            myCom->setParity(PAR_ODD);
            break;
        case 2:
            myCom->setParity(PAR_EVEN);
            break;
        default:
            myCom->setParity(PAR_NONE);
            break;
        }

        //设置停止位
        switch(ui->stopBitsCbBox->currentIndex()){
        case 0:
            myCom->setStopBits(STOP_1);
            break;
        case 1:
#ifdef Q_OS_WIN
            myCom->setStopBits(STOP_1_5);
#endif
            break;
        case 2:
            myCom->setStopBits(STOP_2);
            break;
        default:
            myCom->setStopBits(STOP_1);
            break;
        }
        //设置数据流控制
        myCom->setFlowControl(FLOW_OFF);//无数据流控制
        //设置延时
        myCom->setTimeout(200);//设置延时
        if(myCom->open(QIODevice::ReadWrite)){
            QLOG_INFO()<<"串口打开成功!!";
        }else{
            QLOG_WARN()<<"打开失败，未能打开串口 " + portId + "\n该串口设备不存在或已被占用";
            return;
        }
        /**以上为打开串口及设定部分*/
        num++;
    }else
    { //设置波特率
        QLOG_INFO()<<"串口已经打开，请点击保存更新!!";
    }
}

void DstSrConfig::saveTempConfig()
{//保存临时配置
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
    if((false==portIdList.contains(portId))&&(false==myComList.contains(portId)))
    { /**
       \*检测当前的端口是否新添加的
        */
        portIdList.append(portId);
        this->ui->portIdCbBox->addItem(portId);
        QLOG_INFO()<<portId<<"端口添加成功，已保存临时配置！";
    }
    QLOG_INFO()<<"保存"<<dstName<<"传感器临时配置"<<dstId<<portId<<baudRate<<parity<<dataBits<<stopBits<<rwDelay<<rwItval<<rwAds;
    int n;
    if(myComList.contains(portId))
    {
        n=myComList.indexOf(portId);
        if(n>=0){
            myCom=myComs.at(n);//取出串口
            //设置波特率
            myCom->setBaudRate((BaudRateType)baudRate.toInt());

            //设置数据位
            myCom->setDataBits((DataBitsType)dataBits.toInt());

            //设置校验
            switch(ui->parityCbBox->currentIndex()){
            case 0:
                myCom->setParity(PAR_NONE);
                break;
            case 1:
                myCom->setParity(PAR_ODD);
                break;
            case 2:
                myCom->setParity(PAR_EVEN);
                break;
            default:
                myCom->setParity(PAR_NONE);
                break;
            }
            //设置停止位
            switch(ui->stopBitsCbBox->currentIndex()){
            case 0:
                myCom->setStopBits(STOP_1);
                break;
            case 1:
#ifdef Q_OS_WIN
                myCom->setStopBits(STOP_1_5);
#endif
                break;
            case 2:
                myCom->setStopBits(STOP_2);
                break;
            default:
                myCom->setStopBits(STOP_1);
                break;
            }
            //设置数据流控制
            myCom->setFlowControl(FLOW_OFF);//无数据流控制
            //设置延时
            myCom->setTimeout(200);//设置延时
            QLOG_INFO()<<"串口更新成功!!";
        }else
        {
            QLOG_INFO()<<"还未添加任何测量";
        }
    }else
    {
        QLOG_INFO()<<"还未打开"<<portId<<"串口，尝试打开中";
        /**以下为打开串口及设定部分*/
#ifdef Q_OS_LINUX
        myCom = new QextSerialPort("/dev/" + portId);
#elif defined (Q_OS_WIN)
        myCom = new QextSerialPort(portId);
#endif
        myComs.append(myCom);
        myComList.append(portId);
        QLOG_INFO()<<"打开并添加"<<portId<<"串口";
        //设置波特率
        myCom->setBaudRate((BaudRateType)baudRate.toInt());

        //设置数据位
        myCom->setDataBits((DataBitsType)dataBits.toInt());

        //设置校验
        switch(ui->parityCbBox->currentIndex()){
        case 0:
            myCom->setParity(PAR_NONE);
            break;
        case 1:
            myCom->setParity(PAR_ODD);
            break;
        case 2:
            myCom->setParity(PAR_EVEN);
            break;
        default:
            myCom->setParity(PAR_NONE);
            break;
        }

        //设置停止位
        switch(ui->stopBitsCbBox->currentIndex()){
        case 0:
            myCom->setStopBits(STOP_1);
            break;
        case 1:
#ifdef Q_OS_WIN
            myCom->setStopBits(STOP_1_5);
#endif
            break;
        case 2:
            myCom->setStopBits(STOP_2);
            break;
        default:
            myCom->setStopBits(STOP_1);
            break;
        }
        //设置数据流控制
        myCom->setFlowControl(FLOW_OFF);//无数据流控制
        //设置延时
        myCom->setTimeout(200);//设置延时
        if(myCom->open(QIODevice::ReadWrite)){
            QLOG_INFO()<<"串口打开成功!!";
        }else{
            QLOG_WARN()<<"打开失败，未能打开串口 " + portId + "\n该串口设备不存在或已被占用";
            return;
        }
        /**以上为打开串口及设定部分*/
        num++;
    }
}

void DstSrConfig::addDefaultConfig()
{//添加默认配置
    dstName=this->ui->nameCbBox->currentText();
    while(false==dstList.contains(dstName)&&(dstName!="dst-")&&(false==dstName.isEmpty()))
    {
        /**
         * 简单判断输入
         * 注意分别考虑
         *
       */
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
            QLOG_INFO()<<"添加新的传感器"<<dstName<<"加载默认配置"<<dstId<<portId<<baudRate<<parity<<dataBits<<stopBits<<rwDelay<<rwItval<<rwAds;
            return;
        }else
            QLOG_INFO()<<"已放弃添加";
        return;
    }
}

void DstSrConfig::deldstConfig()
{//移除传感器
    dstName=this->ui->nameCbBox->currentText();
    int index=this->ui->nameCbBox->currentIndex();//获取当前传感器名字及位置
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
            QLOG_INFO()<<"已放弃删除"<<dstName<<"传感器";
        return;
    }
}

void DstSrConfig::onNewPortList(QStringList portNames)
{//更新系统已检测的串口列表,并尝试打开
    this->ui->portIdCbBox->clear();
    this->ui->portIdCbBox->addItems(portNames);
}

void DstSrConfig::openSrToPort(QString dstName)
{//打开串口//Q1？统计数据是根据ID来反馈的吗
    this->ui->nameCbBox->setCurrentText(dstName);
    QString idName=this->ui->dstIdLineEdit->text();
    emit updateMtrName(idName);
    dIdList.append(idName);
    QLOG_INFO()<<tr("添加的%1对应的专属ID号为%2").arg(dstName).arg(idName);
}

/**
\* 模拟收发开始
 */
void DstSrConfig::readMyCom()
{ //读取数据,注意排错
    int demNum1= qrand()%z_Num;
    int demNum2= qrand()%p_Num;
    int demNum3= qrand()%z_Num;
    int demNum4= qrand()%p_Num;
    int demNum5= qrand()%z_Num;
    int demNum6= qrand()%p_Num;
    QByteArray ba = myCom->readAll();
    if(!ba.isEmpty())
    {
        QString le = ba;
        QLOG_INFO()<<"串口接收到数据:"<<le;
        parseData(le);//解析并反馈
    }else
    {
        parseData(QString("DR,1,00+%1.%2,2,01+%3.%4,0,02+%5.%6CRLF").arg(demNum1).arg(demNum2).arg(demNum3).arg(demNum4).arg(demNum5).arg(demNum6));
    }
//        QLOG_WARN()<<"串口未接收到数据！";
}

void DstSrConfig::sendToCom(QString cmd)
{//发送指令，注意将格式化指令补充完整
    char *pt ;
    int write_nu=0;
    QByteArray ba = cmd.toLatin1();
    pt = ba.data();
    write_nu = myCom->write(ba);
    QLOG_INFO()<<QObject::tr("写入命令 :%1 ").arg(cmd);
}
/**
\ * 模拟收发结束
 */

void DstSrConfig::parseData(QString rdata)
{//解析数据
    replyHead=rdata.section(",",0,0);//根据响应来判断是否异常
    replyRear=rdata.section(",",1,-1);//尾部数据
    if(replyHead=="ER")
    {
        parseError(replyRear);
    }else if(replyHead=="AW"){
        parseAwReply(replyRear);
    }
    else if(replyHead=="DR"){
        parseDrReply(replyRear);
    }else if(replyHead=="M0")
    {
        parseM0Reply(replyRear);
    }
    else if(replyHead=="MS"){
        parseMsReply(replyRear);
    }
    else if(replyHead=="SR")
    {
        parseSrReply(replyRear);
    }
    else if(replyHead=="SW")
    {
        parseSwReply(replyRear);
    }else
        QLOG_WARN()<<"串口未正常接收数据！";
}
void DstSrConfig::parseError(QString rdata)
{//处理异常
    errHead=rdata.section(",",0,0);//根据响应来判断是否异常
    errRear=rdata.section(",",1,-1);//尾部数据
    errData=errRear.section("CRLF",0,0);//去除尾部“CRLF”
    if(false==errHead.isEmpty()){//错误信息不为空
        QLOG_INFO()<<QObject::tr("捕获到%1指令异常,错误编号为：%2").arg(errHead).arg(errData);
    }else
        QLOG_WARN()<<"串口未正常捕获到异常信息！";
}

void DstSrConfig::sendFirstCmd(QString cmd)
{//写入第一条命令
    QLOG_INFO()<<"开始写入第一条命令";
    sendToCom(cmd);
}

void DstSrConfig::parseDrReply(QString replyBody)
{//DR指令响应
    QString str=replyBody.section(",",-1);//找出最后一段响应
    bool ok;
    QString id=str.left(2);
    int lastId=id.toInt(&ok,10);
    QString strData=replyBody.section("CRLF",0,0);
    QString firstCtrl=strData.section(",",0,0);
    int firstId=firstCtrl.left(2).toInt(&ok,10);
    int n=lastId-firstId+2;//获取传感器个数，n不要更改！！！
    mtrDataList.clear();//清空
    QStringList rList = strData.split(",");
    int i;
    for(i=0;i<2*n;i+=2)
    {//2*n不要随便改！！！
        QString rdata=rList[i]+","+rList[i+1];//带输出控制的数据段
        QString tdata=rdata.remove(0,2);//含传感器ID的数据段，传给界面部分进行匹配更新
        mtrDataList.append(tdata);
        QLOG_INFO()<<"接收DR指令响应:数据段内容为"<<tdata;
    }
    emit flushMtrData(dIdList,mtrDataList);
    QLOG_INFO()<<"最后一段传感器编号为"<<id<<"一共有"<<n<<"个传感器"<<"全部字符串内容为"<<mtrDataList;
}

void DstSrConfig::parseM0Reply(QString replyBody)
{//读取全部传感器的数据然后构建列表
    QString str=replyBody.section(",",-1);//找出最后一段响应
    bool ok;
    QString id=str.left(2);
    int lastId=id.toInt(&ok,10);
    int firstId=replyBody.left(2).toInt(&ok,10);
    int n=lastId-firstId+2;//获取传感器个数
    QString strData=replyBody.section("CRLF",0,0);
    mtrDataList.clear();//清空
    mtrDataList = strData.split(",");
    emit flushMtrData(dIdList,mtrDataList);
    QLOG_INFO()<<"最后一段传感器编号为"<<id<<"一共有"<<n<<"个传感器"<<"全部字符串内容为"<<mtrDataList;
}

void DstSrConfig::parseMsReply(QString replyBody)
{//MS指令响应
    QString str=replyBody.section(",",-1);//找出最后一段响应
    bool ok;
    QString id=str.left(2);
    int lastId=id.toInt(&ok,10);
    QString strData=replyBody.section("CRLF",0,0);
    QString firstCtrl=strData.section(",",0,0);
    int firstId=firstCtrl.left(2).toInt(&ok,10);
    int n=lastId-firstId+2;//获取传感器个数
    mtrDataList.clear();//清空
    QStringList rList = strData.split(",");
    int i;
    for(i=0;i<2*n;i+=2)
    {//2*n不要随便改！！！
        QString rdata=rList[i]+","+rList[i+1];
        QString tdata=rdata.remove(0,2);//含传感器ID的数据段，传给界面部分进行匹配更新
        mtrDataList.append(tdata);
        QLOG_INFO()<<"接收MS指令响应:数据段内容为"<<tdata;
    }
    emit flushMtrData(dIdList,mtrDataList);
    QLOG_INFO()<<"最后一段传感器编号为"<<id<<"一共有"<<n<<"个传感器";
}

void DstSrConfig::parseSrReply(QString replyBody)
{//SR指令响应
    mtrDataList.clear();//清空
    replyId=replyBody.section(",",0,0);
    replyDataId=replyBody.section(",",1,1);
    QString str=replyBody.section(",",2,-1);
    replyData=str.section("CRLF",0,0);
    QString tData=replyDataId+replyData;
    mtrDataList.append(tData);//含传感器ID的数据段，传给界面部分进行匹配更新
    emit flushMtrData(dIdList,mtrDataList);
    QLOG_INFO()<<"接收SR指令响应:ID编号为"<<replyId<<"数据编号为"<<replyDataId<<"数据为"<<replyData;
}

void DstSrConfig::parseSwReply(QString replyBody)
{//SW指令响应
    replyId=replyBody.section(",",0,0);
    QString str=replyBody.section(",",1,-1);
    replyDataId=str.section("CRLF",0,0);//
    QLOG_INFO()<<"接收SW指令响应:ID编号为"<<replyId<<"数据编号为"<<replyDataId;
}

void DstSrConfig::parseAwReply(QString replyBody)
{//AW指令响应
    replyDataId=replyBody.section("CRLF",0,0);
    QLOG_INFO()<<"接收AW指令响应:数据编号为"<<replyDataId;
}

void DstSrConfig::clearAllModel()
{//清除所有统计
    mtrDataList.clear();
    dIdList.clear();
}
