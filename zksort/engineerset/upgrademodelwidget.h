#ifndef UPGRADEMODELWIDGET_H
#define UPGRADEMODELWIDGET_H

#include "basewidget.h"
#include "logger.h"

namespace Ui {
class upgradeModelWidget;
}

#define PACKET_LEN 1023

class upgradeModelWidget : public basewidget
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    explicit upgradeModelWidget(QWidget *parent = 0);
    ~upgradeModelWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
//    unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT,unsigned short CRC_Temp);
protected:
    void retranslateUiWidget();
    void timerEvent(QTimerEvent *event);
private slots:
    void on_startUpgradeBtn_clicked();
    void on_checkFileBtn_clicked();
//    void on_comboBox_chute_activated(int index);
    void on_comboBox_chain_activated(int index);
    void on_comboBox_function_activated(int index);

private:
    Ui::upgradeModelWidget *ui;
    void updateAll();
    void updateChainCombobox();
//    void updateChuteCombobox();
    void appendUpdateResult(QString str,bool ok = true);
    bool checkUpdateFile();     //检查升级文件是否匹配电路板

    int m_nUpgradeTypeIndex;

    QStringList filelist;

    QString modeFileDataName;   // 准备升级的模型数据文件名,不包含.dma
    QString modeFileDataWholeName;   // 准备升级的模型数据文件地址
    QString modeFileConfigWholeName;   // 准备升级的模型配置文件地址
    int m_count;
    int m_times;

    int m_nChainIndex;
//    int m_nChuteIndex;

    int m_functionflag;

    QStringList m_upgradeResList;
    QStringList viewList;
};

#endif // UPGRADEMODELWIDGET_H
