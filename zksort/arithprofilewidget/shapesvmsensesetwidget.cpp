#include "shapesvmsensesetwidget.h"
#include "ui_shapesvmsensesetwidget.h"
#include <qdebug.h>
ShapeSvmSenseSetWidget::ShapeSvmSenseSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::ShapeSvmSenseSetWidget)
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

    ui->modeTypeBtn->setCheckable(true);
    ui->extendBtn->hide();
    ui->extendBtn->setCheckable(true);
    connect(ui->sNamelineEdit,SIGNAL(valueSetSignal()),this,SLOT(sNameChanged()));
}

ShapeSvmSenseSetWidget::~ShapeSvmSenseSetWidget()
{
    delete ui;
}

void ShapeSvmSenseSetWidget::showPage(bool bshow)
{
    if(bshow){
        updateGroupLabelShow();
        updateChuteState();
        updateModeBtnState();

        ui->groupBox->setVisible(false);

        updateShapeAreaState();
        stru_shape_svm svm = struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struShapeSvm[struGsh.nArithIndex-ARITH_SHAPE_SVM_A];

        QByteArray nameArray(svm.sName);
        QString strName = QString::fromUtf8(nameArray);
        ui->sNamelineEdit->setText(strName);
        ui->extendBtn->setChecked(false);
        ui->groupBox_2->setVisible(false);
    }
}

void ShapeSvmSenseSetWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){
        if(msg2 < MAX_VIEW_UNIT_USE*2){
        }else if(msg2 == 40){
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struShapeSvm[struGsh.nArithIndex-ARITH_SHAPE_SVM_A].nAreaUp = m_areaUp;
            ui->shapeAreaLowEdit->setMinMax(this,1,m_areaUp,41,&m_areaLow);

        }else if(msg2 == 41){
            if(m_areaLow > m_areaUp){
                m_areaLow = m_areaUp;
            }
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struShapeSvm[struGsh.nArithIndex-ARITH_SHAPE_SVM_A].nAreaLow = m_areaLow;

            ui->shapeAreaLowEdit->setText(IntToQString(m_areaLow));
            ui->shapeAreaLowEdit->setMinMax(this,1,m_areaUp,41,&m_areaLow);
        }else if(msg2 == 45){
            struCnfp.nGroupArithEdgeEnable[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup][struGsh.nArithIndex] = struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struShapeSvm[struGsh.nArithIndex-ARITH_SHAPE_SVM_A].nEdge;;
        }
        setAndSendParams();
    }
}

void ShapeSvmSenseSetWidget::retranslateUiWidget()
{
    ui->groupBox_4->setTitle(g_myLan().impurity_name);
    ui->groupBox->setTitle(g_myLan().ids_model);
    ui->standard_titlelbl->setText(g_myLan().standard);
    ui->label_front->setText(g_myLan().front);
    ui->label_rear->setText(g_myLan().rear);
    ui->label_8->setText(g_myLan().grey);
    //area
    ui->shapeAreaGroupbox->setTitle(g_myLan().ids_area);
    ui->shapeAreaUpLbl->setText(g_myLan().ids_uplimit);
    ui->shapeAreaLowLbl->setText(g_myLan().ids_lowlimit);

    ui->edgeBtn->setText(g_myLan().edge_cut);
    ui->edgeLbl->setText(g_myLan().edge_cut);
}

void ShapeSvmSenseSetWidget::updateChuteState()
{
    stru_iden_group_info groupInfo;
    stu_group_identify groupIde;
    memcpy(&groupInfo,&struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].stuIdenGupInfo[struGsh.nMatGroup],sizeof(groupInfo));
    memcpy(&groupIde,&struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup],sizeof(groupIde));

    QByteArray titleArr(groupIde.struShapeSvm[struGsh.nArithIndex-ARITH_SHAPE_SVM_A].sName);
    ui->sNamelineEdit->setText(QString::fromUtf8(titleArr));
    ui->sNamelineEdit->setType(textType);
    int standardSens = groupIde.struShapeSvm[struGsh.nArithIndex-ARITH_SHAPE_SVM_A].nSens;
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
                m_pChuteSenseEdit[0][unitIndex]->setMinMax(this,0-stand,max_svm-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].shapeSvm[struGsh.nArithIndex-ARITH_SHAPE_SVM_A]);
                m_pChuteSenseEdit[1][unitIndex]->setMinMax(this,0-stand,max_svm-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][unitIndex][0].shapeSvm[struGsh.nArithIndex-ARITH_SHAPE_SVM_A]);

                m_pChuteSenseEdit[0][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].shapeSvm[struGsh.nArithIndex-ARITH_SHAPE_SVM_A]));
                m_pChuteSenseEdit[1][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][unitIndex][0].shapeSvm[struGsh.nArithIndex-ARITH_SHAPE_SVM_A]));
            }else{
                m_pChuteSenseEdit[0][unitIndex]->setMinMax(this,0-stand,max_svm-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].shapeSvm[struGsh.nArithIndex-ARITH_SHAPE_SVM_A]);
                m_pChuteSenseEdit[1][unitIndex]->setMinMax(this,0-stand,max_svm-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][unitIndex][0].shapeSvm[struGsh.nArithIndex-ARITH_SHAPE_SVM_A]);
                m_pChuteSenseEdit[0][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].shapeSvm[struGsh.nArithIndex-ARITH_SHAPE_SVM_A]));
                m_pChuteSenseEdit[1][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][unitIndex][0].shapeSvm[struGsh.nArithIndex-ARITH_SHAPE_SVM_A]));
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
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setMinMax(this,0-stand,max_svm-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].shapeSvm[struGsh.nArithIndex-ARITH_SHAPE_SVM_A]);
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].shapeSvm[struGsh.nArithIndex-ARITH_SHAPE_SVM_A]));
            }else{
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setMinMax(this,0-stand,max_svm-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].shapeSvm[struGsh.nArithIndex-ARITH_SHAPE_SVM_A]);
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].shapeSvm[struGsh.nArithIndex-ARITH_SHAPE_SVM_A]));
            }
        }
    }

}

//void ShapeSvmSenseSetWidget::updateEdgeBtn()
//{
//    //添加算法切边
//    int layer = struGsh.nLayer;
//    int view = struGsh.nView;
//    int group = struGsh.nMatGroup;
//    int index = struGsh.nArithIndex;
//    stu_shape_svm &svm = struCnfp.struGroupIdentify[layer][view][group].struShapeSvm;

//    ui->edgeCutEdit->setText(IntToQString(svm.nEdge));
//    ui->edgeCutEdit->setMinMax(this,0,10,45,&svm.nEdge);

//    struCnfp.nGroupArithEdgeEnable[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup][struGsh.nArithIndex] = svm.nEdge;

//}

void ShapeSvmSenseSetWidget::setViewVisibleByArith()
{
    for(int i = 0;i < 4;i++){
        if(m_pViewBtn[i]->isVisible()){
            if(struCnfp.struGroupIdentify[struGsh.nLayer][i][struGsh.nMatGroup].struShapeSvm[struGsh.nArithIndex-ARITH_SHAPE_SVM_A].nDispEnable ==0 ){
                m_pViewBtn[i]->setVisible(false);
            }
        }
    }
}
void ShapeSvmSenseSetWidget::updateModeBtnState()
{
    if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struShapeSvm[struGsh.nArithIndex-ARITH_SHAPE_SVM_A].nModeSvm == 0){
        ui->modeTypeBtn->setChecked(true);
        ui->modeTypeBtn->setText(g_myLan().normal_sort);
    }else{
        ui->modeTypeBtn->setChecked(false);
        ui->modeTypeBtn->setText(g_myLan().reverse_sort);
    }
}

void ShapeSvmSenseSetWidget::sNameChanged()
{
    memset(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struShapeSvm[struGsh.nArithIndex-ARITH_SHAPE_SVM_A].sName,0,sizeof(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struShapeSvm[struGsh.nArithIndex-ARITH_SHAPE_SVM_A].sName));

    QString name = ui->sNamelineEdit->text();
    QByteArray namearr = name.toUtf8();
    const char *strName = namearr.data();
    memcpy(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struShapeSvm[struGsh.nArithIndex-ARITH_SHAPE_SVM_A].sName,strName,strlen(strName));
}

void ShapeSvmSenseSetWidget::updateShapeAreaState()
{
    int layer = struGsh.nLayer;
    int view = struGsh.nView;

    int group = struGsh.nMatGroup;
    stu_shape_svm &svm = struCnfp.struGroupIdentify[layer][view][group].struShapeSvm[struGsh.nArithIndex-ARITH_SHAPE_SVM_A];

    ui->shapeAreaUpEdit->setText(IntToQString(svm.nAreaUp));
    ui->shapeAreaUpEdit->setMinMax(this,1,1023,40,&m_areaUp);
    ui->shapeAreaLowEdit->setText(IntToQString(svm.nAreaLow));
    ui->shapeAreaLowEdit->setMinMax(this,1,svm.nAreaUp,41,&m_areaLow);
}

void ShapeSvmSenseSetWidget::on_modeTypeBtn_clicked(bool checked)
{
    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struShapeSvm[struGsh.nArithIndex-ARITH_SHAPE_SVM_A].nModeSvm = (int)(!checked);
    updateModeBtnState();
    setAndSendParams();
}

void ShapeSvmSenseSetWidget::on_extendBtn_clicked(bool checked)
{
    ui->groupBox_2->setVisible(checked);
}
