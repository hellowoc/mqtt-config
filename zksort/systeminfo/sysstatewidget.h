#ifndef SYSSTATEWIDGET_H
#define SYSSTATEWIDGET_H

#include "basewidget.h"


namespace Ui {
class SysStateWidget;
}

class SysStateWidget : public basewidget
{
    Q_OBJECT
enum
{
    btn_system_info,
    btn_alarm_set,
    btn_sw_version,
    btn_device_info,
    btn_remote_help,
    btn_iPCInfo,
    btn_iPCClassInfo,
    btn_viBoardInfo,
    btn_wifi_set,
    btn_camcali_info,
    Sys_State_Index
};
public:
    explicit SysStateWidget(QWidget *parent = 0);
    ~SysStateWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1,int msg2 = 0,int msg3 = 0);
protected:
    virtual void retranslateUiWidget();

private slots:
    void btn_clicked(bool);

//    void on_btn_Model_Class_clicked();

private:
    Ui::SysStateWidget *ui;

    QButtonGroup *m_pbtnGroup;
};

#endif // SYSSTATEWIDGET_H
