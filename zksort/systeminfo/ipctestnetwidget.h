#ifndef IPCTESTNETWIDGET_H
#define IPCTESTNETWIDGET_H

#include "basewidget.h"


namespace Ui {
class IPCTestNetWidget;
}

class ipcTestNetThread;

class IPCTestNetInfo{
public:
    QString strName;
    QString total, lost, speed;
    IPCTestNetInfo(QString strName1,int tt1, int err1, int speed1){
        strName = strName1;
        total = QString::number(tt1);
        lost = QString::number(err1);
        speed = QString::number(speed1);
    }
};

class IPCTestNetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit IPCTestNetWidget(QWidget *parent = 0);
    ~IPCTestNetWidget();
    virtual void showPage(bool bshow);
    void receiveMsg(int msg1, int msg2, int msg3);
protected:
    virtual void retranslateUiWidget();

private slots:
    void on_starttestbtn_clicked();

    void on_stoptestbtn_clicked();

    void on_moreInfobtn_clicked();

    void on_ipcStatusbtn_clicked();

    void on_starttestbtn_2_clicked();

private:
    QList<IPCTestNetInfo> m_testinfoVec;

    Ui::IPCTestNetWidget *ui;
    ipcTestNetThread    *myTestNetThread;

    void updateTestNetInfo();
    void updateTableWidget();

    void testIpcNetworkSend(int ipcIndex, int startStop);

    int m_curNetTestIpcNum; //错误帧测试，当前正在测试的IPC
};

// 测试网络线程
class ipcTestNetThread : public QThread
{
    Q_OBJECT
public:
   explicit ipcTestNetThread();
   void stop();

   void setTestSpeed(bool isFast);
signals:
   void ejectorChangeSig();         // 喷阀自检数据变化信号
protected:
    void run();
private slots:
//    void ejectTestDoneSlt();
private:
    volatile bool stopped;
    bool m_bIsFast;
};

#endif // IPCTESTNETWIDGET_H
