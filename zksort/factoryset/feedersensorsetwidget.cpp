#include "feedersensorsetwidget.h"
#include "ui_feedersensorsetwidget.h"

FeederSensorSetWidget::FeederSensorSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::FeederSensorSetWidget)
{
    ui->setupUi(this);
    m_pFeederNameEdit[0] = ui->lineEdit;
    m_pFeederNameEdit[1] = ui->lineEdit_2;
    m_pFeederNameEdit[2] = ui->lineEdit_3;
    m_pFeederNameEdit[3] = ui->lineEdit_4;
    m_pFeederNameEdit[4] = ui->lineEdit_5;
    m_pFeederNameEdit[5] = ui->lineEdit_6;
    m_pFeederNameEdit[6] = ui->lineEdit_7;
    m_pFeederNameEdit[7] = ui->lineEdit_8;

    for(int i = 0;i < CTRL_ALARM_MAX_FEED_SENSOR;i++){
        connect(m_pFeederNameEdit[i],SIGNAL(valueSetSignal()),this,SLOT(sNameChanged()));
        m_pFeederNameEdit[i]->setType(textType);
    }

    m_pBtnGroup = new QButtonGroup(this);
    m_pSignalMap = new QSignalMapper(this);
    m_pBtnGroup->addButton(ui->checkBox,0);
    m_pBtnGroup->addButton(ui->checkBox_2,1);
    m_pBtnGroup->addButton(ui->checkBox_3,2);
    m_pBtnGroup->addButton(ui->checkBox_4,3);
    m_pBtnGroup->addButton(ui->checkBox_5,4);
    m_pBtnGroup->addButton(ui->checkBox_6,5);
    m_pBtnGroup->addButton(ui->checkBox_7,6);
    m_pBtnGroup->addButton(ui->checkBox_8,7);
    m_pBtnGroupPosi = new QButtonGroup(this);
    m_pSignalMapPosi = new QSignalMapper(this);
    m_pBtnGroupPosi->addButton(ui->checkBox_9,0);
    m_pBtnGroupPosi->addButton(ui->checkBox_10,1);
    m_pBtnGroupPosi->addButton(ui->checkBox_11,2);
    m_pBtnGroupPosi->addButton(ui->checkBox_12,3);
    m_pBtnGroupPosi->addButton(ui->checkBox_13,4);
    m_pBtnGroupPosi->addButton(ui->checkBox_14,5);
    m_pBtnGroupPosi->addButton(ui->checkBox_15,6);
    m_pBtnGroupPosi->addButton(ui->checkBox_16,7);
    for(int i = 0;i < CTRL_ALARM_MAX_FEED_SENSOR;i++){
        m_pSignalMapPosi->setMapping(m_pBtnGroupPosi->button(i),i);
        connect(m_pBtnGroupPosi->button(i),SIGNAL(clicked()),m_pSignalMapPosi,SLOT(map()));

        m_pSignalMap->setMapping(m_pBtnGroup->button(i),i);
        connect(m_pBtnGroup->button(i),SIGNAL(clicked()),m_pSignalMap,SLOT(map()));
    }
    m_pBtnGroup->setExclusive(false);
    connect(m_pSignalMap,SIGNAL(mapped(int)),this,SLOT(checkboxClicked(int)));

    m_pBtnGroupPosi->setExclusive(false);
    connect(m_pSignalMapPosi,SIGNAL(mapped(int)),this,SLOT(checkboxPosiClicked(int)));
}

FeederSensorSetWidget::~FeederSensorSetWidget()
{
    delete ui;
}

void FeederSensorSetWidget::showPage(bool bshow)
{
    if(bshow){
        updateFeederSensorCheckboxState();
    }
}

void FeederSensorSetWidget::receiveMsg(int msg1, int msg2, int msg3)
{

}

void FeederSensorSetWidget::checkboxClicked(int index)
{
   QCheckBox *check;
   check = (qobject_cast<QCheckBox*>)(m_pBtnGroup->button(index));
   if(check->isChecked()){
       struCnfg.struAlarmConfig.nFeederLevelAlarmEnable[index] = 1;
   }else{
       struCnfg.struAlarmConfig.nFeederLevelAlarmEnable[index] = 0;
   }
}

void FeederSensorSetWidget::checkboxPosiClicked(int index)
{
    QCheckBox *check;
    check = (qobject_cast<QCheckBox*>)(m_pBtnGroupPosi->button(index));
    if(check->isChecked()){
        struCnfg.struAlarmConfig.nFeederLevelAlarmPosi[index] = 1;
    }else{
        struCnfg.struAlarmConfig.nFeederLevelAlarmPosi[index] = 0;
    }
}

void FeederSensorSetWidget::sNameChanged()
{
    int index = 0;
    myLineEdit *m_pEdit;
    for(int i = 0;i < CTRL_ALARM_MAX_FEED_SENSOR;i++){
        m_pEdit = qobject_cast<myLineEdit*>(sender());
        if(m_pEdit == m_pFeederNameEdit[i]){
            index = i;
            break;
        }
    }
    memset(struCnfg.struAlarmConfig.nFeederLevelAlarmName[index],0,sizeof(struCnfg.struAlarmConfig.nFeederLevelAlarmName[index]));

    const char *strName = m_pFeederNameEdit[index]->text().toUtf8();
    memcpy(struCnfg.struAlarmConfig.nFeederLevelAlarmName[index],strName,strlen(strName));
}

void FeederSensorSetWidget::retranslateUiWidget()
{
    for(int i = 0;i < CTRL_ALARM_MAX_FEED_SENSOR;i++){
        m_pBtnGroup->button(i)->setText(g_myLan().level_sensor+QString::number(i+1));
        m_pBtnGroupPosi->button(i)->setText(g_myLan().top);
    }
}

void FeederSensorSetWidget::updateFeederSensorCheckboxState()
{
    for(int i = 0;i < CTRL_ALARM_MAX_FEED_SENSOR;i++){
        m_pBtnGroup->button(i)->setChecked(struCnfg.struAlarmConfig.nFeederLevelAlarmEnable[i] == 1);
        m_pBtnGroupPosi->button(i)->setChecked(struCnfg.struAlarmConfig.nFeederLevelAlarmPosi[i] == 1);
        QByteArray titleArr(struCnfg.struAlarmConfig.nFeederLevelAlarmName[i]);
        m_pFeederNameEdit[i]->setText(QString::fromUtf8(titleArr));
    }
}
