#include "ipcalarmcontrol.h"
#include "ui_ipcalarmcontrol.h"
#include "globalflow.h"

ipcAlarmControlWidget::ipcAlarmControlWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::ipcAlarmControlWidget)
{
    ui->setupUi(this);

    ui->ipcbadstaticscountlabel->setFixedSize(400, 30);
    ui->alarmfeedoffCheckBox->setFixedWidth(600);
    ui->alarmfeedoffCheckBox_2->setFixedWidth(600);
}

ipcAlarmControlWidget::~ipcAlarmControlWidget()
{
    delete ui;
}

void ipcAlarmControlWidget::showPage(bool bshow)
{
    basewidget::showPage(bshow);
    if(bshow){
        updateView();
    }
}
void ipcAlarmControlWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){


    }
}

void ipcAlarmControlWidget::retranslateUiWidget()
{
//    ui->label_blue->setText(g_myLan().ids_blue);
    ui->ipcbadstaticscountlabel->setText(g_myLan().bad_spot_statistical_time+"(Min)");
    ui->alarmfeedoffCheckBox->setText(g_myLan().alarm_and_resatrt_ipc);
    ui->alarmfeedoffCheckBox_2->setText(g_myLan().alarm_and_feed_off);
}

void ipcAlarmControlWidget::updateView()
{
    ui->ipcbadstaticscountlineEdit->setText(QString::number(struCnfg.nIpcBadMatStaticsCount));

    ui->ipcbadstaticscountlineEdit->setMinMax(this,1,100,1,&struCnfg.nIpcBadMatStaticsCount);


    if(struCnfg.nIpcAlarmDealMode == 0)
    {
        ui->alarmfeedoffCheckBox->setChecked(false);
    }
    else
    {
        ui->alarmfeedoffCheckBox->setChecked(true);
    }

    if(struCnfg.nIpcAlarmTurnOff == 0)
    {
        ui->alarmfeedoffCheckBox_2->setChecked(false);
    }
    else
    {
        ui->alarmfeedoffCheckBox_2->setChecked(true);
    }
}

void ipcAlarmControlWidget::on_alarmfeedoffCheckBox_clicked()
{
    struCnfg.nIpcAlarmDealMode = (struCnfg.nIpcAlarmDealMode == 0) ? 1 : 0;

    updateView();
}

void ipcAlarmControlWidget::on_alarmfeedoffCheckBox_2_clicked()
{
    struCnfg.nIpcAlarmTurnOff = (struCnfg.nIpcAlarmTurnOff == 0) ? 1 : 0;

    updateView();
}


void ipcAlarmControlWidget::on_pushButton_2_clicked()
{

}

void ipcAlarmControlWidget::on_watcher_filechanged(QString file)
{
    qDebug()<<"file changed"<<file;
}

void ipcAlarmControlWidget::on_watcher_dirchanged(QString dir)
{

    qDebug()<<"dir changed"<<dir;
}

void ipcAlarmControlWidget::on_pushButton_clicked()
{
}


