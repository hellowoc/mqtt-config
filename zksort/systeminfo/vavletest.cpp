#include "vavletest.h"
#include "globalflow.h"
#include "ui_vavletest.h"

vavletest::vavletest(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::vavletest)
{
    ui->setupUi(this);

    m_pLayerBtn[0] = ui->vavleTestLayer1Btn;
    m_pLayerBtn[1] = ui->vavleTestLayer2Btn;
    m_pViewBtn[0] = ui->pushButton_0;
    m_pViewBtn[1] = ui->pushButton_1;
    m_pViewBtn[2] = ui->pushButton_2;
    m_pViewBtn[3] = ui->pushButton_3;
    m_pViewBtn[4]= ui->pushButton_4;
    m_pViewBtn[5]= ui->pushButton_5;
    m_pViewBtn[6]= ui->pushButton_6;
    m_pViewBtn[7]= ui->pushButton_7;

    setSlotConnect();

    m_pEjectDurationEdit[0]= ui->eject_lineEdit_1;
    m_pEjectDurationEdit[1]= ui->eject_lineEdit_2;
    m_pEjectDurationEdit[2]= ui->eject_lineEdit_3;
    m_pEjectDurationEdit[3]= ui->eject_lineEdit_4;
    m_pEjectDurationEdit[4]= ui->eject_lineEdit_5;

    m_pEjectDurationLabel[0] = ui->eject_label_1;
    m_pEjectDurationLabel[1] = ui->eject_label_2;
    m_pEjectDurationLabel[2] = ui->eject_label_3;
    m_pEjectDurationLabel[3] = ui->eject_label_4;
    m_pEjectDurationLabel[4] = ui->eject_label_5;

    ui->ejectDurationGroup->hide();

    curVavle = 0;

    m_ejPerChute = struCnfg.struLayerInfo[struGsh.nLayer].nEjectorsPerChute;
    totalEjectors = m_ejPerChute*struCnfg.nChuteTotal;
    curEjector = 1;

    dispChuteNum = (curEjector%m_ejPerChute == 0) ? curEjector/m_ejPerChute : curEjector/m_ejPerChute+1;
    dispEjectorNum = (curEjector%m_ejPerChute == 0) ? m_ejPerChute : curEjector%m_ejPerChute;

    singleEjectorTest = false;
    fastEjectorTest = false;
    signalEjectorTest = false;

    myVavleThread  = new vavleTestThread;
    connect(myVavleThread,SIGNAL(ejectorChangeSig()),this,SLOT(vavleTestStartSlt()));
    connect(this,SIGNAL(vavleTestFinishedSignal()),this,SLOT(vavleTestFinishedSlots()));
}

vavletest::~vavletest()
{
    delete ui;
}

void vavletest::showPage(bool bshow)
{
    basewidget::showPage(bshow);
    updateViewBtnState();
    if(bshow){
        if(curVavle == 0){
            ui->vavleLayerBtn->setText(g_myLan().lower_layer);
        } else{
            ui->vavleLayerBtn->setText(g_myLan().upper_layer);
        }
        ui->vavleLayerBtn->setVisible(struCnfg.nMachineAttr == MACHINE_TYPE_BELT_DOUBLE);
        ui->chuteNumWidget->setParams(this,1,struCnfg.nChuteTotal,MSG_ID_CHUTE_NUM,&dispChuteNum);
        ui->ejectorNumWidget->setParams(this,1,struCnfg.struLayerInfo[struGsh.nLayer].nEjectorsPerChute,MSG_ID_EJECTOR_NUM,&dispEjectorNum);

        /* 喷阀自检 */
        dispChuteNum = (curEjector%m_ejPerChute == 0) ? curEjector/m_ejPerChute : curEjector/m_ejPerChute+1;
        dispEjectorNum = (curEjector%m_ejPerChute == 0) ? m_ejPerChute : curEjector%m_ejPerChute;

        ui->widget->setVisible(signalEjectorTest);
        ui->signalTestCheckBox->setChecked(signalEjectorTest);

        ui->chuteNumWidget->setTitle(g_myLan().ids_channel);
        ui->ejectorNumWidget->setTitle(g_myLan().ejector_num);
        ui->chuteNumWidget->setValue(dispChuteNum);
        ui->ejectorNumWidget->setValue(dispEjectorNum);

        for(int i = 0;i < MAX_GROUP_TICK;i++){
            m_ejectDuration[i] = (float)(struCnfp.struGroupTick[struGsh.nLayer][curVavle][i].nEjectDuration/10.0);
        }
        for(int i = 0;i < MAX_GROUP_TICK;i++){
            if(i <  struCnfg.struLayerInfo[struGsh.nLayer].stuVavleInfo[curVavle].nTickGroupTotal){
                m_pEjectDurationLabel[i]->setVisible(true);
                m_pEjectDurationEdit[i]->setVisible(true);
                m_pEjectDurationEdit[i]->setFloatMinMax(this,0,50.0,MSG_ID_EJECT_DURATION_START+i,&m_ejectDuration[i], false);
                m_pEjectDurationEdit[i]->setText(QString::number(m_ejectDuration[i],'f',1));
            }
            else
            {
                m_pEjectDurationEdit[i]->setVisible(false);
                m_pEjectDurationLabel[i]->setVisible(false);
            }
        }
    }
    else
    {
        curVavle = 0;

        if (myVavleThread->isRunning())
        {
            struGsh.nVavleTestIsRun = 0;

            myVavleThread->stop();
            ui->vavleTestStartBtn->setEnabled(true);
            ui->vavleTestStopBtn->setEnabled(false);

            int nInt = 0,board = 0;
            char data[SEND_PACKET_DATA_SIZE];

            for(int i = 0;i < struCnfg.struLayerInfo[struGsh.nLayer].nVavleChainTotal;i++){
                if(struCnfg.nMachineAttr == MACHINE_TYPE_BELT_DOUBLE)
                {
                    board = struCnfg.struLayerInfo[struGsh.nLayer].nVavleBoardType[i];  //认为每层一个喷阀链
                }
                else
                {
                    board = BOARD_TYPE_INTERFACE;
                }
                memset(data, 0, SEND_PACKET_DATA_SIZE);

                data[0] = 0;
                data[1] = 0;
                mySerial.com1WriteDirect(CMD_INT_EJECTOR_VAVLE_TEST, nInt,board, BOARD_TYPE_ALL_IN_CHAIN, data, 1); //关闭所有喷阀
            }

            myFlow.msleep(300);
            struGsh.bStatEject = 0;
            struGsh.nVavleTestIsRun = 0;
            myFlow.vavleOnOff();
            myFlow.resetSortOnOff(0, 1);
            myFlow.configIpcSortStart(0);

            curEjector = 1;
            dispChuteNum = 1;
            dispEjectorNum = 1;
        }
        signalEjectorTest = false;//关闭信号自检
    }
}

void vavletest::receiveMsg(int msg1, int msg2, int msg3)
{
    int tmp = 0;

    if(msg1 == MSG_DATA_CHANGED){

        if(msg2 < MSG_ID_CHUTE_NUM)
        {
            struCnfp.struGroupTick[struGsh.nLayer][curVavle][msg2].nEjectDuration = (int)((m_ejectDuration[msg2]+0.01)*10);
            myFlow.resetEjectDurationTimeRG();
        }
        else if(msg2 == MSG_ID_CHUTE_NUM)
        {
            curEjector = (dispChuteNum-1)*m_ejPerChute+dispEjectorNum;
        }
        else if(msg2 == MSG_ID_EJECTOR_NUM)
        {
            curEjector = (dispChuteNum-1)*m_ejPerChute+dispEjectorNum;
        }
    }
}

void vavletest::retranslateUiWidget()
{
    ui->vavleTestGroup->setTitle(g_myLan().ids_valveTest);
    ui->singleTestCheckBox->setText(g_myLan().ids_singleTest);
    ui->quickTestCheckBox->setText(g_myLan().ejector_test_fast);
    ui->vavleTestStartBtn->setText(g_myLan().ids_run);
    ui->vavleTestStopBtn->setText(g_myLan().ids_stop);
    ui->signalTestCheckBox->setText(g_myLan().ejector_test_signal);
}

void vavletest::OnLayerChange()
{
    showPage();
}

void vavletest::on_singleTestCheckBox_clicked()
{
    singleEjectorTest = (singleEjectorTest == false) ? true : false;
}

void vavletest::on_quickTestCheckBox_clicked()
{
    fastEjectorTest = (fastEjectorTest == false) ? true : false;
    myVavleThread->setTestSpeed(fastEjectorTest);
}

void vavletest::on_signalTestCheckBox_clicked()
{
    signalEjectorTest = (signalEjectorTest == false) ? true : false;
//    ui->widget->setVisible(signalEjectorTest);
//    ui->vavleLayerBtn->setVisible(!signalEjectorTest);

    if(signalEjectorTest == true)
    {
        myFlow.resetSortOnOff(1, 1);
    }
    else
    {
        myFlow.resetSortOnOff(0, 1);
    }
}

void vavletest::on_vavleTestStartBtn_clicked()
{
    if(struGsh.bSortStart == 0)
    {
        if(signalEjectorTest)//信号自检开启
        {
            myFlow.resetSortOnOff(1, 1);
        }
        else
        {
            myFlow.resetSortOnOff(0, 1);
        }
        struGsh.bStatEject = 1;
        myFlow.vavleOnOff();

        if (!myVavleThread->isRunning())
        {
            myVavleThread->start();
            struGsh.nVavleTestIsRun = 1;
            ui->vavleTestStartBtn->setEnabled(false);
            ui->vavleTestStopBtn->setEnabled(true);
		}
	}
    else
    {
        g_infoWidget().setLabelText(g_myLan().msg_turn_off_first);
        g_infoWidget().delayShow();

        myFlow.sleep(3);
        g_infoWidget().hide();
    }
//    else if((struGsh.bSortStart == 0) && (struGsh.bStatEject == 1))
//	{
//        g_infoWidget().setLabelText(g_myLan().turn on vavle first"));
//	    g_infoWidget().delayShow();

//		myFlow.sleep(3);
//		g_infoWidget().hide();
//	}
}

void vavletest::on_vavleTestStopBtn_clicked()
{
    int nInt=0, board;  // 0x00表示当前 视的所有相机板
    char data[SEND_PACKET_DATA_SIZE];
    memset(data, 0, SEND_PACKET_DATA_SIZE);

    board = struCnfg.struLayerInfo[struGsh.nLayer].nVavleBoardType[0];  //认为每层一个喷阀链
    if (myVavleThread->isRunning())
    {
        myVavleThread->stop();
        ui->vavleTestStartBtn->setEnabled(true);
        ui->vavleTestStopBtn->setEnabled(false);

        struGsh.nVavleTestIsRun = 0;
        struGsh.bStatEject = 0;
        myFlow.vavleOnOff();
        myFlow.resetSortOnOff(0, 1);
    }
}

void vavletest::vavleTestStartSlt()
{
    int nInt=0, board, nUnitAddr = 0x00;  // 0x00表示当前 视的所有相机板
    char data[SEND_PACKET_DATA_SIZE];

    if (myVavleThread->isRunning()) {
        dispChuteNum = (curEjector%m_ejPerChute == 0) ? curEjector/m_ejPerChute : curEjector/m_ejPerChute+1;
        dispEjectorNum = (curEjector%m_ejPerChute == 0) ? m_ejPerChute : curEjector%m_ejPerChute;

        ui->chuteNumWidget->setValue(dispChuteNum);
        ui->ejectorNumWidget->setValue(dispEjectorNum);
        memset(data, 0, SEND_PACKET_DATA_SIZE);
        board = struCnfg.struLayerInfo[struGsh.nLayer].nVavleBoardType[0];  //认为每层一个喷阀链

        if(signalEjectorTest)//信号自检开启
        {
            board = struCnfg.struLayerInfo[struGsh.nLayer].nViewBoardType[struGsh.nView];
            nUnitAddr = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nViewUnitId[dispChuteNum-1];

            if((board == BOARD_CAMERA_1_FRONT_MAIN)||(board == BOARD_CAMERA_1_FRONT_VICE)
                ||(board == BOARD_CAMERA_2_FRONT_MAIN) ||(board == BOARD_CAMERA_2_FRONT_VICE))
            {
                data[0] = struCnfg.struLayerInfo[struGsh.nLayer].nEjectorsPerChute+1-dispEjectorNum;
            }
            else
            {
                data[0] = dispEjectorNum;
            }
            data[1] = dispChuteNum;

            mySerial.com1WriteDirect(CMD_INT_EJECTOR_VAVLE_TEST, nInt, board, nUnitAddr, data, 1);
        }
        else
        {
            if(struCnfg.nMachineAttr == MACHINE_TYPE_BELT_DOUBLE)
            {
                board = struCnfg.struLayerInfo[struGsh.nLayer].nVavleBoardType[curVavle];
            }
            else
            {
                board = BOARD_TYPE_INTERFACE;
            }
            data[0] = dispEjectorNum;
            data[1] = dispChuteNum;

            mySerial.com1WriteDirect(CMD_INT_EJECTOR_VAVLE_TEST, nInt, board, BOARD_TYPE_ALL_IN_CHAIN, data, 1);
        }
        if (!singleEjectorTest) {
            curEjector++;
        }

        if (curEjector > totalEjectors) {
            curEjector = 1;
            if (!singleEjectorTest) {
                emit vavleTestFinishedSignal();
                myVavleThread->stop();
                myVavleThread->wait();

                dispChuteNum = 1;
                dispEjectorNum = 1;

                myFlow.msleep(300);
                struGsh.bStatEject = 0;
                struGsh.nVavleTestIsRun = 0;
                myFlow.vavleOnOff();
                myFlow.resetSortOnOff(0, 1);
            }
        }
    }
}

void vavletest::vavleTestFinishedSlots()
{
    ui->vavleTestStartBtn->setEnabled(true);
    ui->vavleTestStopBtn->setEnabled(false);
}


void vavletest::on_vavleLayerBtn_clicked()
{
    curVavle = (curVavle == 0) ? 1 : 0;
    int val = struCnfg.struLayerInfo[struGsh.nLayer].nVavleBoardTotal[curVavle] / (struCnfg.nValveRatio+1);
    if(val == 0) val = 1;
    if(curVavle == 0)
    {
        ui->vavleLayerBtn->setText(g_myLan().lower_layer);
    }
    else
    {
        ui->vavleLayerBtn->setText(g_myLan().upper_layer);
    }
    dispChuteNum = 0;
    totalEjectors = m_ejPerChute*val;
    ui->chuteNumWidget->setParams(this,1,val,MSG_ID_CHUTE_NUM,&dispChuteNum);
}
/* ****************************************************************************
 *
 *                        喷阀自检线程
 *
 *******************************************************************************/
vavleTestThread::vavleTestThread()
{
    stopped = false;
    m_bIsFast = false;
}

void vavleTestThread::run(void)
{

    if (m_bIsFast){
//        QString dateStr = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
//        QString voicePath = "/sdcard/ts/"+dateStr+".wmv";
//        QString cmd = "arecord -D \"plughw:0,0\" +voicePath";
#ifdef Q_OS_UNIX
        system("arecord -D \"plughw:0,0\" /sdcard/ts/valveTest.wav &");
        sleep(3);
#endif
    }
    while (!stopped)
    {
        if(m_bIsFast) {
            msleep(300);
        } else {
            msleep(600);
        }
        emit ejectorChangeSig();
    }
    stopped = false;
    if (m_bIsFast){
#ifdef Q_OS_UNIX
        system("killall arecord");
        QString str = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
        QString str1= "mv /sdcard/ts/valveTest.wav /sdcard/ts/"+str+".wav";
        const char* cmd1 = QString(str1).toLocal8Bit().constData();
        system(cmd1);
        system("sync");
//        QFileInfo file("/sdcard/ts/"+str+".wav");
//        if(file.exists())
//            myMqttMsgParaseThread->n_uploadValveAudioFlag = 1;
#endif
    }

}

void vavleTestThread::stop()
{
    stopped = true;
}

void vavleTestThread::setTestSpeed(bool isFast)
{
    m_bIsFast = isFast;
}
