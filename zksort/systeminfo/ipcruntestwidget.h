#ifndef IPCRUNTESTWIDGET_H
#define IPCRUNTESTWIDGET_H

#include "basewidget.h"

#define CamPerPage 4

namespace Ui {
class IPCRunTestWidget;
}

class ipcRunTestThread;

class IPCRunTestInfo{
public:
    QString timeStamp;
    QString cpuTemp;
    QString cpuUsed;
    QString gpuTemp;
    QString gpuUsed;
    QStringList camBadpoints;
    QStringList netTransSpeed;
    IPCRunTestInfo(QString timeStamp1,int cpuTemp1,int cpuUsed1, int gpuTemp1, int gpuUsed1,QStringList camBadpoints1,QStringList netTransSpeed1){
        timeStamp = timeStamp1;
        cpuTemp = QString::number(cpuTemp1);
        cpuUsed = QString::number(cpuUsed1);
        gpuTemp = QString::number(gpuTemp1);
        gpuUsed = QString::number(gpuUsed1);
        camBadpoints = camBadpoints1;
        netTransSpeed = netTransSpeed1;
    }
};

class IPCRunTestWidget : public basewidget
{
    Q_OBJECT

public:
    explicit IPCRunTestWidget(QWidget *parent = 0);
    ~IPCRunTestWidget();
    virtual void showPage(bool bshow);
    void receiveMsg(int msg1, int msg2, int msg3);
protected:
    virtual void retranslateUiWidget();

private slots:

    void on_ipcStatusbtn_clicked();

    void on_ipcComboBox_activated(int index);

    void on_m_lastBtn_clicked();

    void on_m_nextBtn_clicked();

    void startRunTestErrProcess();

    void updateRunTestInfo();

    void startRunTestTimeOut();

signals:
    void startRunTestErrSig();

private:
    QList<IPCRunTestInfo> m_testinfoVec;

    Ui::IPCRunTestWidget *ui;
    QTimer *mytimer;

    void updatePageShow();
    void updateTestNetInfo();
    void updateTableWidget();

    int m_curIpcNum; //当前正在查询的IPC

    int totalColumnCount;
    int columnCount;
    int m_curPage;
    int m_totalPage;
    int stopped;
};

#endif // IPCRUNTESTWIDGET_H
