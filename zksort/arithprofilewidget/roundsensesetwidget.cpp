#include "roundsensesetwidget.h"
#include "ui_roundsensesetwidget.h"
#include <QButtonGroup>

RoundSenseSetWidget::RoundSenseSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::RoundSenseSetWidget)
{
    ui->setupUi(this);
    m_pChuteTitle[0] = ui->roundLbl1;
    m_pChuteTitle[1] = ui->roundLbl2;
    m_pChuteTitle[2] = ui->roundLbl3;
    m_pChuteTitle[3] = ui->roundLbl4;
    m_pChuteTitle[4] = ui->roundLbl5;
    m_pChuteTitle[5] = ui->roundLbl6;
    m_pChuteTitle[6] = ui->roundLbl7;
    m_pChuteTitle[7] = ui->roundLbl8;
    m_pChuteTitle[8] = ui->roundLbl9;
    m_pChuteTitle[9] = ui->roundLbl10;
    m_pChuteTitle[10] = ui->roundLbl11;
    m_pChuteTitle[11] = ui->roundLbl12;

    m_pChuteSenseEdit[0][0] = ui->roundLineEdit1;
    m_pChuteSenseEdit[0][1] = ui->roundLineEdit2;
    m_pChuteSenseEdit[0][2] = ui->roundLineEdit3;
    m_pChuteSenseEdit[0][3] = ui->roundLineEdit4;
    m_pChuteSenseEdit[0][4] = ui->roundLineEdit5;
    m_pChuteSenseEdit[0][5] = ui->roundLineEdit6;
    m_pChuteSenseEdit[0][6] = ui->roundLineEdit7;
    m_pChuteSenseEdit[0][7] = ui->roundLineEdit8;
    m_pChuteSenseEdit[0][8] = ui->roundLineEdit9;
    m_pChuteSenseEdit[0][9] = ui->roundLineEdit10;
    m_pChuteSenseEdit[0][10] = ui->roundLineEdit11;
    m_pChuteSenseEdit[0][11] = ui->roundLineEdit12;

    m_pChuteSenseEdit[1][0] = ui->roundLineEdit1_1;
    m_pChuteSenseEdit[1][1] = ui->roundLineEdit1_2;
    m_pChuteSenseEdit[1][2] = ui->roundLineEdit1_3;
    m_pChuteSenseEdit[1][3] = ui->roundLineEdit1_4;
    m_pChuteSenseEdit[1][4] = ui->roundLineEdit1_5;
    m_pChuteSenseEdit[1][5] = ui->roundLineEdit1_6;
    m_pChuteSenseEdit[1][6] = ui->roundLineEdit1_7;
    m_pChuteSenseEdit[1][7] = ui->roundLineEdit1_8;
    m_pChuteSenseEdit[1][8] = ui->roundLineEdit1_9;
    m_pChuteSenseEdit[1][9] = ui->roundLineEdit1_10;
    m_pChuteSenseEdit[1][10] = ui->roundLineEdit1_11;
    m_pChuteSenseEdit[1][11] = ui->roundLineEdit1_12;

    m_pModeBtnGroup = new QButtonGroup(this);
    m_pModeBtnGroup->addButton(ui->notRoundBtn,1);
    m_pModeBtnGroup->addButton(ui->roundBtn,0);

    m_pGroupLbl = ui->label_group_2;

    for(int i = 0;i<2;i++){
        m_pModeBtnGroup->button(i)->setCheckable(true);
        connect(m_pModeBtnGroup->button(i),SIGNAL(clicked(bool)),this,SLOT(modeBtnClicked(bool)));
    }
    connect(ui->sNamelineEdit_2,SIGNAL(valueSetSignal()),this,SLOT(sNameChanged()));
}

RoundSenseSetWidget::~RoundSenseSetWidget()
{
    delete ui;
}

void RoundSenseSetWidget::showPage(bool bshow)
{
    if(bshow){
        updateGroupLabelShow();
        updateChuteState();
        updateModeBtnState();

        if(struCnfg.stuProfileIndex[struCnfg.nProfile].nProfileLock == 1)
        {
            ui->groupBox_2->hide();
            ui->groupBox_7->hide();
        }
        else
        {
            ui->groupBox_2->show();
            ui->groupBox_7->show();
        }
    }
}

void RoundSenseSetWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){
        setAndSendParams();
    }
}

void RoundSenseSetWidget::retranslateUiWidget()
{
    ui->groupBox_6->setTitle(g_myLan().impurity_name);
    ui->groupBox_2->setTitle(g_myLan().ids_model);
    ui->groupBox_7->setTitle(g_myLan().params_limit);
    ui->standard_titlelbl_2->setText(g_myLan().standard);
    ui->label_front_2->setText(g_myLan().front);
    ui->label_rear_2->setText(g_myLan().rear);
    ui->roundBtn->setText(g_myLan().ids_selectCircular);
    ui->notRoundBtn->setText(g_myLan().ids_selectNotRound);
}

void RoundSenseSetWidget::updateChuteState()
{
    stru_iden_group_info groupInfo;
    stu_group_identify groupIde;
    memcpy(&groupInfo,&struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].stuIdenGupInfo[struGsh.nMatGroup],sizeof(groupInfo));
    memcpy(&groupIde,&struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup],sizeof(groupIde));

    QByteArray titleArr(groupIde.struRound[struGsh.nArithIndex-ARITH_ROUND].sName);
    ui->sNamelineEdit_2->setText(QString::fromUtf8(titleArr));
    ui->sNamelineEdit_2->setType(textType);
    float standardSens;
    if(groupIde.struRound[struGsh.nArithIndex-ARITH_ROUND].nSelectMode){
        standardSens = groupIde.struRound[struGsh.nArithIndex-ARITH_ROUND].nSensLow;
    }else{
        standardSens = groupIde.struRound[struGsh.nArithIndex-ARITH_ROUND].nSensUp;
    }
    ui->standardlbl_2->setText(IntToQString(standardSens));

    for(int i = 0;i < MAX_VIEW_UNIT_USE;i++){

        m_pChuteTitle[i]->hide();
        m_pChuteSenseEdit[0][i]->hide();
        m_pChuteSenseEdit[1][i]->hide();

    }

    ui->roundLimitEdit->setMinMax(this,1,65535, 100, &struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struRound[struGsh.nArithIndex-ARITH_ROUND].nLimit);
    ui->roundLimitEdit->setText(QString("%1").arg(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struRound[struGsh.nArithIndex-ARITH_ROUND].nLimit));

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
            ui->label_front_2->show();
            ui->label_rear_2->show();

            m_pChuteTitle[unitIndex]->show();
            m_pChuteSenseEdit[0][unitIndex]->show();
            m_pChuteSenseEdit[1][unitIndex]->show();


            if(groupInfo.nUnitGupSplice[i] == 2){
                m_pChuteSenseEdit[0][unitIndex]->setMinMax(this,0-stand,ARITH_ROUND_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].round[struGsh.nArithIndex-ARITH_ROUND]);
                m_pChuteSenseEdit[1][unitIndex]->setMinMax(this,0-stand,ARITH_ROUND_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][unitIndex][0].round[struGsh.nArithIndex-ARITH_ROUND]);

                m_pChuteSenseEdit[0][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].round[struGsh.nArithIndex-ARITH_ROUND]));
                m_pChuteSenseEdit[1][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][unitIndex][0].round[struGsh.nArithIndex-ARITH_ROUND]));
            }else{
                m_pChuteSenseEdit[0][unitIndex]->setMinMax(this,0-stand,ARITH_ROUND_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].round[struGsh.nArithIndex-ARITH_ROUND]);
                m_pChuteSenseEdit[1][unitIndex]->setMinMax(this,0-stand,ARITH_ROUND_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][unitIndex][0].round[struGsh.nArithIndex-ARITH_ROUND]);

                m_pChuteSenseEdit[0][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].round[struGsh.nArithIndex-ARITH_ROUND]));
                m_pChuteSenseEdit[1][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][unitIndex][0].round[struGsh.nArithIndex-ARITH_ROUND]));
            }

        }else{
            if(struGsh.nView%2){
                ui->label_front_2->hide();
                ui->label_rear_2->show();
            }else{
                ui->label_front_2->show();
                ui->label_rear_2->hide();
            }
            m_pChuteTitle[unitIndex]->show();
            m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->show();
            if(groupInfo.nUnitGupSplice[i] == 2){
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setMinMax(this,0-stand,ARITH_ROUND_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].round[struGsh.nArithIndex-ARITH_ROUND]);
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].round[struGsh.nArithIndex-ARITH_ROUND]));
            }else{
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setMinMax(this,0-stand,ARITH_ROUND_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].round[struGsh.nArithIndex-ARITH_ROUND]);
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].round[struGsh.nArithIndex-ARITH_ROUND]));
            }

        }
    }
}
void RoundSenseSetWidget::updateModeBtnState()
{
    if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struRound[struGsh.nArithIndex-ARITH_ROUND].nSelectMode == 0){
        m_pModeBtnGroup->button(0)->setChecked(true);
    }else{
        m_pModeBtnGroup->button(1)->setChecked(true);
    }
}

void RoundSenseSetWidget::onModeChange(int mode)
{
    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struRound[struGsh.nArithIndex-ARITH_ROUND].nSelectMode = mode;
}

void RoundSenseSetWidget::modeBtnClicked(bool)
{
    onModeChange(m_pModeBtnGroup->id((QPushButton*)sender()));
    updateChuteState();
    setAndSendParams();
}

void RoundSenseSetWidget::sNameChanged()
{
    memset(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struRound[struGsh.nArithIndex-ARITH_ROUND].sName,0,sizeof(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struRound[struGsh.nArithIndex-ARITH_ROUND].sName));

    QString name = ui->sNamelineEdit_2->text();
    QByteArray namearr = name.toUtf8();
    const char *strName = namearr.data();
    memcpy(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struRound[struGsh.nArithIndex-ARITH_ROUND].sName,strName,strlen(strName));
}
