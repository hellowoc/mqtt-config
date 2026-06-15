#include "autoctrlboardipsetwidget.h"
#include "ui_autoctrlboardipsetwidget.h"
#include <QSettings>
#include "global.h"
#include <QtNetwork>

AutoCtrlBoardIPSetWidget::AutoCtrlBoardIPSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::AutoCtrlBoardIPSetWidget)
{
    ui->setupUi(this);

    m_nCtrlBoardPort = struCnfg.nAutoBoardPort;
    m_nModbusIP =  struCnfg.nModbusIP;
    m_nBaudRate =  struCnfg.nBaudRate;

    initUi();

    connect(mySerial2Recv, SIGNAL(signalCtrlBoardIpPortQuery()), this, SLOT(updateSlt()));
    //connect(myFlow.myNetWork8083->udprecvthread, SIGNAL(signalCtrlBoardIpPortQuery()), this, SLOT(updateSlt()));
}

AutoCtrlBoardIPSetWidget::~AutoCtrlBoardIPSetWidget()
{
    delete ui;
}

void AutoCtrlBoardIPSetWidget::showPage(bool bshow)
{
    if(bshow){
        updateUi();
    }
}

void AutoCtrlBoardIPSetWidget::retranslateUiWidget()
{
    ui->groupBox->setTitle(g_myLan().ids_autoControlBoard + g_myLan().ids_set);
    ui->groupBox_2->setTitle("Modbus" + g_myLan().ids_set);
    ui->IpqueryBtn->setText(g_myLan().ids_query);
    ui->IpSetBtn->setText(g_myLan().ids_apply);
    ui->ModbusSetBtn->setText(g_myLan().ids_apply);

    ui->label->setText(g_myLan().auto_ctrl_board_ip + QString(":"));
    ui->label_2->setText(g_myLan().auto_ctrl_board_port + QString(":"));
    ui->label_3->setText(g_myLan().gateway + QString(":"));
    ui->label_4->setText(g_myLan().subnet_mask + QString(":"));
    ui->label_5->setText(g_myLan().ip_address + QString(":"));
    ui->label_6->setText(g_myLan().baudrate + QString(":"));
}

void AutoCtrlBoardIPSetWidget::initUi()
{
    ui->IP_LineEdit->setType(textType);
    ui->Gateway_LineEdit->setType(textType);
    ui->SubnetMask_LineEdit->setType(textType);

    ui->Port_LineEdit->setType(intType);
    ui->Port_LineEdit->setMinMax(this,1,65535, 0, &m_nCtrlBoardPort);
    ui->Port_LineEdit->setText(QString("%1").arg(m_nCtrlBoardPort));

    ui->BaudRateCmbox->setCurrentIndex(m_nBaudRate);
    ui->BaudRateCmbox->setStyleSheet("QComboBox::item { font:30px \"Arial\", height: 45px; }");

    ui->IP_LineEdit_2->setType(intType);
    ui->IP_LineEdit_2->setMinMax(this,1,247, 2, &m_nModbusIP);
    ui->IP_LineEdit_2->setText(QString("%1").arg(m_nModbusIP));

}

void AutoCtrlBoardIPSetWidget::updateSlt()
{
    updateUi();
}

void AutoCtrlBoardIPSetWidget::updateUi()
{
    QString ipStr;
    ipStr.sprintf("%d.%d.%d.%d", struCnfg.nAutoBoardIpAddr[0], struCnfg.nAutoBoardIpAddr[1], struCnfg.nAutoBoardIpAddr[2], struCnfg.nAutoBoardIpAddr[3]);
    ui->IP_LineEdit->setText(ipStr);

    QString portStr;
    portStr.sprintf("%d", struCnfg.nAutoBoardPort);
    ui->Port_LineEdit->setText(portStr);

    QString ipStr_Gateway;
    ipStr_Gateway.sprintf("%d.%d.%d.%d", struCnfg.nGateway[0], struCnfg.nGateway[1], struCnfg.nGateway[2], struCnfg.nGateway[3]);;
    ui->Gateway_LineEdit->setText(ipStr_Gateway);

    QString ipStr_SubnetMask;
    ipStr_SubnetMask.sprintf("%d.%d.%d.%d", struCnfg.nSubnetMask[0], struCnfg.nSubnetMask[1], struCnfg.nSubnetMask[2], struCnfg.nSubnetMask[3]);;
    ui->SubnetMask_LineEdit->setText(ipStr_SubnetMask);

    ui->BaudRateCmbox->setCurrentIndex(struCnfg.nBaudRate);

    QString ipStr_Modbus;
    ipStr_Modbus.sprintf("%d", struCnfg.nModbusIP);
    ui->IP_LineEdit_2->setText(ipStr_Modbus);
}

void AutoCtrlBoardIPSetWidget::on_IpqueryBtn_clicked()
{
    struCnfg.bUpdateStatus = 1;  //心跳锁
    struCnfg.nAutoBoardIpAddr[0] = 0;
    struCnfg.nAutoBoardIpAddr[1] = 0;
    struCnfg.nAutoBoardIpAddr[2] = 0;
    struCnfg.nAutoBoardIpAddr[3] = 0;
    struCnfg.nAutoBoardPort = 0;

    struCnfg.nGateway[0] = 0;                             //网关 4段IP 默认192.168.1.1
    struCnfg.nGateway[1] = 0;
    struCnfg.nGateway[2] = 0;
    struCnfg.nGateway[3] = 0;

    struCnfg.nSubnetMask[0] = 0;
    struCnfg.nSubnetMask[1] = 0;
    struCnfg.nSubnetMask[2] = 0;
    struCnfg.nSubnetMask[3] = 0;


    char data[SEND_PACKET_RECTRL_BOARD_SIZE];
    memset(data, 0, SEND_PACKET_RECTRL_BOARD_SIZE);
    mySerial.pushCom2CmdData(CMD_CTRL_BOARD_IP_PORT_QUERY, 0, BOARD_TYPE_CONTROL, 0x0e, data, 3);

    logger()->info("Query Ip and Port");
    g_infoWidget().setLabelText(g_myLan().ids_querying);
    g_infoWidget().delayShow();
    myFlow.sleep(3);
    g_infoWidget().hide();

    updateUi();
    struCnfg.bUpdateStatus = 0;  //心跳开
}

void AutoCtrlBoardIPSetWidget::on_IpSetBtn_clicked()
{
    struCnfg.bUpdateStatus = 1;  //心跳锁
    char data[SEND_PACKET_RECTRL_BOARD_SIZE];
    memset(data, 0, SEND_PACKET_RECTRL_BOARD_SIZE);

    QStringList IPList = ui->IP_LineEdit->text().split(".");
    QStringList IPList_Gateway = ui->Gateway_LineEdit->text().split(".");
    QStringList IPList_SubnetMask = ui->SubnetMask_LineEdit->text().split(".");

    logger()->info("Set Ip and Port");
    struCnfg.nAutoBoardIpAddr[0] = IPList[0].toInt();
    struCnfg.nAutoBoardIpAddr[1] = IPList[1].toInt();
    struCnfg.nAutoBoardIpAddr[2] = IPList[2].toInt();
    struCnfg.nAutoBoardIpAddr[3] = IPList[3].toInt();
    struCnfg.nAutoBoardPort = m_nCtrlBoardPort;

    struCnfg.nGateway[0] = IPList_Gateway[0].toInt();
    struCnfg.nGateway[1] = IPList_Gateway[1].toInt();
    struCnfg.nGateway[2] = IPList_Gateway[2].toInt();
    struCnfg.nGateway[3] = IPList_Gateway[3].toInt();

    struCnfg.nSubnetMask[0] = IPList_SubnetMask[0].toInt();
    struCnfg.nSubnetMask[1] = IPList_SubnetMask[1].toInt();
    struCnfg.nSubnetMask[2] = IPList_SubnetMask[2].toInt();
    struCnfg.nSubnetMask[3] = IPList_SubnetMask[3].toInt();

    data[0] = struCnfg.nAutoBoardIpAddr[0];
    data[1] = struCnfg.nAutoBoardIpAddr[1];
    data[2] = struCnfg.nAutoBoardIpAddr[2];
    data[3] = struCnfg.nAutoBoardIpAddr[3];
    data[4] = struCnfg.nAutoBoardPort/256;
    data[5] = struCnfg.nAutoBoardPort%256;

    data[6] = struCnfg.nGateway[0];
    data[7] = struCnfg.nGateway[1];
    data[8] = struCnfg.nGateway[2];
    data[9] = struCnfg.nGateway[3];
    data[10] = struCnfg.nSubnetMask[0];
    data[11] = struCnfg.nSubnetMask[1];
    data[12] = struCnfg.nSubnetMask[2];
    data[13] = struCnfg.nSubnetMask[3];


    mySerial.pushCom2CmdData(CMD_CTRL_BOARD_IP_PORT_SET, 0, BOARD_TYPE_CONTROL, 0x0e, data, 3);

    g_infoWidget().setLabelText(g_myLan().msg_applying);
    g_infoWidget().delayShow();
    myFlow.sleep(3);
    g_infoWidget().hide();
    struCnfg.bUpdateStatus = 0;  //心跳开
    return;
}

void AutoCtrlBoardIPSetWidget::on_ModbusSetBtn_clicked()
{
    struCnfg.bUpdateStatus = 1;  //心跳锁
    char data[SEND_PACKET_RECTRL_BOARD_SIZE];
    memset(data, 0, SEND_PACKET_RECTRL_BOARD_SIZE);

    logger()->info("Set Modbus Ip and baudRate");
    struCnfg.nModbusIP = m_nModbusIP;
    struCnfg.nBaudRate = m_nBaudRate;
    data[0] = struCnfg.nModbusIP;
    data[1] = struCnfg.nBaudRate;

    mySerial.pushCom2CmdData(CMD_PLC_MODBUS_SET, 0, BOARD_TYPE_CONTROL, 0x0e, data, 3);

    g_infoWidget().setLabelText(g_myLan().msg_applying);
    g_infoWidget().delayShow();
    myFlow.sleep(3);
    g_infoWidget().hide();
    struCnfg.bUpdateStatus = 0;  //心跳锁
    return;
}

void AutoCtrlBoardIPSetWidget::on_BaudRateCmbox_activated(int index)
{
    m_nBaudRate = index;
}

