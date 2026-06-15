#include "colorscalesensesetwidget.h"
#include "ui_colorscalesensesetwidget.h"
#include "globalflow.h"
const int senseMax[5] = {ARITH_GREY_MAX_SENS,ARITH_GREY_MAX_SENS,ARITH_COLOR_MAX_SENS,ARITH_COLOR_MAX_SENS,ARITH_GREY_MAX_SENS};
const int senseMin[5] = {0,0,0,0,0};

ColorScaleSenseSetWidget::ColorScaleSenseSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::ColorScaleSenseSetWidget)
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

    m_pGroupLbl = ui->label_group;
    connect(ui->sNamelineEdit,SIGNAL(valueSetSignal()),this,SLOT(sNameChanged()));
}

ColorScaleSenseSetWidget::~ColorScaleSenseSetWidget()
{
    delete ui;
}

void ColorScaleSenseSetWidget::showPage(bool bshow)
{
    if(bshow){
        updateGroupLabelShow();

        updateChuteState();
        updateValue();
        //进入页面默认隐藏不常用配置
    }
}

void ColorScaleSenseSetWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    const stru_iden_group_info &groupInfo = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].stuIdenGupInfo[struGsh.nMatGroup];
    if(msg1 == MSG_DATA_CHANGED){
        if(msg2 < MAX_VIEW_UNIT_USE*2){

        }else{
            switch (msg2) {
            case 40:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorScale.nLimit = m_scaleLimit;

                ui->arithLimitEdit->setText(QString::number(m_scaleLimit));
                ui->arithLimitEdit->setMinMax(this,1,1023,41,&m_scaleLimit);

                break;
            case 41:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorScale.nLimit = m_scaleLimit;
                break;
            default:
                break;
            }
        }
        setAndSendParams();
    }
}

void ColorScaleSenseSetWidget::retranslateUiWidget()
{
    ui->label->setText(g_myLan().impurity_name);

    ui->groupBox_4->setTitle(g_myLan().basic_set);
    ui->standard_titlelbl->setText(g_myLan().standard);
    ui->label_front->setText(g_myLan().front);
    ui->label_rear->setText(g_myLan().rear);
    ui->label_7->setText(g_myLan().params_limit);
    ui->label_4->setText(g_myLan().scale_index);
}

void ColorScaleSenseSetWidget::updateValue()//更新保留算法相关的数据
{
    ui->comboBox_index->clear();
    QStringList arithIndex;
    arithIndex<<g_myLan().arith_gray_a<<g_myLan().arith_gray_b<<g_myLan().arith_discolor_a<<g_myLan().arith_discolor_b
             <<"SVMA"<<"SVMB"<<g_myLan().hsv_s<<g_myLan().hsv_h;

    stru_color_scale color_scale;
    memcpy(&color_scale,&struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorScale,sizeof(stru_color_scale));

    ui->comboBox_index->addItems(arithIndex);
    ui->comboBox_index->setCurrentIndex(color_scale.nSenIdx);

    m_scaleLimit = color_scale.nLimit;
    ui->arithLimitEdit->setText(QString::number(m_scaleLimit));
    ui->arithLimitEdit->setMinMax(this,1,1023,41,&m_scaleLimit);
}

void ColorScaleSenseSetWidget::updateChuteState()//更新料槽的配置精度值
{
    stru_iden_group_info groupInfo;
    static stu_group_identify groupIde;
    memcpy(&groupInfo,&struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].stuIdenGupInfo[struGsh.nMatGroup],sizeof(groupInfo));
    memcpy(&groupIde,&struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup],sizeof(groupIde));

    QByteArray titleArr(groupIde.struColorScale.sName);
    ui->sNamelineEdit->setText(QString::fromUtf8(titleArr));

    ui->sNamelineEdit->setType(textType);
    int standardSens;
    standardSens = groupIde.struColorScale.nSen;

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
                m_pChuteSenseEdit[0][unitIndex]->setMinMax(this,0-stand,ARITH_SCALE_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][1].scale);
                m_pChuteSenseEdit[1][unitIndex]->setMinMax(this,0-stand,ARITH_SCALE_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][unitIndex][1].scale);
                m_pChuteSenseEdit[0][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][1].scale));
                m_pChuteSenseEdit[1][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][unitIndex][1].scale));
            }else{
                m_pChuteSenseEdit[0][unitIndex]->setMinMax(this,0-stand,ARITH_SCALE_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].scale);
                m_pChuteSenseEdit[1][unitIndex]->setMinMax(this,0-stand,ARITH_SCALE_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][unitIndex][0].scale);
                m_pChuteSenseEdit[0][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].scale));
                m_pChuteSenseEdit[1][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][unitIndex][0].scale));
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
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setMinMax(this,0-stand,ARITH_SCALE_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][1].scale);
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][1].scale));
            }else{
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setMinMax(this,0-stand,ARITH_SCALE_MAX_SENSE-stand,-1,&struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].scale);
                m_pChuteSenseEdit[struGsh.nView%2][unitIndex]->setText(IntToQString(struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][unitIndex][0].scale));
            }

        }
    }
}



void ColorScaleSenseSetWidget::sNameChanged()
{
    memset(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorScale.sName,0,sizeof(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorScale.sName));

    QString name = ui->sNamelineEdit->text();
    QByteArray namearr = name.toUtf8();
    const char *strName = namearr.data();
    memcpy(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorScale.sName,strName,strlen(strName));
}



void ColorScaleSenseSetWidget::on_comboBox_index_activated(int index)
{
    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struColorScale.nSenIdx = index;
    setAndSendParams();
}
