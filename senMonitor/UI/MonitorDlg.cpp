/*!
  *@brief       测量对话框声明文件
  *@author      yuenar
  *@date        16-06-24
  *@version     0.12
  *@note        @16-06-03 yuenar v0.10  初版，大致设计界面及及相关槽函数
  *@note        @16-06-23 yuenar v0.11  更新测量名并发送列表
  *@note        @16-06-24 yuenar v0.12 增加清空模型数据槽函数
  *@warning     关于布局的可编辑需要再分配
  *@copyright   leidun.co
  */
#include "MonitorDlg.h"
#include "ui_MonitorDlg.h"
#include <log/QsLog.h>

#pragma execution_character_set("utf-8")

MonitorDlg::MonitorDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MonitorDlg)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Popup);
}

MonitorDlg::~MonitorDlg()
{
    delete ui;
}

void MonitorDlg::on_addBtn_clicked()
{//添加测量

    curGrpName.clear();
    curSrName.clear();
    curGrpName=ui->mtGroupCbBox->currentText();
    curSrName=ui->srNameCbBox->currentText();
    grpList.append(curGrpName);
    emit addGrp(grpList);
    emit setSrToPort(curSrName);
}

void MonitorDlg::on_cancelBtn_clicked()
{//取消按钮
    this->hide();
    QLOG_INFO() << "已取消添加测量";
}

void MonitorDlg::UpdateSensor(QStringList srList)
{//更新传感器配置
    ui->srNameCbBox->clear();
    QListIterator<QString> i(srList);
    while (i.hasNext()) {
        ui->srNameCbBox->addItem(i.next());
    }
}

void MonitorDlg::UpdateMtrName(QString idName)
{//更新测量名
    curMtrName.clear();
    curMtrName=ui->namelEdit->text();
    QString newName=curMtrName+"("+idName+")";
    mtrList.append(newName);
    emit addMtr(mtrList);
    QLOG_INFO() << "正在添加测量"<<curMtrName;
}
void MonitorDlg::clearAllModel()
{
    mtrList.clear();
    grpList.clear();
}
