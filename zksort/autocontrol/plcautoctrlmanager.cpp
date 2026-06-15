#include "plcautoctrlmanager.h"

PlcAutoCtrlManager *plcautoctrl;

PlcAutoCtrlManager::PlcAutoCtrlManager(QObject *parent):QObject(parent)
{
    //    remoteCtrlIsRunning = 0;

    connect(mySerial2Recv,SIGNAL(signalCtrlBoardOutage()),this,SLOT(shutdownSlt()));
    //connect(myFlow.myNetWork8083->udprecvthread,SIGNAL(signalCtrlBoardOutage()),this,SLOT(shutdownSlt()));

    connect(mySerial2Recv,SIGNAL(signalCtrlBoardFeedOnOffQuery(int)),this,SLOT(feedOnOffSlt(int)));
    //connect(myFlow.myNetWork8083->udprecvthread,SIGNAL(signalCtrlBoardFeedOnOffQuery(int)),this,SLOT(feedOnOffSlt(int)));

    connect(mySerial2Recv,SIGNAL(signalCtrlBoardStatusQuery(int)),this,SLOT(sendStatusSlt(int)));
    //connect(myFlow.myNetWork8083->udprecvthread,SIGNAL(signalCtrlBoardStatusQuery(int)),this,SLOT(sendStatusSlt(int)));

    connect(mySerial2Recv,SIGNAL(signalCtrlBoardMachineInfoQuery()),this,SLOT(sendMachineInfoSlt()));
    //connect(myFlow.myNetWork8083->udprecvthread,SIGNAL(signalCtrlBoardMachineInfoQuery()),this,SLOT(sendMachineInfoSlt()));

    connect(mySerial2Recv,SIGNAL(signalCtrlBoardFeedMainValueQuery()),this,SLOT(FeederMainValueSlt()));
    //connect(myFlow.myNetWork8083->udprecvthread,SIGNAL(signalCtrlBoardFeedMainValueQuery()),this,SLOT(FeederMainValueSlt()));

    connect(mySerial2Recv,SIGNAL(signalCtrlBoardFeedViceValueQuery()),this,SLOT(FeederViceValueSlt()));
    //connect(myFlow.myNetWork8083->udprecvthread,SIGNAL(signalCtrlBoardFeedViceValueQuery()),this,SLOT(FeederViceValueSlt()));

}

PlcAutoCtrlManager::~PlcAutoCtrlManager()
{

}

void PlcAutoCtrlManager::feedOnOffSlt(int flag)
{
    struCnfg.remoteCtrlIsRunning = 1;

    if(struCnfg.nUseIpcEnable == 1)
    {
        if(struIpcShare.allIpcModelLoadOk != 2 )
        {
            struCnfg.remoteCtrlIsRunning = 0;
            return;
        }
    }
    if(struCnfg.nPlcControl) {
        struCnfg.remoteCtrlIsRunning = 0;
        return;
    }

    if(struGsh.bSortStart == 0)
    {
        if(struGsh.bAlarmStatus == 1)
        {
            struCnfg.remoteCtrlIsRunning = 0;
            return;
        }

        if(struCnfg.blampctrlEnable == 1)
        {
            if(flag == 1)
            {
                logger()->info("remote lamp on");
                myFlow.resetLampRG();
            }
        }
        logger()->info("sort start");
        g_infoWidget().setLabelText(g_myLan().msg_turning_on);
        g_infoWidget().delayShow();
        myFlow.onOff();

        myFlow.sleep(1);
        g_infoWidget().hide();
    }
    else
    {
        logger()->info("sort stop");
        g_infoWidget().setLabelText(g_myLan().msg_turning_off);
        g_infoWidget().delayShow();
        myFlow.onOff();
        if(struCnfg.blampctrlEnable == 1)
        {
            if(flag == 0)
            {
                logger()->info("remote lamp off");
                myFlow.resetLampPlcOff();
            }
        }

        myFlow.sleep(1);
        g_infoWidget().hide();

    }
    struCnfg.remoteCtrlIsRunning = 0;
}

void PlcAutoCtrlManager::shutdownSlt()
{
    struCnfg.remoteCtrlIsRunning = 1;
    logger()->info("delay power off");
    if(struGsh.bSortStart == 1)
    {
        g_infoWidget().setLabelText(g_myLan().msg_turning_off);

        g_infoWidget().delayShow();
        myFlow.onOff();
        g_infoWidget().hide();
    }

    struGsh.bUpdateState = 1;
    for(int i = 0; i < struCnfg.nIpcCount; i++)
    {
        myFlow.shutdownIpc(true,0,0,0);
    }

    g_infoWidget().setLabelText(g_myLan().msg_ipc_clean_filter_cotton);
    g_infoWidget().showSecs();


    g_infoWidget().setLabelText(g_myLan().save_parameters);
    g_infoWidget().delayShow();

    bool status = g_Runtime().save();

    g_infoWidget().hide();

    if(!status) {
        g_infoWidget().setLabelText(g_myLan().msg_params_save_error);
        g_infoWidget().delayShow();
        myFlow.sleep(2);
        g_infoWidget().hide();
        struCnfg.remoteCtrlIsRunning = 0;
        return;
    }
    //通知控制板延时断电
    char data[SEND_PACKET_RECTRL_BOARD_SIZE];
    memset(data, 0, SEND_PACKET_RECTRL_BOARD_SIZE);
    data[0] = ((int)(struCnfg.nDelayedShutdownTime))/256;
    data[1] = ((int)(struCnfg.nDelayedShutdownTime))%256;
    mySerial.pushCom2CmdData(CMD_UPPER_COMPUTER_OUTAGE_REQUEST, 0, BOARD_TYPE_CONTROL, 0x0e, data, 3);
    g_infoWidget().setLabelText(g_myLan().DelayPowerCut);
    g_infoWidget().delayShow();
    myFlow.sleep(3);
    g_infoWidget().hide();

    g_infoWidget().setLabelText(g_myLan().ids_shutdown);
    g_infoWidget().delayShow();

#ifdef Q_OS_UNIX
//    myMqttMsgParaseThread->n_uploadLogFileFlag = 1;
#endif
    myFlow.sleep(2);

    g_infoWidget().hide();
    struCnfg.remoteCtrlIsRunning = 0;
    //    this->close();
#ifndef Q_OS_WIN
    logger()->info("shutdown");
    struGsh.bFlagPowerOff = 1;
    myFlow.setTsBackLight(0);
    system("shutdown -h now");
#endif
}

void PlcAutoCtrlManager::sendStatusSlt(int power)
{
    myFlow.sendStatus(power);
}

void PlcAutoCtrlManager::sendMachineInfoSlt()
{
    char data[SEND_PACKET_RECTRL_BOARD_SIZE];
    memset(data, 0, SEND_PACKET_RECTRL_BOARD_SIZE);

    data[0] = 0;
    if(struCnfg.nMachineType == MACHINE_VERTICAL_DM_RG ||
            struCnfg.nMachineType == MACHINE_VERTICAL_DM_ZK)
    {
        data[0] = 0;  //大米
    }
    else if(struCnfg.nMachineType == MACHINE_VERTICAL_ZL_CS ||
            struCnfg.nMachineType == MACHINE_VERTICAL_ZL_CV ||
            struCnfg.nMachineType == MACHINE_LD_ZL_GP ||
            struCnfg.nMachineType == MACHINE_LD2_ZL_GP)
    {
        data[0] = 1;  //杂粮 机器
    }
    data[1] = struCnfg.nMachineAttr;  //Byte1：机型信息 0:立式，1：履带
    data[2] = struCnfg.nLayerTotal-1;   //Byte2：机型层数
    data[3] = struCnfg.nChuteTotal-1;   //Byte3：通道数数量
    data[4] = struCnfg.nChuteTotal-1;   //Byte4：振动器数量
    //data[5] = struCnfg.nWheelCtrl;      //Byte5：皮带控制

    mySerial.pushCom2CmdData(CMD_CTRL_BOARD_MACHINE_INFO_RET_PKG, 0, BOARD_TYPE_CONTROL, 0x0e, data, 3);
}

void PlcAutoCtrlManager::FeederMainValueSlt()
{
    if((struCnfp.bEnableBalanceFeed == 1)
            &&(struCnfg.stuFeedAutoCtrl.autoFeedMode == AUTO_FEED_MODE_0)
            &&(struCnfg.stuFeedAutoCtrl.isHasRadar == 1))
    {
        return;
    }
    else
    {
        for (int i = 0; i < MAX_FEEDER; i++) {
            struCnfp.struGroupCtrl[0].nFeederValueMain[i] = struCnfp.struGroupCtrl[0].nFeederValueMainPlc[i];
            logger()->info("adjust feed chute %1:%2",i,struCnfp.struGroupCtrl[0].nFeederValueMain[i]);
        }
        myFlow.resetFeederRG();
    }
}

void PlcAutoCtrlManager::FeederViceValueSlt()
{
    if((struCnfp.bEnableBalanceFeed == 1)
            &&(struCnfg.stuFeedAutoCtrl.autoFeedMode == AUTO_FEED_MODE_0)
            &&(struCnfg.stuFeedAutoCtrl.isHasRadar == 1))
    {
        return;
    }
    else
    {
        for (int i = 0; i < MAX_FEEDER; i++) {
            struCnfp.struGroupCtrl[1].nFeederValueMain[i] = struCnfp.struGroupCtrl[1].nFeederValueMainPlc[i];
            logger()->info("adjust feed chute %1:%2",i,struCnfp.struGroupCtrl[1].nFeederValueMain[i]);
        }
        myFlow.resetFeederRG();
    }
}

void PlcAutoCtrlManager::onValveClear()
{
    myFlow.resetAgeTestFre(0);
}
