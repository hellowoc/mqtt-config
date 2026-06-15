#include "svmsensesetwidget.h"
#include "ui_svmsensesetwidget.h"
#include <qdebug.h>
SvmSenseSetWidget::SvmSenseSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::SvmSenseSetWidget)
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
    m_pChuteTitle[10] = ui->label_23;
    m_pChuteTitle[11] = ui->label_24;
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
    m_pChuteSenseEdit[0][10] = ui->lineEdit_21;
    m_pChuteSenseEdit[0][11] = ui->lineEdit_22;

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
    m_pChuteSenseEdit[1][10] = ui->lineEdit_1_20;
    m_pChuteSenseEdit[1][11] = ui->lineEdit_1_21;

    m_pGroupLbl = ui->label_group;
    ui->groupBox_3->hide();

    ui->svmTypeBtn->setCheckable(true);
    ui->modeTypeBtn->setCheckable(true);
    ui->extendBtn->hide();
    ui->extendBtn->setCheckable(true);
    connect(ui->sNamelineEdit,SIGNAL(valueSetSignal()),this,SLOT(sNameChanged()));
    connect(myUpdateStatusThread,SIGNAL(sUpdateCornChuteState()),this,SLOT(updateCornChuteState()));

    ui->label_front->hide();
    ui->label_rear->hide();
    ui->standard_titlelbl->hide();
    for(int i = 0;i < MAX_VIEW_UNIT_USE;i++){
        m_pChuteTitle[i]->hide();
        m_pChuteSenseEdit[0][i]->hide();
        m_pChuteSenseEdit[1][i]->hide();
    }

}

SvmSenseSetWidget::~SvmSenseSetWidget()
{
    delete ui;
}

void SvmSenseSetWidget::showPage(bool bshow)
{
    if(bshow){
        updateGroupLabelShow();
        updateChuteState();
        updateModeBtnState();
        updateEdgeBtn();
        updateImpureBtn();
        updateSvmTypeBtnState();

#if USE_NEW_PLATFORM_PROTOCAL == 1
        ui->percentCheckBox->setVisible(struGsh.nArithIndex == ARITH_SVM_A);
        ui->lineDefectCheckbox->setVisible(struGsh.nArithIndex == ARITH_SVM_B);
#else
//        ui->percentCheckBox->setVisible(false);
        ui->percentCheckBox->setVisible(struGsh.nArithIndex == ARITH_SVM_B);
        ui->lineDefectCheckbox->setVisible(false);
        ui->groupBox->setVisible(false);
#endif
        updatePropertionState();
        updateLineDefectState();
        updateSizeState();
        stru_svm svm = struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A];

        QByteArray nameArray(svm.sName);
        QString strName = QString::fromUtf8(nameArray);
        ui->sNamelineEdit->setType(textType);
        ui->sNamelineEdit->setText(strName);
        ui->extendBtn->setChecked(false);
        ui->groupBox_2->setVisible(false);

        ui->consExpandValueLbl->setText(g_myLan().lamp_coef);
        ui->consExpandValueEdit->setMinMax(this,0,100000,21,&struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].nConsExpandedValue);
        ui->consExpandValueEdit->setText(IntToQString(svm.nConsExpandedValue));
        ui->consExpandValueEdit->setVisible(svm.nModeType ==2);
        ui->consExpandValueLbl->setVisible(svm.nModeType ==2);

        ui->maxValueEdit->setMinMax(this,0,99,31,&struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].maxSenseValue);
        ui->maxValueEdit->setText(IntToQString(svm.maxSenseValue));
        ui->ImpurityValueEdit->setMinMax(this,0,99,32,&struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].impuritPos);
        ui->ImpurityValueEdit->setText(IntToQString(svm.impuritPos));

        ui->brightlimitLbl->setVisible(struGsh.bRunMode > RUNMODE_ENGINEER);
        ui->brightlimit_edit->setVisible(struGsh.bRunMode > RUNMODE_ENGINEER);
        ui->brightlimit_edit->setMinMax(this,0,255,33,&struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].brightLimit);
        ui->brightlimit_edit->setText(IntToQString(svm.brightLimit));

        QStringList list;
        list<<g_myLan().ids_delay_select+"1"<<g_myLan().ids_delay_select+"2";
        ui->comboBox_delay->clear();
        ui->comboBox_delay->addItems(list);
        ui->comboBox_delay->setCurrentIndex(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].delayType);
    }
}

void SvmSenseSetWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){
        if(msg2 < MAX_VIEW_UNIT_USE*2){
        }else if(msg2 == 31){
            int min = 100- struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].maxSenseValue;
            int max = 100+ struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].maxSenseValue;
            int sens = struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].nSens;
            if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].nSens<min
                    || struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].nSens>max)
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].nSens = 100;
            updateChuteState();

        }else if(msg2 == 42){
            if(m_nPropotionSens[1]>m_nPropotionSens[0])
            {
                m_nPropotionSens[1] = m_nPropotionSens[0];
            }
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].fPropotionSensUp = m_nPropotionSens[0];
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].fPropotionSensLow = m_nPropotionSens[1];

            ui->PropotionLowEdit->setText(IntToQString(m_nPropotionSens[1]));
            ui->PropotionLowEdit->setFloatMinMax(this,0,m_nPropotionSens[0],43,&m_nPropotionSens[1],false);
        }else if(msg2 == 43){
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].fPropotionSensLow = m_nPropotionSens[1];
        }else if(msg2 == 40){
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].nRow = m_rowInt;
            if(m_percentInt > m_rowInt*m_rowInt)
            {
                m_percentInt = m_rowInt*m_rowInt;
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].nPercent = m_percentInt;
            }
            ui->purityEdit->setText(QString::number(m_percentInt));
            ui->purityEdit->setMinMax(this,1,m_rowInt*m_rowInt,41,&m_percentInt);
        }else if(msg2 == 41){
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].nPercent = m_percentInt;
        }else if(msg2 == 44){
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].nPropotionParaSensLow = m_nPropotionParaSensLow;
        }else if(msg2 == 45){
            struCnfp.nGroupArithEdgeEnable[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup][struGsh.nArithIndex] = struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].nEdge;
        }
        setAndSendParams();
    }
}

void SvmSenseSetWidget::retranslateUiWidget()
{
    ui->groupBox_4->setTitle(g_myLan().impurity_name);
    ui->groupBox->setTitle(g_myLan().ids_model);
    ui->percentCheckBox->setText(g_myLan().arith_scale);
    ui->lineDefectCheckbox->setText(g_myLan().arith_line_defect);
    ui->standard_titlelbl->setText(g_myLan().standard);
    ui->label_front->setText(g_myLan().front);
    ui->label_rear->setText(g_myLan().rear);
    ui->lineDefectGroupbox->setTitle(g_myLan().arith_line_defect);
    //area
    ui->areaGroupbox->setTitle(g_myLan().ids_area);
    ui->areaUpp->setText(g_myLan().ids_uplimit);
    ui->areaLow->setText(g_myLan().ids_lowlimit);

    ui->lengthGroupbox->setTitle(g_myLan().params_limit);
    ui->lengthUpp->setText(g_myLan().ids_uplimit);
    ui->lengthLow->setText(g_myLan().ids_lowlimit);

    ui->sizeGroupbox->setTitle(g_myLan().ids_size);
    ui->puritylbl->setText(g_myLan().ids_purity);
    ui->label_14->setText(g_myLan().ids_size);

    ui->edgeBtn->setText(g_myLan().edge_cut);
    ui->impureBtn->setText(g_myLan().infect_arithmetic);

    ui->scaleGroupbox->setTitle(g_myLan().ids_purity);
    ui->label_11->setText(g_myLan().ids_uplimit);
    ui->label_12->setText(g_myLan().ids_lowlimit);

    ui->edgeLbl->setText(g_myLan().edge_cut);
    ui->PropotionParaLowLbl->setText(g_myLan().params_limit);

    ui->label->setText(g_myLan().ids_delay_select);
    ui->groupBox_3->setTitle(g_myLan().check);
    ui->maxValueLbl->setText(g_myLan().ids_FeedSetRange);
    ui->impurityLbl->setText(g_myLan().ids_rejected);
    ui->brightlimitLbl->setText(g_myLan().light_limit);

    ui->imgBtn->setText(g_myLan().ids_getImage);
    ui->svmBtn->setText("AI");
}

void SvmSenseSetWidget::updateChuteState()
{
#if 0
    stru_iden_group_info groupInfo;
    stu_group_identify groupIde;
    memcpy(&groupInfo,&struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].stuIdenGupInfo[struGsh.nMatGroup],sizeof(groupInfo));
    memcpy(&groupIde,&struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup],sizeof(groupIde));

    QByteArray titleArr(groupIde.struSvm[struGsh.nArithIndex-ARITH_SVM_A].sName);
    ui->sNamelineEdit->setText(QString::fromUtf8(titleArr));
    ui->sNamelineEdit->setType(textType);
    int standardSens = groupIde.struSvm[struGsh.nArithIndex-ARITH_SVM_A].nSens;
    int maxSensVal = groupIde.struSvm[struGsh.nArithIndex-ARITH_SVM_A].maxSenseValue;
    int min = 100-maxSensVal;
    int max = 100+maxSensVal;

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
        if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_SECOND_ALIKE_FIRST)
            unitIndex = i;//算法配置不分次
        else
            unitIndex = groupInfo.nUnitGupId[i];//视链上第几个相机
        if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT){
            ui->label_front->show();
            ui->label_rear->show();

            m_pChuteTitle[unitIndex]->show();
            m_pChuteSenseEdit[0][unitIndex]->show();
            m_pChuteSenseEdit[1][unitIndex]->show();

            if(groupInfo.nUnitGupSplice[i] == 2){
                m_pChuteSenseEdit[0][unitIndex]->setMinMax(this,min-stand,max-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].svm[struGsh.nArithIndex-ARITH_SVM_A]);
                m_pChuteSenseEdit[1][unitIndex]->setMinMax(this,min-stand,max-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][unitIndex][0].svm[struGsh.nArithIndex-ARITH_SVM_A]);

                m_pChuteSenseEdit[0][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].svm[struGsh.nArithIndex-ARITH_SVM_A]));
                m_pChuteSenseEdit[1][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][unitIndex][0].svm[struGsh.nArithIndex-ARITH_SVM_A]));
            }else{
                m_pChuteSenseEdit[0][unitIndex]->setMinMax(this,min-stand,max-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].svm[struGsh.nArithIndex-ARITH_SVM_A]);
                m_pChuteSenseEdit[1][unitIndex]->setMinMax(this,min-stand,max-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][unitIndex][0].svm[struGsh.nArithIndex-ARITH_SVM_A]);
                m_pChuteSenseEdit[0][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].svm[struGsh.nArithIndex-ARITH_SVM_A]));
                m_pChuteSenseEdit[1][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][unitIndex][0].svm[struGsh.nArithIndex-ARITH_SVM_A]));
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
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setMinMax(this,min-stand,max-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].svm[struGsh.nArithIndex-ARITH_SVM_A]);
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].svm[struGsh.nArithIndex-ARITH_SVM_A]));
            }else{
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setMinMax(this,min-stand,max-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].svm[struGsh.nArithIndex-ARITH_SVM_A]);
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].svm[struGsh.nArithIndex-ARITH_SVM_A]));
            }

        }
    }
#endif

}

void SvmSenseSetWidget::updateEdgeBtn()
{
    if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].nEdgeType){
        ui->edgeBtn->setText(g_myLan().edge_cut_2);
    }else{
        ui->edgeBtn->setText(g_myLan().edge_cut_1);
    }
    //添加算法切边
    int layer = struGsh.nLayer;
    int view = struGsh.nView;
    int group = struGsh.nMatGroup;
    int index = struGsh.nArithIndex;
    stu_svm &svm = struCnfp.struGroupIdentify[layer][view][group].struSvm[index-ARITH_SVM_A];

    ui->edgeCutEdit->setText(IntToQString(svm.nEdge));
    ui->edgeCutEdit->setMinMax(this,0,struCnfg.nEdgeCutMaxValue,45,&svm.nEdge);

    struCnfp.nGroupArithEdgeEnable[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup][struGsh.nArithIndex] = svm.nEdge;

}

void SvmSenseSetWidget::updateImpureBtn()
{
    if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].nImpureType){
        ui->impureBtn->setText(g_myLan().infect_arithmetic_2);
    }else{
        ui->impureBtn->setText(g_myLan().infect_arithmetic_1);
    }
}

void SvmSenseSetWidget::setViewVisibleByArith()
{
    for(int i = 0;i < 4;i++){
        if(m_pViewBtn[i]->isVisible()){
            if(struCnfp.struGroupIdentify[struGsh.nLayer][i][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].nDispEnable ==0 ){
                m_pViewBtn[i]->setVisible(false);
            }
        }
    }
}
void SvmSenseSetWidget::updateModeBtnState()
{
    if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].nModeSvm == 0){
        ui->modeTypeBtn->setChecked(true);
        ui->modeTypeBtn->setText(g_myLan().normal_sort);
    }else{
        ui->modeTypeBtn->setChecked(false);
        ui->modeTypeBtn->setText(g_myLan().reverse_sort);
    }
}

void SvmSenseSetWidget::updateSvmTypeBtnState()
{
    ui->svmTypeBtn->hide();
    if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].nTypeIndex == 0){
        ui->svmTypeBtn->setChecked(true);
        ui->svmTypeBtn->setText(g_myLan().svm_type2);
    }else{
        ui->svmTypeBtn->setChecked(false);
        ui->svmTypeBtn->setText(g_myLan().svm_type1);
    }
}

void SvmSenseSetWidget::updatePropertionState()
{
    if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].nPropotionEnable){
        ui->percentCheckBox->setChecked(true);
        ui->scaleGroupbox->setVisible(true);
        ui->PropotionLowEdit->setEnabled(true);
        ui->PropotionUpEdit->setEnabled(true);
    }else{
        ui->percentCheckBox->setChecked(false);
        ui->scaleGroupbox->setVisible(false);
        ui->PropotionLowEdit->setEnabled(false);
        ui->PropotionUpEdit->setEnabled(false);
    }
    m_nPropotionSens[0] = struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].fPropotionSensUp;
    m_nPropotionSens[1] = struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].fPropotionSensLow;
    ui->PropotionUpEdit->setText(IntToQString(m_nPropotionSens[0]));
    ui->PropotionUpEdit->setFloatMinMax(this,0,100,42,&m_nPropotionSens[0],false);
    ui->PropotionLowEdit->setText(IntToQString(m_nPropotionSens[1]));
    ui->PropotionLowEdit->setFloatMinMax(this,0,m_nPropotionSens[0],43,&m_nPropotionSens[1],false);


    m_nPropotionParaSensLow = struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].nPropotionParaSensLow;
    ui->PropotionParaLowEdit->setText(IntToQString(m_nPropotionParaSensLow));
    ui->PropotionParaLowEdit->setMinMax(this,1,1023,44,&m_nPropotionParaSensLow);
}

void SvmSenseSetWidget::on_scaleGroupbox_clicked(bool checked)
{
    ui->PropotionLowEdit->setEnabled(checked);
    ui->PropotionUpEdit->setEnabled(checked);
    if(checked){
        struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].nPropotionEnable = 1;
    }else{
        struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].nPropotionEnable = 0;
    }
    setAndSendParams();
}

void SvmSenseSetWidget::sNameChanged()
{
    memset(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].sName,0,sizeof(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].sName));

    QString name = ui->sNamelineEdit->text();
    QByteArray namearr = name.toUtf8();
    const char *strName = namearr.data();
    memcpy(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].sName,strName,strlen(strName));
}

void SvmSenseSetWidget::updateLineDefectState()
{
    int layer = struGsh.nLayer;
    int view = struGsh.nView;
    int group = struGsh.nMatGroup;
    int index = struGsh.nArithIndex;
    stu_svm &svm = struCnfp.struGroupIdentify[layer][view][group].struSvm[index-ARITH_SVM_A];
    ui->lineDefectCheckbox->setChecked(svm.nLineDefectEnable!=0);
    if(svm.nLineDefectEnable){
        ui->lineDefectGroupbox->setVisible(true);
        ui->areaGroupbox->setEnabled(true);
        ui->lengthGroupbox->setEnabled(true);
    }else{
        ui->lineDefectGroupbox->setVisible(false);
        ui->areaGroupbox->setEnabled(false);
        ui->lengthGroupbox->setEnabled(false);
    }
    m_nPropotionSens[0] = struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].fPropotionSensUp;
    m_nPropotionSens[1] = struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].fPropotionSensLow;
    ui->areaUppEdit->setText(IntToQString(svm.fnLineDefectAreaUp));
    ui->areaLowEdit->setText(IntToQString(svm.fnLineDefectAreaLow));
    ui->lengthUppEdit->setText(IntToQString(svm.fnLineDefectLengthUp));
    ui->lengthLowEdit->setText(IntToQString(svm.fnLineDefectLengthLow));


    ui->areaUppEdit->setMinMax(this,0,100,42,&svm.fnLineDefectAreaUp);
    ui->areaLowEdit->setMinMax(this,0,100,42,&svm.fnLineDefectAreaLow);
    ui->lengthUppEdit->setMinMax(this,0,100,42,&svm.fnLineDefectLengthUp);
    ui->lengthLowEdit->setMinMax(this,0,100,42,&svm.fnLineDefectLengthLow);
}

void SvmSenseSetWidget::updateSizeState()
{
    int layer = struGsh.nLayer;
    int view = struGsh.nView;

    int group = struGsh.nMatGroup;
    int index = struGsh.nArithIndex;
     stu_svm &svm = struCnfp.struGroupIdentify[layer][view][group].struSvm[index-ARITH_SVM_A];
    int maxPercent = 0;
    maxPercent = svm.nRow*svm.nRow;
    if(svm.nLineDefectEnable || svm.nPropotionEnable){
        ui->sizeGroupbox->hide();
    }else{
        ui->sizeGroupbox->show();
    }
    m_rowInt = svm.nRow;
    m_percentInt = svm.nPercent;
    ui->rowEdit->setText(QString::number(m_rowInt));
    ui->rowEdit->setMinMax(this,1,32,40,&m_rowInt);
    ui->purityEdit->setText(QString::number(m_percentInt));
    ui->purityEdit->setMinMax(this,1,maxPercent,41,&m_percentInt);
}

void SvmSenseSetWidget::on_svmTypeBtn_clicked(bool checked)
{
    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].nTypeIndex = (int)(!checked);
    updateSvmTypeBtnState();
    setAndSendParams();
}

void SvmSenseSetWidget::on_modeTypeBtn_clicked(bool checked)
{
    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].nModeSvm = (int)(!checked);
    updateModeBtnState();
    setAndSendParams();
}

void SvmSenseSetWidget::on_lineDefectGroupbox_clicked(bool checked)
{
    int layer = struGsh.nLayer;
    int view = struGsh.nView;
    int group = struGsh.nMatGroup;
    int index = struGsh.nArithIndex;
    struCnfp.struGroupIdentify[layer][view][group].struSvm[index-ARITH_SVM_A].nLineDefectEnable = int(checked);
    updateLineDefectState();
    setAndSendParams();
}

void SvmSenseSetWidget::on_extendBtn_clicked(bool checked)
{
    ui->groupBox_2->setVisible(checked);
}

void SvmSenseSetWidget::on_edgeBtn_clicked(bool checked)
{
    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].nEdgeType = (int)checked;
    updateEdgeBtn();
    setAndSendParams();
}

void SvmSenseSetWidget::on_impureBtn_clicked(bool checked)
{
    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].nImpureType = (int)checked;
    updateImpureBtn();
    setAndSendParams();
}

void SvmSenseSetWidget::on_percentCheckBox_clicked(bool checked)
{
    if(checked){
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].nPropotionEnable = 1;
    }else{
        struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].nPropotionEnable = 0;
    }
    updatePropertionState();
    updateSizeState();
    setAndSendParams();
}

void SvmSenseSetWidget::on_lineDefectCheckbox_clicked(bool checked)
{
    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].nLineDefectEnable = (int)checked;
    updateLineDefectState();
    updateSizeState();
    setAndSendParams();
}

void SvmSenseSetWidget::on_comboBox_delay_activated(int index)
{
    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSvm[struGsh.nArithIndex-ARITH_SVM_A].delayType = index;
    setAndSendParams();
}

void SvmSenseSetWidget::updateCornChuteState()
{
    qDebug()<<"updateing!";
    updateChuteState();

    setAndSendParams();
}

void SvmSenseSetWidget::on_svmBtn_clicked()
{
    g_Runtime().updateTmpImgList();
    if(!g_Runtime().nimageNameVec.isEmpty())
        g_MainManager().ShowWidget(SM_SVM_IMAGE_WIDGET);
    else{
        myMessageBox msg(MSG_INFO,g_myLan().msg_capture_image_first,this);
        msg.exec();
    }
}

void SvmSenseSetWidget::on_imgBtn_clicked()
{
    g_MainManager().ShowWidget(SM_IMAGE_CAPTURE_WIDGET);
}
