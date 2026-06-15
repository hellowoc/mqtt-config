#include "hsvsensesetwidget.h"
#include "ui_hsvsensesetwidget.h"

HsvSenseSetWidget::HsvSenseSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::HsvSenseSetWidget)
{
    ui->setupUi(this);
    m_pChuteTitle[0] = ui->label_1;
    m_pChuteTitle[1] = ui->label_2;
    m_pChuteTitle[2] = ui->label_3;
    m_pChuteTitle[3] = ui->label_4;
    m_pChuteTitle[4] = ui->label_5;
    m_pChuteTitle[5] = ui->label_6;
    m_pChuteTitle[6] = ui->label_7;
    m_pChuteTitle[7] = ui->label_8;
    m_pChuteTitle[8] = ui->label_9;
    m_pChuteTitle[9] = ui->label_10;
    m_pChuteTitle[10] = ui->label_11;
    m_pChuteTitle[11] = ui->label_12;

    m_pChuteSenseEdit[0][0] = ui->lineEdit;
    m_pChuteSenseEdit[0][1] = ui->lineEdit_2;
    m_pChuteSenseEdit[0][2] = ui->lineEdit_3;
    m_pChuteSenseEdit[0][3] = ui->lineEdit_4;
    m_pChuteSenseEdit[0][4] = ui->lineEdit_5;
    m_pChuteSenseEdit[0][5] = ui->lineEdit_6;
    m_pChuteSenseEdit[0][6] = ui->lineEdit_7;
    m_pChuteSenseEdit[0][7] = ui->lineEdit_8;
    m_pChuteSenseEdit[0][8] = ui->lineEdit_9;
    m_pChuteSenseEdit[0][9] = ui->lineEdit_10;
    m_pChuteSenseEdit[0][10] = ui->lineEdit_11;
    m_pChuteSenseEdit[0][11] = ui->lineEdit_12;

    m_pChuteSenseEdit[1][0] = ui->lineEdit_1_0;
    m_pChuteSenseEdit[1][1] = ui->lineEdit_1_1;
    m_pChuteSenseEdit[1][2] = ui->lineEdit_1_2;
    m_pChuteSenseEdit[1][3] = ui->lineEdit_1_3;
    m_pChuteSenseEdit[1][4] = ui->lineEdit_1_4;
    m_pChuteSenseEdit[1][5] = ui->lineEdit_1_5;
    m_pChuteSenseEdit[1][6] = ui->lineEdit_1_6;
    m_pChuteSenseEdit[1][7] = ui->lineEdit_1_7;
    m_pChuteSenseEdit[1][8] = ui->lineEdit_1_8;
    m_pChuteSenseEdit[1][9] = ui->lineEdit_1_9;
    m_pChuteSenseEdit[1][10] = ui->lineEdit_1_10;
    m_pChuteSenseEdit[1][11] = ui->lineEdit_1_11;

    m_pGroupLbl = ui->label_group;
    m_pModeBtnGroup = new QButtonGroup(this);
    m_pModeBtnGroup->addButton(ui->darkBtn,0);
    m_pModeBtnGroup->addButton(ui->lightBtn,1);

//    ui->standardlbl->hide();
//    ui->standard_titlelbl->hide();
//    ui->label_front->hide();
//    ui->label_rear->hide();
    for(int i = 0;i < 2;i++){
        for(int j = 0;j < 12;j++){
//            m_pChuteTitle[j]->hide();
//            m_pChuteSenseEdit[i][j]->hide();
        }
        m_pModeBtnGroup->button(i)->setCheckable(true);
        connect(m_pModeBtnGroup->button(i),SIGNAL(clicked(bool)),this,SLOT(modeBtnClicked(bool)));
    }
    connect(ui->sNamelineEdit,SIGNAL(valueSetSignal()),this,SLOT(sNameChanged()));
    connect(ui->hsvCircleWidget,SIGNAL(colorChanged()),this,SLOT(hsvColorChanged()));

    QStringList radioList;
    radioList<<"1:1"<<"2:1";
    ui->comboBox_ratio->clear();
    ui->comboBox_ratio->addItems(radioList);

    ui->comboBox_ratio->hide();
    ui->label_radio->hide();
    ui->comboBox_2->hide();
    ui->extendBtn->setText("");
    ui->extendBtn->setCheckable(true);
}

HsvSenseSetWidget::~HsvSenseSetWidget()
{
    delete ui;
}


void HsvSenseSetWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    const stru_iden_group_info &groupInfo = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].stuIdenGupInfo[struGsh.nMatGroup];

    int unitIndex = msg2%MAX_VIEW_UNIT_USE;
    int indexInGroup = 0;
    for(int i = 0;i < groupInfo.nGroupUnitTotal;i++){
        if(unitIndex == groupInfo.nUnitGupId[i])//视链上第几个相机
        {
            indexInGroup = i;
            break;
        }
    }
    if(msg1 == MSG_DATA_CHANGED){

        if(msg2 < MAX_VIEW_UNIT_USE*2){
            if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT){//前后视相同
                if(msg2<MAX_VIEW_UNIT_USE)
                {
                    if(groupInfo.nUnitGupSplice[indexInGroup] == 2){
                        struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][1].hsv[struGsh.nArithIndex-ARITH_HSV]= m_ChuteSensFloat[msg2/MAX_VIEW_UNIT_USE][unitIndex]*10+0.5;
                    }else{
                        struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].hsv[struGsh.nArithIndex-ARITH_HSV]= m_ChuteSensFloat[msg2/MAX_VIEW_UNIT_USE][unitIndex]*10+0.5;
                    }
                }
                else
                {
                    if(groupInfo.nUnitGupSplice[indexInGroup] == 2){
                        struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][unitIndex][1].hsv[struGsh.nArithIndex-ARITH_HSV] = m_ChuteSensFloat[msg2/MAX_VIEW_UNIT_USE][unitIndex]*10+0.5;
                    }else{
                        struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][unitIndex][0].hsv[struGsh.nArithIndex-ARITH_HSV]= m_ChuteSensFloat[msg2/MAX_VIEW_UNIT_USE][unitIndex]*10+0.5;
                    }
                }
            }else{
                if(groupInfo.nUnitGupSplice[indexInGroup] == 2){
                    struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][1].hsv[struGsh.nArithIndex-ARITH_HSV] = m_ChuteSensFloat[struGsh.nView%2][unitIndex]*10+0.5;
                }else{
                    struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].hsv[struGsh.nArithIndex-ARITH_HSV] = m_ChuteSensFloat[struGsh.nView%2][unitIndex]*10+0.5;
                }
            }
        }else{
            switch (msg2) {
            case 50:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv[struGsh.nArithIndex-ARITH_HSV].nHsvSensLowH = m_HsvSensLowH*10+0.5;
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv[struGsh.nArithIndex-ARITH_HSV].nHsvSensUpH = m_HsvSensUpH*10+0.5;

                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv[struGsh.nArithIndex-ARITH_HSV].struResGreyColor.nSensUp2 = (int)(m_diffResUpFloat/100.0*1023.0);
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv[struGsh.nArithIndex-ARITH_HSV].struResGreyColor.nSensLow2 = (int)(m_diffResLowFloat/100.0*1023.0);
                break;
            default:
                break;
            }
        }
        updateSizeState();
        updateHsvEdit();
        updateHsvValue();
        setAndSendParams();
    }
}

void HsvSenseSetWidget::showPage(bool bshow)
{
    if(bshow){
        updateGroupLabelShow();
        updateChuteState();
        updateValue();
        updateModeBtnState();
        updateSizeState();
        updateHsvEdit();
        updateHsvValue();

        QStringList userNumList;
        userNumList<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8";
        ui->comboBox_2->clear();
        ui->comboBox_2->addItems(userNumList);
        ui->comboBox_2->setCurrentIndex(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv[struGsh.nArithIndex-ARITH_HSV].nUserArithNum);

        QStringList list;
        list<<g_myLan().ids_delay_select+"1"<<g_myLan().ids_delay_select+"2";
        ui->comboBox_delay->clear();
        ui->comboBox_delay->addItems(list);
        ui->comboBox_delay->setCurrentIndex(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv[struGsh.nArithIndex-ARITH_HSV].delayType);

        ui->extendBtn->setChecked(false);
        ui->reserveArith->setVisible(false);
    }
}

//void HsvSenseSetWidget::receiveMsg(int msg1, int msg2, int msg3)
//{
//    if(msg1 == MSG_DATA_CHANGED){
//        if(msg2 >= HueLow && msg2 <= ValUp)
//        {
//            updateChuteState();
//        }
//        updateSizeState();
//        updateHsvEdit();
//        setAndSendParams();
//    }
//}

void HsvSenseSetWidget::retranslateUiWidget()
{
    ui->label_h->setText(g_myLan().hsv_h);
    ui->label_s->setText(g_myLan().hsv_s);
    ui->label_v->setText(g_myLan().hsv_v);
    ui->label_low->setText(g_myLan().ids_lowlimit);
    ui->label_upper->setText(g_myLan().ids_uplimit);

    ui->groupBox_5->setTitle(g_myLan().basic_set);
    ui->label_18->setText(g_myLan().ids_color);
    ui->standard_titlelbl->setText(g_myLan().standard);
    ui->label_front->setText(g_myLan().front);
    ui->label_rear->setText(g_myLan().rear);
    ui->lightBtn->setText(g_myLan().ids_selectBright);
    ui->darkBtn->setText(g_myLan().ids_selectDark);

    ui->sizeGroupbox->setTitle(g_myLan().ids_size);
    ui->label_20->setText(g_myLan().ids_size);
    ui->label_19->setText(g_myLan().ids_purity);
    ui->label->setText(g_myLan().ids_delay_select);

    ui->reserveArith->setTitle(g_myLan().ids_reserve);
    ui->label_13->setText(g_myLan().ids_gray);
    ui->label_14->setText(g_myLan().ids_diffMColor);
    ui->label_15->setText(g_myLan().impurity_name);
}

void HsvSenseSetWidget::updateValue()
{
    QByteArray titleArr(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv[struGsh.nArithIndex-ARITH_HSV].sName);
    ui->sNamelineEdit->setText(QString::fromUtf8(titleArr));

    ui->sNamelineEdit->setType(textType);
    QStringList hsvColor;
    hsvColor<<g_myLan().hsv_h;
    hsvColor<<g_myLan().hsv_s;
    hsvColor<<g_myLan().hsv_v;
    ui->comboBox->clear();
    ui->comboBox->addItems(hsvColor);
    ui->comboBox->setCurrentIndex(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv[struGsh.nArithIndex-ARITH_HSV].nHsvColor);

    QStringList grayColor,diffColor;

    //switch(struCnfg.struLayerInfo[struGsh.nLayer].nViewSensorType[struGsh.nView])
    switch(struCnfp.nViewSensorType[0][struGsh.nView])
    {
    case SENSOR_D2:
        grayColor<<g_myLan().ids_red<<g_myLan().ids_green<<g_myLan().ids_blue<<g_myLan().ids_black_white<<g_myLan().ids_gray<<g_myLan().ids_n;
        diffColor<<g_myLan().red_green<<g_myLan().red_blue<<g_myLan().green_blue<<g_myLan().red_inf<<g_myLan().green_inf<<g_myLan().blue_inf;
        break;
    case SENSOR_T1:
    case SENSOR_T1_850:
    case SENSOR_C1:
        grayColor<<g_myLan().ids_red<<g_myLan().ids_green<<g_myLan().ids_blue<<g_myLan().ids_black_white<<g_myLan().ids_gray;
        diffColor<<g_myLan().red_green<<g_myLan().red_blue<<g_myLan().green_blue;
        break;
    default:
        grayColor<<g_myLan().ids_red<<g_myLan().ids_green<<g_myLan().ids_blue<<g_myLan().ids_black_white<<g_myLan().ids_gray;
        diffColor<<g_myLan().red_green<<g_myLan().red_blue<<g_myLan().green_blue;
        break;
    }
    ui->comboBox_diff->clear();//保留算法
    ui->comboBox_gray->clear();
    ui->comboBox_gray->addItems(grayColor);
    ui->comboBox_diff->addItems(diffColor);

    stu_hsv &hsv_arith = struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv[struGsh.nArithIndex-ARITH_HSV];
    if(hsv_arith.struResGreyColor.nColor1 <= BACKGROUND_COLOR_GREY ) //灰度没有黄背景
    {
        ui->comboBox_gray->setCurrentIndex(hsv_arith.struResGreyColor.nColor1);
    }
    else
    {
        ui->comboBox_gray->setCurrentIndex(BACKGROUND_COLOR_NIR-1);
    }

    ui->comboBox_diff->setCurrentIndex(hsv_arith.struResGreyColor.nResDiffColor);
    ui->colorResUp->setText(IntToQString(hsv_arith.struResGreyColor.nSensUp1));
    ui->colorResLow->setText(IntToQString(hsv_arith.struResGreyColor.nSensLow1));
    ui->colorResUp->setMinMax(this,0,ARITH_GREY_MAX_SENS,51,&hsv_arith.struResGreyColor.nSensUp1);
    ui->colorResLow->setMinMax(this,0,ARITH_GREY_MAX_SENS,51,&hsv_arith.struResGreyColor.nSensLow1);
    m_diffResUpFloat = (float)hsv_arith.struResGreyColor.nSensUp2/1023.0*100.0;
    m_diffResLowFloat = (float)hsv_arith.struResGreyColor.nSensLow2/1023.0*100.0;
    ui->diffResLow->setText(FloatToPercentQString(m_diffResLowFloat));
    ui->diffResUp->setText(FloatToPercentQString(m_diffResUpFloat));
    ui->diffResUp->setFloatMinMax(this,0,100.0,50,&m_diffResUpFloat,false);
    ui->diffResLow->setFloatMinMax(this,0,100.0,50,&m_diffResLowFloat,false);
}

void HsvSenseSetWidget::updateHsvEdit()
{
    int layer = struGsh.nLayer;
    int view = struGsh.nView;
    int group = struGsh.nMatGroup;
    stu_hsv &hsv = struCnfp.struGroupIdentify[layer][view][group].struHsv[struGsh.nArithIndex-ARITH_HSV];
    ui->hsvCircleWidget->setHsv(hsv.nHsvSensLowH,hsv.nHsvSensLowS,hsv.nHsvSensLowV,hsv.nHsvSensUpH,hsv.nHsvSensUpS,hsv.nHsvSensUpV);

//    ui->HLow->setMinMax(this,0,359,0,&hsv.nHsvSensLowH);
//    ui->HUpper->setMinMax(this,0,359,0,&hsv.nHsvSensUpH);
    ui->HLow->setFloatMinMax(this,0,359.9,50,&m_HsvSensLowH,percentFloatType);
    ui->HUpper->setFloatMinMax(this,0,359.9,50,&m_HsvSensUpH,percentFloatType);

    ui->SLow->setMinMax(this,0,hsv.nHsvSensUpS-1,51,&hsv.nHsvSensLowS);
    ui->SUpper->setMinMax(this,hsv.nHsvSensLowS+1,255,51,&hsv.nHsvSensUpS);

    ui->VLow->setMinMax(this,0,hsv.nHsvSensUpV-1,51,&hsv.nHsvSensLowV);
    ui->VUpper->setMinMax(this,hsv.nHsvSensLowV+1,255,51,&hsv.nHsvSensUpV);
}

void HsvSenseSetWidget::updateHsvValue()
{
    int layer = struGsh.nLayer;
    int view = struGsh.nView;
    int group = struGsh.nMatGroup;
    stu_hsv hsv = struCnfp.struGroupIdentify[layer][view][group].struHsv[struGsh.nArithIndex-ARITH_HSV];
    m_HsvSensLowH = hsv.nHsvSensLowH /10.0;
    m_HsvSensUpH  = hsv.nHsvSensUpH /10.0;

    ui->HLow->setText(FloatToPercentQString(m_HsvSensLowH));
    ui->HUpper->setText(FloatToPercentQString(m_HsvSensUpH));
    ui->SLow->setText(QString::number(hsv.nHsvSensLowS));
    ui->SUpper->setText(QString::number(hsv.nHsvSensUpS));
    ui->VLow->setText(QString::number(hsv.nHsvSensLowV));
    ui->VUpper->setText(QString::number(hsv.nHsvSensUpV));
}

void HsvSenseSetWidget::updateChuteState()
{
#if 1
    stru_iden_group_info groupInfo;
    static stu_group_identify groupIde;
    memcpy(&groupInfo,&struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].stuIdenGupInfo[struGsh.nMatGroup],sizeof(groupInfo));
    memcpy(&groupIde,&struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup],sizeof(groupIde));

    QByteArray titleArr(groupIde.struHsv[struGsh.nArithIndex-ARITH_HSV].sName);
    ui->sNamelineEdit->setText(QString::fromUtf8(titleArr));

    ui->sNamelineEdit->setType(textType);
    int standardSens;
    int senseMin = 0, senseMax;
    int *pSens1;
    int *pSens2;
    float stand_h,senseMax_h;

    switch(groupIde.struHsv[struGsh.nArithIndex-ARITH_HSV].nHsvColor)
    {
    case 0:
        standardSens = (groupIde.struHsv[struGsh.nArithIndex-ARITH_HSV].nHsvMode == 0) ? groupIde.struHsv[struGsh.nArithIndex-ARITH_HSV].nHsvSensLowH : groupIde.struHsv[struGsh.nArithIndex-ARITH_HSV].nHsvSensUpH;
        stand_h = standardSens /10.0;
        ui->standardlbl->setText(FloatToPercentQString(stand_h));
        senseMax_h = 359.9;
        break;
    case 1:
        standardSens = (groupIde.struHsv[struGsh.nArithIndex-ARITH_HSV].nHsvMode == 0) ? groupIde.struHsv[struGsh.nArithIndex-ARITH_HSV].nHsvSensLowS : groupIde.struHsv[struGsh.nArithIndex-ARITH_HSV].nHsvSensUpS;
        ui->standardlbl->setText(IntToQString(standardSens));
        senseMax = 255;
        break;
    case 2:
        standardSens = (groupIde.struHsv[struGsh.nArithIndex-ARITH_HSV].nHsvMode == 0) ? groupIde.struHsv[struGsh.nArithIndex-ARITH_HSV].nHsvSensLowV : groupIde.struHsv[struGsh.nArithIndex-ARITH_HSV].nHsvSensUpV;
        ui->standardlbl->setText(IntToQString(standardSens));
        senseMax = 255;
        break;
    }

    for(int i = 0;i < MAX_VIEW_UNIT_USE;i++){
        m_pChuteTitle[i]->hide();
        m_pChuteSenseEdit[0][i]->hide();
        m_pChuteSenseEdit[1][i]->hide();
    }

    int tempRepeatCondition = 0;
    if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_SECOND_ALIKE_FIRST){
        tempRepeatCondition = struCnfg.nChuteTotal;
    }else{
        tempRepeatCondition = groupInfo.nGroupUnitTotal;
    }
    int unitIndex = 0;

    for(int i = 0;i < tempRepeatCondition;i++){

        if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_SECOND_ALIKE_FIRST){
            unitIndex = i;//算法配置不分次
        }else{
            unitIndex = groupInfo.nUnitGupId[i];//视链上第几个相机
        }

        if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT){//前后视相同
            ui->label_front->show();
            ui->label_rear->show();

            m_pChuteTitle[unitIndex]->show();
            m_pChuteSenseEdit[0][unitIndex]->show();
            m_pChuteSenseEdit[1][unitIndex]->show();

            if(groupIde.struHsv[struGsh.nArithIndex-ARITH_HSV].nHsvColor == 0){
                m_ChuteSensFloat[0][unitIndex] = struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].hsv[struGsh.nArithIndex-ARITH_HSV] /10.0;
                m_ChuteSensFloat[1][unitIndex] = struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][unitIndex][0].hsv[struGsh.nArithIndex-ARITH_HSV] /10.0;
                m_pChuteSenseEdit[0][unitIndex]->setFloatMinMax(this,0-stand_h,359.9-stand_h,unitIndex,&m_ChuteSensFloat[0][unitIndex],false);
                m_pChuteSenseEdit[1][unitIndex]->setFloatMinMax(this,0-stand_h,359.9-stand_h,unitIndex+MAX_VIEW_UNIT_USE,&m_ChuteSensFloat[1][unitIndex],false);
                m_pChuteSenseEdit[0][unitIndex]->setText(FloatToPercentQString(m_ChuteSensFloat[0][unitIndex]));
                m_pChuteSenseEdit[1][unitIndex]->setText(FloatToPercentQString(m_ChuteSensFloat[1][unitIndex]));
            }else{
                pSens1 = &struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].hsv[struGsh.nArithIndex-ARITH_HSV];
                pSens2 = &struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][unitIndex][0].hsv[struGsh.nArithIndex-ARITH_HSV];
                m_pChuteSenseEdit[0][unitIndex]->setMinMax(this,senseMin-standardSens,senseMax-standardSens,-1,pSens1);
                m_pChuteSenseEdit[1][unitIndex]->setMinMax(this,senseMin-standardSens,senseMax-standardSens,-1,pSens2);
            }
        }else{
            if(struGsh.nView%2){
                ui->label_front->hide();
                ui->label_rear->show();
            }else{
                ui->label_front->show();
                ui->label_rear->hide();
            }
            m_pChuteTitle[unitIndex]->show();
            m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->show();
            if(groupIde.struHsv[struGsh.nArithIndex-ARITH_HSV].nHsvColor == 0){
                m_ChuteSensFloat[0][unitIndex] = struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].hsv[struGsh.nArithIndex-ARITH_HSV] /10.0;
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setFloatMinMax(this,0-stand_h,359.9-stand_h,unitIndex,&m_ChuteSensFloat[0][unitIndex],false);
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setText(FloatToPercentQString(m_ChuteSensFloat[0][unitIndex]));
            }else{
                pSens1 = &struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView%2][unitIndex][0].hsv[struGsh.nArithIndex-ARITH_HSV];
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setMinMax(this,senseMin-standardSens,senseMax-standardSens,-1,pSens1);
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setText(IntToQString(*pSens1));
            }
        }
    }
#endif
}

void HsvSenseSetWidget::setViewVisibleByArith()
{

}

void HsvSenseSetWidget::onModeChange(int mode)
{
    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv[struGsh.nArithIndex-ARITH_HSV].nHsvMode = mode;
}

void HsvSenseSetWidget::updateSizeState()
{
    int layer = struGsh.nLayer;
    int view = struGsh.nView;
    int group = struGsh.nMatGroup;
    stu_hsv &hsv = struCnfp.struGroupIdentify[layer][view][group].struHsv[struGsh.nArithIndex-ARITH_HSV];
    int maxPercent = 0;
    maxPercent = hsv.nRow*hsv.nRow;

    m_rowInt = hsv.nRow;
    m_percentInt = hsv.nPercent;

    maxPercent = hsv.nRow*hsv.nRow;
    if(hsv.nPercent > maxPercent)
    {
        hsv.nPercent = maxPercent;
    }
    ui->rowEdit->setText(QString::number(hsv.nRow));
    ui->rowEdit->setMinMax(this,1,16,40,&hsv.nRow);
    ui->percentEdit->setText(QString::number(hsv.nPercent));
    ui->percentEdit->setMinMax(this,1,maxPercent,41,&hsv.nPercent);
}

void HsvSenseSetWidget::updateModeBtnState()
{
    if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv[struGsh.nArithIndex-ARITH_HSV].nHsvMode == 0){
        m_pModeBtnGroup->button(0)->setChecked(true);
    }else{
        m_pModeBtnGroup->button(1)->setChecked(true);
    }
}



void HsvSenseSetWidget::modeBtnClicked(bool checked)
{
    if(checked)
        onModeChange(m_pModeBtnGroup->id((QPushButton*)sender()));
    updateChuteState();
    setAndSendParams();
}

void HsvSenseSetWidget::on_comboBox_activated(int index)
{
    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv[struGsh.nArithIndex-ARITH_HSV].nHsvColor = index;
    updateChuteState();
    setAndSendParams();
}

void HsvSenseSetWidget::sNameChanged()
{
    memset(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv[struGsh.nArithIndex-ARITH_HSV].sName,0,sizeof(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv[struGsh.nArithIndex-ARITH_HSV].sName));

    QString name = ui->sNamelineEdit->text();
    QByteArray namearr = name.toUtf8();
    const char *strName = namearr.data();

    memcpy(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv[struGsh.nArithIndex-ARITH_HSV].sName,strName,strlen(strName));
}

void HsvSenseSetWidget::hsvColorChanged()
{
    int layer = struGsh.nLayer;
    int view = struGsh.nView;
    int group = struGsh.nMatGroup;
    stu_hsv &hsv = struCnfp.struGroupIdentify[layer][view][group].struHsv[struGsh.nArithIndex-ARITH_HSV];
    ui->hsvCircleWidget->getHsv(hsv.nHsvSensLowH,hsv.nHsvSensLowS,hsv.nHsvSensLowV,hsv.nHsvSensUpH,hsv.nHsvSensUpS,hsv.nHsvSensUpV);
    updateHsvValue();
    setAndSendParams();
}

void HsvSenseSetWidget::on_comboBox_2_activated(int index)
{
    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv[struGsh.nArithIndex-ARITH_HSV].nUserArithNum = index;
}

void HsvSenseSetWidget::on_comboBox_ratio_activated(int index)
{
    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv[struGsh.nArithIndex-ARITH_HSV].nRadioMode = index;
    updateSizeState();
}

void HsvSenseSetWidget::on_comboBox_delay_activated(int index)
{
    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv[struGsh.nArithIndex-ARITH_HSV].delayType = index;
    setAndSendParams();
}

void HsvSenseSetWidget::on_comboBox_gray_activated(int index)
{
    if(index <= BACKGROUND_COLOR_GREY ) //灰度没有黄背景
    {
       struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv[struGsh.nArithIndex-ARITH_HSV].struResGreyColor.nColor1 = index;
    }
    else
    {
        struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv[struGsh.nArithIndex-ARITH_HSV].struResGreyColor.nColor1 = BACKGROUND_COLOR_NIR;
    }
    setAndSendParams();
}

void HsvSenseSetWidget::on_comboBox_diff_activated(int index)
{
    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv[struGsh.nArithIndex-ARITH_HSV].struResGreyColor.nResDiffColor = index;

    setAndSendParams();
}

void HsvSenseSetWidget::on_extendBtn_clicked(bool checked)
{
    ui->reserveArith->setVisible(checked);
}
