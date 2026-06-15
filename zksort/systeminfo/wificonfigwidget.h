#ifndef WIFICONFIGWIDGET_H
#define WIFICONFIGWIDGET_H

#include "basewidget.h"
#include "wifiinterface.h"

#include <QTimer>

enum {
     wifi_mode = 0,  // WIFI模式
    hot_mode = 1     // 热点模式

};
namespace Ui {
class WifiConfigWidget;
}
class WifiConfigWidget : public basewidget
{
    Q_OBJECT
public:
    explicit WifiConfigWidget(QWidget* parent = 0);

    ~WifiConfigWidget();
    void showPage(bool bshow) override;
    void retranslateUiWidget() override;
    void receiveMsg(int msg1, int msg2, int msg3) override;
private slots:



    void on_updateBtn_clicked();

    void on_dhcpBtn_toggled(bool checked);

    void on_confirmBtn_clicked();

    void on_switchModePtn_clicked();

    void on_apApplyBtn_clicked();

    void on_apRestoreBtn_clicked();

    void updateWifiNetInfo();

    void updateWiFiConnState();

    void updateWifiList();

    void singleShotFunc();

    void on_pwdEdit_textChanged(const QString& pwd);

    void updateApIpAndSsidAndPsk();

    void updateConnDevice();

    void on_tabWidget_currentChanged(int index);

    void on_connect_clicked();

private:
    Ui::WifiConfigWidget *ui;
    QTimer *m_FreshInfoTimer;
    QTimer *m_updateConnDevTimer;
    WifiInterface* m_wifiInterface;
    QString m_pwd;

    int current_mode;

};

#endif // WIFICONFIGWIDGET_H
