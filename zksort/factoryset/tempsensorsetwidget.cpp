#include "tempsensorsetwidget.h"
#include "ui_tempsensorsetwidget.h"

TempSensorSetWidget::TempSensorSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::TempSensorSetWidget)
{
    ui->setupUi(this);

    m_pSensorEdit[0] = ui->lineEdit;
    m_pSensorEdit[1] = ui->lineEdit_2;
    m_pSensorEdit[2] = ui->lineEdit_3;
    m_pSensorEdit[3] = ui->lineEdit_4;
    m_pSensorEdit[4] = ui->lineEdit_5;

    m_pSensorNameEdit[0] = ui->nameEdit1;
    m_pSensorNameEdit[1] = ui->nameEdit2;
    m_pSensorNameEdit[2] = ui->nameEdit3;
    m_pSensorNameEdit[3] = ui->nameEdit4;
    m_pSensorNameEdit[4] = ui->nameEdit5;

    for(int i = 0;i < CTRL_ALARM_MAX_TEMP_SENSOR;i++){
        connect(m_pSensorNameEdit[i],SIGNAL(valueSetSignal()),this,SLOT(sNameChanged()));
        m_pSensorNameEdit[i]->setType(textType);
    }
    m_pBtnGroup = new QButtonGroup(this);
    m_pSignalMap = new QSignalMapper(this);
    m_pBtnGroup->addButton(ui->checkBox,0);
    m_pBtnGroup->addButton(ui->checkBox_2,1);
    m_pBtnGroup->addButton(ui->checkBox_3,2);
    m_pBtnGroup->addButton(ui->checkBox_4,3);
    m_pBtnGroup->addButton(ui->checkBox_5,4);
    for(int i = 0;i < m_pBtnGroup->buttons().count();i++){
        m_pSignalMap->setMapping(m_pBtnGroup->button(i),i);
        connect(m_pBtnGroup->button(i),SIGNAL(clicked()),m_pSignalMap,SLOT(map()));
    }
    m_pBtnGroup->setExclusive(false);
    connect(m_pSignalMap,SIGNAL(mapped(int)),this,SLOT(checkBtnClicked(int)));
}

TempSensorSetWidget::~TempSensorSetWidget()
{
    delete ui;
}

void TempSensorSetWidget::showPage(bool bshow)
{
    if(bshow){
        updateWidget();
    }
}

void TempSensorSetWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){

    }
}

void TempSensorSetWidget::retranslateUiWidget()
{
    for(int i = 0;i < CTRL_ALARM_MAX_TEMP_SENSOR;i++){
          m_pBtnGroup->button(i)->setText(g_myLan().temperature_sensor+QString::number(i+1));
    }
}

void TempSensorSetWidget::checkBtnClicked(int index)
{
    QCheckBox *check;
    check = (qobject_cast<QCheckBox*>)(m_pBtnGroup->button(index));
    if(check->isChecked()){
        struCnfg.struAlarmConfig.nTemptureAlarmEnable[index] = 1;
    }else{
        struCnfg.struAlarmConfig.nTemptureAlarmEnable[index] = 0;
    }
}

void TempSensorSetWidget::sNameChanged()
{
    int index = 0;
    myLineEdit *m_pEdit;
    for(int i = 0;i < CTRL_ALARM_MAX_TEMP_SENSOR;i++){
        m_pEdit = qobject_cast<myLineEdit*>(sender());
        if(m_pEdit == m_pSensorNameEdit[i]){
            index = i;
            break;
        }
    }
    memset(struCnfg.struAlarmConfig.nTemptureAlarmName[index],0,sizeof(struCnfg.struAlarmConfig.nTemptureAlarmName[index]));
    const char *strName = m_pSensorNameEdit[index]->text().toUtf8();
    memcpy(struCnfg.struAlarmConfig.nTemptureAlarmName[index],strName,strlen(strName));
}

void TempSensorSetWidget::updateWidget()
{
    for(int i = 0;i < CTRL_ALARM_MAX_TEMP_SENSOR;i++){
        m_pSensorEdit[i]->setText(QString::number(struCnfg.struAlarmConfig.nTemptureAlarmLimit[i]));
        m_pSensorEdit[i]->setMinMax(this,1,100,-1,&struCnfg.struAlarmConfig.nTemptureAlarmLimit[i]);
        QByteArray titleArr(struCnfg.struAlarmConfig.nTemptureAlarmName[i]);
        m_pSensorNameEdit[i]->setText(QString::fromUtf8(titleArr));
        m_pBtnGroup->button(i)->setChecked(struCnfg.struAlarmConfig.nTemptureAlarmEnable[i] == 1);
    }
}
