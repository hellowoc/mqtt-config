#include "cameracolorrestore.h"
#include "ui_cameracolorrestore.h"
#include "globalflow.h"

cameraColorRestoreWidget::cameraColorRestoreWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::cameraColorRestoreWidget)
{
    ui->setupUi(this);

    m_pViewBtn[0] = ui->pushButton;
    m_pViewBtn[1] = ui->pushButton_2;
    m_pViewBtn[2] = ui->pushButton_3;
    m_pViewBtn[3] = ui->pushButton_4;
    m_pViewBtn[4] = ui->pushButton_5;
    m_pViewBtn[5] = ui->pushButton_6;
    m_pViewBtn[6] = ui->pushButton_7;
    m_pViewBtn[7] = ui->pushButton_8;

    m_pLayerBtn[0] = ui->mainBtn;
    m_pLayerBtn[1] = ui->viceBtn;
    setSlotConnect();

    ui->pushButton_apply->hide();
}

cameraColorRestoreWidget::~cameraColorRestoreWidget()
{
    delete ui;
}

void cameraColorRestoreWidget::showPage(bool bshow)
{
    basewidget::showPage(bshow);
    if(bshow){
        updateView();
    }
}
void cameraColorRestoreWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){
        switch(msg2)
        {
        case 51:
            myFlow.setCameraColorRestore(0);
            break;
        case 52:
            myFlow.setCameraColorRestore(1);
            break;
        case 53:
            myFlow.setCameraColorRestore(2);
            break;
        }
    }
}

void cameraColorRestoreWidget::OnViewChange(int view)
{
    updateView();
}

void cameraColorRestoreWidget::OnChuteChange()
{
    updateView();
}

void cameraColorRestoreWidget::OnLayerChange()
{
    updateView();
}

void cameraColorRestoreWidget::retranslateUiWidget()
{
    ui->label_blue->setText(g_myLan().ids_blue);
    ui->label_green->setText(g_myLan().ids_green);
    ui->label_red->setText(g_myLan().ids_red);

    ui->colorRestoreCheckBox->setText(g_myLan().ids_enable);
}

void cameraColorRestoreWidget::updateView()
{
    ui->lineEdit_R11->setText(QString::number(struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nColorRestoreRed[0]));
    ui->lineEdit_R12->setText(QString::number(struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nColorRestoreRed[1]));
    ui->lineEdit_R13->setText(QString::number(struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nColorRestoreRed[2]));

    ui->lineEdit_G11->setText(QString::number(struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nColorRestoreGreen[0]));
    ui->lineEdit_G12->setText(QString::number(struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nColorRestoreGreen[1]));
    ui->lineEdit_G13->setText(QString::number(struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nColorRestoreGreen[2]));

    ui->lineEdit_B11->setText(QString::number(struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nColorRestoreBlue[0]));
    ui->lineEdit_B12->setText(QString::number(struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nColorRestoreBlue[1]));
    ui->lineEdit_B13->setText(QString::number(struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nColorRestoreBlue[2]));

    ui->lineEdit_R11->setMinMax(this,-32767,32767,51,&struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nColorRestoreRed[0]);
    ui->lineEdit_R12->setMinMax(this,-32767,32767,51,&struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nColorRestoreRed[1]);
    ui->lineEdit_R13->setMinMax(this,-32767,32767,51,&struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nColorRestoreRed[2]);

    ui->lineEdit_G11->setMinMax(this,-32767,32767,52,&struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nColorRestoreGreen[0]);
    ui->lineEdit_G12->setMinMax(this,-32767,32767,52,&struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nColorRestoreGreen[1]);
    ui->lineEdit_G13->setMinMax(this,-32767,32767,52,&struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nColorRestoreGreen[2]);

    ui->lineEdit_B11->setMinMax(this,-32767,32767,53,&struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nColorRestoreBlue[0]);
    ui->lineEdit_B12->setMinMax(this,-32767,32767,53,&struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nColorRestoreBlue[1]);
    ui->lineEdit_B13->setMinMax(this,-32767,32767,53,&struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nColorRestoreBlue[2]);


    if(struCnfp.bColorRestoreEnable == 0)
    {
        ui->colorRestoreCheckBox->setChecked(false);
    }
    else
    {
        ui->colorRestoreCheckBox->setChecked(true);
    }
}

void cameraColorRestoreWidget::on_colorRestoreCheckBox_clicked()
{
    struCnfp.bColorRestoreEnable = (struCnfp.bColorRestoreEnable == 0) ? 1 : 0;

    myFlow.colorRestoreEnable();
}
