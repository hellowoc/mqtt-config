#ifndef VAVLETEST_H
#define VAVLETEST_H

#include "basewidget.h"
#include "global.h"
#include "mylineedit.h"

namespace Ui {
class vavletest;
}

class vavleTestThread;

class vavletest : public basewidget
{
    Q_OBJECT
enum
{
    MSG_ID_EJECT_DURATION_START = 0,
    MSG_ID_CHUTE_NUM = 20,
    MSG_ID_EJECTOR_NUM = 21
};

public:
    explicit vavletest(QWidget *parent = 0);
    ~vavletest();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
    void OnLayerChange();
protected:
    void retranslateUiWidget();
private slots:

    void on_quickTestCheckBox_clicked();
    void on_vavleTestStartBtn_clicked();
    void on_vavleTestStopBtn_clicked();
    void on_singleTestCheckBox_clicked();

    void vavleTestStartSlt();
    void on_signalTestCheckBox_clicked();
    void vavleTestFinishedSlots();

    void on_vavleLayerBtn_clicked();

signals:
    void vavleTestFinishedSignal();

private:
    Ui::vavletest *ui;
    vavleTestThread    *myVavleThread;

    int curVavle;
    int totalEjectors, curEjector;
    int dispChuteNum, dispEjectorNum;
    int m_ejPerChute;
    bool singleEjectorTest; //是否是单喷嘴检测， true, 单喷嘴检测， false整机顺序检测
    bool fastEjectorTest;   //是否是快速喷阀检测， true, 快速， false常规速度
    bool signalEjectorTest; //是否是信号自检

    myLineEdit* m_pEjectDurationEdit[MAX_GROUP_TICK];
    QLabel *m_pEjectDurationLabel[MAX_GROUP_TICK];

    float m_ejectDuration[MAX_GROUP_TICK];

};

#if 1
// 喷阀自检线程
class vavleTestThread : public QThread
{
    Q_OBJECT
public:
   explicit vavleTestThread();
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
#endif

#endif // VAVLETEST_H
