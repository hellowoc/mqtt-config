#include "colorvaluesensesetwidget.h"
#include "ui_colorvaluesensesetwidget.h"
#include <QButtonGroup>

ColorValueSenseSetWidget::ColorValueSenseSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::ColorValueSenseSetWidget)
{
    ui->setupUi(this);
    m_pChuteTitle[0] = ui->valueLabel1;
    m_pChuteTitle[1] = ui->valueLabel2;
    m_pChuteTitle[2] = ui->valueLabel3;
    m_pChuteTitle[3] = ui->valueLabel4;
    m_pChuteTitle[4] = ui->valueLabel5;
    m_pChuteTitle[5] = ui->valueLabel6;
    m_pChuteTitle[6] = ui->valueLabel7;
    m_pChuteTitle[7] = ui->valueLabel8;
    m_pChuteTitle[8] = ui->valueLabel9;
    m_pChuteTitle[9] = ui->valueLabel10;
    m_pChuteTitle[10] = ui->valueLabel11;
    m_pChuteTitle[11] = ui->valueLabel12;

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

    for(int i = 0; i < 2; i++)
    {
        m_pModeBtnGroup->button(i)->setCheckable(true);
        connect(m_pModeBtnGroup->button(i), SIGNAL(clicked(bool)), this, SLOT(modeBtnClicked(bool)));
    }

    connect(ui->sNamelineEdit, SIGNAL(editingFinished()), this, SLOT(sNameChanged()));
}

ColorValueSenseSetWidget::~ColorValueSenseSetWidget()
{
    delete ui;
}

void ColorValueSenseSetWidget::showPage(bool bshow)
{
    if(bshow)
    {
        updateGroupLabelShow();
        updateChuteState();
        updateModeBtnState();
        updateSizeState();
    }
}

void ColorValueSenseSetWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    const stru_iden_group_info &groupInfo = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].stuIdenGupInfo[struGsh.nMatGroup];

    if(msg1 == MSG_DATA_CHANGED)
    {
        if(msg2 < MAX_VIEW_UNIT_USE*2)
        {


        }
        else
        {
            switch (msg2)
            {
            case 40:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorValue.nRow = m_rowInt;
                if(m_percentInt > m_rowInt*m_rowInt)
                {
                    m_percentInt = m_rowInt*m_rowInt;
                    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorValue.nPercent = m_percentInt;
                }

                ui->percentEdit->setText(QString::number(m_percentInt));
                ui->percentEdit->setMinMax(this,1,m_rowInt*m_rowInt,41,&m_percentInt);
                break;
            case 41:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorValue.nPercent = m_percentInt;
                break;
            default:
                break;
            }
        }
        setAndSendParams();
    }
}

void ColorValueSenseSetWidget::retranslateUiWidget()
{
    ui->label->setText(g_myLan().impurity_name);
    ui->groupBox_4->setTitle(g_myLan().basic_set);
    ui->standard_titlelbl->setText(g_myLan().standard);
    ui->label_front->setText(g_myLan().front);
    ui->label_rear->setText(g_myLan().rear);
    ui->lightBtn->setText(g_myLan().ids_selectBright);
    ui->darkBtn->setText(g_myLan().ids_selectDark);
    ui->sizeGroupbox->setTitle(g_myLan().ids_size);
    ui->label_2->setText(g_myLan().ids_size);
    ui->label_3->setText(g_myLan().percent);
}

void ColorValueSenseSetWidget::updateChuteState()
{
    stru_iden_group_info groupInfo;
    stu_group_identify groupIde;
    memcpy(&groupInfo,&struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].stuIdenGupInfo[struGsh.nMatGroup],sizeof(groupInfo));
    memcpy(&groupIde,&struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup],sizeof(groupIde));

    QByteArray titleArr(groupIde.struColorValue.sName);
    ui->sNamelineEdit->setText(QString::fromUtf8(titleArr));
    ui->sNamelineEdit->setType(textType);

    int standardSens;
    if(groupIde.struColorValue.nMode){
        standardSens = groupIde.struColorValue.nSensUp;
    }else{
        standardSens = groupIde.struColorValue.nSensLow;
    }
    ui->standardlbl->setText(IntToQString(standardSens));
    for(int i = 0;i < MAX_VIEW_UNIT_USE;i++){
        m_pChuteTitle[i]->hide();
        m_pChuteSenseEdit[0][i]->hide();
        m_pChuteSenseEdit[1][i]->hide();
    }

    int stand = standardSens;
    int tempRepeatCondition = 0;
    if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_SECOND_ALIKE_FIRST){
        tempRepeatCondition = struCnfg.nChuteTotal;
    }else{
        tempRepeatCondition = groupInfo.nGroupUnitTotal;
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
                m_pChuteSenseEdit[0][unitIndex]->setMinMax(this,0-stand,ARITH_VALUE_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][1].value);
                m_pChuteSenseEdit[1][unitIndex]->setMinMax(this,0-stand,ARITH_VALUE_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][1+struGsh.nView][unitIndex][1].value);

                m_pChuteSenseEdit[0][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][1].value));
                m_pChuteSenseEdit[1][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][1+struGsh.nView][unitIndex][1].value));
            }else{
                m_pChuteSenseEdit[0][unitIndex]->setMinMax(this,0-stand,ARITH_VALUE_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].value);
                m_pChuteSenseEdit[1][unitIndex]->setMinMax(this,0-stand,ARITH_VALUE_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][1+struGsh.nView][unitIndex][0].value);

                m_pChuteSenseEdit[0][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].value));
                m_pChuteSenseEdit[1][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][1+struGsh.nView][unitIndex][0].value));
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
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setMinMax(this,0-stand,ARITH_VALUE_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][1].value);
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][1].value));
            }else{
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setMinMax(this,0-stand,ARITH_VALUE_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].value);
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].value));
            }
        }
    }
}

void ColorValueSenseSetWidget::updateModeBtnState()
{
    if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorValue.nMode == 0){
        m_pModeBtnGroup->button(0)->setChecked(true);
    }else{
        m_pModeBtnGroup->button(1)->setChecked(true);
    }
}

void ColorValueSenseSetWidget::updateSizeState()
{
    int layer = struGsh.nLayer;
    int view = struGsh.nView;
    int group = struGsh.nMatGroup;
    stru_color_value  &color_value  = struCnfp.struGroupIdentify[layer][view][group].struColorValue;
    int maxPercent = 0;
    maxPercent = color_value.nRow*color_value.nRow;
    m_rowInt = color_value.nRow;
    m_percentInt = color_value.nPercent;
    ui->rowEdit->setText(QString::number(m_rowInt));
    ui->rowEdit->setMinMax(this,1,32,40,&m_rowInt);
    ui->percentEdit->setText(QString::number(m_percentInt));
    ui->percentEdit->setMinMax(this,1,maxPercent,41,&m_percentInt);
}

void ColorValueSenseSetWidget::onModeChange(int mode)
{
    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorValue.nMode = mode;
}

void ColorValueSenseSetWidget::modeBtnClicked(bool)
{
    onModeChange(m_pModeBtnGroup->id((QPushButton*)sender()));
    updateChuteState();
    setAndSendParams();
}

void ColorValueSenseSetWidget::sNameChanged()
{
    memset(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorValue.sName,0,sizeof(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorValue.sName));

    const char *strName = ui->sNamelineEdit->text().toUtf8();
    memcpy(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorValue.sName,strName,strlen(strName));

}
