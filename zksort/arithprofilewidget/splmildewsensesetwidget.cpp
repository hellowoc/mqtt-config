#include "splmildewsensesetwidget.h"
#include "ui_splmildewsensesetwidget.h"

splMildewSenseSetWidget::splMildewSenseSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::splMildewSenseSetWidget)
{
    ui->setupUi(this);
    m_pChuteTitle[0] = ui->mildewlbl1;
    m_pChuteTitle[1] = ui->mildewlbl2;
    m_pChuteTitle[2] = ui->mildewlbl3;
    m_pChuteTitle[3] = ui->mildewlbl4;
    m_pChuteTitle[4] = ui->mildewlbl5;
    m_pChuteTitle[5] = ui->mildewlbl6;
    m_pChuteTitle[6] = ui->mildewlbl7;
    m_pChuteTitle[7] = ui->mildewlbl8;
    m_pChuteTitle[8] = ui->mildewlbl9;
    m_pChuteTitle[9] = ui->mildewlbl10;
    m_pChuteTitle[10] = ui->mildewlbl11;
    m_pChuteTitle[11] = ui->mildewlbl12;

    m_pChuteSenseEdit[0][0] = ui->mildewEdit1;
    m_pChuteSenseEdit[0][1] = ui->mildewEdit2;
    m_pChuteSenseEdit[0][2] = ui->mildewEdit3;
    m_pChuteSenseEdit[0][3] = ui->mildewEdit4;
    m_pChuteSenseEdit[0][4] = ui->mildewEdit5;
    m_pChuteSenseEdit[0][5] = ui->mildewEdit6;
    m_pChuteSenseEdit[0][6] = ui->mildewEdit7;
    m_pChuteSenseEdit[0][7] = ui->mildewEdit8;
    m_pChuteSenseEdit[0][8] = ui->mildewEdit9;
    m_pChuteSenseEdit[0][9] = ui->mildewEdit10;
    m_pChuteSenseEdit[0][10] = ui->mildewEdit11;
    m_pChuteSenseEdit[0][11] = ui->mildewEdit12;

    m_pChuteSenseEdit[1][0] = ui->mildewEdit1_1;
    m_pChuteSenseEdit[1][1] = ui->mildewEdit1_2;
    m_pChuteSenseEdit[1][2] = ui->mildewEdit1_3;
    m_pChuteSenseEdit[1][3] = ui->mildewEdit1_4;
    m_pChuteSenseEdit[1][4] = ui->mildewEdit1_5;
    m_pChuteSenseEdit[1][5] = ui->mildewEdit1_6;
    m_pChuteSenseEdit[1][6] = ui->mildewEdit1_7;
    m_pChuteSenseEdit[1][7] = ui->mildewEdit1_8;
    m_pChuteSenseEdit[1][8] = ui->mildewEdit1_9;
    m_pChuteSenseEdit[1][9] = ui->mildewEdit1_10;
    m_pChuteSenseEdit[1][10] = ui->mildewEdit1_11;
    m_pChuteSenseEdit[1][11] = ui->mildewEdit1_12;

    m_pGroupLbl = ui->label_group;

    connect(ui->sNamelineEdit_4,SIGNAL(editingFinished()),this,SLOT(sNameChanged()));

}

splMildewSenseSetWidget::~splMildewSenseSetWidget()
{
    delete ui;
}

void splMildewSenseSetWidget::showPage(bool bshow)
{
    if(bshow)
    {
        updateGroupLabelShow();
        updateChuteState();

        ui->groupBox_8->hide();
        ui->groupBox_9->hide();
        ui->groupBox_10->hide();

    }
}

void splMildewSenseSetWidget::receiveMsg(int msg1, int msg2, int msg3)
{

    if(msg1 == MSG_DATA_CHANGED)
    {
        setAndSendParams();
    }
}

void splMildewSenseSetWidget::retranslateUiWidget()
{
    ui->senseNameLabel->setText(g_myLan().impurity_name);
    ui->groupBox_8->setTitle(g_myLan().purity);
    ui->groupBox_9->setTitle(g_myLan().mode);
    ui->groupBox_10->setTitle(g_myLan().params_limit);
    ui->standard_titlelbl_3->setText(g_myLan().standard);
    ui->label_front_3->setText(g_myLan().front);
    ui->label_rear_3->setText(g_myLan().rear);
    ui->darkModeBtn->setText(g_myLan().ids_selectDark);
    ui->brightModeBtn->setText(g_myLan().ids_selectBright);

    ui->groupBox_8->hide();
    ui->groupBox_9->hide();
    ui->groupBox_10->hide();
    ui->darkModeBtn->hide();
    ui->brightModeBtn->hide();
}

void splMildewSenseSetWidget::updateChuteState()
{
    stru_iden_group_info groupInfo;
    stu_group_identify groupIde;
    memcpy(&groupInfo, &struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].stuIdenGupInfo[struGsh.nMatGroup], sizeof(groupInfo));
    memcpy(&groupIde, &struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup], sizeof(groupIde));

    QByteArray titleArr(groupIde.struSplMildew.sName);
    ui->sNamelineEdit_4->setText(QString::fromUtf8(titleArr));
    ui->sNamelineEdit_4->setType(textType);

    int standardSens;
    standardSens = groupIde.struSplMildew.nSens;
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
                m_pChuteSenseEdit[0][unitIndex]->setMinMax(this,0-stand,ARITH_SPL_MILDEW_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].splMildew);
                m_pChuteSenseEdit[1][unitIndex]->setMinMax(this,0-stand,ARITH_SPL_MILDEW_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][1+struGsh.nView][unitIndex][0].splMildew);

                m_pChuteSenseEdit[0][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].splMildew));
                m_pChuteSenseEdit[1][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][1+struGsh.nView][unitIndex][0].splMildew));
            }
            else
            {
                m_pChuteSenseEdit[0][unitIndex]->setMinMax(this,0-stand,ARITH_SPL_MILDEW_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].splMildew);
                m_pChuteSenseEdit[1][unitIndex]->setMinMax(this,0-stand,ARITH_SPL_MILDEW_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][1+struGsh.nView][unitIndex][0].splMildew);

                m_pChuteSenseEdit[0][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].splMildew));
                m_pChuteSenseEdit[1][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][1+struGsh.nView][unitIndex][0].splMildew));
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
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setMinMax(this,0-stand,ARITH_SPL_MILDEW_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].splMildew);
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].splMildew));
            }
            else
            {
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setMinMax(this,0-stand,ARITH_SPL_MILDEW_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].splMildew);
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].splMildew));
            }
        }
    }
}

void splMildewSenseSetWidget::sNameChanged()
{
    memset(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSplMildew.sName,0,sizeof(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSplMildew.sName));
    const char *strName = ui->sNamelineEdit_4->text().toUtf8();
    memcpy(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struSplMildew.sName,strName,strlen(strName));
}
