#include "graysensesetwidget.h"
#include "ui_graysensesetwidget.h"
#include "globalflow.h"
const int senseMax[5] = {ARITH_GREY_MAX_SENS,ARITH_GREY_MAX_SENS,ARITH_COLOR_MAX_SENS,ARITH_COLOR_MAX_SENS,ARITH_GREY_MAX_SENS};
const int senseMin[5] = {0,0,0,0,0};
GraySenseSetWidget::GraySenseSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::GraySenseSetWidget)
{
    ui->setupUi(this);
    m_pChuteTitle[0] = ui->label_1_1;
    m_pChuteTitle[1] = ui->label_1_2;
    m_pChuteTitle[2] = ui->label_1_3;
    m_pChuteTitle[3] = ui->label_1_4;
    m_pChuteTitle[4] = ui->label_1_5;
    m_pChuteTitle[5] = ui->label_1_6;
    m_pChuteTitle[6] = ui->label_1_7;
    m_pChuteTitle[7] = ui->label_1_8;
    m_pChuteTitle[8] = ui->label_1_9;
    m_pChuteTitle[9] = ui->label_1_10;
    m_pChuteTitle[10] = ui->label_1_11;
    m_pChuteTitle[11] = ui->label_1_12;
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
    for(int i = 0;i < 2;i++){
        m_pModeBtnGroup->button(i)->setCheckable(true);
        connect(m_pModeBtnGroup->button(i),SIGNAL(clicked(bool)),this,SLOT(modeBtnClicked(bool)));
    }

    ui->extendBtn->setCheckable(true);
    connect(ui->sNamelineEdit,SIGNAL(valueSetSignal()),this,SLOT(sNameChanged()));

    ui->edgeBtn->hide();
    ui->infectBtn->hide();
    ui->label_13->hide();
    ui->SensUpdateCheckBox->hide();
}

GraySenseSetWidget::~GraySenseSetWidget()
{
    delete ui;
}

void GraySenseSetWidget::showPage(bool bshow)
{
    if(bshow){
        updateGroupLabelShow();

        updateChuteState();
        updateValue();
        updateEdgeBtn();
        updateinfectBtn();
        updateModeBtnState();

        //进入页面默认隐藏不常用配置
        ui->extendBtn->setChecked(false);
        ui->reserveArith->setVisible(false);
        ui->groupBox_2->setVisible(false);

        setViewVisibleByArith();
        updatePercentViewState();
        updateLineDefectState();
        updateSizeState();

        if(struCnfg.stuProfileIndex[struCnfg.nProfile].nProfileLock == 1)
        {
            ui->comboBox->setEnabled(false);
            m_pModeBtnGroup->button(0)->hide();
            m_pModeBtnGroup->button(1)->hide();

            ui->extendBtn->hide();   //扩展按钮
        }
        else
        {
            ui->comboBox->setEnabled(true);

            m_pModeBtnGroup->button(0)->show();
            m_pModeBtnGroup->button(1)->show();

            ui->extendBtn->show();
        }

        QStringList list;
        list<<g_myLan().ids_delay_select+"1"<<g_myLan().ids_delay_select+"2";
        ui->comboBox_delay->clear();
        ui->comboBox_delay->addItems(list);
        ui->comboBox_delay->setCurrentIndex(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].delayType);
    }
}

void GraySenseSetWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    const stru_iden_group_info &groupInfo = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].stuIdenGupInfo[struGsh.nMatGroup];

    if(msg1 == MSG_DATA_CHANGED){
        if(msg2 < MAX_VIEW_UNIT_USE*2){

        }else{
            switch (msg2) {
            case 40:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].nRow = m_rowInt;
                if(m_percentInt > m_rowInt*m_rowInt)
                {
                    m_percentInt = m_rowInt*m_rowInt;
                    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].nPercent = m_percentInt;
                }
                ui->percentEdit->setText(QString::number(m_percentInt));
                ui->percentEdit->setMinMax(this,1,m_rowInt*m_rowInt,41,&m_percentInt);
                break;
            case 41:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].nPercent = m_percentInt;
                break;
            case 42:
                if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].fPropotionSensLow >
                        struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].fPropotionSensUp)
                {
                    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].fPropotionSensLow =
                            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].fPropotionSensUp;
                }
                ui->PropotionLowEdit->setText(FloatToPercentQString(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].fPropotionSensLow));
                ui->PropotionLowEdit->setFloatMinMax(this,0,struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].fPropotionSensUp,43,
                        &struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].fPropotionSensLow,false);
                break;
            case 51:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].struResGreyColor.nSensUp2 = (int)(m_diffResUpFloat/100.0*1023.0);
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].struResGreyColor.nSensLow2 = (int)(m_diffResLowFloat/100.0*1023.0);
                break;
            case 52:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].nPropResSens = m_percentResInt;
                break;
            case 55 :
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].nEdge = m_arithEdge;
                struCnfp.nGroupArithEdgeEnable[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup][struGsh.nArithIndex] = m_arithEdge;
                break;
            default:
                break;
            }
        }
        setAndSendParams();
    }
}

void GraySenseSetWidget::retranslateUiWidget()
{
    ui->label->setText(g_myLan().impurity_name);
    ui->label_8->setText(g_myLan().ids_color);
    ui->groupBox_4->setTitle(g_myLan().basic_set);
    ui->standard_titlelbl->setText(g_myLan().standard);
    ui->label_front->setText(g_myLan().front);
    ui->label_rear->setText(g_myLan().rear);
    ui->lightBtn->setText(g_myLan().ids_selectBright);
    ui->darkBtn->setText(g_myLan().ids_selectDark);
    ui->percentCheckBox->setText(g_myLan().arith_scale);
    ui->lineDefectCheckbox->setText(g_myLan().arith_line_defect);

    ui->lineDefectGroupbox->setTitle(g_myLan().arith_line_defect);
    //area
    ui->areaGroupbox->setTitle(g_myLan().ids_area);
    ui->areaUpp->setText(g_myLan().ids_uplimit);
    ui->areaLow->setText(g_myLan().ids_lowlimit);

    ui->lengthGroupbox->setTitle(g_myLan().ids_selectLong);
    ui->lengthUpp->setText(g_myLan().ids_uplimit);
    ui->lengthLow->setText(g_myLan().ids_lowlimit);

    ui->sizeGroupbox->setTitle(g_myLan().ids_size);
    ui->label_2->setText(g_myLan().ids_size);
    ui->label_3->setText(g_myLan().ids_purity);

    ui->percentGroupbox->setTitle(g_myLan().ids_purity);
    ui->areaUpp_2->setText(g_myLan().ids_uplimit);
    ui->areaLow_2->setText(g_myLan().ids_lowlimit);
    ui->edgeBtn->setText(g_myLan().edge_cut);
    ui->infectBtn->setText(g_myLan().infect_arithmetic);
    ui->label_13->setText(g_myLan().ids_AdjustSens);

    ui->reserveArith->setTitle(g_myLan().ids_reserve);

    ui->label_10->setText(g_myLan().ids_gray);
    ui->label_14->setText(g_myLan().arith_propotion);
    ui->label_9->setText(g_myLan().ids_diffMColor);
    ui->brightlimitlbl->setText(g_myLan().light_limit);

    ui->edgeGroupbox->setTitle(g_myLan().edge_cut);
    ui->edgeLbl->setText(g_myLan().edge_cut);
    ui->PropotionParaLowLbl->setText(g_myLan().params_limit);

    ui->label_4->setText(g_myLan().ids_delay_select);
}

void GraySenseSetWidget::updateValue()//更新保留算法相关的数据
{
    QStringList grayColor,diffColor;

    //switch(struCnfg.struLayerInfo[struGsh.nLayer].nViewSensorType[struGsh.nView])
    switch(struCnfp.nViewSensorType[0][struGsh.nView])
    {
    case SENSOR_C1:
        grayColor<<g_myLan().ids_red<<g_myLan().ids_green<<g_myLan().ids_blue<<g_myLan().ids_black_white<<g_myLan().ids_gray;
        diffColor<<g_myLan().red_green<<g_myLan().red_blue<<g_myLan().green_blue;
        break;
    case SENSOR_D2:
        grayColor<<g_myLan().ids_red<<g_myLan().ids_green<<g_myLan().ids_blue<<g_myLan().ids_black_white<<g_myLan().ids_gray<<g_myLan().ids_n;
        diffColor<<g_myLan().red_green<<g_myLan().red_blue<<g_myLan().green_blue<<g_myLan().red_inf<<g_myLan().green_inf<<g_myLan().blue_inf;
        break;
    case SENSOR_T1:
    case SENSOR_T1_850:
        grayColor<<g_myLan().ids_red<<g_myLan().ids_green<<g_myLan().ids_blue<<g_myLan().ids_black_white<<g_myLan().ids_gray;
        diffColor<<g_myLan().red_green<<g_myLan().red_blue<<g_myLan().green_blue;;
        break;
    default:
        grayColor<<g_myLan().ids_red<<g_myLan().ids_green<<g_myLan().ids_blue<<g_myLan().ids_black_white<<g_myLan().ids_gray;
        diffColor<<g_myLan().red_green<<g_myLan().red_blue
                <<g_myLan().green_blue;
        break;
    }

//    grayColor<<g_myLan().ids_red<<g_myLan().ids_green<<g_myLan().ids_blue;
//    diffColor<<g_myLan().ids_green<<g_myLan().ids_blue<<g_myLan().green_blue;

    ui->comboBox->clear();
    ui->comboBox_diff->clear();//保留算法
    ui->comboBox_gray->clear();
    ui->comboBox_gray->addItems(grayColor);
    ui->comboBox_diff->addItems(diffColor);

    stu_color_arith &color_arith = struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex];
    switch(struGsh.nArithIndex)
    {
    case ARITH_GREY_A:
    case ARITH_GREY_B:
    case ARITH_GREY_C:
    case ARITH_GREY_D:
        ui->comboBox->addItems(grayColor);
        if(color_arith.nMain <= BACKGROUND_COLOR_GREY ) //灰度没有黄背景
        {
            ui->comboBox->setCurrentIndex(color_arith.nMain);
        }
        else
        {
            ui->comboBox->setCurrentIndex(BACKGROUND_COLOR_NIR-1);
        }

        ui->darkBtn->show();
        ui->lightBtn->show();
        break;
    case ARITH_DISCOLOR_A:
    case ARITH_DISCOLOR_B:
    case ARITH_CROSS: //色差C
        ui->comboBox->addItems(diffColor);
        ui->comboBox->setCurrentIndex(color_arith.nDiffColor);
        ui->darkBtn->show();
        ui->lightBtn->show();
        break;
    }

    if(color_arith.struResGreyColor.nColor1 <= BACKGROUND_COLOR_GREY ) //灰度没有黄背景
    {
        ui->comboBox_gray->setCurrentIndex(color_arith.struResGreyColor.nColor1);
    }
    else
    {
        ui->comboBox_gray->setCurrentIndex(BACKGROUND_COLOR_NIR-1);
    }

    ui->comboBox_diff->setCurrentIndex(color_arith.struResGreyColor.nResDiffColor);
    ui->edgeBtn->setChecked((bool)color_arith.nEdgeType);
    ui->infectBtn->setChecked((bool)(color_arith.nImpureType));
    ui->SensUpdateCheckBox->setChecked(color_arith.nSensUpdate == 1);
    m_pModeBtnGroup->button(color_arith.nMode)->setChecked(true);



    ui->colorResUp->setText(IntToQString(color_arith.struResGreyColor.nSensUp1));
    ui->colorResLow->setText(IntToQString(color_arith.struResGreyColor.nSensLow1));

    m_diffResUpFloat = (float)color_arith.struResGreyColor.nSensUp2/1023.0*100.0;
    m_diffResLowFloat = (float)color_arith.struResGreyColor.nSensLow2/1023.0*100.0;
    ui->diffResLow->setText(FloatToPercentQString(m_diffResLowFloat));
    ui->diffResUp->setText(FloatToPercentQString(m_diffResUpFloat));

    ui->colorResUp->setMinMax(this,0,ARITH_GREY_MAX_SENS,50,&color_arith.struResGreyColor.nSensUp1);
    ui->colorResLow->setMinMax(this,0,ARITH_GREY_MAX_SENS,50,&color_arith.struResGreyColor.nSensLow1);
    ui->diffResUp->setFloatMinMax(this,0,100.0,51,&m_diffResUpFloat,false);
    ui->diffResLow->setFloatMinMax(this,0,100.0,51,&m_diffResLowFloat,false);
    ui->percentResEdit->setText(IntToQString(color_arith.nPropResSens));

    m_arithEdge = color_arith.nEdge;
    ui->edgeCutEdit->setText(IntToQString(m_arithEdge));
    ui->edgeCutEdit->setMinMax(this,0,struCnfg.nEdgeCutMaxValue,55,&m_arithEdge);
    struCnfp.nGroupArithEdgeEnable[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup][struGsh.nArithIndex] = m_arithEdge;


}

void GraySenseSetWidget::updateChuteState()//更新料槽的配置精度值
{
    stru_iden_group_info groupInfo;
    static stu_group_identify groupIde;
    memcpy(&groupInfo,&struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].stuIdenGupInfo[struGsh.nMatGroup],sizeof(groupInfo));
    memcpy(&groupIde,&struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup],sizeof(groupIde));

    QByteArray titleArr(groupIde.struGreyColor[struGsh.nArithIndex].sName);
    ui->sNamelineEdit->setText(QString::fromUtf8(titleArr));

    ui->sNamelineEdit->setType(textType);
    int standardSens;
    if(groupIde.struGreyColor[struGsh.nArithIndex].nMode){
        standardSens = groupIde.struGreyColor[struGsh.nArithIndex].nSensUp;

    }else{
        standardSens = groupIde.struGreyColor[struGsh.nArithIndex].nSensLow;
    }
    ui->standardlbl->setText(IntToQString(standardSens));

    for(int i = 0;i < MAX_VIEW_UNIT_USE;i++){
        m_pChuteTitle[i]->hide();
        m_pChuteSenseEdit[0][i]->hide();
        m_pChuteSenseEdit[1][i]->hide();
    }
    int stand = standardSens;

    int tempRepeatCondition = 0;
    if(2 == struGsh.nView || 3 == struGsh.nView || 6 == struGsh.nView || 7 == struGsh.nView)
    {
        if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_SECOND_ALIKE_FIRST){
            if(struCnfg.bCrossMode2 == 1)
            {
                tempRepeatCondition = struCnfg.nChuteTotal+1;
            }
            else
            {
                tempRepeatCondition = struCnfg.nChuteTotal-1;
            }

        }else{
             tempRepeatCondition = groupInfo.nGroupUnitTotal;
        }
    }
    else
    {
        if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_SECOND_ALIKE_FIRST){
            tempRepeatCondition = struCnfg.nChuteTotal;
        }else{
            tempRepeatCondition = groupInfo.nGroupUnitTotal;
        }
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

            if(groupInfo.nUnitGupSplice[i] == 2){
                m_pChuteSenseEdit[0][unitIndex]->setMinMax(this,senseMin[struGsh.nArithIndex]-stand,senseMax[struGsh.nArithIndex]-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].greyColor[struGsh.nArithIndex]);
                m_pChuteSenseEdit[1][unitIndex]->setMinMax(this,senseMin[struGsh.nArithIndex]-stand,senseMax[struGsh.nArithIndex]-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][unitIndex][0].greyColor[struGsh.nArithIndex]);
                m_pChuteSenseEdit[0][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].greyColor[struGsh.nArithIndex]));
                m_pChuteSenseEdit[1][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][unitIndex][0].greyColor[struGsh.nArithIndex]));
            }else{
                m_pChuteSenseEdit[0][unitIndex]->setMinMax(this,senseMin[struGsh.nArithIndex]-stand,senseMax[struGsh.nArithIndex]-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].greyColor[struGsh.nArithIndex]);
                m_pChuteSenseEdit[1][unitIndex]->setMinMax(this,senseMin[struGsh.nArithIndex]-stand,senseMax[struGsh.nArithIndex]-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][unitIndex][0].greyColor[struGsh.nArithIndex]);
                m_pChuteSenseEdit[0][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].greyColor[struGsh.nArithIndex]));
                m_pChuteSenseEdit[1][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][unitIndex][0].greyColor[struGsh.nArithIndex]));
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
            if(groupInfo.nUnitGupSplice[i] == 2){
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setMinMax(this,senseMin[struGsh.nArithIndex]-stand,senseMax[struGsh.nArithIndex]-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].greyColor[struGsh.nArithIndex]);
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].greyColor[struGsh.nArithIndex]));
            }else{
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setMinMax(this,senseMin[struGsh.nArithIndex]-stand,senseMax[struGsh.nArithIndex]-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].greyColor[struGsh.nArithIndex]);
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].greyColor[struGsh.nArithIndex]));
            }

        }
    }
}

void GraySenseSetWidget::updateEdgeBtn()
{
    if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].nEdgeType){
        ui->edgeBtn->setText(g_myLan().edge_cut_2);
    }else{
        ui->edgeBtn->setText(g_myLan().edge_cut_1);
    }
}

void GraySenseSetWidget::updateinfectBtn()
{
    if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].nImpureType){
        ui->infectBtn->setText(g_myLan().infect_arithmetic_2);
    }else{
        ui->infectBtn->setText(g_myLan().infect_arithmetic_1);
    }
}

void GraySenseSetWidget::setViewVisibleByArith()//更新保留算法控件的显示和隐藏
{
//    switch(struCnfg.struLayerInfo[struGsh.nLayer].nViewSensorType[struGsh.nView]){
//    case SENSOR_T1://可见
//    case SENSOR_T1_850://近红外
//    case SENSOR_D2:     //D2
//        switch (struGsh.nArithIndex) {
//        case ARITH_GREY_A:
//        case ARITH_GREY_B:
//        case ARITH_DISCOLOR_A:
//        case ARITH_DISCOLOR_B:
            ui->percentWidget->hide();

            ui->grayResWidget->show();
            ui->diffResWidget->show();
//            break;
//        case ARITH_GREY_C:
//        case ARITH_GREY_D:
//            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].nResType = 2;
//            ui->percentWidget->show();
//            ui->grayResWidget->hide();
//            ui->diffResWidget->hide();
//        default:
//            break;
//        }
//        break;
//    case SENSOR_C1://红外
//        ui->reserveArith->hide();
//        break;
//    default:
//        break;
//    }
    stu_color_arith color_arith;
    memcpy(&color_arith,&struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex],sizeof(color_arith));

    updatePercentViewState();
}
void GraySenseSetWidget::updateModeBtnState()
{
    if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].nMode == 0){
        m_pModeBtnGroup->button(0)->setChecked(true);
    }else{
        m_pModeBtnGroup->button(1)->setChecked(true);
    }
}
void GraySenseSetWidget::onModeChange(int mode)
{
    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].nMode = mode;
}

void GraySenseSetWidget::updatePercentViewState()//更新比例算法相关控件的显示和隐藏
{
    //只有灰度A有比例
    ui->percentCheckBox->setVisible(struGsh.nArithIndex == ARITH_GREY_A);

    int layer = struGsh.nLayer;
    int view = struGsh.nView;
    int group = struGsh.nMatGroup;
    int index = struGsh.nArithIndex;
    stu_color_arith &color_arith = struCnfp.struGroupIdentify[layer][view][group].struGreyColor[index];
    ui->percentCheckBox->setChecked(color_arith.nPropotionEnable != 0);
    if(color_arith.nPropotionEnable){
        ui->percentGroupbox->show();
        ui->percentCheckBox->setChecked(true);
    }else{
        ui->percentGroupbox->hide();
        ui->percentCheckBox->setChecked(false);
    }
    ui->PropotionUpEdit->setText(FloatToPercentQString(color_arith.fPropotionSensUp));
    ui->PropotionUpEdit->setFloatMinMax(this,0,100,42,&color_arith.fPropotionSensUp,false);
    ui->PropotionLowEdit->setText(FloatToPercentQString(color_arith.fPropotionSensLow));
    ui->PropotionLowEdit->setFloatMinMax(this,0,color_arith.fPropotionSensUp,43,&color_arith.fPropotionSensLow,false);

    ui->PropotionParaLowEdit->setText(IntToQString(color_arith.nPropotionParaSensLow));
    ui->PropotionParaLowEdit->setMinMax(this,1,1023,44,&color_arith.nPropotionParaSensLow);


    //version 1.5
//    ui->percentCheckBox->hide();
//    ui->percentGroupbox->hide();
}

void GraySenseSetWidget::updateLineDefectState()
{
    int layer = struGsh.nLayer;
    int view = struGsh.nView;
    int group = struGsh.nMatGroup;
    int index = struGsh.nArithIndex;
    stu_color_arith &color_arith = struCnfp.struGroupIdentify[layer][view][group].struGreyColor[index];
    ui->lineDefectCheckbox->setChecked(color_arith.nLineDefectEnable);
    if(color_arith.nLineDefectEnable){
        ui->lineDefectGroupbox->setVisible(true);
    }else{
        ui->lineDefectGroupbox->setVisible(false);
    }
     ui->areaUppEdit->setText(IntToQString(color_arith.fnLineDefectAreaUp));
    ui->areaLowEdit->setText(IntToQString(color_arith.fnLineDefectAreaLow));
    ui->lengthUppEdit->setText(IntToQString(color_arith.fnLineDefectLengthUp));
    ui->lengthLowEdit->setText(IntToQString(color_arith.fnLineDefectLengthLow));


    ui->areaUppEdit->setMinMax(this,0,100,42,&color_arith.fnLineDefectAreaUp);
    ui->areaLowEdit->setMinMax(this,0,100,42,&color_arith.fnLineDefectAreaLow);
    ui->lengthUppEdit->setMinMax(this,0,100,42,&color_arith.fnLineDefectLengthUp);
    ui->lengthLowEdit->setMinMax(this,0,100,42,&color_arith.fnLineDefectLengthLow);

    ui->brightlimit_edit->setMinMax(this,0,255,42,&color_arith.lightLimit);
    ui->brightlimit_edit->setText(IntToQString(color_arith.lightLimit));

    //1.5
    ui->lineDefectCheckbox->hide();
    ui->lineDefectGroupbox->hide();
}

void GraySenseSetWidget::updateSizeState()
{
    int layer = struGsh.nLayer;
    int view = struGsh.nView;
    int group = struGsh.nMatGroup;
    int index = struGsh.nArithIndex;
    stu_color_arith &color_arith = struCnfp.struGroupIdentify[layer][view][group].struGreyColor[index];
    int maxPercent = 0;
    maxPercent = color_arith.nRow*color_arith.nRow;
    if(color_arith.nLineDefectEnable || color_arith.nPropotionEnable){
        ui->sizeGroupbox->hide();
    }else{
        ui->sizeGroupbox->show();
    }
    m_rowInt = color_arith.nRow;
    m_percentInt = color_arith.nPercent;
    ui->rowEdit->setText(QString::number(m_rowInt));
    ui->rowEdit->setMinMax(this,1,32,40,&m_rowInt);
    ui->percentEdit->setText(QString::number(m_percentInt));
    ui->percentEdit->setMinMax(this,1,maxPercent,41,&m_percentInt);
}



void GraySenseSetWidget::on_edgeBtn_clicked(bool checked)
{
    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].nEdgeType = (int)checked;
    updateEdgeBtn();
    setAndSendParams();
}

void GraySenseSetWidget::on_infectBtn_clicked(bool checked)
{
    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].nImpureType = (int)checked;
    updateinfectBtn();
    setAndSendParams();
}

void GraySenseSetWidget::on_comboBox_activated(int index)
{
    if(index <= BACKGROUND_COLOR_GREY ) //灰度没有黄背景
    {
        struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].nMain = index;
    }
    else
    {
        struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].nMain = BACKGROUND_COLOR_NIR;
    }

    setAndSendParams();
}

void GraySenseSetWidget::sNameChanged()
{
    memset(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].sName,0,sizeof(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].sName));

    QString name = ui->sNamelineEdit->text();
    QByteArray namearr = name.toUtf8();
    const char *strName = namearr.data();
    memcpy(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].sName,strName,strlen(strName));
}

void GraySenseSetWidget::on_SensUpdateCheckBox_clicked(bool checked)
{
    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].nSensUpdate = (int)checked;
    setAndSendParams();
}

void GraySenseSetWidget::on_comboBox_diff_activated(int index)
{
    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].struResGreyColor.nResDiffColor = index;

    setAndSendParams();
}

void GraySenseSetWidget::on_comboBox_gray_activated(int index)
{
    if(index <= BACKGROUND_COLOR_GREY ) //灰度没有黄背景
    {
       struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].struResGreyColor.nColor1 = index;
    }
    else
    {
        struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].struResGreyColor.nColor1 = BACKGROUND_COLOR_NIR;
    }
    setAndSendParams();
}

void GraySenseSetWidget::modeBtnClicked(bool check)//算法的选亮选暗切换
{
    if(check)
        onModeChange(m_pModeBtnGroup->id((QPushButton*)sender()));
    updateChuteState();
    setAndSendParams();
}

void GraySenseSetWidget::on_percentCheckBox_clicked(bool checked)
{
    if(checked){
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].nPropotionEnable = 1;
    }else{
        struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].nPropotionEnable = 0;
    }
    updatePercentViewState();
    updateSizeState();
    setAndSendParams();
}

void GraySenseSetWidget::on_extendBtn_clicked(bool checked)
{
    ui->groupBox_2->setVisible((struGsh.nArithIndex == ARITH_DISCOLOR_A)||(struGsh.nArithIndex == ARITH_DISCOLOR_B));
    //switch(struCnfg.struLayerInfo[struGsh.nLayer].nViewSensorType[struGsh.nView]){
    switch(struCnfp.nViewSensorType[0][struGsh.nView])
    {
    case SENSOR_T1:
    case SENSOR_T1_850:
    case SENSOR_D2:
        ui->reserveArith->setVisible(checked);
        break;
    case SENSOR_C1:
        ui->reserveArith->setVisible(false);
        break;
    default:
        ui->reserveArith->setVisible(checked);
        break;
    }
}

void GraySenseSetWidget::on_lineDefectCheckbox_clicked(bool checked)
{
    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].nLineDefectEnable = (int)checked;
    updateLineDefectState();
    updateSizeState();
    setAndSendParams();
}

void GraySenseSetWidget::on_comboBox_delay_activated(int index)
{
    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].delayType = index;
    setAndSendParams();
}
