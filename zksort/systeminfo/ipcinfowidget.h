#ifndef IPCINFOWIDGET_H
#define IPCINFOWIDGET_H

#include "basewidget.h"

namespace Ui {
class IPCInfoWidget;
}

class IPCVersionInfo{
public:
    QString name;
    QString ver;
    QString roiInfo;
    QString modelStat;
    QString lineInfo;
    QString freeSpace;
    QString cpuInfo;
    QString tempInfo;
    QString lineFreq;
    QString picCnt;
    int     offlineStat;

    IPCVersionInfo(QString strName,QString strVer,QString strRoiInfo,
                   QString strModelStat,QString strLineInfo,QString strFreeSpace,
                   QString strCpuInfo,QString strTempInfo,QString strLineFreq,QString strPicCnt,int offStat){
        name        = strName;
        ver         = strVer;
        roiInfo     = strRoiInfo;
        modelStat   = strModelStat;
        lineInfo    = strLineInfo;
        freeSpace   = strFreeSpace;
        cpuInfo     = strCpuInfo;
        tempInfo    = strTempInfo;
        lineFreq    = strLineFreq;
        picCnt      = strPicCnt;
        offlineStat = offStat;
    }
};

class IPCInfoWidget : public basewidget
{
    Q_OBJECT

public:
    explicit IPCInfoWidget(QWidget *parent = 0);
    ~IPCInfoWidget();
    virtual void showPage(bool bshow);
    void retranslateUiWidget();
private slots:
    void on_search_clicked();
    void updateALl();

private:
    Ui::IPCInfoWidget *ui;

    QList<IPCVersionInfo> m_verinfoVec;
    void updateVersionInfo();
    void updateTableWidget();
};

#endif // IPCINFOWIDGET_H
