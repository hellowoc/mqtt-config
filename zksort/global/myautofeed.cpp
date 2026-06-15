#include "globalparams.h"
#include "myautofeed.h"

myAutoFeedSetClass myAutoFeed;
static int FlowSwitchHour = 36000;

myAutoFeedSetClass::myAutoFeedSetClass()
{
    countPerMin = 30;
    countLimitQuick = struCnfg.stuFeedAutoCtrl.feedLevelChangeWaitTime/2;
    countLimitSlow= 5;

//    countPerMin = 3;
//    countLimitQuick = 2;
//    countLimitSlow= 2;

    enbleFreshFeedPage(false);

    for(int i = 0; i < MAX_GROUP_TICK; i++)
    {
        clearBalStateCount(i);

        mode1ClearBalPartCount(i);

        modeSwitchClearBalStateCount(i);
        struFeedAutoConfigMode1[i].mode1BalancePart = MODE1_BALANCE_STAT_1;
    }
    mode2ClearBalStateCount();

    int view = struGsh.nViewforstatic;
    for(int i = 0;i < struCnfg.nLayerTotal;i++){
        for(int j = 0;j < struCnfg.struLayerInfo[i].stuViewInfo[view].nIdentifyGroupTotal;j++)
        {
            struFeedAutoConfigMode2.groupFlowTarget[j] = struCnfp.nAutoFeed[i][j].target_feeder;
        }
    }

    mode3ClearBalStateCount();
    struFeedAutoConfigMode3.lastRadarPosi = 1;
    struFeedAutoConfigMode3.curReadIndex = 0;
    struFeedAutoConfigMode3.levelMoreCount = 0;
    struFeedAutoConfigMode3.mode3BalLowPosiCount = 0;
}

void myAutoFeedSetClass::enbleFreshFeedPage(bool isEnable)
{
    needFreshFeedPage = isEnable;
}

bool myAutoFeedSetClass::getFreshFeedPage()
{
    return needFreshFeedPage;
}

/*
流量初始值公式：根据当前料位判定
  (Px-P2)/(P1-Px) = (F2-Fx)/(Fx-F1) ->
1. Case 均衡区域(P2,P1)    Fx = [F2*(P1-Px)+F1(Px-P2)]/(P1-P2)
2. Case 料多区域(P3，P2)    Fx = [F3*(P2-Px)+F2(Px-P3)]/(P2-P3)
3. Case 料满(0, P3)       Fx = 100
4. Case 料空(P1, Depth)   Fx = 20（最低流量通道20）

P3：满仓门限，默认250；P2:补偿门限，默认300；P1：空仓门限，默认Depth-100
F3：满仓流量，默认100；F2:补偿流量，默认90；F1：空仓流量，默认20
Px:当前料位，Fx，当前料位对应的流量
*/

void myAutoFeedSetClass::getCurFeedByLevel(int isAll, int index)
{
    #if 0
    int view = struGsh.nViewforstatic, totalFeed = 0;
//    int feedNeedReset = 0;  //需要重发流量
    int px = struGsh.struMachineStatus[struGsh.nLayer].nFeedLvelValue[0];
    int f1, f2, f3;
    int p1, p2, p3;
    int group_feed_total, totalFeed;

    p1 = struCnfg.stuFeedAutoCtrl.levelEmptyPosi;
    p2 = struCnfg.stuFeedAutoCtrl.levelMorePosi;
    p3 = struCnfg.stuFeedAutoCtrl.levelFullPosi;

    f1 = struCnfg.stuFeedAutoCtrl.feedEmpytLowLimitValue;
    f2 = struCnfg.stuFeedAutoCtrl.feedMoreStartValue;;
    f3 = 99;

    for(int group = 0; group < struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nViewforstatic].nIdentifyGroupTotal; i++)
    {
        if(struCnfp.nAutoFeed[struGsh.nLayer][group].bEnable == 1)
        {
            switch(index)
            {
            case STAT_LEVEL_EMPTY:
                struGsh.feedByRadar = f1;
                break;
            case STAT_LEVEL_BALANCE:
                struGsh.feedByRadar = (f2*(p1-px)+f1*(px-p2))/(p1-p2);
                break;
            case STAT_LEVEL_MORE:
                struGsh.feedByRadar = (f2*(p1-px)+f1*(px-p2))/(p1-p2);
                break;
            case STAT_LEVEL_FULL:
                struGsh.feedByRadar = 100;
                break;
            }

            if((struCnfg.nMachineSpliceType == MACHINE_SPLICE_TYPE_BIG)&&(struCnfg.nChuteTotal > 1 || (struCnfg.nChuteTotal == 1 && !struCnfg.nAllInOne)))
            {
                totalFeed = struCnfg.nChuteTotal+1;
            }
            else
            {
                totalFeed = struCnfg.nChuteTotal;
            }
            if(index == STAT_LEVEL_FULL)
            {
                for(int i = 0;i < totalFeed;i++)
                {
                    struGsh.feederRealSend[i] = struGsh.feedByRadar;
                }
            }
            else
            {
                for(int i = 0;i < struCnfg.nLayerTotal;i++){
                    for(int j = 0;j < struCnfg.struLayerInfo[i].stuViewInfo[view].nIdentifyGroupTotal;j++)
                    {
                        if((isAll == 0)&&(j > 0))
                        {
                            break;
                        }

                        for(int k = 0;k < struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nGroupUnitTotal;k++)
                        {
                            int unitIndex = struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nUnitGupId[k]%64;

                            struGsh.feederRealSend[unitIndex] = struGsh.feedByRadar+struCnfg.stuFeedAutoCtrl.nFeedValueBias[unitIndex]-100;
                            struGsh.feederRealSend[unitIndex] = (struGsh.feederRealSend[unitIndex] <= 100) ? struGsh.feederRealSend[unitIndex] : 100;
                            struGsh.feederRealSend[unitIndex] = (struGsh.feederRealSend[unitIndex] > struCnfg.stuFeedAutoCtrl.feedEmpytLowLimitValue) ? struGsh.feederRealSend[unitIndex] : struCnfg.stuFeedAutoCtrl.feedEmpytLowLimitValue;
                        }
                    }
                }
            }
        }
    }
#endif
}

void myAutoFeedSetClass::clearBalStateCount(int group)
{
#if 0
    struFeedAutoConfigNormal.feedEmptyCount = 0;
    struFeedAutoConfigNormal.feedFullCount = 0;
    struFeedAutoConfigNormal.levelMoreToNormalCount = 0;

    struFeedAutoConfigNormal.moreToBal2ndEnable = true;

    struFeedAutoConfigNormal.balanceStateChange = 1;
    for(int i = 0; i < 6; i++)
    {
        struFeedAutoConfigNormal.curCountBalStateChg[i] = 0;
    }
#endif
}

bool myAutoFeedSetClass::checkCurRicePoSi()
{
    bool feedNeedReset = false;
#if 0
    int i, totalFeed;
    int curLevel = struGsh.struMachineStatus[struGsh.nLayer].nFeedLvelValue[0];
    int depth = struCnfg.stuFeedAutoCtrl.riceContainerDepth;
    int curFeeder[MAX_FEEDER];
    bool feedNeedReset = false;
    int view = 0;

    switch(struGsh.curBalanceStateByLevel)
    {
    case STAT_LEVEL_EMPTY:   //S∈(L1，Depth], 默认（900，1050）
        if(curLevel <= struCnfg.stuFeedAutoCtrl.levelEmptyPosi)
        {
            if(struFeedAutoConfigNormal.curCountBalStateChg[STAT_LEVEL_EMPTY]++ >= countLimitQuick)
            {
                struGsh.lastBalanceStateByLevel = STAT_LEVEL_EMPTY;
                struGsh.curBalanceStateByLevel = STAT_LEVEL_BALANCE;
                clearBalStateCount();
                break;
            }
        }
        else
        {
            if(struFeedAutoConfigNormal.balanceStateChange == 1)
            {
                struFeedAutoConfigNormal.balanceStateChange = 0;
            }
            struFeedAutoConfigNormal.curCountBalStateChg[STAT_LEVEL_EMPTY] = 0;

            getCurFeedByLevel(1, STAT_LEVEL_EMPTY);

        }
        break;
    case STAT_LEVEL_BALANCE: //S∈(L2，Depth-L1]，默认（450， 900]
        if(curLevel > struCnfg.stuFeedAutoCtrl.levelEmptyPosi)
        {
            if(struFeedAutoConfigNormal.curCountBalStateChg[STAT_LEVEL_BALANCE]++ >= countLimitSlow)
            {
                struGsh.lastBalanceStateByLevel = STAT_LEVEL_BALANCE;
                struGsh.curBalanceStateByLevel = STAT_LEVEL_EMPTY;
                clearBalStateCount();
                break;
            }
        }
        else if(curLevel <= struCnfg.stuFeedAutoCtrl.levelMorePosi) //<=450
        {
            if(struFeedAutoConfigNormal.curCountBalStateChg[STAT_LEVEL_BALANCE]++ >= countLimitQuick)
            {
                struGsh.lastBalanceStateByLevel = STAT_LEVEL_BALANCE;
                struGsh.curBalanceStateByLevel = STAT_LEVEL_MORE;
                clearBalStateCount();
                break;
            }
        }
        else
        {
            if(struFeedAutoConfigNormal.balanceStateChange == 1)
            {
                struFeedAutoConfigNormal.balanceStateChange = 0;
            }
            struFeedAutoConfigNormal.curCountBalStateChg[STAT_LEVEL_BALANCE] = 0;

            if((struGsh.lastBalanceStateByLevel == STAT_LEVEL_MORE)&&(struFeedAutoConfigNormal.moreToBal2ndEnable == true))
            {
                if(struFeedAutoConfigNormal.levelMoreToNormalCount++ > struCnfg.stuFeedAutoCtrl.feedFullToNormalContinueTime*countPerMin)
                {
                    struFeedAutoConfigNormal.levelMoreToNormalCount = struCnfg.stuFeedAutoCtrl.feedFullToNormalContinueTime*countPerMin+1;
                    struFeedAutoConfigNormal.moreToBal2ndEnable = false;
                    getCurFeedByLevel(1, STAT_LEVEL_BALANCE);
                    break;
                }
                else
                {
                    getCurFeedByLevel(0, STAT_LEVEL_BALANCE);
                    break;
                }
            }

            getCurFeedByLevel(1, STAT_LEVEL_BALANCE);
        }
        break;
    case STAT_LEVEL_MORE:  //S∈(L3，L2]，默认（300， 450]
        if(curLevel <= struCnfg.stuFeedAutoCtrl.levelFullPosi) // <=300
        {
            if(struFeedAutoConfigNormal.curCountBalStateChg[STAT_LEVEL_MORE]++ >= countLimitQuick)
            {
                struGsh.lastBalanceStateByLevel = STAT_LEVEL_MORE;
                struGsh.curBalanceStateByLevel = STAT_LEVEL_FULL;
                clearBalStateCount();
                break;
            }
        }
        else if(curLevel > struCnfg.stuFeedAutoCtrl.levelMorePosi) // >450
        {
            if(struFeedAutoConfigNormal.curCountBalStateChg[STAT_LEVEL_MORE]++ >= countLimitSlow)
            {
                struGsh.lastBalanceStateByLevel = STAT_LEVEL_MORE;
                struGsh.curBalanceStateByLevel = STAT_LEVEL_BALANCE;
                clearBalStateCount();
                break;
            }
        }
        else
        {
            if(struFeedAutoConfigNormal.balanceStateChange == 1)
            {
                struFeedAutoConfigNormal.balanceStateChange = 0;
            }
            struFeedAutoConfigNormal.curCountBalStateChg[STAT_LEVEL_MORE] = 0;

            if((struGsh.lastBalanceStateByLevel == STAT_LEVEL_FULL)&&(struFeedAutoConfigNormal.moreToBal2ndEnable == true))
            {
                if(struFeedAutoConfigNormal.levelMoreToNormalCount++ > struCnfg.stuFeedAutoCtrl.feedFullToNormalContinueTime*countPerMin)
                {
                    struFeedAutoConfigNormal.levelMoreToNormalCount = struCnfg.stuFeedAutoCtrl.feedFullToNormalContinueTime*countPerMin+1;
                    struFeedAutoConfigNormal.moreToBal2ndEnable = false;
                    getCurFeedByLevel(1, STAT_LEVEL_MORE);
                    break;
                }
                else
                {
                    getCurFeedByLevel(0, STAT_LEVEL_MORE);
                    break;
                }
            }

            getCurFeedByLevel(1, STAT_LEVEL_MORE);
        }
        break;
    case STAT_LEVEL_FULL: //S∈ (0，L3]，默认（0， 300）
        if(curLevel > struCnfg.stuFeedAutoCtrl.levelFullPosi) // >300
        {
            if(struFeedAutoConfigNormal.curCountBalStateChg[STAT_LEVEL_FULL]++ >= countLimitSlow)
            {
                struGsh.lastBalanceStateByLevel = STAT_LEVEL_FULL;
                struGsh.curBalanceStateByLevel = STAT_LEVEL_MORE;
                clearBalStateCount();
                break;
            }
        }
        else
        {
            if(struFeedAutoConfigNormal.balanceStateChange == 1)
            {
                struFeedAutoConfigNormal.balanceStateChange = 0;
            }
            struFeedAutoConfigNormal.curCountBalStateChg[STAT_LEVEL_FULL] = 0;

            getCurFeedByLevel(1, STAT_LEVEL_FULL);
        }

        break;
    }

    if((struCnfg.nMachineSpliceType == MACHINE_SPLICE_TYPE_BIG)&&(struCnfg.nChuteTotal > 1 || (struCnfg.nChuteTotal == 1 && !struCnfg.nAllInOne)))
    {
        totalFeed = struCnfg.nChuteTotal+1;
    }
    else
    {
        totalFeed = struCnfg.nChuteTotal;
    }
    feedNeedReset = false;
    for(i = 0; i < totalFeed; i++)
    {
        if(struGsh.feederLastRealSend[i] != struGsh.feederRealSend[i])
        {
            qDebug("%d-last is %d, now is %d", i, struGsh.feederLastRealSend[i],struGsh.feederRealSend[i]);
            feedNeedReset = true;
            break;
        }
    }        

    if((struGsh.curBalanceStateByLevel == STAT_LEVEL_EMPTY) || (struGsh.curBalanceStateByLevel == STAT_LEVEL_FULL))
    {
        struGsh.isRadarStateAlarm = true;
    }
    else
    {
        struGsh.isRadarStateAlarm = false;
    }
#endif
    return feedNeedReset;
}

bool myAutoFeedSetClass::autoFlowAdjust()
{
    bool bResetFeed = false;//是否需要发送给料参数
#if 0
    struFeedAutoConfigNormal.isInBalancing = true;

    bool bResetFeed = false;//是否需要发送给料参数
    int view = 0;//使用前视计算,还有添加
    for(int i = 0;i < struCnfg.nLayerTotal;i++){
        for(int j = 0;j < struCnfg.struLayerInfo[i].stuViewInfo[view].nIdentifyGroupTotal;j++)
        {
            if(struCnfp.nAutoFeed[i][j].bEnable == 1){
//               struGsh.autoFeedEnabled = true;
                for(int k = 0;k < struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nGroupUnitTotal;k++){
                    //停运低于门限的料槽
                    int unitIndex = struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nUnitGupId[k];

                    if(struGsh.struStatics[i][view][unitIndex].nStaticsPeriod == 0 || struCnfp.n_coff == 0)
                        break;
//                    qDebug()<<"chute:"<<unitIndex;
                    if(struGsh.struStatics[i][view][unitIndex].nStaticsPeriod > struCnfg.sTotalStaticsGetDuration*13)
                        return false;
                    int flow = (float)struGsh.struStatics[i][view][unitIndex].nThroughout/(float)struCnfp.n_coff*(float)FlowSwitchHour/(float)struGsh.struStatics[i][view][unitIndex].nStaticsPeriod;

                    int offset;
                    offset = flow - struCnfp.nAutoFeed[i][j].target_feeder;

                    if(qAbs(offset) > 60){//目标产量与实际产量相差超过 60kg/h才进行调节
                        int quo = 200;
                        quo = (struCnfp.nAutoFeed[i][j].bQuickBalance == 0) ? 200 : 100;
                        int step = 0;
                        if(qAbs(offset) > 800)
                            step = offset/quo;
                        else if(qAbs(offset) > 600)
                            step = offset/quo;
                        else if(qAbs(offset) > 400)
                            step = offset/quo;
                        else if(qAbs(offset) > 200)
                            step = offset/quo;
                        else if(qAbs(offset) > 100)
                            step = offset/100;
                        else if(qAbs(offset) > 20)
                            step = 1;
                        else
                            step = 0;
                        step = qAbs(step);
                        if(step != 0){
                            if(offset > 0){
                                if((struGsh.chuteFeedMainValue[unitIndex]-step) > 0){
                                    struGsh.chuteFeedMainValue[unitIndex]-=step;
                                }
                            }else{
                                if((struGsh.chuteFeedMainValue[unitIndex]+step) < 100){
                                    struGsh.chuteFeedMainValue[unitIndex]+=step;
                                }
                            }
                            bResetFeed = true;
                        }
                    }
#if 0
//                    qDebug()<<"current flow:"<<flow;
                    if(struGsh.chuteStopStateByDuration[unitIndex]){//判断此通道是否处于停运状态
//                        qDebug()<<"in stop state";
//                        qDebug()<<"stop time"<< struGsh.chuteStopDuration[unitIndex];
                        if(struGsh.chuteStopDuration[unitIndex] >= struCnfp.nAutoFeed[i][j].stop_duration){//停运时间到达
//                            qDebug()<<"stop time out";
                            struGsh.chuteStopStateByDuration[unitIndex] = 0;
                            struGsh.chuteFeedMainValue[unitIndex] = struGsh.chuteFeederBeforeStop[unitIndex];
                            bResetFeed = true;
                        }
                    }else{
                        if(flow < struCnfp.nAutoFeed[i][j].stop_feeder){//判断产量是否到达停运临界值
//                                struGsh.chuteFeederBeforeStop[unitIndex] = struCnfp.struGroupCtrl[0].nFeederValueMain[unitIndex];
                            struGsh.chuteFeedMainValue[unitIndex] = 0;
                            struGsh.chuteStopDuration[unitIndex] = 0;
                            struGsh.chuteStopStateByDuration[unitIndex] = 1;
                            bResetFeed = true;
                        }
                    }

                    if(flow > struCnfp.nAutoFeed[i][j].start_feeder){//高于匀料门限
                        if(struGsh.chuteAjustState[unitIndex] == 0){
                            struGsh.chuteFeedMainValue[unitIndex] = struCnfp.struGroupCtrl[0].nFeederValueMain[unitIndex];
//                            qDebug()<<"feeder Index"<<unitIndex;
//                            qDebug()<<"in adjust state";
                            struGsh.chuteAjustState[unitIndex] = 1;

                        }else{
                            int offset;
                            offset = flow - struCnfp.nAutoFeed[i][j].target_feeder;

                            if(qAbs(offset) > 60){//目标产量与实际产量相差超过 60kg/h才进行调节
                                int quo = 200;
                                quo = (struCnfp.nAutoFeed[i][j].bQuickBalance == 0) ? 200 : 100;
                                int step = 0;
                                if(qAbs(offset) > 800)
                                    step = offset/quo;
                                else if(qAbs(offset) > 600)
                                    step = offset/quo;
                                else if(qAbs(offset) > 400)
                                    step = offset/quo;
                                else if(qAbs(offset) > 200)
                                    step = offset/quo;
                                else if(qAbs(offset) > 100)
                                    step = offset/100;
                                else if(qAbs(offset) > 20)
                                    step = 1;
                                else
                                    step = 0;
                                step = qAbs(step);
                                if(step != 0){
                                    if(offset > 0){
                                        if((struGsh.chuteFeedMainValue[unitIndex]-step) > 0){
                                            struGsh.chuteFeedMainValue[unitIndex]-=step;
                                        }
                                    }else{
                                        if((struGsh.chuteFeedMainValue[unitIndex]+step) < 100){
                                            struGsh.chuteFeedMainValue[unitIndex]+=step;
                                        }
                                    }
//                                    qDebug()<<"ajust feeder";
//                                    qDebug()<<struGsh.chuteFeedMainValue[unitIndex];
                                    bResetFeed = true;
                                }
                            }
                        }
                    }else{
                        struGsh.chuteAjustState[unitIndex] = 0;
                    }
#endif
                }
            }
        }
    }
    if(bResetFeed){
        for(int i = 0;i < struCnfg.nLayerTotal;i++){
            for(int j = 0;j < struCnfg.struLayerInfo[i].stuViewInfo[view].nIdentifyGroupTotal;j++)
            {
                if(struCnfp.nAutoFeed[i][j].bEnable == 1){
                    for(int k = 0;k < struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nGroupUnitTotal;k++){
                        //停运低于门限的料槽
                        int unitIndex = struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nUnitGupId[k]%64;

                        struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[unitIndex] = struGsh.chuteFeedMainValue[unitIndex];
                        struGsh.chuteFeedAutoLastValue[unitIndex] = struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[unitIndex];
                    }
                }
            }
        }

    }

    struFeedAutoConfigNormal.isInBalancing = false;

#endif
    return bResetFeed;
}

/********************************************************************************************************
 *
 * mode1:  对应界面模式2，雷达传感器，带自动料位监测+均衡供料， 多档数目标
 *
 * 根据目标值，分段目标
 * step 1: mode1CheckCurRicePoSi(), 根据料位，判断处于料空、均衡、料满的哪个阶段，需要连续几次才确定，忽略干扰
 * step 2:mode1ClearBalStateCount(), 状态切换时，需要清除相关变量和计数器
 * step 3:mode1ResetFeed(), 状态切换时，料空、均衡、料满发生状态切换时，需要流量值复位,包括二三四次
 * step 4:mode1GetCurBalancePartPoSi(), 均衡状态下，，根据料位，判断当前均衡目标值属于哪个 分段，从而调整均衡的目标值
 * step 5:mode1AutoFlowAdjust(), 均衡状态下，，根据不同分段的均衡目标值，调整各通道流量，维持在均衡目标值
 * ******************************************************************************************************/

void myAutoFeedSetClass::getCurRadarLevelMode1WhileTurnOn()
{
    int view = struGsh.nViewforstatic;
    int px;
    int f1, f2, f3;
    int p1, p2, p3;
    int group_feed_total, totalFeed;

    p1 = struCnfg.stuFeedAutoCtrl.levelEmptyPosi;
    p2 = struCnfg.stuFeedAutoCtrl.levelFullPosi;


    if((struCnfg.nMachineSpliceType == MACHINE_SPLICE_TYPE_BIG)&&(struCnfg.nChuteTotal > 1 || (struCnfg.nChuteTotal == 1 && !struCnfg.nAllInOne)))
    {
        totalFeed = struCnfg.nChuteTotal+1;
    }
    else
    {
        totalFeed = struCnfg.nChuteTotal;
    }

    for(int group = 0; group < struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nViewforstatic].nIdentifyGroupTotal; group++)
    {
        if(struCnfp.nAutoFeed[struGsh.nLayer][group].bEnable == 1)
        {
            px = struGsh.struMachineStatus[struGsh.nLayer].nFeedLvelValue[group];
            if(px < struCnfg.stuFeedAutoCtrl.levelFullPosi)    //料满
            {
                 struFeedAutoConfigMode1[group].curBalanceStateByLevel = STAT_LEVEL_FULL_MODE1;
                 group_feed_total = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nViewforstatic].stuIdenGupInfo[group].nGroupUnitTotal;
                 for(int ff = 0;ff < group_feed_total;ff++)
                 {
                     int feedId = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nViewforstatic].stuIdenGupInfo[group].nUnitGupId[ff]%64;
                     struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = 100;
                     if(struGsh.chuteFeedAutoLastValue[feedId] == 100)
                     {
                        struGsh.chuteFeedAutoLastValue[feedId] = 95;
                     }
                 }
            }
            else if(px < struCnfg.stuFeedAutoCtrl.levelEmptyPosi)   // 料匀
            {
                struFeedAutoConfigMode1[group].curBalanceStateByLevel = STAT_LEVEL_BALANCE_MODE1;

                switch(struCnfg.stuFeedAutoCtrl.mode1FeedTargetPartCount)
                {
                case 1:
                    struFeedAutoConfigMode1[group].mode1BalancePart = MODE1_BALANCE_STAT_1;
                    break;
                case 2:
                    if(px <= struFeedAutoConfigMode1[group].mode1FeedBalPartScale[MODE1_BALANCE_STAT_1])
                    {
                        struFeedAutoConfigMode1[group].mode1BalancePart = MODE1_BALANCE_STAT_1;
                    }
                    else
                    {
                        struFeedAutoConfigMode1[group].mode1BalancePart = MODE1_BALANCE_STAT_2;
                    }
                    break;
                case 3:
                    if(px <= struFeedAutoConfigMode1[group].mode1FeedBalPartScale[MODE1_BALANCE_STAT_1])
                    {
                        struFeedAutoConfigMode1[group].mode1BalancePart = MODE1_BALANCE_STAT_1;
                    }
                    else if(px <= struFeedAutoConfigMode1[group].mode1FeedBalPartScale[MODE1_BALANCE_STAT_2])
                    {
                        struFeedAutoConfigMode1[group].mode1BalancePart = MODE1_BALANCE_STAT_2;
                    }
                    else
                    {
                        struFeedAutoConfigMode1[group].mode1BalancePart = MODE1_BALANCE_STAT_3;
                    }
                    break;
                case 4:
                    if(px <= struFeedAutoConfigMode1[group].mode1FeedBalPartScale[MODE1_BALANCE_STAT_1])
                    {
                        struFeedAutoConfigMode1[group].mode1BalancePart = MODE1_BALANCE_STAT_1;
                    }
                    else if(px <= struFeedAutoConfigMode1[group].mode1FeedBalPartScale[MODE1_BALANCE_STAT_2])
                    {
                        struFeedAutoConfigMode1[group].mode1BalancePart = MODE1_BALANCE_STAT_2;
                    }
                    else if(px <= struFeedAutoConfigMode1[group].mode1FeedBalPartScale[MODE1_BALANCE_STAT_3])
                    {
                        struFeedAutoConfigMode1[group].mode1BalancePart = MODE1_BALANCE_STAT_3;
                    }
                    else
                    {
                        struFeedAutoConfigMode1[group].mode1BalancePart = MODE1_BALANCE_STAT_4;
                    }
                    break;
                }
            }
            else // 料空
            {
                struFeedAutoConfigMode1[group].curBalanceStateByLevel = STAT_LEVEL_EMPTY_MODE1;
                group_feed_total = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nViewforstatic].stuIdenGupInfo[group].nGroupUnitTotal;
                for(int ff = 0;ff < group_feed_total;ff++)
                {
                    int feedId = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nViewforstatic].stuIdenGupInfo[group].nUnitGupId[ff]%64;
                    struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struCnfg.stuFeedAutoCtrl.feedEmpytLowLimitValue;
                }
            }
        }
    }
}

/* step 1：料位状态切换料空、均衡、料满*/
bool myAutoFeedSetClass::mode1CheckCurRicePoSi()
{
    bool feedNeedReset[MAX_GROUP_TICK];
    for(int i =0; i < MAX_GROUP_TICK; i++)
    {
        feedNeedReset[i] = false;
    }

 //   int view = 0;

    countLimitQuick = struCnfg.stuFeedAutoCtrl.feedLevelChangeWaitTime/2;

    for(int group = 0; group < struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nViewforstatic].nIdentifyGroupTotal; group++)
    {
        if(struCnfp.nAutoFeed[struGsh.nLayer][group].bEnable == 1)
        {
            int curLevel = struGsh.struMachineStatus[struGsh.nLayer].nFeedLvelValue[group];
            switch(struFeedAutoConfigMode1[group].curBalanceStateByLevel)
            {
            case STAT_LEVEL_EMPTY_MODE1:   //S∈(L1，Depth], 默认（900，1050）
                if(curLevel <= struCnfg.stuFeedAutoCtrl.levelEmptyPosi)
                {
                    if(struFeedAutoConfigMode1[group].curCountBalStateChg[STAT_LEVEL_EMPTY_MODE1]++ >= countLimitQuick)
                    {
                        struFeedAutoConfigMode1[group].curBalanceStateByLevel = STAT_LEVEL_BALANCE_MODE1;
                        mode1ClearBalStateCount(group);
//                        mode1StateChangeToResetFeed(STAT_LEVEL_BALANCE_MODE1, false, false);
                        struFeedAutoConfigMode1[group].mode1FeedStateChangeToBal = 2;
                        feedNeedReset[group] = true;
                        break;
                    }
                }
                else
                {
                    feedNeedReset[group] = false;
                    struFeedAutoConfigMode1[group].curCountBalStateChg[STAT_LEVEL_EMPTY_MODE1] = 0;
                }
                break;
            case STAT_LEVEL_BALANCE_MODE1: //S∈(L2，Depth-L1]，默认（450， 900]
                if(curLevel > struCnfg.stuFeedAutoCtrl.levelEmptyPosi)
                {
                    if(struFeedAutoConfigMode1[group].curCountBalStateChg[STAT_LEVEL_BALANCE_MODE1]++ >= countLimitQuick)
                    {
                        struFeedAutoConfigMode1[group].curBalanceStateByLevel = STAT_LEVEL_EMPTY_MODE1;
                        mode1ClearBalStateCount(group);
//                        mode1StateChangeToResetFeed(STAT_LEVEL_EMPTY_MODE1, false, false);
                        feedNeedReset[group] = true;
                        break;
                    }
                }
                else if(curLevel <= struCnfg.stuFeedAutoCtrl.levelFullPosi) //<=450
                {
                    if(struFeedAutoConfigMode1[group].curCountBalStateChg[STAT_LEVEL_BALANCE_MODE1]++ >= countLimitQuick)
                    {
                        struFeedAutoConfigMode1[group].curBalanceStateByLevel = STAT_LEVEL_FULL_MODE1;  //取消预满20220801
                        mode1ClearBalStateCount(group);
                        feedNeedReset[group] = true;
                        break;
                    }
                }
                else
                {
                    feedNeedReset[group] = false;
                    struFeedAutoConfigMode1[group].curCountBalStateChg[STAT_LEVEL_BALANCE_MODE1] = 0;
                    mode1GetCurBalancePartPoSi(group, curLevel);
                }
                break;
            case STAT_LEVEL_PRE_FULL_MODE1: //S∈ (0，L3]，默认（0， 300）  //此状态不用20220801
                if(curLevel > struCnfg.stuFeedAutoCtrl.levelFullPosi) // >300
                {
                    if(struFeedAutoConfigMode1[group].curCountBalStateChg[STAT_LEVEL_PRE_FULL_MODE1]++ >= countLimitQuick)
                    {
                        struFeedAutoConfigMode1[group].curBalanceStateByLevel = STAT_LEVEL_BALANCE_MODE1;
                        mode1ClearBalStateCount(group);
                        struFeedAutoConfigMode1[group].mode1FeedStateChangeToBal = 1;
//                        mode1StateChangeToResetFeed(STAT_LEVEL_BALANCE_MODE1, false, false);
                        feedNeedReset[group] = true;
                        break;
                    }
                }
                else
                {
                    if(struFeedAutoConfigMode1[group].curCountBalStateChg[STAT_LEVEL_PRE_FULL_MODE1]++ >= struCnfg.stuFeedAutoCtrl.feedLevelPreFullWaitTime/2)
                    {
                        struFeedAutoConfigMode1[group].curBalanceStateByLevel = STAT_LEVEL_FULL_MODE1;
                        mode1ClearBalStateCount(group);
//                        mode1StateChangeToResetFeed(STAT_LEVEL_FULL_MODE1, false, false);
                        feedNeedReset[group] = false;
                        break;
                    }
                }
                struFeedAutoConfigMode1[group].curCountBalStateChg[STAT_LEVEL_PRE_FULL_MODE1] = 0;
                break;
            case STAT_LEVEL_FULL_MODE1: //S∈ (0，L3]，默认（0， 300）
                if(curLevel > struCnfg.stuFeedAutoCtrl.levelFullPosi) // >300
                {
                    if(struFeedAutoConfigMode1[group].curCountBalStateChg[STAT_LEVEL_FULL_MODE1]++ >= countLimitQuick)
                    {
                        struFeedAutoConfigMode1[group].curBalanceStateByLevel = STAT_LEVEL_BALANCE_MODE1;
                        mode1ClearBalStateCount(group);
                        struFeedAutoConfigMode1[group].mode1FeedStateChangeToBal = 1;

                        feedNeedReset[group] = true;
                        break;
                    }
                }
                else
                {
                    feedNeedReset[group] = false;
                    struFeedAutoConfigMode1[group].curCountBalStateChg[STAT_LEVEL_FULL_MODE1] = 0;
                }

                break;
            }
        }
    }


    struGsh.isRadarStateAlarm = false;
    {
        for(int group = 0; group < struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nViewforstatic].nIdentifyGroupTotal; group++)
        {
            if((struFeedAutoConfigMode1[group].curBalanceStateByLevel == STAT_LEVEL_EMPTY_MODE1)
                        || (struFeedAutoConfigMode1[group].curBalanceStateByLevel == STAT_LEVEL_FULL_MODE1))
            {
                struGsh.isRadarStateAlarm = true;
            }
        }
    }

    bool needReset = false;

    for(int group = 0; group < struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nViewforstatic].nIdentifyGroupTotal; group++)
    {        
        if(struCnfp.nAutoFeed[struGsh.nLayer][group].bEnable == 1)
        {
            if(feedNeedReset[group] = true)
            {
                needReset = true;
            }

            if(struFeedAutoConfigMode1[group].mode1FeedStateChangeToBal > 0)    //处于料满->均衡， 或料空->均衡
            {
                needReset = true;
                break;
            }
        }
    }
    if(needReset == true)
    {
        mode1StateChangeToResetFeed(STAT_LEVEL_BALANCE_MODE1, false, false);    //STAT_LEVEL_BALANCE_MODE1 不需要了
    }

    return needReset;
}

void myAutoFeedSetClass::mode1ClearBalStateCount(int group)
{
    struFeedAutoConfigMode1[group].feedEmptyCount = 0;
    struFeedAutoConfigMode1[group].feedFullCount = 0;
    struFeedAutoConfigMode1[group].levelMoreToNormalCount = 0;

    struFeedAutoConfigMode1[group].moreToBal2ndEnable = true;

    for(int i = 0; i < 6; i++)
    {
        struFeedAutoConfigMode1[group].curCountBalStateChg[i] = 0;
        struFeedAutoConfigMode1[group].curCountBalChgClock[i] = 0;
    }

    int aveScale = (struCnfg.stuFeedAutoCtrl.levelEmptyPosi-struCnfg.stuFeedAutoCtrl.levelFullPosi)/struCnfg.stuFeedAutoCtrl.mode1FeedTargetPartCount;
    if(struCnfg.stuFeedAutoCtrl.mode1FeedTargetPartCount == 1)
    {
        struFeedAutoConfigMode1[group].mode1FeedBalPartScale[MODE1_BALANCE_STAT_1] = struCnfg.stuFeedAutoCtrl.levelFullPosi;
    }
    else if(struCnfg.stuFeedAutoCtrl.mode1FeedTargetPartCount == 2)
    {
        struFeedAutoConfigMode1[group].mode1FeedBalPartScale[MODE1_BALANCE_STAT_1] = struCnfg.stuFeedAutoCtrl.levelEmptyPosi-aveScale;
        struFeedAutoConfigMode1[group].mode1FeedBalPartScale[MODE1_BALANCE_STAT_2] = struCnfg.stuFeedAutoCtrl.levelEmptyPosi;
    }
    else if(struCnfg.stuFeedAutoCtrl.mode1FeedTargetPartCount == 3)
    {
        struFeedAutoConfigMode1[group].mode1FeedBalPartScale[MODE1_BALANCE_STAT_1] = struCnfg.stuFeedAutoCtrl.levelEmptyPosi-aveScale*2;
        struFeedAutoConfigMode1[group].mode1FeedBalPartScale[MODE1_BALANCE_STAT_2] = struCnfg.stuFeedAutoCtrl.levelEmptyPosi-aveScale;
        struFeedAutoConfigMode1[group].mode1FeedBalPartScale[MODE1_BALANCE_STAT_3] = struCnfg.stuFeedAutoCtrl.levelEmptyPosi;
    }
    else if(struCnfg.stuFeedAutoCtrl.mode1FeedTargetPartCount == 4)
    {
        struFeedAutoConfigMode1[group].mode1FeedBalPartScale[MODE1_BALANCE_STAT_1] = struCnfg.stuFeedAutoCtrl.levelEmptyPosi-aveScale*3;
        struFeedAutoConfigMode1[group].mode1FeedBalPartScale[MODE1_BALANCE_STAT_2] = struCnfg.stuFeedAutoCtrl.levelEmptyPosi-aveScale*2;
        struFeedAutoConfigMode1[group].mode1FeedBalPartScale[MODE1_BALANCE_STAT_3] = struCnfg.stuFeedAutoCtrl.levelEmptyPosi-aveScale*1;
        struFeedAutoConfigMode1[group].mode1FeedBalPartScale[MODE1_BALANCE_STAT_4] = struCnfg.stuFeedAutoCtrl.levelEmptyPosi;
    }

//    qDebug("111 struFeedAutoConfigMode1.curBalanceStateByLevel is %d", struFeedAutoConfigMode1[group].curBalanceStateByLevel);
//    qDebug("222 struFeedAutoConfigMode1.mode1BalancePart is %d",struFeedAutoConfigMode1[group].mode1BalancePart);
//    qDebug("333 current target  is %d",struCnfp.nAutoFeed[0][0].mode1FeedTargetPart[struFeedAutoConfigMode1[group].mode1BalancePart]);
}

/* 清零均衡各阶段计数器*/
void myAutoFeedSetClass::mode1ClearBalPartCount(int group)
{
    for(int i = 0; i < AUTO_FEED_TARGET_PART; i++)
    {
        struFeedAutoConfigMode1[group].mode1CountBalPartChg[i] = 0;
    }
//    qDebug("444 current target  is %d",struCnfp.nAutoFeed[0][0].mode1FeedTargetPart[struFeedAutoConfigMode1[group].mode1BalancePart]);
}

/* 模式1，料位变化，导致状态料空、均衡、料满发生状态切换时，需要流量复位,包括二三四次
needDelay：用于判断二三四次，是否需要迅速响应

*/
void myAutoFeedSetClass::mode1StateChangeToResetFeed(int index, bool isChangeToBal, bool needDelay)
{
    int view = 0, totalFeed = 0;
    int group_feed_total;

    if((struCnfg.nMachineSpliceType == MACHINE_SPLICE_TYPE_BIG)&&(struCnfg.nChuteTotal > 1 || (struCnfg.nChuteTotal == 1 && !struCnfg.nAllInOne)))
    {
        totalFeed = struCnfg.nChuteTotal+1;
    }
    else
    {
        totalFeed = struCnfg.nChuteTotal;
    }
    view = struGsh.nViewforstatic;//使用前视计算,还有添加

    for(int group = 0; group < struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].nIdentifyGroupTotal; group++)
    {
        if(struCnfp.nAutoFeed[struGsh.nLayer][group].bEnable == 1)
        {
            group_feed_total = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].stuIdenGupInfo[group].nGroupUnitTotal;

            switch(struFeedAutoConfigMode1[group].curBalanceStateByLevel)
            {
            case STAT_LEVEL_EMPTY_MODE1:
                if(myAutoFeed.struFeedAutoConfigMode1[group].curCountBalChgClock[STAT_LEVEL_EMPTY_MODE1]++ >= struCnfg.stuFeedAutoCtrl.feedBalAdjustDuration/2)
                {
                    for(int ff = 0;ff < group_feed_total;ff++)
                    {
                        int feedId = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].stuIdenGupInfo[group].nUnitGupId[ff]%64;

                        if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] > struCnfg.stuFeedAutoCtrl.feedEmpytLowLimitValue)
                        {
                            struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] -= struCnfg.stuFeedAutoCtrl.feedBalAdjustStep;
                            if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] <= struCnfg.stuFeedAutoCtrl.feedEmpytLowLimitValue)
                            {
                                struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struCnfg.stuFeedAutoCtrl.feedEmpytLowLimitValue;
                            }
                        }
//                        qDebug("STAT_LEVEL_EMPTY_MODE1 group = %d, feed[%d] is %d", group, feedId, struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId]);
                    }
                    myAutoFeed.struFeedAutoConfigMode1[group].curCountBalChgClock[STAT_LEVEL_EMPTY_MODE1] = 0;
                }
                break;
            case STAT_LEVEL_BALANCE_MODE1:
                if(struFeedAutoConfigMode1[group].mode1FeedStateChangeToBal > 0)
                {
                    bool needChange = true;

                    if(myAutoFeed.struFeedAutoConfigMode1[group].curCountBalChgClock[STAT_LEVEL_BALANCE_MODE1]++ >= struCnfg.stuFeedAutoCtrl.feedBalAdjustDuration/2)
                    {
                        for(int ff = 0;ff < group_feed_total;ff++)
                        {
                            int feedId = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].stuIdenGupInfo[group].nUnitGupId[ff]%64;

                            if(struFeedAutoConfigMode1[group].mode1FeedStateChangeToBal == 1)   //料满-》均衡
                            {
                                if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] > struGsh.chuteFeedAutoLastValue[feedId])
                                {
                                    struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] -= struCnfg.stuFeedAutoCtrl.feedBalAdjustStep;
                                    if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] <= struGsh.chuteFeedAutoLastValue[feedId])
                                    {
                                        struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struGsh.chuteFeedAutoLastValue[feedId];
                                        needChange = false;
                                    }
                                }
                            }
                            else if(struFeedAutoConfigMode1[group].mode1FeedStateChangeToBal == 2)   //料空-》均衡
                            {
                                if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] < struGsh.chuteFeedAutoLastValue[feedId])
                                {
                                    struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] += struCnfg.stuFeedAutoCtrl.feedBalAdjustStep;
                                    if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] >= struGsh.chuteFeedAutoLastValue[feedId])
                                    {
                                        struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struGsh.chuteFeedAutoLastValue[feedId];
                                        needChange = false;
                                    }
                                }
                            }
//                            qDebug("STAT_LEVEL_BALANCE_MODE1 group = %d,feed[%d] is %d",group, feedId,struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId]);
                        }
                        myAutoFeed.struFeedAutoConfigMode1[group].curCountBalChgClock[STAT_LEVEL_BALANCE_MODE1] = 0;

                    }
                    if(needChange == false)
                    {
                        for(int ff = 0;ff < group_feed_total;ff++)
                        {
                            int feedId = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].stuIdenGupInfo[group].nUnitGupId[ff]%64;
                            struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struGsh.chuteFeedAutoLastValue[feedId];
                            struGsh.chuteFeedMainValue[feedId] =  struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId];
                        }

                        struFeedAutoConfigMode1[group].mode1FeedStateChangeToBal  = 0;
                    }
                }
                else
                {
                    for(int ff = 0;ff < group_feed_total;ff++)
                    {
                        int feedId = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].stuIdenGupInfo[group].nUnitGupId[ff]%64;
                        struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struGsh.chuteFeedAutoLastValue[feedId];
                        struGsh.chuteFeedMainValue[feedId] =  struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId];
                    }
                }


//                for(int ff = 0;ff < group_feed_total;ff++)
//                {
//                    int feedId = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].stuIdenGupInfo[group].nUnitGupId[ff]%64;
//                    struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struGsh.chuteFeedAutoLastValue[feedId];
//                    struGsh.chuteFeedMainValue[feedId] =  struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId];
//                }

                break;
            case STAT_LEVEL_PRE_FULL_MODE1:
                for(int ff = 0;ff < group_feed_total;ff++)
                {
                    int feedId = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].stuIdenGupInfo[group].nUnitGupId[ff]%64;
                    struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] += struCnfg.stuFeedAutoCtrl.feedPreFullConstValue;
                }

                break;
            case STAT_LEVEL_FULL_MODE1:
                if(myAutoFeed.struFeedAutoConfigMode1[group].curCountBalChgClock[STAT_LEVEL_FULL_MODE1]++ >= struCnfg.stuFeedAutoCtrl.feedBalAdjustDuration/2)
                {
                    for(int ff = 0;ff < group_feed_total;ff++)
                    {
                        int feedId = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].stuIdenGupInfo[group].nUnitGupId[ff]%64;

                        struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] += struCnfg.stuFeedAutoCtrl.feedBalAdjustStep;
                        if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] > 100)
                        {
                            struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = 100;
                        }
//                        qDebug("STAT_LEVEL_FULL_MODE1 group=%d, feed[%d] is %d",group,feedId,struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId]);
                    }
                    myAutoFeed.struFeedAutoConfigMode1[group].curCountBalChgClock[STAT_LEVEL_FULL_MODE1] = 0;
//                    struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = 100;
                }
                break;
            default:
                break;
            }
        }
    }
    myFlow.resetFeederRG(); //重置流量
}
/*
if(px <= myAutoFeed.struFeedAutoConfigMode1.mode1FeedBalPartScale[MODE1_BALANCE_STAT_1])
{
    myAutoFeed.struFeedAutoConfigMode1.mode1BalancePart = MODE1_BALANCE_STAT_4;
}
else if(px <= myAutoFeed.struFeedAutoConfigMode1.mode1FeedBalPartScale[MODE1_BALANCE_STAT_2])
{
    myAutoFeed.struFeedAutoConfigMode1.mode1BalancePart = MODE1_BALANCE_STAT_3;
}
else if(px <= myAutoFeed.struFeedAutoConfigMode1.mode1FeedBalPartScale[MODE1_BALANCE_STAT_3])
{
    myAutoFeed.struFeedAutoConfigMode1.mode1BalancePart = MODE1_BALANCE_STAT_2;
}
else
{
    myAutoFeed.struFeedAutoConfigMode1.mode1BalancePart = MODE1_BALANCE_STAT_1;
}
*/
/* 检测当前均衡目标值属于哪个 分段 */
void myAutoFeedSetClass::mode1GetCurBalancePartPoSi(int group, int level)
{
    int curLevel = level;
    int constChgCount = countLimitSlow;    //3次跳转才切换

    switch(struFeedAutoConfigMode1[group].mode1BalancePart)
    {
    case MODE1_BALANCE_STAT_1:   //S∈(L1，Depth], 默认（900，1050）
        if(struCnfg.stuFeedAutoCtrl.mode1FeedTargetPartCount <= 1)
        {
            break;
        }

        if(curLevel > struFeedAutoConfigMode1[group].mode1FeedBalPartScale[MODE1_BALANCE_STAT_1])
        {
            if(struFeedAutoConfigMode1[group].mode1CountBalPartChg[MODE1_BALANCE_STAT_1]++ >= constChgCount)
            {
                struFeedAutoConfigMode1[group].mode1BalancePart = MODE1_BALANCE_STAT_2;

                mode1ClearBalPartCount(group);
                break;
            }
        }
        else
        {
            struFeedAutoConfigMode1[group].mode1CountBalPartChg[MODE1_BALANCE_STAT_1] = 0;
        }
        break;
    case MODE1_BALANCE_STAT_2: //S∈(L2，Depth-L1]，默认（450， 900]
        if(struCnfg.stuFeedAutoCtrl.mode1FeedTargetPartCount <= 1)
        {
            struFeedAutoConfigMode1[group].mode1BalancePart = MODE1_BALANCE_STAT_1;
            mode1ClearBalPartCount(group);
            break;
        }
        if(curLevel <= struFeedAutoConfigMode1[group].mode1FeedBalPartScale[MODE1_BALANCE_STAT_1]) //500
        {
            if(struFeedAutoConfigMode1[group].mode1CountBalPartChg[MODE1_BALANCE_STAT_2]++ >= constChgCount)
            {
                struFeedAutoConfigMode1[group].mode1BalancePart = MODE1_BALANCE_STAT_1;
                mode1ClearBalPartCount(group);
                break;
            }
        }
        else if(curLevel > struFeedAutoConfigMode1[group].mode1FeedBalPartScale[MODE1_BALANCE_STAT_2])    //750
        {
            if((struCnfg.stuFeedAutoCtrl.mode1FeedTargetPartCount > 2))
            {
                if(struFeedAutoConfigMode1[group].mode1CountBalPartChg[MODE1_BALANCE_STAT_2]++ >= constChgCount)
                {
                    struFeedAutoConfigMode1[group].mode1BalancePart = MODE1_BALANCE_STAT_3;
                    mode1ClearBalPartCount(group);
                    break;
                }
            }
        }
        else
        {
            struFeedAutoConfigMode1[group].mode1CountBalPartChg[MODE1_BALANCE_STAT_2] = 0;
        }
        break;
    case MODE1_BALANCE_STAT_3: //S∈ (0，L3]，默认（0， 300）
        if(struCnfg.stuFeedAutoCtrl.mode1FeedTargetPartCount <= 2)
        {
            struFeedAutoConfigMode1[group].mode1BalancePart = MODE1_BALANCE_STAT_1;
            mode1ClearBalPartCount(group);
            break;
        }
        if(curLevel <= struFeedAutoConfigMode1[group].mode1FeedBalPartScale[MODE1_BALANCE_STAT_2])
        {
            if(struFeedAutoConfigMode1[group].mode1CountBalPartChg[MODE1_BALANCE_STAT_3]++ >= constChgCount)
            {
                struFeedAutoConfigMode1[group].mode1BalancePart = MODE1_BALANCE_STAT_2;
                mode1ClearBalPartCount(group);
                break;
            }
        }
        else if(curLevel > struFeedAutoConfigMode1[group].mode1FeedBalPartScale[MODE1_BALANCE_STAT_3])
        {
            if((struCnfg.stuFeedAutoCtrl.mode1FeedTargetPartCount > 3))
            {
                if(struFeedAutoConfigMode1[group].mode1CountBalPartChg[MODE1_BALANCE_STAT_3]++ >= constChgCount)
                {
                    struFeedAutoConfigMode1[group].mode1BalancePart = MODE1_BALANCE_STAT_4;
                    mode1ClearBalPartCount(group);
                    break;
                }
            }
        }
        else
        {
            struFeedAutoConfigMode1[group].mode1CountBalPartChg[MODE1_BALANCE_STAT_3] = 0;
        }
        break;
    case MODE1_BALANCE_STAT_4: //S∈ (0，L3]，默认（0， 300）
        if(struCnfg.stuFeedAutoCtrl.mode1FeedTargetPartCount <= 3)
        {
            struFeedAutoConfigMode1[group].mode1BalancePart = MODE1_BALANCE_STAT_1;
            mode1ClearBalPartCount(group);
            break;
        }
        if(curLevel <= struFeedAutoConfigMode1[group].mode1FeedBalPartScale[MODE1_BALANCE_STAT_3])
        {
            if(struFeedAutoConfigMode1[group].mode1CountBalPartChg[MODE1_BALANCE_STAT_4]++ >= constChgCount)
            {
                struFeedAutoConfigMode1[group].mode1BalancePart = MODE1_BALANCE_STAT_3;
                mode1ClearBalPartCount(group);
                break;
            }
        }
        else
        {
            struFeedAutoConfigMode1[group].mode1CountBalPartChg[MODE1_BALANCE_STAT_4] = 0;
        }
        break;
    }
}

bool myAutoFeedSetClass::mode1AutoFlowAdjust()
{

    bool bResetFeed = false;//是否需要发送给料参数
    int view = struGsh.nViewforstatic;//使用前视计算,还有添加
    for(int i = 0;i < struCnfg.nLayerTotal;i++){
        for(int j = 0;j < struCnfg.struLayerInfo[i].stuViewInfo[view].nIdentifyGroupTotal;j++)
        {
            if((struCnfp.nAutoFeed[i][j].bEnable == 1)
                    &&(struFeedAutoConfigMode1[j].curBalanceStateByLevel == STAT_LEVEL_BALANCE_MODE1)
                    &&(struFeedAutoConfigMode1[j].mode1FeedStateChangeToBal == 0))
            {
                isInBalancing = true;
//               struGsh.autoFeedEnabled = true;
                for(int k = 0;k < struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nGroupUnitTotal;k++){
                    //停运低于门限的料槽
                    int unitIndex = struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nUnitGupId[k]%64;

                    if(struGsh.struStatics[i][view][unitIndex].nStaticsPeriod == 0 || struCnfp.n_coff == 0)
                        break;
//                    qDebug()<<"chute:"<<unitIndex;
                    if(struGsh.struStatics[i][view][unitIndex].nStaticsPeriod > struCnfg.sTotalStaticsGetDuration*13)
                        return false;
                    int flow = (float)struGsh.struStatics[i][view][unitIndex].nThroughout/(float)struCnfp.n_coff*(float)FlowSwitchHour/(float)struGsh.struStatics[i][view][unitIndex].nStaticsPeriod;

                    int offset;
//                    offset = flow - struFeedAutoConfigMode1.mode1FeedTargetPart[struFeedAutoConfigMode1.mode1BalancePart];
                    offset = flow - struCnfp.nAutoFeed[i][j].mode1FeedTargetPart[struFeedAutoConfigMode1[j].mode1BalancePart];

                    if(qAbs(offset) > 60){//目标产量与实际产量相差超过 60kg/h才进行调节
                        int quo = 200;
                        quo = (struCnfp.nAutoFeed[i][j].bQuickBalance == 0) ? 200 : 100;
                        int step = 0;
                        if(qAbs(offset) > 800)
                            step = offset/quo;
                        else if(qAbs(offset) > 600)
                            step = offset/quo;
                        else if(qAbs(offset) > 400)
                            step = offset/quo;
                        else if(qAbs(offset) > 200)
                            step = offset/quo;
                        else if(qAbs(offset) > 100)
                            step = offset/100;
                        else if(qAbs(offset) > 20)
                            step = 1;
                        else
                            step = 0;
                        step = qAbs(step);
                        if(step != 0){  //同时满足产量和流量的条件，再均衡
                            if(!((flow <= struCnfg.stuFeedAutoCtrl.balUpLowFlow)&&(struGsh.chuteFeedMainValue[unitIndex] >= struCnfg.stuFeedAutoCtrl.balUpFeedWhileLowFlow)))
                            {
                                if(offset > 0){
                                    if((struGsh.chuteFeedMainValue[unitIndex]-step) > 0){
                                        struGsh.chuteFeedMainValue[unitIndex]-=step;
                                    }
                                }else{
                                    int _t_maxFeed = (struCnfg.stuFeedAutoCtrl.nFeedValueBias[unitIndex] < 100) ? struCnfg.stuFeedAutoCtrl.nFeedValueBias[unitIndex] : 100;
                                    struGsh.chuteFeedMainValue[unitIndex] += step;
                                    if(struGsh.chuteFeedMainValue[unitIndex] > _t_maxFeed){
                                        struGsh.chuteFeedMainValue[unitIndex] = _t_maxFeed;
                                    }
                                }
                                bResetFeed = true;
                            }
                        }
                    }
                }
            }
        }
    }
    if(bResetFeed){
        for(int i = 0;i < struCnfg.nLayerTotal;i++){
            for(int j = 0;j < struCnfg.struLayerInfo[i].stuViewInfo[view].nIdentifyGroupTotal;j++)
            {
                if(struCnfp.nAutoFeed[i][j].bEnable == 1){
                    for(int k = 0;k < struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nGroupUnitTotal;k++){
                        //停运低于门限的料槽
                        int unitIndex = struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nUnitGupId[k]%64;

                        struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[unitIndex] = struGsh.chuteFeedMainValue[unitIndex];
                        struGsh.chuteFeedAutoLastValue[unitIndex] = struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[unitIndex];
                    }
                }
            }
        }
    }

    isInBalancing = false;

    return bResetFeed;
}
/********************************************************************************************************
 *
 * mode2:  对应界面模式3，雷达传感器，带自动料位监测+均衡供料，自动目标值
 *
 * 根据料仓位置，自动计算实时目标产量，再根据实时的目标产量，调整各通道流量
 * step 1: mode2CheckCurRicePoSi(), 根据料位，判断处于料空、均衡、料满的哪个阶段，需要连续几次才确定，忽略干扰
 * step 2:mode2ClearBalStateCount(), 状态切换时，需要清除相关变量和计数器
 * step 3:mode2ResetFeed(), 状态切换时，料空、均衡、料满发生状态切换时，需要流量值复位,包括二三四次
 * step 4:mode2GetCurBalanceTargetByPoSi(), 均衡状态下，，根据料位，判断当前均衡目标值属于哪个 分段，从而调整均衡的目标值
 * step 5:mode2AutoFlowAdjust(), 均衡状态下，，根据不同分段的均衡目标值，调整各通道流量，维持在均衡目标值
 * ******************************************************************************************************/

void myAutoFeedSetClass::getCurRadarLevelMode2WhileTurnOn()
{
#if 1
    int view = 0;
    int px = struGsh.struMachineStatus[struGsh.nLayer].nFeedLvelValue[0];
    int f1, f2, f3;
    int p1, p2, p3;

    p1 = struCnfg.stuFeedAutoCtrl.levelEmptyPosi;
    p2 = struCnfg.stuFeedAutoCtrl.levelFullPosi;

    int totalFeed;
    if((struCnfg.nMachineSpliceType == MACHINE_SPLICE_TYPE_BIG)&&(struCnfg.nChuteTotal > 1 || (struCnfg.nChuteTotal == 1 && !struCnfg.nAllInOne)))
    {
        totalFeed = struCnfg.nChuteTotal+1;
    }
    else
    {
        totalFeed = struCnfg.nChuteTotal;
    }

    if(px < struCnfg.stuFeedAutoCtrl.levelFullPosi)    //料满
    {
         struFeedAutoConfigMode2.curBalanceStateByLevel = STAT_LEVEL_FULL_MODE2;
         for(int i = 0;i < totalFeed;i++)
         {
             struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[i] = 100;
         }
         int view = struGsh.nViewforstatic;
         for(int i = 0;i < struCnfg.nLayerTotal;i++){
             for(int j = 0;j < struCnfg.struLayerInfo[i].stuViewInfo[view].nIdentifyGroupTotal;j++)
             {
                 if(struCnfp.nAutoFeed[i][j].bEnable == 1)
                 {
                     if(struGsh.chuteFeedAutoLastValue[i] == 100)
                     {
                        struGsh.chuteFeedAutoLastValue[i] = 95;
                     }
                 }
             }
         }
    }
    else if(px < struCnfg.stuFeedAutoCtrl.levelEmptyPosi)   // 料匀
    {
        struFeedAutoConfigMode2.curBalanceStateByLevel = STAT_LEVEL_BALANCE_MODE2;

        int view = struGsh.nViewforstatic;
        for(int i = 0;i < struCnfg.nLayerTotal;i++){
            for(int j = 0;j < struCnfg.struLayerInfo[i].stuViewInfo[view].nIdentifyGroupTotal;j++)
            {
                if(struCnfp.nAutoFeed[i][j].bEnable == 1)
                {
                    int k1 = px-struCnfg.stuFeedAutoCtrl.levelFullPosi;
                    int k2 = struCnfp.nAutoFeed[i][j].mode2_target_upper-struCnfp.nAutoFeed[i][j].mode2_target_lower;
                    int k3 = struCnfg.stuFeedAutoCtrl.levelEmptyPosi-struCnfg.stuFeedAutoCtrl.levelFullPosi;
                    struFeedAutoConfigMode2.groupFlowTarget[j] = struCnfp.nAutoFeed[i][j].mode2_target_upper-(k1*k2/k3);
                }
            }
        }
    }
    else // 料空
    {
        struFeedAutoConfigMode2.curBalanceStateByLevel = STAT_LEVEL_EMPTY_MODE2;
        for(int i = 0;i < totalFeed;i++)
        {
            struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[i] = struCnfg.stuFeedAutoCtrl.feedEmpytLowLimitValue;
        }
    }
#endif
}

/* step 1：料位状态切换料空、均衡、料满*/
bool myAutoFeedSetClass::mode2CheckCurRicePoSi()
{
    bool feedNeedReset = false;
#if 1
    int view, totalFeed;
    int curLevel = struGsh.struMachineStatus[struGsh.nLayer].nFeedLvelValue[0];
 //   int view = 0;

    switch(struFeedAutoConfigMode2.curBalanceStateByLevel)
    {
    case STAT_LEVEL_EMPTY_MODE2:   //S∈(L1，Depth], 默认（900，1050）
        if(curLevel <= struCnfg.stuFeedAutoCtrl.levelEmptyPosi)
        {
            if(struFeedAutoConfigMode2.curCountBalStateChg[STAT_LEVEL_EMPTY_MODE2]++ >= countLimitQuick)
            {
                struFeedAutoConfigMode2.curBalanceStateByLevel = STAT_LEVEL_BALANCE_MODE2;
                mode2ClearBalStateCount();
               // mode2StateChangeToResetFeed(STAT_LEVEL_BALANCE_MODE2, false, false);
                struFeedAutoConfigMode2.mode2FeedStateChangeToBal = 2;
                feedNeedReset = true;
                break;
            }
        }
        else
        {
            feedNeedReset = false;
            struFeedAutoConfigMode2.curCountBalStateChg[STAT_LEVEL_EMPTY_MODE2] = 0;
        }
        break;
    case STAT_LEVEL_BALANCE_MODE2: //S∈(L2，Depth-L1]，默认（450， 900]
        if(curLevel > struCnfg.stuFeedAutoCtrl.levelEmptyPosi)
        {
            if(struFeedAutoConfigMode2.curCountBalStateChg[STAT_LEVEL_BALANCE_MODE2]++ >= countLimitSlow)
            {
                struFeedAutoConfigMode2.curBalanceStateByLevel = STAT_LEVEL_EMPTY_MODE2;
                mode2ClearBalStateCount();
                //mode2StateChangeToResetFeed(STAT_LEVEL_EMPTY_MODE2, false, false);
                feedNeedReset = true;
                break;
            }
        }
        else if(curLevel <= struCnfg.stuFeedAutoCtrl.levelFullPosi) //<=450
        {
            if(struFeedAutoConfigMode2.curCountBalStateChg[STAT_LEVEL_BALANCE_MODE2]++ >= countLimitQuick)
            {
                struFeedAutoConfigMode2.curBalanceStateByLevel = STAT_LEVEL_FULL_MODE2;
                mode2ClearBalStateCount();
                //mode2StateChangeToResetFeed(STAT_LEVEL_FULL_MODE2, false, false);
                feedNeedReset = true;
                break;
            }
        }
        else
        {
            feedNeedReset = false;

            struFeedAutoConfigMode2.curCountBalStateChg[STAT_LEVEL_BALANCE_MODE2] = 0;

            mode2GetCurBalanceTargetByPoSi(curLevel);
        }
        break;
    case STAT_LEVEL_FULL_MODE2: //S∈ (0，L3]，默认（0， 300）
        if(curLevel > struCnfg.stuFeedAutoCtrl.levelFullPosi) // >300
        {
            if(struFeedAutoConfigMode2.curCountBalStateChg[STAT_LEVEL_FULL_MODE2]++ >= countLimitSlow)
            {
                struFeedAutoConfigMode2.curBalanceStateByLevel = STAT_LEVEL_BALANCE_MODE2;
                mode2ClearBalStateCount();
                struFeedAutoConfigMode2.mode2FeedStateChangeToBal = 1;
                struFeedAutoConfigMode2.mode2LevelFullToBalCount = 0;
                //mode2StateChangeToResetFeed(STAT_LEVEL_BALANCE_MODE2, false, true);
                feedNeedReset = true;
                break;
            }
        }
        else
        {
            feedNeedReset = false;
            struFeedAutoConfigMode2.curCountBalStateChg[STAT_LEVEL_FULL_MODE2] = 0;
        }

        break;
    }

    if((struFeedAutoConfigMode2.curBalanceStateByLevel == STAT_LEVEL_EMPTY_MODE2) || (struFeedAutoConfigMode2.curBalanceStateByLevel == STAT_LEVEL_FULL_MODE2))
    {
        struGsh.isRadarStateAlarm = true;
    }
    else
    {
        struGsh.isRadarStateAlarm = false;
    }
#endif
    if(feedNeedReset ==  true)
    {
        mode2StateChangeToResetFeed(STAT_LEVEL_BALANCE_MODE2, false, false);
    }
    return feedNeedReset;
}

void myAutoFeedSetClass::mode2ClearBalStateCount()
{
#if 1
    struFeedAutoConfigMode2.feedEmptyCount = 0;
    struFeedAutoConfigMode2.feedFullCount = 0;
    struFeedAutoConfigMode2.levelMoreToNormalCount = 0;

    struFeedAutoConfigMode2.moreToBal2ndEnable = true;

    for(int i = 0; i < 6; i++)
    {
        struFeedAutoConfigMode2.curCountBalStateChg[i] = 0;
        struFeedAutoConfigMode2.curCountBalChgClock[i] = 0;
    }
#endif
}

/* 模式1，料位变化，导致状态料空、均衡、料满发生状态切换时，需要流量复位,包括二三四次 */
void myAutoFeedSetClass::mode2StateChangeToResetFeed(int index, bool isChangeToBal, bool needDelay)
{
#if 1
    int view = 0, totalFeed = 0;

    if((struCnfg.nMachineSpliceType == MACHINE_SPLICE_TYPE_BIG)&&(struCnfg.nChuteTotal > 1 || (struCnfg.nChuteTotal == 1 && !struCnfg.nAllInOne)))
    {
        totalFeed = struCnfg.nChuteTotal+1;
    }
    else
    {
        totalFeed = struCnfg.nChuteTotal;
    }

    switch(index)
    {
    case STAT_LEVEL_EMPTY_MODE2:
        if(myAutoFeed.struFeedAutoConfigMode2.curCountBalChgClock[STAT_LEVEL_EMPTY_MODE2]++ >= struCnfg.stuFeedAutoCtrl.feedBalAdjustDuration/2)
        {
            for(int i = 0;i < struCnfg.nLayerTotal;i++){
                for(int j = 0;j < struCnfg.struLayerInfo[i].stuViewInfo[view].nIdentifyGroupTotal;j++)
                {
                    if(struCnfp.nAutoFeed[i][j].bEnable == 1){
                        for(int k = 0;k < struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nGroupUnitTotal;k++)
                        {
                            int feedId = struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nUnitGupId[k]%64;

                            if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] > struCnfg.stuFeedAutoCtrl.feedEmpytLowLimitValue)
                            {
                                struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] -= struCnfg.stuFeedAutoCtrl.feedBalAdjustStep;
                                if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] <= struCnfg.stuFeedAutoCtrl.feedEmpytLowLimitValue)
                                {
                                    struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struCnfg.stuFeedAutoCtrl.feedEmpytLowLimitValue;
                                }
                            }
                        }
                    }
                }
            }
            myAutoFeed.struFeedAutoConfigMode2.curCountBalChgClock[STAT_LEVEL_EMPTY_MODE2] = 0;
        }
        break;
    case STAT_LEVEL_BALANCE_MODE2:
        view = struGsh.nViewforstatic;//使用前视计算,还有添加
        if(struFeedAutoConfigMode2.mode2FeedStateChangeToBal > 0)
        {
            bool needChange = true;
            if(myAutoFeed.struFeedAutoConfigMode2.curCountBalChgClock[STAT_LEVEL_BALANCE_MODE2]++ >= struCnfg.stuFeedAutoCtrl.feedBalAdjustDuration/2)
            {
                for(int i = 0;i < struCnfg.nLayerTotal;i++){
                    for(int j = 0;j < struCnfg.struLayerInfo[i].stuViewInfo[view].nIdentifyGroupTotal;j++)
                    {
                        if(struCnfp.nAutoFeed[i][j].bEnable == 1){
                            for(int k = 0;k < struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nGroupUnitTotal;k++)
                            {
                                int feedId = struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nUnitGupId[k]%64;
                                if(struFeedAutoConfigMode2.mode2FeedStateChangeToBal == 1)   //料满-》均衡
                                {
                                    if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] > struGsh.chuteFeedAutoLastValue[feedId])
                                    {
                                        struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] -= struCnfg.stuFeedAutoCtrl.feedBalAdjustStep;
                                        if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] <= struGsh.chuteFeedAutoLastValue[feedId])
                                        {
                                            struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struGsh.chuteFeedAutoLastValue[feedId];
                                            needChange = false;
                                        }
                                    }
                                }
                                else if(struFeedAutoConfigMode2.mode2FeedStateChangeToBal == 2)   //料空-》均衡
                                {
                                    if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] < struGsh.chuteFeedAutoLastValue[feedId])
                                    {
                                        struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] += struCnfg.stuFeedAutoCtrl.feedBalAdjustStep;
                                        if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] >= struGsh.chuteFeedAutoLastValue[feedId])
                                        {
                                            struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struGsh.chuteFeedAutoLastValue[feedId];
                                            needChange = false;
                                        }
                                    }
                                }
    //                            qDebug("STAT_LEVEL_BALANCE_MODE1 group = %d,feed[%d] is %d",group, feedId,struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId]);
                            }

                        }
                    }
                }
                struFeedAutoConfigMode2.curCountBalChgClock[STAT_LEVEL_BALANCE_MODE2] = 0;
            }
            if(needChange == false)
            {
                for(int i = 0;i < struCnfg.nLayerTotal;i++){
                    for(int j = 0;j < struCnfg.struLayerInfo[i].stuViewInfo[view].nIdentifyGroupTotal;j++)
                    {
                        if(struCnfp.nAutoFeed[i][j].bEnable == 1){
                            for(int k = 0;k < struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nGroupUnitTotal;k++)
                            {
                                int feedId = struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nUnitGupId[k]%64;

                                struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struGsh.chuteFeedAutoLastValue[feedId];
                                struGsh.chuteFeedMainValue[feedId] =  struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId];
                            }
                        }
                    }
                }
                struFeedAutoConfigMode2.mode2FeedStateChangeToBal  = 0;
            }
        }
        else
        {
            for(int i = 0;i < struCnfg.nLayerTotal;i++){
                for(int j = 0;j < struCnfg.struLayerInfo[i].stuViewInfo[view].nIdentifyGroupTotal;j++)
                {
                    if(struCnfp.nAutoFeed[i][j].bEnable == 1){
                        for(int k = 0;k < struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nGroupUnitTotal;k++)
                        {
                            int feedId = struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nUnitGupId[k]%64;

                            struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struGsh.chuteFeedAutoLastValue[feedId];
                            struGsh.chuteFeedMainValue[feedId] =  struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId];
                        }
                    }
                    else
                    {
                        if(struFeedAutoConfigMode2.mode2LevelFullToBalCount++ > struCnfg.stuFeedAutoCtrl.feedFullToNormalContinueTime*countPerMin)
                        {
                            struFeedAutoConfigMode2.mode2LevelFullToBalCount = struCnfg.stuFeedAutoCtrl.feedFullToNormalContinueTime*countPerMin+1;
                            for(int k = 0;k < struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nGroupUnitTotal;k++)
                            {
                                int feedId = struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nUnitGupId[k]%64;

                                struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struGsh.chuteFeedAutoLastValue[feedId];
                                struGsh.chuteFeedMainValue[feedId] =  struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId];
                            }
                        }
                    }
                }
            }
        }
        break;
    case STAT_LEVEL_FULL_MODE2:
        if(myAutoFeed.struFeedAutoConfigMode2.curCountBalChgClock[STAT_LEVEL_FULL_MODE2]++ >= struCnfg.stuFeedAutoCtrl.feedBalAdjustDuration/2)
        {
            for(int i = 0;i < struCnfg.nLayerTotal;i++){
                for(int j = 0;j < struCnfg.struLayerInfo[i].stuViewInfo[view].nIdentifyGroupTotal;j++)
                {
                    if(struCnfp.nAutoFeed[i][j].bEnable == 1){
                        for(int k = 0;k < struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nGroupUnitTotal;k++)
                        {
                            int feedId = struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nUnitGupId[k]%64;
                            struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] += struCnfg.stuFeedAutoCtrl.feedBalAdjustStep;
                            if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] > 100)
                            {
                                struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = 100;
                            }
                        }
                    }
                }
            }
            myAutoFeed.struFeedAutoConfigMode2.curCountBalChgClock[STAT_LEVEL_FULL_MODE2] = 0;
        }
        break;
    default:
        break;
    }

    myFlow.resetFeederRG(); //重置流量
#endif
}

/* 根据料位，实时更新当前均衡目标值 */
void myAutoFeedSetClass::mode2GetCurBalanceTargetByPoSi(int level)
{
#if 1
    int view = struGsh.nViewforstatic;
    for(int i = 0;i < struCnfg.nLayerTotal;i++){
        for(int j = 0;j < struCnfg.struLayerInfo[i].stuViewInfo[view].nIdentifyGroupTotal;j++)
        {
            if(struCnfp.nAutoFeed[i][j].bEnable == 1)
            {
                int k1 = level-struCnfg.stuFeedAutoCtrl.levelFullPosi;
                int k2 = struCnfp.nAutoFeed[i][j].mode2_target_upper-struCnfp.nAutoFeed[i][j].mode2_target_lower;
                int k3 = struCnfg.stuFeedAutoCtrl.levelEmptyPosi-struCnfg.stuFeedAutoCtrl.levelFullPosi;
                struFeedAutoConfigMode2.groupFlowTarget[j] = struCnfp.nAutoFeed[i][j].mode2_target_upper-(k1*k2/k3);

//                qDebug("struFeedAutoConfigMode2.groupFlowTarget[%d] is %d", j, struFeedAutoConfigMode2.groupFlowTarget[j]);
            }
        }
    }
#endif
}

bool myAutoFeedSetClass::mode2AutoFlowAdjust()
{
    bool bResetFeed = false;//是否需要发送给料参数
#if 1
    //struFeedAutoConfigMode2.isInBalancing = true;
    int view = struGsh.nViewforstatic;//使用前视计算,还有添加
    for(int i = 0;i < struCnfg.nLayerTotal;i++){
        for(int j = 0;j < struCnfg.struLayerInfo[i].stuViewInfo[view].nIdentifyGroupTotal;j++)
        {
            if(struCnfp.nAutoFeed[i][j].bEnable == 1){
//               struGsh.autoFeedEnabled = true;
                for(int k = 0;k < struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nGroupUnitTotal;k++){
                    //停运低于门限的料槽
                    int unitIndex = struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nUnitGupId[k]%64;

                    if(struGsh.struStatics[i][view][unitIndex].nStaticsPeriod == 0 || struCnfp.n_coff == 0)
                        break;
//                    qDebug()<<"chute:"<<unitIndex;
                    if(struGsh.struStatics[i][view][unitIndex].nStaticsPeriod > struCnfg.sTotalStaticsGetDuration*13)
                        return false;
                    int flow = (float)struGsh.struStatics[i][view][unitIndex].nThroughout/(float)struCnfp.n_coff*(float)FlowSwitchHour/(float)struGsh.struStatics[i][view][unitIndex].nStaticsPeriod;

                    int offset;
                    offset = flow - struFeedAutoConfigMode2.groupFlowTarget[j];

                    if(qAbs(offset) > 60){//目标产量与实际产量相差超过 60kg/h才进行调节
                        int quo = 200;
                        quo = (struCnfp.nAutoFeed[i][j].bQuickBalance == 0) ? 200 : 100;
                        int step = 0;
                        if(qAbs(offset) > 800)
                            step = offset/quo;
                        else if(qAbs(offset) > 600)
                            step = offset/quo;
                        else if(qAbs(offset) > 400)
                            step = offset/quo;
                        else if(qAbs(offset) > 200)
                            step = offset/quo;
                        else if(qAbs(offset) > 100)
                            step = offset/100;
                        else if(qAbs(offset) > 20)
                            step = 1;
                        else
                            step = 0;
                        step = qAbs(step);
                        if(step != 0){
                            if(offset > 0){
                                if((struGsh.chuteFeedMainValue[unitIndex]-step) > 0){
                                    struGsh.chuteFeedMainValue[unitIndex]-=step;
                                }
                            }else{                                
                                int _t_maxFeed = (struCnfg.stuFeedAutoCtrl.nFeedValueBias[unitIndex] < 100) ? struCnfg.stuFeedAutoCtrl.nFeedValueBias[unitIndex] : 100;
                                struGsh.chuteFeedMainValue[unitIndex] += step;
                                if(struGsh.chuteFeedMainValue[unitIndex] > _t_maxFeed){
                                    struGsh.chuteFeedMainValue[unitIndex] = _t_maxFeed;
                                }
                            }
                            bResetFeed = true;
                        }
                    }
                }
            }
        }
    }
    if(bResetFeed){
        for(int i = 0;i < struCnfg.nLayerTotal;i++){
            for(int j = 0;j < struCnfg.struLayerInfo[i].stuViewInfo[view].nIdentifyGroupTotal;j++)
            {
                if(struCnfp.nAutoFeed[i][j].bEnable == 1){
                    for(int k = 0;k < struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nGroupUnitTotal;k++){
                        //停运低于门限的料槽
                        int unitIndex = struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nUnitGupId[k]%64;

                        struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[unitIndex] = struGsh.chuteFeedMainValue[unitIndex];
                        struGsh.chuteFeedAutoLastValue[unitIndex] = struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[unitIndex];
                    }
                }
            }
        }
    }

    //struFeedAutoConfigMode2.isInBalancing = false;
#endif
    return bResetFeed;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************************************************
 *
 * mode-料位开关模式:  对应界面模式2，料位传感器，带料位监测+均衡供料， 单档数目标
 *
 * step 1:getCurFeedLevelModeSwitchWhileTurnOn() 刚下料时，判断当前料位状况
 * step 1: modeSwitchCheckCurRicePoSi(), 根据料位，判断处于料空、均衡、料满的哪个阶段，需要连续几次才确定，忽略干扰
 * step 2:modeSwitchClearBalStateCount(), 状态切换时，需要清除相关变量和计数器
 * step 3:modeSwitchStateChangeToResetFeed(), 状态切换时，料空、均衡、料满发生状态切换时，需要流量值复位,包括二三四次
 * step 4:modeSwitchAutoFlowAdjust(), 均衡状态下，，根据不同分段的均衡目标值，调整各通道流量，维持在均衡目标值
 * ******************************************************************************************************/

void myAutoFeedSetClass::getCurFeedLevelModeSwitchWhileTurnOn()
{
//    int view = 0;
//    int px = struGsh.struMachineStatus[struGsh.nLayer].nFeedLvelValue;
//    int f1, f2, f3;
//    int p1, p2, p3;

    int group_feed_total, totalFeed;
    int view = struGsh.nViewforstatic;//使用前视计算,还有添加

    if((struCnfg.nMachineSpliceType == MACHINE_SPLICE_TYPE_BIG)&&(struCnfg.nChuteTotal > 1 || (struCnfg.nChuteTotal == 1 && !struCnfg.nAllInOne)))
    {
        totalFeed = struCnfg.nChuteTotal+1;
    }
    else
    {
        totalFeed = struCnfg.nChuteTotal;
    }

//    switch(struFeedAutoConfigModeSwitch.curBalanceStateByLevel)

    for(int group = 0; group < struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nViewforstatic].nIdentifyGroupTotal; group++)
    {
        if(struCnfp.nAutoFeed[struGsh.nLayer][group].bEnable == 1)
        {
            group_feed_total = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nViewforstatic].stuIdenGupInfo[group].nGroupUnitTotal;
            switch(struGsh.struMachineStatus[struGsh.nLayer].nFeedLevelSensorSwitchStat)
            {
            case STAT_LEVEL_FULL_MODE_SWITCH:
                for(int ff = 0;ff < group_feed_total;ff++)
                {
                    int feedId = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].stuIdenGupInfo[group].nUnitGupId[ff]%64;
                    struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = 100;
                }
                struFeedAutoConfigModeSwitch[group].curBalanceStateByLevel = STAT_LEVEL_FULL_MODE_SWITCH;
                break;
            case STAT_LEVEL_PRE_FULL_MODE_SWITCH:
                for(int ff = 0;ff < group_feed_total;ff++)
                {
                    int feedId = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].stuIdenGupInfo[group].nUnitGupId[ff]%64;
                    struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struCnfg.stuFeedAutoCtrl.feedPreFullConstValue;
                }
                struFeedAutoConfigModeSwitch[group].curBalanceStateByLevel = STAT_LEVEL_PRE_FULL_MODE_SWITCH;
                break;
            case STAT_LEVEL_BALANCE_MODE_SWITCH:
                struFeedAutoConfigModeSwitch[group].curBalanceStateByLevel = STAT_LEVEL_BALANCE_MODE_SWITCH;

                break;
            case STAT_LEVEL_EMPTY_MODE_SWITCH:                
                for(int ff = 0;ff < group_feed_total;ff++)
                {
                    int feedId = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].stuIdenGupInfo[group].nUnitGupId[ff]%64;
                    struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struCnfg.stuFeedAutoCtrl.feedEmpytLowLimitValue;
                }
                struFeedAutoConfigModeSwitch[group].curBalanceStateByLevel = STAT_LEVEL_EMPTY_MODE_SWITCH;
                break;
            }
        }
    }
}

/* step 1：料位状态切换料空、均衡、料满*/
bool myAutoFeedSetClass::modeSwitchCheckCurRicePoSi()
{
    int i, totalFeed;
    int curLevelStat = struGsh.struMachineStatus[struGsh.nLayer].nFeedLevelSensorSwitchStat;
    bool feedNeedReset = false;

    countLimitQuick = struCnfg.stuFeedAutoCtrl.feedLevelChangeWaitTime/2;

    for(int group = 0; group < struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nViewforstatic].nIdentifyGroupTotal; group++)
    {
        if(struCnfp.nAutoFeed[struGsh.nLayer][group].bEnable == 1)
        {
            switch(struFeedAutoConfigModeSwitch[group].curBalanceStateByLevel)
            {
            case STAT_LEVEL_EMPTY_MODE_SWITCH:   //S∈(L1，Depth], 默认（900，1050）
                if(curLevelStat == STAT_LEVEL_BALANCE_MODE_SWITCH)
                {
                    if(struFeedAutoConfigModeSwitch[group].curCountBalStateChg[STAT_LEVEL_EMPTY_MODE_SWITCH]++ >= countLimitQuick)
                    {
                        struFeedAutoConfigModeSwitch[group].curBalanceStateByLevel = STAT_LEVEL_BALANCE_MODE_SWITCH;
                        modeSwitchClearBalStateCount(group);
                        struFeedAutoConfigModeSwitch[group].modeSwitchFeedStateChangeToBal = 2;
//                        modeSwitchStateChangeToResetFeed(STAT_LEVEL_BALANCE_MODE_SWITCH, false, false);
                        feedNeedReset = true;
                        break;
                    }
                }
                else
                {
                    struFeedAutoConfigModeSwitch[group].curCountBalStateChg[STAT_LEVEL_EMPTY_MODE_SWITCH] = 0;
                }
                break;
            case STAT_LEVEL_BALANCE_MODE_SWITCH: //S∈(L2，Depth-L1]，默认（450， 900]
                if(curLevelStat == STAT_LEVEL_EMPTY_MODE_SWITCH)
                {
                    if(struFeedAutoConfigModeSwitch[group].curCountBalStateChg[STAT_LEVEL_BALANCE_MODE_SWITCH]++ >= countLimitQuick)
                    {
                        struFeedAutoConfigModeSwitch[group].curBalanceStateByLevel = STAT_LEVEL_EMPTY_MODE_SWITCH;
                        modeSwitchClearBalStateCount(group);
//                        modeSwitchStateChangeToResetFeed(STAT_LEVEL_EMPTY_MODE_SWITCH, false, false);
                        feedNeedReset = true;
                        break;
                    }
                }
                else if(curLevelStat == STAT_LEVEL_FULL_MODE_SWITCH) //<=450
                {
                    if(struFeedAutoConfigModeSwitch[group].curCountBalStateChg[STAT_LEVEL_BALANCE_MODE_SWITCH]++ >= countLimitQuick)
                    {
                        struFeedAutoConfigModeSwitch[group].curBalanceStateByLevel = STAT_LEVEL_FULL_MODE_SWITCH;
                        modeSwitchClearBalStateCount(group);
//                        modeSwitchStateChangeToResetFeed(STAT_LEVEL_PRE_FULL_MODE_SWITCH, false, false);
                        feedNeedReset = true;
                        break;
                    }
                }
                else
                {
                    struFeedAutoConfigModeSwitch[group].curCountBalStateChg[STAT_LEVEL_BALANCE_MODE_SWITCH] = 0;
                }
                break;
            case STAT_LEVEL_PRE_FULL_MODE_SWITCH: //S∈ (0，L3]，默认（0， 300）    // 取消
                if((curLevelStat == STAT_LEVEL_EMPTY_MODE_SWITCH) || (curLevelStat == STAT_LEVEL_BALANCE_MODE_SWITCH)) // >300
                {
                    if(struFeedAutoConfigModeSwitch[group].curCountBalStateChg[STAT_LEVEL_PRE_FULL_MODE_SWITCH]++ >= countLimitQuick)
                    {
                        struFeedAutoConfigModeSwitch[group].curBalanceStateByLevel = STAT_LEVEL_BALANCE_MODE_SWITCH;
                        modeSwitchClearBalStateCount(group);
                        struFeedAutoConfigModeSwitch[group].modeSwitchFeedStateChangeToBal = 1;
//                        modeSwitchStateChangeToResetFeed(STAT_LEVEL_BALANCE_MODE_SWITCH, false, true);
                        feedNeedReset = true;
                        break;
                    }
                }
                else if(curLevelStat == STAT_LEVEL_FULL_MODE_SWITCH)
                {
                    if(struFeedAutoConfigModeSwitch[group].curCountBalStateChg[STAT_LEVEL_PRE_FULL_MODE_SWITCH]++ >= struCnfg.stuFeedAutoCtrl.feedLevelPreFullWaitTime/2)
                    {
                        struFeedAutoConfigModeSwitch[group].curBalanceStateByLevel = STAT_LEVEL_FULL_MODE_SWITCH;
                        modeSwitchClearBalStateCount(group);
//                        modeSwitchStateChangeToResetFeed(STAT_LEVEL_FULL_MODE_SWITCH, false, false);
                        feedNeedReset = true;
                        break;
                    }
                }
                else
                {
                    struFeedAutoConfigModeSwitch[group].curCountBalStateChg[STAT_LEVEL_PRE_FULL_MODE_SWITCH] = 0;
                }
                break;
            case STAT_LEVEL_FULL_MODE_SWITCH: //S∈ (0，L3]，默认（0， 300）
                if((curLevelStat == STAT_LEVEL_EMPTY_MODE_SWITCH) || (curLevelStat == STAT_LEVEL_BALANCE_MODE_SWITCH)) // >300
                {
                    if(struFeedAutoConfigModeSwitch[group].curCountBalStateChg[STAT_LEVEL_FULL_MODE_SWITCH]++ >= countLimitQuick)
                    {
                        struFeedAutoConfigModeSwitch[group].curBalanceStateByLevel = STAT_LEVEL_BALANCE_MODE_SWITCH;
                        modeSwitchClearBalStateCount(group);
                        struFeedAutoConfigModeSwitch[group].modeSwitchFeedStateChangeToBal = 1;
//                        modeSwitchStateChangeToResetFeed(STAT_LEVEL_BALANCE_MODE_SWITCH, false, true);
                        feedNeedReset = true;
                        break;
                    }
                }
                else
                {
                    struFeedAutoConfigModeSwitch[group].curCountBalStateChg[STAT_LEVEL_FULL_MODE_SWITCH] = 0;
                }

                break;
            }
        }
    }

    struGsh.isRadarStateAlarm = false;
    {
        for(int group = 0; group < struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nViewforstatic].nIdentifyGroupTotal; group++)
        {
            if((struFeedAutoConfigModeSwitch[group].curBalanceStateByLevel == STAT_LEVEL_EMPTY_MODE_SWITCH)
                        || (struFeedAutoConfigModeSwitch[group].curBalanceStateByLevel == STAT_LEVEL_FULL_MODE_SWITCH))
            {
                struGsh.isRadarStateAlarm = true;
            }
        }
    }

    for(int group = 0; group < struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nViewforstatic].nIdentifyGroupTotal; group++)
    {
        if(struCnfp.nAutoFeed[struGsh.nLayer][group].bEnable == 1)
        {
            if(struFeedAutoConfigModeSwitch[group].modeSwitchFeedStateChangeToBal > 0)    //处于料满->均衡， 或料空->均衡
            {
                feedNeedReset = true;
                break;
            }
        }
    }

    if(feedNeedReset == true)
    {
        mode1StateChangeToResetFeed(STAT_LEVEL_BALANCE_MODE1, false, false);    //STAT_LEVEL_BALANCE_MODE1 不需要了
    }

    return feedNeedReset;
}

void myAutoFeedSetClass::modeSwitchClearBalStateCount(int group)
{

    struFeedAutoConfigModeSwitch[group].levelFullToNormalCount = 0;
    for(int i = 0; i < 6; i++)
    {
        struFeedAutoConfigModeSwitch[group].curCountBalStateChg[i] = 0;
        struFeedAutoConfigMode1[group].curCountBalChgClock[i] = 0;
    }


//    qDebug("111 struFeedAutoConfigModeSwitch[group].curBalanceStateByLevel is %d", struFeedAutoConfigModeSwitch[group].curBalanceStateByLevel);
}


/* 模式 料位开关，料位变化，导致状态料空、均衡、料满发生状态切换时，需要流量复位,包括二三四次
needDelay：用于判断二三四次，是否需要迅速响应
*/
void myAutoFeedSetClass::modeSwitchStateChangeToResetFeed(int index, bool isChangeToBal, bool needDelay)
{
    int totalFeed = 0;
    int view = struGsh.nViewforstatic;//使用前视计算,还有添加
    int group_feed_total;

    if((struCnfg.nMachineSpliceType == MACHINE_SPLICE_TYPE_BIG)&&(struCnfg.nChuteTotal > 1 || (struCnfg.nChuteTotal == 1 && !struCnfg.nAllInOne)))
    {
        totalFeed = struCnfg.nChuteTotal+1;
    }
    else
    {
        totalFeed = struCnfg.nChuteTotal;
    }

    for(int group = 0; group < struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].nIdentifyGroupTotal; group++)
    {
        if(struCnfp.nAutoFeed[struGsh.nLayer][group].bEnable == 1)
        {
            group_feed_total = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].stuIdenGupInfo[group].nGroupUnitTotal;
            switch(struFeedAutoConfigModeSwitch[group].curBalanceStateByLevel)
            {
            case STAT_LEVEL_EMPTY_MODE_SWITCH:
                for(int ff = 0;ff < group_feed_total;ff++)
                {
                    int feedId = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].stuIdenGupInfo[group].nUnitGupId[ff]%64;

                    if(myAutoFeed.struFeedAutoConfigModeSwitch[group].curCountBalChgClock[STAT_LEVEL_EMPTY_MODE_SWITCH]++ >= struCnfg.stuFeedAutoCtrl.feedBalAdjustStep/2)
                    {
                        myAutoFeed.struFeedAutoConfigModeSwitch[group].curCountBalChgClock[STAT_LEVEL_EMPTY_MODE_SWITCH] = 0;
                        if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] > struCnfg.stuFeedAutoCtrl.feedEmpytLowLimitValue)
                        {
                            struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] -= struCnfg.stuFeedAutoCtrl.feedBalAdjustStep;
                            if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] <= struCnfg.stuFeedAutoCtrl.feedEmpytLowLimitValue)
                            {
                                struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struCnfg.stuFeedAutoCtrl.feedEmpytLowLimitValue;
                            }
                        }
                    }

//                    struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struCnfg.stuFeedAutoCtrl.feedEmpytLowLimitValue;
                }
                break;
            case STAT_LEVEL_BALANCE_MODE_SWITCH:
                if(struFeedAutoConfigModeSwitch[group].modeSwitchFeedStateChangeToBal > 0)
                {
                    bool needChange = true;

                    for(int ff = 0;ff < group_feed_total;ff++)
                    {
                        int feedId = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].stuIdenGupInfo[group].nUnitGupId[ff]%64;

                        if(struFeedAutoConfigModeSwitch[group].modeSwitchFeedStateChangeToBal == 1)   //料满-》均衡
                        {
                            if(myAutoFeed.struFeedAutoConfigModeSwitch[group].curCountBalChgClock[STAT_LEVEL_BALANCE_MODE_SWITCH]++ >= struCnfg.stuFeedAutoCtrl.feedBalAdjustStep/2)
                            {
                                myAutoFeed.struFeedAutoConfigModeSwitch[group].curCountBalChgClock[STAT_LEVEL_BALANCE_MODE_SWITCH] = 0;
                                if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] > struGsh.chuteFeedAutoLastValue[feedId])
                                {
                                    struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] -= struCnfg.stuFeedAutoCtrl.feedBalAdjustStep;
                                    if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] <= struGsh.chuteFeedAutoLastValue[feedId])
                                    {
                                        struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struGsh.chuteFeedAutoLastValue[feedId];
                                        needChange = false;
                                    }
                                }
                            }
                        }
                        else if(struFeedAutoConfigModeSwitch[group].modeSwitchFeedStateChangeToBal == 2)   //料空-》均衡
                        {
                            if(myAutoFeed.struFeedAutoConfigMode1[group].curCountBalChgClock[STAT_LEVEL_BALANCE_MODE_SWITCH]++ >= struCnfg.stuFeedAutoCtrl.feedBalAdjustStep/2)
                            {
                                myAutoFeed.struFeedAutoConfigModeSwitch[group].curCountBalChgClock[STAT_LEVEL_BALANCE_MODE_SWITCH] = 0;
                                if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] < struGsh.chuteFeedAutoLastValue[feedId])
                                {
                                    struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] += struCnfg.stuFeedAutoCtrl.feedBalAdjustStep;
                                    if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] >= struGsh.chuteFeedAutoLastValue[feedId])
                                    {
                                        struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struGsh.chuteFeedAutoLastValue[feedId];
                                        needChange = false;
                                    }
                                }
                            }
                        }
                    }
                    if(needChange == false)
                    {
                        for(int ff = 0;ff < group_feed_total;ff++)
                        {
                            int feedId = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].stuIdenGupInfo[group].nUnitGupId[ff]%64;
                            struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struGsh.chuteFeedAutoLastValue[feedId];
                            struGsh.chuteFeedMainValue[feedId] =  struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId];
                        }

                        struFeedAutoConfigModeSwitch[group].modeSwitchFeedStateChangeToBal  = 0;
                    }
                }
                else
                {
                    for(int ff = 0;ff < group_feed_total;ff++)
                    {
                        int feedId = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].stuIdenGupInfo[group].nUnitGupId[ff]%64;
                        struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struGsh.chuteFeedAutoLastValue[feedId];
                        struGsh.chuteFeedMainValue[feedId] =  struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId];
                    }
                }
                break;
            case STAT_LEVEL_PRE_FULL_MODE_SWITCH:
                for(int ff = 0;ff < group_feed_total;ff++)
                {
                    int feedId = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].stuIdenGupInfo[group].nUnitGupId[ff]%64;
                    struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struCnfg.stuFeedAutoCtrl.feedPreFullConstValue;
                }
                break;
            case STAT_LEVEL_FULL_MODE_SWITCH:
                for(int ff = 0;ff < group_feed_total;ff++)
                {
                    int feedId = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].stuIdenGupInfo[group].nUnitGupId[ff]%64;
                    if(myAutoFeed.struFeedAutoConfigModeSwitch[group].curCountBalChgClock[STAT_LEVEL_FULL_MODE_SWITCH]++ >= struCnfg.stuFeedAutoCtrl.feedBalAdjustStep/2)
                    {
                        myAutoFeed.struFeedAutoConfigModeSwitch[group].curCountBalChgClock[STAT_LEVEL_FULL_MODE_SWITCH] = 0;
                        struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] += struCnfg.stuFeedAutoCtrl.feedBalAdjustStep;
                        if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] > 100)
                        {
                            struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = 100;
                        }
                    }
                }

//                for(int ff = 0;ff < group_feed_total;ff++)
//                {
//                    int feedId = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].stuIdenGupInfo[group].nUnitGupId[ff]%64;
//                    struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = 100;
//                }
                break;
            default:
                break;
            }
        }
    }

    myFlow.resetFeederRG(); //重置流量
}

bool myAutoFeedSetClass::modeSwitchAutoFlowAdjust()
{
    isInBalancing = true;

    bool bResetFeed = false;//是否需要发送给料参数
    int view = struGsh.nViewforstatic;//使用前视计算,还有添加
    for(int i = 0;i < struCnfg.nLayerTotal;i++){
        for(int j = 0;j < struCnfg.struLayerInfo[i].stuViewInfo[view].nIdentifyGroupTotal;j++)
        {
            if((struCnfp.nAutoFeed[i][j].bEnable == 1)
                    &&(struFeedAutoConfigModeSwitch[j].curBalanceStateByLevel = STAT_LEVEL_BALANCE_MODE_SWITCH)
                    &&(struFeedAutoConfigModeSwitch[j].modeSwitchFeedStateChangeToBal == 0))
            {
//               struGsh.autoFeedEnabled = true;
                for(int k = 0;k < struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nGroupUnitTotal;k++){
                    //停运低于门限的料槽
                    int unitIndex = struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nUnitGupId[k]%64;

                    if(struGsh.struStatics[i][view][unitIndex].nStaticsPeriod == 0 || struCnfp.n_coff == 0)
                        break;
//                    qDebug()<<"chute:"<<unitIndex;
                    if(struGsh.struStatics[i][view][unitIndex].nStaticsPeriod > struCnfg.sTotalStaticsGetDuration*13)
                        return false;
                    int flow = (float)struGsh.struStatics[i][view][unitIndex].nThroughout/(float)struCnfp.n_coff*(float)FlowSwitchHour/(float)struGsh.struStatics[i][view][unitIndex].nStaticsPeriod;

                    int offset;

                    offset = flow - struCnfp.nAutoFeed[i][j].mode1FeedTargetPart[0];    //相当于雷达监测模式2

                    if(qAbs(offset) > 60){//目标产量与实际产量相差超过 60kg/h才进行调节
                        int quo = 200;
                        quo = (struCnfp.nAutoFeed[i][j].bQuickBalance == 0) ? 200 : 100;
                        int step = 0;
                        if(qAbs(offset) > 800)
                            step = offset/quo;
                        else if(qAbs(offset) > 600)
                            step = offset/quo;
                        else if(qAbs(offset) > 400)
                            step = offset/quo;
                        else if(qAbs(offset) > 200)
                            step = offset/quo;
                        else if(qAbs(offset) > 100)
                            step = offset/100;
                        else if(qAbs(offset) > 20)
                            step = 1;
                        else
                            step = 0;
                        step = qAbs(step);
                        if(step != 0){
                            if(offset > 0){
                                if((struGsh.chuteFeedMainValue[unitIndex]-step) > 0){
                                    struGsh.chuteFeedMainValue[unitIndex]-=step;
                                }
                            }else{
                                int _t_maxFeed = (struCnfg.stuFeedAutoCtrl.nFeedValueBias[unitIndex] < 100) ? struCnfg.stuFeedAutoCtrl.nFeedValueBias[unitIndex] : 100;
                                struGsh.chuteFeedMainValue[unitIndex] += step;
                                if(struGsh.chuteFeedMainValue[unitIndex] > _t_maxFeed){
                                    struGsh.chuteFeedMainValue[unitIndex] = _t_maxFeed;
                                }
                            }
                            bResetFeed = true;
                        }
                    }
                }
            }
        }
    }
    if(bResetFeed){
        for(int i = 0;i < struCnfg.nLayerTotal;i++){
            for(int j = 0;j < struCnfg.struLayerInfo[i].stuViewInfo[view].nIdentifyGroupTotal;j++)
            {
                if(struCnfp.nAutoFeed[i][j].bEnable == 1){
                    for(int k = 0;k < struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nGroupUnitTotal;k++){
                        //停运低于门限的料槽
                        int unitIndex = struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nUnitGupId[k]%64;

                        struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[unitIndex] = struGsh.chuteFeedMainValue[unitIndex];
                        struGsh.chuteFeedAutoLastValue[unitIndex] = struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[unitIndex];
                    }
                }
            }
        }
    }

    isInBalancing = false;

    return bResetFeed;
}


/********************************************************************************************************
 *
 * mode3:  对应界面模式4，雷达传感器，带自动料位监测+料位稳定，自动目标值
 *
 * 根据料仓位置，自动计算实时目标产量，再根据实时的目标产量，调整各通道流量
 * step 1: mode3CheckCurRicePoSi(), 根据料位，判断处于料空、均衡、料满的哪个阶段，需要连续几次才确定，忽略干扰
 * step 2:mode3ClearBalStateCount(), 状态切换时，需要清除相关变量和计数器
 * step 3:mode3ResetFeed(), 状态切换时，料空、均衡、料满发生状态切换时，需要流量值复位,包括二三四次
 * step 4:mode3GetCurBalanceTargetByPoSi(), 均衡状态下，，根据料位，判断当前均衡目标值属于哪个 分段，从而调整均衡的目标值
 * step 5:mode3AutoFlowAdjust(), 均衡状态下，，根据不同分段的均衡目标值，调整各通道流量，维持在均衡目标值
 * ******************************************************************************************************/

void myAutoFeedSetClass::getCurRadarLevelMode3WhileTurnOn()
{
#if 1
    int view = 0;
    int px = struGsh.struMachineStatus[struGsh.nLayer].nFeedLvelValue[0];
    int f1, f2, f3;
    int p1, p2, p3;

    p1 = struCnfg.stuFeedAutoCtrl.levelEmptyPosi;
    p2 = struCnfg.stuFeedAutoCtrl.levelFullPosi;

    int totalFeed;
    if((struCnfg.nMachineSpliceType == MACHINE_SPLICE_TYPE_BIG)&&(struCnfg.nChuteTotal > 1 || (struCnfg.nChuteTotal == 1 && !struCnfg.nAllInOne)))
    {
        totalFeed = struCnfg.nChuteTotal+1;
    }
    else
    {
        totalFeed = struCnfg.nChuteTotal;
    }

    if(px < struCnfg.stuFeedAutoCtrl.levelFullPosi)    //料满
    {
         struFeedAutoConfigMode3.curBalanceStateByLevel = STAT_LEVEL_FULL_MODE3;
         int view = struGsh.nViewforstatic;
         for(int i = 0;i < struCnfg.nLayerTotal;i++){
             for(int j = 0;j < struCnfg.struLayerInfo[i].stuViewInfo[view].nIdentifyGroupTotal;j++)
             {
                 if(struCnfp.nAutoFeed[i][j].bEnable == 1)
                 {
                     for(int k = 0;k < struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nGroupUnitTotal;k++){
                         int feedId = struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nUnitGupId[k]%64;

                         struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struCnfg.stuFeedAutoCtrl.feedPreFullConstValue;
                     }
                 }
             }
         }
    }
    else if(px < struCnfg.stuFeedAutoCtrl.levelEmptyPosi)   // 料匀
    {
        struFeedAutoConfigMode3.curBalanceStateByLevel = STAT_LEVEL_BALANCE_MODE3;

        int view = struGsh.nViewforstatic;
        for(int i = 0;i < struCnfg.nLayerTotal;i++){
            for(int j = 0;j < struCnfg.struLayerInfo[i].stuViewInfo[view].nIdentifyGroupTotal;j++)
            {
                if(struCnfp.nAutoFeed[i][j].bEnable == 1)
                {
                    for(int k = 0;k < struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nGroupUnitTotal;k++)
                    {
                        int feedId = struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nUnitGupId[k]%64;
                        struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struCnfp.nAutoFeed[i][j].target_feeder;
//                        if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] > struCnfg.stuFeedAutoCtrl.feedEmpytLowLimitValue)
//                        {
//                            struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[i] = struCnfp.nAutoFeed[i][j].target_feeder;
//                        }
                    }
                }
            }
        }
    }
    else // 料空
    {
        struFeedAutoConfigMode3.curBalanceStateByLevel = STAT_LEVEL_EMPTY_MODE3;
        for(int i = 0;i < struCnfg.nLayerTotal;i++)
        {
            for(int j = 0;j < struCnfg.struLayerInfo[i].stuViewInfo[view].nIdentifyGroupTotal;j++)
            {
                if(struCnfp.nAutoFeed[i][j].bEnable == 1)
                {
                    for(int k = 0;k < struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nGroupUnitTotal;k++)
                    {
                        int feedId = struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nUnitGupId[k]%64;
                        struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struCnfg.stuFeedAutoCtrl.feedEmpytLowLimitValue;
                    }
                }
            }
        }
    }

//        for(int i = 0;i < totalFeed;i++)
//        {
//            struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[i] = struCnfg.stuFeedAutoCtrl.feedEmpytLowLimitValue;
//        }
#endif
}

/* step 1：料位状态切换料空、均衡、料满*/
bool myAutoFeedSetClass::mode3CheckCurRicePoSi()
{
    bool feedNeedReset = false;
#if 1
    int view = 0, totalFeed;

    struFeedAutoConfigMode3.curReadIndex = (struFeedAutoConfigMode3.curReadIndex == 0) ? 1 : 0;
    struFeedAutoConfigMode3.tmpPosiSave[struFeedAutoConfigMode3.curReadIndex] = struGsh.struMachineStatus[struGsh.nLayer].nFeedLvelValue[0];
    int curLevel = (struFeedAutoConfigMode3.tmpPosiSave[0]+struFeedAutoConfigMode3.tmpPosiSave[1])/2;

    //int curLevel = struGsh.struMachineStatus[struGsh.nLayer].nFeedLvelValue[0];

    if((struCnfg.nMachineSpliceType == MACHINE_SPLICE_TYPE_BIG)&&(struCnfg.nChuteTotal > 1 || (struCnfg.nChuteTotal == 1 && !struCnfg.nAllInOne)))
    {
        totalFeed = struCnfg.nChuteTotal+1;
    }
    else
    {
        totalFeed = struCnfg.nChuteTotal;
    }

    //qDebug("curBalanceStateByLevel=%d, curLevel=%d", struFeedAutoConfigMode3.curBalanceStateByLevel, curLevel);
    switch(struFeedAutoConfigMode3.curBalanceStateByLevel)
    {
    case STAT_LEVEL_EMPTY_MODE3:   //S∈(L1，Depth], 默认（900，1050）
        if(curLevel <= struCnfg.stuFeedAutoCtrl.levelEmptyPosi)
        {
            if(struFeedAutoConfigMode3.curCountBalStateChg[STAT_LEVEL_EMPTY_MODE3]++ >= countLimitQuick)
            {
                struFeedAutoConfigMode3.curBalanceStateByLevel = STAT_LEVEL_BALANCE_MODE3;
                mode3ClearBalStateCount();
                struFeedAutoConfigMode3.mode3FeedStateBalToEmpty = 0;
                feedNeedReset = true;
                break;
            }
            struFeedAutoConfigMode3.lastRadarPosi = curLevel;
        }
        else
        {
            if(struFeedAutoConfigMode3.mode3FeedStateBalToEmpty == 1)
            {
                if(struFeedAutoConfigMode3.mode3BalToEmptyAdjustCount++ >= struCnfg.stuFeedAutoCtrl.posiChgWait)
                {
                    for(int i = 0;i < struCnfg.nLayerTotal;i++)
                    {
                        for(int j = 0;j < struCnfg.struLayerInfo[i].stuViewInfo[view].nIdentifyGroupTotal;j++)
                        {
                            if(struCnfp.nAutoFeed[i][j].bEnable == 1){
                                for(int k = 0;k < struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nGroupUnitTotal;k++)
                                {
                                    int feedId = struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nUnitGupId[k]%64;

                                    if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] > struCnfg.stuFeedAutoCtrl.feedEmpytLowLimitValue)
                                    {
                                        struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] -= struCnfg.stuFeedAutoCtrl.feedBalAdjustStep;
                                        if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] <= struCnfg.stuFeedAutoCtrl.feedEmpytLowLimitValue)
                                        {
                                            struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struCnfg.stuFeedAutoCtrl.feedEmpytLowLimitValue;
                                            struFeedAutoConfigMode3.mode3FeedStateBalToEmpty = 0;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    feedNeedReset = true;
                    myFlow.resetFeederRG(); //重置流量
                    struFeedAutoConfigMode3.mode3BalToEmptyAdjustCount = 0;
                }

                //myAutoFeed.struFeedAutoConfigMode3.curCountBalChgClock[STAT_LEVEL_EMPTY_MODE3] = 0;
            }
            struFeedAutoConfigMode3.lastRadarPosi = curLevel;
            struFeedAutoConfigMode3.curCountBalStateChg[STAT_LEVEL_EMPTY_MODE3] = 0;
        }
        break;
    case STAT_LEVEL_BALANCE_MODE3: //S∈(L2，Depth-L1]，默认（450， 900]
        if(curLevel > struCnfg.stuFeedAutoCtrl.levelEmptyPosi)
        {
            if(struFeedAutoConfigMode3.curCountBalStateChg[STAT_LEVEL_BALANCE_MODE3]++ >= countLimitSlow)
            {
                struFeedAutoConfigMode3.curBalanceStateByLevel = STAT_LEVEL_EMPTY_MODE3;
                mode3ClearBalStateCount();
                myAutoFeed.struFeedAutoConfigMode3.curCountBalChgClock[STAT_LEVEL_BALANCE_MODE3] = 0;
                struFeedAutoConfigMode3.mode3FeedStateBalToEmpty = 1;
                struFeedAutoConfigMode3.lastRadarPosi = curLevel;
                break;
            }

            if(curLevel > struCnfg.stuFeedAutoCtrl.levelLowPosi)
            {
                if(struFeedAutoConfigMode3.mode3BalLowPosiCount++ >= 2)
                {
                    int view = struGsh.nViewforstatic;
                    for(int i = 0;i < struCnfg.nLayerTotal;i++)
                    {
                        for(int j = 0;j < struCnfg.struLayerInfo[i].stuViewInfo[view].nIdentifyGroupTotal;j++)
                        {
                            if(struCnfp.nAutoFeed[i][j].bEnable == 1)
                            {
                                for(int k = 0;k < struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nGroupUnitTotal;k++){
                                    int feedId = struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nUnitGupId[k]%64;

                                    if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] > struCnfg.stuFeedAutoCtrl.balLowFeedflow)
                                    {
                                        struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] -= 2;
                                        if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] <= struCnfg.stuFeedAutoCtrl.balLowFeedflow)
                                        {
                                            struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struCnfg.stuFeedAutoCtrl.balLowFeedflow;
                                        }
                                    }
                                }

                            }
                        }
                    }
                    struFeedAutoConfigMode3.mode3BalLowPosiCount = 0;
                    feedNeedReset = true;
                    myFlow.resetFeederRG(); //重置流量
                }
            }
            else
            {
                struFeedAutoConfigMode3.mode3BalLowPosiCount = 0;
            }
            struFeedAutoConfigMode3.lastRadarPosi = curLevel;
        }
        else if(curLevel <= struCnfg.stuFeedAutoCtrl.levelFullPosi) //<=450
        {
            if(struFeedAutoConfigMode3.curCountBalStateChg[STAT_LEVEL_BALANCE_MODE3]++ >= countLimitQuick)//料满状态，直接跳转到料满
            {
                struFeedAutoConfigMode3.curBalanceStateByLevel = STAT_LEVEL_FULL_MODE3;
                mode3ClearBalStateCount();
                myAutoFeed.struFeedAutoConfigMode3.curCountBalChgClock[STAT_LEVEL_BALANCE_MODE3] = 0;
                struFeedAutoConfigMode3.mode3FeedStateBalToEmpty = 2;
            }
            struFeedAutoConfigMode3.lastRadarPosi = curLevel;
            struFeedAutoConfigMode3.mode3BalLowPosiCount = 0;
        }
        else
        {
            struFeedAutoConfigMode3.curCountBalStateChg[STAT_LEVEL_BALANCE_MODE3] = 0;
            struFeedAutoConfigMode3.mode3BalLowPosiCount = 0;
            mode3SetCurFeedValueByPoSi(curLevel);
            feedNeedReset = true;
        }
        break;
    case STAT_LEVEL_FULL_MODE3: //S∈ (0，L3]，默认（0， 300）
        if(curLevel > struCnfg.stuFeedAutoCtrl.levelFullPosi) // >300
        {
            struFeedAutoConfigMode3.lastRadarPosi = curLevel;
            if(struFeedAutoConfigMode3.curCountBalStateChg[STAT_LEVEL_FULL_MODE3]++ >= countLimitSlow)
            {
                struFeedAutoConfigMode3.curBalanceStateByLevel = STAT_LEVEL_BALANCE_MODE3;
                mode3ClearBalStateCount();
                struFeedAutoConfigMode3.mode3FeedStatefullToBal = 1;
                struFeedAutoConfigMode3.mode3LevelFullToBalCount = 0;
                feedNeedReset = true;
                break;
            }           
        }
        else
        {
            if(struFeedAutoConfigMode3.mode3FeedStateBalToEmpty == 2)
            {
                int view = struGsh.nViewforstatic;
                for(int i = 0;i < struCnfg.nLayerTotal;i++)
                {
                    for(int j = 0;j < struCnfg.struLayerInfo[i].stuViewInfo[view].nIdentifyGroupTotal;j++)
                    {
                        if(struCnfp.nAutoFeed[i][j].bEnable == 1)
                        {
                            for(int k = 0;k < struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nGroupUnitTotal;k++){
                                int feedId = struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nUnitGupId[k]%64;

                                if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] < struCnfg.stuFeedAutoCtrl.feedPreFullConstValue)
                                {
                                    struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] += struCnfg.stuFeedAutoCtrl.feedBalAdjustStep;
                                    if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] >= struCnfg.stuFeedAutoCtrl.feedPreFullConstValue)
                                    {
                                        struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struCnfg.stuFeedAutoCtrl.feedPreFullConstValue;
                                        struFeedAutoConfigMode3.mode3FeedStateBalToEmpty = 0;
                                    }
                                }
                            }

                        }
                    }
                }
                feedNeedReset = true;
                myFlow.resetFeederRG(); //重置流量
            }
            struFeedAutoConfigMode3.curCountBalStateChg[STAT_LEVEL_FULL_MODE3] = 0;
            struFeedAutoConfigMode3.lastRadarPosi = curLevel;
        }       
        break;
    }

    if((struFeedAutoConfigMode3.curBalanceStateByLevel == STAT_LEVEL_EMPTY_MODE3)
            || (struFeedAutoConfigMode3.curBalanceStateByLevel == STAT_LEVEL_FULL_MODE3))
    {
        struGsh.isRadarStateAlarm = true;
    }
    else
    {
        struGsh.isRadarStateAlarm = false;
    }
#endif

//    if(feedNeedReset ==  true)
//    {
//        myFlow.resetFeederRG(); //重置流量
//        //mode3StateChangeToResetFeed(STAT_LEVEL_BALANCE_MODE3, false, false);
//    }

    //struFeedAutoConfigMode3.lastRadarPosi = curLevel;
    return feedNeedReset;
}

void myAutoFeedSetClass::mode3ClearBalStateCount()
{
    struFeedAutoConfigMode3.feedEmptyCount = 0;
    struFeedAutoConfigMode3.feedFullCount = 0;

    for(int i = 0; i < 6; i++)
    {
        struFeedAutoConfigMode3.curCountBalStateChg[i] = 0;
        struFeedAutoConfigMode3.curCountBalChgClock[i] = 0;
    }

    struFeedAutoConfigMode3.mode3BalToEmptyAdjustCount = 0;
    struFeedAutoConfigMode3.levelMoreCount = 0;
    struFeedAutoConfigMode3.mode3BalLowPosiCount = 0;
}

/* 根据料位，实时更新, 判断是否重置流量 */
void myAutoFeedSetClass::mode3SetCurFeedValueByPoSi(int level)
{
#if 1
    int deltaPosi = abs(struFeedAutoConfigMode3.lastRadarPosi-level);
    //int tmp_step = struCnfg.stuFeedAutoCtrl.feedBalAdjustStep*((deltaPosi/struCnfg.stuFeedAutoCtrl.adjustPosuUnit)+1);
    //int tmp_step = deltaPosi/struCnfg.stuFeedAutoCtrl.adjustPosuUnit;
    int tmp_step = struCnfg.stuFeedAutoCtrl.feedBalAdjustStep;
    int tmp_wait = 0;
    int feedId;
    int tmp_index = 0;

    /*
    for(int i = 0;i < struCnfg.nLayerTotal;i++){
        for(int j = 0;j < struCnfg.struLayerInfo[i].stuViewInfo[view].nIdentifyGroupTotal;j++)
        {
            if(struCnfp.nAutoFeed[i][j].bEnable == 1)
            {
                for(int k = 0;k < struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nGroupUnitTotal;k++){
                    tmp_index = struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nUnitGupId[k]%64;
                    break;
                }
            }
        }
    }

    if((struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[tmp_index] > struCnfg.stuFeedAutoCtrl.balLowFeedflow)
        &&(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[tmp_index] < struCnfg.stuFeedAutoCtrl.balUpFeedflow))
    {
        tmp_wait = struCnfg.stuFeedAutoCtrl.posiChgWait*1.5;
    }
    else
    {
        tmp_wait = struCnfg.stuFeedAutoCtrl.posiChgWait;
    }
    */
    tmp_wait = struCnfg.stuFeedAutoCtrl.posiChgWait;

    //tmp_step = (tmp_step <= 10) ? tmp_step : 10;
    qDebug("tmp_step = %d, lastRadarPosi=%d, cur level=%d, posiChgThres=%d, posiDescendCount=%d, posiChgWait=%d",tmp_step,
           struFeedAutoConfigMode3.lastRadarPosi, level, struCnfg.stuFeedAutoCtrl.posiChgThres,
           struFeedAutoConfigMode3.posiDescendCount, struCnfg.stuFeedAutoCtrl.posiChgWait);

    int view = struGsh.nViewforstatic;
    /*
    if(struGsh.bfeedAutoHasWipe == 1)
    {
        for(int i = 0;i < struCnfg.nLayerTotal;i++){
            for(int j = 0;j < struCnfg.struLayerInfo[i].stuViewInfo[view].nIdentifyGroupTotal;j++)
            {
                if(struCnfp.nAutoFeed[i][j].bEnable == 1)
                {
                    for(int k = 0;k < struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nGroupUnitTotal;k++){
                        int feedId = struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nUnitGupId[k]%64;

                        struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] += 1;
                        if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] > struCnfg.stuFeedAutoCtrl.balUpFeedflow)
                        {
                            struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struCnfg.stuFeedAutoCtrl.balUpFeedflow;
                        }
                        struGsh.chuteFeedAutoLastValue[feedId] = struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId];
                    }
                }
            }
        }
        struGsh.bfeedAutoHasWipe = 0;
        myFlow.resetFeederRG(); //重置流量
    }*/

    for(int i = 0;i < struCnfg.nLayerTotal;i++){
        for(int j = 0;j < struCnfg.struLayerInfo[i].stuViewInfo[view].nIdentifyGroupTotal;j++)
        {
            if(struCnfp.nAutoFeed[i][j].bEnable == 1)
            {
                if(level < struCnfg.stuFeedAutoCtrl.levelMorePosi)
                {
                    if(struFeedAutoConfigMode3.levelMoreCount++ >= 2)
                    {
                        for(int k = 0;k < struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nGroupUnitTotal;k++){
                            feedId = struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nUnitGupId[k]%64;

                            struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] += 2;
                            if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] >= struCnfg.stuFeedAutoCtrl.balUpFeedflow)
                            {
                                struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struCnfg.stuFeedAutoCtrl.balUpFeedflow;
                            }
                            struGsh.chuteFeedAutoLastValue[feedId] = struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId];;
                        }
                        struFeedAutoConfigMode3.levelMoreCount = 0;
                        myFlow.resetFeederRG(); //重置流量
                    }
                    struFeedAutoConfigMode3.lastRadarPosi = level;
                    struFeedAutoConfigMode3.posiDescendCount = 0;
                    struFeedAutoConfigMode3.posiRiseCount = 0;
                }
                else
                {
                    struFeedAutoConfigMode3.levelMoreCount = 0;
                    if(deltaPosi > struCnfg.stuFeedAutoCtrl.posiChgThres)
                    {
                        if(struFeedAutoConfigMode3.lastRadarPosi < level)
                        {
                            struFeedAutoConfigMode3.posiRiseCount = 0;
                            if(struFeedAutoConfigMode3.posiDescendCount++ >= tmp_wait)
                            {
                                for(int k = 0;k < struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nGroupUnitTotal;k++){
                                    feedId = struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nUnitGupId[k]%64;

                                    if(level > struCnfg.stuFeedAutoCtrl.levelLowPosi)
                                    {
                                        struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] -= 2;
                                    }
                                    else
                                    {
                                        struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] -= tmp_step;
                                    }

//                                    struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] -= tmp_step;
                                    if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] < struCnfg.stuFeedAutoCtrl.balLowFeedflow)
                                    {
                                        struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struCnfg.stuFeedAutoCtrl.balLowFeedflow;
                                    }
                                    struGsh.chuteFeedAutoLastValue[feedId] = struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId];
                                }
                                struFeedAutoConfigMode3.lastRadarPosi = level;
                                struFeedAutoConfigMode3.posiDescendCount = 0;
                                myFlow.resetFeederRG(); //重置流量
                            }
                        }
                        else
                        {
                            struFeedAutoConfigMode3.posiDescendCount = 0;

                            if(struFeedAutoConfigMode3.posiRiseCount++ >= tmp_wait)
                            {
                               // int tmpUpFeed = (level < )
                                for(int k = 0;k < struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nGroupUnitTotal;k++){
                                    feedId = struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nUnitGupId[k]%64;

                                    struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] += tmp_step;
                                    if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] > struCnfg.stuFeedAutoCtrl.balUpFeedflow)
                                    {
                                        struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struCnfg.stuFeedAutoCtrl.balUpFeedflow;
                                    }
                                    struGsh.chuteFeedAutoLastValue[feedId] = struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId];
                                }
                                struFeedAutoConfigMode3.lastRadarPosi = level;
                                struFeedAutoConfigMode3.posiRiseCount = 0;
                                myFlow.resetFeederRG(); //重置流量
                            }
                        }

                    }
                }
            }
        }
    }

#if 0   //二次流量固定
    if(struFeedAutoConfigMode3.mode3FeedStatefullToBal == 1)
    {
        if(struFeedAutoConfigMode3.mode3LevelFullToBalCount++ > struCnfg.stuFeedAutoCtrl.feedFullToNormalContinueTime*countPerMin)
        {
            struFeedAutoConfigMode3.mode3LevelFullToBalCount = struCnfg.stuFeedAutoCtrl.feedFullToNormalContinueTime*countPerMin+1;
            for(int i = 0;i < struCnfg.nLayerTotal;i++)
            {
                for(int j = 0;j < struCnfg.struLayerInfo[i].stuViewInfo[view].nIdentifyGroupTotal;j++)
                {
                    if(struCnfp.nAutoFeed[i][j].bEnable == 0)
                    {
                        for(int k = 0;k < struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nGroupUnitTotal;k++)
                        {
                            feedId = struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nUnitGupId[k]%64;

                            struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struGsh.chuteFeedAutoLastValue[feedId];
                        }
                    }
                }
            }
            struFeedAutoConfigMode3.mode3FeedStatefullToBal = 0;
            myFlow.resetFeederRG(); //重置流量
        }
    }
#endif
#endif
}

/* 此函数不用，模式4，料位变化，导致状态料空、均衡、料满发生状态切换时，需要流量复位,包括二三四次 */
void myAutoFeedSetClass::mode3StateChangeToResetFeed(int index, bool isChangeToBal, bool needDelay)
{
#if 1
    int view = 0, totalFeed = 0;

    if((struCnfg.nMachineSpliceType == MACHINE_SPLICE_TYPE_BIG)&&(struCnfg.nChuteTotal > 1 || (struCnfg.nChuteTotal == 1 && !struCnfg.nAllInOne)))
    {
        totalFeed = struCnfg.nChuteTotal+1;
    }
    else
    {
        totalFeed = struCnfg.nChuteTotal;
    }

    switch(index)
    {
    case STAT_LEVEL_EMPTY_MODE3:
        if(myAutoFeed.struFeedAutoConfigMode3.curCountBalChgClock[STAT_LEVEL_EMPTY_MODE3]++ >= struCnfg.stuFeedAutoCtrl.feedBalAdjustDuration/2)
        {
            for(int i = 0;i < struCnfg.nLayerTotal;i++){
                for(int j = 0;j < struCnfg.struLayerInfo[i].stuViewInfo[view].nIdentifyGroupTotal;j++)
                {
                    if(struCnfp.nAutoFeed[i][j].bEnable == 1){
                        for(int k = 0;k < struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nGroupUnitTotal;k++)
                        {
                            int feedId = struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nUnitGupId[k]%64;

                            if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] > struCnfg.stuFeedAutoCtrl.feedEmpytLowLimitValue)
                            {
                                struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] -= struCnfg.stuFeedAutoCtrl.feedBalAdjustStep;
                                if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] <= struCnfg.stuFeedAutoCtrl.feedEmpytLowLimitValue)
                                {
                                    struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struCnfg.stuFeedAutoCtrl.feedEmpytLowLimitValue;
                                }
                            }
                        }
                    }
                }
            }
            myAutoFeed.struFeedAutoConfigMode3.curCountBalChgClock[STAT_LEVEL_EMPTY_MODE3] = 0;
        }
        break;
    case STAT_LEVEL_BALANCE_MODE3:
/*
        view = struGsh.nViewforstatic;//使用前视计算,还有添加
        if(struFeedAutoConfigMode3.mode3FeedStateChangeToBal > 0)
        {
            if(struFeedAutoConfigMode3.mode3FeedStateChangeToBal == 1)
            {
                for(int i = 0;i < struCnfg.nLayerTotal;i++){
                    for(int j = 0;j < struCnfg.struLayerInfo[i].stuViewInfo[view].nIdentifyGroupTotal;j++)
                    {
                        if(struCnfp.nAutoFeed[i][j].bEnable == 0){
                            if(struFeedAutoConfigMode3.mode3LevelFullToBalCount++ > struCnfg.stuFeedAutoCtrl.feedFullToNormalContinueTime*countPerMin)
                            {
                                struFeedAutoConfigMode3.mode3LevelFullToBalCount = struCnfg.stuFeedAutoCtrl.feedFullToNormalContinueTime*countPerMin+1;
                                for(int k = 0;k < struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nGroupUnitTotal;k++)
                                {
                                    int feedId = struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nUnitGupId[k]%64;

                                    struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struGsh.chuteFeedAutoLastValue[feedId];
                                    struGsh.chuteFeedMainValue[feedId] =  struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId];
                                }
                            }
                        }
                    }
                }
            }

            if(myAutoFeed.struFeedAutoConfigMode3.curCountBalChgClock[STAT_LEVEL_BALANCE_MODE3]++ >= struCnfg.stuFeedAutoCtrl.feedBalAdjustDuration/2)
            {
                struFeedAutoConfigMode3.curCountBalChgClock[STAT_LEVEL_BALANCE_MODE3] = 0;

                struFeedAutoConfigMode3.mode3FeedStateChangeToBal  = 0;
            }
        }
        else
        {
            for(int i = 0;i < struCnfg.nLayerTotal;i++){
                for(int j = 0;j < struCnfg.struLayerInfo[i].stuViewInfo[view].nIdentifyGroupTotal;j++)
                {
                    if(struCnfp.nAutoFeed[i][j].bEnable == 0){
                        for(int k = 0;k < struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nGroupUnitTotal;k++)
                        {
                            int feedId = struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nUnitGupId[k]%64;

                            struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struGsh.chuteFeedAutoLastValue[feedId];
                            struGsh.chuteFeedMainValue[feedId] =  struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId];
                        }
                    }
                    else
                    {
                        if(struFeedAutoConfigMode3.mode3LevelFullToBalCount++ > struCnfg.stuFeedAutoCtrl.feedFullToNormalContinueTime*countPerMin)
                        {
                            struFeedAutoConfigMode3.mode3LevelFullToBalCount = struCnfg.stuFeedAutoCtrl.feedFullToNormalContinueTime*countPerMin+1;
                            for(int k = 0;k < struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nGroupUnitTotal;k++)
                            {
                                int feedId = struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nUnitGupId[k]%64;

                                struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struGsh.chuteFeedAutoLastValue[feedId];
                                struGsh.chuteFeedMainValue[feedId] =  struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId];
                            }
                        }
                    }
                }
            }
        }
*/
        break;
    case STAT_LEVEL_FULL_MODE3:
        if(myAutoFeed.struFeedAutoConfigMode3.curCountBalChgClock[STAT_LEVEL_FULL_MODE3]++ >= struCnfg.stuFeedAutoCtrl.feedBalAdjustDuration/2)
        {
            int view = struGsh.nViewforstatic;
            for(int i = 0;i < struCnfg.nLayerTotal;i++){
                for(int j = 0;j < struCnfg.struLayerInfo[i].stuViewInfo[view].nIdentifyGroupTotal;j++)
                {
                    if(struCnfp.nAutoFeed[i][j].bEnable == 1)
                    {
                        for(int k = 0;k < struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nGroupUnitTotal;k++){
                            int feedId = struCnfg.struLayerInfo[i].stuViewInfo[view].stuIdenGupInfo[j].nUnitGupId[k]%64;

                            struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedId] = struCnfg.stuFeedAutoCtrl.feedPreFullConstValue;
                        }
                    }
                }
            }
            myAutoFeed.struFeedAutoConfigMode3.curCountBalChgClock[STAT_LEVEL_FULL_MODE3] = 0;
        }
        break;
    default:
        break;
    }

    myFlow.resetFeederRG(); //重置流量
#endif
}
