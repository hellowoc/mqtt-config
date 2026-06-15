#include "backgroundsetwidget.h"
#include "ui_backgroundsetwidget.h"
#include "mynetwork.h"
#include "global/globalflow.h"

enum{
    msg_bg_value_up = 10,
    msg_bg_value_low= 11,
    msg_bg_propotion_up = 12,
    msg_bg_propotion_low= 13,
    msg_bg_hsv_value_up = 14,
    msg_bg_hsv_value_low= 15,
};

BackGroundSetWidget::BackGroundSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::BackGroundSetWidget)
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

    m_pChuteWidget = ui->chuteWidget;
    m_pLayerBtn[0] = ui->mainBtn;
    m_pLayerBtn[1] = ui->viceBtn;
    setSlotConnect();
    m_labelAvg[0] = ui->avg_R_lbl;
    m_labelAvg[1] = ui->avg_G_lbl;
    m_labelAvg[2] = ui->avg_B_lbl;
    m_labelAvg[3] = ui->avg_N_lbl;
    myFlow.myNetWork->setUdpRecvType(CAPTURE_SINGLE_WAVE);
    connect(myFlow.myNetWork->udpthread,SIGNAL(SignalWaveRecvFinished()),this,SLOT(updateWave()));
    ui->widget->setDrawType(BackGround_Page);

    connect(this,SIGNAL(addBtnClicked()),ui->chuteWidget,SLOT(on_addBtn_clicked()));
    shottimer = new QTimer;
    connect(shottimer,SIGNAL(timeout()),this, SLOT(getscreenshot()));
}

BackGroundSetWidget::~BackGroundSetWidget()
{
    delete ui;
}

void BackGroundSetWidget::showPage(bool bshow)
{
    //    myFlow.myNetWork->udpthread->isrunning = false;
    basewidget::showPage(bshow);
    if(bshow){
        myFlow.myNetWork->setUdpRecvType(CAPTURE_SINGLE_WAVE);

        myFlow.myNetWork->udpStart();

        updateView();
        updateUpdateCaliState();

        if(struGsh.bScreenShotStatus == 1)
        {
            shottimer->start(1000);
        }

    }else{
        myFlow.myNetWork->udpStop();
    }

}

void BackGroundSetWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == WAVE_MSG){
        ui->widget->update();
    }else if(msg1 == MSG_DATA_CHANGED){
        if(msg2 == chute_change){
            basewidget::receiveMsg(msg1,msg2,msg3);
            updateView();
        }
        else if((msg2 >= msg_bg_value_up)&&(msg2 <= msg_bg_hsv_value_low))
        {
            QString tmpStr = QString("adjust bg view is %1, color is %2, value low-upper is %3-%4, propotion low-up is %5-%6").arg(struGsh.nView).arg(struCnfp.nBackgroundColor[struGsh.nLayer][struGsh.nView][struGsh.nTdMode])
                    .arg(struCnfp.nBackgroundColorValueLow[struGsh.nLayer][struGsh.nView][struGsh.nTdMode]).arg(struCnfp.nBackgroundColorValueUp[struGsh.nLayer][struGsh.nView][struGsh.nTdMode])
                    .arg(struCnfp.nBackgroundColorProLow[struGsh.nLayer][struGsh.nView][struGsh.nTdMode]).arg(struCnfp.nBackgroundColorProUp[struGsh.nLayer][struGsh.nView][struGsh.nTdMode]);
            logger()->info(tmpStr.toLatin1().data());
            myFlow.resetBackgroundColorRG(true,struGsh.nLayer,struGsh.nView);
        }
    }
}

void BackGroundSetWidget::OnViewChange(int view)
{
    ui->widget->update();
    updateView();
}

void BackGroundSetWidget::OnChuteChange()
{
    ui->widget->update();
    updateView();
}

void BackGroundSetWidget::OnLayerChange()
{
    ui->widget->update();
    updateView();
}

void BackGroundSetWidget::retranslateUiWidget()
{
    ui->checkBox_bias->setText(g_myLan().camera_signal_bias_enable);
    ui->checkBox_cali->setText(g_myLan().camera_signal_cali_enable);
    ui->label->setText(g_myLan().ids_color);
    ui->groupBox->setTitle(g_myLan().ids_threshold);
    ui->groupBox_2->setTitle(g_myLan().ids_ratio);
    ui->groupBox_4->setTitle(g_myLan().ids_threshold+"2");
    ui->label_4->setText(g_myLan().ids_uplimit);
    ui->label_3->setText(g_myLan().ids_lowlimit);
    ui->label_5->setText(g_myLan().ids_uplimit);
    ui->label_6->setText(g_myLan().ids_lowlimit);
    ui->label_11->setText(g_myLan().ids_uplimit);
    ui->label_12->setText(g_myLan().ids_lowlimit);
}

void BackGroundSetWidget::getscreenshot()
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

void BackGroundSetWidget::updateWave()
{
    if (struGsh.nRemoteImageCaptureIsRun) return;
    ui->widget->update();
    updateAvgValue();
}

void BackGroundSetWidget::updateView()
{
    updateTDSwitchBtnStat();
    updateAvgVisiable();

    ui->comboBox->clear();
    QStringList colorList;
    //switch(struCnfg.struLayerInfo[struGsh.nLayer].nViewSensorType[struGsh.nView])
    switch(struCnfp.nViewSensorType[0][struGsh.nView])
    {
    case SENSOR_C1:
        colorList<<g_myLan().ids_red<<g_myLan().ids_green<<g_myLan().ids_blue<<g_myLan().ids_black_white<<g_myLan().grey<<g_myLan().yellow;
        break;
    case SENSOR_D2:
        colorList<<g_myLan().ids_red<<g_myLan().ids_green<<g_myLan().ids_blue<<g_myLan().ids_black_white<<g_myLan().grey<<g_myLan().yellow<<g_myLan().ids_n;
        break;
    case SENSOR_T1:
    case SENSOR_T1_850:
        colorList<<g_myLan().ids_red<<g_myLan().ids_green<<g_myLan().ids_blue<<g_myLan().ids_black_white<<g_myLan().grey<<g_myLan().yellow;
        break;
    default:
        colorList<<g_myLan().ids_red<<g_myLan().ids_green<<g_myLan().ids_blue<<g_myLan().ids_black_white<<g_myLan().grey<<g_myLan().yellow;
        break;
    }

    ui->comboBox->addItems(colorList);
    if(struCnfp.nBackgroundColor[struGsh.nLayer][struGsh.nView][struGsh.nTdMode] < colorList.count())
        ui->comboBox->setCurrentIndex(struCnfp.nBackgroundColor[struGsh.nLayer][struGsh.nView][struGsh.nTdMode]);
    else
        ui->comboBox->setCurrentIndex(0);

    switch(struCnfp.nBackgroundColor[struGsh.nLayer][struGsh.nView][struGsh.nTdMode])
    {
    case BACKGROUND_COLOR_RED:
    case BACKGROUND_COLOR_GREEN:
    case BACKGROUND_COLOR_BLUE:
        ui->valueUpEdit->hide();
        ui->propertyUpEdit->hide();
        ui->propertyLowEdit->show();
        ui->label_4->hide();
        ui->label_5->hide();
        ui->label_3->hide();
        ui->label_6->hide();
        ui->groupBox_2->show();
        ui->groupBox_4->hide();
        break;
    case BACKGROUND_COLOR_BLACK_WHITE:
    case BACKGROUND_COLOR_GREY:
        ui->valueUpEdit->show();
        ui->propertyUpEdit->hide();
        ui->propertyLowEdit->hide();
        ui->label_4->show();
        ui->label_5->hide();
        ui->label_6->hide();
        ui->label_3->show();
        ui->groupBox_2->hide();
        ui->groupBox_4->hide();
        break;
    case BACKGROUND_COLOR_YELLOW:
        ui->valueUpEdit->show();
        ui->propertyUpEdit->hide();
        ui->propertyLowEdit->hide();
        ui->label_4->show();
        ui->label_5->hide();
        ui->label_6->hide();
        ui->label_3->show();
        ui->groupBox_2->hide();
        ui->groupBox_4->show();
        break;
    case BACKGROUND_COLOR_NIR:
        ui->valueUpEdit->show();
        ui->propertyUpEdit->hide();
        ui->propertyLowEdit->hide();
        ui->label_4->show();
        ui->label_5->hide();
        ui->label_6->hide();
        ui->label_3->show();
        ui->groupBox_2->hide();
        ui->groupBox_4->hide();
        break;
    default:
        break;
    }

    if(struCnfp.nBackgroundColor[struGsh.nLayer][struGsh.nView][struGsh.nTdMode] == BACKGROUND_COLOR_YELLOW)
    {
        ui->valueUpEdit->setMinMax(this,0,359,msg_bg_value_up,&struCnfp.nBackgroundColorValueUp[struGsh.nLayer][struGsh.nView][struGsh.nTdMode]);
        ui->ValueLowEdit->setMinMax(this,0,359,msg_bg_value_low,&struCnfp.nBackgroundColorValueLow[struGsh.nLayer][struGsh.nView][struGsh.nTdMode]);
    }else{
        ui->valueUpEdit->setMinMax(this,0,255,msg_bg_value_up,&struCnfp.nBackgroundColorValueUp[struGsh.nLayer][struGsh.nView][struGsh.nTdMode]);
        ui->ValueLowEdit->setMinMax(this,0,255,msg_bg_value_low,&struCnfp.nBackgroundColorValueLow[struGsh.nLayer][struGsh.nView][struGsh.nTdMode]);
    }

    ui->propertyUpEdit->setMinMax(this,0,100,msg_bg_propotion_up,&struCnfp.nBackgroundColorProUp[struGsh.nLayer][struGsh.nView][struGsh.nTdMode]);
    ui->propertyLowEdit->setMinMax(this,0,100,msg_bg_propotion_low,&struCnfp.nBackgroundColorProLow[struGsh.nLayer][struGsh.nView][struGsh.nTdMode]);

    ui->HSVValueUpEdit->setMinMax(this,0,255,msg_bg_hsv_value_up,&struCnfp.nBackgroundColorValueVUp[struGsh.nLayer][struGsh.nView][struGsh.nTdMode]);
    ui->HSVValueLowEdit->setMinMax(this,0,255,msg_bg_hsv_value_low,&struCnfp.nBackgroundColorValueVLow[struGsh.nLayer][struGsh.nView][struGsh.nTdMode]);

    ui->valueUpEdit->setText(QString::number(struCnfp.nBackgroundColorValueUp[struGsh.nLayer][struGsh.nView][struGsh.nTdMode]));
    ui->ValueLowEdit->setText(QString::number(struCnfp.nBackgroundColorValueLow[struGsh.nLayer][struGsh.nView][struGsh.nTdMode]));

    ui->propertyLowEdit->setText(QString::number(struCnfp.nBackgroundColorProLow[struGsh.nLayer][struGsh.nView][struGsh.nTdMode]));
    ui->propertyUpEdit->setText(QString::number(struCnfp.nBackgroundColorProUp[struGsh.nLayer][struGsh.nView][struGsh.nTdMode]));

    ui->HSVValueUpEdit->setText(QString::number(struCnfp.nBackgroundColorValueVUp[struGsh.nLayer][struGsh.nView][struGsh.nTdMode]));
    ui->HSVValueLowEdit->setText(QString::number(struCnfp.nBackgroundColorValueVLow[struGsh.nLayer][struGsh.nView][struGsh.nTdMode]));
}

void BackGroundSetWidget::updateAvgVisiable()
{
    int dim = struCnfp.nSensorDim[struGsh.nLayer][struGsh.nView];
    for(int j = 0;j < 4;j++){
        m_labelAvg[j]->setVisible(j<dim);
    }
    updateAvgValue();
}

void BackGroundSetWidget::updateAvgValue()
{
    ui->avg_R_lbl->setText("R:"+QString::number(struGsh.fRowAveRed,'f',1));
    ui->avg_G_lbl->setText("G:"+QString::number(struGsh.fRowAveGreen,'f',1));
    ui->avg_B_lbl->setText("B:"+QString::number(struGsh.fRowAveBlue,'f',1));
    ui->avg_N_lbl->setText("N:"+QString::number(struGsh.fRowAveInf,'f',1));

}

void BackGroundSetWidget::updateUpdateCaliState()
{
    ui->checkBox_cali->setChecked(struCnfp.bCaliGainEnable);
    ui->checkBox_bias->setChecked(struCnfp.bCaliBiasEnable);
    if (struGsh.bRunMode == RUNMODE_ENGINEER){
        ui->checkBox_bias->hide();
        ui->checkBox_cali->hide();
        if (!struCnfp.bCaliGainEnable || !struCnfp.bCaliBiasEnable){
            struCnfp.bCaliBiasEnable = 1;
            struCnfp.bCaliGainEnable = 1;
            myFlow.cameraWaveCaliEnable();
        }
    }
    else{
        ui->checkBox_bias->show();
        ui->checkBox_cali->show();
    }
}
void BackGroundSetWidget::updateTDSwitchBtnStat()
{
    myFlow.updateCameraTimeDivisionMode(struGsh.nLayer,struGsh.nView);
    if(struGsh.nCamTdMode == TD_1024_A ||struGsh.nCamTdMode == TD_2048_A )
        ui->TDSwitchBtn->setText("TD_A");
    else if(struGsh.nCamTdMode == TD_1024_B ||struGsh.nCamTdMode == TD_2048_B )
        ui->TDSwitchBtn->setText("TD_B");
    ui->TDSwitchBtn->setVisible(struGsh.nCamTdMode > 0);
}
void BackGroundSetWidget::on_comboBox_activated(int index)
{
    struCnfp.nBackgroundColor[struGsh.nLayer][struGsh.nView][struGsh.nTdMode] = index;
    updateView();
    myFlow.resetBackgroundColorRG(true,struGsh.nLayer,struGsh.nView);
}

void BackGroundSetWidget::on_checkBox_bias_clicked(bool checked)
{
    struCnfp.bCaliBiasEnable = checked;
    myFlow.cameraWaveCaliEnable();
}

void BackGroundSetWidget::on_checkBox_cali_clicked(bool checked)
{
    struCnfp.bCaliGainEnable = checked;
    myFlow.cameraWaveCaliEnable();
}

void BackGroundSetWidget::on_TDSwitchBtn_clicked()
{
    if(struGsh.nCamTdMode < TD_2048_A)
        struGsh.nCamTdMode = (struGsh.nCamTdMode == TD_1024_A ? TD_1024_B : TD_1024_A);
    else
        struGsh.nCamTdMode = (struGsh.nCamTdMode == TD_2048_A ? TD_2048_B : TD_2048_A);

    struGsh.nTdMode = (struGsh.nCamTdMode == TD_1024_A || struGsh.nCamTdMode == TD_2048_A)? TD_1024_A : TD_1024_B;
    updateView();
    ui->widget->update();
}
