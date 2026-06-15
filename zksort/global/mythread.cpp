#include <QDebug>
#include <QTimer>
#include "mythread.h"
#include "mqttsrv.h"
#include "myudpthread.h"

const int FlowSwitchHour = 36000;
updateStatusThread *myUpdateStatusThread;


/* ****************************************************************************
 *
 *                        系统时钟运行和清灰处理线程
 *
 *******************************************************************************/
updateStatusThread::updateStatusThread()
{
    memset(cmdData, 0, SEND_PACKET_DATA_SIZE);
    memset(m_alarminfo, 0, sizeof(m_alarminfo));
    memcpy(m_alarmFeedLevel,struGsh.struMachineStatus[0].bAlarmFeedLevel,sizeof(m_alarmFeedLevel));
    m_alarm_IPC = 0;

    curIpcIndex = 0;
    lastIpcIndex = 0;

    // connect(this, SIGNAL(sStartGetStaticsSig(int)), this, SLOT(getStaticsSlt(int)));
    connect(this,SIGNAL(setSignal()),this,SLOT(setSlots()));
    connect(this, SIGNAL(sCheckCurFeederLevelSig()), this, SLOT(checkCurFeederLevelSlt()));

    backlightOffCount = 0;

    alarmcount = 0;
    connect(this, SIGNAL(sGetIpcHeartSingle(int)), this, SLOT(getIpcHeartSingleSlt(int)));
    connect(this,SIGNAL(sGetIpcHeart()),this,SLOT(getIpcHeartSlt()));
    connect(this, SIGNAL(sGetIpcBadMaterial()), this, SLOT(getIpcBadMaterialSlt()));
    connect(this, SIGNAL(sGetIpcRunTestInfo()), this, SLOT(getIpcRunTestInfo()));


    connect(&g_Udp,SIGNAL(sGetChannelFeedIsOn(quint8)),this,SLOT(getChannelFeedIsOn(quint8)));

}

void updateStatusThread::run(void)
{
    while(this->isRunning()) {
        msleep(1000);
        systemClockRun();
    }

}

void updateStatusThread::systemClockRun()
{
    unsigned char i, j;
    unsigned int board;
    struGsh.nCounter++;
    struGsh.nRunCounter++;
    struGsh.nSortStartCount++;
    struGsh.nStaticCounter++;
    struCnfg.nCounter++;
    struGsh.nBacklightCounter++;

    /************************** 高优先级工作 ***************************/

    /* 更新系统时间 */
    emit sUpdateSystemTimeSig();

    /*****************************************************************/

    if (struGsh.bUpdateState) return; //远程升级停止低优先级工作

    /* 加密连续不关机，到期提示解密 */
    if(paramDelayCode.enable == 0xAA)
    {
        if((struGsh.nCounter%28800) == 0)    // 28800 = 3600*8
        {
            QString dateText = QDateTime::currentDateTime().toString("yyyyMMdd");
            int curTime = dateText.toInt(0,10);
            if(atoi(paramDelayCode.limit_date_cur) <= curTime)
            {
                emit sDccryptWarningSig();
            }
        }
        else
        {
            if((struGsh.nCounter%3600) == 3599)
            {
                emit sDccryptCodeWriteSig();
            }
        }
    }
    else if(paramDelayCode30Day.product30DayUseStartEnable == 0xAA)
    {
        if((struGsh.nCounter%3600) == 3599)
        {
            emit s30DayDccryptTimeWriteSig();
        }
    }
#ifdef Q_OS_UNIX
    /* 背光时间点亮时间超时 */
    if (struCnfg.nScreenSet && (struGsh.nBacklightCounter >= BACKLIGHT_TIMEOUT)
            && (struGsh.nBacklightStat == 1)&& (!struGsh.bFlagWiping)) {

        backlightOffCount++;
        if ((backlightOffCount == 1) && (struGsh.bSortStart == 0)){
            emit sInfoStartValve();
        }
        myFlow.setTsBackLight(0);
        struGsh.bFlagFirstPwdValid = 0;
        struGsh.bFlagSecondPwdValid = 0;
    }
#endif
    //    if((struGsh.bSortStart == 1)&&(struGsh.bFlagWiping == 0)){
    //        if(struCnfg.nAutoStaticsEnable == 1)
    if( (struGsh.bSortStart == 1)&&(struGsh.bFlagWiping == 0) &&
            (!myFlow.myNetWork->udpthread->isrunning) && (!struGsh.nRemoteImageCaptureIsRun))
    {
        if(struCnfg.nAutoStaticsEnable == 1 && struGsh.IpcStatisdataisrun == 0)
        {
            if(struGsh.nRunCounter>struCnfg.sTotalStaticsGetDuration)
            {
                if(struGsh.nCounter % struCnfg.sTotalStaticsGetDuration == 0) {
                    struGsh.IpcStatisdataisrun = 1;
                    emit sGetIpcBadMaterial();
                }
            }
        }
    }
    if(struCnfg.nUseIpcEnable == 1)
    {
        if(struGsh.bKeepAliveType == 1){
            if(struGsh.nCounter%5 == 0)
            {
                emit sGetIpcHeart();
            }
        }else{
            if(struGsh.nCounter%10 == 0)
            {
                emit sGetIpcHeart();
            }
        }
    }

    if(struGsh.nAlarmStautsCurrent[ALARM_CLASS]==1 )
    {
        alarmcount++;
        if(alarmcount == 10)
        {
            struGsh.nAlarmStautsCurrent[ALARM_CLASS]= 0;
            alarmcount = 0;
        }
    }

    if(struCnfp.struGroupCtrl[struGsh.nLayer].nWipeInterval ){
        if(((struGsh.nCounter%(60*struCnfp.struGroupCtrl[struGsh.nLayer].nWipeInterval) == 0)
            && (struGsh.bSortStart == 1)
            && (struGsh.bFlagAutowipe == 1)
            && (struCnfg.nWipeEnable == 1)
            && (struGsh.bAlarmStatus == 0)
            && (struGsh.bFlagWiping == 0)
            && (struGsh.nVavleTestIsRun == 0)
            && (struGsh.nVersionCaptureIsRun == 0)
            && (struGsh.nImageCaptureIsRun == 0)
            && (struGsh.nRemoteImageCaptureIsRun == 0)
            && (struGsh.bFlagGettingStatistic == 0)
            && (CUSTOM != CUSTOM_YANSI) )
                || (struGsh.bFlagManuwipe == 1)     //手动清灰
                || (struGsh.bAlarmNeedWipe == 1))   //吹气次数高的清灰
        {
            emit sWipeStartSig();
            struGsh.bFlagManuwipe = 0;
            struCnfg.ntotalWipeCount ++;
        }
        else if((struGsh.nCounter%2 == 0)
                &&(struGsh.nVavleTestIsRun == 0)
                &&(struGsh.nVersionCaptureIsRun == 0)
                //            &&(struGsh.bFlagWiping == 0)    //正在清灰中不读取报警
                &&(struGsh.bFlagAutowipe == 1) //23/10/9 防止连续两次处理报警冲突
                &&(struGsh.nImageCaptureIsRun == 0)/* 读取报警 每2秒钟1次, 喷阀自检时，不能查询*/
                &&(struGsh.nRemoteImageCaptureIsRun == 0)/* 远程采图时不能查询*/
                &&(struGsh.nValveBlowCaptureIsRun == 0)
                &&(struCnfg.bFlagFirstrun == 1))
        {
            if(struCnfg.alarmCmdType == 0)
                mySerial.pushCom1CmdData(CMD_CTRL_ALARM_CAPTURE, 0, BOARD_TYPE_CONTROL, 0, cmdData, 1);
            else if(struCnfg.alarmCmdType == 1)
                mySerial.pushCom1CmdData(CMD_CTRL_NEW_ALARM_CAPTURE, 0, BOARD_TYPE_CONTROL, 0, cmdData, 1);

            if(!struGsh.bFlagWiping)
            {
                /* 处理响应报警事件 */
                checkAlarmStatus();
                myFlow.resetControlAlarmRG();
            }
            if((struGsh.bSortStart == 1)
                    &&(struCnfp.bEnableBalanceFeed == 1)
                    &&(struGsh.isClearMode == false))    //色选状态+柔性流量，才判断料位
            {
                emit sCheckCurFeederLevelSig();
            }
        }
    }
}

/* 检查报警状态 */
void updateStatusThread::checkAlarmStatus()
{
    /*TODO: 根据报警回包，添加处理事件，如关闭下料、自动清灰等... */
    if(!struCnfg.nPlcControl){
        if((struGsh.bAlarmStatus == 1)&&(struGsh.bSortStart == 1))
        {
            emit sAlarmTurnOffSig();
        }
        else if((struGsh.bAlarmStatus == 0)&&(struGsh.bAlarmLastFeedoff == 1))
        {
            emit sAlarmTurnOnSig();
        }

        if ((memcmp(m_alarminfo,struGsh.nAlarmStautsCurrent,sizeof(m_alarminfo)) && struGsh.bStatEject == 1)
                ||( m_alarm_IPC != struGsh.struMachineStatus[struGsh.nLayer].nColorVoiceAlarmValue && struCnfg.nUseIpcEnable == 1))
        {
            memcpy(m_alarminfo,struGsh.nAlarmStautsCurrent,sizeof(m_alarminfo));
            m_alarm_IPC = struGsh.struMachineStatus[struGsh.nLayer].nColorVoiceAlarmValue;
#ifdef Q_OS_UNIX
//            myMqttMsgParaseThread->n_uploadAlarmFlag = 1;
#endif
        }
    }
    else { // 使能plc控制: 料位2-振动器开关，料位3-喷阀开关
        if (!g_infoWidget().isHidden()) return;
        struGsh.bAlarmFeedStaus[1] = (m_alarmFeedLevel[1] == struGsh.struMachineStatus[0].bAlarmFeedLevel[1]) ? 0 : 1;
        struGsh.bAlarmFeedStaus[2] = (m_alarmFeedLevel[2] == struGsh.struMachineStatus[0].bAlarmFeedLevel[2]) ? 0 : 1;
        if(struGsh.bAlarmFeedStaus[1] || struGsh.bAlarmFeedStaus[2])
        {
            m_alarmFeedLevel[1] = struGsh.struMachineStatus[0].bAlarmFeedLevel[1];
            m_alarmFeedLevel[2] = struGsh.struMachineStatus[0].bAlarmFeedLevel[2];
            if( struGsh.bAlarmFeedStaus[1] &&struGsh.struMachineStatus[0].bAlarmFeedLevel[1] && !struGsh.bSortStart){

                emit sAlarmTurnOnSig();
            }
            if(struGsh.bAlarmFeedStaus[2] &&struGsh.struMachineStatus[0].bAlarmFeedLevel[2] && !struGsh.bStatEject ){
                emit sAlarmTurnOnSig();

            }
            if(struGsh.bAlarmFeedStaus[1] && !struGsh.struMachineStatus[0].bAlarmFeedLevel[1]&& struGsh.bSortStart){

                emit sAlarmTurnOffSig();
            }

            if(struGsh.bAlarmFeedStaus[2] &&!struGsh.struMachineStatus[0].bAlarmFeedLevel[2] &&struGsh.bStatEject){
                emit sAlarmTurnOffSig();
            }
        }
        qDebug()<< struGsh.struMachineStatus[0].bAlarmFeedLevel[1]<<struGsh.bAlarmFeedStaus[1]<< struGsh.bSortStart;
    }
}

//长心跳和短心跳维持，不要修改
void updateStatusThread::getIpcHeart()
{
    emit sGetIpcHeart();
}



void updateStatusThread::checkCurFeederLevelSlt()
{
    int totalFeed;
    bool feedNeedReset;

    switch(struCnfg.stuFeedAutoCtrl.autoFeedMode)
    {
    case AUTO_FEED_MODE_0:
        feedNeedReset = myAutoFeed.checkCurRicePoSi();

        if((struCnfg.nMachineSpliceType == MACHINE_SPLICE_TYPE_BIG)&&(struCnfg.nChuteTotal > 1 || (struCnfg.nChuteTotal == 1 && !struCnfg.nAllInOne)))
        {
            totalFeed = struCnfg.nChuteTotal+1;
        }
        else
        {
            totalFeed = struCnfg.nChuteTotal;
        }
        if(feedNeedReset)
        {
            for(int i = 0; i < totalFeed; i++)
            {
                struGsh.feederLastRealSend[i] = struGsh.feederRealSend[i];
            }
            myFlow.resetFeederRG();
        }
        break;
    case AUTO_FEED_MODE_1:
        if(struCnfg.stuFeedAutoCtrl.isHasRadar == 1)
        {
            feedNeedReset = myAutoFeed.mode1CheckCurRicePoSi();
        }
        else if(struCnfg.stuFeedAutoCtrl.isHasRadar == 2)
        {
            feedNeedReset = myAutoFeed.modeSwitchCheckCurRicePoSi();
        }
        break;
    case AUTO_FEED_MODE_2:
        feedNeedReset = myAutoFeed.mode2CheckCurRicePoSi();
        break;
    case AUTO_FEED_MODE_3:
        feedNeedReset = myAutoFeed.mode3CheckCurRicePoSi();
        break;
    }

    if(feedNeedReset == true)
    {
        emit sRefreshBalFeedSig();
    }
    else if(myAutoFeed.getFreshFeedPage() == true)
    {
        emit sRefreshBalFeedSig();

        myAutoFeed.enbleFreshFeedPage(false);
    }
}

void updateStatusThread::getStaticsSlt(int groupIndex)
{
    //xiewj:21/12/22 清灰时不查询产量，清灰后首次查询数据舍弃
    if (struGsh.bFlagWiping){
        return;
    }
    return;

    int nInt=0, board, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];

    int nUnit = 0;
    bool getview = false;

    memset(data, 0, SEND_PACKET_DATA_SIZE);
    struGsh.bFlagGettingStatistic = 1;

    struGsh.nViewforstatic = 0;

    for(int j = 0; j < MAX_VIEW; j++)
    {
        if(struCnfg.struLayerInfo[0].nViewIsUsed[j])
        {
            for(int m = 0; m < MAX_GROUP_IDENTIFY; m++)
            {
                for(int n = 0; n < ARITHMETIC_TOTAL; n++)
                {
                    if(struCnfp.nGroupArithEnable[0][j][m][n] == 1)
                    {
                        struGsh.nViewforstatic = j;
                        getview = true;
                        break;
                    }
                }
            }
        }
        if(getview == true)
            break;
    }

    mySerial.pushCom1CmdData(CMD_CAMERA_STATICS_CAPTURE, 0, BOARD_TYPE_INTERFACE, 0, data, 1);
    myFlow.msleep(10);

    for(int i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(int j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if (!struCnfg.struLayerInfo[i].nViewIsUsed[j]) continue;
            if (struCnfp.nViewSensorType[0][j] != SENSOR_T1) continue; //只有T1支持产量含杂查询

            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            nUnit = 0;
            for(int k = 0; k < struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
            {
                for(int m = 0; m < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++) //
                {
                    if (struGsh.struVer.nUnitRes[i][j][nUnit+m] != 1) continue; //相机版本未查到即不查询产量信息

                    nUnitAddr = myFlow.getIdentifyGroupAddr(i, j, k, m);   //防止分割

                    if(nUnitAddr > 128) //克服通道分割
                    {
                        continue;
                    }
                    mySerial.pushCom1CmdData(CMD_CAMERA_STATICS_CAPTURE, nInt, board, nUnitAddr, data, 1);
                    //                        qDebug()<<"get chute flow:"<<struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nUnitGupId[m];
                    myFlow.msleep(10);
                }
                nUnit += struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal;
            }
        }
    }

    myFlow.sleep(1);

    int view = 0;
    if (!struGsh.bFlagWiping){
        struGsh.autoFeedEnabled = false;
        for(int i = 0;i < struCnfg.nLayerTotal;i++)
        {
            for(int j = 0;j < struCnfg.struLayerInfo[i].stuViewInfo[view].nIdentifyGroupTotal;j++)
            {
                if(struCnfp.nAutoFeed[i][j].bEnable == 1)
                {
                    struGsh.autoFeedEnabled = true;
                }
                else    //未开均衡的通道，也需要判断当前
                {
                    for(int k = 0;k < struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nGroupUnitTotal;k++){
                        //停运低于门限的料槽
                        int unitIndex = struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nUnitGupId[k];

                        if(struGsh.struStatics[i][view][unitIndex].nStaticsPeriod == 0 || struCnfp.n_coff == 0)
                            break;
                        //                        qDebug()<<"chute:"<<unitIndex;
                        if(struGsh.struStatics[i][view][unitIndex].nStaticsPeriod > struCnfg.sTotalStaticsGetDuration*13)
                        {
                            struGsh.bFlagGettingStatistic = 0;
                            return;
                        }
                        //                        m_flowOfChute[unitIndex] = (float)struGsh.struStatics[i][view][unitIndex].nThroughout/(float)struCnfp.n_coff*(float)FlowSwitchHour/(float)struGsh.struStatics[i][view][unitIndex].nStaticsPeriod;
                    }
                }
            }
        }
#if 1   //不管是否清灰过，产量均是正确的
        if(struGsh.autoFeedEnabled == true)
        {
            if(struCnfg.stuFeedAutoCtrl.isHasRadar == 1)
            {
                if((struCnfg.stuFeedAutoCtrl.autoFeedMode == AUTO_FEED_MODE_1))
                {
                    bool bResetFeed = myAutoFeed.mode1AutoFlowAdjust();
                    if(bResetFeed)
                    {
                        emit setSignal();
                    }
                }
                else
                {
                    if((struCnfg.stuFeedAutoCtrl.autoFeedMode == AUTO_FEED_MODE_2))
                    {
                        bool bResetFeed = myAutoFeed.mode2AutoFlowAdjust();
                        if(bResetFeed)
                        {
                            emit setSignal();
                        }
                    }
                }
            }
            else if(struCnfg.stuFeedAutoCtrl.isHasRadar == 2)
            {
                if((struCnfg.stuFeedAutoCtrl.autoFeedMode == AUTO_FEED_MODE_1))
                {
                    bool bResetFeed = myAutoFeed.modeSwitchAutoFlowAdjust();
                    if(bResetFeed)
                    {
                        emit setSignal();
                    }
                }
            }
        }
#endif
    }

    struGsh.bFlagGettingStatistic = 0;
}

void updateStatusThread::setSlots()
{
    myFlow.resetFeederRG();
}

void updateStatusThread::getIpcHeartSlt()
{
    if(struIpcShare.allIpcIsCalling == 1) return;
    struGsh.intefaceAlive ++;
    myFlow.getIpcInfo(true,0,0);
    emit sUpdateIpcInfo();
}

void updateStatusThread::getIpcHeartSingleSlt(int groupIndex)
{
    //    struIpcShare.ipcHeartStatus[groupIndex] = 0;
    //    g_Udp.pushUdpCmdData(CMD_UDP_IPC_REQ_STATUS, true, struCnfg.nIpcIpAddrBase+groupIndex, 0, 0, NULL);
}

void updateStatusThread::getIpcBadMaterialSlt()
{
    myFlow.getIpcCamBadPointsCount();
}

void updateStatusThread::getIpcRunTestInfo()
{
    //    qDebug()<<"ipcruntest request "<<struCnfg.ipc_run_test_count;
    //    if( struCnfg.ipc_run_test_count < 99)
    //        struCnfg.ipc_run_test_count ++;

    //    unsigned char data[16];
    //    memset(data, 0, 16);

    //    for(int i = 0; i < struCnfg.nIpcCount; i++)
    //    {
    //        struIpcShare.struIpcReqStauts[i].testNetHardwareInfoReq = IPC_REQ_RETURN_NULL;

    //        g_Udp.pushUdpCmdData(CMD_UDP_IPC_REQ_HARDWARE_INFO, true, struCnfg.nIpcIpAddrBase+i, 0, 1, data);

    //        for(int ii = 0; ii < 20; ii++)
    //        {
    //            myFlow.msleep(400);
    //            if(struIpcShare.struIpcReqStauts[i].testNetHardwareInfoReq == 1)
    //            {
    //                break;
    //            }
    //        }

    //        g_Udp.pushUdpCmdData(CMD_UDP_IPC_REQ_BAD_MATERIAL, true, struCnfg.nIpcIpAddrBase+i, 0, 0, NULL);
    //        myFlow.msleep(500);
    //    }
    //    emit gUpdateIpcRunTestInfo();

}

void updateStatusThread::getChannelFeedIsOn(quint8 ipcindex)
{
    if(struGsh.nRemoteImageCaptureIsRun) return; //22/6/18 远程那帮逼又在骚操作

    for(int view =0; view < MAX_VIEW; view++){
        if (struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[view] && (view %2 == 0)){
            for(int unit =0; unit<struCnfg.struLayerInfo[struGsh.nLayer].nViewUnitTotal[view];unit++){
                //                if(struGsh.nChuteFeedIsOn[view][unit]){
                if((struCnfg.struIpcCameraParams[struGsh.nLayer][view][unit].ipcType == ipcindex+1)
                        && (struGsh.bFlagWiping == 0) && (struGsh.bStatEject == 1))
                {
                    if(!myFlow.myNetWork->udpthread->isrunning){
                        struGsh.nRemoteImageCaptureIsRun = 1;
                        struGsh.nIpcAlarmImageCaptureIsRun = 1;
                        myFlow.myNetWork->setUdpRecvType(CAPTURE_IMAGE_TEMP,struGsh.nLayer,view,unit);
                        myFlow.myNetWork->udpthread->imgCorrectHight = 0;
                        myFlow.myNetWork->udpStart();
                        for(int m = 0;m < 6;m++){
                            if ( myFlow.myNetWork->udpthread->imgCorrectHight > 0){
                                break;
                            }
                            myFlow.msleep(500);
                        }
                        myFlow.myNetWork->udpStop();

                        if(myFlow.myNetWork->udpthread->materPropor > struCnfp.nIpcHasMatPixelPercert) //物料屏占比>有料下限 此时正常下料
                        {
                            struGsh.bAlarmIpcStatus[ipcindex] = 1;//有料才报警，防止换料时误报

                            if(struCnfg.nIpcAlarmDealMode == 1)
                            {
                                //                                struIpcShare.isIpcRestart[ipcindex] = 255;
                                //                                g_MainManager().SetBottomEnable(false);
                                //                                myFlow.setIpcRestart(ipcindex);
                            }

                            if((struCnfg.nIpcAlarmTurnOff == 1)&&(struGsh.bSortStart == 1))
                            {
                                myFlow.onOff();
                                g_MainManager().SendCurWidgetMsg(MSG_OFF);
                                g_MainManager().RefreshBottomStatus();
                            }

                            struGsh.nRemoteImageCaptureIsRun = 0;
                            struGsh.nIpcAlarmImageCaptureIsRun = 0;
                            return;
                        }
                        else
                        {
                            struGsh.bAlarmIpcStatus[ipcindex] = 0;
                        }
                    }
                }
                //                struGsh.nChuteFeedIsOn[view][unit] = 0;
            }
        }
    }

    struGsh.nRemoteImageCaptureIsRun = 0;
    struGsh.nIpcAlarmImageCaptureIsRun = 0;
    return;
}
