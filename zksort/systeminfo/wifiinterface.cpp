#include "wifiinterface.h"

#include <QDebug>
#include <QFile>

#define LOG_CURRENT_LOCATION_DEBUG qDebug() << "File:" << __FILE__ << " Function:" << Q_FUNC_INFO << '\n'

WifiInterface::WifiInterface(QObject *parent) : QObject(parent)
{
    m_process = new QProcess(this);

    if(!QFile::exists(WIFI_CONFIG_FILE))
        LOG_CURRENT_LOCATION_DEBUG << QString("wifiCtrol file don't exist in %1").arg(WIFI_CONFIG_FILE);

    m_process->start("chmod 777 " + WIFI_CONFIG_FILE);
    int ret = m_process->waitForFinished(MAX_EXEC_TIME);
    if(ret < 0){
        LOG_CURRENT_LOCATION_DEBUG << "chmod error";
        m_process->deleteLater();
        m_process = new QProcess(this);
    }
    this->execCmd("makeSureEnv");
    this->execCmd("restartEnv");
}

WifiInterface* WifiInterface::instance(QObject *parent){
    static WifiInterface*wifiInterface = new WifiInterface(parent);
    return wifiInterface;
}

QByteArray WifiInterface::execCmd(const QString& cmd){
    if(!m_process)
        return ERR_STR;
    m_process->start(WIFI_CONFIG_FILE + " " + cmd);

    int ret = m_process->waitForFinished(MAX_EXEC_TIME);

    // time out
    if(ret < 0){
        LOG_CURRENT_LOCATION_DEBUG << WIFI_CONFIG_FILE + " " + cmd;
        m_process->deleteLater();
        m_process = new QProcess(this);
        return ERR_STR;
    }

    QString cmdRet = m_process->readLine();

    // error (返回-1或者其他情况)
    if(cmdRet != "1\n" && cmdRet != "0\n"){
        m_process->deleteLater();
        m_process = new QProcess(this);
        return ERR_STR;
    }

    // debug
    if(cmdRet == "0\n"){
        qDebug() << "Debug: " << cmd << "\n" << m_process->readAllStandardError();
    }

    // success
    return m_process->readAllStandardOutput();
}

bool WifiInterface::setWifiSsidAndPwd(const QString& ssid, const QString& pwd){
    //get wifiId
     QString ret = this->execCmd(QString("addNetworkToWpa \"%1\" \"%2\"").arg(ssid).arg(pwd));
    if(ret == ERR_STR){
        return false;
    }
    return true;
}

bool WifiInterface::connectWifi(const QString& ssid){
    //connect wifiId from ssid
    QString ret = this->execCmd(QString("connectWifiFromSsid \"%1\"").arg(ssid));
    if(ret == ERR_STR){
        return false;
    }
    return true;
}

bool WifiInterface::getWiFiState(){
    QString ret = this->execCmd("getWifiConnectState");
    if(ret == ERR_STR){
        return false;
    }
    return ret == "1";
}

QStringList WifiInterface::getWifiScanResult(){
    QString ret = this->execCmd("getWifiScanResult");
    if(ret == ERR_STR){
        return QStringList();
    }
    return ret.split('\n', QString::SkipEmptyParts);
}

bool WifiInterface::setDhcpState(bool state){
    QString ret = this->execCmd(QString("setDhcpState %1").arg(state ? 1 : 0));
    if(ret == ERR_STR){
        return false;
    }
    return true;
}

bool WifiInterface::getDhcpState(){
    QString ret = this->execCmd("getDhcpState");
    if(ret == ERR_STR){
        return false;
    }
    return ret == "1";
}

bool WifiInterface::setIpCfg(const WIFI::IpCfg& ipCfg){
    QString ret = this->execCmd(QString("setNetCfg %1 %2 %3 %4")
                                .arg(ipCfg.m_ipv4)
                                .arg(ipCfg.m_mask)
                                .arg(ipCfg.m_gate)
                                .arg(ipCfg.m_dns));
    if(ret == ERR_STR){
        return false;
    }
    return true;
}

WIFI::IpCfg WifiInterface::getIpCfg(){
    QString ret = this->execCmd("getNetCfg");
    if(ret == ERR_STR){
        return WIFI::IpCfg("NULL","NULL","NULL","NULL");
    }

    QStringList rets = ret.split('\n', QString::SkipEmptyParts);
    if(rets.size() != 4){
        return WIFI::IpCfg("NULL","NULL","NULL","NULL");
    }

    return WIFI::IpCfg(rets[0], rets[1], rets[2], rets[3]);
}

bool WifiInterface::setWifiMode(const WifiInterface::MODE& mode){
    QString ret = this->execCmd(QString("switchWifiMode %1").arg(mode));
    if(ret == ERR_STR){
        return false;
    }
    return true;
}

WifiInterface::MODE WifiInterface::getCurrentWifiMode(){
    QString ret = this->execCmd("getCurrentWifiMode");
    if(ret == ERR_STR){
        return WifiInterface::NONE;
    }
    return WifiInterface::MODE(ret == "0" ? 0 : 1);
}


bool WifiInterface::setApSsid(const QString& ssid){
    QString ret = this->execCmd(QString("setApSsid \"%1\"").arg(ssid));
    if(ret == ERR_STR){
        return false;
    }
    return true;
}

QString WifiInterface::getApSsid(){
    QString ret = this->execCmd("getApSsid");
    if(ret == ERR_STR){
        return ERR_STR;
    }
    return ret.replace('\n',"");
}

bool WifiInterface::setApIp(const QString& ip){
    QString ret = this->execCmd(QString("setApIp %1").arg(ip));
    if(ret == ERR_STR){
        return false;
    }
    return true;
}

QString WifiInterface::getApIp(){
    QString ret = this->execCmd("getApIp");
    if(ret == ERR_STR){
        return ERR_STR;
    }
    return ret.replace('\n',"");
}

QStringList WifiInterface::getApConnDevice(){
    QString ret = this->execCmd("getApConnDevice");
    if(ret == ERR_STR){
        return QStringList();
    }
    return ret.split('\n', QString::SkipEmptyParts);
}

bool WifiInterface::disconnectWifi(){
    QString ret = this->execCmd("disWifiConnect");
    if(ret == ERR_STR){
        return false;
    }
    return true;
}

bool WifiInterface::setApPsk(const QString& psk){
    QString ret = this->execCmd(QString("setApPsk \"%1\"").arg(psk));
    if(ret == ERR_STR){
        return false;
    }
    return true;
}

QString WifiInterface::getApPsk(){
    QString ret = this->execCmd("getApPsk");
    if(ret == ERR_STR){
        return ERR_STR;
    }
    return ret.replace('\n',"");
}
