#include "irbadpointsetwidget.h"
#include "ui_irbadpointsetwidget.h"
#define pointDataChange 10
IrBadPointSetWidget::IrBadPointSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::IrBadPointSetWidget)
{
    ui->setupUi(this);
    m_pViewBtn[0] = ui->pushButton;
    m_pViewBtn[1] = ui->pushButton_2;
    m_pViewBtn[2] = ui->pushButton_3;
    m_pViewBtn[3] = ui->pushButton_4;
    m_pViewBtn[4] = ui->pushButton_6;
    m_pViewBtn[5] = ui->pushButton_7;
    m_pViewBtn[6] = ui->pushButton_9;
    m_pViewBtn[7] = ui->pushButton_10;
    m_pChuteWidget = ui->chuteWidget;
    m_pLayerBtn[0] = ui->mainBtn;
    m_pLayerBtn[1] = ui->viceBtn;
    setSlotConnect();


    myFlow.myNetWork->setUdpRecvType(CAPTURE_SINGLE_WAVE);

    connect(myFlow.myNetWork->udpthread,SIGNAL(SignalWaveRecvFinished()),this,SLOT(updateWave()));
    connect(myFlow.myNetWork->udpthread,SIGNAL(SignalSocketFailed()),this,SLOT(updateWave()));
    ui->widget->setDrawType(IR_BadPoints);
    ui->checkBox_correct->setVisible(false);
}

IrBadPointSetWidget::~IrBadPointSetWidget()
{
    delete ui;
}

void IrBadPointSetWidget::showPage(bool bshow)
{
    basewidget::showPage(bshow);

    if(bshow){

        updateUpdateCaliState();
        ui->checkBox_enable_updown->setChecked((bool)struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nNirUpdownEnable);
        updateUpDown();
        myFlow.myNetWork->setUdpRecvType(CAPTURE_SINGLE_WAVE);
        myFlow.myNetWork->udpStart();

    }else{
        myFlow.myNetWork->udpStop();
    }

}

void IrBadPointSetWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == WAVE_MSG){
        updateWave();
    }else if(msg1 == MSG_DATA_CHANGED){
        if(msg2 == chute_change){
            basewidget::receiveMsg(msg1,msg2,msg3);
        }
        else if(msg2 == pointDataChange)
        {
            updateValue();
            updateWave();
        }
        else{
            updateWave();
        }
    }
}

void IrBadPointSetWidget::OnViewChange(int view)
{
    updateWave();
    updateValue();
}

void IrBadPointSetWidget::OnChuteChange()
{
    updateValue();
}

void IrBadPointSetWidget::OnLayerChange()
{
    updateValue();
}

void IrBadPointSetWidget::retranslateUiWidget()
{
    ui->checkBox_origin->setText(g_myLan().camera_signal_bias_enable);
    ui->checkBox_cali->setText(g_myLan().camera_signal_cali_enable);

    ui->groupBox->setTitle(g_myLan().nir_bad);
    ui->checkBox_red->setText(g_myLan().nir_bad);
    ui->checkBox_green->setText(g_myLan().nir_bad);
    ui->checkBox_blue->setText(g_myLan().nir_bad);
    ui->checkBox_n->setText(g_myLan().nir_bad);
    ui->pushButton_5->setText(g_myLan().nir_bad_list);
    ui->groupBox_tdi->setTitle(g_myLan().nir_tdi);
    ui->checkBox_tdi->setText(g_myLan().ids_enableTDI);
    ui->checkBox_enable_updown->setText(g_myLan().nir_up_down);
    ui->checkBox_correct->setText(g_myLan().nir_correct);
    ui->pushButton_8->setText(g_myLan().nir_show_all);
}

void IrBadPointSetWidget::updateWave()
{
    ui->widget->update();
}

void IrBadPointSetWidget::updateUpdateCaliState()
{
    ui->checkBox_cali->setChecked(struGsh.bCaliGainEnable);
    ui->checkBox_origin->setChecked(struGsh.bCaliBiasEnable);
}

void IrBadPointSetWidget::updateValue()
{
    int sensorPixel = myFlow.getSensorPixels(struGsh.nLayer,struGsh.nView)-1;

    int dim = struCnfp.nSensorDim[struGsh.nLayer][struGsh.nView];
    bool bshow = true;
    bool bNShow = false;
    if(dim < 3)
    {
       bshow = false;
    }
    if(dim == 4)
    {
        bNShow = true;
    }
    ui->blueDot->setVisible(bshow);
    ui->blueDot_2->setVisible(bshow);
    ui->checkBox_blue->setVisible(bshow);
    ui->checkBox_blue_2->setVisible(bshow);

    ui->nDot->setVisible(bNShow);
    ui->nDot_2->setVisible(bNShow);
    ui->checkBox_n->setVisible(bNShow);
    ui->checkBox_n_2->setVisible(bNShow);

    ui->redDot->setTitle(g_myLan().ids_red);
    ui->greenDot->setTitle(g_myLan().ids_green);
    ui->blueDot->setTitle(g_myLan().ids_blue);
    ui->nDot->setTitle(g_myLan().ids_n);

    ui->redDot->setParams(this,1,sensorPixel+1,pointDataChange,&struGsh.nIrCurPointIndex[0]);
    ui->greenDot->setParams(this,1,sensorPixel+1,pointDataChange,&struGsh.nIrCurPointIndex[0]);
    ui->blueDot->setParams(this,1,sensorPixel+1,pointDataChange,&struGsh.nIrCurPointIndex[0]);
    ui->nDot->setParams(this,1,sensorPixel+1,pointDataChange,&struGsh.nIrCurPointIndex[0]);

    ui->redDot->setValue(struGsh.nIrCurPointIndex[0]);
    ui->greenDot->setValue(struGsh.nIrCurPointIndex[0]);
    ui->blueDot->setValue(struGsh.nIrCurPointIndex[0]);
    ui->nDot->setValue(struGsh.nIrCurPointIndex[0]);

    ui->redDot_2->setTitle(g_myLan().ids_red);
    ui->greenDot_2->setTitle(g_myLan().ids_green);
    ui->blueDot_2->setTitle(g_myLan().ids_blue);
    ui->nDot_2->setTitle(g_myLan().ids_n);

    ui->redDot_2->setParams(this,1,sensorPixel+1,pointDataChange,&struGsh.nIrCurPointIndex[1]);
    ui->greenDot_2->setParams(this,1,sensorPixel+1,pointDataChange,&struGsh.nIrCurPointIndex[1]);
    ui->blueDot_2->setParams(this,1,sensorPixel+1,pointDataChange,&struGsh.nIrCurPointIndex[1]);
    ui->nDot_2->setParams(this,1,sensorPixel+1,pointDataChange,&struGsh.nIrCurPointIndex[1]);

    ui->redDot_2->setValue(struGsh.nIrCurPointIndex[1]);
    ui->greenDot_2->setValue(struGsh.nIrCurPointIndex[1]);
    ui->blueDot_2->setValue(struGsh.nIrCurPointIndex[1]);
    ui->nDot_2->setValue(struGsh.nIrCurPointIndex[1]);


    updateCheckBox();
}

void IrBadPointSetWidget::updateCheckBox()
{
    ui->checkBox_red->setChecked(struGsh.nNirDotCorrectiveRed[struGsh.nView][struGsh.nUnit][0][struGsh.nIrCurPointIndex[0]-1]);
    ui->checkBox_green->setChecked(struGsh.nNirDotCorrectiveGreen[struGsh.nView][struGsh.nUnit][0][struGsh.nIrCurPointIndex[0]-1]);
    ui->checkBox_blue->setChecked(struGsh.nNirDotCorrectiveBlue[struGsh.nView][struGsh.nUnit][0][struGsh.nIrCurPointIndex[0]-1]);
    ui->checkBox_n->setChecked(struGsh.nNirDotCorrectiveN[struGsh.nView][struGsh.nUnit][0][struGsh.nIrCurPointIndex[0]-1]);

    ui->checkBox_red_2->setChecked(struGsh.nNirDotCorrectiveRed[struGsh.nView][struGsh.nUnit][1][struGsh.nIrCurPointIndex[1]-1]);
    ui->checkBox_green_2->setChecked(struGsh.nNirDotCorrectiveGreen[struGsh.nView][struGsh.nUnit][1][struGsh.nIrCurPointIndex[1]-1]);
    ui->checkBox_blue_2->setChecked(struGsh.nNirDotCorrectiveBlue[struGsh.nView][struGsh.nUnit][1][struGsh.nIrCurPointIndex[1]-1]);
    ui->checkBox_n_2->setChecked(struGsh.nNirDotCorrectiveN[struGsh.nView][struGsh.nUnit][1][struGsh.nIrCurPointIndex[1]-1]);

    updateRGB();

    ui->checkBox_tdi->setChecked(struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nNirTdiEnable==1);
    ui->checkBox_enable_updown->setChecked(struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nNirUpdownEnable);
    if(SENSOR_C1_2 ==struCnfp.nViewSensorType[0][struGsh.nView] )
    {
        ui->checkBox_correct->setChecked(0);
    }
    else
    {
        ui->checkBox_correct->setChecked(struGsh.nNirDotCorrectived);
    }
    switch (struGsh.nSignalDispColor)
    {
    case 0:
        ui->pushButton_8->setText(g_myLan().nir_show_all);
        break;
    case 1:
        ui->pushButton_8->setText(g_myLan().ids_red);
        break;
    case 2:
        ui->pushButton_8->setText(g_myLan().ids_green);
        break;
    case 3:
        ui->pushButton_8->setText(g_myLan().ids_blue);
        break;
    case 4:
        ui->pushButton_8->setText(g_myLan().ids_n);
        break;
    default:
        ui->pushButton_8->setText(g_myLan().nir_show_all);
        break;
    }
}

void IrBadPointSetWidget::updateUpDown()
{
    myFlow.resetCameraNirTdiEnable();
    updateWave();
    ui->stackedWidget->setCurrentIndex(struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nNirUpdownEnable);
    updateValue();
    updateRGB();
}

void IrBadPointSetWidget::updateRGB()
{
    int dim = struCnfp.nSensorDim[struGsh.nLayer][struGsh.nView];
    int updown = struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nNirUpdownEnable;
    QString str;
    switch(dim)
    {
    case 1:
        str = QString("R=%1").arg(struGsh.sRowRed[struGsh.nIrCurPointIndex[updown]-1]);
        break;
    case 2:
        str = QString("R=%1,G=%2").arg(struGsh.sRowRed[struGsh.nIrCurPointIndex[updown]-1]).arg(struGsh.sRowGreen[struGsh.nIrCurPointIndex[updown]-1]);
        break;
    case 3:
        str = QString("R=%1,G=%2,\nB=%3").arg(struGsh.sRowRed[struGsh.nIrCurPointIndex[updown]-1]).arg(struGsh.sRowGreen[struGsh.nIrCurPointIndex[updown]-1]).arg(struGsh.sRowBlue[struGsh.nIrCurPointIndex[updown]-1]);
        break;
    case 4:
        str = QString("R=%1,G=%2,B=%3,\nN=%4").arg(struGsh.sRowRed[struGsh.nIrCurPointIndex[updown]-1]).arg(struGsh.sRowGreen[struGsh.nIrCurPointIndex[updown]-1]).arg(struGsh.sRowBlue[struGsh.nIrCurPointIndex[updown]-1]).arg(struGsh.sRowInf[struGsh.nIrCurPointIndex[updown]-1]);
        break;
    }

    ui->label_rgb->setText(str);
}

void IrBadPointSetWidget::on_pushButton_5_clicked()
{
    g_MainManager().ShowWidget(SM_NIR_DOT_CORRECTIVE_LIST_WIDGET);
}


void IrBadPointSetWidget::on_checkBox_green_clicked(bool checked)
{
    struGsh.nNirDotCorrectiveGreen[struGsh.nView][struGsh.nUnit][0][struGsh.nIrCurPointIndex[0]-1] = (int)checked;
    updateCheckBox();
}

void IrBadPointSetWidget::on_checkBox_red_clicked(bool checked)
{
    struGsh.nNirDotCorrectiveRed[struGsh.nView][struGsh.nUnit][0][struGsh.nIrCurPointIndex[0]-1] = (int)checked;
    updateCheckBox();
}

void IrBadPointSetWidget::on_checkBox_blue_clicked(bool checked)
{
    struGsh.nNirDotCorrectiveBlue[struGsh.nView][struGsh.nUnit][0][struGsh.nIrCurPointIndex[0]-1] = (int)checked;
    updateCheckBox();
}

//void IrBadPointSetWidget::on_checkBox_tdi_clicked(bool checked)
//{
//    struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nNirTdiEnable = (struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nNirTdiEnable == 0) ? 1 : 0;
//    myFlow.resetCameraNirTdiEnable();
//    updateCheckBox();
//}

void IrBadPointSetWidget::on_checkBox_enable_updown_clicked(bool checked)
{
    struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nNirUpdownEnable = (int)checked;
    updateUpDown();
}

void IrBadPointSetWidget::on_pushButton_8_clicked()
{
    struGsh.nSignalDispColor++;
    int dim = struCnfp.nSensorDim[struGsh.nLayer][struGsh.nView];
    if(dim < 3)
    {
        if(struGsh.nSignalDispColor == 3)
        {
            struGsh.nSignalDispColor = 0;
        }
    }
    else
    {
        if(struGsh.nSignalDispColor == 5)
        {
            struGsh.nSignalDispColor = 0;
        }
    }


    updateCheckBox();
}

void IrBadPointSetWidget::on_checkBox_correct_clicked(bool checked)
{
    struGsh.nNirDotCorrectived = (checked) ? 1: 0;
    updateCheckBox();
}

void IrBadPointSetWidget::on_checkBox_origin_clicked(bool checked)
{
    struGsh.bCaliBiasEnable = checked;
    myFlow.cameraWaveCaliEnable();

}

void IrBadPointSetWidget::on_checkBox_cali_clicked(bool checked)
{
    struGsh.bCaliGainEnable = checked;
    myFlow.cameraWaveCaliEnable();
}

void IrBadPointSetWidget::on_checkBox_tdi_clicked()
{
    struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nNirTdiEnable = (struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nNirTdiEnable == 0) ? 1 : 0;
    myFlow.resetCameraNirTdiEnable();
    updateCheckBox();
}


void IrBadPointSetWidget::on_checkBox_red_2_clicked(bool checked)
{
    struGsh.nNirDotCorrectiveRed[struGsh.nView][struGsh.nUnit][1][struGsh.nIrCurPointIndex[1]-1] = (int)checked;
    updateCheckBox();
}

void IrBadPointSetWidget::on_checkBox_green_2_clicked(bool checked)
{
    struGsh.nNirDotCorrectiveGreen[struGsh.nView][struGsh.nUnit][1][struGsh.nIrCurPointIndex[1]-1] = (int)checked;
    updateCheckBox();
}

void IrBadPointSetWidget::on_checkBox_blue_2_clicked(bool checked)
{
    struGsh.nNirDotCorrectiveBlue[struGsh.nView][struGsh.nUnit][1][struGsh.nIrCurPointIndex[1]-1] = (int)checked;
    updateCheckBox();
}


void IrBadPointSetWidget::on_checkBox_n_clicked(bool checked)
{
    struGsh.nNirDotCorrectiveN[struGsh.nView][struGsh.nUnit][0][struGsh.nIrCurPointIndex[0]-1] = (int)checked;
    updateCheckBox();
}

void IrBadPointSetWidget::on_checkBox_n_2_clicked(bool checked)
{
    struGsh.nNirDotCorrectiveN[struGsh.nView][struGsh.nUnit][1][struGsh.nIrCurPointIndex[1]-1] = (int)checked;
    updateCheckBox();
}
