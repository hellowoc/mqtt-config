#ifndef WIFIINTERFACE_H
#define WIFIINTERFACE_H

#include <QObject>
#include <QProcess>
#include <QDebug>
#include <QHash>

namespace WIFI {
struct IpCfg{
    IpCfg(QString ipv4, QString mask, QString gate, QString dns = "127.0.0.1"):
        m_ipv4(ipv4),
        m_mask(mask),
        m_gate(gate),
        m_dns(dns)
    {}
    QString m_ipv4;
    QString m_mask;
    QString m_gate;
    QString m_dns;
};
}

class WifiInterface : public QObject
{
    Q_OBJECT
public:

    enum MODE{
        WIFI = 0,
        AP = 1,
        NONE = 2
    };

    /**
     * @brief instance 获取单例接口
     * @param parent
     * @return
     */
    static WifiInterface* instance(QObject *parent = 0);

    /**
     * @brief setWifiSsidAndPwd 设置wifi的帐号和密码
     * @param ssid ssid
     * @param pwd 密码
     * @return success 1 fail 0
     */
    bool setWifiSsidAndPwd(const QString& ssid, const QString& pwd);

    /**
     * @brief connectWifi 连接指定ssid的wifi,若有多个同名的wifi，则任选一个连接;
     * @param ssid wifi ssid
     * @return success 1 fail 0。返回值表示设置成功，不表示是否连接成功
     */
    bool connectWifi(const QString& ssid);

    /**
     * @brief getWiFiState 获取wifi的链接状态
     * @return true表示wifi已连接;false未连接
     */
    bool getWiFiState();

    /**
     * @brief getWifiScanResult 获取wifi的扫描结果
     * @return 扫描的wifi热点ssid组成的QStringList
     */
    QStringList getWifiScanResult();

    /**
     * @brief setDhcpState 设置dhcp的使能状态
     * @param state 使能dhcp的状态
     * @return success 1 fail 0
     */
    bool setDhcpState(bool state);

    /**
     * @brief getDhcpState 获取dhcp的使能状态
     * @return open 1 , close 0
     */
    bool getDhcpState();

    /**
     * @brief setIpCfg 设置指定的ip，mask，网关，DNS
     * @param ipCfg 传入的配置结构体数据
     * @return success 1 fail 0
     */
    bool setIpCfg(const WIFI::IpCfg& ipCfg);

    /**
     * @brief getIpCfg 获取当前的ip，mask，网关，DNS
     * @return ip配置结构体数据
     */
    WIFI::IpCfg getIpCfg();

    /**
     * @brief setWifiMode 切换wifi的热点模式和连接模式
     * @param mode 0 连接模式(sta) 1 热点模式(ap)
     * @return success 1 fail 0
     */
    bool setWifiMode(const WifiInterface::MODE& mode);

    /**
     * @brief getCurrentWifiMode 获取当前设备的wifi模式
     * @return 0 sta 1 ap
     */
    WifiInterface::MODE getCurrentWifiMode();

    /**
     * @brief setApSsid 设置Ip模式下的
     * @param ssid
     * @return success 1 fail 0
     */
    bool setApSsid(const QString& ssid);

    /**
     * @brief getApSsid 获取热点模式下的ssid
     * @return ssid
     */
    QString getApSsid();

    /**
     * @brief setApIp 设置热点模式下的ip
     * @param ip
     * @return success 1 fail 0
     */
    bool setApIp(const QString& ip);

    /**
     * @brief getApIp 获取热点模式下的ip
     * @return ip
     */
    QString getApIp();

    /**
     * @brief getApConnDevice 获取当前连接到ap的设备mac地址
     * @return 设备mac地址列表
     */
    QStringList getApConnDevice();

    /**
     * @brief disconnectWifi 断开当前Wifi连接
     * @return success 1 fail 0
     */
    bool disconnectWifi();

    /**
     * @brief setApPsk 设置热点的wifi密码
     * @param psk wifi密码
     * @return success 1 fail 0
     */
    bool setApPsk(const QString& psk);

    /**
     * @brief getApPsk 获取默认的wifi密码
     * @return psk
     */
    QString getApPsk();

private:
    explicit WifiInterface(QObject *parent = 0);
    const char* ERR_STR = "ERR";
    const int MAX_EXEC_TIME = 5000; //最大等待执行时间m5s，否则认为超时出错，重启m_process
    const QString WIFI_CONFIG_FILE = "/sdcard/wifi/wifiCtrol.sh";
    QProcess *m_process;

    /**
     * @brief wifiDisconnect 断开当前的wifi连接
     * @return success 1 fail 0
     */
    bool wifiDisconnect();

    /**
     * @brief execCmd 執行命令并等待结束，返回命令输出
     * @param cmd 命令
     * @return 若返回"ERR",则表示执行错误,返回每一行命令
     */
    QByteArray execCmd(const QString& cmd);

signals:

public slots:
};

#endif // WIFIINTERFACE_H
