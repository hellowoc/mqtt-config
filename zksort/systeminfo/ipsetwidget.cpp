#include "ipsetwidget.h"
#include "ui_ipsetwidget.h"
#include <QSettings>
#include "global.h"
#include <QtNetwork>
#include "mqttsrv.h"

IPSetWidget::IPSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::IPSetWidget)
{
    ui->setupUi(this);
    m_bModify = false;
    virtualAddr = "";
    initUi();

    /* 每5s与服务器建立一次连接，判断服务器是否有效 */
    m_thread = new pingThread;
    connect(m_thread, SIGNAL(pingFailSig()), this, SLOT(on_btn_disconnect_clicked()));
    ui->btn_request_cert->hide();
    retranslateUiWidget();
}

IPSetWidget::~IPSetWidget()
{
    delete ui;
}

void IPSetWidget::showPage(bool bshow)
{
    if(bshow){
        updateUi();
        QSettings setting(CFG_APPSet,QSettings::IniFormat);
        setting.beginGroup("eth0");
        QString ipStr0 =  setting.value("ipStr","192.168.0.10").toString();
        QString maskStr0 = setting.value("maskStr","255.255.255.0").toString();
        QString gatewayStr0 = setting.value("gatewayStr","192.168.0.1").toString();
        setting.endGroup();
        ui->IP_LineEdit->setText(ipStr0);
        ui->Mask_LineEdit->setText(maskStr0);
        ui->Gateway_LineEdit->setText(gatewayStr0);

        setting.beginGroup("eth1");
        ipStr1 =  setting.value("ipStr").toString();
        QString maskStr1 = setting.value("maskStr","255.255.255.0").toString();
        QString gatewayStr1 = setting.value("gatewayStr","192.168.11.1").toString();
        setting.endGroup();
        if(ipStr1.isEmpty()){
            ui->label_6->hide();
            ui->IP_LineEdit_2->hide();
            ui->Mask_LineEdit_2->hide();
            ui->Gateway_LineEdit_2->hide();
            ui->Dns_LineEdit_2->hide();
        }else{
            ui->IP_LineEdit_2->setText(ipStr1);
            ui->Mask_LineEdit_2->setText(maskStr1);
            ui->Gateway_LineEdit_2->setText(gatewayStr1);
        }

        ui->btn_connect->setVisible(struGsh.bRunMode != RUNMODE_ENGINEER);
        ui->btn_disconnect->setVisible(struGsh.bRunMode != RUNMODE_ENGINEER);
        ui->btn_import_cert->setVisible(struGsh.bRunMode != RUNMODE_ENGINEER);
        ui->btn_nettest->setVisible(struGsh.bRunMode != RUNMODE_ENGINEER);
        ui->btn_request_cert->setVisible(struGsh.bRunMode != RUNMODE_ENGINEER);
        ui->okBtn->setVisible(struGsh.bRunMode != RUNMODE_ENGINEER);
        ui->checkBox->setVisible(struGsh.bRunMode != RUNMODE_ENGINEER);
        ui->lbl_connectedIP->setVisible(struGsh.bRunMode != RUNMODE_ENGINEER);
        ui->changeBtn->setVisible(struGsh.bRunMode != RUNMODE_ENGINEER);
    }
}

void IPSetWidget::retranslateUiWidget()
{
    ui->label->setText(g_myLan().ip_address);
    ui->label_2->setText(g_myLan().subnet_mask);
    ui->label_3->setText(g_myLan().default_gateway);
    ui->label_4->setText(g_myLan().dns_server);
    ui->label_5->setText(g_myLan().network_1);
    ui->label_6->setText(g_myLan().network_2);
    ui->checkBox->setText(g_myLan().network_config);
    ui->okBtn->setText(g_myLan().ids_apply);
    ui->btn_import_cert->setText(g_myLan().msg_cert_import);
    ui->btn_request_cert->setText(g_myLan().msg_cert_request);
    ui->btn_nettest->setText(g_myLan().network_test);
    ui->btn_connect->setText(g_myLan().ids_remote);
    ui->btn_disconnect->setText(g_myLan().disconnect);
    ui->changeBtn->setText(g_myLan().ids_Sort);

}

void IPSetWidget::on_okBtn_clicked()
{
    if(m_bModify){
        g_infoWidget().setLabelText(g_myLan().network_config);
        g_infoWidget().delayShow();
        QSettings setting(CFG_APPSet,QSettings::IniFormat);
        setting.beginGroup("eth0");
        setting.setValue("ipStr",ui->IP_LineEdit->text());
        setting.setValue("maskStr",ui->Mask_LineEdit->text());
        setting.setValue("gatewayStr",ui->Gateway_LineEdit->text());
        setting.endGroup();

        if(!ipStr1.isEmpty()){
            setting.beginGroup("eth1");
            setting.setValue("ipStr",ui->IP_LineEdit_2->text());
            setting.setValue("maskStr",ui->Mask_LineEdit_2->text());
            setting.setValue("gatewayStr",ui->Gateway_LineEdit_2->text());
            setting.endGroup();
        }
        setting.sync();

        myFlow.getSetting();
        g_infoWidget().delayHide();
        m_bModify = false;
        ui->checkBox->setChecked(m_bModify);
        updateUi();
    }
}

void IPSetWidget::initUi()
{
    ui->checkBox->setChecked(m_bModify);
    ui->IP_LineEdit->setType(textType);
    ui->Mask_LineEdit->setType(textType);
    ui->Gateway_LineEdit->setType(textType);
    ui->Dns_LineEdit->setType(textType);

    ui->IP_LineEdit_2->setType(textType);
    ui->Mask_LineEdit_2->setType(textType);
    ui->Gateway_LineEdit_2->setType(textType);
    ui->Dns_LineEdit_2->setType(textType);

    ui->lbl_netstate->setAlignment(Qt::AlignHCenter);
    ui->lbl_connectedIP->setAlignment(Qt::AlignHCenter);

    ui->btn_disconnect->setEnabled(false);

}

void IPSetWidget::updateUi()
{
    ui->IP_LineEdit->setEnabled(m_bModify);
    ui->Mask_LineEdit->setEnabled(m_bModify);
    ui->Gateway_LineEdit->setEnabled(m_bModify);
    ui->Dns_LineEdit->setEnabled(false);

    ui->IP_LineEdit_2->setEnabled(m_bModify);
    ui->Mask_LineEdit_2->setEnabled(m_bModify);
    ui->Gateway_LineEdit_2->setEnabled(m_bModify);
    ui->Dns_LineEdit_2->setEnabled(false);

    ui->lbl_netstate->hide();

}

void IPSetWidget::on_checkBox_clicked(bool checked)
{
    m_bModify = checked;
    updateUi();
}

void IPSetWidget::on_btn_nettest_clicked()
{
    g_infoWidget().setLabelText(g_myLan().msg_network_testing);
    g_infoWidget().delayShow();

    QProcess m_cmd;
    m_cmd.start("ping -w 5 ntp.ntsc.ac.cn");

    if(m_cmd.waitForFinished())
    {
        QString str = m_cmd.readAll();

#ifdef Q_OS_UNIX
        if(!str.contains("ttl"))
#else
        if(!str.contains("TTL"))
#endif
        {
            m_cmd.kill();
            g_infoWidget().setLabelText(g_myLan().msg_network_fail);
            myFlow.sleep(2);
            g_infoWidget().hide();
            return;
        }
    }
    g_infoWidget().setLabelText(g_myLan().msg_network_success);
    myFlow.sleep(2);
    g_infoWidget().hide();
}


void IPSetWidget::on_btn_connect_clicked()
{
    if(g_Runtime().GetIpAddress("tun0") == "")
    {
        g_infoWidget().setLabelText(g_myLan().msg_connecting);
        g_infoWidget().delayShow();

       #ifdef Q_OS_UNIX
           char cmd[64];
           sprintf(cmd, "cd /app/;./openvpn --config /sdcard/cfg/zk.ovpn &");
           system(cmd);
       #endif

           if(connetToServer())    // 连接服务器成功
           {
               ui->btn_connect->setEnabled(false);
               ui->btn_disconnect->setEnabled(true);
               ui->btn_import_cert->setEnabled(false);
               ui->btn_request_cert->setEnabled(false);

               g_infoWidget().hide();
               m_thread->start();
           }
           else                    // 连接服务器失败
           {
#ifdef Q_OS_UNIX
               system("killall openvpn");
#endif
               g_infoWidget().setLabelText(g_myLan().msg_connect_fail);
               g_infoWidget().showSecs(2);
           }
    }
    else{
        getLocalVirtualAddr();
        ui->btn_disconnect->setEnabled(true);
    }
}

/* 判断屏连接服务器是否成功 */
bool IPSetWidget::connetToServer()
{
    int maxTimes = 30;

    for(int i=0; i<maxTimes; i++)
    {
        if(getLocalVirtualAddr())
            return true;

        myFlow.sleep(1);
    }

    return false;
}
/* 获取本地虚拟网络地址 */
bool IPSetWidget::getLocalVirtualAddr()
{
    QList<QNetworkInterface> interfaceList = QNetworkInterface::allInterfaces();
    foreach (QNetworkInterface interface, interfaceList)
    {
        if(interface.name() == QString("tun0"))
        {
            QList<QNetworkAddressEntry> hostAddressList = interface.addressEntries();
            foreach (QNetworkAddressEntry address, hostAddressList)
            {
                if(address.ip().protocol() == QAbstractSocket::IPv4Protocol)
                {
                    virtualAddr = address.ip().toString();
                    ui->lbl_connectedIP->setText(virtualAddr);
                }
            }

            return true;
        }
    }

    return false;
}


/* 响应断开连接按钮按下事件 */
void IPSetWidget::on_btn_disconnect_clicked()
{
#ifdef Q_OS_UNIX
    char cmd[64];
    sprintf(cmd, "killall openvpn");
    system(cmd);
#endif

    ui->btn_connect->setEnabled(true);
    ui->btn_disconnect->setEnabled(false);
    ui->btn_import_cert->setEnabled(true);
    ui->btn_request_cert->setEnabled(true);
    ui->lbl_connectedIP->setText("0.0.0.0");
    m_thread->stopPing();
}


/* pingThread的构造函数 */
pingThread::pingThread()
{
    m_cmd = 0;
    m_bIsRunning = true;
}

/* 判断服务器是否有效 */
bool pingThread::serverIsAlive()
{
    QStringList list = virtualAddr.split(".");
    if (list.length() != 4)
        return false;

    QString vpnIp = "10.18.0.1";
//    for(int i = 0;i<list.length();i++)
//    {
//        if (i == list.length()-1)
//        {
//            vpnIp.append("1");
//        }
//        else
//        {
//            vpnIp.append(list.at(i));
//            vpnIp.append(".");
//        }
//    }

    QString cmd = "ping -w 5 "+ vpnIp;

    qDebug()<<cmd;

    m_cmd = new QProcess;
    m_cmd->start(cmd.toLocal8Bit().constData());

    if(m_cmd->waitForFinished())
    {
        QString str = m_cmd->readAll();

#ifdef Q_OS_UNIX
        if(!str.contains("ttl"))
#else
        if(!str.contains("TTL"))
#endif
        {
            m_cmd->kill();
            return false;
        }
    }

    return true;
}

/* pingThread的运行函数 */
void pingThread::run()
{
    m_bIsRunning = true;
    while(m_bIsRunning)
    {
        if(!serverIsAlive())
        {
            qDebug("ping Fail!");
            emit pingFailSig();
            return;
        }

        qDebug("ping OK!");
        myFlow.sleep(20);
    }
}

/* 停止ping操作 */
void pingThread::stopPing()
{
    m_bIsRunning = false;
    m_cmd->kill();
}


void IPSetWidget::on_btn_request_cert_clicked()
{
#ifdef Q_OS_UNIX
    QSettings setting(CFG_APPSet,QSettings::IniFormat);
    QString remote_access = setting.value("remote_access").toString();
    if(remote_access == ""||remote_access == "0")
    {
        QFile *myfile = new QFile("/sdcard/cfg/zk.ovpn");
        myfile->open(QIODevice::ReadOnly|QIODevice::Text);
        QString str = myfile->readAll();
        myLongMessageBox msgBox(MSG_QUES, str);

        int ret = msgBox.exec();
        if (ret == QDialog::Accepted)
        {
            setting.setValue("remote_access",1);
            setting.sync();
//            myMqttMsgParaseThread->uploadRemoteAccess(1);
        }
        else
        {
            setting.setValue("remote_access",0);
            setting.sync();
//            myMqttMsgParaseThread->uploadRemoteAccess(0);
        }
    }
    else if(remote_access == "1")
    {
        g_infoWidget().setLabelText(g_myLan().msg_cert_request+g_myLan().msg_connecting);
        g_infoWidget().delayShow();
        int ret;       
        MyHttpFileClient client; //避免影响mqtt
        if (struCnfg.nLVMachine)
            ret = client.requestVPNCertificate(URL_LV_VPN_CERT_REQUEST);
        else
            ret = client.requestVPNCertificate(URL_VPN_CERT_REQUEST);
        if(ret == 1)
        {
            g_infoWidget().setLabelText(g_myLan().msg_cert_request_success);
            g_infoWidget().showSecs();
        }
        else
        {
            g_infoWidget().setLabelText(g_myLan().msg_cert_not_exist);
            g_infoWidget().showSecs();
        }

    }

#endif
}

void IPSetWidget::on_btn_import_cert_clicked()
{
    if(!g_Runtime().checkUsbExist()){
        g_infoWidget().setLabelText(g_myLan().msg_usb_insert);
        g_infoWidget().showSecs();
        return;
    }
    QFileInfo file(g_Runtime().getUsbPath()+"zk.ovpn");
    if(!file.exists()){
        g_infoWidget().setLabelText(g_myLan().msg_cert_not_exist);
        g_infoWidget().showSecs();
        return;
    }
#ifdef Q_OS_UNIX
    g_Runtime().copyFileToPath(g_Runtime().getUsbPath()+"zk.ovpn",OPENVPN_CERT_PATH,true);
#endif
    g_infoWidget().setLabelText(g_myLan().msg_cert_import_success);
    g_infoWidget().showSecs();
}

/* 切换网口1和网口2 IP地址*/
void IPSetWidget::on_changeBtn_clicked()
{
    QString str = g_myLan().ids_Sort+g_myLan().ip_address+"?";
    myMessageBox msgBox(MSG_QUES, str);

    int ret = msgBox.exec();
    if (ret != QDialog::Accepted)
    {
        return;
    }

    QString ipStr0 = ui->IP_LineEdit->text();
    QString maskStr0 = ui->Mask_LineEdit->text();
    QString gatewayStr0 = ui->Gateway_LineEdit->text();

    QString ipStr1 = ui->IP_LineEdit_2->text();
    QString maskStr1 = ui->Mask_LineEdit_2->text();
    QString gatewayStr1 = ui->Gateway_LineEdit_2->text();

    ui->IP_LineEdit->setText(ipStr1);
    ui->Mask_LineEdit->setText(maskStr1);
    ui->Gateway_LineEdit->setText(gatewayStr1);
    ui->IP_LineEdit_2->setText(ipStr0);
    ui->Mask_LineEdit_2->setText(maskStr0);
    ui->Gateway_LineEdit_2->setText(gatewayStr0);

    QSettings setting(CFG_APPSet,QSettings::IniFormat);
    setting.beginGroup("eth0");
    setting.setValue("ipStr",ipStr1);
    setting.setValue("maskStr",maskStr1);
    setting.setValue("gatewayStr",gatewayStr1);
    setting.endGroup();
    setting.beginGroup("eth1");
    setting.setValue("ipStr",ipStr0);
    setting.setValue("maskStr",maskStr0);
    setting.setValue("gatewayStr",gatewayStr0);
    setting.endGroup();
    setting.sync();

    myFlow.getSetting();
    updateUi();
}
