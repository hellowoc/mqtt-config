#ifndef EJECTDELAY_H
#define EJECTDELAY_H

#include "basewidget.h"
#include "mylineedit.h"
#include "global.h"

namespace Ui {
class ejectDelay;
}

#if 1
#define DELAY_DP 409.6
#define DELAY    204.4
#else
#define DELAY_DP 102.3
#define DELAY    51.1
#endif
class ejectDelay : public basewidget
{
    Q_OBJECT
    enum{
        Duration,
        Duration2,
        Delay,
        Delay2,
        Delay3,
        Delay4,
        modelDelay,
        Wait,
        Tail,
        valveDefendTime
    };
public:
    explicit ejectDelay(QWidget *parent = 0);
    ~ejectDelay();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
    void OnGroupChange(int groupIndex);
    void OnViewChange(int view);
    void OnLayerChange();

protected:
    void retranslateUiWidget();
private slots:
    void on_expandTickCheckBox_clicked(bool checked);

    void getscreenshot();

private:
    Ui::ejectDelay *ui;

    void updateGroup();
    //    void updateGroupValue();

    int curVavle;
    int curGroup;
    int curView;

    float m_delay, m_delay2,m_delay3,m_delay4,m_modeldelay, m_duration, m_duration2, m_wait,m_tail,m_vavleDefendTime,m_vavleUndefendTime;
    bool m_expandEnable;


    QTimer *shottimer;
};

#endif // EJECTDELAY_H
