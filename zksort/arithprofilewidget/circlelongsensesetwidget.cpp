#include "circlelongsensesetwidget.h"
#include "ui_circlelongsensesetwidget.h"

CircleLongSenseSetWidget::CircleLongSenseSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::CircleLongSenseSetWidget)
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

    m_pModeBtnGroup = new QButtonGroup(this);   
    m_pModeBtnGroup->addButton(ui->pushButton_2,1);
    m_pModeBtnGroup->addButton(ui->pushButton,0);

    m_pGroupLbl = ui->label_group;

    for(int i = 0;i<2;i++){
        m_pModeBtnGroup->button(i)->setCheckable(true);
        connect(m_pModeBtnGroup->button(i),SIGNAL(clicked(bool)),this,SLOT(modeBtnClicked(bool)));
    }
    connect(ui->sNamelineEdit,SIGNAL(valueSetSignal()),this,SLOT(sNameChanged()));
}

CircleLongSenseSetWidget::~CircleLongSenseSetWidget()
{
    delete ui;
}

void CircleLongSenseSetWidget::showPage(bool bshow)
{
    if(bshow){
        updateGroupLabelShow();
        updateChuteState();
        updateModeBtnState();

        if(struCnfg.stuProfileIndex[struCnfg.nProfile].nProfileLock == 1)
        {
            ui->groupBox->hide();
            ui->groupBox_5->hide();
        }
        else
        {
            ui->groupBox->show();
            ui->groupBox_5->show();
        }

        QStringList list;
        list<<g_myLan().ids_delay_select+"1"<<g_myLan().ids_delay_select+"2";
        ui->comboBox_delay->clear();
        ui->comboBox_delay->addItems(list);
        ui->comboBox_delay->setCurrentIndex(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struCircleLong[struGsh.nArithIndex-ARITH_CIRCLE_LONG].delayType);
    }
}

void CircleLongSenseSetWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){
        setAndSendParams();
    }
}

void CircleLongSenseSetWidget::retranslateUiWidget()
{
    ui->groupBox_4->setTitle(g_myLan().impurity_name);
    ui->groupBox->setTitle(g_myLan().ids_model);
    ui->groupBox_5->setTitle(g_myLan().params_limit);
    ui->standard_titlelbl->setText(g_myLan().standard);
    ui->label_front->setText(g_myLan().front);
    ui->label_rear->setText(g_myLan().rear);
    ui->pushButton_2->setText(g_myLan().ids_selectThin);
    ui->pushButton->setText(g_myLan().ids_selectThick);
    ui->groupBox_2->setTitle(g_myLan().ids_delay_select);

}

void CircleLongSenseSetWidget::updateChuteState()
{
    stru_iden_group_info groupInfo;
    stu_group_identify groupIde;
    memcpy(&groupInfo,&struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].stuIdenGupInfo[struGsh.nMatGroup],sizeof(groupInfo));
    memcpy(&groupIde,&struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup],sizeof(groupIde));

    QByteArray titleArr(groupIde.struCircleLong[struGsh.nArithIndex-ARITH_CIRCLE_LONG].sName);
    ui->sNamelineEdit->setText(QString::fromUtf8(titleArr));
    ui->sNamelineEdit->setType(textType);
    float standardSens;
    if(groupIde.struCircleLong[struGsh.nArithIndex-ARITH_CIRCLE_LONG].nMode){
        standardSens = groupIde.struCircleLong[struGsh.nArithIndex-ARITH_CIRCLE_LONG].nCilosUp;
    }else{
        standardSens = groupIde.struCircleLong[struGsh.nArithIndex-ARITH_CIRCLE_LONG].nCiloLow;
    }
    ui->standardlbl->setText(IntToQString(standardSens));

    for(int i = 0;i < MAX_VIEW_UNIT_USE;i++){

        m_pChuteTitle[i]->hide();
        m_pChuteSenseEdit[0][i]->hide();
        m_pChuteSenseEdit[1][i]->hide();

    }

    ui->limitLineEdit->setMinMax(this,1,65535, 100, &struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struCircleLong[struGsh.nArithIndex-ARITH_CIRCLE_LONG].nLimit);
    ui->limitLineEdit->setText(QString("%1").arg(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struCircleLong[struGsh.nArithIndex-ARITH_CIRCLE_LONG].nLimit));

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
                m_pChuteSenseEdit[0][unitIndex]->setMinMax(this,0-stand,ARITH_LONG_CIRCLE_MAX_VALUE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].circleLong[struGsh.nArithIndex-ARITH_CIRCLE_LONG]);
                m_pChuteSenseEdit[1][unitIndex]->setMinMax(this,0-stand,ARITH_LONG_CIRCLE_MAX_VALUE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][unitIndex][0].circleLong[struGsh.nArithIndex-ARITH_CIRCLE_LONG]);

                m_pChuteSenseEdit[0][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].circleLong[struGsh.nArithIndex-ARITH_CIRCLE_LONG]));
                m_pChuteSenseEdit[1][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][unitIndex][0].circleLong[struGsh.nArithIndex-ARITH_CIRCLE_LONG]));
            }else{
                m_pChuteSenseEdit[0][unitIndex]->setMinMax(this,0-stand,ARITH_LONG_CIRCLE_MAX_VALUE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].circleLong[struGsh.nArithIndex-ARITH_CIRCLE_LONG]);
                m_pChuteSenseEdit[1][unitIndex]->setMinMax(this,0-stand,ARITH_LONG_CIRCLE_MAX_VALUE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][unitIndex][0].circleLong[struGsh.nArithIndex-ARITH_CIRCLE_LONG]);

                m_pChuteSenseEdit[0][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].circleLong[struGsh.nArithIndex-ARITH_CIRCLE_LONG]));
                m_pChuteSenseEdit[1][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][unitIndex][0].circleLong[struGsh.nArithIndex-ARITH_CIRCLE_LONG]));
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
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setMinMax(this,0-stand,ARITH_LONG_CIRCLE_MAX_VALUE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].circleLong[struGsh.nArithIndex-ARITH_CIRCLE_LONG]);
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].circleLong[struGsh.nArithIndex-ARITH_CIRCLE_LONG]));
            }else{
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setMinMax(this,0-stand,ARITH_LONG_CIRCLE_MAX_VALUE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].circleLong[struGsh.nArithIndex-ARITH_CIRCLE_LONG]);
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].circleLong[struGsh.nArithIndex-ARITH_CIRCLE_LONG]));
            }

        }
    }
}
void CircleLongSenseSetWidget::updateModeBtnState()
{
    if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struCircleLong[struGsh.nArithIndex-ARITH_CIRCLE_LONG].nMode == 0){
        m_pModeBtnGroup->button(0)->setChecked(true);
    }else{
        m_pModeBtnGroup->button(1)->setChecked(true);
    }
}

void CircleLongSenseSetWidget::onModeChange(int mode)
{
    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struCircleLong[struGsh.nArithIndex-ARITH_CIRCLE_LONG].nMode = mode;
}

void CircleLongSenseSetWidget::modeBtnClicked(bool)
{
    onModeChange(m_pModeBtnGroup->id((QPushButton*)sender()));
    updateChuteState();
    setAndSendParams();
}

void CircleLongSenseSetWidget::sNameChanged()
{
    memset(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struCircleLong[struGsh.nArithIndex-ARITH_CIRCLE_LONG].sName,0,sizeof(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struCircleLong[struGsh.nArithIndex-ARITH_CIRCLE_LONG].sName));

    QString name = ui->sNamelineEdit->text();
    QByteArray namearr = name.toUtf8();
    const char *strName = namearr.data();
    memcpy(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struCircleLong[struGsh.nArithIndex-ARITH_CIRCLE_LONG].sName,strName,strlen(strName));
}

void CircleLongSenseSetWidget::on_comboBox_delay_activated(int index)
{
    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struCircleLong[struGsh.nArithIndex-ARITH_CIRCLE_LONG].delayType = index;
    setAndSendParams();
}
