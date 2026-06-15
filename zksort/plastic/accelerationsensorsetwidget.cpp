#include "accelerationsensorsetwidget.h"
#include "ui_accelerationsensorsetwidget.h"
#include <QButtonGroup>

AccelerationSensorSetWidget::AccelerationSensorSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::AccelerationSensorSetWidget)
{
    ui->setupUi(this);
    pAmplitudeLdt[0] = ui->amplitudeLdt_1;
    pAmplitudeLdt[1] = ui->amplitudeLdt_2;
    pAmplitudeLdt[2] = ui->amplitudeLdt_3;
    pAmplitudeLdt[3] = ui->amplitudeLdt_4;
    pAmplitudeLdt[4] = ui->amplitudeLdt_5;
    pAmplitudeLdt[5] = ui->amplitudeLdt_6;
    pAmplitudeLdt[6] = ui->amplitudeLdt_7;
    pAmplitudeLdt[7] = ui->amplitudeLdt_8;
    pAmplitudeLdt[8] = ui->amplitudeLdt_9;
    pAmplitudeLdt[9] = ui->amplitudeLdt_10;
    pAmplitudeLdt[10] = ui->amplitudeLdt_11;
    pAmplitudeLdt[11] = ui->amplitudeLdt_12;

    pDutyCycleLdt[0] = ui->dutyCycleLdt_1;
    pDutyCycleLdt[1] = ui->dutyCycleLdt_2;
    pDutyCycleLdt[2] = ui->dutyCycleLdt_3;
    pDutyCycleLdt[3] = ui->dutyCycleLdt_4;
    pDutyCycleLdt[4] = ui->dutyCycleLdt_5;
    pDutyCycleLdt[5] = ui->dutyCycleLdt_6;
    pDutyCycleLdt[6] = ui->dutyCycleLdt_7;
    pDutyCycleLdt[7] = ui->dutyCycleLdt_8;
    pDutyCycleLdt[8] = ui->dutyCycleLdt_9;
    pDutyCycleLdt[9] = ui->dutyCycleLdt_10;
    pDutyCycleLdt[10] = ui->dutyCycleLdt_11;
    pDutyCycleLdt[11] = ui->dutyCycleLdt_12;

    pDutyCycleChangedLdt[0] = ui->dutyCycleChangedLdt_1;
    pDutyCycleChangedLdt[1] = ui->dutyCycleChangedLdt_2;
    pDutyCycleChangedLdt[2] = ui->dutyCycleChangedLdt_3;
    pDutyCycleChangedLdt[3] = ui->dutyCycleChangedLdt_4;
    pDutyCycleChangedLdt[4] = ui->dutyCycleChangedLdt_5;
    pDutyCycleChangedLdt[5] = ui->dutyCycleChangedLdt_6;
    pDutyCycleChangedLdt[6] = ui->dutyCycleChangedLdt_7;
    pDutyCycleChangedLdt[7] = ui->dutyCycleChangedLdt_8;
    pDutyCycleChangedLdt[8] = ui->dutyCycleChangedLdt_9;
    pDutyCycleChangedLdt[9] = ui->dutyCycleChangedLdt_10;
    pDutyCycleChangedLdt[10] = ui->dutyCycleChangedLdt_11;
    pDutyCycleChangedLdt[11] = ui->dutyCycleChangedLdt_12;

    pAmplitudeLdt2[0] = ui->amplitudeLdt2_1;
    pAmplitudeLdt2[1] = ui->amplitudeLdt2_2;
    pAmplitudeLdt2[2] = ui->amplitudeLdt2_3;
    pAmplitudeLdt2[3] = ui->amplitudeLdt2_4;
    pAmplitudeLdt2[4] = ui->amplitudeLdt2_5;
    pAmplitudeLdt2[5] = ui->amplitudeLdt2_6;
    pAmplitudeLdt2[6] = ui->amplitudeLdt2_7;
    pAmplitudeLdt2[7] = ui->amplitudeLdt2_8;
    pAmplitudeLdt2[8] = ui->amplitudeLdt2_9;
    pAmplitudeLdt2[9] = ui->amplitudeLdt2_10;
    pAmplitudeLdt2[10] = ui->amplitudeLdt2_11;
    pAmplitudeLdt2[11] = ui->amplitudeLdt2_12;

    pDutyCycleLdt2[0] = ui->dutyCycleLdt2_1;
    pDutyCycleLdt2[1] = ui->dutyCycleLdt2_2;
    pDutyCycleLdt2[2] = ui->dutyCycleLdt2_3;
    pDutyCycleLdt2[3] = ui->dutyCycleLdt2_4;
    pDutyCycleLdt2[4] = ui->dutyCycleLdt2_5;
    pDutyCycleLdt2[5] = ui->dutyCycleLdt2_6;
    pDutyCycleLdt2[6] = ui->dutyCycleLdt2_7;
    pDutyCycleLdt2[7] = ui->dutyCycleLdt2_8;
    pDutyCycleLdt2[8] = ui->dutyCycleLdt2_9;
    pDutyCycleLdt2[9] = ui->dutyCycleLdt2_10;
    pDutyCycleLdt2[10] = ui->dutyCycleLdt2_11;
    pDutyCycleLdt2[11] = ui->dutyCycleLdt2_12;

    pDutyCycleChangedLdt2[0] = ui->dutyCycleChangedLdt2_1;
    pDutyCycleChangedLdt2[1] = ui->dutyCycleChangedLdt2_2;
    pDutyCycleChangedLdt2[2] = ui->dutyCycleChangedLdt2_3;
    pDutyCycleChangedLdt2[3] = ui->dutyCycleChangedLdt2_4;
    pDutyCycleChangedLdt2[4] = ui->dutyCycleChangedLdt2_5;
    pDutyCycleChangedLdt2[5] = ui->dutyCycleChangedLdt2_6;
    pDutyCycleChangedLdt2[6] = ui->dutyCycleChangedLdt2_7;
    pDutyCycleChangedLdt2[7] = ui->dutyCycleChangedLdt2_8;
    pDutyCycleChangedLdt2[8] = ui->dutyCycleChangedLdt2_9;
    pDutyCycleChangedLdt2[9] = ui->dutyCycleChangedLdt2_10;
    pDutyCycleChangedLdt2[10] = ui->dutyCycleChangedLdt2_11;
    pDutyCycleChangedLdt2[11] = ui->dutyCycleChangedLdt2_12;

    pAccelerationEnable[0] = ui->checkBox;
    pAccelerationEnable[1] = ui->checkBox_2;
    pAccelerationEnable[2] = ui->checkBox_3;
    pAccelerationEnable[3] = ui->checkBox_4;
    pAccelerationEnable[4] = ui->checkBox_5;
    pAccelerationEnable[5] = ui->checkBox_6;
    pAccelerationEnable[6] = ui->checkBox_7;
    pAccelerationEnable[7] = ui->checkBox_8;
    pAccelerationEnable[8] = ui->checkBox_9;
    pAccelerationEnable[9] = ui->checkBox_10;
    pAccelerationEnable[10] = ui->checkBox_11;
    pAccelerationEnable[11] = ui->checkBox_12;

    pAccelerationEnable2[0] = ui->checkBox2_1;
    pAccelerationEnable2[1] = ui->checkBox2_2;
    pAccelerationEnable2[2] = ui->checkBox2_3;
    pAccelerationEnable2[3] = ui->checkBox2_4;
    pAccelerationEnable2[4] = ui->checkBox2_5;
    pAccelerationEnable2[5] = ui->checkBox2_6;
    pAccelerationEnable2[6] = ui->checkBox2_7;
    pAccelerationEnable2[7] = ui->checkBox2_8;
    pAccelerationEnable2[8] = ui->checkBox2_9;
    pAccelerationEnable2[9] = ui->checkBox2_10;
    pAccelerationEnable2[10] = ui->checkBox2_11;
    pAccelerationEnable2[11] = ui->checkBox2_12;

    pAccelerationState[0] = ui->stateLbl_1;
    pAccelerationState[1] = ui->stateLbl_2;
    pAccelerationState[2] = ui->stateLbl_3;
    pAccelerationState[3] = ui->stateLbl_4;
    pAccelerationState[4] = ui->stateLbl_5;
    pAccelerationState[5] = ui->stateLbl_6;
    pAccelerationState[6] = ui->stateLbl_7;
    pAccelerationState[7] = ui->stateLbl_8;
    pAccelerationState[8] = ui->stateLbl_9;
    pAccelerationState[9] = ui->stateLbl_10;
    pAccelerationState[10] = ui->stateLbl_11;
    pAccelerationState[11] = ui->stateLbl_12;

    pAccelerationState2[0] = ui->stateLbl2_1;
    pAccelerationState2[1] = ui->stateLbl2_2;
    pAccelerationState2[2] = ui->stateLbl2_3;
    pAccelerationState2[3] = ui->stateLbl2_4;
    pAccelerationState2[4] = ui->stateLbl2_5;
    pAccelerationState2[5] = ui->stateLbl2_6;
    pAccelerationState2[6] = ui->stateLbl2_7;
    pAccelerationState2[7] = ui->stateLbl2_8;
    pAccelerationState2[8] = ui->stateLbl2_9;
    pAccelerationState2[9] = ui->stateLbl2_10;
    pAccelerationState2[10] = ui->stateLbl2_11;
    pAccelerationState2[11] = ui->stateLbl2_12;

    pAmplitudeLbl[0] = ui->amplitudeLbl_1;
    pAmplitudeLbl[1] = ui->amplitudeLbl_2;
    pAmplitudeLbl[2] = ui->amplitudeLbl_3;
    pAmplitudeLbl[3] = ui->amplitudeLbl_4;
    pAmplitudeLbl[4] = ui->amplitudeLbl_5;
    pAmplitudeLbl[5] = ui->amplitudeLbl_6;
    pAmplitudeLbl[6] = ui->amplitudeLbl_7;
    pAmplitudeLbl[7] = ui->amplitudeLbl_8;
    pAmplitudeLbl[8] = ui->amplitudeLbl_9;
    pAmplitudeLbl[9] = ui->amplitudeLbl_10;
    pAmplitudeLbl[10] = ui->amplitudeLbl_11;
    pAmplitudeLbl[11] = ui->amplitudeLbl_12;

    pAmplitudeLbl2[0] = ui->amplitudeLbl2_1;
    pAmplitudeLbl2[1] = ui->amplitudeLbl2_2;
    pAmplitudeLbl2[2] = ui->amplitudeLbl2_3;
    pAmplitudeLbl2[3] = ui->amplitudeLbl2_4;
    pAmplitudeLbl2[4] = ui->amplitudeLbl2_5;
    pAmplitudeLbl2[5] = ui->amplitudeLbl2_6;
    pAmplitudeLbl2[6] = ui->amplitudeLbl2_7;
    pAmplitudeLbl2[7] = ui->amplitudeLbl2_8;
    pAmplitudeLbl2[8] = ui->amplitudeLbl2_9;
    pAmplitudeLbl2[9] = ui->amplitudeLbl2_10;
    pAmplitudeLbl2[10] = ui->amplitudeLbl2_11;
    pAmplitudeLbl2[11] = ui->amplitudeLbl2_12;

    pDutyCycleLbl[0] = ui->dutyCycleLbl_1;
    pDutyCycleLbl[1] = ui->dutyCycleLbl_2;
    pDutyCycleLbl[2] = ui->dutyCycleLbl_3;
    pDutyCycleLbl[3] = ui->dutyCycleLbl_4;
    pDutyCycleLbl[4] = ui->dutyCycleLbl_5;
    pDutyCycleLbl[5] = ui->dutyCycleLbl_6;
    pDutyCycleLbl[6] = ui->dutyCycleLbl_7;
    pDutyCycleLbl[7] = ui->dutyCycleLbl_8;
    pDutyCycleLbl[8] = ui->dutyCycleLbl_9;
    pDutyCycleLbl[9] = ui->dutyCycleLbl_10;
    pDutyCycleLbl[10] = ui->dutyCycleLbl_11;
    pDutyCycleLbl[11] = ui->dutyCycleLbl_12;

    pDutyCycleLbl2[0] = ui->dutyCycleLbl2_1;
    pDutyCycleLbl2[1] = ui->dutyCycleLbl2_2;
    pDutyCycleLbl2[2] = ui->dutyCycleLbl2_3;
    pDutyCycleLbl2[3] = ui->dutyCycleLbl2_4;
    pDutyCycleLbl2[4] = ui->dutyCycleLbl2_5;
    pDutyCycleLbl2[5] = ui->dutyCycleLbl2_6;
    pDutyCycleLbl2[6] = ui->dutyCycleLbl2_7;
    pDutyCycleLbl2[7] = ui->dutyCycleLbl2_8;
    pDutyCycleLbl2[8] = ui->dutyCycleLbl2_9;
    pDutyCycleLbl2[9] = ui->dutyCycleLbl2_10;
    pDutyCycleLbl2[10] = ui->dutyCycleLbl2_11;
    pDutyCycleLbl2[11] = ui->dutyCycleLbl2_12;

    pDutyCycleChangedLbl[0] = ui->dutyCycleChangedLbl_1;
    pDutyCycleChangedLbl[1] = ui->dutyCycleChangedLbl_2;
    pDutyCycleChangedLbl[2] = ui->dutyCycleChangedLbl_3;
    pDutyCycleChangedLbl[3] = ui->dutyCycleChangedLbl_4;
    pDutyCycleChangedLbl[4] = ui->dutyCycleChangedLbl_5;
    pDutyCycleChangedLbl[5] = ui->dutyCycleChangedLbl_6;
    pDutyCycleChangedLbl[6] = ui->dutyCycleChangedLbl_7;
    pDutyCycleChangedLbl[7] = ui->dutyCycleChangedLbl_8;
    pDutyCycleChangedLbl[8] = ui->dutyCycleChangedLbl_9;
    pDutyCycleChangedLbl[9] = ui->dutyCycleChangedLbl_10;
    pDutyCycleChangedLbl[10] = ui->dutyCycleChangedLbl_11;
    pDutyCycleChangedLbl[11] = ui->dutyCycleChangedLbl_12;

    pDutyCycleChangedLbl2[0] = ui->dutyCycleChangedLbl2_1;
    pDutyCycleChangedLbl2[1] = ui->dutyCycleChangedLbl2_2;
    pDutyCycleChangedLbl2[2] = ui->dutyCycleChangedLbl2_3;
    pDutyCycleChangedLbl2[3] = ui->dutyCycleChangedLbl2_4;
    pDutyCycleChangedLbl2[4] = ui->dutyCycleChangedLbl2_5;
    pDutyCycleChangedLbl2[5] = ui->dutyCycleChangedLbl2_6;
    pDutyCycleChangedLbl2[6] = ui->dutyCycleChangedLbl2_7;
    pDutyCycleChangedLbl2[7] = ui->dutyCycleChangedLbl2_8;
    pDutyCycleChangedLbl2[8] = ui->dutyCycleChangedLbl2_9;
    pDutyCycleChangedLbl2[9] = ui->dutyCycleChangedLbl2_10;
    pDutyCycleChangedLbl2[10] = ui->dutyCycleChangedLbl2_11;
    pDutyCycleChangedLbl2[11] = ui->dutyCycleChangedLbl2_12;

    m_pbtnGroup = new QButtonGroup(this);
    m_pbtnGroup->addButton(ui->controlBoard1Btn,0);
    m_pbtnGroup->addButton(ui->controlBoard2Btn,1);

    for(int i = 0;i< m_pbtnGroup->buttons().count();i++){
        m_pbtnGroup->button(i)->setCheckable(true);
        connect(m_pbtnGroup->button(i),SIGNAL(clicked(bool)),this,SLOT(btn_clicked(bool)));
    }

    // 控制板1
    pAccelerationEnableSigMapper = new QSignalMapper(this);

    for(int i = 0;i < MAX_ACCELERATION;i++){
        pAccelerationEnableSigMapper->setMapping(pAccelerationEnable[i],i);
        connect(pAccelerationEnable[i],SIGNAL(pressed()),pAccelerationEnableSigMapper,SLOT(map()));
    }

    connect(pAccelerationEnableSigMapper,SIGNAL(mapped(int)),this,SLOT(accelerationEnableClicked(int)));

    // 控制板2
    pAccelerationEnableSigMapper2 = new QSignalMapper(this);

    for(int i = 0;i < MAX_ACCELERATION;i++){
        pAccelerationEnableSigMapper2->setMapping(pAccelerationEnable2[i],i);
        connect(pAccelerationEnable2[i],SIGNAL(pressed()),pAccelerationEnableSigMapper2,SLOT(map()));
    }

    connect(pAccelerationEnableSigMapper2,SIGNAL(mapped(int)),this,SLOT(accelerationEnable2Clicked(int)));

    memset(cmdEnableData, 0, SEND_PACKET_DATA_SIZE);
    memset(cmdEnableData2, 0, SEND_PACKET_DATA_SIZE);

    ui->stackedWidget->setCurrentIndex(0);
    ui->controlBoard1Btn->setChecked(true);

//    ui->feederSelectionCbx->hide();
//    ui->feederSelectionCbx_2->hide();

    ui->pushButton->setText(g_myLan().ids_query);
    ui->controlBoard1Btn->setText(g_myLan().ids_controlBoard+"1");
    ui->controlBoard2Btn->setText(g_myLan().ids_controlBoard+"2");
}

AccelerationSensorSetWidget::~AccelerationSensorSetWidget()
{
    delete ui;
}

void AccelerationSensorSetWidget::showPage(bool bshow)
{
    if(bshow)
    {
        ui->controlBoard1Btn->setVisible(struCnfg.nChuteTotal>8);
        ui->controlBoard2Btn->setVisible(struCnfg.nChuteTotal>8);
        if(struCnfg.nChuteTotal <= MAX_ACCELERATION)
            ui->stackedWidget->setCurrentIndex(0);

        int cnt = struCnfg.nChuteTotal;
        int cnt2 = qMax(cnt - MAX_ACCELERATION,0);

        for(int i = 0; i < MAX_ACCELERATION; i++)
        {
            pAmplitudeLbl[i]->setVisible(i < cnt);
            pDutyCycleLbl[i]->setVisible(i < cnt);
            pDutyCycleChangedLbl[i]->setVisible(i < cnt);

            pAmplitudeLdt[i]->setVisible(i < cnt);
            pDutyCycleLdt[i]->setVisible(i < cnt);
            pDutyCycleChangedLdt[i]->setVisible(i < cnt);
            pAccelerationEnable[i]->setVisible(i < cnt);
            pAccelerationState[i]->setVisible(i < cnt);

            pAmplitudeLbl2[i]->setVisible(i < cnt2);
            pDutyCycleLbl2[i]->setVisible(i < cnt2);
            pDutyCycleChangedLbl2[i]->setVisible(i < cnt2);

            pAmplitudeLdt2[i]->setVisible(i < cnt2);
            pDutyCycleLdt2[i]->setVisible(i < cnt2);
            pDutyCycleChangedLdt2[i]->setVisible(i < cnt2);
            pAccelerationEnable2[i]->setVisible(i < cnt2);
            pAccelerationState2[i]->setVisible(i < cnt2);
        }
        for(int i = 0; i < MAX_ACCELERATION; i ++)
        {
            updateAccelerationEnable(i);
            updateAccelerationData(i);
            updateAcceleration2Enable(i);
        }
        updateAccelerationTolerance();
        updateFeederSelection();
    }
}

void AccelerationSensorSetWidget::retranslateUiWidget()
{
    for(int i = 0; i < MAX_ACCELERATION; i++)
    {
        pAmplitudeLbl[i]->setText(g_myLan().ids_sensitivity2);
        pAmplitudeLbl2[i]->setText(g_myLan().ids_sensitivity2);

        pDutyCycleLbl[i]->setText(g_myLan().ids_acceleration_feedback1);
        pDutyCycleLbl2[i]->setText(g_myLan().ids_acceleration_feedback1);

        pDutyCycleChangedLbl[i]->setText(g_myLan().ids_acceleration_feedback2);
        pDutyCycleChangedLbl2[i]->setText(g_myLan().ids_acceleration_feedback2);
    }
    ui->toleranceLbl->setText(g_myLan().ids_acceleration_tolerance);
    ui->toleranceLbl_2->setText(g_myLan().ids_acceleration_tolerance);

    ui->numLbl_2->setText(g_myLan().ids_acceleration_feeder);
    ui->numLbl->setText(g_myLan().ids_acceleration_feeder);

    ui->pushButton->setText(g_myLan().ids_query);
    ui->controlBoard1Btn->setText(g_myLan().ids_controlBoard+"1");
    ui->controlBoard2Btn->setText(g_myLan().ids_controlBoard+"2");
}

void AccelerationSensorSetWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){
        switch (msg2)
        {
        case 1:
            cmdEnableData[13] = struCnfg.accelerationNum/256;
            cmdEnableData[14] = struCnfg.accelerationNum%256;
            cmdEnableData[15] = struCnfg.accelerationTolerance;
            mySerial.pushCom1CmdData(CMD_ACCELERATION_ENABLE, 0, BOARD_TYPE_CONTROL, 0, cmdEnableData, 1);
            break;
        case 2:
            cmdEnableData2[13] = struCnfg.accelerationNum2/256;
            cmdEnableData2[14] = struCnfg.accelerationNum2%256;
            cmdEnableData2[15] = struCnfg.accelerationTolerance2;
            mySerial.pushCom1CmdData(CMD_ACCELERATION_ENABLE, 0, BOARD_TYPE_CONTROL, 1, cmdEnableData2, 1);
            break;
            break;
        }

    }
}

void AccelerationSensorSetWidget::updateAccelerationEnable(int index)
{
    pAccelerationEnable[index]->setChecked(struCnfg.accelerationEnable[index]);
    cmdEnableData[index] = struCnfg.accelerationEnable[index];
}

void AccelerationSensorSetWidget::updateAcceleration2Enable(int index)
{
    pAccelerationEnable2[index]->setChecked(struCnfg.accelerationEnable2[index]);
    cmdEnableData2[index] = struCnfg.accelerationEnable2[index];
}

void AccelerationSensorSetWidget::accelerationEnableClicked(int index)
{

    if(struCnfg.accelerationEnable[index])
    {
        struCnfg.accelerationEnable[index] = 0;
    }
    else
    {
        struCnfg.accelerationEnable[index] = 1;

    }
    cmdEnableData[index] = struCnfg.accelerationEnable[index];
    cmdEnableData[14] = struCnfg.accelerationNum;
    cmdEnableData[15] = struCnfg.accelerationTolerance;
    mySerial.pushCom1CmdData(CMD_ACCELERATION_ENABLE, 0, BOARD_TYPE_CONTROL, 0, cmdEnableData, 1);
}

void AccelerationSensorSetWidget::accelerationEnable2Clicked(int index)
{

    if(struCnfg.accelerationEnable2[index])
    {
        struCnfg.accelerationEnable2[index] = 0;
    }
    else
    {
        struCnfg.accelerationEnable2[index] = 1;
    }
    cmdEnableData2[index] = struCnfg.accelerationEnable2[index];
    cmdEnableData2[14] = struCnfg.accelerationNum2;
    cmdEnableData2[15] = struCnfg.accelerationTolerance2;
    mySerial.pushCom1CmdData(CMD_ACCELERATION_ENABLE, 0, BOARD_TYPE_CONTROL, 1, cmdEnableData2, 1);

}

void AccelerationSensorSetWidget::on_pushButton_clicked()
{
    memset(cmdData, 0, SEND_PACKET_DATA_SIZE);
    mySerial.pushCom1CmdData(CMD_ACCELERATION_CAPTURE, 0, BOARD_TYPE_CONTROL, 0, cmdData, 1);
    myFlow.msleep(100);
    mySerial.pushCom1CmdData(CMD_ACCELERATION_CAPTURE, 0, BOARD_TYPE_CONTROL, 1, cmdData, 1);

    for(int i = 0; i < MAX_ACCELERATION; i ++)
    {
        updateAccelerationData(i);
    }
}

void AccelerationSensorSetWidget::updateAccelerationTolerance()
{
    ui->toleranceLdt->setMinMax(this,0,15,1,&struCnfg.accelerationTolerance);
    ui->toleranceLdt->setText(QString::number(struCnfg.accelerationTolerance));
    ui->toleranceLdt_2->setMinMax(this,0,15,2,&struCnfg.accelerationTolerance2);
    ui->toleranceLdt_2->setText(QString::number(struCnfg.accelerationTolerance2));

//    ui->feederSelectionLdt->setMinMax(this,0,2000,1,&struCnfg.accelerationNum);
//    ui->feederSelectionLdt->setText(QString::number(struCnfg.accelerationNum));
//    ui->feederSelectionLdt_2->setMinMax(this,0,2000,2,&struCnfg.accelerationNum2);
//    ui->feederSelectionLdt_2->setText(QString::number(struCnfg.accelerationNum2));
}

void AccelerationSensorSetWidget::updateAccelerationData(int index)
{
    QString str = "";
    pAmplitudeLdt[index]->setEnabled(false);
    pDutyCycleLdt[index]->setEnabled(false);
    pDutyCycleChangedLdt[index]->setEnabled(false);
    pAmplitudeLdt[index]->setText(IntToQString(struGsh.accelerationAmplitude[index]));
    pDutyCycleLdt[index]->setText(IntToQString(struGsh.accelerationDutyCycle[index]));
    pDutyCycleChangedLdt[index]->setText(IntToQString(struGsh.accelerationChangedDutyCycle[index]));

    pAmplitudeLdt2[index]->setEnabled(false);
    pDutyCycleLdt2[index]->setEnabled(false);
    pDutyCycleChangedLdt2[index]->setEnabled(false);
    pAmplitudeLdt2[index]->setText(IntToQString(struGsh.accelerationAmplitude2[index]));
    pDutyCycleLdt2[index]->setText(IntToQString(struGsh.accelerationDutyCycle2[index]));
    pDutyCycleChangedLdt2[index]->setText(IntToQString(struGsh.accelerationChangedDutyCycle2[index]));

    str = g_myLan().state + QString(":%1").arg(struGsh.struMachineStatus[struGsh.nLayer].nAccelerationStatus[index]);
    pAccelerationState[index]->setText(str);

    str = g_myLan().state + QString(":%1").arg(struGsh.struMachineStatus[struGsh.nLayer].nAccelerationStatus2[index]);
    pAccelerationState2[index]->setText(str);
}

void AccelerationSensorSetWidget::btn_clicked(bool check)
{
    if(check)
    {
        for(int i = 0; i < MAX_ACCELERATION; i ++)
        {
            updateAccelerationEnable(i);
            updateAccelerationData(i);
            updateAcceleration2Enable(i);
        }
        updateFeederSelection();
        ui->stackedWidget->setCurrentIndex(m_pbtnGroup->id((QPushButton*)sender()));
    }
}

void AccelerationSensorSetWidget::on_feederSelectionCbx_activated(int index)
{
    struCnfg.accelerationNum = index;
    updateFeederSelection();
    cmdEnableData[14] = struCnfg.accelerationNum;
    mySerial.pushCom1CmdData(CMD_ACCELERATION_ENABLE, 0, BOARD_TYPE_CONTROL, 0, cmdEnableData, 1);
}

void AccelerationSensorSetWidget::updateFeederSelection()
{
    ui->feederSelectionCbx->clear();
    QStringList list;
    list<<g_myLan().ids_acceleration_oblique<<g_myLan().ids_acceleration_straight;
    ui->feederSelectionCbx->addItems(list);
    ui->feederSelectionCbx->setCurrentIndex(struCnfg.accelerationNum);

    ui->feederSelectionCbx_2->clear();
    ui->feederSelectionCbx_2->addItems(list);
    ui->feederSelectionCbx_2->setCurrentIndex(struCnfg.accelerationNum2);
}

void AccelerationSensorSetWidget::on_feederSelectionCbx_2_activated(int index)
{
    struCnfg.accelerationNum2 = index;
    updateFeederSelection();
    cmdEnableData2[14] = struCnfg.accelerationNum2;
    mySerial.pushCom1CmdData(CMD_ACCELERATION_ENABLE, 0, BOARD_TYPE_CONTROL, 1, cmdEnableData2, 1);
}
