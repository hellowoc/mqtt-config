#include "wipesetwidget.h"
#include "globalflow.h"
#include "ui_wipesetwidget.h"

wipeSetWidget::wipeSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::wipeSetWidget)
{
    ui->setupUi(this);
}

wipeSetWidget::~wipeSetWidget()
{
    delete ui;
}

void wipeSetWidget::on_manuWipePushBtn_clicked()
{
    struGsh.bFlagManuwipe = 1;
}

void wipeSetWidget::showPage(bool bshow)
{
    if(bshow){
        m_wipeDuration = struCnfp.struGroupCtrl[struGsh.nLayer].nWipeDuration;
        m_wipeInterval = struCnfp.struGroupCtrl[struGsh.nLayer].nWipeInterval;
        ui->wipeDurationEdit->setText(QString("%1").arg(m_wipeDuration));
        ui->wipeIntervalEdit->setText(QString("%1").arg(m_wipeInterval));

        ui->wipeDurationEdit->setMinMax(this,4,60,0,&m_wipeDuration);
        ui->wipeIntervalEdit->setMinMax(this,1,500,1,&m_wipeInterval);

        if(struCnfp.struGroupCtrl[struGsh.nLayer].nWipeTimes == 1) {
            ui->wipeSingleCheckBox->setChecked(true);
        }
        else
        {
            ui->wipeSingleCheckBox->setChecked(false);
        }

        if(struCnfp.struGroupCtrl[struGsh.nLayer].nWipeUnion == 0)
        {
            ui->timerWipeRadioBtn->setChecked(true);
            ui->combineWipeRadioBtn->setChecked(false);
            ui->aiWipeBtn->hide();
            ui->wipeSingleCheckBox->show();
        }
        else
        {
            ui->timerWipeRadioBtn->setChecked(false);
            ui->combineWipeRadioBtn->setChecked(true);
            ui->aiWipeBtn->show();
            ui->wipeSingleCheckBox->hide();

        }
        ui->combineWipeRadioBtn->setVisible(struGsh.bRunMode != RUNMODE_USER);
        ui->aiWipeBtn->setVisible(struGsh.bRunMode != RUNMODE_USER & ui->combineWipeRadioBtn->isChecked());
        ui->checkBox->setChecked(struCnfp.struGroupCtrl[struGsh.nLayer].nWipeSortOnOff);
        ui->checkBox_2->setChecked(struCnfp.struGroupCtrl[struGsh.nLayer].nWipeFeedOnOff);
    }
}

void wipeSetWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){
        switch(msg2)
        {
        case 0:
            struCnfp.struGroupCtrl[struGsh.nLayer].nWipeDuration = m_wipeDuration;
            break;
        case 1:
            struCnfp.struGroupCtrl[struGsh.nLayer].nWipeInterval = m_wipeInterval;
            break;
        }
        myFlow.resetWiperRG(0x55);
    }
}

void wipeSetWidget::retranslateUiWidget()
{
    ui->wipeDuration->setText(g_myLan().wipe_duration);
    ui->wipeInterval->setText(g_myLan().ids_cleaningInterval);
    ui->timerWipeRadioBtn->setText(g_myLan().ids_timingMode);
    ui->wipeSingleCheckBox->setText(g_myLan().wipe_single);
    ui->manuWipePushBtn->setText(g_myLan().wipe_manual);
    ui->combineWipeRadioBtn->setText(g_myLan().ids_timingMode_smart);
    ui->aiWipeBtn ->setText(g_myLan().wipe_ai_set);
    ui->checkBox->setText(g_myLan().wipe_continue_eject);
    ui->checkBox_2->setText(g_myLan().wipe_continue_feed);
}

void wipeSetWidget::on_wipeSingleCheckBox_clicked()
{
    struCnfp.struGroupCtrl[struGsh.nLayer].nWipeTimes = (struCnfp.struGroupCtrl[struGsh.nLayer].nWipeTimes == 1) ? 0 : 1;
    myFlow.resetSystemControlRG();
}

void wipeSetWidget::on_timerWipeRadioBtn_clicked()
{
    struCnfp.struGroupCtrl[struGsh.nLayer].nWipeUnion = 0;
    myFlow.resetSystemControlRG();
    ui->aiWipeBtn->hide();
    ui->wipeSingleCheckBox->show();
}

void wipeSetWidget::on_combineWipeRadioBtn_clicked()
{
    struCnfp.struGroupCtrl[struGsh.nLayer].nWipeTimes = 0;
    ui->wipeSingleCheckBox->setChecked(false);

    myFlow.resetSystemControlRG();

    struCnfp.struGroupCtrl[struGsh.nLayer].nWipeUnion = 2;
    myFlow.resetSystemControlRG();

    ui->wipeSingleCheckBox->hide();
    ui->aiWipeBtn->show();
}

void wipeSetWidget::on_aiWipeBtn_clicked()
{
    g_MainManager().ShowWidget(SM_AI_WIPE_SET_WIDGET);
}

void wipeSetWidget::on_checkBox_clicked(bool checked)
{
    struCnfp.struGroupCtrl[struGsh.nLayer].nWipeSortOnOff = checked;
}

void wipeSetWidget::on_checkBox_2_clicked(bool checked)
{
    struCnfp.struGroupCtrl[struGsh.nLayer].nWipeFeedOnOff = checked;
}
