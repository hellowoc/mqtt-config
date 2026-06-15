#include "camerasignalwidget.h"
#include "ui_camerasignalwidget.h"
#include "global/globalflow.h"

enum
{
    Biasing = 0,
    Gain = 10,
    Target = 20,
    Focus = 30,
    Offset = 40
};

CameraSignalWidget::CameraSignalWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::CameraSignalWidget)
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

    m_edit[0][0] = ui->lineEdit_Biasing1;
    m_edit[0][1] = ui->lineEdit_Biasing2;
    m_edit[0][2] = ui->lineEdit_Biasing3;
    m_edit[0][3] = ui->lineEdit_Biasing4;

    m_edit[1][0] = ui->lineEdit_Gain1;
    m_edit[1][1] = ui->lineEdit_Gain2;
    m_edit[1][2] = ui->lineEdit_Gain3;
    m_edit[1][3] = ui->lineEdit_Gain4;

    m_edit[2][0] = ui->lineEdit_Target1;
    m_edit[2][1] = ui->lineEdit_Target2;
    m_edit[2][2] = ui->lineEdit_Target3;
    m_edit[2][3] = ui->lineEdit_Target4;

    m_label[0] = ui->label_11;
    m_label[1] = ui->label_12;
    m_label[2] = ui->label_14;
    m_label[3] = ui->label_13;

    m_labelAvg[0] = ui->avg_R_lbl;
    m_labelAvg[1] = ui->avg_G_lbl;
    m_labelAvg[2] = ui->avg_B_lbl;
    m_labelAvg[3] = ui->avg_N_lbl;

    m_labelFocus = ui->avg_focus_lbl;
    m_labelFocus2 = ui->avg_focus_lbl_2;

    m_pChuteWidget = ui->chuteWidget;
    m_pLayerBtn[0] = ui->mainBtn;
    m_pLayerBtn[1] = ui->viceBtn;
    setSlotConnect();

    myFlow.myNetWork->setUdpRecvType(CAPTURE_SINGLE_WAVE);
    connect(myFlow.myNetWork->udpthread,SIGNAL(SignalWaveRecvFinished()),this,SLOT(updateWave()));
    connect(myFlow.myNetWork->udpthread,SIGNAL(SignalSocketFailed()),this,SLOT(updateWave()));
    ui->widget->setDrawType(Gain_Page);

    ui->brightCaliSaveBtn->setVisible(false);
    ui->darkSaveBtn->setVisible(false);

    connect(this,SIGNAL(addBtnClicked()),ui->chuteWidget,SLOT(on_addBtn_clicked()));

    shottimer = new QTimer;
    connect(shottimer,SIGNAL(timeout()),this, SLOT(getscreenshot()));

    ui->caliModeWidget->setTitle("");
    ui->caliModeWidget->setParams(this,0,5,calimode_change,&bCurCal);

    tdModeList<<"SS"<<"SS1"<<"SS2"<<"SS3";
}

CameraSignalWidget::~CameraSignalWidget()
{
    delete ui;
}

void CameraSignalWidget::showPage(bool bshow)
{
    basewidget::showPage(bshow);
    if(bshow){
        myFlow.myNetWork->setUdpRecvType(CAPTURE_SINGLE_WAVE);
        myFlow.myNetWork->udpStart();
        updateViewButtonVisible();
        updateSetValue();
        updateChuteNum();
        updateUpdateCaliState();
        updateEditVisible();
        updateAnaGainLockBtnStatus();
        updateTDSwitchBtnStat();
        ui->colorRestore->setVisible(false);

        if(struGsh.bScreenShotStatus == 1)
        {
            shottimer->start(1000);

        }
        ui->pixelAdjust->setVisible(struGsh.debugMode == 1);

    }else{
        myFlow.myNetWork->udpStop();
        //        if(struGsh.bCaliBiasEnable == 0 || struGsh.bCaliGainEnable == 0){
        //            struGsh.bCaliBiasEnable = 1;
        //            struGsh.bCaliGainEnable = 1;
        //            myFlow.cameraWaveCaliEnable();
        //        }
    }
}

void CameraSignalWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == WAVE_MSG){
        updateWave();

    }else if(msg1 == MSG_DATA_CHANGED){
        if(msg2 == chute_change){
            basewidget::receiveMsg(msg1,msg2,msg3);
        }else if(msg2 == calimode_change){
            myMessageBox msgBox(MSG_QUES, g_myLan().cfmChgCorrection);

            int ret = msgBox.exec();
            if (ret == QDialog::Accepted)
            {
                struCnfp.bCurCal = bCurCal;
                g_infoWidget().setLabelText(g_myLan().msg_mode_changing);
                g_infoWidget().show();

                myFlow.cameraChangeCalibrate();

                updateSetValue();
                updateUpdateCaliState();

                myFlow.resetCamera();
                /* 发送相机板IP */
                if(struCnfg.nDPMachine)
                    myFlow.ipcResetCameraIpaddr(true, 0, 0, 0);

                /* 发送控制板参数 */
                myFlow.resetControl();
                /* 发送色选参数 */
               // myFlow.resetSortParams();
                /* 发送整机相机校正使能 */
                myFlow.cameraWaveCaliEnable();
                /* 发送灵敏度校正使能 */
                myFlow.resetSensUpdateEnable();
               // myFlow.initSendAllParams();

                myFlow.sleep(2);
                g_infoWidget().hide();

            }
            else{
                ui->caliModeWidget->setValue(struCnfp.bCurCal);
            }

        }else{
            if((msg2 >= Biasing)&&(msg2 < Gain))
            {
                myFlow.resetCameraBiasRG(false,struGsh.nLayer,struGsh.nView,struGsh.nUnit);
            }
            else if((msg2 >= Gain)&&(msg2 < Target))
            {
                myFlow.resetCameraAnaGainRG(false,struGsh.nLayer,struGsh.nView,struGsh.nUnit);
            }
            else if((msg2 >= Target)&&(msg2 < Target+4))
            {
                for(int i = 0;i < struCnfg.struLayerInfo[struGsh.nLayer].nViewUnitTotal[struGsh.nView];i++)
                {
                    if(msg2 == Target)
                        struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nCalibrateRed[i][struCnfp.bCurCal] = msg3;
                    else if(msg2 == Target+1)
                        struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nCalibrateGreen[i][struCnfp.bCurCal] = msg3;
                    else if(msg2 == Target+2)
                        struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nCalibrateBlue[i][struCnfp.bCurCal] = msg3;
                    else if(msg2 == Target+3)
                        struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nCalibrateNir[i][struCnfp.bCurCal] = msg3;
                }
                updateSetValue();
            }
            else if(msg2 >= Offset)
            {
                myFlow.resetIrCameraOffset(false,struGsh.nLayer,struGsh.nView,struGsh.nUnit);
            }
        }
    }
}

void CameraSignalWidget::OnViewChange(int view)
{
    updateWave();
    updateSetValue();
    updateEditVisible();
    updateAnaGainLockBtnStatus();
    updateTDSwitchBtnStat();
}

void CameraSignalWidget::OnChuteChange()
{
    updateSetValue();
    updateAnaGainLockBtnStatus();
}

void CameraSignalWidget::OnLayerChange()
{
    updateSetValue();
    updateEditVisible();
    updateAnaGainLockBtnStatus();
    updateTDSwitchBtnStat();
}

void CameraSignalWidget::retranslateUiWidget()
{
    ui->checkBox_origin->setText(g_myLan().camera_signal_bias_enable);
    ui->checkBox_cali->setText(g_myLan().camera_signal_cali_enable);
    ui->label->setText(g_myLan().ids_offset);
    ui->label_2->setText(g_myLan().ids_gain);
    ui->label_15->setText(g_myLan().ids_target);
    ui->label_11->setText(g_myLan().ids_red);
    ui->label_12->setText(g_myLan().ids_green);
    ui->label_14->setText(g_myLan().ids_blue);
    ui->label_13->setText(g_myLan().ids_n);
    ui->pixelAdjust->setText(g_myLan().pixel_join);
    ui->brightCaliBtn->setText(g_myLan().ids_getCorrection);
    ui->darkCaliBtn->setText(g_myLan().ids_getDark);
    ui->brightCaliSaveBtn->setText(g_myLan().ids_saveCorrection);
    ui->darkSaveBtn->setText(g_myLan().ids_saveDark);
    ui->groupBox_3->setTitle(tdModeList.at(struCnfp.nTimeDivisionScheme[struGsh.nLayer][struGsh.nView]));
    ui->colorRestore->setText(g_myLan().ids_color+g_myLan().restore);
    ui->pushButton_9->setText(g_myLan().ids_saveSet);

    ui->brightCaliOneKeyBtn->setText(g_myLan().ids_onekeyBright);
    ui->darkCaliOneKeyBtn->setText(g_myLan().ids_onekeyDark);
    updateAnaGainLockBtnStatus();
}

void CameraSignalWidget::getscreenshot()
{
    shottimer->stop();
    {
        for(int i=0;i<MAX_VIEW;i++)
        {
            if(struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[i])
            {
                struGsh.nUnit = 0;
                QMetaObject::invokeMethod(m_pViewBtn[i],"clicked", Qt::QueuedConnection);
                for(int j=0; j<struCnfg.nChuteTotal;j++)
                {
                    updateChuteNum();
                    myFlow.sleep(2);
                    myFlow.saveScreenshot();
                    emit addBtnClicked();
                }
            }
        }

        myFlow.sleep(1);
        struGsh.bScreenShotStatus = 0;
    }
}

void CameraSignalWidget::updateWave()
{
    ui->widget->update();
    updateAvgValue();
}

void CameraSignalWidget::updateSetValue()
{
    stru_view_camera_Info &cameraInfo = struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView];
    int chute = struGsh.nUnit;
    int curcal = struCnfp.bCurCal;
    int curPixels = myFlow.getSensorPixels(struGsh.nLayer, struGsh.nView);

    m_EditValueInt[0][0] = cameraInfo.nBiasingRed[chute];
    m_EditValueInt[0][1] = cameraInfo.nBiasingGreen[chute];
    m_EditValueInt[0][2] = cameraInfo.nBiasingBlue[chute];
    m_EditValueInt[0][3] = cameraInfo.nBiasingNir[chute];

    m_EditValueInt[1][0] = cameraInfo.nAnaGainRed[chute][curcal];
    m_EditValueInt[1][1] = cameraInfo.nAnaGainGreen[chute][curcal];
    m_EditValueInt[1][2] = cameraInfo.nAnaGainBlue[chute][curcal];
    m_EditValueInt[1][3] = cameraInfo.nAnaGainNir[chute][curcal];

    m_EditValueInt[2][0] = cameraInfo.nCalibrateRed[chute][curcal];
    m_EditValueInt[2][1] = cameraInfo.nCalibrateGreen[chute][curcal];
    m_EditValueInt[2][2] = cameraInfo.nCalibrateBlue[chute][curcal];
    m_EditValueInt[2][3] = cameraInfo.nCalibrateNir[chute][curcal];


    for(int i = 0;i < 3;i++){
        for(int j = 0;j < 4;j++){
            m_edit[i][j]->setText(QString::number(m_EditValueInt[i][j]));
            //            m_edit[i][j]->setMinMax(this,0,255,i*10+j,&m_EditValueInt[i][j]);
        }
    }

    ui->lineEdit_Biasing1->setMinMax(this,0,255,Biasing,&cameraInfo.nBiasingRed[chute]);
    ui->lineEdit_Biasing2->setMinMax(this,0,255,Biasing+1,&cameraInfo.nBiasingGreen[chute]);
    ui->lineEdit_Biasing3->setMinMax(this,0,255,Biasing+2,&cameraInfo.nBiasingBlue[chute]);
    ui->lineEdit_Biasing4->setMinMax(this,0,255,Biasing+3,&cameraInfo.nBiasingNir[chute]);

    //if(struCnfg.struLayerInfo[struGsh.nLayer].nViewSensorType[struGsh.nView] == SENSOR_D2)
    if(struCnfp.nViewSensorType[0][struGsh.nView] ==SENSOR_D2)
    {
        ui->lineEdit_Gain1->setMinMax(this,0,16,Gain,&cameraInfo.nAnaGainRed[chute][curcal]);
        ui->lineEdit_Gain2->setMinMax(this,0,16,Gain+1,&cameraInfo.nAnaGainGreen[chute][curcal]);
        ui->lineEdit_Gain3->setMinMax(this,0,16,Gain+2,&cameraInfo.nAnaGainBlue[chute][curcal]);
        ui->lineEdit_Gain4->setMinMax(this,0,16,Gain+3,&cameraInfo.nAnaGainNir[chute][curcal]);
    }
    else
    {
        ui->lineEdit_Gain1->setMinMax(this,0,255,Gain,&cameraInfo.nAnaGainRed[chute][curcal]);
        ui->lineEdit_Gain2->setMinMax(this,0,255,Gain+1,&cameraInfo.nAnaGainGreen[chute][curcal]);
        ui->lineEdit_Gain3->setMinMax(this,0,255,Gain+2,&cameraInfo.nAnaGainBlue[chute][curcal]);
        ui->lineEdit_Gain4->setMinMax(this,0,255,Gain+3,&cameraInfo.nAnaGainNir[chute][curcal]);
    }

    ui->lineEdit_Target1->setMinMax(this,0,255,Target,&cameraInfo.nCalibrateRed[chute][curcal]);
    ui->lineEdit_Target2->setMinMax(this,0,255,Target+1,&cameraInfo.nCalibrateGreen[chute][curcal]);
    ui->lineEdit_Target3->setMinMax(this,0,255,Target+2,&cameraInfo.nCalibrateBlue[chute][curcal]);
    ui->lineEdit_Target4->setMinMax(this,0,255,Target+3,&cameraInfo.nCalibrateNir[chute][curcal]);

    ui->focusBeginLineEdit->setMinMax(this,0,curPixels-2, Focus,&struGsh.focusAveBeginPixel);
    ui->focusEndLineEdit->setMinMax(this,0,curPixels-2, Focus,&struGsh.focusAveEndPixel);
    ui->focusBeginLineEdit_2->setMinMax(this,0,curPixels-2, Focus,&struGsh.focusAveBeginPixel2);
    ui->focusEndLineEdit_2->setMinMax(this,0,curPixels-2, Focus,&struGsh.focusAveEndPixel2);

    ui->focusBeginLineEdit->setText(QString::number(struGsh.focusAveBeginPixel));
    ui->focusEndLineEdit->setText(QString::number(struGsh.focusAveEndPixel));
    ui->focusBeginLineEdit_2->setText(QString::number(struGsh.focusAveBeginPixel2));
    ui->focusEndLineEdit_2->setText(QString::number(struGsh.focusAveEndPixel2));

    // 红外部分
    ui->lineEdit_gx->setMinMax(this,-48,48,Offset,&cameraInfo.nNirGreenOffsetX[chute]);
    ui->lineEdit_gy->setMinMax(this,-48,48,Offset+1,&cameraInfo.nNirGreenOffsetY[chute]);

    ui->lineEdit_gx->setText(QString::number(cameraInfo.nNirGreenOffsetX[chute]));
    ui->lineEdit_gy->setText(QString::number(cameraInfo.nNirGreenOffsetY[chute]));

    QString title = tdModeList.at(struCnfp.nTimeDivisionScheme[struGsh.nLayer][struGsh.nView])
            +"/"+QString::number(struCnfp.nSensorRowFre[struGsh.nLayer][struGsh.nView]/100.0,'f',2);
    ui->groupBox_3->setTitle(title);

    ui->caliModeWidget->setValue(struCnfp.bCurCal);

    if(struGsh.strumachineset.targetCount != 0)
    {
        for(int i = 0 ; i < struGsh.strumachineset.targetCount; i++)
        {
            ui->lineEdit_Target1->setEnabled(i >= struGsh.strumachineset.targetCount);
            ui->lineEdit_Target2->setEnabled(i >= struGsh.strumachineset.targetCount);
            ui->lineEdit_Target3->setEnabled(i >= struGsh.strumachineset.targetCount);
        }
    }
}

void CameraSignalWidget::updateAvgValue()
{
    ui->avg_focus_lbl->setText(QString::number(struGsh.focusAveVavle,'f',1));
    ui->avg_focus_lbl_2->setText(QString::number(struGsh.focusAveVavle2,'f',1));


    ui->avg_R_lbl->setText("R:"+QString::number(struGsh.fRowAveRed,'f',1));
    ui->avg_G_lbl->setText("G:"+QString::number(struGsh.fRowAveGreen,'f',1));
    ui->avg_B_lbl->setText("B:"+QString::number(struGsh.fRowAveBlue,'f',1));
    ui->avg_N_lbl->setText("I:"+QString::number(struGsh.fRowAveInf,'f',1));
}

void CameraSignalWidget::updateUpdateCaliState()
{
    ui->checkBox_cali->setChecked(struCnfp.bCaliGainEnable);
    ui->checkBox_origin->setChecked(struCnfp.bCaliBiasEnable);
    ui->brightCaliOneKeyBtn->setVisible(false);
}

void CameraSignalWidget::updateEditVisible()
{
    int dim = struCnfp.nSensorDim[0][struGsh.nView];
    for(int i = 0;i < 3;i++)
        for(int j = 0;j < 4;j++){
            m_edit[i][j]->setVisible(j<dim);
        }

    for(int j = 0;j < 4;j++){
        m_label[j]->setVisible(j < dim);
        m_labelAvg[j]->setVisible(j<dim);
    }

    //if(SENSOR_C1_2 == struCnfg.struLayerInfo[struGsh.nLayer].nViewSensorType[struGsh.nView])
    if(SENSOR_C1_2 == struCnfp.nViewSensorType[0][struGsh.nView])
    {
        ui->groupBox_pixel->show();
        ui->groupBox_pixel_2->show();
    }
    else
    {
        // ui->groupBox_pixel->hide();
        ui->groupBox_pixel_2->hide();
    }

    updateAvgValue();
}

void CameraSignalWidget::updateAnaGainLockBtnStatus()
{
    for(int i = 0; i < 4; i++)
    {
        m_edit[1][i]->setEnabled(true);
    }

    //if(struCnfg.struLayerInfo[struGsh.nLayer].nViewSensorType[struGsh.nView] == SENSOR_D2)
    if(struCnfp.nViewSensorType[0][struGsh.nView] == SENSOR_D2)
    {
        if(struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].anaGainLockStatus[struGsh.nUnit] == 1)
        {
            m_edit[1][struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].anaGainLockColor[struGsh.nUnit]]->setEnabled(false);
        }

        ui->pixelBad->setText(g_myLan().ana_gain_reset);
    }
    else
    {
        ui->pixelBad->setText(g_myLan().pixelBad);
    }
    //switch (struCnfg.struLayerInfo[struGsh.nLayer].nViewSensorType[struGsh.nView]) {
    switch(struCnfp.nViewSensorType[0][struGsh.nView])
    {
    case SENSOR_C1:
    case SENSOR_D2:
    case SENSOR_C1_2:
        ui->pixelBad->setVisible(true);
        break;
    default:
        ui->pixelBad->setVisible(false);
        break;
    }
}
void CameraSignalWidget::updateTDSwitchBtnStat()
{
    myFlow.updateCameraTimeDivisionMode(struGsh.nLayer,struGsh.nView);
    if(struGsh.nCamTdMode == TD_1024_A||struGsh.nCamTdMode == TD_2048_A )
        ui->TDSwitchBtn->setText("TD_A");
    else if(struGsh.nCamTdMode == TD_1024_B ||struGsh.nCamTdMode == TD_2048_B )
        ui->TDSwitchBtn->setText("TD_B");
    ui->TDSwitchBtn->setVisible(struGsh.nCamTdMode > 0);
}

void CameraSignalWidget::on_brightCaliBtn_clicked()
{
    myMessageBox msgBox(MSG_QUES, g_myLan().cfm_bright_cali_capture);
    int ret = msgBox.exec();
    if (ret == QDialog::Accepted){
        logger()->info("bright calibrate capturing");
        g_infoWidget().setLabelText(g_myLan().msg_bright_cali_capture);

        g_infoWidget().delayShow();
        myFlow.cameraBrightCalibrateRG(false, struGsh.nLayer, struGsh.nView, struGsh.nUnit, 0);
        myFlow.sleep(2);
        g_infoWidget().hide();

        //this->setEnabled(false);
        this->parentWidget()->parentWidget()->setEnabled(false);
        myFlow.sleep(1);
        //this->setEnabled(true);
        this->parentWidget()->parentWidget()->setEnabled(true);
        myMessageBox msgBox(MSG_QUES, g_myLan().cfm_bright_cali_save);
        int ret = msgBox.exec();
        if (ret == QDialog::Accepted)
        {
            logger()->info("bright calibrate saving");
            g_infoWidget().setLabelText(g_myLan().msg_bright_cali_save);

            g_infoWidget().delayShow();
            myFlow.cameraBrightCalibrateRG(false, struGsh.nLayer, struGsh.nView, struGsh.nUnit, 1);

            myFlow.cameraSaveCalibrate();

            //            for(int i = 0; i < MAX_LAMP_BOARD_PER_INTERFACE;i++){
            //                for(int j = 0; j < MAX_LAMP_PER_BOARD;j++){
            //                    struCnfg.stuLayerLampCfg.nLampValue[i][j][struCnfp.bCurCal] = struCnfp.nLampValue[0][i][j];
            //                    struCnfg.stuLayerLampCfg.nLampEnable[i][j][struCnfp.bCurCal] = struCnfp.nLampEnable[0][i][j];
            //                    struCnfg.stuLayerLampCfg.nLampSeq[i][j][struCnfp.bCurCal] = struCnfp.nLampSeq[0][i][j];
            //                }
            //            }
            //            for(int i = 0; i < MAX_VIEW; i++){
            //                struCnfg.nSenseBrightSelection_calb[i][struCnfp.bCurCal] = struCnfp.nSenseBrightSelection[struGsh.nLayer][i];
            //                struCnfg.nSensorRowFre_calb[i][struCnfp.bCurCal] = struCnfp.nSensorRowFre[struGsh.nLayer][i];
            //                struCnfg.bCaliTDmode_calb[i][struCnfp.bCurCal] = struCnfp.nTimeDivisionScheme[struGsh.nLayer][i];
            //            }
            //            struCnfg.bCaliGainEnable_calb[struCnfp.bCurCal] = struCnfp.bCaliGainEnable;
            //            struCnfg.bCaliBiasEnable_calb[struCnfp.bCurCal] = struCnfp.bCaliBiasEnable;


            myFlow.sleep(2);
            g_infoWidget().hide();
        }
    }
}

void CameraSignalWidget::on_brightCaliSaveBtn_clicked()
{
    myMessageBox msgBox(MSG_QUES, g_myLan().cfm_bright_cali_save);

    int ret = msgBox.exec();
    if (ret == QDialog::Accepted)
    {
        logger()->info("bright calibrate saving");
        g_infoWidget().setLabelText(g_myLan().msg_bright_cali_save);

        g_infoWidget().delayShow();
        myFlow.cameraBrightCalibrateRG(false, struGsh.nLayer, struGsh.nView, struGsh.nUnit, 1);
        myFlow.sleep(3);
        g_infoWidget().hide();
    }
}

void CameraSignalWidget::on_darkCaliBtn_clicked()
{
    myMessageBox msgBox(MSG_QUES, g_myLan().cfm_dark_cali_capture);
    int ret = msgBox.exec();
    if (ret == QDialog::Accepted){
        logger()->info("dark calibrate capturing");
        g_infoWidget().setLabelText(g_myLan().msg_dark_cali_capture);

        g_infoWidget().delayShow();
        myFlow.cameraDarkCalibrateRG(false, struGsh.nLayer, struGsh.nView, struGsh.nUnit, 0);
        myFlow.sleep(2);
        g_infoWidget().hide();

        this->parentWidget()->parentWidget()->setEnabled(false);
        myFlow.sleep(1);
        this->parentWidget()->parentWidget()->setEnabled(true);
        myMessageBox msgBox(MSG_QUES, g_myLan().cfm_dark_cali_save);
        int ret = msgBox.exec();
        if (ret == QDialog::Accepted)
        {
            logger()->info("dark calibrate saving");
            g_infoWidget().setLabelText(g_myLan().msg_dark_cali_save);

            g_infoWidget().delayShow();
            myFlow.cameraDarkCalibrateRG(false, struGsh.nLayer, struGsh.nView, struGsh.nUnit, 1);
            myFlow.sleep(2);
            g_infoWidget().hide();
        }
    }
}

void CameraSignalWidget::on_darkSaveBtn_clicked()
{
    myMessageBox msgBox(MSG_QUES, g_myLan().cfm_dark_cali_save);

    int ret = msgBox.exec();
    if (ret == QDialog::Accepted)
    {
        logger()->info("dark calibrate saving");
        g_infoWidget().setLabelText(g_myLan().msg_dark_cali_save);

        g_infoWidget().delayShow();
        myFlow.cameraDarkCalibrateRG(false, struGsh.nLayer, struGsh.nView, struGsh.nUnit, 1);
        myFlow.sleep(3);
        g_infoWidget().hide();
    }
}

void CameraSignalWidget::on_checkBox_origin_clicked(bool checked)
{
    struCnfp.bCaliBiasEnable = checked;
    myFlow.cameraWaveCaliEnable();
}

void CameraSignalWidget::on_checkBox_cali_clicked(bool checked)
{
    struCnfp.bCaliGainEnable = checked;
    myFlow.cameraWaveCaliEnable();
}

void CameraSignalWidget::on_pixelAdjust_clicked()
{
    logger()->info("enter pixel adjust page");
    g_MainManager().ShowWidget(SM_PIXEL_AJUST_WIDGET);
}

void CameraSignalWidget::on_pixelBad_clicked()
{
    //if(struCnfg.struLayerInfo[struGsh.nLayer].nViewSensorType[struGsh.nView] == SENSOR_D2)
    if(struCnfp.nViewSensorType[0][struGsh.nView] == SENSOR_D2)
    {
        logger()->info("reset gain page for D2 sensor");

        myMessageBox msgBox1(MSG_QUES, g_myLan().cfm_ana_gain_reset);

        int ret1 = msgBox1.exec();
        if (ret1 == QDialog::Accepted)
        {
            g_infoWidget().setLabelText(g_myLan().ana_gain_reset);

            g_infoWidget().delayShow();

            if((struCnfp.bCaliBiasEnable == 1)||(struCnfp.bCaliGainEnable == 1))
            {
                struCnfp.bCaliBiasEnable = 0;
                struCnfp.bCaliGainEnable = 0;

                myFlow.cameraWaveCaliEnable();
                ui->checkBox_cali->setChecked(struCnfp.bCaliGainEnable);
                ui->checkBox_origin->setChecked(struCnfp.bCaliBiasEnable);
                myFlow.sleep(1);
            }

            struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nAnaGainRed[struGsh.nUnit][struCnfp.bCurCal] = 0;
            struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nAnaGainGreen[struGsh.nUnit][struCnfp.bCurCal] = 0;
            struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nAnaGainBlue[struGsh.nUnit][struCnfp.bCurCal] = 0;
            struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nAnaGainNir[struGsh.nUnit][struCnfp.bCurCal] = 0;

            myFlow.resetCameraAnaGainRG(false,struGsh.nLayer,struGsh.nView,struGsh.nUnit);

            myFlow.sleep(2);
            struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].anaGainLockStatus[struGsh.nUnit] = 1;

            struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].anaGainLockColor[struGsh.nUnit] = 0;

            float max = struGsh.fRowAveRed;

            max = (max >= struGsh.fRowAveGreen) ? max : struGsh.fRowAveGreen;
            max = (max >= struGsh.fRowAveBlue) ? max : struGsh.fRowAveBlue;
            max = (max >= struGsh.fRowAveInf) ? max : struGsh.fRowAveInf;

            if(max == struGsh.fRowAveRed)
            {
                struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].anaGainLockColor[struGsh.nUnit] = 0;
            }
            else if(max == struGsh.fRowAveGreen)
            {
                struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].anaGainLockColor[struGsh.nUnit] = 1;
            }
            else if(max == struGsh.fRowAveBlue)
            {
                struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].anaGainLockColor[struGsh.nUnit] = 2;
            }
            else if(max == struGsh.fRowAveInf)
            {
                struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].anaGainLockColor[struGsh.nUnit] = 3;
            }

            for(int i = 0; i < 4; i++)
            {
                m_edit[1][struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].anaGainLockColor[struGsh.nUnit]]->setEnabled(true);
            }

            m_edit[1][struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].anaGainLockColor[struGsh.nUnit]]->setEnabled(false);

            updateSetValue();

            myFlow.sleep(1);
            g_infoWidget().hide();

        }

        updateAnaGainLockBtnStatus();
    }
    else
    {
        logger()->info("enter bad pixel corrcet page");
        g_MainManager().ShowWidget(SM_IR_BAD_POINT_WIDGET_I);
    }
}

void CameraSignalWidget::on_colorRestore_clicked()
{
    logger()->info("enter color restore page");
    g_MainManager().ShowWidget(SM_CAMERA_COLOR_RESTORE_WIDGET);
}

void CameraSignalWidget::on_darkCaliOneKeyBtn_clicked()
{
    myMessageBox msgBox(MSG_QUES, g_myLan().cfm_bright_cali_capture);
    int ret = msgBox.exec();
    if (ret == QDialog::Accepted){
        logger()->info("one key dark calibrate capturing");
        g_infoWidget().setLabelText(g_myLan().msg_dark_cali_capture);

        g_infoWidget().delayShow();
        myFlow.cameraDarkCalibrateRG(true, struGsh.nLayer, struGsh.nView, struGsh.nUnit, 0);
        myFlow.sleep(2);
        myFlow.cameraDarkCalibrateRG(true, struGsh.nLayer, struGsh.nView, struGsh.nUnit, 1);
        g_infoWidget().hide();
    }
}

void CameraSignalWidget::on_brightCaliOneKeyBtn_clicked()
{
    logger()->info("one key bright calibrate capturing");
    g_infoWidget().setLabelText(g_myLan().msg_bright_cali_capture);

    g_infoWidget().delayShow();
    myFlow.cameraBrightCalibrateRG(true, struGsh.nLayer, struGsh.nView, struGsh.nUnit, 0);
    myFlow.sleep(2);
    myFlow.cameraBrightCalibrateRG(true, struGsh.nLayer, struGsh.nView, struGsh.nUnit, 1);
    g_infoWidget().hide();
}

void CameraSignalWidget::on_TDSwitchBtn_clicked()
{
    if(struGsh.nCamTdMode < TD_2048_A)
        struGsh.nCamTdMode = (struGsh.nCamTdMode == TD_1024_A? TD_1024_B : TD_1024_A);
    else
        struGsh.nCamTdMode = (struGsh.nCamTdMode == TD_2048_A? TD_2048_B : TD_2048_A);

    struGsh.nTdMode = (struGsh.nCamTdMode == TD_1024_A || struGsh.nCamTdMode == TD_2048_A)? TD_1024_A : TD_1024_B;
    updateTDSwitchBtnStat();
}

void CameraSignalWidget::on_pushButton_9_clicked()
{

    logger()->info("bright calibrate value save");
    g_infoWidget().setLabelText(g_myLan().msg_applying);

    g_infoWidget().delayShow();
    myFlow.cameraSaveCalibrate();
    myFlow.sleep(2);
    g_infoWidget().hide();


}
