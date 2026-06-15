#include "whitepeanutsensesetwidget.h"
#include "ui_whitepeanutsensesetwidget.h"

WhitePeanutSenseSetWidget::WhitePeanutSenseSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::WhitePeanutSenseSetWidget)
{
    ui->setupUi(this);
    m_pChuteTitle[0] = ui->peanutlbl1;
    m_pChuteTitle[1] = ui->peanutlbl2;
    m_pChuteTitle[2] = ui->peanutlbl3;
    m_pChuteTitle[3] = ui->peanutlbl4;
    m_pChuteTitle[4] = ui->peanutlbl5;
    m_pChuteTitle[5] = ui->peanutlbl6;
    m_pChuteTitle[6] = ui->peanutlbl7;
    m_pChuteTitle[7] = ui->peanutlbl8;
    m_pChuteTitle[8] = ui->peanutlbl9;
    m_pChuteTitle[9] = ui->peanutlbl10;
    m_pChuteTitle[10] = ui->peanutlbl11;
    m_pChuteTitle[11] = ui->peanutlbl12;

    m_pChuteSenseEdit[0][0] = ui->peanutEdit1;
    m_pChuteSenseEdit[0][1] = ui->peanutEdit2;
    m_pChuteSenseEdit[0][2] = ui->peanutEdit3;
    m_pChuteSenseEdit[0][3] = ui->peanutEdit4;
    m_pChuteSenseEdit[0][4] = ui->peanutEdit5;
    m_pChuteSenseEdit[0][5] = ui->peanutEdit6;
    m_pChuteSenseEdit[0][6] = ui->peanutEdit7;
    m_pChuteSenseEdit[0][7] = ui->peanutEdit8;
    m_pChuteSenseEdit[0][8] = ui->peanutEdit9;
    m_pChuteSenseEdit[0][9] = ui->peanutEdit10;
    m_pChuteSenseEdit[0][10] = ui->peanutEdit11;
    m_pChuteSenseEdit[0][11] = ui->peanutEdit12;

    m_pChuteSenseEdit[1][0] = ui->peanutEdit1_1;
    m_pChuteSenseEdit[1][1] = ui->peanutEdit1_2;
    m_pChuteSenseEdit[1][2] = ui->peanutEdit1_3;
    m_pChuteSenseEdit[1][3] = ui->peanutEdit1_4;
    m_pChuteSenseEdit[1][4] = ui->peanutEdit1_5;
    m_pChuteSenseEdit[1][5] = ui->peanutEdit1_6;
    m_pChuteSenseEdit[1][6] = ui->peanutEdit1_7;
    m_pChuteSenseEdit[1][7] = ui->peanutEdit1_8;
    m_pChuteSenseEdit[1][8] = ui->peanutEdit1_9;
    m_pChuteSenseEdit[1][9] = ui->peanutEdit1_10;
    m_pChuteSenseEdit[1][10] = ui->peanutEdit1_11;
    m_pChuteSenseEdit[1][11] = ui->peanutEdit1_12;

    m_pGroupLbl = ui->label_group;

    connect(ui->sNamelineEdit,SIGNAL(editingFinished()),this,SLOT(sNameChanged()));
}

WhitePeanutSenseSetWidget::~WhitePeanutSenseSetWidget()
{
    delete ui;
}

void WhitePeanutSenseSetWidget::showPage(bool bshow)
{
    if(bshow)
    {
        updateGroupLabelShow();
        updateChuteState();

    }
}

void WhitePeanutSenseSetWidget::receiveMsg(int msg1, int msg2, int msg3)
{

    if(msg1 == MSG_DATA_CHANGED)
    {
        setAndSendParams();
    }
}

void WhitePeanutSenseSetWidget::retranslateUiWidget()
{
    ui->senseNameLabel->setText(g_myLan().impurity_name);
    ui->groupBox_8->setTitle(g_myLan().sense_up);
    ui->standard_titlelbl_3->setText(g_myLan().standard);
}

void WhitePeanutSenseSetWidget::updateChuteState()
{
    stru_iden_group_info groupInfo;
    stu_group_identify groupIde;
    memcpy(&groupInfo, &struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].stuIdenGupInfo[struGsh.nMatGroup], sizeof(groupInfo));
    memcpy(&groupIde, &struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup], sizeof(groupIde));

    QByteArray titleArr(groupIde.struWhitePeanut.sName);
    ui->sNamelineEdit->setText(QString::fromUtf8(titleArr));
    ui->sNamelineEdit->setType(textType);

    ui->senseUpEdit->setMinMax(this,1,ARITH_WHITE_PEANUT_MAX_SENSE, 100, &struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struWhitePeanut.nSensUp);
    ui->senseUpEdit->setText(QString("%1").arg(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struWhitePeanut.nSensUp));


    int standardSens;
    standardSens = groupIde.struWhitePeanut.nSensLow;
    ui->standardlbl_3->setText(IntToQString(standardSens));

    for(int i = 0; i < MAX_VIEW_UNIT_USE; i++ )
    {
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
    for(int i = 0;i < tempRepeatCondition;i++)
    {
        if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_SECOND_ALIKE_FIRST)
            unitIndex = i;//算法配置不分次
        else
            unitIndex = groupInfo.nUnitGupId[i];//视链上第几个相机

        if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT)
        {
            ui->label_front_3->show();
            ui->label_rear_3->show();
            m_pChuteTitle[unitIndex]->show();
            m_pChuteSenseEdit[0][unitIndex]->show();
            m_pChuteSenseEdit[1][unitIndex]->show();

            if(groupInfo.nUnitGupSplice[i] == 2)
            {
                m_pChuteSenseEdit[0][unitIndex]->setMinMax(this,0-stand,ARITH_WHITE_PEANUT_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].whitePeanut);
                m_pChuteSenseEdit[1][unitIndex]->setMinMax(this,0-stand,ARITH_WHITE_PEANUT_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][1+struGsh.nView][unitIndex][0].whitePeanut);

                m_pChuteSenseEdit[0][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].whitePeanut));
                m_pChuteSenseEdit[1][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][1+struGsh.nView][unitIndex][0].whitePeanut));
            }
            else
            {
                m_pChuteSenseEdit[0][unitIndex]->setMinMax(this,0-stand,ARITH_WHITE_PEANUT_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].whitePeanut);
                m_pChuteSenseEdit[1][unitIndex]->setMinMax(this,0-stand,ARITH_WHITE_PEANUT_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][1+struGsh.nView][unitIndex][0].whitePeanut);

                m_pChuteSenseEdit[0][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].whitePeanut));
                m_pChuteSenseEdit[1][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][1+struGsh.nView][unitIndex][0].whitePeanut));
            }

        }else{
            if(struGsh.nView%2)
            {
                ui->label_front_3->hide();
                ui->label_rear_3->show();
            }
            else
            {
                ui->label_front_3->show();
                ui->label_rear_3->hide();
            }
            m_pChuteTitle[unitIndex]->show();
            m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->show();

            if(groupInfo.nUnitGupSplice[i] == 2)
            {
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setMinMax(this,0-stand,ARITH_WHITE_PEANUT_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].whitePeanut);
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].whitePeanut));
            }
            else
            {
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setMinMax(this,0-stand,ARITH_WHITE_PEANUT_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].whitePeanut);
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].whitePeanut));
            }
        }
    }
}

void WhitePeanutSenseSetWidget::sNameChanged()
{
    memset(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struWhitePeanut.sName,0,sizeof(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struWhitePeanut.sName));
    const char *strName = ui->sNamelineEdit->text().toUtf8();
    memcpy(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struWhitePeanut.sName,strName,strlen(strName));
}
