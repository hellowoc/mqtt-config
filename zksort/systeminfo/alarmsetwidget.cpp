#include "alarmsetwidget.h"
#include "ui_alarmsetwidget.h"

AlarmSetWidget::AlarmSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::AlarmSetWidget)
{
    ui->setupUi(this);

    cBox[0] = ui->alarmFeedEnable;
    cBox[1] = ui->alarmWipeEnable;
    cBox[2] = ui->alarmPressureEnable;
    cBox[3] = ui->alarmFeederLevelEnable;
    cBox[4] = ui->alarmClassEnable;
    cBox[5] = ui->feedOffEnable;
    cBox[6] = ui->alarmIpcEnable;

    for (int i = 0;i < CHECKBOXCOUNT;i++ )
    {
        connect(cBox[i],SIGNAL(clicked(bool)),this,SLOT(updateCheckBoxStatus(bool)));
    }
}

AlarmSetWidget::~AlarmSetWidget()
{
    delete ui;
}

void AlarmSetWidget::showPage(bool bshow)
{
    if(bshow){
        updateUi();
        ui->feedOffEnable->hide();
        ui->alarmFeedEnable->hide();
        ui->alarmWipeEnable->hide();
    }
}

void AlarmSetWidget::retranslateUiWidget()
{
    ui->alarmFeedEnable->setText(g_myLan().ids_feedAlarm);
    ui->alarmWipeEnable->setText(g_myLan().ids_cleaningAlarm);
    ui->alarmPressureEnable->setText(g_myLan().ids_airpressureAlarm);
    ui->alarmFeederLevelEnable->setText(g_myLan().ids_materialLevelAlarm);
    ui->alarmClassEnable->setText(g_myLan().classType+g_myLan().alarm);
    ui->feedOffEnable->setText(g_myLan().alarm_turn_off);
    ui->clearRunTime->setText(g_myLan().clearTotalRunTime);

    ui->alarmIpcEnable->setText("IPC"+g_myLan().alarm);
}

void AlarmSetWidget::updateUi()
{
    for (int i = 0;i < CHECKBOXCOUNT;i++ )
    {
        cBox[i]->setChecked(false);
    }
    cBox[0]->setChecked(struCnfg.nAlarmValid[ALARM_FEED]);
    cBox[1]->setChecked(struCnfg.nAlarmValid[ALARM_WIPE]);
    cBox[2]->setChecked(struCnfg.nAlarmValid[ALARM_PRESSURE]);
    cBox[3]->setChecked(struCnfg.nAlarmValid[ALARM_FEED_LEVEL]);
    cBox[6]->setChecked(struCnfg.nAlarmValid[ALARM_IPC]);
    cBox[4]->setChecked(struCnfg.nAlarmValid[ALARM_CLASS]);
    cBox[5]->setChecked(struCnfg.nControlAlarmSet);

}

void AlarmSetWidget::updateCheckBoxStatus(bool checked)
{
    QCheckBox *ckbox = (QCheckBox *)this->sender();
    if(ckbox == cBox[0])
    {
        struCnfg.nAlarmValid[ALARM_FEED] = checked;
    }
    else if (ckbox == cBox[1])
    {
        struCnfg.nAlarmValid[ALARM_WIPE] = checked;
    }
    else if (ckbox == cBox[2])
    {
        struCnfg.nAlarmValid[ALARM_PRESSURE] = checked;
    }
    else if (ckbox == cBox[3])
    {
        struCnfg.nAlarmValid[ALARM_FEED_LEVEL] = checked;
    }
    else if (ckbox == cBox[6])
    {
        struCnfg.nAlarmValid[ALARM_IPC] = checked;
    }
    else if (ckbox == cBox[4])
    {
        struCnfg.nAlarmValid[ALARM_CLASS] = checked;
    }
    else if (ckbox == cBox[5])
    {
        struCnfg.nControlAlarmSet = checked;
        myFlow.resetControlAlarmRG();
    }
}

void AlarmSetWidget::on_clearRunTime_clicked()
{
    struCnfg.nCounter = 0;
    struCnfg.ntotalFeedTime =0;
    struCnfg.ntotalAlarmCount = 0;

    QSettings setting(CFG_APPSet,QSettings::IniFormat);
    setting.setValue("onoffcnts",0);
    setting.sync();
}
