#include "aiwipesetwidget.h"
#include "ui_aiwipesetwidget.h"

AiWipeSetWidget::AiWipeSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::AiWipeSetWidget)
{
    ui->setupUi(this);
}

AiWipeSetWidget::~AiWipeSetWidget()
{
    delete ui;
}

void AiWipeSetWidget::showPage(bool bshow)
{
    if(bshow){
        ui->lineEdit_closechuteNum->setText(QString::number(struCnfg.struIntelWipe.feederOnffCount));
        ui->lineEdit_durationtime->setText(QString::number(struCnfg.struIntelWipe.wiperDurationPerChute));
        ui->lineEdit_wipeBrushWidth->setText(QString::number(struCnfg.struIntelWipe.wiperWidth));
        ui->lineEdit_wipeBrushPercent->setText(QString::number(struCnfg.struIntelWipe.wiperConfirmThread));
        ui->lineEdit_wipeBrush_position->setText(QString::number(struCnfg.struIntelWipe.wiperDefaultPosi));


        ui->lineEdit_closechuteNum->setMinMax(this,0,12,-1,&struCnfg.struIntelWipe.feederOnffCount);
        ui->lineEdit_durationtime->setMinMax(this,0,100,-1,&struCnfg.struIntelWipe.wiperDurationPerChute);
        ui->lineEdit_wipeBrushWidth->setMinMax(this,0,300,-1,&struCnfg.struIntelWipe.wiperWidth);
        ui->lineEdit_wipeBrushPercent->setMinMax(this,0,100,-1,&struCnfg.struIntelWipe.wiperConfirmThread);
        ui->lineEdit_wipeBrush_position->setMinMax(this,0,1,-1,&struCnfg.struIntelWipe.wiperDefaultPosi);
    }
}

void AiWipeSetWidget::retranslateUiWidget()
{
    ui->offChutelabel->setText(g_myLan().wipe_ai_off_chute);
    ui->speedlabel->setText(g_myLan().wipe_ai_speed);
    ui->widthlabel->setText(g_myLan().wipe_ai_width);
    ui->positionlabel->setText(g_myLan().wipe_ai_position);
    ui->propotionlabel->setText(g_myLan().wipe_ai_propotion);
}
