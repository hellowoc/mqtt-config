#include "wificonfigwidget.h"
#include "ui_wificonfigwidget.h"
#include "global.h"

WifiConfigWidget::WifiConfigWidget(QWidget* parent):
    basewidget(parent),
    ui(new Ui::WifiConfigWidget)
{
    ui->setupUi(this);
    //m_pMessage = 0;

    // timer
    m_FreshInfoTimer = new QTimer(this);
    connect(m_FreshInfoTimer, SIGNAL(timeout()), this, SLOT(updateWiFiConnState()));
    m_updateConnDevTimer = new QTimer(this);
    connect(m_updateConnDevTimer, SIGNAL(timeout()), this, SLOT(updateConnDevice()));

    //left widgey
    ui->connectState->setText(g_myLan().connect);
    ui->pwdEdit->setType(textType);

    //right widget
    ui->ipEdit->setType(textType);
    ui->maskEdit->setType(textType);
    ui->gateEdit->setType(textType);
    ui->dnsEdit->setType(textType);

    //ui->dhcpBtn->setToggle(false);
    ui->ipEdit->setEnabled(true);
    ui->maskEdit->setEnabled(true);
    ui->gateEdit->setEnabled(true);
    ui->dnsEdit->setEnabled(true);
    ui->confirmBtn->setEnabled(true);

    //ap
    ui->apIpEdit->setType(textType);
    ui->apSsidEdit->setType(textType);
    ui->apPskEdit->setType(textType);
    ui->apPskEdit->setText("123456789");

    //wifi
    m_wifiInterface = WifiInterface::instance(this);

    //默认热点模式
    ui->tabWidget->setCurrentIndex(0);
    m_wifiInterface->setWifiMode(WifiInterface::WIFI);
    //ui->curModeLabel->setText(g_myLan().WIFI_mode);
    ui->curModeLabel->hide();
    ui->modeLabel->hide();

    current_mode =wifi_mode;

    ui->switchModePtn->hide();


}

WifiConfigWidget::~WifiConfigWidget()
{
    delete ui;
}

void WifiConfigWidget::showPage(bool bshow){
    if(bshow){
        if(current_mode == WifiInterface::WIFI)
            m_FreshInfoTimer->start(4000);
        else if(current_mode == WifiInterface::AP)
             m_updateConnDevTimer->start(4000);

    }else{
        m_FreshInfoTimer->stop();
        m_updateConnDevTimer->stop();
    }
}

void WifiConfigWidget::retranslateUiWidget(){
    ui->connect->setText(g_myLan().connect);
    //ui->modeLabel->setText(g_myLan().current_mode+":");
    ui->ssid->setText(g_myLan().WIFI_name);
    ui->password->setText(g_myLan().password);
    ui->maskLabel->setText(g_myLan().subnet_mask);
    ui->gateLabel->setText(g_myLan().default_gateway);
    ui->updateBtn->setText(g_myLan().update);
    ui->password_3->setText(g_myLan().connected_machine);
    ui->switchModePtn->setText(g_myLan().change_mode);

    ui->confirmBtn->setText(g_myLan().ids_apply);
   ui->apSsidLabel->setText(g_myLan().WIFI_name);
   ui->apPskLabel->setText(g_myLan().password);
   ui->apRestoreBtn->setText(g_myLan().ids_restore);
   ui->apApplyBtn->setText(g_myLan().ids_apply);

   ui->tabWidget->setTabText(0, g_myLan().WIFI_mode);
   ui->tabWidget->setTabText(1, g_myLan().Hot_mode);
}

void WifiConfigWidget::receiveMsg(int msg1, int msg2, int msg3){

}

void WifiConfigWidget::on_updateBtn_clicked(){
    this->updateWifiList();
}



void WifiConfigWidget::on_pwdEdit_textChanged(const QString& pwd){
    if(pwd == "****" || pwd.isEmpty())
        return;
    m_pwd = pwd;
    ui->pwdEdit->setText("****");
}

void WifiConfigWidget::singleShotFunc(){
    this->updateWiFiConnState();
    //成功连接，自动开启dhcp,并获取ip
    if(ui->connectState->text() ==g_myLan().msg_network_success){
        ui->dhcpBtn->setToggle(true);
        this->on_dhcpBtn_toggled(true);
    }else{

    }
    m_FreshInfoTimer->start(4000);
    ui->connect->setEnabled(true);
    ui->updateBtn->setEnabled(true);
}

void WifiConfigWidget::updateWiFiConnState(){
    if(m_wifiInterface->getWiFiState()){
        ui->connectState->setText(g_myLan().msg_network_success);
        if(ui->connect->text() == g_myLan().connect){
            ui->connect->setText(g_myLan().disconnect);
            ui->updateBtn->setEnabled(false);
        }
    }else{
        ui->connectState->setText(g_myLan().disconnect);
        if(ui->connect->text() == g_myLan().disconnect){
            ui->connect->setText(g_myLan().connect);
            ui->updateBtn->setEnabled(true);
        }
        if(ui->dhcpBtn->isToggled()){
            ui->dhcpBtn->setToggle(false);
            this->on_dhcpBtn_toggled(false);
        }
    }
}

void WifiConfigWidget::updateWifiList(){
    //get wifi list
    QStringList wifiList = m_wifiInterface->getWifiScanResult();
    ui->ssidBox->clear();
    ui->ssidBox->addItems(wifiList);
}

void WifiConfigWidget::updateWifiNetInfo(){
    WIFI::IpCfg ipCfg = m_wifiInterface->getIpCfg();
    ui->ipEdit->setText(ipCfg.m_ipv4);
    ui->maskEdit->setText(ipCfg.m_mask);
    ui->gateEdit->setText(ipCfg.m_gate);
    ui->dnsEdit->setText(ipCfg.m_dns);
}

void WifiConfigWidget::on_dhcpBtn_toggled(bool checked){
    if(checked){
        ui->ipEdit->setEnabled(false);
        ui->maskEdit->setEnabled(false);
        ui->gateEdit->setEnabled(false);
        ui->dnsEdit->setEnabled(false);
        ui->confirmBtn->setEnabled(false);

        //dhcp on
        m_wifiInterface->setDhcpState(true);

    }else{
        ui->ipEdit->setEnabled(true);
        ui->maskEdit->setEnabled(true);
        ui->gateEdit->setEnabled(true);
        ui->dnsEdit->setEnabled(true);
        ui->confirmBtn->setEnabled(true);

        //dhcp off
        m_wifiInterface->setDhcpState(false);
    }

    //updateNetInfo
    this->updateWifiNetInfo();
}

void WifiConfigWidget::on_confirmBtn_clicked(){
    m_wifiInterface->setIpCfg(WIFI::IpCfg(
                                  ui->ipEdit->text(),
                                  ui->maskEdit->text(),
                                  ui->gateEdit->text(),
                                  ui->dnsEdit->text()
                                  ));
    this->updateWifiNetInfo();
}

void WifiConfigWidget::on_switchModePtn_clicked(){

    WifiInterface::MODE curWifiMode = m_wifiInterface->getCurrentWifiMode();

    if(curWifiMode == WifiInterface::WIFI){
        m_wifiInterface->setWifiMode(WifiInterface::AP);
    }else{
        m_wifiInterface->setWifiMode(WifiInterface::WIFI);
    }

    if(curWifiMode == WifiInterface::WIFI){
        this->updateApIpAndSsidAndPsk();
        this->updateConnDevice();
        m_FreshInfoTimer->stop();
        m_updateConnDevTimer->start(4000);
      //  ui->curModeLabel->setText(g_myLan().Hot_mode);
        //ui->stackedWidget->setCurrentIndex(1);
    }else{
        this->updateWiFiConnState();
        m_updateConnDevTimer->stop();
        m_FreshInfoTimer->start(4000);
        //ui->stackedWidget->setCurrentIndex(0);
       // ui->curModeLabel->setText(g_myLan().WIFI_mode);
    }

}

void WifiConfigWidget::on_apApplyBtn_clicked(){
    m_wifiInterface->setApSsid(ui->apSsidEdit->text());
    m_wifiInterface->setApIp(ui->apIpEdit->text());
    if(ui->apPskEdit->text().size() >= 8)
        m_wifiInterface->setApPsk(ui->apPskEdit->text());
    this->updateApIpAndSsidAndPsk();
}

void WifiConfigWidget::on_apRestoreBtn_clicked(){
    m_wifiInterface->setWifiMode(WifiInterface::AP);
    this->updateApIpAndSsidAndPsk();
}

void WifiConfigWidget::updateApIpAndSsidAndPsk(){
    ui->apIpEdit->setText(m_wifiInterface->getApIp());
    ui->apSsidEdit->setText(m_wifiInterface->getApSsid());
    ui->apPskEdit->setText(m_wifiInterface->getApPsk());
}

void WifiConfigWidget::updateConnDevice(){
    QStringList devs = m_wifiInterface->getApConnDevice();
    ui->apListWidget->clear();
    ui->apListWidget->addItems(devs);
}

void WifiConfigWidget::on_tabWidget_currentChanged(int index)
{

    qDebug()<<index;
    current_mode = index;
    //WifiInterface::MODE curWifiMode = m_wifiInterface->getCurrentWifiMode();

    if(current_mode == WifiInterface::WIFI){
        m_wifiInterface->setWifiMode(WifiInterface::WIFI);
    }else{
        m_wifiInterface->setWifiMode(WifiInterface::AP);
    }

    if(current_mode == WifiInterface::WIFI){

        this->updateWiFiConnState();
        m_updateConnDevTimer->stop();
        m_FreshInfoTimer->start(4000);

      //  ui->curModeLabel->setText(g_myLan().Hot_mode);

    }else{


        this->updateApIpAndSsidAndPsk();
        this->updateConnDevice();
        m_FreshInfoTimer->stop();
        m_updateConnDevTimer->start(4000);

       // ui->curModeLabel->setText(g_myLan().WIFI_mode);
    }

}

void WifiConfigWidget::on_connect_clicked()
{
    if(ui->connect->text() == g_myLan().connect){
        ui->connectState->setText(g_myLan().msg_connecting);
        m_FreshInfoTimer->stop();
        m_wifiInterface->setWifiSsidAndPwd(ui->ssidBox->currentText(), m_pwd);
        m_wifiInterface->connectWifi(ui->ssidBox->currentText());
        QTimer::singleShot(8000, this, SLOT(singleShotFunc()));

    }else{
        ui->connectState->setText(g_myLan().disconnect);
        m_FreshInfoTimer->stop();
        m_wifiInterface->disconnectWifi();
        QTimer::singleShot(3000, this, SLOT(singleShotFunc()));
    }
    ui->connect->setEnabled(false);
    ui->updateBtn->setEnabled(false);

}
