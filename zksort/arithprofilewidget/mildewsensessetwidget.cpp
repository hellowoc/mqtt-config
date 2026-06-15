#include "mildewsensessetwidget.h"
#include "ui_mildewsensessetwidget.h"
#include <QButtonGroup>

MildewSensesSetWidget::MildewSensesSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::MildewSensesSetWidget)
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

    m_pModeBtnGroup = new QButtonGroup(this);
    m_pModeBtnGroup->addButton(ui->brightModeBtn,0);// 选亮
    m_pModeBtnGroup->addButton(ui->darkModeBtn,1);// 选暗

    for(int i = 0; i<2; i++ )
    {
        m_pModeBtnGroup->button(i)->setCheckable(true);
        connect(m_pModeBtnGroup->button(i),SIGNAL(clicked(bool)),this,SLOT(modeBtnClicked(bool)));
    }

    m_pTypeBtnGroup = new QButtonGroup(this);
    m_pTypeBtnGroup->addButton(ui->heaveBtn,0);
    m_pTypeBtnGroup->addButton(ui->lightBtn,1);
    m_pTypeBtnGroup->addButton(ui->allBtn,2);
    for(int i = 0;i < 3;i++){
        m_pTypeBtnGroup->button(i)->setCheckable(true);
        connect(m_pTypeBtnGroup->button(i),SIGNAL(clicked(bool)),this,SLOT(typeBtnClicked(bool)));
    }

    connect(ui->sNamelineEdit_4,SIGNAL(editingFinished()),this,SLOT(sNameChanged()));
}

MildewSensesSetWidget::~MildewSensesSetWidget()
{
    delete ui;

}

void MildewSensesSetWidget::showPage(bool bshow)
{
    if(bshow)
    {
        updateGroupLabelShow();
        updateChuteState();
        updateModeBtnState();
        updateTypeBtnState();

        if(struCnfg.stuProfileIndex[struCnfg.nProfile].nProfileLock == 1 )
        {
            ui->groupBox_8->hide();
            ui->groupBox_9->hide();
            ui->groupBox_10->hide();
        }
        else
        {
            ui->groupBox_8->show();
            ui->groupBox_9->show();
            ui->groupBox_10->show();
        }
    }
}

void MildewSensesSetWidget::receiveMsg(int msg1, int msg2, int msg3)
{

    if(msg1 == MSG_DATA_CHANGED){
        if(msg2 < MAX_VIEW_UNIT_USE*2){

        }else{
            switch (msg2) {
            case 40:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struMildew.nLimit = m_limitInt;
                if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT)
                {
                    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView+1][struGsh.nMatGroup].struMildew.nLimit = m_limitInt;
                }
                break;
            case 41:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struMildew.nPercent = m_percentInt;
                if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT)
                {
                    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView+1][struGsh.nMatGroup].struMildew.nPercent = m_percentInt;
                }
                break;
            default:
                break;
            }
        }
        setAndSendParams();
    }
}

void MildewSensesSetWidget::retranslateUiWidget()
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
    ui->groupBox_11->setTitle(g_myLan().type);
    ui->heaveBtn->setText(g_myLan().heavy+g_myLan().arith_mildew);
    ui->lightBtn->setText(g_myLan().light+g_myLan().arith_mildew);
    ui->allBtn->setText(g_myLan().all+g_myLan().arith_mildew);

}

void MildewSensesSetWidget::updateChuteState()
{
    stru_iden_group_info groupInfo;
    stu_group_identify groupIde;
    memcpy(&groupInfo, &struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].stuIdenGupInfo[struGsh.nMatGroup], sizeof(groupInfo));
    memcpy(&groupIde, &struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup], sizeof(groupIde));

    QByteArray titleArr(groupIde.struMildew.sName);
    ui->sNamelineEdit_4->setText(QString::fromUtf8(titleArr));
    ui->sNamelineEdit_4->setType(textType);

    int standardSens;
    standardSens = groupIde.struMildew.nSens;
    ui->standardlbl_3->setText(IntToQString(standardSens));

    for(int i = 0; i < MAX_VIEW_UNIT_USE; i++ )
    {
        m_pChuteTitle[i]->hide();
        m_pChuteSenseEdit[0][i]->hide();
        m_pChuteSenseEdit[1][i]->hide();
    }

    m_limitInt = struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struMildew.nLimit;
    ui->limitEdit->setMinMax(this, 1, 1023, 40, &m_limitInt);
    ui->limitEdit->setText(QString::number(m_limitInt));

    m_percentInt = struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struMildew.nPercent;
    ui->purityEdit->setMinMax(this, 1, 1023, 41, &m_percentInt);
    ui->purityEdit->setText(QString::number(m_percentInt));

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
                m_pChuteSenseEdit[0][unitIndex]->setMinMax(this,0-stand,ARITH_MILDEW_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].mildew);
                m_pChuteSenseEdit[1][unitIndex]->setMinMax(this,0-stand,ARITH_MILDEW_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][1+struGsh.nView][unitIndex][0].mildew);

                m_pChuteSenseEdit[0][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].mildew));
                m_pChuteSenseEdit[1][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][1+struGsh.nView][unitIndex][0].mildew));
            }
            else
            {
                m_pChuteSenseEdit[0][unitIndex]->setMinMax(this,0-stand,ARITH_MILDEW_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].mildew);
                m_pChuteSenseEdit[1][unitIndex]->setMinMax(this,0-stand,ARITH_MILDEW_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][1+struGsh.nView][unitIndex][0].mildew);

                m_pChuteSenseEdit[0][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].mildew));
                m_pChuteSenseEdit[1][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][1+struGsh.nView][unitIndex][0].mildew));
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
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setMinMax(this,0-stand,ARITH_MILDEW_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].mildew);
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].mildew));
            }
            else
            {
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setMinMax(this,0-stand,ARITH_MILDEW_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].mildew);
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].mildew));
            }
        }
    }
}

void MildewSensesSetWidget::updateModeBtnState()
{
    if( struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struMildew.nMark == 1)
    {
        m_pModeBtnGroup->button(1)->setChecked(true);
    }
    else
    {
        m_pModeBtnGroup->button(0)->setChecked(true);
    }
}

void MildewSensesSetWidget::onModeChange(int mode)
{
    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struMildew.nMark = mode;
    if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT)
    {
        struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView+1][struGsh.nMatGroup].struMildew.nMark = mode;
    }
}

void MildewSensesSetWidget::modeBtnClicked(bool)
{
    onModeChange(m_pModeBtnGroup->id((QPushButton*)sender()));
    updateChuteState();
    setAndSendParams();
}

void MildewSensesSetWidget::updateTypeBtnState()
{
    for(int i = 0;i< 3;i++){
        m_pTypeBtnGroup->button(i)->setChecked(i == struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struMildew.nType);
    }
}

void MildewSensesSetWidget::onTypeChange(int type)
{
    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struMildew.nType = type;
    if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT)
    {
        struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView+1][struGsh.nMatGroup].struMildew.nType = type;
    }
}

void MildewSensesSetWidget::typeBtnClicked(bool)
{
    onTypeChange(m_pTypeBtnGroup->id((QPushButton*)sender()));
    updateChuteState();
    setAndSendParams();
}

void MildewSensesSetWidget::sNameChanged()
{
    memset(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struMildew.sName,0,sizeof(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struMildew.sName));
    const char *strName = ui->sNamelineEdit_4->text().toUtf8();
    memcpy(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struMildew.sName,strName,strlen(strName));
}
