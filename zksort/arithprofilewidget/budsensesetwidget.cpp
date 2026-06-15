#include "budsensesetwidget.h"
#include "ui_budsensesetwidget.h"

BudSenseSetWidget::BudSenseSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::BudSenseSetWidget)
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

    connect(ui->sNamelineEdit, SIGNAL(editingFinished()), this, SLOT(sNameChanged()) );
}

BudSenseSetWidget::~BudSenseSetWidget()
{
    delete ui;
}

void BudSenseSetWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){
        if(msg2 < MAX_VIEW_UNIT_USE*2){

        }else{
            switch (msg2) {
            case 40:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struBud[struGsh.nArithIndex-ARITH_BUD].nSize = m_sizeInt;
                if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT)
                {
                    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView+1][struGsh.nMatGroup].struBud[struGsh.nArithIndex-ARITH_BUD].nSize = m_sizeInt;
                }
                break;
            case 41:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struBud[struGsh.nArithIndex-ARITH_BUD].nLimit = m_limitInt;
                if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT)
                {
                    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView+1][struGsh.nMatGroup].struBud[struGsh.nArithIndex-ARITH_BUD].nLimit = m_limitInt;
                }
                break;
            case 42:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struBud[struGsh.nArithIndex-ARITH_BUD].nYellowSens = m_yellowSensInt;
                if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT)
                {
                    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView+1][struGsh.nMatGroup].struBud[struGsh.nArithIndex-ARITH_BUD].nYellowSens = m_yellowSensInt;
                }
                break;
            case 43:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struBud[struGsh.nArithIndex-ARITH_BUD].nBlackSens = m_blackSensInt;
                if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT)
                {
                    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView+1][struGsh.nMatGroup].struBud[struGsh.nArithIndex-ARITH_BUD].nBlackSens = m_blackSensInt;
                }
                break;
            default:
                break;
            }
        }
        setAndSendParams();
    }
}

void BudSenseSetWidget::showPage(bool bshow)
{
    if(bshow)
    {
        updateGroupLabelShow();
        updateChuteState();
    }
}

void BudSenseSetWidget::retranslateUiWidget()
{
    ui->senseNameLabel->setText(g_myLan().impurity_name);
    ui->groupBox->setTitle(g_myLan().ids_size);
    ui->groupBox_5->setTitle(g_myLan().params_limit);
    ui->standard_titlelbl->setText(g_myLan().standard);
    ui->label_front->setText(g_myLan().front);
    ui->label_rear->setText(g_myLan().rear);
    ui->limitLabel->setText(g_myLan().params_limit);
    ui->yellowLabel->setText(g_myLan().yellow_sense);
    ui->blackLabel->setText(g_myLan().black_sense);
}

void BudSenseSetWidget::updateChuteState()
{
    stru_iden_group_info groupInfo;
    stu_group_identify groupIde;
    memcpy(&groupInfo, &struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].stuIdenGupInfo[struGsh.nMatGroup],sizeof(groupInfo));
    memcpy(&groupIde, &struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup], sizeof(groupIde));

    QByteArray titleArr(groupIde.struBud[struGsh.nArithIndex-ARITH_BUD].sName);
    ui->sNamelineEdit->setText(QString::fromUtf8(titleArr));
    ui->sNamelineEdit->setType(textType);

    int standardSens;
    standardSens = groupIde.struBud[struGsh.nArithIndex-ARITH_BUD].nSens;
    ui->standardlbl->setText(IntToQString(standardSens));
    for(int i = 0; i < MAX_VIEW_UNIT_USE; i ++ )
    {
        m_pChuteTitle[i]->hide();
        m_pChuteSenseEdit[0][i]->hide();
        m_pChuteSenseEdit[1][i]->hide();
    }

    m_sizeInt = groupIde.struBud[struGsh.nArithIndex-ARITH_BUD].nSize;
    m_limitInt = groupIde.struBud[struGsh.nArithIndex-ARITH_BUD].nLimit;
    m_yellowSensInt = groupIde.struBud[struGsh.nArithIndex-ARITH_BUD].nYellowSens;
    m_blackSensInt = groupIde.struBud[struGsh.nArithIndex-ARITH_BUD].nBlackSens;

    ui->sizeLineEdit->setText(QString::number(m_sizeInt));
    ui->sizeLineEdit->setMinMax(this, 1, 32, 40, &m_sizeInt);

    ui->limitLineEdit->setText(QString::number(m_limitInt));
    ui->limitLineEdit->setMinMax(this, 0, 255, 41, &m_limitInt);

    ui->yellowLineEdit->setText(QString::number(m_yellowSensInt));
    ui->yellowLineEdit->setMinMax(this, 0, 255, 42, &m_yellowSensInt);

    ui->blackLineEdit->setText(QString::number(m_blackSensInt));
    ui->blackLineEdit->setMinMax(this, 0, 255, 43, &m_blackSensInt);


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
    for(int i = 0; i < tempRepeatCondition; i++ )
    {
        if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_SECOND_ALIKE_FIRST)
        {
            unitIndex = i ;
        }
        else
        {
            unitIndex = groupInfo.nUnitGupId[i];
        }

        if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT)
        {
            ui->label_front->show();
            ui->label_rear->show();
            m_pChuteTitle[unitIndex]->show();
            m_pChuteSenseEdit[0][unitIndex]->show();
            m_pChuteSenseEdit[1][unitIndex]->show();

            if(groupInfo.nUnitGupSplice[i] == 2)
            {
                m_pChuteSenseEdit[0][unitIndex]->setMinMax(this, 0-stand, ARITH_BUD_MAX_SENSE-stand, -1, &struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][1].bud[struGsh.nArithIndex-ARITH_BUD]);
                m_pChuteSenseEdit[1][unitIndex]->setMinMax(this, 0-stand, ARITH_BUD_MAX_SENSE-stand, -1, &struCnfp.struUnitSens[struGsh.nLayer][1+struGsh.nView][unitIndex][1].bud[struGsh.nArithIndex-ARITH_BUD]);

                m_pChuteSenseEdit[0][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][1].bud[struGsh.nArithIndex-ARITH_BUD]));
                m_pChuteSenseEdit[1][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][1+struGsh.nView][unitIndex][1].bud[struGsh.nArithIndex-ARITH_BUD]));

            }
            else
            {
                m_pChuteSenseEdit[0][unitIndex]->setMinMax(this, 0-stand, ARITH_BUD_MAX_SENSE-stand, -1, &struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].bud[struGsh.nArithIndex-ARITH_BUD]);
                m_pChuteSenseEdit[1][unitIndex]->setMinMax(this, 0-stand, ARITH_BUD_MAX_SENSE-stand, -1, &struCnfp.struUnitSens[struGsh.nLayer][1+struGsh.nView][unitIndex][0].bud[struGsh.nArithIndex-ARITH_BUD]);

                m_pChuteSenseEdit[0][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].bud[struGsh.nArithIndex-ARITH_BUD]));
                m_pChuteSenseEdit[1][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][1+struGsh.nView][unitIndex][0].bud[struGsh.nArithIndex-ARITH_BUD]));

            }
        }
        else
        {
            if(struGsh.nView%2)
            {
                ui->label_front->hide();
                ui->label_rear->show();
            }
            else
            {
               ui->label_front->show();
               ui->label_rear->hide();
            }

            m_pChuteTitle[unitIndex]->show();
            m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->show();

            if(groupInfo.nUnitGupSplice[i] == 2)
            {
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setMinMax(this, 0-stand, ARITH_BUD_MAX_SENSE-stand, -1, &struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][1].bud[struGsh.nArithIndex-ARITH_BUD]);
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][1].bud[struGsh.nArithIndex-ARITH_BUD]));

            }
            else
            {
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setMinMax(this, 0-stand, ARITH_BUD_MAX_SENSE-stand, -1, &struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].bud[struGsh.nArithIndex-ARITH_BUD]);
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].bud[struGsh.nArithIndex-ARITH_BUD]));

            }
        }
    }
}

void BudSenseSetWidget::sNameChanged()
{
    memset(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struBud[struGsh.nArithIndex-ARITH_BUD].sName, 0, sizeof(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struBud[struGsh.nArithIndex-ARITH_BUD].sName));

    const char *strName = ui->sNamelineEdit->text().toUtf8();
    memcpy(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struBud[struGsh.nArithIndex-ARITH_BUD].sName, strName, strlen(strName));
}









































