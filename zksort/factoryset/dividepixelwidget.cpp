#include "dividepixelwidget.h"
#include "ui_dividepixelwidget.h"

enum
{
    msg_pixel_begin = 10,
    msg_pixel_end = 11,
    msg_eject_begin = 12,
    msg_eject_end = 13

};

DividePixelWidget::DividePixelWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::DividePixelWidget)
{
    ui->setupUi(this);
    m_pViewBtn[0] = ui->pushButton;
    m_pViewBtn[1] = ui->pushButton_2;
    m_pViewBtn[2] = ui->pushButton_3;
    m_pViewBtn[3] = ui->pushButton_4;
    m_pViewBtn[4] = ui->pushButton_6;
    m_pViewBtn[5] = ui->pushButton_7;
    m_pViewBtn[6] = ui->pushButton_8;
    m_pViewBtn[7] = ui->pushButton_9;
    m_pChuteWidget = ui->chuteWidget;
    m_pLayerBtn[0] = ui->mainBtn;
    m_pLayerBtn[1] = ui->viceBtn;
    setSlotConnect();
    m_labelAvg[0] = ui->avg_R_lbl;
    m_labelAvg[1] = ui->avg_G_lbl;
    m_labelAvg[2] = ui->avg_B_lbl;
    m_labelAvg[3] = ui->avg_N_lbl;

    ui->sendBtn->hide();
    myFlow.myNetWork->setUdpRecvType(CAPTURE_SINGLE_WAVE);

    connect(myFlow.myNetWork->udpthread,SIGNAL(SignalWaveRecvFinished()),this,SLOT(updateWave()));
    connect(myFlow.myNetWork->udpthread,SIGNAL(SignalSocketFailed()),this,SLOT(updateWave()));

    m_showSourceData = false;
    ui->widget->setDrawType(Divide_PIXEL,m_showSourceData);
}

DividePixelWidget::~DividePixelWidget()
{
    delete ui;
}

void DividePixelWidget::showPage(bool bshow)
{
    basewidget::showPage(bshow);
    if(bshow){
        updateValue();
        updateUpdateCaliState();
        myFlow.myNetWork->setUdpRecvType(CAPTURE_SINGLE_WAVE);
        myFlow.myNetWork->udpStart();

    }else{
        myFlow.myNetWork->udpStop();
        //        if(struGsh.bCaliBiasEnable == 0 || struGsh.bCaliGainEnable == 0){
        //            struGsh.bCaliBiasEnable = 1;
        //            struGsh.bCaliGainEnable = 1;
        //            myFlow.cameraWaveCaliEnable();
        //        }
    }
}

void DividePixelWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == WAVE_MSG){
        updateWave();
    }else if(msg1 == MSG_DATA_CHANGED){
        if(msg2 == chute_change){
            basewidget::receiveMsg(msg1,msg2,msg3);
        }
        else if((msg2==msg_pixel_begin)||(msg2==msg_pixel_end))
        {
            struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][struGsh.nUnit].nHoriOffset = struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nChannelBegin[struGsh.nUnit];
            struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][struGsh.nUnit].nHoriWidth
                    = struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nChannelEnd[struGsh.nUnit]-struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nChannelBegin[struGsh.nUnit];
            if(struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nChannelEnd2[struGsh.nUnit] >
                    struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nChannelEnd[struGsh.nUnit])
                struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][struGsh.nUnit].nHoriWidth_2
                        = struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nChannelEnd2[struGsh.nUnit]-
                        struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nChannelEnd[struGsh.nUnit]+1;
            myFlow.configIpcCamera();
            myFlow.resetCameraPixelRG(false,struGsh.nLayer,struGsh.nView,struGsh.nUnit);
            updateWave();
        }
        else if(msg2 == msg_eject_begin ||msg2 == msg_eject_end)
        {
            myFlow.resetCameraPixelRG(false,struGsh.nLayer,struGsh.nView,struGsh.nUnit);
            updateValue();
        }
        else{
            updateWave();
        }
    }
}

void DividePixelWidget::OnViewChange(int view)
{
    updateWave();
    updateValue();
}

void DividePixelWidget::OnChuteChange()
{
    updateValue();
}

void DividePixelWidget::OnLayerChange()
{
    updateValue();
}

void DividePixelWidget::retranslateUiWidget()
{
    ui->checkBox_origin->setText(g_myLan().camera_signal_bias_enable);
    ui->checkBox_cali->setText(g_myLan().camera_signal_cali_enable);
    ui->sendBtn->setText(g_myLan().ids_apply);
    ui->pushButton_5->setText(g_myLan().ids_pixelList);
    ui->checkBox_source->setText(g_myLan().ids_originalSignal);
    ui->checkBox_stop->setText(g_myLan().ids_stop);
}

void DividePixelWidget::updateWave()
{
    ui->widget->update();
    updateAvgValue();
}

void DividePixelWidget::updateValue()
{
    updateAvgVisiable();
    int sensorPixel = myFlow.getSensorPixels(struGsh.nLayer,struGsh.nView)-1;
    int maxejectnum = struCnfg.nChuteTotal*struCnfg.struLayerInfo[struGsh.nLayer].nEjectorsPerChute;
    //    ui->lineEdit_StartPix->setMinMax(this,0,startMax,1,&struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nChannelBegin[struGsh.nUnit]);
    //    ui->lineEdit_EndPix->setMinMax(this,endMin,sensorPixel,2,&struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nChannelEnd[struGsh.nUnit]);
    //    ui->lineEdit_StartPix->setText(QString::number(struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nChannelBegin[struGsh.nUnit]));
    //    ui->lineEdit_EndPix->setText(QString::number(struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nChannelEnd[struGsh.nUnit]));

    ui->startPix->setTitle(g_myLan().ids_startPixel);
    ui->endPix->setTitle(g_myLan().ids_endPixel);
    ui->startPix->setParams(this,1,sensorPixel,msg_pixel_begin,&struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nChannelBegin[struGsh.nUnit]);
    ui->endPix->setParams(this,1,sensorPixel,msg_pixel_end,&struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nChannelEnd[struGsh.nUnit]);
    ui->startPix->setValue(struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nChannelBegin[struGsh.nUnit]);
    ui->endPix->setValue(struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nChannelEnd[struGsh.nUnit]);
    ui->startPix_2->setTitle(g_myLan().ids_startPixel+"2");
    ui->endPix_2->setTitle(g_myLan().ids_endPixel+"2");
    ui->startPix_2->setParams(this,0,sensorPixel,msg_pixel_begin,&struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nChannelBegin2[struGsh.nUnit]);
    ui->endPix_2->setParams(this,0,sensorPixel,msg_pixel_end,&struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nChannelEnd2[struGsh.nUnit]);
    ui->startPix_2->setValue(struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nChannelBegin2[struGsh.nUnit]);
    ui->endPix_2->setValue(struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nChannelEnd2[struGsh.nUnit]);

    ui->startEject->setTitle(g_myLan().ejectstart);
    ui->endEject->setTitle(g_myLan().ejectend);

    ui->startEject->setParams(this,1,struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nEjectEnd[struGsh.nUnit],
            msg_eject_begin,&struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nEjectBegin[struGsh.nUnit]);
    ui->endEject->setParams(this,struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nEjectBegin[struGsh.nUnit],
            maxejectnum,msg_eject_end,&struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nEjectEnd[struGsh.nUnit]);
    ui->startEject->setValue(struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nEjectBegin[struGsh.nUnit]);
    ui->endEject->setValue(struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nEjectEnd[struGsh.nUnit]);
    ui->startEject_2->setTitle(g_myLan().ejectstart+"2");
    ui->endEject_2->setTitle(g_myLan().ejectend+"2");
    ui->startEject_2->setParams(this,0,struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nEjectEnd2[struGsh.nUnit],
            msg_eject_begin,&struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nEjectBegin2[struGsh.nUnit]);
    ui->endEject_2->setParams(this,struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nEjectBegin2[struGsh.nUnit],
            maxejectnum,msg_eject_end,&struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nEjectEnd2[struGsh.nUnit]);
    ui->startEject_2->setValue(struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nEjectBegin2[struGsh.nUnit]);
    ui->endEject_2->setValue(struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nEjectEnd2[struGsh.nUnit]);

    ui->startPix_2->setEnabled(struGsh.debugMode);
    ui->endPix_2->setEnabled(struGsh.debugMode);
    ui->startEject->setEnabled(struGsh.debugMode);
    ui->startEject_2->setEnabled(struGsh.debugMode);
    ui->endEject->setEnabled(struGsh.debugMode);
    ui->endEject_2->setEnabled(struGsh.debugMode);
}

void DividePixelWidget::updateUpdateCaliState()
{
    ui->checkBox_cali->setChecked(struCnfp.bCaliGainEnable);
    ui->checkBox_origin->setChecked(struCnfp.bCaliBiasEnable);
    ui->checkBox_source->setChecked(m_showSourceData);
    ui->checkBox_stop->setChecked(m_showStop);
}

void DividePixelWidget::updateAvgVisiable()
{
    int dim = struCnfp.nSensorDim[0][struGsh.nView];
    for(int j = 0;j < 4;j++){
        m_labelAvg[j]->setVisible(j<dim);
    }
    updateAvgValue();
}

void DividePixelWidget::updateAvgValue()
{
    ui->avg_R_lbl->setText("R:"+QString::number(struGsh.fRowAveRed,'f',1));
    ui->avg_G_lbl->setText("G:"+QString::number(struGsh.fRowAveGreen,'f',1));
    ui->avg_B_lbl->setText("B:"+QString::number(struGsh.fRowAveBlue,'f',1));
    ui->avg_N_lbl->setText("I:"+QString::number(struGsh.fRowAveInf,'f',1));
}

void DividePixelWidget::on_pushButton_5_clicked()
{
    logger()->info("enter deviding pixels show all page");
    g_MainManager().ShowWidget(SM_DIVIDE_PIXEL_SHOWALL_WIDGET);
}

void DividePixelWidget::on_sendBtn_clicked()
{
    myFlow.getCameraEjectorPixel(struGsh.nLayer,struGsh.nView,struGsh.nUnit);
    myFlow.resetCameraPixelRG(false,struGsh.nLayer,struGsh.nView,struGsh.nUnit);

    updateWave();
}

void DividePixelWidget::on_checkBox_origin_clicked(bool checked)
{
    struCnfp.bCaliBiasEnable = checked;
    myFlow.cameraWaveCaliEnable();
}

void DividePixelWidget::on_checkBox_cali_clicked(bool checked)
{
    struCnfp.bCaliGainEnable = checked;
    myFlow.cameraWaveCaliEnable();
}

void DividePixelWidget::on_checkBox_source_clicked(bool checked)
{
    m_showSourceData = checked;
    ui->widget->setDrawType(Divide_PIXEL,m_showSourceData);
    updateWave();
}

void DividePixelWidget::on_checkBox_stop_clicked(bool checked)
{
    if(checked){
        myFlow.myNetWork->udpStop();
    }else{
        myFlow.myNetWork->setUdpRecvType(CAPTURE_SINGLE_WAVE);
        myFlow.myNetWork->udpStart();
    }
}
