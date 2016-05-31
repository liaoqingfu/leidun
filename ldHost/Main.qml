/*!
  *@brief       系统控制面板界面
  *@author      yuenar
  *@date        16-05-27
  *@version     0.14
  *@note        @16-04-16 yuenar v0.11  初版，大致设计界面及鼠标事件
  *@note        @16-04-18 yuenar v0.12  自定义按钮悬停变化
  *@note        @16-04-20 yuenar v0.13  颜色变化设计
  *@note        @16-05-27 yuenar v0.14  修改解决BUG
  *@warning     关于布局的可编辑需要再分配
  *@copyright   leidun.co
  */
import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQml 2.2

ApplicationWindow{
    id:root
    visible: true
    width: 1024
    height: 768
    //设置无边框
    flags: Qt.FramelessWindowHint | Qt.WindowSystemMenuHint| Qt.WindowMinimizeBtnHint| Qt.Window
    property int  btnid

    Rectangle
    {//主界面按钮
        id:mainBtn
        x:0
        y:660
        implicitWidth: 146.5
        implicitHeight: 108
        color: (btnid==0)?"lightblue":"blue"
        border.width: 1
        border.color: "#345678"

        Text{
            text:qsTr("主界面");
            color: "white"
            font.family: "微软雅黑"
            font.bold: true
            font.pixelSize: 30
            anchors.centerIn: parent
        }
        MouseArea{
            hoverEnabled :true
            onClicked: {
                mainUI.show();
                btnid=0;
            }
            anchors.fill: parent
            onContainsMouseChanged: containsMouse?(parent.height+=10):(parent.height=108)//鼠标悬停改动大小以突出
        }
    }
    Rectangle
    {//标准设置按钮
        id:setBtn
        x:146.5
        y:660
        implicitWidth: 146.5
        implicitHeight: 108
        border.width: 1
        border.color: "#345678"
        color: (btnid==1)?"lightblue":"blue"
        Text{
            text:qsTr("标准设置");
            color: "white"
            font.family: "微软雅黑"
            font.bold: true
            font.pixelSize: 30
            anchors.centerIn: parent
        }
        MouseArea{
            hoverEnabled :true
            onClicked: {
                mainUI.hide();
                btnid=1;
            }
            anchors.fill: parent
            onContainsMouseChanged: containsMouse?(parent.height+=10):(parent.height=108)
        }
    }
    Rectangle
    {//历史记录按钮
        id:historyBtn
        x:293
        y:660
        implicitWidth: 146.5
        implicitHeight: 108
        border.width: 1
        border.color: "#345678"
        color: (btnid==2)?"lightblue":"blue"
        Text{
            text:qsTr("历史记录");
            color: "white"
            font.family: "微软雅黑"
            font.bold: true
            font.pixelSize: 30
            anchors.centerIn: parent
        }
        MouseArea{
            hoverEnabled :true
            onClicked: {
                mainUI.hide();
                btnid=2;
            }
            anchors.fill: parent
            onContainsMouseChanged: containsMouse?(parent.height+=10):(parent.height=108)
        }
    }
    Rectangle
    {//传感器按钮
        id:sensorBtn
        x:439.5
        y:660
        implicitWidth: 146.5
        implicitHeight: 108
        border.width: 1
        border.color: "#345678"
        color: (btnid==3)?"lightblue":"blue"
        Text{
            text:qsTr("传感器校准");
            color: "white"
            font.family: "微软雅黑"
            font.bold: true
            font.pixelSize: 30
            anchors.centerIn: parent
        }
        MouseArea{
            hoverEnabled :true
            onClicked: {

                mainUI.hide();
                btnid=3;
            }
            anchors.fill: parent
            onContainsMouseChanged: containsMouse?(parent.height+=10):(parent.height=108)
        }
    }
    Rectangle
    {//运行日志按钮
        id:logBtn
        x:586
        y:660
        implicitWidth: 146.5
        implicitHeight: 108
        border.width: 1
        border.color: "#345678"
        focus: false
        color: (btnid==4)?"lightblue":"blue"
        Text{
            text:qsTr("查看日志");
            color: "white"
            font.family: "微软雅黑"
            font.bold: true
            font.pixelSize: 30
            anchors.centerIn: parent
        }
        MouseArea{
            hoverEnabled :true
            onClicked: {
                mainUI.hide();
                btnid=4;
            }
            anchors.fill: parent
            onContainsMouseChanged: containsMouse?(parent.height+=10):(parent.height=108)
        }
    }
    Rectangle
    {//运行日志按钮
        id:helpBtn
        x:730.5
        y:660
        implicitWidth: 146.5
        implicitHeight: 108
        border.width: 1
        border.color: "#345678"
        color: (btnid==5)?"lightblue":"blue"
        Text{
            text:qsTr("帮助说明");
            color: "white"
            font.family: "微软雅黑"
            font.bold: true
            font.pixelSize: 30
            anchors.centerIn: parent
        }
        MouseArea{
            hoverEnabled :true
            onClicked: {
                mainUI.hide();
                btnid=5;
            }
            anchors.fill: parent
            onContainsMouseChanged: containsMouse?(parent.height+=10):(parent.height=108)
        }
    }
    Rectangle
    {//退出系统
        id:exitBtn
        x:877
        y:660
        implicitWidth: 147
        implicitHeight: 108
        border.width: 1
        border.color: "#345678"
        color: "blue"
        Text{
            text:qsTr("退出系统");
            color: "white"
            font.family: "微软雅黑"
            font.bold: true
            font.pixelSize: 30
            anchors.centerIn: parent
        }
        MouseArea{
            hoverEnabled :true
            onClicked: {Qt.quit();}
            anchors.fill: parent
            onContainsMouseChanged: containsMouse?parent.color = "lightblue":parent.color = "blue"
        }
    }
}
