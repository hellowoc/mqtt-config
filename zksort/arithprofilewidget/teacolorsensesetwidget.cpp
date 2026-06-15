#include "teacolorsensesetwidget.h"
#include "ui_teacolorsensesetwidget.h"
enum {
    tc= 10,
    ec,ac,type_sense
};
TeaColorSenseSetWidget::TeaColorSenseSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::TeaColorSenseSetWidget)
{
    ui->setupUi(this);
    m_pModeBtnGroup = new QButtonGroup(this);
    m_pModeBtnGroup->addButton(ui->darkBtn,0);
    m_pModeBtnGroup->addButton(ui->lightBtn,1);


    connect(ui->sNamelineEdit,SIGNAL(valueSetSignal()),this,SLOT(sNameChanged()));
    for(int i = 0;i < 2;i++){
        m_pModeBtnGroup->button(i)->setCheckable(true);
        connect(m_pModeBtnGroup->button(i),SIGNAL(clicked(bool)),this,SLOT(modeBtnClicked(bool)));
    }
}

TeaColorSenseSetWidget::~TeaColorSenseSetWidget()
{
    delete ui;
}

void TeaColorSenseSetWidget::showPage(bool bshow)
{
    if(bshow){
        updateValue();
        updateSizeState();
        updateModeBtnState();
    }
}

void TeaColorSenseSetWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    const stru_iden_group_info &groupInfo = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].stuIdenGupInfo[struGsh.nMatGroup];

    int index = struGsh.nArithIndex-TEA_ARITH_COLOR;
    if(msg1 == MSG_DATA_CHANGED){
        switch (msg2) {
            case 40:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struTeaColor[index].nRow = m_rowInt;
                if(m_percentInt > m_rowInt*m_rowInt)
                {
                    m_percentInt = m_rowInt*m_rowInt;
                    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struTeaColor[index].nPercent = m_percentInt;
                }
                ui->percentEdit->setText(QString::number(m_percentInt));
                ui->percentEdit->setMinMax(this,1,m_rowInt*m_rowInt,41,&m_percentInt);

                break;
            case 41:
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struTeaColor[index].nPercent= m_percentInt;
                break;
            case 42:
                break;
            default:
                break;
            }

        setAndSendParams();
    }
}

void TeaColorSenseSetWidget::retranslateUiWidget()
{
    ui->groupBox_4->setTitle(g_myLan().impurity_name);
    ui->groupBox_7->setTitle(g_myLan().basic_set);
    ui->label_2->setText(g_myLan().ids_size);
    ui->label_3->setText(g_myLan().ids_purity);
    ui->lightBtn->setText(g_myLan().ids_selectBright);
    ui->darkBtn->setText(g_myLan().ids_selectDark);
    ui->label_keepSense->setText(g_myLan().ids_select);
    ui->label_5->setText(g_myLan().edge_cut);
    ui->label_4->setText(g_myLan().arith_suppress_large);
}
void TeaColorSenseSetWidget::updateModeBtnState()
{
    int index = struGsh.nArithIndex-TEA_ARITH_COLOR;
    if(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struTeaColor[index].nMode == 0){
        m_pModeBtnGroup->button(0)->setChecked(true);
    }else{
        m_pModeBtnGroup->button(1)->setChecked(true);
    }
}
void TeaColorSenseSetWidget::onModeChange(int mode)
{
    int index = struGsh.nArithIndex-TEA_ARITH_COLOR;
    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struTeaColor[index].nMode = mode;
}

void TeaColorSenseSetWidget::updateValue()//更新比例算法相关控件的显示和隐藏
{
    int layer = struGsh.nLayer;
    int view = struGsh.nView;
    int group = struGsh.nMatGroup;
    int index = struGsh.nArithIndex-TEA_ARITH_COLOR;

    stu_tea_color &tea_color= struCnfp.struGroupIdentify[layer][view][group].struTeaColor[index];

    QByteArray titleArr(tea_color.sName);
    ui->sNamelineEdit->setText(QString::fromUtf8(titleArr));
    ui->sNamelineEdit->setType(textType);

    ui->tcEdit->setText(FloatToPercentQString(tea_color.tc));
    ui->ecEdit->setText(FloatToPercentQString(tea_color.ec));
    ui->acEdit->setText(FloatToPercentQString(tea_color.ac));
    ui->senseEdit->setText(FloatToPercentQString(tea_color.sense));
    ui->tcEdit->setFloatMinMax(this,0,100,tc,&tea_color.tc,0);
    ui->ecEdit->setFloatMinMax(this,-128,127,ec,&tea_color.ec,0);
    ui->acEdit->setFloatMinMax(this,-128,127,ac,&tea_color.ac,0);
    ui->senseEdit->setFloatMinMax(this,0,100,type_sense,&tea_color.sense,0);
    ui->edgeEdit->setMinMax(this,0,7,-1,&tea_color.nEdge);
    ui->edgeEdit->setText(QString::number(tea_color.nEdge));
    ui->checkBoxInhebit->setChecked(tea_color.nInhibit == 0);


    ui->sizeGroupbox->setVisible(struGsh.nArithIndex != TEA_ARITH_COLOR_5);
    ui->sizeGroupbox_2->setVisible(struGsh.nArithIndex != TEA_ARITH_COLOR_5);
}

void TeaColorSenseSetWidget::updateSizeState()
{
    int layer = struGsh.nLayer;
    int view = struGsh.nView;
    int group = struGsh.nMatGroup;
    int index = struGsh.nArithIndex-TEA_ARITH_COLOR;
    stu_tea_color &tea_color= struCnfp.struGroupIdentify[layer][view][group].struTeaColor[index];
    int maxPercent = tea_color.nRow*tea_color.nRow;
    m_rowInt = tea_color.nRow;
    m_percentInt = tea_color.nPercent;
    ui->rowEdit->setText(QString::number(m_rowInt));
    ui->rowEdit->setMinMax(this,1,32,40,&m_rowInt);
    ui->percentEdit->setText(QString::number(m_percentInt));
    ui->percentEdit->setMinMax(this,1,maxPercent,41,&m_percentInt);
}

void TeaColorSenseSetWidget::sNameChanged()
{
    int index = struGsh.nArithIndex-TEA_ARITH_COLOR;
    memset(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struTeaColor[index].sName,0,sizeof(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struTeaColor[index].sName));

    QString name = ui->sNamelineEdit->text();
    QByteArray namearr = name.toUtf8();
    const char *strName = namearr.data();
    memcpy(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struTeaColor[index].sName,strName,strlen(strName));
}

void TeaColorSenseSetWidget::modeBtnClicked(bool check)//算法的选亮选暗切换
{
    if(check)
        onModeChange(m_pModeBtnGroup->id((QPushButton*)sender()));
    setAndSendParams();
}

void TeaColorSenseSetWidget::on_checkBoxInhebit_clicked()
{
    int layer = struGsh.nLayer;
    int view = struGsh.nView;
    int group = struGsh.nMatGroup;
    int index = struGsh.nArithIndex-TEA_ARITH_COLOR;
    stu_tea_color &tea_color= struCnfp.struGroupIdentify[layer][view][group].struTeaColor[index];
    tea_color.nInhibit = tea_color.nInhibit == 1?0:1;
    setAndSendParams();
}
