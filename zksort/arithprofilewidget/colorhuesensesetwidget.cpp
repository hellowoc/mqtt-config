#include "colorhuesensesetwidget.h"
#include "ui_colorhuesensesetwidget.h"
#include "globalflow.h"
const int senseMax[5] = {ARITH_GREY_MAX_SENS,ARITH_GREY_MAX_SENS,ARITH_COLOR_MAX_SENS,ARITH_COLOR_MAX_SENS,ARITH_GREY_MAX_SENS};
const int senseMin[5] = {0,0,0,0,0};

ColorHueSenseSetWidget::ColorHueSenseSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::ColorHueSenseSetWidget)
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
    m_pResBtnGroup = new QButtonGroup(this);
    m_pResBtnGroup->addButton(ui->noReserve_RadioBtn,0);
    m_pResBtnGroup->addButton(ui->grayReserve_RadioBtn,1);
    m_pResBtnGroup->addButton(ui->disColorReserve_RadioBtn,2);
    m_pResBtnGroup->addButton(ui->proportionReserve_radioBtn,3);

    m_pGroupLbl = ui->label_group;
    for(int i = 0;i< 4;i++){
        m_pResBtnGroup->button(i)->setCheckable(true);
        connect(m_pResBtnGroup->button(i),SIGNAL(clicked(bool)),this,SLOT(resRadioBtnClicked(bool)));
    }
    m_pModeBtnGroup = new QButtonGroup(this);
    m_pModeBtnGroup->addButton(ui->darkBtn,0);
    m_pModeBtnGroup->addButton(ui->lightBtn,1);

    m_pModeBtnGroupColorRes = new QButtonGroup(this);
    m_pModeBtnGroupColorRes->addButton(ui->darkRes1Btn,0);
    m_pModeBtnGroupColorRes->addButton(ui->lightRes1Btn,1);

    m_pModeBtnGroupDiffRes = new QButtonGroup(this);
    m_pModeBtnGroupDiffRes->addButton(ui->darkRes2Btn,0);
    m_pModeBtnGroupDiffRes->addButton(ui->lightRes2Btn,1);

    for(int i = 0;i < 2;i++){
        m_pModeBtnGroup->button(i)->setCheckable(true);
        connect(m_pModeBtnGroup->button(i),SIGNAL(clicked(bool)),this,SLOT(modeBtnClicked(bool)));

        m_pModeBtnGroupColorRes->button(i)->setCheckable(true);
        m_pModeBtnGroupDiffRes->button(i)->setCheckable(true);
        connect(m_pModeBtnGroupColorRes->button(i),SIGNAL(clicked(bool)),this,SLOT(modeBtnRes1Clicked(bool)));
        connect(m_pModeBtnGroupDiffRes->button(i),SIGNAL(clicked(bool)),this,SLOT(modeBtnRes2Clicked(bool)));
    }

    ui->pushButton_3->setCheckable(true);
    connect(ui->sNamelineEdit,SIGNAL(valueSetSignal()),this,SLOT(sNameChanged()));

    ui->edgeBtn->hide();
    ui->infectBtn->hide();
    ui->label_13->hide();
    ui->SensUpdateCheckBox->hide();
    ui->label_8->hide();
    ui->comboBox->hide();
}

ColorHueSenseSetWidget::~ColorHueSenseSetWidget()
{
    delete ui;
}

void ColorHueSenseSetWidget::showPage(bool bshow)
{
    if(bshow){
        updateGroupLabelShow();

        updateChuteState();
        updateValue();
        updateEdgeBtn();
        updateinfectBtn();
        updateModeBtnState();

        //进入页面默认隐藏不常用配置
        ui->pushButton_3->hide();
        ui->reserveArith->setVisible(false);
        ui->groupBox_2->setVisible(false);

        ui->percentCheckBox->setVisible(false);
        ui->lineDefectCheckbox->setVisible(false);

        ui->brightlimitlbl->setVisible(false);
        ui->brightlimit_edit->setVisible(false);
        setViewVisibleByArith();
        updatePercentViewState();
        updateLineDefectState();
        updateSizeState();

    }
}

void ColorHueSenseSetWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    const stru_iden_group_info &groupInfo = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].stuIdenGupInfo[struGsh.nMatGroup];

    if(msg1 == MSG_DATA_CHANGED){
        if(msg2 < MAX_VIEW_UNIT_USE*2){

        }else{
            switch (msg2) {
            case 40:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorHue.nRow = m_rowInt;
                if(m_percentInt > m_rowInt*m_rowInt)
                {
                    m_percentInt = m_rowInt*m_rowInt;
                    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorHue.nPercent = m_percentInt;
                }
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorHue.lightLimit = m_lightLimit;
                ui->percentEdit->setText(QString::number(m_percentInt));
                ui->percentEdit->setMinMax(this,1,m_rowInt*m_rowInt,41,&m_percentInt);

                ui->lightEdit->setText(QString::number(m_lightLimit));
                ui->lightEdit->setMinMax(this,1,1023,41,&m_lightLimit);

                break;
            case 41:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorHue.nPercent = m_percentInt;
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorHue.lightLimit = m_lightLimit;
                break;
            default:
                break;
            }
        }
        setAndSendParams();
    }
}

void ColorHueSenseSetWidget::retranslateUiWidget()
{
    ui->label->setText(g_myLan().impurity_name);
    ui->label_8->setText(g_myLan().ids_color);
    ui->label_8->hide();
    ui->groupBox_4->setTitle(g_myLan().basic_set);
    ui->standard_titlelbl->setText(g_myLan().standard);
    ui->label_front->setText(g_myLan().front);
    ui->label_rear->setText(g_myLan().rear);
    ui->lightBtn->setText(g_myLan().ids_light);
    ui->darkBtn->setText(g_myLan().ids_selectDark);
    ui->percentCheckBox->setText(g_myLan().arith_propotion);
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
    ui->label_3->setText(g_myLan().percent);
    ui->label_4->setText(g_myLan().light_limit);

    ui->percentGroupbox->setTitle(g_myLan().percent);
    ui->areaUpp_2->setText(g_myLan().ids_uplimit);
    ui->areaLow_2->setText(g_myLan().ids_lowlimit);
    ui->edgeBtn->setText(g_myLan().edge_cut);
    ui->infectBtn->setText(g_myLan().infect_arithmetic);
    ui->label_13->setText(g_myLan().ids_AdjustSens);

    ui->reserveArith->setTitle(g_myLan().ids_reserve);
    ui->noReserve_RadioBtn->setText(g_myLan().ids_disable);
    ui->grayReserve_RadioBtn->setText(g_myLan().ids_gray);
    ui->disColorReserve_RadioBtn->setText(g_myLan().ids_difference);
    ui->proportionReserve_radioBtn->setText(g_myLan().arith_propotion);

    ui->label_10->setText(g_myLan().ids_color);
    ui->label_5->setText(g_myLan().ids_sensitivity);
    ui->lightRes1Btn->setText(g_myLan().ids_light);
    ui->darkRes1Btn->setText(g_myLan().ids_selectDark);
    ui->label_14->setText(g_myLan().ids_sensitivity);
    ui->label_9->setText(g_myLan().ids_color);
    ui->label_6->setText(g_myLan().ids_sensitivity);
    ui->lightRes2Btn->setText(g_myLan().ids_light);
    ui->darkRes2Btn->setText(g_myLan().ids_selectDark);
    ui->brightlimitlbl->setText(g_myLan().light_limit);
}

void ColorHueSenseSetWidget::updateValue()//更新保留算法相关的数据
{
    ui->comboBox->hide();
    ui->comboBox_diff->hide();//保留算法
    ui->comboBox_gray->hide();

    ui->darkBtn->hide();
    ui->lightBtn->hide();
    static stu_group_identify &groupIde = struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup];

    int standardSens;
    int standardHueLowSens;

    standardHueLowSens = groupIde.struColorHue.nLowHue;
    standardSens = groupIde.struColorHue.nHighHue;
    ui->HighHueEdit->setText(QString::number(standardSens));
    ui->HighHueEdit->setMinMax(this,0,360,0,&groupIde.struColorHue.nHighHue);
    ui->LowHueEdit->setText(QString::number(standardHueLowSens));
    ui->LowHueEdit->setMinMax(this,0,360,0,&groupIde.struColorHue.nLowHue);

}

void ColorHueSenseSetWidget::updateChuteState()//更新料槽的配置精度值
{
    stru_iden_group_info groupInfo;
    static stu_group_identify groupIde;
    memcpy(&groupInfo,&struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].stuIdenGupInfo[struGsh.nMatGroup],sizeof(groupInfo));
    memcpy(&groupIde,&struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup],sizeof(groupIde));

    QByteArray titleArr(groupIde.struColorHue.sName);
    ui->sNamelineEdit->setText(QString::fromUtf8(titleArr));

    ui->sNamelineEdit->setType(textType);
    int standardSens;
    int standardHueLowSens;

    standardHueLowSens = groupIde.struColorHue.nLowHue;
    standardSens = groupIde.struColorHue.nHighHue;
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
                m_pChuteSenseEdit[0][unitIndex]->setMinMax(this,0-stand,ARITH_HUE_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][1].hueH);
                m_pChuteSenseEdit[1][unitIndex]->setMinMax(this,0-stand,ARITH_HUE_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][unitIndex][1].hueH);
                m_pChuteSenseEdit[0][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][1].hueH));
                m_pChuteSenseEdit[1][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][unitIndex][1].hueH));
            }else{
                m_pChuteSenseEdit[0][unitIndex]->setMinMax(this,0-stand,ARITH_HUE_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].hueH);
                m_pChuteSenseEdit[1][unitIndex]->setMinMax(this,0-stand,ARITH_HUE_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][unitIndex][0].hueH);
                m_pChuteSenseEdit[0][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].hueH));
                m_pChuteSenseEdit[1][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][unitIndex][0].hueH));
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
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setMinMax(this,0-stand,ARITH_HUE_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][1].hueH);
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][1].hueH));
            }else{
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setMinMax(this,0-stand,ARITH_HUE_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].hueH);
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].hueH));
            }

        }
    }
}


void ColorHueSenseSetWidget::updateEdgeBtn()
{

}

void ColorHueSenseSetWidget::updateinfectBtn()
{

}

void ColorHueSenseSetWidget::setViewVisibleByArith()//更新保留算法控件的显示和隐藏
{
    ui->proportionReserve_radioBtn->hide();
    ui->percentWidget->hide();

    ui->grayReserve_RadioBtn->hide();
    ui->disColorReserve_RadioBtn->hide();
    ui->grayResWidget->hide();
    ui->diffResWidget->hide();
}
void ColorHueSenseSetWidget::updateModeBtnState()
{

}
void ColorHueSenseSetWidget::onModeChange(int mode)
{
//    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorHue.nMode = mode;
}

void ColorHueSenseSetWidget::updatePercentViewState()//更新比例算法相关控件的显示和隐藏
{
  //version 1.5
    ui->percentCheckBox->hide();
    ui->percentGroupbox->hide();
}

void ColorHueSenseSetWidget::updateLineDefectState()
{
    ui->lineDefectCheckbox->hide();
    ui->lineDefectGroupbox->hide();
}

void ColorHueSenseSetWidget::updateSizeState()
{
    int layer = struGsh.nLayer;
    int view = struGsh.nView;
    int group = struGsh.nMatGroup;
    int index = struGsh.nArithIndex;
    stru_color_hue  &color_hue  = struCnfp.struGroupIdentify[layer][view][group].struColorHue;
    int maxPercent = 0;
    maxPercent = color_hue.nRow*color_hue.nRow;
    m_rowInt = color_hue.nRow;
    m_percentInt = color_hue.nPercent;
    m_lightLimit = color_hue.lightLimit;
    ui->rowEdit->setText(QString::number(m_rowInt));
    ui->rowEdit->setMinMax(this,1,32,40,&m_rowInt);
    ui->percentEdit->setText(QString::number(m_percentInt));
    ui->percentEdit->setMinMax(this,1,maxPercent,41,&m_percentInt);
    ui->lightEdit->setText(QString::number(m_lightLimit));
    ui->lightEdit->setMinMax(this,1,1023,41,&m_lightLimit);
}

void ColorHueSenseSetWidget::resRadioBtnClicked(bool checked)
{

}

void ColorHueSenseSetWidget::on_edgeBtn_clicked(bool checked)
{

}

void ColorHueSenseSetWidget::on_infectBtn_clicked(bool checked)
{

}

void ColorHueSenseSetWidget::on_comboBox_activated(int index)
{

}

void ColorHueSenseSetWidget::sNameChanged()
{
    memset(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorHue.sName,0,sizeof(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorHue.sName));

    QString name = ui->sNamelineEdit->text();
    QByteArray namearr = name.toUtf8();
    const char *strName = namearr.data();
    memcpy(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorHue.sName,strName,strlen(strName));
}

void ColorHueSenseSetWidget::on_SensUpdateCheckBox_clicked(bool checked)
{
    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].nSensUpdate = (int)checked;
    setAndSendParams();
}

void ColorHueSenseSetWidget::on_comboBox_diff_activated(int index)
{

}

void ColorHueSenseSetWidget::on_comboBox_gray_activated(int index)
{

}

void ColorHueSenseSetWidget::modeBtnClicked(bool check)//算法的选亮选暗切换
{
    if(check)
        onModeChange(m_pModeBtnGroup->id((QPushButton*)sender()));
    updateChuteState();
    setAndSendParams();
}

void ColorHueSenseSetWidget::modeBtnRes1Clicked(bool)//灰度保留的选亮选暗切换
{
    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].struResGreyColor.nMode1 = m_pModeBtnGroupColorRes->id((QPushButton*)sender());
    updateValue();
    setAndSendParams();
}

void ColorHueSenseSetWidget::modeBtnRes2Clicked(bool)//色差保留的选亮选暗切换
{
    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].struResGreyColor.nMode2 = m_pModeBtnGroupDiffRes->id((QPushButton*)sender());
    updateValue();
    setAndSendParams();
}

void ColorHueSenseSetWidget::on_percentCheckBox_clicked(bool checked)
{

}

void ColorHueSenseSetWidget::on_pushButton_3_clicked(bool checked)
{
    ui->groupBox_2->setVisible((struGsh.nArithIndex == ARITH_DISCOLOR_A)||(struGsh.nArithIndex == ARITH_DISCOLOR_B));
    //switch(struCnfg.struLayerInfo[struGsh.nLayer].nViewSensorType[struGsh.nView])
    switch(struCnfp.nViewSensorType[0][struGsh.nView])
    {
    case SENSOR_T1:
    case SENSOR_T1_850:
        ui->reserveArith->setVisible(checked);
        break;
    case SENSOR_C1:
        ui->reserveArith->setVisible(false);
        break;
    }
}

void ColorHueSenseSetWidget::on_lineDefectCheckbox_clicked(bool checked)
{
    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struGreyColor[struGsh.nArithIndex].nLineDefectEnable = (int)checked;
    updateLineDefectState();
    updateSizeState();
    setAndSendParams();
}
