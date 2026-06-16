// [zcy 2026.06.15] mqtt config
// 为了适配 MQTT 的第三方支持和各项文件依赖所进行的改动
#include "mqttsrv.h"
#include "basewidget.h"
#include <QtNetwork>
#ifdef Q_OS_UNIX
mqttThread *myMqttThread;
mqttMsgParaseThread *myMqttMsgParaseThread;
#endif

/*
 *  2020/11/2  数据传输addr = https:// + testip + url
 */

void mqttsrv::on_subscribe(int mid, int qos_count, const int *granted_qos)
{
    qDebug()<<"订阅 mid = "<<mid;

}
void mqttsrv::on_message(const struct mosquitto_message *message)
{
    bool res = false;
    string topic = QString("/" + myHttpFileClient->g_connectName).toStdString();

    mosqpp::topic_matches_sub(topic.c_str(),message->topic,&res);

    if( res )
    {
        strRcv = (char*)message->payload;
        QMutexLocker lock(&m_paraseLock);
        recvMsgList.append(strRcv);
        qDebug()<<QString().fromStdString(message->topic)<<QString().fromStdString(strRcv)<<"\n";
    }
}


/* ****************************************************************************
 *
 *                        MQTT远程控制线程
 *
 ******************************************************************************/
#ifdef Q_OS_UNIX
mqttThread::mqttThread(QThread *parent) : QThread(parent)
{
    m_connect_name = QString(myHttpFileClient->g_connectName).toStdString();

    networkConnectIsOk = 0;
    struGsh.bFlagMqttConnect = 0;
    topic = "/";
    topic += myHttpFileClient->g_connectName;
    test = NULL;
    if (myHttpFileClient->g_connectName != "")
        test = new mqttsrv(myHttpFileClient->g_connectName.toLocal8Bit().constData());
}
void mqttThread::run()
{
    sleep(3);
    if (myHttpFileClient->g_connectName != "")
    {
        QProcess m_cmd;
        for(int i =0;i< 15;i++){
            // m_cmd.start("ping -w 5 ntp.ntsc.ac.cn");
            m_cmd.start("ping -w 5 cloud.chinaamd.cn");
            if(m_cmd.waitForFinished())
            {
                QString str = m_cmd.readAll();
                if(!str.contains("ttl"))
                {
                    m_cmd.kill();
                    sleep(20);
                    continue;
                }
                struGsh.bFlagMqttConnect = 1;
                break;
            }
            m_cmd.close();
            sleep(20);
        }
    }
    networkConnectIsOk = struGsh.bFlagMqttConnect;

    if(test == NULL) return;
    if(!struGsh.bFlagMqttConnect) {
        logger()->info("network connect fail");
        return;
    }

    /************** 微信小程序开启 *************/
    mosqpp::lib_init();
    char buf[1024] = "Color Sorter";
    int a,b,c,d;
    a = mosqpp::lib_version(&b,&c,&d);
    int rc;
    /* 尝试采用鉴权 */
    test->tls_opts_set(1,"tlsv1",NULL);
    test->tls_insecure_set(true);
    rc = test->tls_set("./root.crt", NULL);

    QSettings setting(CFG_APPSet,QSettings::IniFormat);
    QString testip = setting.value("testIp","cloud.chinaamd.cn").toString();

    qDebug()<<"iot addr : "<<testip;
    rc = test->connect(testip.toLocal8Bit().constData(),1884,50);
    if( rc == MOSQ_ERR_ERRNO )
    {
        while (1)
        {
            rc = test->connect(testip.toLocal8Bit().constData(),1884,50);//本地IP
            if (MOSQ_ERR_SUCCESS == rc)
            {
                qDebug()<<"connect ok";
                break;
            }
            qDebug()<<"connecting...";
            sleep(1);
        }
    }
    if ( MOSQ_ERR_SUCCESS == rc )
    {
#if 0
        //发布测试
        rc = test->loop ();
        if ( MOSQ_ERR_SUCCESS == rc )
        {
            qDebug()<<"publishing...";
            rc = test->publish(NULL, "MqttServerClient", strlen(buf), (const void *)buf);
            rc = test->loop();
        }

        qDebug()<<"subscribing...";
#endif
        rc = test->subscribe(NULL,topic.toLocal8Bit().constData());
        if (rc != 0)
        {
            rc = test->subscribe(NULL,topic.toLocal8Bit().constData());
        }
        if (rc != 0)
        {
            rc = test->subscribe(NULL,topic.toLocal8Bit().constData());
            //rc = test.loop();
        }

        if (rc != 0)
        {
            mosqpp::lib_cleanup();
            qDebug()<<"MQTT连接连接失败";
            return ;
        }
        qDebug()<<"MQTT连接成功";

        //        test.loop_forever();						//订阅测试取消注释该行

        system("ntpdate 0.pool.ntp.org");  //同步时间专用
        system("hwclock -w");

        while (1)
        {
            rc = test->loop();
            if(rc){
                struGsh.bFlagMqttConnect = 0;
                QString dateText = QDateTime::currentDateTime().toString("h:mm:ss");
                qDebug()<<dateText<<"reconnect";
                logger()->info("reconnect");
                sleep(5);
                rc = test->reconnect();
                if(rc == 0)
                {
                    struGsh.bFlagMqttConnect = 1;
                    test->subscribe(NULL,topic.toLocal8Bit().constData());
                }
            }
            msleep(100);
        }
    }
    mosqpp::lib_cleanup();
    return ;
}


mqttMsgParaseThread::mqttMsgParaseThread(QThread *parent) : QThread(parent)
{
    msg = "";
    n_imagePath = "";
    n_uploadImageFlag = 0;
    n_uploadWaveFlag  = 0;
    n_uploadBlowCountsFlag = 0;
    n_uploadWipeSpeedFlag = 0;

    n_uploadPartsStatusFlag = 0;
    n_uploadAlarmFlag = 0;
    n_uploadStatisticsFlag = 0;
    n_uploadRealTimeParaFlag = 0;

    n_uploadValveAudioFlag = 0;
    n_uploadSchemeInfoFlag = 0;
    n_uploadIpcNetTestInfoFlag = 0;
    n_uploadLogFileFlag = 0;
    n_getModelListFlag = 0;
    n_uploadModelEvaluate = 0;
    n_downLoadFlag = 0;
    n_uploadRemoteControlFlag = 0;
    n_uploadModelParamsFlag = 0;

    m_uploadStat = 0;
    n_uploadOneKeyFixFlag = 0;
    reportFlag = Normal_Send;
    m_nextWeek = 0;
    m_updateFileType = UPDATE_NULL;

    static const QDate m_buildDate = QLocale( QLocale::English ).toDate( QString(__DATE__).replace("  ", " 0"), "MMM dd yyyy");
    //    static const QTime m_buildTime = QTime::fromString(__TIME__, "hh:mm:ss");
    //    QString buildInfo = QString("%1 : %2 %3").arg(g_myLan().build_time).arg(buildDate.toString("yyyy.MM.dd")).arg(buildTime.toString());
    QString versionStr = QString(SCREEN_VERSION)+"("+m_buildDate.toString("yyyyMMdd")+")";
    m_version = QString(versionStr).toStdString();
    logger()->info("Ver:%1",versionStr);

    connect(this,SIGNAL(StartRemoteControl(QString) ),this,SLOT(DealStartRemoteControl(QString)));
    connect(myFlow.myNetWork->udpthread,SIGNAL(SignalImageRecvFinished(int,QString)),this,SLOT(imgaeCaptureOver(int,QString)));
    connect(myFlow.myNetWork->udpthread,SIGNAL(SignalWaveRecvFinished()),this,SLOT(waveCaptureOver()));
    connect(myFlow.myNetWork->udpthread,SIGNAL(SignalBlowCountsRecvFinished()),this,SLOT(blowCountsCaptureOver()) , Qt::DirectConnection);
    connect(this,SIGNAL(SeriesBlowCount(int,int) ),this,SLOT(DealSeriesBlowCount(int,int)));
}

void mqttMsgParaseThread::run()
{
#if HTTP_TEST
    sleep(10);
    //开机最多重连5分钟
    for(int i = 0;i < 10;i++)
    {
        if (!myMqttThread->networkConnectIsOk){
            sleep(30);
            continue;
        }
        myHttpFileClient->requestTokenUpdate(URL_TOKEN_REQUEST);
        if (myHttpFileClient->g_token == "")
        {
            sleep(30);
            continue;
        }
        break;
    }
#endif

    uploadLocalRemoteInfo();

    upLoadSchemeInfo();

    uploadPara();

    onModelParamsUpload();

    //    uploadRealTimePara();

    uploadMachinePara();

    uploadLogFile();



    if(myHttpFileClient->g_token == "" || !myMqttThread->test)
        return;

    myHttpFileClient->requestModelLableList(URL_MODEL_TYPEANDLIST);


    for(;;){
        sleep(1);
        reportFlag = Normal_Send;

        if(myHttpFileClient->g_token_expires < struGsh.nCounter - m_nextWeek ){
            m_nextWeek = struGsh.nCounter;
            myHttpFileClient->g_token_expires = 0;
            myHttpFileClient->requestTokenUpdate(URL_TOKEN_REQUEST);
            if(myHttpFileClient->g_token_expires == 0){
                logger()->error("requestToken timeout expire");
                sleep(60);
            }
        }

        if(n_uploadPartsStatusFlag){
            uploadIpcPartsStatusInfo();
            struGsh.nIpcRestartFlag = 0;
        }
        if(n_uploadAlarmFlag){
            uploadParaAlarm();
        }
        if(n_uploadWipeSpeedFlag){
            onWipeSpeedUpload();
        }
        if(n_uploadRealTimeParaFlag)
        {
            uploadRealTimePara();
        }
        if(n_uploadSchemeInfoFlag){
            upLoadSchemeInfo();
            uploadPara();
        }
        if(n_uploadStatisticsFlag){
            uploadStatistics();
        }
        if(n_uploadBlowCountsFlag){
            uploadBlowCounts();
        }
        if(n_uploadValveAudioFlag){
            onVavleAudioUpload();
        }
        if(n_uploadLogFileFlag){
            QString dateStr = QDateTime::currentDateTime().toString("yyyy-MM-dd");
            uploadLogFile(dateStr.toStdString());
        }
        if(n_getModelListFlag)
        {
            getModeList();
            emit getModelistResult_sig();
        }
        if(n_uploadModelEvaluate)
        {
            uploadModelEvaluate();
            //emit uploadEvaluateResult_sig();
        }
        if(n_uploadRemoteControlFlag){

            uploadRemoteControl();
            reportFlag = Normal_Send;
        }
        if(n_uploadModelParamsFlag)
        {
            onModelParamsUpload();
            reportFlag = Normal_Send;
        }
        if(n_uploadOneKeyFixFlag)
        {
            uploadOneKeyFixInfo();
            reportFlag = Normal_Send;
        }
        if(n_downLoadFlag)
        {
            QString value;
            QSettings setting(CFG_APPSet,QSettings::IniFormat);
            QString testip = setting.value("testIp","cloud.chinaamd.cn").toString();

            m_tmp = "CMD_UpdateIpcModel";
            m_uuid = "";
            //拼接命令内容
            value += HTTPS+testip+URL_MODEL_DOWNLOAD + QString("%1").arg(struGsh.stumodelData.modelpath[struGsh.stumodelData.checkindex]) + "::"; // url
            value += " ::"; // md5
            value += " ::"; // ver
            value += "1"; // force

            onUpdateZKSort(value.toStdString());
        }

        while(!myMqttThread->test->recvMsgList.isEmpty()){

            reportFlag = Request_Send;
            msg = myMqttThread->test->recvMsgList[0];
            doParseMessage();
            {
                QMutexLocker lock(&myMqttThread->test->m_paraseLock);
                myMqttThread->test->recvMsgList.removeAt(0);
            }
        }
        msleep(200);
    }
}


void mqttMsgParaseThread::doParseMessage()
{
    //    string utfRec = "[{\"cmdCode\":\"CMD_Feed\",\"content\":\"50:20:30:80:50:20:30:80:50:20\",\"devCode\":\"ZKGDDEV47\",\"uuid\":\"0a2de935-b381-fd09-f68b-b071ba65c275\"},{\"cmdCode\":\"CMD_OpenSwitchs\",\"content\":\"0:0:0:0:0:0:0:0:0:0:0:0:0:0\",\"devCode\":\"ZKGDDEV47\",\"uuid\":\"ea53e47e-c3b5-874e-c04f-454c699ee9a6\"}]";
    string utfRec ="{\"cmd\":"+msg+"}";

    Json::Reader reader;
    Json::Value root;
    if (reader.parse(utfRec.c_str(), root)) // reader将Json字符串解析到root，root将包含Json里所有子元素
    {

        int size = root["cmd"].size();

        for(int i = 0; i < size; i++)
        {
            Json::Value val_image = root["cmd"][i]["cmdCode"];
            string tmp =  val_image.asString();
            string value = root["cmd"][i]["content"].asString();
            string uuid = root["cmd"][i]["uuid"].asString();


            if( tmp == string("CMD_LockStatusUpload") )
            {
                string value_lock = root["cmd"][i]["content"].asString();
                string uuid_lock = root["cmd"][i]["uuid"].asString();
                if(struGsh.remoControlmode == 1 || struGsh.remoControlmode == 2 )
                {
                    if(QString::fromStdString(value_lock) == userphonenum)
                        onCmdReturn(uuid_lock,tmp,SUCCESS);
                    else
                        onCmdReturn(uuid_lock,tmp,OCCUPY_ERROR);
                }
                else
                    onCmdReturn(uuid_lock,tmp,LOCKED_ERROR);

                reportFlag = Normal_Send;
                return;
            }


            /*　先回传指令收到响应　*/
            onCmdReturn(uuid,tmp,RECEIVE);

            if(tmp == string("CMD_ConnectVPN"))
            {
                int errorcode = onVPNConnect(value);
                onCmdReturn(uuid,tmp,errorcode);
                return;
            }

            /*  正在清灰不执行;
                采图界面，工程师设置界面，方案切换界面不执行  */
            if(struGsh.bFlagWiping){
                onCmdReturn(uuid,tmp,CLEAN_ING);//返回 清灰中```
                return;
            }
            if(struGsh.nVersionCaptureIsRun){
                onCmdReturn(uuid,tmp,NOT_MAIN_WND);//返回 清灰中```
                return;
            }
            if(!struCnfg.bFlagFirstrun){
                onCmdReturn(uuid,tmp,SYSTEM_ERROR);//返回
                return;
            }
            if(struGsh.offlineDetection || struGsh.remoteOnekeytest)
            {
                onCmdReturn(uuid,tmp,CMD_ISRUNNING);//返回
                return;
            }

            basewidget* widget = g_MainManager().GetCurWidget();
            int curPageId = widget->m_CurrentPageId;
            if(curPageId != SM_MAIN_PAGE_WIDGET_NEW && curPageId != SM_REMOTE_CONTROL_WIDGET)
            {
                if(curPageId != SM_MAIN_PAGE_WIDGET_NEW)
                {
                    onCmdReturn(uuid,tmp,NOT_MAIN_WND);
                    return;
                }
                else
                {
                    onCmdReturn(uuid,tmp,LOCKED_ERROR);
                    return;
                }

            }


            //            logger()->info("recv CMD %1,value %2",QString::fromStdString(tmp),QString::fromStdString(value));
            if (tmp == string("CMD_OpenSwitch"))
            {
                int errorcode = onOpenSwitch(value);
                if (errorcode == SUCCESS)
                {
                    uploadPara();
                }
                onCmdReturn(uuid,tmp,errorcode);
                return;
            }
            else if(tmp == string("CMD_RemoteControl"))
            {
                QString strVec = QString().fromStdString(value);
                QStringList vecList = strVec.split(":");
                int cmd = vecList.at(0).toInt();
                int time = vecList.at(1).toInt();
                QString phonenum = vecList.at(2);
                onRemotecontrol(cmd,time,phonenum);

                reportFlag = Normal_Send;
                if(struGsh.remoControlmode != 2)
                    onCmdReturn(uuid,tmp,struGsh.remoteControlstatus);
                else
                    remotelock_uuid = uuid ;
                return;
            }
            else if (tmp == string("CMD_Feed"))
            {
                int errorcode = onCmdFeed(value);
                if (errorcode == SUCCESS)
                {
                    uploadPara();
                }
                onCmdReturn(uuid,tmp,errorcode);
                return;

            }
            else if(tmp == string("CMD_ShutDown"))
            {
                //关机命令收到时先回传成功标识
                onCmdReturn(uuid,tmp,SUCCESS);
                onShutDown();
                return;
            }
            else if(tmp == string("CMD_MemSet"))
            {
                int errorcode = onMemSet(value,uuid);
                if (errorcode == SUCCESS)
                {
                    uploadPara();
                }
                onCmdReturn(uuid,tmp,errorcode);
                return;
            }
            else if(tmp == string("CMD_OpenSwitchs"))
            {
                int errorcode = onOpenSwitchs(value);
                if (errorcode == SUCCESS)
                {
                    uploadPara();
                }
                onCmdReturn(uuid,tmp,errorcode);
                return;
            }
            else if(tmp == string("CMD_SaveCfg"))
            {
                int errorcode = onSaveCfg();
                if (errorcode == SUCCESS)
                {
                    uploadPara();
                    onModelParamsUpload();
                }
                onCmdReturn(uuid,tmp,errorcode);
                return;
            }
            else if(tmp == string("CMD_GlobalParaSet"))
            {
                int errorcode = onGlobalParaSet(value);
                if (errorcode == SUCCESS)
                {
                    uploadPara();
                }
                onCmdReturn(uuid,tmp,errorcode);
                return;
            }
            else if (tmp == string("CMD_PushVPNCert"))
            {
                int errorcode = onVPNPushCert(value);
                onCmdReturn(uuid,tmp,errorcode);
                return;
            }
            //            else if (tmp == string("CMD_ConnectVPN"))
            //            {
            //                int errorcode = onVPNConnect(value);
            //                onCmdReturn(uuid,tmp,errorcode);
            //                return;
            //            }
            else if (tmp == string("CMD_RealTimeParaUpload"))
            {
                uploadRealTimePara();
                onCmdReturn(uuid,tmp,0);
                return;
            }
            else if (tmp == string("CMD_MachineParaUpload"))
            {
                uploadMachinePara();
                onCmdReturn(uuid,tmp,0);
                return;
            }
            else if (tmp == string("CMD_PartsStatusInfoUpload"))
            {
                int ret = uploadIpcPartsStatusInfo();
                onCmdReturn(uuid,tmp,ret);
                return;
            }
            else if (tmp == string("CMD_ParaAlarmUpload"))
            {
                uploadParaAlarm();
                onCmdReturn(uuid,tmp,0);
                return;
            }
            else if (tmp == string("CMD_StatisticsUpload"))
            {
                //                MySqlite mysql;
                //                mysql.updateTable();
                uploadBlowCounts();
                uploadStatistics();
                onCmdReturn(uuid,tmp,0);
                return;
            }
            else if (tmp == string("CMD_CaptureImage") || tmp == string("CMD_CaptureWave"))
            {
                m_tmp = tmp;
                m_uuid = uuid;
                if ( struGsh.nIpcAlarmImageCaptureIsRun == 1)
                {
                    onCmdReturn(uuid,tmp,OTHER_ERR);
                    return;
                }
                int errorcode = onCaptureImage(value,tmp);
                onCmdReturn(uuid,tmp,errorcode);
                return;
            }
            else if (tmp == string("CMD_LogfileUpload"))
            {
                int errorcode = onLogfileUpload(value);
                onCmdReturn(uuid,tmp,errorcode);
                return;
            }
            else if (tmp == string("CMD_CnffileUpload"))
            {
                int errorcode = onCnffileUpload();
                onCmdReturn(uuid,tmp,errorcode);
                return;
            }
            else if (tmp == string("CMD_WipeSpeedUpload"))
            {
                int errorcode = onWipeSpeedUpload();
                onCmdReturn(uuid,tmp,errorcode);
                return;
            }
            else if (tmp == string("CMD_BlowCountsUpload"))
            {
                int errorcode = onBlowCountsUpload();
                onCmdReturn(uuid,tmp,errorcode);
                return;
            }
            else if (tmp == string("CMD_VavleAudioUpload"))
            {
                int errorcode = onVavleAudioUpload();
                onCmdReturn(uuid,tmp,errorcode);
                return;
            }
            else if (tmp == string("CMD_CaptureAllImage"))
            {
                m_tmp = tmp;
                m_uuid = uuid;
                if ( struGsh.nIpcAlarmImageCaptureIsRun == 1)
                {
                    onCmdReturn(uuid,tmp,OTHER_ERR);
                    return;
                }
                int errcode = onCaptureAllImage();

                onCmdReturn(uuid,tmp,errcode);
                return;
            }
            else if (tmp == string("CMD_UpdateZKSort")|| (tmp == string("CMD_UpdateIpcModel")))
            {
                m_tmp = tmp;
                m_uuid = uuid;
                onUpdateZKSort(value);
                return;
            }
            else if (tmp == string("CMD_Shell_Cmd"))
            {
                m_tmp = tmp;
                m_uuid = uuid;
                onShellCmd(value);
                return;
            }
            else if (tmp == string("CMD_IpcInfoUpload"))
            {
                m_tmp = tmp;
                m_uuid = uuid;
                onIpcInfoUpload(value);
                return;
            }
            else if(tmp ==  string("CMD_ModelParamsUpload"))
            {
                m_tmp = tmp;
                m_uuid = uuid;
                int errcode = onModelParamsUpload();
                onCmdReturn(uuid,tmp,errcode);
            }
            else if(tmp ==  string("CMD_ModelParamsSet"))
            {
                m_tmp = tmp;
                m_uuid = uuid;
                int errcode = onModelParamsSet(value);
                if(errcode == SUCCESS)
                {
                    onModelParamsUpload();
                }
                onCmdReturn(uuid,tmp,errcode);
            }
            else if(tmp ==  string("CMD_SeriesBlowCountsUpload"))
            {
                m_tmp = tmp;
                m_uuid = uuid;
                QString strVec = QString().fromStdString(value);
                QStringList vecList = strVec.split(":");
                int fre = vecList.at(0).toInt();
                int total_time =  vecList.at(1).toInt();

                int errorcode = onSeriesBlowCounts(total_time,fre);
                onCmdReturn(uuid,tmp,errorcode);
                return;
            }
            else if(tmp ==  string("CMD_OfflineDetection")) // 下线检测
            {
                m_tmp = tmp;
                m_uuid = uuid;
                int errorcode =  offlineDetection(value);
                onCmdReturn(uuid,tmp,errorcode);
                resultinfo.clear();
                return;
            }
            else if(tmp ==  string("CMD_RemoteOneKeyTest")) //一键调试
            {
                m_tmp = tmp;
                m_uuid = uuid;
                onRemoteOneKeyTest(value);
                return;
            }
            else if(tmp ==  string("CMD_UpdateAllPara")) //上传所有参数
            {
                m_tmp = tmp;
                m_uuid = uuid;
                uploadPara();
                onModelParamsUpload();
                onCmdReturn(uuid,tmp,SUCCESS);
                return;
            }
            else
            {
                int errorcode = UNKNOWN_CMD;
                onCmdReturn(uuid,tmp,errorcode);
                return;
            }
        }
    }
}


//流量
int mqttMsgParaseThread::onCmdFeed(string cmd_value)
{
    QString strVec = QString().fromStdString(cmd_value);
    QStringList vecList = strVec.split(":");

    //控制字段不等于上位机配置振动器数目
    if (vecList.size()!=struCnfg.nChuteTotal)
    {
        return CMD_PARA_ERROR;
    }

    for(int i = 0;i < vecList.size(); i++)
    {
        struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[i] = vecList.at(i).toInt();
    }

    myFlow.resetFeederRG();
    return SUCCESS;
}

//供料子开关
int mqttMsgParaseThread::onOpenSwitchs(string cmd_value)
{
    QString strVec = QString().fromStdString(cmd_value);
    QStringList vecList = strVec.split(":");

    //控制字段不等于上位机配置振动器数目
    if (vecList.size()!=struCnfg.nChuteTotal)
    {
        return CMD_PARA_ERROR;
    }

    for(int i = 0;i < vecList.size(); i++)
    {
        if (vecList.at(i).toInt() == 0)
            struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[i] = 0;
    }

    myFlow.resetFeederRG();
    return SUCCESS;

}
//供料总开关
int mqttMsgParaseThread::onOpenSwitch(string cmd_value)
{
    /* 气压报警不允许打开下料 */
    if(struGsh.bAlarmStatus == 1)
    {
        return ALARM_PRESSUER;
    }

    int cmd = atoi(cmd_value.c_str());

    if (cmd == struGsh.bSortStart)
    {
        return NONE_EXECUTE; //no need change
    }

    if(struCnfg.nUseIpcEnable == 1)
    {
        if(struIpcShare.allIpcModelLoadOk != 2 )
        {
            return IPC_CONNECT_FAIL;
        }
    }

    myFlow.onOff();

    myFlow.resetNetWaveStart();

    return SUCCESS;
}

int mqttMsgParaseThread::onParaUpload(QString cmd_value)
{
    return SUCCESS;
}

//关机
void mqttMsgParaseThread::onShutDown()
{
    if(struGsh.bSortStart == 1)
    {
        myFlow.onOff();
    }
#ifndef Q_OS_WIN
    struGsh.bFlagPowerOff = 1;
    system("shutdown -h now");
#endif
}

//全局精度
int mqttMsgParaseThread::onGlobalParaSet(string cmd_value)
{
    //"{\"sortSerial\":0,\"viewData\":[{\"view\":0,\"algs\":[{\"id\":0,\"value\":25,\"state\":1},{\"id\":3,\"value\":120,\"state\":1},{\"id\":6,\"value\":100,\"state\":1}]},{\"view\":1,\"algs\":[{\"id\":1,\"value\":27,\"state\":1},{\"id\":4,\"value\":85,\"state\":1},{\"id\":7,\"value\":100,\"state\":1},{\"id\":11,\"value\":0,\"state\":0}]},{\"view\":2,\"algs\":[{\"id\":0,\"value\":0,\"state\":0}]}]}"

    Json::Reader reader;
    Json::Value root;

    int sortSerialTotal = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[0].nIdentifyGroupTotal;

    if (reader.parse(cmd_value, root)) // reader将Json字符串解析到root，root将包含Json里所有子元素
    {
        int sortSerial = root["sortSerial"].asInt();
        int viewDataSize = root["viewData"].size();
        for(int i =0;i<viewDataSize;i++){
            int view = root["viewData"][i]["view"].asInt();
            int algsSize = root["viewData"][i]["algs"].size();
            for(int j=0;j<algsSize;j++){
                int arithid = root["viewData"][i]["algs"][j]["id"].asInt();
                float value = root["viewData"][i]["algs"][j]["value"].asFloat();
                int state = root["viewData"][i]["algs"][j]["state"].asInt();
                /*  平台视角类型 1-前视，2-后视，3-辅视
                    我们每视的编号 0：前主，1：后主，2：前辅，3：后辅
                    <现默认平台视角的辅视为前辅>  */
                int viewindex =0;
                switch (view) {
                case 0:
                    viewindex =0;
                    break;
                case 1:
                    viewindex =1;
                    break;
                case 2://
                    viewindex =2;
                    break;
                case 3://
                    viewindex =3;
                    break;
                case 4://
                    viewindex =4;
                    break;
                case 5://
                    viewindex =5;
                    break;
                case 6://
                    viewindex =6;
                    break;
                case 7://
                    viewindex =7;
                    break;
                default:
                    break;
                }

                /*  上传时以算法中使能为准，下发时修改两个使能   */
                switch (arithid){
                case ARITH_GREY_A:
                case ARITH_GREY_B:
                case ARITH_GREY_C:
                case ARITH_GREY_D:

                    if(struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struGreyColor[arithid].nMode == 0)
                        struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struGreyColor[arithid].nSensLow = value;

                    else
                        struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struGreyColor[arithid].nSensUp = value;
                    struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struGreyColor[arithid].nArithEnable = state;
                    struCnfp.nGroupArithEnable[struGsh.nLayer][viewindex][sortSerial][arithid] = state;

                    if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT){//前后相同或整机相同时
                        if(struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struGreyColor[arithid].nMode == 0)
                            struCnfp.struGroupIdentify[struGsh.nLayer][viewindex+1][sortSerial].struGreyColor[arithid].nSensLow = value;
                        else
                            struCnfp.struGroupIdentify[struGsh.nLayer][viewindex+1][sortSerial].struGreyColor[arithid].nSensUp = value;
                        struCnfp.struGroupIdentify[struGsh.nLayer][viewindex+1][sortSerial].struGreyColor[arithid].nArithEnable = state;
                        struCnfp.nGroupArithEnable[struGsh.nLayer][viewindex+1][sortSerial][arithid] = state;
                    }
                    //                    if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_SECOND_ALIKE_FIRST){
                    //                        for(int = 0;i <sortSerialTotal;i++)
                    //                        {
                    //                            if(struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][i].struGreyColor[arithid].nMode == 0)
                    //                                struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][i].struGreyColor[arithid].nSensLow = value;
                    //                            else
                    //                                struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][i].struGreyColor[arithid].nSensUp = value;
                    //                            struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struGreyColor[arithid].nArithEnable = state;
                    //                            struCnfp.nGroupArithEnable[struGsh.nLayer][viewindex+1][sortSerial][arithid] = state;
                    //                        }
                    //                    }
                    break;

                case ARITH_DISCOLOR_A:
                case ARITH_DISCOLOR_B:
                case ARITH_CROSS:
                    if(struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struGreyColor[arithid].nMode == 0)
                        struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struGreyColor[arithid].nSensLow = (int)(value/100*(ARITH_COLOR_MAX_SENS-0));

                    else
                        struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struGreyColor[arithid].nSensUp = (int)(value/100*(ARITH_COLOR_MAX_SENS-0));;
                    struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struGreyColor[arithid].nArithEnable = state;
                    struCnfp.nGroupArithEnable[struGsh.nLayer][viewindex][sortSerial][arithid] = state;

                    struCnfp.nGroupArithEnable[struGsh.nLayer][view][sortSerial][ARITH_GREY_A+i];
                    if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT){//前后相同或整机相同时
                        if(struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struGreyColor[arithid].nMode == 0)
                            struCnfp.struGroupIdentify[struGsh.nLayer][viewindex+1][sortSerial].struGreyColor[arithid].nSensLow = (int)(value/100*(ARITH_COLOR_MAX_SENS-0));
                        else
                            struCnfp.struGroupIdentify[struGsh.nLayer][viewindex+1][sortSerial].struGreyColor[arithid].nSensUp = (int)(value/100*(ARITH_COLOR_MAX_SENS-0));
                        struCnfp.struGroupIdentify[struGsh.nLayer][viewindex+1][sortSerial].struGreyColor[arithid].nArithEnable = state;
                        struCnfp.nGroupArithEnable[struGsh.nLayer][viewindex+1][sortSerial][arithid] = state;
                    }
                    break;

                case ARITH_SVM_A:
                case ARITH_SVM_B:
                case ARITH_SVM_C:
                case ARITH_SVM_D:
                    struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struSvm[arithid-ARITH_SVM_A].nSens = value;
                    struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struSvm[arithid-ARITH_SVM_A].nArithEnable = state;
                    struCnfp.nGroupArithEnable[struGsh.nLayer][viewindex][sortSerial][arithid] = state;

                    if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT){//前后相同或整机相同时
                        struCnfp.struGroupIdentify[struGsh.nLayer][viewindex+1][sortSerial].struSvm[arithid-ARITH_SVM_A].nSens = value;
                        struCnfp.struGroupIdentify[struGsh.nLayer][viewindex+1][sortSerial].struSvm[arithid-ARITH_SVM_A].nArithEnable = state;
                        struCnfp.nGroupArithEnable[struGsh.nLayer][viewindex+1][sortSerial][arithid] = state;
                    }
                    break;

                case ARITH_BIG_SMALL:
                    if(struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struBigSmall[0].nMode ==0){
                        struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struBigSmall[0].nAreaLow = value;
                    }else{
                        struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struBigSmall[0].nAreaUp = value;
                    }
                    struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struBigSmall[0].nArithEnable = state;
                    struCnfp.nGroupArithEnable[struGsh.nLayer][viewindex][sortSerial][arithid] = state;

                    if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT){//前后相同或整机相同时
                        if(struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struBigSmall[0].nMode ==0){
                            struCnfp.struGroupIdentify[struGsh.nLayer][viewindex+1][sortSerial].struBigSmall[0].nAreaLow = value;
                        }else{
                            struCnfp.struGroupIdentify[struGsh.nLayer][viewindex+1][sortSerial].struBigSmall[0].nAreaUp = value;
                        }
                        struCnfp.struGroupIdentify[struGsh.nLayer][viewindex+1][sortSerial].struBigSmall[0].nArithEnable = state;
                        struCnfp.nGroupArithEnable[struGsh.nLayer][viewindex+1][sortSerial][arithid] = state;
                    }
                    break;

                case ARITH_LONG_SHORT:
                case ARITH_LONG_SHORT_B:
                    if(struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struLongShort[0].nLengthMode == 0){
                        struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struLongShort[0].nLengthLow = value;
                    }else{
                        struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struLongShort[0].nLengthUp = value;
                    }
                    struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struLongShort[0].nArithEnable = state;
                    struCnfp.nGroupArithEnable[struGsh.nLayer][viewindex][sortSerial][arithid] = state;
                    if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT){//前后相同或整机相同时
                        if(struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struLongShort[0].nLengthMode == 0){
                            struCnfp.struGroupIdentify[struGsh.nLayer][viewindex+1][sortSerial].struLongShort[0].nLengthLow = value;
                        }else{
                            struCnfp.struGroupIdentify[struGsh.nLayer][viewindex+1][sortSerial].struLongShort[0].nLengthUp = value;
                        }
                        struCnfp.struGroupIdentify[struGsh.nLayer][viewindex+1][sortSerial].struLongShort[0].nArithEnable = state;
                    }
                    break;

                case ARITH_CIRCLE_LONG:
                    if(struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struCircleLong[0].nMode == 0){
                        struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struCircleLong[0].nCiloLow = value;
                    }else{
                        struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struCircleLong[0].nCilosUp = value;
                    }

                    struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struCircleLong[0].nArithEnable = state;
                    struCnfp.nGroupArithEnable[struGsh.nLayer][viewindex][sortSerial][arithid] = state;
                    if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT){//前后相同或整机相同时
                        if(struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struCircleLong[0].nMode == 0){
                            struCnfp.struGroupIdentify[struGsh.nLayer][viewindex+1][sortSerial].struCircleLong[0].nCiloLow = value;
                        }else{
                            struCnfp.struGroupIdentify[struGsh.nLayer][viewindex+1][sortSerial].struCircleLong[0].nCilosUp = value;
                        }

                        struCnfp.struGroupIdentify[struGsh.nLayer][viewindex+1][sortSerial].struCircleLong[0].nArithEnable = state;
                        struCnfp.nGroupArithEnable[struGsh.nLayer][viewindex+1][sortSerial][arithid] = state;
                    }
                    break;
                case ARITH_ROUND:
                    if(struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struRound[0].nSelectMode == 0){
                        struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struRound[0].nSensUp = value;
                    }else{
                        struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struRound[0].nSensLow = value;
                    }

                    struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struRound[0].nArithEnable = state;
                    struCnfp.nGroupArithEnable[struGsh.nLayer][viewindex][sortSerial][arithid] = state;
                    if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT){//前后相同或整机相同时
                        if(struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struRound[0].nSelectMode == 0){
                            struCnfp.struGroupIdentify[struGsh.nLayer][viewindex+1][sortSerial].struRound[0].nSensUp = value;
                        }else{
                            struCnfp.struGroupIdentify[struGsh.nLayer][viewindex+1][sortSerial].struRound[0].nSensLow = value;
                        }

                        struCnfp.struGroupIdentify[struGsh.nLayer][viewindex+1][sortSerial].struRound[0].nArithEnable = state;
                        struCnfp.nGroupArithEnable[struGsh.nLayer][viewindex+1][sortSerial][arithid] = state;
                    }
                    break;
                case TEA_ARITH_COLOR:
                case TEA_ARITH_COLOR_2:
                case TEA_ARITH_COLOR_3:
                case TEA_ARITH_COLOR_4:
                case TEA_ARITH_COLOR_5:
                    struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struTeaColor[arithid-TEA_ARITH_COLOR].sense = value;
                    struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struTeaColor[arithid-TEA_ARITH_COLOR].nArithEnable = state;
                    struCnfp.nGroupArithEnable[struGsh.nLayer][viewindex][sortSerial][arithid] = state;
                    if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT){//前后相同或整机相同时
                        struCnfp.struGroupIdentify[struGsh.nLayer][viewindex+1][sortSerial].struTeaColor[arithid-TEA_ARITH_COLOR].sense = value;
                        struCnfp.struGroupIdentify[struGsh.nLayer][viewindex+1][sortSerial].struTeaColor[arithid-TEA_ARITH_COLOR].nArithEnable = state;
                        struCnfp.nGroupArithEnable[struGsh.nLayer][viewindex+1][sortSerial][arithid] = state;
                    }
                    break;

                 case ARITH_HSV:
                 case ARITH_HSV_B:
                 case ARITH_HSV_C:
                    struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struHsv[arithid-ARITH_HSV].nArithEnable = state;
                    struCnfp.nGroupArithEnable[struGsh.nLayer][viewindex][sortSerial][arithid] = state;
                    switch(struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struHsv[arithid-ARITH_HSV].nHsvColor)
                    {
                    case 0:
                        if(struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struHsv[arithid-ARITH_HSV].nHsvMode == 0){
                            struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struHsv[arithid-ARITH_HSV].nHsvSensLowH = value*10 ;
                        }else{
                           struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struHsv[arithid-ARITH_HSV].nHsvSensUpH= value*10 ;
                        }
                        break;
                    case 1:
                        if(struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struHsv[arithid-ARITH_HSV].nHsvMode == 0){
                            value = struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struHsv[arithid-ARITH_HSV].nHsvSensLowS = value;
                        }else{
                            value = struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struHsv[arithid-ARITH_HSV].nHsvSensUpS=value ;
                        }

                        break;
                    case 2:
                        if(struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struHsv[arithid-ARITH_HSV].nHsvMode == 0){
                            struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struHsv[arithid-ARITH_HSV].nHsvSensLowV=value ;
                        }else{
                            struCnfp.struGroupIdentify[struGsh.nLayer][viewindex][sortSerial].struHsv[arithid-ARITH_HSV].nHsvSensUpV=value ;
                        }
                        break;
                    }

                    if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT){//前后相同或整机相同时
                        struCnfp.struGroupIdentify[struGsh.nLayer][viewindex+1][sortSerial].struHsv[arithid-ARITH_HSV].nArithEnable = state;
                        struCnfp.nGroupArithEnable[struGsh.nLayer][viewindex+1][sortSerial][arithid] = state;
                        switch(struCnfp.struGroupIdentify[struGsh.nLayer][viewindex+1][sortSerial].struHsv[arithid-ARITH_HSV].nHsvColor)
                        {
                        case 0:
                            if(struCnfp.struGroupIdentify[struGsh.nLayer][viewindex+1][sortSerial].struHsv[arithid-ARITH_HSV].nHsvMode == 0){
                                struCnfp.struGroupIdentify[struGsh.nLayer][viewindex+1][sortSerial].struHsv[arithid-ARITH_HSV].nHsvSensLowH = value*10 ;
                            }else{
                               struCnfp.struGroupIdentify[struGsh.nLayer][viewindex+1][sortSerial].struHsv[arithid-ARITH_HSV].nHsvSensUpH= value*10 ;
                            }

                            break;
                        case 1:
                            if(struCnfp.struGroupIdentify[struGsh.nLayer][viewindex+1][sortSerial].struHsv[arithid-ARITH_HSV].nHsvMode == 0){
                                value = struCnfp.struGroupIdentify[struGsh.nLayer][viewindex+1][sortSerial].struHsv[arithid-ARITH_HSV].nHsvSensLowS = value;
                            }else{
                                value = struCnfp.struGroupIdentify[struGsh.nLayer][viewindex+1][sortSerial].struHsv[arithid-ARITH_HSV].nHsvSensUpS=value ;
                            }

                            break;
                        case 2:
                            if(struCnfp.struGroupIdentify[struGsh.nLayer][viewindex+1][sortSerial].struHsv[arithid-ARITH_HSV].nHsvMode == 0){
                                struCnfp.struGroupIdentify[struGsh.nLayer][viewindex+1][sortSerial].struHsv[arithid-ARITH_HSV].nHsvSensLowV=value ;
                            }else{
                                struCnfp.struGroupIdentify[struGsh.nLayer][viewindex+1][sortSerial].struHsv[arithid-ARITH_HSV].nHsvSensUpV=value ;
                            }
                            break;
                        }
                    }

                    break;

                default:
                    break;
                }



                myFlow.materialParamsCopySendAssemble(struGsh.nLayer, viewindex, sortSerial, false, arithid, 0, true);

                myFlow.materialArithEnableCopy(struGsh.nLayer, viewindex, sortSerial, arithid, true); //根据参数设置方式，算法使能
                myFlow.materialParamsCopySendAssemble(struGsh.nLayer, viewindex, sortSerial, false, arithid, 0, true);
            }
        }

    }
    return SUCCESS;

}

//切换方案
int mqttMsgParaseThread::onMemSet(string cmd_value,string uuid)
{
    QString strVec = QString().fromStdString(cmd_value);
    QStringList vecList = strVec.split(":");

    if (vecList.size() != 2)
    {
        return CMD_PARA_ERROR;
    }
    int num = vecList.at(0).toInt();

    myFlow.saveProfile();
    struCnfg.nProfile = num;
    myFlow.getProfile();

    /* 发送控制板参数 */
    myFlow.resetControl();
    /* 发送色选参数 */
    myFlow.resetSortParams();
    /* 关闭下位机色选 */
    myFlow.resetSortOnOff(0, 0);	//turn off sort
    myFlow.configIpcSortStart(0);

    /* 关闭下位机喷阀 */
    myFlow.vavleOnOff();

    return SUCCESS;

}

//保存参数
int mqttMsgParaseThread::onSaveCfg()
{
    g_Runtime().save();

    return SUCCESS;
}

//远程推送VPN证书
int mqttMsgParaseThread::onVPNPushCert(string cmd_value)
{
    Json::Reader reader;
    Json::Value root;


    if (reader.parse(cmd_value, root)) // reader将Json字符串解析到root，root将包含Json里所有子元素
    {
        Json::Value root_cert= root["client_config"];
        string s_cert =root_cert.asString();

        QString str = QString().fromStdString(s_cert).replace("/\n/g", "\\\\n");

        QFile myfile("/sdcard/cfg/zk.ovpn");
        if (!myfile.open(QIODevice::WriteOnly | QIODevice::Text))
            return SYSTEM_ERROR;

        myfile.write(str.toLocal8Bit(),str.length());
        system("sync");
        myfile.close();
        return SUCCESS;
    }
    return CMD_PARA_ERROR;
}

//远程VPN连接开关
int mqttMsgParaseThread::onVPNConnect(string cmd_value)
{

    int maxTimes = 10;
    bool ok;
    int cmd = atoi(cmd_value.c_str());


    if (cmd)
    {
        if(g_Runtime().GetIpAddress("tun0") == "")
        {
            char cmd[64];
            sprintf(cmd, "cd /app;./openvpn --config /sdcard/cfg/zk.ovpn &");
            system(cmd);

            for(int i=0; i<maxTimes; i++)
            {
                QList<QNetworkInterface> interfaceList = QNetworkInterface::allInterfaces();

                foreach (QNetworkInterface interface, interfaceList)
                {
                    if(interface.name() == QString("tun0"))
                    {
                        QList<QNetworkAddressEntry> hostAddressList = interface.addressEntries();
                        foreach (QNetworkAddressEntry address, hostAddressList)
                        {
                            if(address.ip().protocol() == QAbstractSocket::IPv4Protocol)
                            {
                                myMqttThread->m_connect_IP = address.ip().toString();
                                qDebug()<<myMqttThread->m_connect_IP;
                            }
                        }
                        return SUCCESS;
                    }
                }
                myFlow.sleep(1);
            }
            return M_TIMEOUT;
        }
        return SUCCESS;
    }
    else
    {
        char cmd[64];
        sprintf(cmd, "killall openvpn");
        system(cmd);
        return SUCCESS;
    }
}

//所有通道采图
int mqttMsgParaseThread::onCaptureAllImage()
{
    int m =0, n = 0;
    for(int i = 0;i< struCnfg.struLayerInfo[0].nViewChainTotal;i++){
        if(struCnfg.struLayerInfo[0].nViewIsUsed[i]){
            for(int j = 0;j<struCnfg.nChuteTotal;j++){
                m++;
                struGsh.nRemoteImageCaptureIsRun = 1;
                myFlow.msleep(100); //确保报警查询停掉
                myFlow.myNetWork->setUdpRecvType(CAPTURE_MULTI_IMAGE,0,i,j);
                myFlow.myNetWork->udpStart();
                if(myFlow.myNetWork->udpthread->wait(30000))
                {
                    msleep(100);//收到采图正常结束信号
                    if (m_uploadStat)
                    {
                        n++;
                        uploadCaptureImage();
                    }
                }
            }
        }
    }

    struGsh.nRemoteImageCaptureIsRun = 0;
    if( n != m ) return IMG_NOT_CORRECT;
    return SUCCESS;

}
int mqttMsgParaseThread::onCaptureAllWave()
{

    struGsh.nRemoteCaptureAllWave = 1;
    int m =0, n = 0;
    for(int i = 0;i< struCnfg.struLayerInfo[0].nViewChainTotal;i++){
        if(struCnfg.struLayerInfo[0].nViewIsUsed[i]){
            for(int j = 0;j<struCnfg.nChuteTotal;j++){
                m++;
                struGsh.nRemoteImageCaptureIsRun = 1;
                myFlow.msleep(100); //
                qDebug()<<i<<j;
                myFlow.myNetWork->setUdpRecvType(CAPTURE_SINGLE_WAVE,0,i,j);
                myFlow.myNetWork->udpStart();
                myFlow.msleep(10); //
                if(myFlow.myNetWork->udpthread->wait(30000))
                {
                    msleep(100);//收到采图正常结束信号
                    if (n_uploadWaveFlag)
                    {
                        n++;
                        uploadCaptureWave(0,i,j);
                    }
                }
            }
        }
    }
    struGsh.nRemoteCaptureAllWave = 0;
    struGsh.nRemoteImageCaptureIsRun = 0;
    if( n != m ) return IMG_NOT_CORRECT;
    return SUCCESS;
}
//采图、波形  (层：视：通道)
int mqttMsgParaseThread::onCaptureImage(string cmd_value,string tmp)
{

    logger()->info("recv CMD %1",QString::fromStdString(tmp));
    if(myFlow.myNetWork->udpthread->isrunning)
        return SYSTEM_ERROR;

    unsigned int layer = 0,view = 0,unit = 0,type =0;

    QString strVec = QString().fromStdString(cmd_value);
    QStringList vecList = strVec.split(":");

    m_value = cmd_value;
    layer = vecList.at(0).toInt();
    view = vecList.at(1).toInt();
    unit = vecList.at(2).toInt();
    type = vecList.at(3).toInt();
    if (layer > MAX_LAYER  || view > MAX_VIEW || unit > (struCnfg.nChuteTotal-1) \
            || !struCnfg.struLayerInfo[layer].nViewIsUsed[view])
    {
        return CMD_PARA_ERROR;
    }

    struGsh.nRemoteImageCaptureIsRun = 1;

    int time = 2*1000;
    if (tmp == string("CMD_CaptureImage")){
        myFlow.myNetWork->setUdpRecvType(CAPTURE_MULTI_IMAGE,layer,view,unit);
        time = 30*1000;
    }
    else{
        if (type == 0)
            myFlow.myNetWork->setUdpRecvType(CAPTURE_SINGLE_WAVE,layer,view,unit);
        else
            myFlow.myNetWork->setUdpRecvType(CAPTURE_BACKGROUND_WAVE,layer,view,unit);
    }

    int ret;
    myFlow.myNetWork->udpStart();

    msleep(10);
    if(myFlow.myNetWork->udpthread->wait(time))
    {
        msleep(100);//收到采图正常结束信号
        if (n_uploadWaveFlag == 1){
            ret = uploadCaptureWave();
            return ret;
        }
        if (n_uploadImageFlag == 1){
            ret = uploadCaptureImage();
            return ret;
        }
    }
    logger()->info("Capture Image Error: Timeout");
    return M_TIMEOUT;
}

//上位机远程升级
void mqttMsgParaseThread::onUpdateZKSort(string cmd_value)
{

    n_downLoadFlag = 0;

    QString strVec = QString().fromStdString(cmd_value);
    QStringList vecList = strVec.split(QRegExp("::"));

    if (vecList.size() < 4 || !vecList.at(0).contains("http"))
    {
        onCmdReturn(m_uuid,m_tmp,CMD_PARA_ERROR);
        return;
    }
    UpdateUnit unit;
    unit.m_cmd = m_tmp;
    unit.m_uuid = m_uuid;

    unit.url = vecList.at(0);
    unit.md5 = vecList.at(1);
    unit.ver = vecList.at(2);
    unit.force = vecList.at(3).toInt();

    if(m_tmp == "CMD_UpdateZKSort"){
        if(g_Runtime().getFileMd5("/media/mmcblk0p1/3-app.bin") == unit.md5){
            onCmdReturn(m_uuid,m_tmp,VERSIONSAME);
        }
        m_updateFileType = UPDATE_ZKSORT;
    }
    else if (m_tmp == "CMD_UpdateIpcModel"){
        m_updateFileType = UPDATE_IPCMODEL;
    }
    QVariant m_unit = QVariant::fromValue(unit);
    emit cmd_updatezksort_ask(m_unit);
}

//执行远程shell命令
void mqttMsgParaseThread::onShellCmd(string cmd_value)
{
    if(g_Runtime().mySystem(QString::fromStdString(cmd_value))){
        onCmdReturn(m_uuid,m_tmp,SUCCESS);
    }
    else{
        onCmdReturn(m_uuid,m_tmp,CMD_PARA_ERROR);
    }
}
//日志上传
int mqttMsgParaseThread::onLogfileUpload(string cmd_value)
{
    QString date = QString().fromStdString(cmd_value);
    QStringList vecList = date.split("-");

    if (vecList.at(0).toInt()>2050 || vecList.at(1).toInt() >13|| vecList.at(2).toInt()>31)
    {
        return CMD_PARA_ERROR;
    }

    int ret = uploadLogFile(cmd_value);

    return ret;
}

//上传所有参数文件
int mqttMsgParaseThread::onCnffileUpload()
{
    QString date = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss");
    QString tarCmd = "cd /sdcard/ ;tar czvf cnf"+date+".tgz"+" cnf/";
    g_Runtime().mySystem(tarCmd);
    QString cnffilePath = "/sdcard/cnf"+date+".tgz";
    QFileInfo file(cnffilePath);

    if(file.exists() && file.size() >0)
    {
        QMap<int,QString> m_machineTypeMap;

        m_machineTypeMap.clear();
        m_machineTypeMap.insert(MACHINE_VERTICAL_ZL_CV,"ZL_LS");
        m_machineTypeMap.insert(MACHINE_LD_ZL_GP,"ZL_LD");
        m_machineTypeMap.insert(MACHINE_LD2_ZL_GP,"ZL_LDS");

        QString model = m_machineTypeMap.value(struCnfg.nMachineType);

        Json::Value root;
        Json::FastWriter info;
        root["code"] = myMqttThread->m_connect_name;
        root["fileName"] = QString(QFileInfo(cnffilePath).fileName()).toStdString();
        root["uploadType"] = "RUN_PARAM";//上传类型：DEVICE_MATERIAL_IMG,DEVICE_LIGHT_IMG,DEVICE_GLASS_IMG,OPERATE_LOG,RUN_PARAM
        root["reportFlag"] = reportFlag;
        std::string str = info.write(root);

        if (myHttpFileClient->requestOssUploadSign(URL_GETOSSUPLOADSIGN,str))
        {
            if(myHttpFileClient->upLoadOssFile(cnffilePath))
            {
                /* 回报上传结果给中科，主要就是存在阿里云上的路径 */
                root.clear();
                root["code"] = myMqttThread->m_connect_name;
                root["model"] = QString(model).toStdString();
                root["name"] = QString(QFileInfo(cnffilePath).fileName()).toStdString();
                root["url"] = QString(myHttpFileClient->fileUrl).toStdString();
                root["fileSize"] = QString(g_Runtime().getFileSize(cnffilePath)).toStdString();
                root["md5"] = QString(g_Runtime().getFileMd5(cnffilePath)).toStdString();
                QString dateText = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
                root["logDate"] = QString(dateText).toStdString();
                if(reportFlag == Request_Send)
                    root["cmdUuid"] = m_uuid;
                str = info.write(root);

                if(myHttpFileClient->upLoadData(URL_CNFFILE_UPLOAD,str)){
                    g_Runtime().mySystem("rm "+cnffilePath);
                    return SUCCESS;
                }
                return UPLOAD_ERR;
            }
            return OSS_UPLOAD_ERR;
        }
        return OSS_REQUESRT_ERR;
    }
    return NOT_EXIST;
}

/* 清灰速度上传 */
int mqttMsgParaseThread::onWipeSpeedUpload()
{
    n_uploadWipeSpeedFlag = 0;
    if ( myHttpFileClient->g_connectName == "")
    {
        return -1;
    }
    Json::Value root;
    Json::Value value;
    Json::Value arrayObj;
    Json::FastWriter info;

    root["code"] = myMqttThread->m_connect_name;

    for(int i =0; i< struCnfg.nChuteTotal;i++){
        value["id"] = i;
        value["value"] = struGsh.struCameraStatus[0][0][i].nWipeSpeed;
        value["valueBackward"] = struGsh.struCameraStatus[0][0][i].nWipeSpeed_backward;
        value["valueForward"] = struGsh.struCameraStatus[0][0][i].nWipeSpeed_forward;
        arrayObj.append(value);
    }
    //以前主1为准
    root["valuePre"] = struGsh.struCameraStatus[0][0][0].nWipeSpeed_pre;
    root["valueTotal"] = struGsh.struCameraStatus[0][0][0].nWipeSpeed_total;

    root["wipeSpeed"] = arrayObj;
    root["wipeDuration"]= struCnfp.struGroupCtrl[struGsh.nLayer].nWipeDuration;
    root["wipeInterval"]= struCnfp.struGroupCtrl[struGsh.nLayer].nWipeInterval;
    root["wipeCost"] = struGsh.struCameraStatus[0][0][0].nWipeSpeed_pre+struGsh.struCameraStatus[0][0][0].nWipeSpeed_total;
    if(reportFlag == Request_Send)
        root["cmdUuid"] = m_uuid;
    QString dateText = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    root["reportTime"] = QString(dateText).toStdString();
    root["reportFlag"] = reportFlag;

    std::string str = info.write(root);

    //    qDebug()<<QString::fromStdString(str);
    if (!struGsh.bFlagMqttConnect)
    {
        root["reportFlag"] = reportFlag+2;
        str = info.write(root);
        cacheLocalRemoteInfo(URL_WIPESPEED_UPLOAD,str);
        return NETWORK_ERR;
    }
    if (!myHttpFileClient->upLoadData(URL_WIPESPEED_UPLOAD,str))
    {
        root["reportFlag"] = reportFlag+2;
        str = info.write(root);
        cacheLocalRemoteInfo(URL_WIPESPEED_UPLOAD,str);
        return UPLOAD_ERR;
    }
    return SUCCESS;
}

/* 吹气次数上传 */
int mqttMsgParaseThread::onBlowCountsUpload()
{
    if(myFlow.myNetWork->udpthread->isrunning)
        return SYSTEM_ERROR;

    struGsh.nRemoteImageCaptureIsRun = 1;

    int time = 8;

    myFlow.myNetWork->setUdpRecvType(CAPTURE_BLOW_COUNTS);
    myFlow.myNetWork->udpStart();

    for (int i = 0;i < time; i++){

        sleep(1);
        if (n_uploadBlowCountsFlag == 1){
            int ret = uploadBlowCounts();
            return ret;
        }
    }
    return M_TIMEOUT;
}

/* 喷阀自检音频上传 */
int mqttMsgParaseThread::onVavleAudioUpload()
{
    n_uploadValveAudioFlag = 0;
    if (!struGsh.bFlagMqttConnect || myHttpFileClient->g_connectName =="") return NETWORK_ERR;
    QDir dir("/sdcard/ts");
    QStringList filter;
    filter<<"*.wav";
    dir.setNameFilters(filter);
    //按时间排序
    QFileInfoList fileInfo = dir.entryInfoList(filter,QDir::AllEntries,QDir::Name);

    if (fileInfo.size() == 0){
        return NOT_EXIST;
    }
    if(fileInfo.size() >= 3){
        QFile::remove(fileInfo.at(0).absoluteFilePath());
    }
    Json::Value root;
    Json::FastWriter info;
    root["code"] = myMqttThread->m_connect_name;
    root["fileName"] =  QString(fileInfo.last().fileName()).toStdString();
    root["uploadType"] = "DEVICE_AUDIO_VALVE";
    root["reportFlag"] = reportFlag;

    std::string str = info.write(root);

    if (myHttpFileClient->requestOssUploadSign(URL_GETOSSUPLOADSIGN,str))
    {
        if(myHttpFileClient->upLoadOssFile(fileInfo.last().absoluteFilePath()))
        {
            /* 回报上传结果给中科，主要就是图片存在阿里云上的路径 */
            root.clear();
            root["code"] = myMqttThread->m_connect_name;
            root["name"] = QString(fileInfo.last().fileName()).toStdString();
            root["url"] = QString(myHttpFileClient->fileUrl).toStdString();

            QString dateText = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            root["reportTime"] = QString(dateText).toStdString();

            str = info.write(root);

            if(myHttpFileClient->upLoadData(URL_VALVEAUDIO_UPLOAD,str))
            {
                g_Runtime().mySystem("rm "+fileInfo.last().absoluteFilePath());
                return SUCCESS;
            }
            return UPLOAD_ERR;
        }
        return OSS_UPLOAD_ERR;
    }
    return OSS_REQUESRT_ERR;
}

//执行结果上报
void mqttMsgParaseThread::onCmdReturn(string uuid,string cmd,int errorcode)
{
    Json::Value root;
    Json::Value  fields;
    Json::Value arrayObj;
    Json::FastWriter info;
    CMyJson json;

    qDebug()<<"onCmdReturn errcode"<<errorcode<<"\n";
    root["uuid"] = uuid;
    root["devId"] = myMqttThread->m_connect_name;
    root["cmdCode"] = cmd;
    root["result"] = QString().number(errorcode).toStdString();
    root["type"] = "push";

    if(cmd == "CMD_ConnectVPN")
    {
        root["remark"] = QString(myMqttThread->m_connect_IP).toStdString();
    }
    else if(cmd == "CMD_OfflineDetection" && resultinfo.size() >0 )
    {
        root["remark"] = (resultinfo);
    }
    std::string str = info.write(root);
    int rc = myMqttThread->test->publish(NULL,"MqttServerClient",str.length(),str.c_str(),2);
    //        theApp.amdLog->Info("mqtt指令返回，uuid："+uuid+"\tcmd:"+cmd+"\terrorcode:"+to_string(errorcode));
}

void mqttMsgParaseThread::doStateChange(QString tmp)
{

}
void mqttMsgParaseThread::uploadMemsetReturn()//上传切换成功
{

}
int mqttMsgParaseThread::onAllEjtTest(QString cmd_value)
{

}
int mqttMsgParaseThread::onSglEjtTest(QString cmd_value)
{

}
int mqttMsgParaseThread::onReSetEjt(QString cmd_value)
{

}
int mqttMsgParaseThread::onEjtTestMode(QString cmd_value)
{

}
int mqttMsgParaseThread::onRemotecontrol(int cmd_value,int time, QString phonenum)
{
    int errorcode = 0;
    int cmd = cmd_value;

    basewidget* widget = g_MainManager().GetCurWidget();
    int curPageId = widget->m_CurrentPageId;

    if(cmd == 1) // 开启远程控制
    {
        if(time < 0)  // 控制时间为0 报错
        {
            struGsh.remoteControlstatus =  CMD_PARA_ERROR;
        }
        else if( curPageId == SM_REMOTE_CONTROL_WIDGET && ( phonenum != userphonenum) ) // 当前已经开启远程控制 屏被占用
        {
            struGsh.remoteControlstatus = OCCUPY_ERROR;
        }
        else
        {
            struGsh.remoControlmode = 2; // 等待确认
            struGsh.remoControltime = time;
            userphonenum = phonenum;
            uploadPara();
            onModelParamsUpload();
            emit StartRemoteControl(phonenum); // 开始控制
        }
    }
    else if(cmd == 0) // 断开远程控制
    {
        if(struGsh.remoControlmode == 0) // 当前已经断开
        {
            struGsh.remoteControlstatus = 0;
        }
        else
        {
            emit EndRemoteControl();
        }
    }
    myFlow.msleep(100);
}
//老接口，上传系统状态信息，包含精度和延时信息
void mqttMsgParaseThread::uploadPara()
{
    Json::FastWriter info;
    Json::Value  root;
    Json::Value  flowBatch;
    Json::Value  switchs;
    Json::Value  precisions;
    Json::Value  precisionParams;
    Json::Value  viewParam;
    Json::Value  fields;
    Json::Value  delays;
    Json::Value  delayParams;
    Json::Value arrayObj;
    Json::Value array2Obj;
    Json::Value array3Obj;

    stu_group_identify senseIdentify;
    QString strSenseName;
    QMap<int,string> m_machineTypeMap;
    int used = 0;
    int value =50;
    int  max = 100;
    int min = 0;

    float f_value = 50.0;
    float f_max = 100.0;
    float f_min = 0.0;
    float temp;
    int index = 0;

    int viewId = 0;

    m_machineTypeMap.clear();
    m_machineTypeMap.insert(MACHINE_VERTICAL_ZL_CV,"ZL_LS");
    m_machineTypeMap.insert(MACHINE_LD_ZL_GP,"ZL_LD");
    m_machineTypeMap.insert(MACHINE_LD2_ZL_GP,"ZL_LDS");


    //	定义全局分组次数，目前为：主视前视，主视后视，辅视前视，辅视后视
    root["code"] = myMqttThread->m_connect_name;
    root["globalSwitch"] = struGsh.bSortStart;

    root["model"] = m_machineTypeMap.value(struCnfg.nMachineType);
    root["scheme"] = struCnfg.stuProfileIndex[struCnfg.nProfile].sMaterialName;
    root["schemeId"] = struCnfg.nProfile;
    root["version"] = m_version;
    root["batchForOutput"] =  (struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeedSetType == 0) ? false:true;
    root["batchForPrecision"] = (struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[0].nIdentifyGroupTotal == 0) ? false:true;

    //产量分次
    if (struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeedSetType )
    {
        for(int i=0;i<struCnfg.nLayerTotal;i++)
        {
            for(int j = 0;j < struCnfg.struLayerInfo[i].stuVavleInfo[0].nTickGroupTotal;j++){
                flowBatch["sortSerial"] = j;
                stru_vavle_group_info vavle_group = struCnfg.struLayerInfo[i].stuVavleInfo[0].stuTickGupInfo[j];

                for(int k = 0;k < vavle_group.nEjBoardGupTotal;k++){
                    int feedid = vavle_group.nEjBoardGupId[k];
                    switchs["flow"] = struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedid];
                    switchs["id"] = feedid;
                    if (struGsh.bSortStart == 1)
                        switchs["state"] = ( struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedid] == 0)? false:true;
                    else
                        switchs["state"] = false;

                    arrayObj.append(switchs);

                }
                flowBatch["switchs"] = arrayObj;
                arrayObj.clear();
                array2Obj.append(flowBatch);

            }
        }
    }
    else //产量按通道
    {
        for(int i=0;i<struCnfg.nLayerTotal;i++)
        {
            for(int j = 0;j < struCnfg.nChuteTotal;j++){
                flowBatch["sortSerial"] = j;
                switchs["flow"] =  struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[j];
                switchs["id"] = j;
                if (struGsh.bSortStart == 1)
                    switchs["state"] = (  struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[j]== 0)? false:true;
                else
                    switchs["state"] = false;

                arrayObj.append(switchs);
            }
            flowBatch["switchs"] = arrayObj;
            array2Obj.append(flowBatch);
        }
    }
    root["flowBatch"] = array2Obj;

    arrayObj.clear();
    array2Obj.clear();


    //全局精度
    for(int i=0;i<struCnfg.nLayerTotal;i++)
    {
        for(int j=0;j<struCnfg.struLayerInfo[i].stuViewInfo[0].nIdentifyGroupTotal;j++)
        {
            precisions["enable"] = true;
            precisions["sortSerial"] = j;

            for(int k=0;k<MAX_VIEW;k++)
            {
                if (struCnfg.struLayerInfo[i].nViewIsUsed[k])
                {
                    memcpy(&senseIdentify,&struCnfp.struGroupIdentify[i][k][j],sizeof(senseIdentify));

                    int viewId = struCnfg.struLayerInfo[i].nViewId[k];
                    precisionParams["enable"] = true;

                    switch (viewId){
                    case 0:
                        precisionParams["viewName"] = QString(g_myLan().ids_fm).toStdString();
                        precisionParams["view"] = 0;
                        break;
                    case 1:
                        precisionParams["viewName"] = QString(g_myLan().ids_bm).toStdString();
                        precisionParams["view"] = 1;
                        break;
                    case 2:
                        precisionParams["viewName"] = QString(g_myLan().ids_fs).toStdString();
                        precisionParams["view"] = 2;
                        break;
                    case 3:
                        precisionParams["viewName"] = QString(g_myLan().ids_bs).toStdString();
                        precisionParams["view"] = 3;
                        break;
                    case 4:
                        precisionParams["viewName"] = QString("II"+g_myLan().ids_fm).toStdString();
                        precisionParams["view"] = 4;
                        break;
                    case 5:
                        precisionParams["viewName"] = QString("II"+g_myLan().ids_bm).toStdString();
                        precisionParams["view"] = 5;
                        break;
                    case 6:
                        precisionParams["viewName"] = QString("II"+g_myLan().ids_fs).toStdString();
                        precisionParams["view"] = 6;
                        break;
                    case 7:
                        precisionParams["viewName"] = QString("II"+g_myLan().ids_bs).toStdString();
                        precisionParams["view"] = 7;
                        break;

                    default:
                        break;
                    }

                    for(int m = 0;m < MAX_GREY_COLOR_ARITH_TOTAL;m++){
                        if(senseIdentify.struGreyColor[m].nDispEnable){
                            switch (m) {
                            case ARITH_GREY_A:
                            case ARITH_GREY_B:
                            case ARITH_GREY_C:
                            case ARITH_GREY_D:

                                max = ARITH_GREY_MAX_SENS;
                                break;
                            case ARITH_DISCOLOR_A:
                            case ARITH_DISCOLOR_B:
                            case ARITH_CROSS:
                                max = ARITH_COLOR_MAX_SENS;
                                break;
                            default:
                                break;
                            }
                            if(senseIdentify.struGreyColor[m].nMode){
                                value = senseIdentify.struGreyColor[m].nSensUp;
                            }else{
                                value = senseIdentify.struGreyColor[m].nSensLow;
                            }

                            //界面1000个点需对应实际1024个值，存在精度丢失
                            temp = (float)value/(max-min)*100.0;
                            f_value = ((float)((int)((temp+0.05)*100)))/100;

                            temp = (float)max/(max-min)*100.0;
                            f_max = ((float)((int)(temp*100)))/100;

                            temp = (float)min/(max-min)*100.0;
                            f_min = ((float)((int)(temp*100)))/100;

                            //                            used = struCnfp.nGroupArithEnable[i][k][j][ARITH_GREY_A+m];
                            used = struCnfp.struGroupIdentify[i][k][j].struGreyColor[m].nArithEnable;

                            viewParam["id"] = m;
                            if(m ==ARITH_DISCOLOR_A || m ==ARITH_DISCOLOR_B || m ==ARITH_CROSS)
                            {
                                viewParam["maxValue"] = f_max;
                                viewParam["minValue"] = f_min;
                                viewParam["paraValue"] = f_value;
                                viewParam["step"] = 0.1;
                                //                                temp = 100.0/(max-min);
                                //                                viewParam["step"] = ((float)((int)(temp*100)))/100;
                            }
                            else
                            {
                                viewParam["maxValue"] = max;
                                viewParam["minValue"] = min;
                                viewParam["paraValue"] = value;
                                viewParam["step"] = 1;
                            }
                            QByteArray senseNameArr(senseIdentify.struGreyColor[m].sName);
                            strSenseName = QString::fromUtf8(senseNameArr);
                            viewParam["paraName"] = QString(strSenseName).toStdString();
                            viewParam["state"] = used; //使能：1-开启并显示，0-关闭并显示 -1-关闭且不显示
                            arrayObj.append(viewParam);
                        }
                    }

                    for(int m = 0;m < MAX_SVM_ARITH_TOTAL;m++){
                        if(senseIdentify.struSvm[m].nDispEnable){
                            index = ARITH_SVM_A+m;
                            min = 1;
                            max = 200;
                            value = senseIdentify.struSvm[m].nSens;
                            used = struCnfp.struGroupIdentify[i][k][j].struSvm[m].nArithEnable;
                            QByteArray senseNameArr(senseIdentify.struSvm[m].sName);
                            strSenseName = QString::fromUtf8(senseNameArr);

                            viewParam["id"] = index;
                            viewParam["maxValue"] = (float)max;
                            viewParam["minValue"] = (float)min;
                            viewParam["paraName"] = QString(strSenseName).toStdString();
                            viewParam["paraValue"] = (float)value;

                            viewParam["state"] = used; //使能：1-开启并显示，0-关闭并显示 -1-关闭且不显示
                            viewParam["step"] = 1;

                            arrayObj.append(viewParam);
                        }
                    }

                    for(int m = 0;m < MAX_SHAPE_SVM_ARITH_TOTAL;m++){
                        if(senseIdentify.struShapeSvm[m].nDispEnable){
                            index = ARITH_SHAPE_SVM_A+m;
                            min = 1;
                            max = 200;
                            value = senseIdentify.struShapeSvm[m].nSens;
                            used = struCnfp.struGroupIdentify[i][k][j].struShapeSvm[m].nArithEnable;
                            QByteArray senseNameArr(senseIdentify.struShapeSvm[m].sName);
                            strSenseName = QString::fromUtf8(senseNameArr);

                            viewParam["id"] = index;
                            viewParam["maxValue"] = (float)max;
                            viewParam["minValue"] = (float)min;
                            viewParam["paraName"] = QString(strSenseName).toStdString();
                            viewParam["paraValue"] = (float)value;

                            viewParam["state"] = used; //使能：1-开启并显示，0-关闭并显示 -1-关闭且不显示
                            viewParam["step"] = 1;

                            arrayObj.append(viewParam);
                        }
                    }

                    for(int m = 0;m < 2;m++){
                        if(senseIdentify.struBigSmall[m].nDispEnable){
                            index = ARITH_BIG_SMALL +m;
                            min = 0;
                            max = struCnfg.nBigSmallArithMaxValue;
                            if(senseIdentify.struBigSmall[m].nMode)
                                value = senseIdentify.struBigSmall[m].nAreaUp;
                            else
                                value = senseIdentify.struBigSmall[m].nAreaLow;
                            //                        used = struCnfp.nGroupArithEnable[i][k][j][ARITH_BIG_SMALL];
                            used = struCnfp.struGroupIdentify[i][k][j].struBigSmall[m].nArithEnable;

                            QByteArray senseNameArr(senseIdentify.struBigSmall[m].sName);
                            strSenseName = QString::fromUtf8(senseNameArr);

                            viewParam["id"] = index;
                            viewParam["maxValue"] = (float)max;
                            viewParam["minValue"] = (float)min;
                            viewParam["paraName"] = QString(strSenseName).toStdString();
                            viewParam["paraValue"] = (float)value;

                            viewParam["state"] = used; //使能：1-开启并显示，0-关闭并显示 -1-关闭且不显示
                            viewParam["step"] = 1;

                            arrayObj.append(viewParam);
                        }
                    }
                    for(int m = 0;m < 2;m++){
                        if(senseIdentify.struCircleLong[m].nDispEnable){
                            index = ARITH_CIRCLE_LONG+m;
                            min = 0;
                            max = ARITH_LONG_CIRCLE_MAX_VALUE;
                            if(senseIdentify.struCircleLong[m].nMode)
                                value = senseIdentify.struCircleLong[m].nCilosUp;
                            else
                                value = senseIdentify.struCircleLong[m].nCiloLow;
                            //                        used = struCnfp.nGroupArithEnable[i][k][j][ARITH_CIRCLE_LONG];

                            used = struCnfp.struGroupIdentify[i][k][j].struCircleLong[m].nArithEnable;
                            QByteArray senseNameArr(senseIdentify.struCircleLong[m].sName);
                            strSenseName = QString::fromUtf8(senseNameArr);

                            viewParam["id"] = index;
                            viewParam["maxValue"] = (float)max;
                            viewParam["minValue"] = (float)min;
                            viewParam["paraName"] = QString(strSenseName).toStdString();
                            viewParam["paraValue"] = (float)value;
                            viewParam["state"] = used; //使能：1-开启并显示，0-关闭并显示 -1-关闭且不显示
                            viewParam["step"] = 1;
                            arrayObj.append(viewParam);
                        }

                    }

                    for(int m = 0;m < 2;m++){
                        if(senseIdentify.struLongShort[m].nDispEnable){
                            index = ARITH_LONG_SHORT+m;
                            min = 0;
                            max = ARITH_LENGTH_MAX_VALUE;
                            if(senseIdentify.struLongShort[m].nLengthMode)
                                value = senseIdentify.struLongShort[m].nLengthUp;
                            else
                                value = senseIdentify.struLongShort[m].nLengthLow;

                            //                        used = struCnfp.nGroupArithEnable[i][k][j][ARITH_LONG_SHORT];
                            used = struCnfp.struGroupIdentify[i][k][j].struLongShort[m].nArithEnable;

                            QByteArray senseNameArr(senseIdentify.struLongShort[m].sName);
                            strSenseName = QString::fromUtf8(senseNameArr);
                            viewParam["id"] = index;
                            viewParam["maxValue"] = (float)max;
                            viewParam["minValue"] = (float)min;
                            viewParam["paraName"] = QString(strSenseName).toStdString();
                            viewParam["paraValue"] = (float)value;
                            viewParam["state"] = used; //使能：1-开启并显示，0-关闭并显示 -1-关闭且不显示
                            viewParam["step"] = 1;
                            arrayObj.append(viewParam);
                        }

                    }

                    for(int m = 0;m < 3;m++){
                        if(senseIdentify.struHsv[m].nDispEnable){
                            index = ARITH_HSV+m;
                            min = 0;
                            max = ARITH_SAT_MAX_SENSE;
                            used = struCnfp.struGroupIdentify[i][k][j].struHsv[m].nArithEnable;
                            QByteArray senseNameArr(senseIdentify.struHsv[m].sName);
                            strSenseName = QString::fromUtf8(senseNameArr);
                            switch(senseIdentify.struHsv[m].nHsvColor)
                            {
                            case 0:
                                if(senseIdentify.struHsv[m].nHsvMode == 0){
                                    f_value = senseIdentify.struHsv[m].nHsvSensLowH/10.0 ;
                                }else{
                                    f_value = senseIdentify.struHsv[m].nHsvSensUpH/10.0;
                                }
                                max = ARITH_HUE_MAX_SENSE;
                                viewParam["paraValue"] = (float)f_value;
                                break;
                            case 1:
                                if(senseIdentify.struHsv[m].nHsvMode == 0){
                                    value = senseIdentify.struHsv[m].nHsvSensLowS;
                                }else{
                                    value = senseIdentify.struHsv[m].nHsvSensUpS ;
                                }
                                viewParam["paraValue"] = (float)value;
                                break;
                            case 2:
                                if(senseIdentify.struHsv[m].nHsvMode == 0){
                                    value = senseIdentify.struHsv[m].nHsvSensLowV ;
                                }else{
                                    value = senseIdentify.struHsv[m].nHsvSensUpV ;
                                }
                                viewParam["paraValue"] = (float)value;
                                break;
                            }
                            viewParam["id"] = index;
                            viewParam["maxValue"] = (float)max;
                            viewParam["minValue"] = (float)min;
                            viewParam["paraName"] = QString(strSenseName).toStdString();
                            viewParam["state"] = used; //使能：1-开启并显示，0-关闭并显示 -1-关闭且不显示
                            viewParam["step"] = 1;
                            arrayObj.append(viewParam);
                        }

                    }

                    if(senseIdentify.struColorSat.nDispEnable){
                        index = ARITH_SAT;
                        min = 0;
                        max = 100;
                        if(senseIdentify.struColorSat.nMode)
                            value = senseIdentify.struColorSat.nSensUp;
                        else
                            value = senseIdentify.struColorSat.nSensLow;

                        used = struCnfp.struGroupIdentify[i][k][j].struColorSat.nArithEnable;
                        QByteArray senseNameArr(senseIdentify.struColorSat.sName);
                        strSenseName = QString::fromUtf8(senseNameArr);

                        viewParam["id"] = index;
                        viewParam["maxValue"] = (float)max;
                        viewParam["minValue"] = (float)min;
                        viewParam["paraName"] = QString(strSenseName).toStdString();
                        viewParam["paraValue"] = (float)value;
                        viewParam["state"] = used; //使能：1-开启并显示，0-关闭并显示 -1-关闭且不显示
                        viewParam["step"] = 1;
                        arrayObj.append(viewParam);
                    }

                    if(senseIdentify.struColorScale.nDispEnable){
                        index = ARITH_SCALE;
                        min = 0;
                        max = 100;
                        value = senseIdentify.struColorScale.nSen;
                        used = struCnfp.struGroupIdentify[i][k][j].struColorScale.nArithEnable;
                        QByteArray senseNameArr(senseIdentify.struColorScale.sName);
                        strSenseName = QString::fromUtf8(senseNameArr);
                        viewParam["id"] = index;
                        viewParam["maxValue"] = (float)max;
                        viewParam["minValue"] = (float)min;
                        viewParam["paraName"] = QString(strSenseName).toStdString();
                        viewParam["paraValue"] = (float)value;
                        viewParam["state"] = used; //使能：1-开启并显示，0-关闭并显示 -1-关闭且不显示
                        viewParam["step"] = 1;
                        arrayObj.append(viewParam);
                    }

                    if(senseIdentify.struColorHue.nDispEnable){
                        index = ARITH_HUE;
                        min = 0;
                        max = 100;
                        value = senseIdentify.struColorHue.nHighHue;
                        used = struCnfp.struGroupIdentify[i][k][j].struColorHue.nArithEnable;
                        QByteArray senseNameArr(senseIdentify.struColorHue.sName);
                        strSenseName = QString::fromUtf8(senseNameArr);
                        viewParam["id"] = index;
                        viewParam["maxValue"] = (float)max;
                        viewParam["minValue"] = (float)min;
                        viewParam["paraName"] = QString(strSenseName).toStdString();
                        viewParam["paraValue"] = (float)value;
                        viewParam["state"] = used; //使能：1-开启并显示，0-关闭并显示 -1-关闭且不显示
                        viewParam["step"] = 1;
                        arrayObj.append(viewParam);
                    }

                    if(senseIdentify.struColorValue.nDispEnable){
                        index = ARITH_VALUE;
                        min = 0;
                        max = 100;
                        if(senseIdentify.struColorValue.nMode)
                            value = senseIdentify.struColorValue.nSensUp;
                        else
                            value = senseIdentify.struColorValue.nSensLow;

                        used = struCnfp.struGroupIdentify[i][k][j].struColorValue.nArithEnable;
                        QByteArray senseNameArr(senseIdentify.struColorValue.sName);
                        strSenseName = QString::fromUtf8(senseNameArr);

                        viewParam["id"] = index;
                        viewParam["maxValue"] = (float)max;
                        viewParam["minValue"] = (float)min;
                        viewParam["paraName"] = QString(strSenseName).toStdString();
                        viewParam["paraValue"] = (float)value;
                        viewParam["state"] = used; //使能：1-开启并显示，0-关闭并显示 -1-关闭且不显示
                        viewParam["step"] = 1;
                        arrayObj.append(viewParam);
                    }

                    if(senseIdentify.struMildew.nDispEnable){
                        index = ARITH_MILDEW;
                        min = 0;
                        max = 100;

                        value = senseIdentify.struMildew.nSens;
                        used = struCnfp.struGroupIdentify[i][k][j].struMildew.nArithEnable;
                        QByteArray senseNameArr(senseIdentify.struMildew.sName);
                        strSenseName = QString::fromUtf8(senseNameArr);
                        viewParam["id"] = index;
                        viewParam["maxValue"] = (float)max;
                        viewParam["minValue"] = (float)min;
                        viewParam["paraName"] = QString(strSenseName).toStdString();
                        viewParam["paraValue"] = (float)value;
                        viewParam["state"] = used; //使能：1-开启并显示，0-关闭并显示 -1-关闭且不显示
                        viewParam["step"] = 1;
                        arrayObj.append(viewParam);
                    }

                    if(senseIdentify.struOutline.nDispEnable){
                        index = ARITH_OUTLINE;
                        min = 0;
                        max = 100;

                        value = senseIdentify.struOutline.nSens;
                        used = struCnfp.struGroupIdentify[i][k][j].struOutline.nArithEnable;
                        QByteArray senseNameArr(senseIdentify.struOutline.sName);
                        strSenseName = QString::fromUtf8(senseNameArr);
                        viewParam["id"] = index;
                        viewParam["maxValue"] = (float)max;
                        viewParam["minValue"] = (float)min;
                        viewParam["paraName"] = QString(strSenseName).toStdString();
                        viewParam["paraValue"] = (float)value;
                        viewParam["state"] = used; //使能：1-开启并显示，0-关闭并显示 -1-关闭且不显示
                        viewParam["step"] = 1;
                        arrayObj.append(viewParam);
                    }

                    if(senseIdentify.struLine.nDispEnable){
                        index = ARITH_LINE;
                        min = 0;
                        max = 100;

                        value = senseIdentify.struLine.nSens1;
                        used = struCnfp.struGroupIdentify[i][k][j].struLine.nArithEnable;
                        QByteArray senseNameArr(senseIdentify.struLine.sName);
                        strSenseName = QString::fromUtf8(senseNameArr);
                        viewParam["id"] = index;
                        viewParam["maxValue"] = (float)max;
                        viewParam["minValue"] = (float)min;
                        viewParam["paraName"] = QString(strSenseName).toStdString();
                        viewParam["paraValue"] = (float)value;
                        viewParam["state"] = used; //使能：1-开启并显示，0-关闭并显示 -1-关闭且不显示
                        viewParam["step"] = 1;
                        arrayObj.append(viewParam);
                    }

                    for(int m = 0;m < 2;m++){
                        if(senseIdentify.struBud[m].nDispEnable){
                            index = ARITH_BUD + m;
                            min = 0;
                            max = 100;

                            value = senseIdentify.struBud[m].nYellowSens;
                            used = struCnfp.struGroupIdentify[i][k][j].struBud[m].nArithEnable;
                            QByteArray senseNameArr(senseIdentify.struBud[m].sName);
                            strSenseName = QString::fromUtf8(senseNameArr);
                            viewParam["id"] = index;
                            viewParam["maxValue"] = (float)max;
                            viewParam["minValue"] = (float)min;
                            viewParam["paraName"] = QString(strSenseName).toStdString();
                            viewParam["paraValue"] = (float)value;
                            viewParam["state"] = used; //使能：1-开启并显示，0-关闭并显示 -1-关闭且不显示
                            viewParam["step"] = 1;
                            arrayObj.append(viewParam);
                        }
                    }

                    for(int m = 0;m < 2;m++){
                        if(senseIdentify.struRound[m].nDispEnable){
                            index = ARITH_ROUND + m;
                            min = 0;
                            max = 1023;
                            if(senseIdentify.struRound[m].nSelectMode == 0)
                                value = senseIdentify.struRound[m].nSensUp;
                            else if(senseIdentify.struRound[m].nSelectMode == 1)
                                value = senseIdentify.struRound[m].nSensLow;

                            used = struCnfp.struGroupIdentify[i][k][j].struRound[m].nArithEnable;
                            QByteArray senseNameArr(senseIdentify.struRound[m].sName);
                            strSenseName = QString::fromUtf8(senseNameArr);

                            viewParam["id"] = index;
                            viewParam["maxValue"] = (float)max;
                            viewParam["minValue"] = (float)min;
                            viewParam["paraName"] = QString(strSenseName).toStdString();
                            viewParam["paraValue"] = (float)value;
                            viewParam["state"] = used; //使能：1-开启并显示，0-关闭并显示 -1-关闭且不显示
                            viewParam["step"] = 1;
                            arrayObj.append(viewParam);
                        }
                    }


                    if(senseIdentify.struSplMildew.nDispEnable){
                        index = ARITH_SPL_MILDEW ;
                        min = 0;
                        max = 100;
                        value = senseIdentify.struSplMildew.nSens;
                        used = struCnfp.struGroupIdentify[i][k][j].struSplMildew.nArithEnable;
                        QByteArray senseNameArr(senseIdentify.struSplMildew.sName);
                        strSenseName = QString::fromUtf8(senseNameArr);

                        viewParam["id"] = index;
                        viewParam["maxValue"] = (float)max;
                        viewParam["minValue"] = (float)min;
                        viewParam["paraName"] = QString(strSenseName).toStdString();
                        viewParam["paraValue"] = (float)value;
                        viewParam["state"] = used; //使能：1-开启并显示，0-关闭并显示 -1-关闭且不显示
                        viewParam["step"] = 1;
                        arrayObj.append(viewParam);
                    }

                    if(senseIdentify.struWhitePeanut.nDispEnable){
                        index = ARITH_W_PEANUT ;
                        min = 0;
                        max = 100;
                        value = senseIdentify.struWhitePeanut.nSensLow;
                        used = struCnfp.struGroupIdentify[i][k][j].struWhitePeanut.nArithEnable;
                        QByteArray senseNameArr(senseIdentify.struWhitePeanut.sName);
                        strSenseName = QString::fromUtf8(senseNameArr);

                        viewParam["id"] = index;
                        viewParam["maxValue"] = (float)max;
                        viewParam["minValue"] = (float)min;
                        viewParam["paraName"] = QString(strSenseName).toStdString();
                        viewParam["paraValue"] = (float)value;
                        viewParam["state"] = used; //使能：1-开启并显示，0-关闭并显示 -1-关闭且不显示
                        viewParam["step"] = 1;
                        arrayObj.append(viewParam);
                    }

                    if(senseIdentify.struPepperHandle.nDispEnable){
                        index = ARITH_PEPPER_HANDLE ;
                        min = 0;
                        max = 100;
                        value = senseIdentify.struPepperHandle.nSens;
                        used = struCnfp.struGroupIdentify[i][k][j].struPepperHandle.nArithEnable;
                        QByteArray senseNameArr(senseIdentify.struPepperHandle.sName);
                        strSenseName = QString::fromUtf8(senseNameArr);

                        viewParam["id"] = index;
                        viewParam["maxValue"] = (float)max;
                        viewParam["minValue"] = (float)min;
                        viewParam["paraName"] = QString(strSenseName).toStdString();
                        viewParam["paraValue"] = (float)value;
                        viewParam["state"] = used; //使能：1-开启并显示，0-关闭并显示 -1-关闭且不显示
                        viewParam["step"] = 1;
                        arrayObj.append(viewParam);
                    }

                    for(int m = 0;m < 3;m++){

                        if(senseIdentify.struSuppressLarge[m].nDispEnable){
                            index = ARITH_SUPPRESS_LARGE_A+m ;
                            min = 0;
                            max = 100;
                            value = senseIdentify.struSuppressLarge[m].nSize1;
                            used = struCnfp.struGroupIdentify[i][k][j].struSuppressLarge[m].nArithEnable;
                            QByteArray senseNameArr(senseIdentify.struSuppressLarge[m].sName);
                            strSenseName = QString::fromUtf8(senseNameArr);

                            viewParam["id"] = index;
                            viewParam["maxValue"] = (float)max;
                            viewParam["minValue"] = (float)min;
                            viewParam["paraName"] = QString(strSenseName).toStdString();
                            viewParam["paraValue"] = (float)value;
                            viewParam["state"] = used; //使能：1-开启并显示，0-关闭并显示 -1-关闭且不显示
                            viewParam["step"] = 1;
                            arrayObj.append(viewParam);
                        }
                    }

                    if(senseIdentify.struWheatSprout.nDispEnable){
                        index = ARITH_PEPPER_HANDLE ;
                        min = 0;
                        max = 100;
                        value = senseIdentify.struWheatSprout.nSense;
                        used = struCnfp.struGroupIdentify[i][k][j].struWheatSprout.nArithEnable;
                        QByteArray senseNameArr(senseIdentify.struWheatSprout.sName);
                        strSenseName = QString::fromUtf8(senseNameArr);

                        viewParam["id"] = index;
                        viewParam["maxValue"] = (float)max;
                        viewParam["minValue"] = (float)min;
                        viewParam["paraName"] = QString(strSenseName).toStdString();
                        viewParam["paraValue"] = (float)value;
                        viewParam["state"] = used; //使能：1-开启并显示，0-关闭并显示 -1-关闭且不显示
                        viewParam["step"] = 1;
                        arrayObj.append(viewParam);
                    }


                    for(int m = 0;m < MAX_TEA_ARITH_TOTAL;m++){

                        if(senseIdentify.struTeaColor[m].nDispEnable){
                            index = TEA_ARITH_COLOR+m ;
                            min = 0;
                            max = 100;
                            f_value = senseIdentify.struTeaColor[m].sense;
                            used = struCnfp.struGroupIdentify[i][k][j].struTeaColor[m].nArithEnable;
                            QByteArray senseNameArr(senseIdentify.struTeaColor[m].sName);
                            strSenseName = QString::fromUtf8(senseNameArr);

                            viewParam["id"] = index;
                            viewParam["maxValue"] = (float)max;
                            viewParam["minValue"] = (float)min;
                            viewParam["paraName"] = QString(strSenseName).toStdString();
                            viewParam["paraValue"] = (float)f_value;
                            viewParam["state"] = used; //使能：1-开启并显示，0-关闭并显示 -1-关闭且不显示
                            viewParam["step"] = 1;
                            arrayObj.append(viewParam);
                        }
                    }
                    precisionParams["viewParam"] = arrayObj;
                    arrayObj.clear();
                    array2Obj.append(precisionParams);

                }//end used view

            }//end max view

            precisions["precisionParams"] = array2Obj;
            array2Obj.clear();
            array3Obj.append(precisions);

        }//end group

        root["precisions"] = array3Obj;

    }// end layer ps:目前后台没有层的结构


    if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT)//前后相同或整机相同时
        root["sameForView"] = true;
    else
        root["sameForView"] = false;

    arrayObj.clear();

    for( int i=0;i<struCnfg.nChuteTotal;i++)
    {
        value = struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[i];
        switchs["flow"] = value;
        switchs["id"] = i;
        if (struGsh.bSortStart == 1)
            switchs["state"] = (value == 0)? false: true;
        else
            switchs["state"] = false;

        arrayObj.append(switchs);
    }
    root["switchs"] = arrayObj;


    //添加延时信息 2020/8/26
    arrayObj.clear();
    array2Obj.clear();
    viewParam.clear();
    for(int i=0;i<struCnfg.nLayerTotal;i++)
    {
        for(int j=0;j<struCnfg.struLayerInfo[i].stuViewInfo[0].nIdentifyGroupTotal;j++)
        {
            delays["sortSerial"] = j;

            if (struCnfg.nVavleTimeSetMode == 1)
            {
                viewId = struCnfg.struLayerInfo[i].nViewId[0];
                viewParam["view"]      = viewId;
                viewParam["delaytime1"] = (float)(struCnfp.struGroupIdentify[i][0][j].nEjectDelay/10.0);
                viewParam["delaytime2"] = (float)(struCnfp.struGroupIdentify[i][0][j].nEjectDelayShape/10.0);
                viewParam["blowTime"]   = (float)(struCnfp.struGroupTick[i][0][j].nEjectDuration/10.0);
                viewParam["ejectType"] = struCnfp.struGroupTick[i][0][j].nEjwidth;


                viewParam["delaytimeModel"] = (float)(struCnfp.struGroupIdentify[i][0][j].nEjectDelayModel/10.0);
                viewParam["blowTime2"] = (float)(struCnfp.struGroupTick[i][0][j].nEjectDuration2/10.0);
                viewParam["defendtime"]   = (float)(struCnfg.nValveDefendTime/10.0);
                viewParam["underdefendtime"] = (float)(struCnfg.nValveUndefendTime/10.0);

                if(!struCnfp.struGroupTick[i][0][j].nEjwidth )
                {
                    viewParam["waitTime"]   = (float)(struCnfp.struGroupTick[i][0][j].nWaitTime/10.0);
                    viewParam["tailTime"]   = (float)(struCnfp.struGroupTick[i][0][j].nTailTime/10.0);
                }
                arrayObj.append(viewParam);
                viewParam.clear();
            }
            else
            {
                for(int k=0;k<MAX_VIEW;k++)
                {
                    if (struCnfg.struLayerInfo[i].nViewIsUsed[k])
                    {
                        viewId = struCnfg.struLayerInfo[i].nViewId[k];

                        viewParam["view"]      = viewId;
                        viewParam["delaytime1"] = (float)(struCnfp.struGroupIdentify[i][k][j].nEjectDelay/10.0);
                        viewParam["delaytime2"] = (float)(struCnfp.struGroupIdentify[i][k][j].nEjectDelayShape/10.0);
                        viewParam["blowTime"]   = (float)(struCnfp.struGroupTick[i][k][j].nEjectDuration/10.0);
                        viewParam["ejectType"] = struCnfp.struGroupTick[i][k][j].nEjwidth;
                        viewParam["delaytimeModel"] = (float)(struCnfp.struGroupIdentify[i][k][j].nEjectDelayModel/10.0);
                        viewParam["blowTime2"] = (float)(struCnfp.struGroupTick[i][k][j].nEjectDuration2/10.0);
                        viewParam["defendtime"]   = (float)(struCnfg.nValveDefendTime/10.0);
                        viewParam["underdefendtime"] = (float)(struCnfg.nValveUndefendTime/10.0);

                        if(!struCnfp.struGroupTick[i][0][j].nEjwidth )
                        {
                            viewParam["waitTime"]   = (float)(struCnfp.struGroupTick[i][0][j].nWaitTime/10.0);
                            viewParam["tailTime"]   = (float)(struCnfp.struGroupTick[i][0][j].nTailTime/10.0);
                        }
                        arrayObj.append(viewParam);
                        viewParam.clear();
                    }
                }
            }

            delays["delayParams"] = arrayObj;
            arrayObj.clear();
            array2Obj.append(delays);
            delays.clear();
        }

    }
    root["delays"] = array2Obj;
    root["reportFlag"] = reportFlag;

    std::string str = info.write(root);

    // qDebug()<<QString::fromStdString(str);
    if (!struGsh.bFlagMqttConnect)
    {
        root["reportFlag"] = reportFlag+2;
        str = info.write(root);
        cacheLocalRemoteInfo(URL_STATUSINFO_UPLOAD,str);
        return;
    }
    if (!myHttpFileClient->upLoadData(URL_STATUSINFO_UPLOAD,str))
    {
        root["reportFlag"] = reportFlag+2;
        str = info.write(root);
        cacheLocalRemoteInfo(URL_STATUSINFO_UPLOAD,str);
    }

}

/* 方案信息上传 */
void mqttMsgParaseThread::upLoadSchemeInfo()
{ 
    n_uploadSchemeInfoFlag = 0;
    if ( myHttpFileClient->g_connectName == "")
    {
        return;
    }
    Json::Value root;
    Json::Value  schemes;
    Json::Value arrayObj;
    Json::FastWriter info;
    QMap<int,string> m_machineTypeMap;

    m_machineTypeMap.clear();
    m_machineTypeMap.insert(MACHINE_VERTICAL_ZL_CV,"ZL_LS");
    m_machineTypeMap.insert(MACHINE_LD_ZL_GP,"ZL_LD");
    m_machineTypeMap.insert(MACHINE_LD2_ZL_GP,"ZL_LDS");

    root["code"] = myMqttThread->m_connect_name;
    root["model"] = m_machineTypeMap.value(struCnfg.nMachineType);

    root["version"] = m_version;

    for(int i =0;i< struCnfg.nProfileTotal;i++)
    {
        QString profileName = struCnfg.stuProfileIndex[i].sMaterialName;
        schemes["scheme"] = QString(profileName).toStdString();
        schemes["schemeId"] = i;

        arrayObj.append(schemes);
    }
    root["schemes"] = arrayObj;

    root["reportFlag"] = reportFlag;

    std::string str = info.write(root);

    if (!struGsh.bFlagMqttConnect)
    {
        root["reportFlag"] = reportFlag+2;
        str = info.write(root);
        cacheLocalRemoteInfo(URL_SCHEMEINFO_UPLOAD,str);
        return;
    }

    if(!myHttpFileClient->upLoadData(URL_SCHEMEINFO_UPLOAD,str))
    {
        root["reportFlag"] = reportFlag+2;
        str = info.write(root);
        cacheLocalRemoteInfo(URL_SCHEMEINFO_UPLOAD,str);
    }
}

/* 实时参数上报 */
void mqttMsgParaseThread::uploadRealTimePara()
{
    n_uploadRealTimeParaFlag = 0; //上传完成后 复位0
    if (myHttpFileClient->g_connectName == "")
    {
        return;
    }
    int cav_background,cav_material;
    Json::Value root;
    Json::Value lamps;
    Json::Value lampValue;
    Json::Value value;
    Json::Value arrayObj;
    Json::Value array2Obj;
    Json::FastWriter info;

    arrayObj.clear();
    array2Obj.clear();

    root["code"] = myMqttThread->m_connect_name;

    if(struCnfg.struAlarmConfig.nAirPressureAlarmType == 1)
    {
        root["pressure"] =((float)((int)((struGsh.struMachineStatus[struGsh.nLayer].nPressureValue+0.005)*1000)))/1000;
    }

    for(int i = 0;i< struCnfg.stuLayerLampCfg.nLampBoardTotal;i++){
        lamps["lampBoard"] = i;
        cav_background = 0;
        cav_material = 0;
        for(int j = 0;j < MAX_LAMP_PER_BOARD;j++){
            if(struCnfg.stuLayerLampCfg.nLampLayerAddr[i][j] == 0){//只处理设置为0层的
                if(struCnfg.stuLayerLampCfg.nLampPortEnable[i][j]){//used

                    lampValue["lampType"] = struCnfg.stuLayerLampCfg.nLampPortType[i][j];
                    lampValue["view"] = struCnfg.stuLayerLampCfg.nLampViewAddr[i][j];
                    switch (struCnfg.stuLayerLampCfg.nLampPortType[i][j]) {
                    case 0:
                        lampValue["lampId"] = cav_material++;
                        break;
                    case 1:
                        lampValue["lampId"] = cav_background++;
                        break;
                    default:
                        break;
                    }

                    //2021/4/21
                    lampValue["lampStatus"] = struCnfp.nLampEnable[struGsh.nLayer][i][j];
                    lampValue["value"] = struCnfp.nLampValue[struGsh.nLayer][i][j];

                    arrayObj.append(lampValue);
                }
            }
        }
        lamps["lampValues"] = arrayObj;
        arrayObj.clear();
        array2Obj.append(lamps);
    }
    root["lamps"] = array2Obj;
    array2Obj.clear();

    for (int i=0; i< CTRL_ALARM_MAX_TEMP_SENSOR; i++)
    {
        value["id"] = i;
        value["value"] = struGsh.struMachineStatus[struGsh.nLayer].nTempSensorValue[i];
        value["limit"] = struCnfg.struAlarmConfig.nTemptureAlarmLimit[i];
        QString str = QString("%1").arg(struCnfg.struAlarmConfig.nTemptureAlarmName[i]);
        value["name"] = str.toStdString();
        arrayObj.append(value);
    }
    root["temperatures"] = arrayObj;


    /*-----------------------------2021/4/21-------------------------------*/
    int hour = struCnfg.ntotalFeedTime/3600;
    int minute = struCnfg.ntotalFeedTime%3600/60;
    int second = struCnfg.ntotalFeedTime%60;
    QString timeFormatStr = QString("%1:%2:%3").arg(hour).arg(minute).arg(second);
    root["totalFeedTime"] = QString(timeFormatStr).toStdString();


    hour = struGsh.nCounter/3600;
    minute = struGsh.nCounter%3600/60;
    second = struGsh.nCounter%60;
    timeFormatStr = QString("%1:%2:%3").arg(hour).arg(minute).arg(second);
    root["thisRunTime"] = QString(timeFormatStr).toStdString();

    hour = struCnfg.nCounter/3600;
    minute = struCnfg.nCounter%3600/60;
    second = struCnfg.nCounter%60;
    timeFormatStr = QString("%1:%2:%3").arg(hour).arg(minute).arg(second);
    root["totalRunTime"] = QString(timeFormatStr).toStdString();

    hour = struCnfg.nthisFeedTime/3600;
    minute = struCnfg.nthisFeedTime%3600/60;
    second = struCnfg.nthisFeedTime%60;
    timeFormatStr = QString("%1:%2:%3").arg(hour).arg(minute).arg(second);
    root["thisFeedTime"] = QString(timeFormatStr).toStdString();

    root["totalWipeCount"]=struCnfg.ntotalWipeCount;
    root["sortStatus"]=struGsh.bSortStart;
    /*---------------------------------------------------------------------*/


    QString dateText = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    root["reportTime"] = QString(dateText).toStdString();
    root["reportFlag"] = reportFlag;

    std::string str = info.write(root);

    if (!struGsh.bFlagMqttConnect)
    {
        root["reportFlag"] = reportFlag+2;
        str = info.write(root);
        cacheLocalRemoteInfo(URL_REALTIMEINFO_UPLOAD,str);
        return;
    }
    if (!myHttpFileClient->upLoadData(URL_REALTIMEINFO_UPLOAD,str))
    {
        root["reportFlag"] = reportFlag+2;
        str = info.write(root);
        cacheLocalRemoteInfo(URL_REALTIMEINFO_UPLOAD,str);
    }
}

/* 整机参数上报 */
void mqttMsgParaseThread::uploadMachinePara()
{
    if ( myHttpFileClient->g_connectName == "")
    {
        return;
    }
    int cav = 0;
    int cmcnt = 0;
    Json::Value root;
    Json::Value bgLamps;
    Json::Value bgLampValue;
    Json::Value pixels;
    Json::Value value;
    Json::Value arrayObj;
    Json::Value array2Obj;
    Json::FastWriter info;

    arrayObj.clear();
    array2Obj.clear();

    root["code"] = myMqttThread->m_connect_name;
    QString mystr= "";
    for(int j=0;j<struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[0].nIdentifyGroupTotal;j++)
    {
        int groupUnitCnt =struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[0].stuIdenGupInfo[j].nGroupUnitTotal;
        mystr += QString::number(groupUnitCnt);
        if(j != struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[0].nIdentifyGroupTotal -1)
        {
            mystr +=":";
        }
    }
    root["channelRatio"] = QString(mystr).toStdString();

    for(int i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(int j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            pixels["view"] = j;
            if (struCnfg.struLayerInfo[i].nViewIsUsed[j])
            {
                for(int k = 0;k < struCnfg.struLayerInfo[i].nViewUnitTotal[j];k++)
                {
                    cmcnt ++;
                    value["id"] = k;
                    value["pixelStart"] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nChannelBegin[k];
                    value["pixelEnd"] = struCnfg.struLayerInfo[i].stuCameraInfo[j].nChannelEnd[k];
                    arrayObj.append(value);
                }
            }
            pixels["pixelValues"] = arrayObj;
            arrayObj.clear();
            array2Obj.append(pixels);
        }
    }
    root["pixels"] = array2Obj;
    array2Obj.clear();
    root["cameraCount"] = cmcnt;

    QString dateText = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    root["reportTime"] = QString(dateText).toStdString();
    root["reportFlag"] = reportFlag;

    std::string str = info.write(root);

    if (!struGsh.bFlagMqttConnect)
    {
        root["reportFlag"] = reportFlag+2;
        str = info.write(root);
        cacheLocalRemoteInfo(URL_MACHINEINFO_UPLOAD,str);
        return;
    }

    if (!myHttpFileClient->upLoadData(URL_MACHINEINFO_UPLOAD,str))
    {
        root["reportFlag"] = reportFlag+2;
        str = info.write(root);
        cacheLocalRemoteInfo(URL_MACHINEINFO_UPLOAD,str);
    }
}

/* 部件状态+预警上报 */
int  mqttMsgParaseThread::uploadPartsStatusInfo()
{
    n_uploadPartsStatusFlag = 0;
    if (myHttpFileClient->g_connectName == "")
    {
        return SYSTEM_ERROR ;
    }
    Json::Value root;
    Json::Value boards;
    Json::Value value;
    Json::Value lifeValue;
    Json::Value arrayObj;
    Json::Value array2Obj;
    Json::FastWriter info;

    int state = 0;
    QString namestr = "";
    int i,j,n;
    arrayObj.clear();
    array2Obj.clear();

    root["code"] = myMqttThread->m_connect_name;
    root["version"]= m_version;

    stru_version version;
    memcpy(&version,&struGsh.struVer,sizeof(version));

    QString m_type;
    QString m_mode;
    QString valuestr;
    if(version.sInterface[4] == 0)
        m_type = "M";
    else if (version.sInterface[4] == 48)
        m_type = "C";
    if(version.sInterface[2] == 0)
        m_mode = "(F)";
    else
        m_mode = "(U)";

    if (version.nInterfaceRes == 1)
    {
        if(version.sInterface[1] < 10)
            valuestr = QString("%1.0%2.%3%4").arg(QString::number(version.sInterface[0])).arg(QString::number(version.sInterface[1])).arg(m_type).arg(m_mode);
        else
            valuestr = QString("%1.%2.%3%4").arg(QString::number(version.sInterface[0])).arg(QString::number(version.sInterface[1])).arg(m_type).arg(m_mode);

        root["interfaceBoard"] = 0;
        root["interfaceVersion"] = valuestr.toStdString();
    }
    else if(version.nInterfaceRes == 0)
    {
        if(version.sInterface[1] < 10)
            valuestr = QString("%1.0%2.%3%4").arg(QString::number(version.sInterface[0])).arg(QString::number(version.sInterface[1])).arg(m_type).arg(m_mode);
        else
            valuestr = QString("%1.%2.%3%4").arg(QString::number(version.sInterface[0])).arg(QString::number(version.sInterface[1])).arg(m_type).arg(m_mode);

        root["interfaceBoard"] = 1;
        root["interfaceVersion"] = valuestr.toStdString();
    }
    else  // 未查询
    {
        root["interfaceBoard"] = 2;
        root["interfaceVersion"] = "-";
    }
    for(n = 0;n < struCnfg.nLayerTotal;n++)
    {
        for(i = 0;i < struCnfg.struLayerInfo[n].nViewChainTotal;i++)
        {
            if (struCnfg.struLayerInfo[n].nViewIsUsed[i])
            {
                boards["view"] = i;
                for(j = 0;j < struCnfg.struLayerInfo[n].nViewUnitTotal[i];j++){
                    value["id"] = j+1; // 通道
                    
                    if(version.sUnit[n][i][j][4] == 0)
                        m_type = "M";
                    else if (version.sUnit[n][i][j][4] == 48)
                        m_type = "C";
                    if(version.sUnit[n][i][j][2]== 0)
                        m_mode = "(F)";
                    else
                        m_mode = "(U)";
                    state = version.nUnitRes[n][i][j];
                    if (state == 1)
                    {
                        //value["value"] = 0;
                        if(version.sUnit[n][i][j][1] < 10){
                            valuestr = QString("%1.0%2.%3%4").arg(version.sUnit[n][i][j][0]).arg(version.sUnit[n][i][j][1]).arg(m_type).arg(m_mode);
                        }else{
                            valuestr = QString("%1.%2.%3%4").arg(version.sUnit[n][i][j][0]).arg(version.sUnit[n][i][j][1]).arg(m_type).arg(m_mode);
                        }
                        value["value"] = 0;
                        value["version"] = valuestr.toStdString();
                    }
                    else if(state == 0)
                    {
                        if(version.sUnit[n][i][j][1] < 10){
                            valuestr = QString("%1.0%2.%3%4").arg(version.sUnit[n][i][j][0]).arg(version.sUnit[n][i][j][1]).arg(m_type).arg(m_mode);
                        }else{
                            valuestr = QString("%1.%2.%3%4").arg(version.sUnit[n][i][j][0]).arg(version.sUnit[n][i][j][1]).arg(m_type).arg(m_mode);
                        }

                        value["value"] = 1;
                        value["version"] = valuestr.toStdString();
                    }
                    else
                    {
                        value["value"]= 2;
                        value["version"] = "-";
                    }
                    arrayObj.append(value);
                }
                boards["unitValues"] = arrayObj;
                arrayObj.clear();
                array2Obj.append(boards);
            }
        }
    }
    root["unitBoards"] = array2Obj;
    boards.clear();
    value.clear();
    array2Obj.clear();

    for(n = 0;n < struCnfg.nLayerTotal;n++)
    {
        for(i = 0;i < struCnfg.struLayerInfo[n].nVavleChainTotal;i++){
            if (struCnfg.struLayerInfo[n].nViewIsUsed[i])
            {
                boards["view"] = i;
                for(j = 0;j < struCnfg.struLayerInfo[n].nVavleBoardTotal[i];j++){
                    value["id"] = j+1;

                    if(version.sVavle[n][i][j][4] == 0)
                        m_type = "M";
                    else if (version.sVavle[n][i][j][4] == 48)
                        m_type = "C";
                    if(version.sVavle[n][i][j][2]== 0)
                        m_mode = "(F)";
                    else
                        m_mode = "(U)";

                    state = version.nVavleRes[n][i][j];
                    if (state == 1)
                    {
                        value["value"] = 0;
                        if(version.sVavle[n][i][j][1] < 10){
                            valuestr = QString("%1.0%2.%3%4").arg(version.sVavle[n][i][j][0]).arg(version.sVavle[n][i][j][1]).arg(m_type).arg(m_mode);
                        }else{
                            valuestr = QString("%1.%2.%3%4").arg(version.sVavle[n][i][j][0]).arg(version.sVavle[n][i][j][1]).arg(m_type).arg(m_mode);
                        }
                        value["version"] = valuestr.toStdString();
                    }
                    else if(state == 0)
                    {
                        value["value"]= 1;
                        if(version.sVavle[n][i][j][1] < 10){
                            valuestr = QString("%1.0%2.%3%4").arg(version.sVavle[n][i][j][0]).arg(version.sVavle[n][i][j][1]).arg(m_type).arg(m_mode);
                        }else{
                            valuestr = QString("%1.%2.%3%4").arg(version.sVavle[n][i][j][0]).arg(version.sVavle[n][i][j][1]).arg(m_type).arg(m_mode);
                        }
                        value["version"] = valuestr.toStdString();
                    }
                    else
                    {
                        value["value"]= 2;
                        value["version"]= "-";
                    }
                    arrayObj.append(value);
                }
                boards["valveValues"] = arrayObj;
                arrayObj.clear();
                array2Obj.append(boards);
            }
        }
    }
    root["valveBoards"] = array2Obj;
    boards.clear();
    value.clear();
    array2Obj.clear();

    if(struCnfg.nChuteTotal > 1 || (struCnfg.nChuteTotal == 1 && !struCnfg.nAllInOne))
    {
        for(j = 0;j < struCnfg.nCtrlBoardTotal;j++){

            value["id"] = j+1;

            if(version.sCtrlBoard[j][4] == 0)
                m_type = "M";
            else if (version.sCtrlBoard[j][4] == 48)
                m_type = "C";
            if(version.sCtrlBoard[j][2]== 0)
                m_mode = "(F)";
            else
                m_mode = "(U)";

            state = version.nCtrlRes[j];
            if(state == 1){
                value["value"] = 0;
                if(version.sCtrlBoard[j][1] < 10){
                    valuestr = QString("%1.0%2.%3%4").arg(version.sCtrlBoard[j][0]).arg(version.sCtrlBoard[j][1]).arg(m_type).arg(m_mode);
                }
                else
                {
                    valuestr = QString("%1.%2.%3%4").arg(version.sCtrlBoard[j][0]).arg(version.sCtrlBoard[j][1]).arg(m_type).arg(m_mode);
                }
                value["version"] = valuestr.toStdString();
            }
            else if(state == 0)
            {
                value["value"]= 1;
                if(version.sCtrlBoard[j][1] < 10){
                    valuestr = QString("%1.0%2.%3%4").arg(version.sCtrlBoard[j][0]).arg(version.sCtrlBoard[j][1]).arg(m_type).arg(m_mode);
                }
                else
                {
                    valuestr = QString("%1.%2.%3%4").arg(version.sCtrlBoard[j][0]).arg(version.sCtrlBoard[j][1]).arg(m_type).arg(m_mode);
                }

                value["version"] = valuestr.toStdString();
            }
            else
            {
                value["value"]= 2;
                value["version"]= "-";
            }
            arrayObj.append(value);
        }
        root["ctrlBoards"] = arrayObj;
        arrayObj.clear();
        value.clear();

        for(j = 0;j < struCnfg.stuLayerLampCfg.nLampBoardTotal;j++){
            value["id"] = j+1;

            if(version.sLampBoard[j][4] == 0)
                m_type = "M";
            else if (version.sLampBoard[j][4] == 48)
                m_type = "C";
            if(version.sLampBoard[j][2]== 0)
                m_mode = "(F)";
            else
                m_mode = "(U)";

            state = version.nLampRes[j];
            if(state == 1){
                value["value"] = 0;
                if(version.sLampBoard[j][1] < 10){
                    valuestr = QString("%1.0%2.%3%4").arg(version.sLampBoard[j][0]).arg(version.sLampBoard[j][1]).arg(m_type).arg(m_mode);
                }else{
                    valuestr = QString("%1.%2.%3%4").arg(version.sLampBoard[j][0]).arg(version.sLampBoard[j][1]).arg(m_type).arg(m_mode);
                }
                value["version"]= valuestr.toStdString();
            }
            else if(state == 0){
                value["value"] = 1;
                if(version.sLampBoard[j][1] < 10){
                    valuestr = QString("%1.0%2.%3%4").arg(version.sLampBoard[j][0]).arg(version.sLampBoard[j][1]).arg(m_type).arg(m_mode);
                }else{
                    valuestr = QString("%1.%2.%3%4").arg(version.sLampBoard[j][0]).arg(version.sLampBoard[j][1]).arg(m_type).arg(m_mode);
                }
                value["version"]= valuestr.toStdString();
            }
            else
            {
                value["value"] = 2;
                value["version"]= "-";
            }
            arrayObj.append(value);
        }
        root["lampBoards"] = arrayObj;
        arrayObj.clear();
        value.clear();
    }

    //添加部件寿命及预警 2020/9/2
    lifeValue["used"]  = (struCnfg.nCounter - struCnfg.stuUseLimitCfg.valve_start_use)/3600/24;
    lifeValue["total"] = struCnfg.stuUseLimitCfg.valve_life;
    root["valveLife"] = lifeValue;
    lifeValue.clear();

    lifeValue["used"]  = (struCnfg.nCounter - struCnfg.stuUseLimitCfg.filter_start_use)/3600/24;
    lifeValue["total"] = struCnfg.stuUseLimitCfg.filter_life;
    root["filterLife"] = lifeValue;
    lifeValue.clear();

    lifeValue["used"]  = struCnfg.ntotalWipeCount - struCnfg.stuUseLimitCfg.wipe_start_use;
    lifeValue["total"] = struCnfg.stuUseLimitCfg.wipe_life;
    root["wipeLife"] = lifeValue;
    lifeValue.clear();

    lifeValue["used"]  = (struCnfg.nCounter - struCnfg.stuUseLimitCfg.lamp_start_use)/3600/24;
    lifeValue["total"] = struCnfg.stuUseLimitCfg.lamp_life;
    root["lampLife"] = lifeValue;
    lifeValue.clear();

    if(reportFlag == Request_Send)
        root["cmdUuid"] = m_uuid;
    QString dateText = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    root["reportTime"] = QString(dateText).toStdString();
    root["reportFlag"] = reportFlag;

    std::string str = info.write(root);

    if (!struGsh.bFlagMqttConnect)
    {
        root["reportFlag"] = reportFlag+2;
        str = info.write(root);
        cacheLocalRemoteInfo(URL_PARTSTATUSINFO_UPLOAD,str);
        return SYSTEM_ERROR;
    }

    if (!myHttpFileClient->upLoadData(URL_PARTSTATUSINFO_UPLOAD,str))
    {
        root["reportFlag"] = reportFlag+2;
        str = info.write(root);
        cacheLocalRemoteInfo(URL_PARTSTATUSINFO_UPLOAD,str);
        return SYSTEM_ERROR;
    }
    return SUCCESS;
}

/* 参数异常报警上报 */
void mqttMsgParaseThread::uploadParaAlarm()
{
    n_uploadAlarmFlag = 0;
    if ( myHttpFileClient->g_connectName == "")
    {
        return;
    }
    Json::Value root;
    Json::FastWriter info;

    root["code"] = myMqttThread->m_connect_name;

    if (struGsh.nAlarmStautsCurrent[ALARM_PRESSURE])
        root["alarmPressure"] = struGsh.nAlarmStautsCurrent[ALARM_PRESSURE];

    if (struGsh.nAlarmStautsCurrent[ALARM_WIPE])
        root["alarmWipe"]     = struGsh.nAlarmStautsCurrent[ALARM_WIPE] ;

    if (struGsh.nAlarmStautsCurrent[ALARM_TEMP_HIGH])
        root["alarmEnvTemp"]  = struGsh.nAlarmStautsCurrent[ALARM_TEMP_HIGH] ;

    if (struGsh.nAlarmStautsCurrent[ALARM_FEED_LEVEL])
        root["alarmFeed"]     = struGsh.nAlarmStautsCurrent[ALARM_FEED_LEVEL];

    if (struCnfg.nUseIpcEnable == 1 && (struGsh.bAlarmIpcStatus[0]|struGsh.bAlarmIpcStatus[1])== 1){
        root["alarmIpc"]      = struGsh.bAlarmIpcStatus[0]|struGsh.bAlarmIpcStatus[1];
    }

    QString dateText = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    root["reportTime"] = QString(dateText).toStdString();
    root["reportFlag"] = reportFlag;

    std::string str = info.write(root);

    if (!struGsh.bFlagMqttConnect)
    {
        root["reportFlag"] = reportFlag+2;
        str = info.write(root);
        cacheLocalRemoteInfo(URL_PARAALARM_UPLOAD,str);
        return;
    }

    if (!myHttpFileClient->upLoadData(URL_PARAALARM_UPLOAD,str))
    {
        root["reportFlag"] = reportFlag+2;
        str = info.write(root);
        cacheLocalRemoteInfo(URL_PARAALARM_UPLOAD,str);

    }
}

/* 统计信息上报 */
int mqttMsgParaseThread::uploadStatistics()
{
    n_uploadStatisticsFlag = 0;

    if (myHttpFileClient->g_connectName == "")
    {
        return SYSTEM_ERROR;
    }
#if 0
    Json::Value root;
    Json::Value value;
    Json::Value ejectValue;
    Json::Value arrayObj;
    Json::Value array2Obj;
    Json::FastWriter info;
    float temp,temp1;
    int temp2;
    int  tmpValue[ARITH_HSV+1];       //存储各个算法对应的吹气次数
    bool tmpValueEnable[ARITH_HSV+1]; //算法使能标识
    int  totalValue = 0;              //该视链总吹气次数
    QString fts;

    for(int i = 0;i < ARITH_HSV+1;i++)
    {
        tmpValue[i] = 0;
        tmpValueEnable[i] = 0;
    }
    arrayObj.clear();
    array2Obj.clear();

    QStringList arithNameList;
    arithNameList<< g_myLan().arith_gray_a << g_myLan().arith_gray_b << g_myLan().arith_gray_c<< g_myLan().arith_gray_d
                 << g_myLan().arith_discolor_a << g_myLan().arith_discolor_b << g_myLan().arith_cross
                 << g_myLan().arith_svm_a << g_myLan().arith_svm_b << g_myLan().arith_svm_c
                 << g_myLan().arith_big_small << g_myLan().arith_long_short << g_myLan().arith_circle_long << g_myLan().arith_hsv;

    root["code"] = myMqttThread->m_connect_name;

    temp = 0.0;temp1 = 0.0;temp2 = 0;
    for(int m = 0;m < struCnfg.struLayerInfo[0].stuViewInfo[0].stuIdenGupInfo[0].nGroupUnitTotal; m++)
    {
        value["id"] = m;
        fts = QString("%1").arg((int)(struGsh.stagedThroughout[0][0][m]*1000+0.5)/1000.0);
        value["value"] = fts.toLocal8Bit().data();
        temp1 += (float)(struGsh.stagedThroughout[0][0][m]);
        temp2 += struGsh.stagedTotalArithEjectTimes[0][0][m];
        arrayObj.append(value);
    }
    root["throughout"] = arrayObj;
    arrayObj.clear();
    root["totalThroughout"] = temp1;
    root["totalGoodThroughout"] = temp1 - (float)temp2*180/(float)struCnfp.n_coff;

    //    if ((float)temp2*180/(float)struCnfp.n_coff > temp1) return; //avoid para error
    //    if ((float)temp2*180/(float)struCnfp.n_coff > temp1){
    //        root["totalGoodThroughout"] = temp1;
    //    }

    for(int i = 0;i < struCnfg.nLayerTotal;i++)
    {
        for(int j=0;j<MAX_VIEW;j++)
        {
            if (struCnfg.struLayerInfo[i].nViewIsUsed[j])
            {
                value["view"] = j;

                memset(tmpValue,0,sizeof(tmpValue));
                memset(tmpValueEnable,0,sizeof(tmpValueEnable));
                for(int k=0;k<struCnfg.struLayerInfo[i].stuViewInfo[0].nIdentifyGroupTotal;k++)
                {
                    for(int m = 0;m < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++)
                    {
                        for(int n=0; n<ARITH_HSV+1; n++)
                        {
                            switch (n){
                            case ARITH_GREY_A:
                            case ARITH_GREY_B:
                            case ARITH_GREY_C:
                                if(struCnfp.struGroupIdentify[i][j][k].struGreyColor[n].nArithEnable)
                                {
                                    tmpValueEnable[n] = 1;
                                    tmpValue[n] += struGsh.struStatics[i][j][m].nGreyBadData[n];

                                }
                                break;
                            case ARITH_DISCOLOR_A:
                            case ARITH_DISCOLOR_B:
                                if(struCnfp.struGroupIdentify[i][j][k].struGreyColor[n].nArithEnable)
                                {
                                    tmpValueEnable[n] = 1;
                                    tmpValue[n] += struGsh.struStatics[i][j][m].nDiscoloryBadData[n-ARITH_DISCOLOR_A];
                                }
                                break;
                            case ARITH_SVM_A:
                            case ARITH_SVM_B:
                            case ARITH_SVM_C:
                                if(struCnfp.struGroupIdentify[i][j][k].struSvm[n-ARITH_SVM_A].nArithEnable)
                                {
                                    tmpValueEnable[n] = 1;
                                    tmpValue[n] += struGsh.struStatics[i][j][m].nSvmData[n-ARITH_SVM_A];
                                }
                                break;
                            default:
                                break;
                            }

                            totalValue += tmpValue[n];
                        }
                    }
                }
                value["totalValue"] = totalValue;

                for(int n=0; n<ARITH_HSV+1; n++)
                {
                    if (tmpValueEnable[n])
                    {
                        ejectValue["arithId"] = n;
                        ejectValue["ejectTimesValue"] = tmpValue[n];

                        ejectValue["arithName"] = QString(arithNameList.at(n)).toLocal8Bit().data();

                        arrayObj.append(ejectValue);
                    }
                }
                value["values"] = arrayObj;
                arrayObj.clear();

                array2Obj.append(value);

            }
        }
    }
    root["ejectTimes"] = array2Obj;
    array2Obj.clear();

    temp = 0.0;
    //    for(int m = 0;m < struCnfg.struLayerInfo[0].nViewUnitTotal[0] ; m++)
    //    {
    //        if (struGsh.struStatics[0][0][m].nThroughout == 0)
    //            temp += 0;
    //        else
    //            temp += struGsh.struStatics[0][0][m].nTotalArithEjectTimes*180/(float)struGsh.struStatics[0][0][m].nThroughout;
    //    }
    temp = (float)temp2*180/(float)temp1/(float)struCnfp.n_coff;
    root["bad"] = ((float)((int)(temp*10000)))/100;

    //    if (((float)((int)(temp*10000)))/100 >100) return; //avoid para error
    //    if (((float)((int)(temp*10000)))/100 >100){
    //        root["bad"] = 0.01;
    //    }

    //含碎还是老算法，待更新
    temp = 0.0;
    for(int m = 0;m < struCnfg.struLayerInfo[0].nViewUnitTotal[0] ; m++)
    {
        if (struGsh.struStatics[0][0][m].nThroughout == 0)
            temp += 0;
        else
            temp += struGsh.struStatics[0][0][m].nShapeData*180/(float)struGsh.struStatics[0][0][m].nThroughout;
    }
    root["broken"] = ((float)((int)(temp*10000)))/100;

    //    if (((float)((int)(temp*10000)))/100 >100) return; //avoid para error
    //    if (((float)((int)(temp*10000)))/100 >100){
    //        root["broken"] = 0.01;
    //    }

    QString dateText = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    root["reportTime"] = QString(dateText).toStdString();
    root["reportFlag"] = reportFlag;

    std::string str = info.write(root);

    if (!struGsh.bFlagMqttConnect)
    {
        root["reportFlag"] = reportFlag+2;
        str = info.write(root);
        cacheLocalRemoteInfo(URL_STATISTICS_UPLOAD,str);
        return;
    }

    if (!myHttpFileClient->upLoadData(URL_STATISTICS_UPLOAD,str))
    {
        root["reportFlag"] = reportFlag+2;
        str = info.write(root);
        cacheLocalRemoteInfo(URL_STATISTICS_UPLOAD,str);
    }

    uploadLocalRemoteInfo();//本次开机缓存的数据在此处补发
#endif
}

/* 远程服务许可上报 */
void mqttMsgParaseThread::uploadRemoteAccess(int val)
{
    if ( myHttpFileClient->g_connectName == "")
    {
        return;
    }
    Json::Value root;
    Json::FastWriter info;

    root["code"] = myMqttThread->m_connect_name;
    root["approve"] = val;

    QString dateText = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    root["reportTime"] = QString(dateText).toStdString();

    std::string str = info.write(root);

    if (!struGsh.bFlagMqttConnect)
    {
        cacheLocalRemoteInfo(URL_REMOTEACCESS_UPLOAD,str);
        return;
    }

    if (!myHttpFileClient->upLoadData(URL_REMOTEACCESS_UPLOAD,str))
    {
        struGsh.bFlagMqttConnect = 0;
        cacheLocalRemoteInfo(URL_REMOTEACCESS_UPLOAD,str);
    }
}

#if 0
/* 日志文件上报 */
int mqttThread::uploadLogFile(string date)
{
    if ( myHttpFileClient->g_connectName == "")
    {
        return 0;
    }
    QMap<int,QString> m_machineTypeMap;

    m_machineTypeMap.clear();
    m_machineTypeMap.insert(0,"DM_RG");
    m_machineTypeMap.insert(1,"DM_ZK");
    m_machineTypeMap.insert(2,"ZL_CS");
    m_machineTypeMap.insert(3,"ZL_CV");
    m_machineTypeMap.insert(4,"ZL_LD");
    m_machineTypeMap.insert(5,"ZL_LD2");

    QString model = m_machineTypeMap.value(struCnfg.nMachineType);

#ifdef Q_OS_WIN
    QString path = qApp->applicationDirPath()+"/logs/";
#else
    QString path = "/sdcard/logs/";
#endif
    QString dateStr = QString::fromStdString(date);
    QString logPath = path+"log.txt."+dateStr;
    if (dateStr == "")
    {
        QDir dir(path);
        QStringList filter;
        filter<<"log.txt.*";
        dir.setNameFilters(filter);
        //按时间排序
        QList<QFileInfo> fileInfo = QList<QFileInfo>(dir.entryInfoList(filter,QDir::AllEntries,QDir::Name));

        if (fileInfo.size() < 2) return 0;
        logPath= fileInfo.at(fileInfo.size()-2).absoluteFilePath();
        //    QString logName = fileInfo.at(fileInfo.size()-2).baseName();
        QString logName = fileInfo.at(fileInfo.size()-2).suffix();
        dateStr = logName.remove("-");
    }

    QString url = URL_LOGFILE_UPLOAD+ model+"/"+ myHttpFileClient->g_connectName +"/"+dateStr;
    //upLoadFile("https://www.chinaamd.cn:8900/api/operatelog/file/upload/RG/xiewj-test/20200201","./2020-02-01.log");

    return (myHttpFileClient->upLoadFile(url,logPath));

}
#endif

/* 日志文件上报 */
int mqttMsgParaseThread::uploadLogFile(string date)
{
    n_uploadLogFileFlag = 0;
    if ( myHttpFileClient->g_connectName == "")
    {
        return SYSTEM_ERROR;
    }
    QMap<int,QString> m_machineTypeMap;

    m_machineTypeMap.clear();
    m_machineTypeMap.insert(MACHINE_VERTICAL_ZL_CV,"ZL_LS");
    m_machineTypeMap.insert(MACHINE_LD_ZL_GP,"ZL_LD");
    m_machineTypeMap.insert(MACHINE_LD2_ZL_GP,"ZL_LDS");

    QString model = m_machineTypeMap.value(struCnfg.nMachineType);

#ifdef Q_OS_WIN
    QString path = qApp->applicationDirPath()+"/logs/";
#else
    QString path = "/sdcard/logs/";
#endif
    QString dateStr = QString::fromStdString(date);
    QString logPath = path+"log.txt."+dateStr;
    if (dateStr == ""){
        QDir dir(path);
        QStringList filter;
        filter<<"log.txt.*";
        dir.setNameFilters(filter);
        //按时间排序
        QList<QFileInfo> fileInfo = QList<QFileInfo>(dir.entryInfoList(filter,QDir::AllEntries,QDir::Name));

        if (fileInfo.size() < 2) return 0;
        logPath= fileInfo.at(fileInfo.size()-1).absoluteFilePath();
        //    QString logName = fileInfo.at(fileInfo.size()-2).baseName();
        QString logName = fileInfo.at(fileInfo.size()-1).suffix();
        dateStr = logName;
    }
    else{
        QDateTime  dateTime = QDateTime::currentDateTime();
        int year = dateTime.date().year();
        int month = dateTime.date().month();
        int day = dateTime.date().day();

        QStringList vecList = dateStr.split("-");

        if (vecList.at(2).toInt() != day || vecList.at(1).toInt() != month || vecList.at(0).toInt() != year)
            logPath = "/sdcard/logs/log.txt."+dateStr;
        else{
            g_Runtime().copyFileToPath("/sdcard/logs/log.txt","/sdcard/logs/todaylog.txt",true);
            logPath = "/sdcard/logs/todaylog.txt";//支持当天日志上传
        }
    }

    QFileInfo logFileInfo(logPath);
    if (!logFileInfo.exists())
    {
        return NOT_EXIST;
    }

    Json::Value root;
    Json::FastWriter info;
    root["code"] = myMqttThread->m_connect_name;
    root["fileName"] = QString(QFileInfo(logPath).fileName()).toStdString();
    root["uploadType"] = "OPERATE_LOG";//上传类型：DEVICE_MATERIAL_IMG,DEVICE_LIGHT_IMG,DEVICE_GLASS_IMG,OPERATE_LOG,RUN_PARAM
    std::string str = info.write(root);

    if (myHttpFileClient->requestOssUploadSign(URL_GETOSSUPLOADSIGN,str))
    {
        if(myHttpFileClient->upLoadOssFile(logPath))
        {
            /* 回报上传结果给中科，主要就是存在阿里云上的路径 */
            root.clear();
            root["code"] = myMqttThread->m_connect_name;
            root["model"] = QString(model).toStdString();
            root["name"] = QString(QFileInfo(logPath).fileName()).toStdString();
            root["fileSize"] = QString(g_Runtime().getFileSize(logPath)).toStdString();
            root["url"] = QString(myHttpFileClient->fileUrl).toStdString();
            root["logDate"] = QString(dateStr).toStdString();
            if(reportFlag == Request_Send)
                root["cmdUuid"] = m_uuid;
            root["md5"] = QString(g_Runtime().getFileMd5(logPath)).toStdString();
            root["reportFlag"] = reportFlag;
            str = info.write(root);

            //            return(myHttpFileClient->upLoadData(URL_LOGFILE_UPLOAD,str));

            if(myHttpFileClient->upLoadData(URL_LOGFILE_UPLOAD,str)){
                return SUCCESS;
            }
            return UPLOAD_ERR;
        }
        return OSS_UPLOAD_ERR;
    }
    return OSS_REQUESRT_ERR;

}

/* 缓存本地数据 */
void mqttMsgParaseThread::cacheLocalRemoteInfo(string url,string val)
{
    if (myHttpFileClient->g_connectName == "")
    {
        return;
    }

    QFile data("/sdcard/cnf/remotecache.txt");
    data.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
    QTextStream ofile(&data);

    //    QSettings setting(CFG_APPSet,QSettings::IniFormat);
    //    string testip = setting.value("testIp","www.chinaamd.cn").toString().toStdString();
    //    string str = HTTPS+testip+url + "_#_" + val;
    string str = url + "_#_" + val;
    ofile<<QString().fromStdString(str);
    msleep(100);
    data.close();
}

/* 本地缓存数据补发 */
void mqttMsgParaseThread::uploadLocalRemoteInfo()
{
    if (!struGsh.bFlagMqttConnect) return;

    QFileInfo file("/sdcard/cnf/remotecache.txt");
    if (!file.exists() || file.size() == 0) return;

    QFile tempdata("/sdcard/cnf/.tempcache.txt");
    tempdata.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
    QTextStream ofile(&tempdata);

    QFile data("/sdcard/cnf/remotecache.txt");
    data.open(QFile::ReadOnly | QIODevice::Text);
    while(!data.atEnd())
    {
        QByteArray ba = data.readLine();
        QStringList list = QString(ba).split("_#_");
        QString url = list.at(0);
        if (url.isNull()) continue;
        string str = QString(list.at(1)).toStdString();

        if (list.length() == 2)
        {
            if (!struGsh.bFlagMqttConnect )
            {
                ofile<<QString(ba);
                continue;
            }
            if (!myHttpFileClient->upLoadData(url,str))
            {
                //                struGsh.bFlagMqttConnect = 0;
                ofile<<QString(ba);
                break;
            }
        }
    }
    data.close();
    tempdata.close();

    QFileInfo tempfile("/sdcard/cnf/.tempcache.txt");
    if (tempfile.size()>0)
    {
        system("mv /sdcard/cnf/.tempcache.txt  /sdcard/cnf/remotecache.txt");
        system("sync");
    }
    else
    {
        system("rm /sdcard/cnf/remotecache.txt");
        system("sync");
    }
}

/* 远程采图图像上传 */
int mqttMsgParaseThread::uploadCaptureImage()
{
    n_uploadImageFlag = 0;
    m_uploadStat = 0;
    QString imageName = QFileInfo(n_imagePath).baseName();
    QStringList list = imageName.split("_");
    Json::Value root;
    Json::FastWriter info;
    root["code"] = myMqttThread->m_connect_name;
    root["fileName"] = QString(QFileInfo(n_imagePath).fileName()).toStdString();
    root["uploadType"] = "DEVICE_MATERIAL_IMG";//上传类型：DEVICE_MATERIAL_IMG,DEVICE_LIGHT_IMG,DEVICE_GLASS_IMG
    std::string str = info.write(root);


    logger()->info("image:%1_%2_%3 uld...",list.at(0),list.at(1),list.at(2));

    if (myHttpFileClient->requestOssUploadSign(URL_GETOSSUPLOADSIGN,str))
    {
        if(myHttpFileClient->upLoadOssFile(n_imagePath))
        {
            /* 回报上传结果给中科，主要就是图片存在阿里云上的路径 */
            root.clear();
            root["code"] = myMqttThread->m_connect_name;
            root["layer"] = QString(list.at(0)).toStdString();
            root["views"] = QString(list.at(1)).toStdString();
            root["channel"] = QString(list.at(2)).toStdString();
            //            root["type"] = QString(list.at(3)).toStdString();
            if(reportFlag == Request_Send)
                root["cmdUuid"] = m_uuid;
            root["name"] = QString(QFileInfo(n_imagePath).fileName()).toStdString();
            root["url"] = QString(myHttpFileClient->fileUrl).toStdString();

            QString dateText = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            root["reportTime"] = QString(dateText).toStdString();
            root["reportFlag"] = reportFlag;

            str = info.write(root);

            if(myHttpFileClient->upLoadData(URL_IMAGECAPTURE_UPLOAD,str))
            {
                g_Runtime().mySystem("rm "+n_imagePath);
                logger()->info("img uld ok");
                return SUCCESS;
            }
            g_Runtime().mySystem("rm "+n_imagePath);
            logger()->info("img uld err");
            return UPLOAD_ERR;
        }
        else{
            logger()->info("img ossuld err");
            return OSS_UPLOAD_ERR;
        }
    }
    else{
        logger()->info("img ossreq err");
        return OSS_REQUESRT_ERR;
    }
}

/* 远程波形数据上传 */
int mqttMsgParaseThread::uploadCaptureWave(int Layer,int View,int Unit)
{
    n_uploadWaveFlag = 0;

    unsigned int layer = 0,view = 0,unit = 0,type = 0;
    int sensorPixel = 1024;

    if(struGsh.nRemoteCaptureAllWave == 1)
    {
        layer = Layer;
        view = View;
        unit = Unit;
    }
    else
    {
        QString strVec = QString().fromStdString(m_value);
        QStringList vecList = strVec.split(":");

        layer = vecList.at(0).toInt();
        view = vecList.at(1).toInt();
        unit = vecList.at(2).toInt();
        type = vecList.at(3).toInt();
    }

    Json::Value root;
    Json::Value arrayObj_r;
    Json::Value arrayObj_g;
    Json::Value arrayObj_b;
    Json::Value arrayObj_i;
    Json::FastWriter info;
    root["code"]  = myMqttThread->m_connect_name;
    root["layer"] = QString::number(layer).toStdString();
    root["views"] = QString::number(view).toStdString();
    root["channel"] = QString::number(unit).toStdString();
    root["type"]  = QString::number(type).toStdString();
    if(reportFlag == Request_Send)
        root["cmdUuid"] = m_uuid;
    switch (struCnfp.nSensorPixel[0][view])
    {
    case SENSOR_PIXELS_1024:
        sensorPixel = 1024;
        break;
    case SENSOR_PIXELS_2048:
        sensorPixel = 2048;
        break;
    case SENSOR_PIXELS_512:
        sensorPixel = 512;
        break;
    case SENSOR_PIXELS_256:
        sensorPixel = 256;
        break;
    }

    for(int i =0; i< sensorPixel;i++){
        arrayObj_r.append(struGsh.sRowRed[i]);
        arrayObj_g.append(struGsh.sRowGreen[i]);
        arrayObj_b.append(struGsh.sRowBlue[i]);
        arrayObj_i.append(struGsh.sRowInf[i]);
    }

    switch (struCnfp.nSensorDim[layer][view] )
    {
    case 1:
        root["r"] = arrayObj_r;
        break;
    case 2:
        root["r"] = arrayObj_r;
        root["g"] = arrayObj_g;
        break;
    case 3:
        root["r"] = arrayObj_r;
        root["g"] = arrayObj_g;
        root["b"] = arrayObj_b;
        break;
    case 4:
        root["r"] = arrayObj_r;
        root["g"] = arrayObj_g;
        root["b"] = arrayObj_b;
        root["n"] = arrayObj_i;
        break;
    }
    QString dateText = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    root["reportTime"] = QString(dateText).toStdString();
    root["reportFlag"] = reportFlag;

    std::string str = info.write(root);

    if(myHttpFileClient->upLoadData(URL_WAVECAPTURE_UPLOAD,str))
    {
        return SUCCESS;
    }
    return UPLOAD_ERR;
}

/* 吹气次数数据上传 */
int mqttMsgParaseThread::uploadBlowCounts()
{

    if ( myMqttThread->m_connect_name == "") return SYSTEM_ERROR;
    n_uploadBlowCountsFlag = 0;
    Json::Value root;
    Json::Value arrayObj;
    Json::Value arrayObj1;
    Json::Value arrayObj2;
    Json::FastWriter info;
    root["code"] =  myMqttThread->m_connect_name;

    if(struGsh.nRemoteSeriesBlowCount == 1 ||reportFlag == Request_Send)
        root["cmdUuid"] = m_uuid;

    root["channelCount"] = QString::number(struCnfg.nChuteTotal).toStdString();
    root["ejectorCount"] = QString::number(struCnfg.struLayerInfo[struGsh.nLayer].nEjectorsPerChute).toStdString();

    for(int i = 0; i< struCnfg.nChuteTotal*struCnfg.struLayerInfo[struGsh.nLayer].nEjectorsPerChute;i++){

        arrayObj.append(QString::number(struGsh.sBlowTimes[i]).toStdString());

        //struCnfg.totalBlowCount[i] += (myFlow.myNetWork->udpthread->blowCountsTempDataBuf[i])/1000;

        arrayObj1.append(QString::number(struCnfg.totalBlowCount[i]/1000).toStdString());

    }

    root["blowCounts"] = arrayObj;
    arrayObj.clear();

    for(int i = 0; i <struCnfg.nChuteTotal; i++ )
    {
        arrayObj2["channel"] = i;
        arrayObj2["status"] = (int)(struGsh.valveProtectValue[i]);
        arrayObj.append(arrayObj2);
        arrayObj2.clear();
    }
    root["valveProtection"] = arrayObj;
    root["totalblowCounts"] =arrayObj1;
    QString dateText = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    root["reportTime"] = QString(dateText).toStdString();
    root["reportFlag"] = reportFlag;

    std::string str = info.write(root);
    memset(struGsh.sBlowTimes,0,sizeof(struGsh.sBlowTimes));
    if (!struGsh.bFlagMqttConnect)
    {
        root["reportFlag"] = reportFlag+2;
        str = info.write(root);
        cacheLocalRemoteInfo(URL_BLOWCOUNTS_UPLOAD,str);
        return SYSTEM_ERROR;
    }

    if (!myHttpFileClient->upLoadData(URL_BLOWCOUNTS_UPLOAD,str))
    {
        if (!myHttpFileClient->upLoadData(URL_BLOWCOUNTS_UPLOAD,str))
        {
            struGsh.bFlagMqttConnect = 0;
            root["reportFlag"] = reportFlag+2;
            str = info.write(root);
            cacheLocalRemoteInfo(URL_BLOWCOUNTS_UPLOAD,str);
            return SYSTEM_ERROR;
        }
    }

    if(struGsh.nRemoteSeriesBlowCount == 1)
        onCmdReturn(m_uuid,m_tmp,SUCCESS);


    return SUCCESS;
}


/* 服务器请求上传工控机信息 */
void mqttMsgParaseThread::onIpcInfoUpload(string value)
{
}

/* 部件状态+预警上报 */
/* 替代原先接口，加入深度学习参数信息及网络诊断*/
int mqttMsgParaseThread::uploadIpcPartsStatusInfo()
{
    n_uploadPartsStatusFlag = 0;
    if (myHttpFileClient->g_connectName == "")
    {
        return -1;
    }
    Json::Value root;
    Json::Value boards;
    Json::Value value;
    Json::Value lifeValue;
    Json::Value ipcInfo;
    Json::Value arrayObj;
    Json::Value array2Obj;
    Json::FastWriter info;

    int state = 0;
    QString namestr = "";
    int i,j,n;
    arrayObj.clear();
    array2Obj.clear();

    root["code"] = myMqttThread->m_connect_name;
    root["version"]= m_version;

    stru_version version;
    memcpy(&version,&struGsh.struVer,sizeof(version));

    QString m_type;
    QString m_mode;
    QString valuestr;

    m_type = myFlow.getFpgaVersionProduct(version.sInterface[4]);
    if(version.sInterface[2] == 0)
        m_mode = "(F)";
    else
        m_mode = "(U)";

    if (version.nInterfaceRes == 1)
    {
        if(version.sInterface[1] < 10)
            valuestr = QString("%1.0%2.%3%4").arg(QString::number(version.sInterface[0])).arg(QString::number(version.sInterface[1])).arg(m_type).arg(m_mode);
        else
            valuestr = QString("%1.%2.%3%4").arg(QString::number(version.sInterface[0])).arg(QString::number(version.sInterface[1])).arg(m_type).arg(m_mode);

        root["interfaceBoard"] = 0;
        root["interfaceVersion"] = valuestr.toStdString();
    }
    else if(version.nInterfaceRes == 0)
    {
        if(version.sInterface[1] < 10)
            valuestr = QString("%1.0%2.%3%4").arg(QString::number(version.sInterface[0])).arg(QString::number(version.sInterface[1])).arg(m_type).arg(m_mode);
        else
            valuestr = QString("%1.%2.%3%4").arg(QString::number(version.sInterface[0])).arg(QString::number(version.sInterface[1])).arg(m_type).arg(m_mode);

        root["interfaceBoard"] = 1;
        root["interfaceVersion"] = valuestr.toStdString();
    }
    else  // 未查询
    {
        root["interfaceBoard"] = 2;
        root["interfaceVersion"] = "-";
    }
    for(n = 0;n < struCnfg.nLayerTotal;n++)
    {
        for(i = 0;i < struCnfg.struLayerInfo[n].nViewChainTotal;i++)
        {
            if (struCnfg.struLayerInfo[n].nViewIsUsed[i])
            {
                boards["view"] = i;
                for(j = 0;j < struCnfg.struLayerInfo[n].nViewUnitTotal[i];j++){
                    value["id"] = j+1; // 通道

                    //普通相机
                    m_type = myFlow.getFpgaVersionProduct(version.sUnit[n][i][j][4]);
                    if(version.sUnit[n][i][j][2]== 0)
                        m_mode = "(F)";
                    else
                        m_mode = "(U)";
                    state = version.nUnitRes[n][i][j];
                    if (state == 1)
                    {
                        //value["value"] = 0;
                        if(version.sUnit[n][i][j][1] < 10){
                            valuestr = QString("%1.0%2.%3%4").arg(version.sUnit[n][i][j][0]).arg(version.sUnit[n][i][j][1]).arg(m_type).arg(m_mode);
                        }else{
                            valuestr = QString("%1.%2.%3%4").arg(version.sUnit[n][i][j][0]).arg(version.sUnit[n][i][j][1]).arg(m_type).arg(m_mode);
                        }
                        value["value"] = 0;
                        value["version"] = valuestr.toStdString();
                    }
                    else if(state == 0)
                    {
                        if(version.sUnit[n][i][j][1] < 10){
                            valuestr = QString("%1.0%2.%3%4").arg(version.sUnit[n][i][j][0]).arg(version.sUnit[n][i][j][1]).arg(m_type).arg(m_mode);
                        }else{
                            valuestr = QString("%1.%2.%3%4").arg(version.sUnit[n][i][j][0]).arg(version.sUnit[n][i][j][1]).arg(m_type).arg(m_mode);
                        }

                        value["value"] = 1;
                        value["version"] = valuestr.toStdString();
                    }
                    else
                    {
                        value["value"]= 2;
                        value["version"] = "-";
                    }

                    //IPC相机
                    if (struCnfg.nDPMachine)
                    {
                        m_type = myFlow.getFpgaVersionProduct(version.sUnitIpc[n][i][j][4]);

                        if(version.sUnitIpc[n][i][j][2]== 0)
                            m_mode = "(F)";
                        else
                            m_mode = "(U)";

                        if(version.nUnitRes[n][i][j] == -1){
                            valuestr = "-";
                        }else if(version.nUnitRes[n][i][j] == 0){
                            if(version.sUnitIpc[n][i][j][1] < 10){
                                valuestr = QString("%1.0%2.%3%4").arg(version.sUnitIpc[n][i][j][0]).arg(version.sUnitIpc[n][i][j][1]).arg(m_type).arg(m_mode);
                            }else{
                                valuestr = QString("%1.%2.%3%4").arg(version.sUnitIpc[n][i][j][0]).arg(version.sUnitIpc[n][i][j][1]).arg(m_type).arg(m_mode);
                            }
                        }else if(version.nUnitRes[n][i][j] == 1){
                            if(version.sUnitIpc[n][i][j][1] < 10){
                                valuestr = QString("%1.0%2.%3%4").arg(version.sUnitIpc[n][i][j][0]).arg(version.sUnitIpc[n][i][j][1]).arg(m_type).arg(m_mode);
                            }else{
                                valuestr = QString("%1.%2.%3%4").arg(version.sUnitIpc[n][i][j][0]).arg(version.sUnitIpc[n][i][j][1]).arg(m_type).arg(m_mode);
                            }
                        }
                        value["ipcVerison"] = valuestr.toStdString();

                        value["ipcType"] = struCnfg.struIpcCameraParams[n][i][j].ipcType;
                        value["cameraId"] = struCnfg.struIpcCameraParams[n][i][j].cameraId;
                    }

                    arrayObj.append(value);
                }
                boards["unitValues"] = arrayObj;

                //颜色还原目前各视统一
                if (struCnfg.nDPMachine)
                {
                    boards["colorRestoreStatus"] = struCnfp.bColorRestoreEnable;
                    boards["ra"] = struCnfg.struLayerInfo[n].stuCameraInfo[i].nColorRestoreRed[0];
                    boards["rb"] = struCnfg.struLayerInfo[n].stuCameraInfo[i].nColorRestoreRed[1];
                    boards["rc"] = struCnfg.struLayerInfo[n].stuCameraInfo[i].nColorRestoreRed[2];

                    boards["ga"] = struCnfg.struLayerInfo[n].stuCameraInfo[i].nColorRestoreGreen[0];
                    boards["gb"] = struCnfg.struLayerInfo[n].stuCameraInfo[i].nColorRestoreGreen[1];
                    boards["gc"] = struCnfg.struLayerInfo[n].stuCameraInfo[i].nColorRestoreGreen[2];

                    boards["ba"] = struCnfg.struLayerInfo[n].stuCameraInfo[i].nColorRestoreBlue[0];
                    boards["bb"] = struCnfg.struLayerInfo[n].stuCameraInfo[i].nColorRestoreBlue[1];
                    boards["bc"] = struCnfg.struLayerInfo[n].stuCameraInfo[i].nColorRestoreBlue[2];
                }

                arrayObj.clear();
                array2Obj.append(boards);
            }
        }
    }
    root["unitBoards"] = array2Obj;
    boards.clear();
    value.clear();
    array2Obj.clear();

    for(n = 0;n < struCnfg.nLayerTotal;n++)
    {
        for(i = 0;i < struCnfg.struLayerInfo[n].nVavleChainTotal;i++){
            if (struCnfg.struLayerInfo[n].nViewIsUsed[i])
            {
                boards["view"] = i;
                for(j = 0;j < struCnfg.struLayerInfo[n].nVavleBoardTotal[i];j++){
                    value["id"] = j+1;

                    m_type = myFlow.getFpgaVersionProduct(version.sVavle[n][i][j][4]);
                    if(version.sVavle[n][i][j][2]== 0)
                        m_mode = "(F)";
                    else
                        m_mode = "(U)";

                    state = version.nVavleRes[n][i][j];
                    if (state == 1)
                    {
                        value["value"] = 0;
                        if(version.sVavle[n][i][j][1] < 10){
                            valuestr = QString("%1.0%2.%3%4").arg(version.sVavle[n][i][j][0]).arg(version.sVavle[n][i][j][1]).arg(m_type).arg(m_mode);
                        }else{
                            valuestr = QString("%1.%2.%3%4").arg(version.sVavle[n][i][j][0]).arg(version.sVavle[n][i][j][1]).arg(m_type).arg(m_mode);
                        }
                        value["version"] = valuestr.toStdString();
                    }
                    else if(state == 0)
                    {
                        value["value"]= 1;
                        if(version.sVavle[n][i][j][1] < 10){
                            valuestr = QString("%1.0%2.%3%4").arg(version.sVavle[n][i][j][0]).arg(version.sVavle[n][i][j][1]).arg(m_type).arg(m_mode);
                        }else{
                            valuestr = QString("%1.%2.%3%4").arg(version.sVavle[n][i][j][0]).arg(version.sVavle[n][i][j][1]).arg(m_type).arg(m_mode);
                        }
                        value["version"] = valuestr.toStdString();
                    }
                    else
                    {
                        value["value"]= 2;
                        value["version"]= "-";
                    }
                    arrayObj.append(value);
                }
                boards["valveValues"] = arrayObj;
                arrayObj.clear();
                array2Obj.append(boards);
            }
        }
    }
    root["valveBoards"] = array2Obj;
    boards.clear();
    value.clear();
    array2Obj.clear();

    if(struCnfg.nChuteTotal > 1 || (struCnfg.nChuteTotal == 1 && !struCnfg.nAllInOne))
    {
        for(j = 0;j < struCnfg.nCtrlBoardTotal;j++){

            value["id"] = j+1;

            m_type = myFlow.getFpgaVersionProduct(version.sCtrlBoard[j][4]);
            if(version.sCtrlBoard[j][2]== 0)
                m_mode = "(F)";
            else
                m_mode = "(U)";

            state = version.nCtrlRes[j];
            if(state == 1){
                value["value"] = 0;
                if(version.sCtrlBoard[j][1] < 10){
                    valuestr = QString("%1.0%2.%3%4").arg(version.sCtrlBoard[j][0]).arg(version.sCtrlBoard[j][1]).arg(m_type).arg(m_mode);
                }
                else
                {
                    valuestr = QString("%1.%2.%3%4").arg(version.sCtrlBoard[j][0]).arg(version.sCtrlBoard[j][1]).arg(m_type).arg(m_mode);
                }
                value["version"] = valuestr.toStdString();
            }
            else if(state == 0)
            {
                value["value"]= 1;
                if(version.sCtrlBoard[j][1] < 10){
                    valuestr = QString("%1.0%2.%3%4").arg(version.sCtrlBoard[j][0]).arg(version.sCtrlBoard[j][1]).arg(m_type).arg(m_mode);
                }
                else
                {
                    valuestr = QString("%1.%2.%3%4").arg(version.sCtrlBoard[j][0]).arg(version.sCtrlBoard[j][1]).arg(m_type).arg(m_mode);
                }

                value["version"] = valuestr.toStdString();
            }
            else
            {
                value["value"]= 2;
                value["version"]= "-";
            }
            arrayObj.append(value);
        }
        root["ctrlBoards"] = arrayObj;
        arrayObj.clear();
        value.clear();

        for(j = 0;j < struCnfg.stuLayerLampCfg.nLampBoardTotal;j++){
            value["id"] = j+1;

            m_type = myFlow.getFpgaVersionProduct(version.sLampBoard[j][4]);
            if(version.sLampBoard[j][2]== 0)
                m_mode = "(F)";
            else
                m_mode = "(U)";

            state = version.nLampRes[j];
            if(state == 1){
                value["value"] = 0;
                if(version.sLampBoard[j][1] < 10){
                    valuestr = QString("%1.0%2.%3%4").arg(version.sLampBoard[j][0]).arg(version.sLampBoard[j][1]).arg(m_type).arg(m_mode);
                }else{
                    valuestr = QString("%1.%2.%3%4").arg(version.sLampBoard[j][0]).arg(version.sLampBoard[j][1]).arg(m_type).arg(m_mode);
                }
                value["version"]= valuestr.toStdString();
            }
            else if(state == 0){
                value["value"] = 1;
                if(version.sLampBoard[j][1] < 10){
                    valuestr = QString("%1.0%2.%3%4").arg(version.sLampBoard[j][0]).arg(version.sLampBoard[j][1]).arg(m_type).arg(m_mode);
                }else{
                    valuestr = QString("%1.%2.%3%4").arg(version.sLampBoard[j][0]).arg(version.sLampBoard[j][1]).arg(m_type).arg(m_mode);
                }
                value["version"]= valuestr.toStdString();
            }
            else
            {
                value["value"] = 2;
                value["version"]= "-";
            }
            arrayObj.append(value);
        }
        root["lampBoards"] = arrayObj;
        arrayObj.clear();
        value.clear();
    }

    QString dateText = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    root["reportTime"] = QString(dateText).toStdString();
    root["reportFlag"] = reportFlag;

    std::string str = info.write(root);

    //      qDebug()<<"IPC remote info: "<<QString::fromStdString(str);

    if (!struGsh.bFlagMqttConnect)
    {
        root["reportFlag"] = reportFlag+2;
        str = info.write(root);
        cacheLocalRemoteInfo(URL_PARTSTATUSINFO_UPLOAD,str);
    }

    if (!myHttpFileClient->upLoadData(URL_PARTSTATUSINFO_UPLOAD,str))
    {
        root["reportFlag"] = reportFlag+2;
        str = info.write(root);
        cacheLocalRemoteInfo(URL_PARTSTATUSINFO_UPLOAD,str);
        return UPLOAD_ERR;
    }
    return SUCCESS;
}


/************************************** SLOTS *******************************************/
/* 远程采图结束槽函数 */
void mqttMsgParaseThread::imgaeCaptureOver(int imageHeight,QString imagePath)
{
    if (!struGsh.nRemoteImageCaptureIsRun || struGsh.nImageCaptureIsRun) return;
    struGsh.nRemoteImageCaptureIsRun = 0;

    myFlow.myNetWork->udpStop();

    if (!struGsh.bFlagMqttConnect) return;

    if (imageHeight == 0)
    {
        onCmdReturn(m_uuid,m_tmp,IMG_NOT_CORRECT);
        logger()->info("cap %1",imageHeight);
        return;
    }

    QString imageName = QFileInfo(imagePath).baseName();
    QStringList list = imageName.split("_");
    if (list.length() < 4 ) return;

    n_imagePath = imagePath;
    n_uploadImageFlag = 1;
    m_uploadStat = 1;
    return;
}

/* 远程波形采集结束槽函数 */
void mqttMsgParaseThread::waveCaptureOver()
{
    if (!struGsh.nRemoteImageCaptureIsRun || struGsh.nImageCaptureIsRun) return;
    struGsh.nRemoteImageCaptureIsRun = 0;

    myFlow.myNetWork->udpStop();

    n_uploadWaveFlag = 1;
}

/* 吹气次数采集结束槽函数 */
void mqttMsgParaseThread::blowCountsCaptureOver()
{
    qDebug()<<"blowCountsCaptureOver";
    if (!struGsh.nRemoteImageCaptureIsRun || struGsh.nImageCaptureIsRun) return;
    if(struGsh.nRemoteSeriesBlowCount == 0)
    {
        struGsh.nRemoteImageCaptureIsRun = 0;

        myFlow.myNetWork->udpStop();
    }

    n_uploadBlowCountsFlag = 1;



}
void mqttMsgParaseThread::DealStartRemoteControl(QString phonenum)
{
    QTimer timer_start;
    timer_start.setSingleShot(true);
    myMessageBox msgBox(MSG_QUES, g_myLan().remote_control);
    myMessageBox msgBox_1(MSG_QUES, g_myLan().remote_control +"\n"+phonenum);
    QObject::connect(&timer_start, SIGNAL(timeout()), &msgBox, SLOT(hide())); //
    QObject::connect(&timer_start, SIGNAL(timeout()), &msgBox, SLOT(hide())); //

    g_MainManager().ShowWidget(SM_REMOTE_CONTROL_WIDGET); // 跳转到确认界面
}

void mqttMsgParaseThread::DealSeriesBlowCount(int total_time, int cmd_fre)
{
    int time = total_time;
    struGsh.nRemoteSeriesBlowCount = 1;
    struGsh.nRemoteImageCaptureIsRun = 1;
    myFlow.myNetWork->setUdpRecvType(CAPTURE_BLOW_COUNTS);
    myFlow.myNetWork->setBlowQueryFreq(cmd_fre); // 每2秒采集
    myFlow.myNetWork->udpStart();
    while(time > 0)
    {
        sleep(cmd_fre);
        time = time - cmd_fre;
    }
    struGsh.nRemoteSeriesBlowCount = 0;
    myFlow.myNetWork->setBlowQueryFreq(0);
    myFlow.myNetWork->udpStop();
    reportFlag = Normal_Send;
}
/************************************* SLOTS END ****************************************/

//获取模型列表
int mqttMsgParaseThread::getModeList()
{

    n_getModelListFlag = 0;
    Json::Value root;
    Json::Value labellist;
    Json::Value arrayObj;
    Json::Value arrayObj2;
    Json::FastWriter info;


    root["collectFlag"] = 0;
    root["keyword"] = QString("%1").arg(struCnfg.stumodeLableSelect.keyword).toStdString();

    int j = 0;
    for(int i = 0; i < MAX_MODE_BIGLABLE_NUM ; i++)
    {
        if(struCnfg.stumodeLableSelect.labelindex[i] == 0)
            continue;


        j++;
        int index = struCnfg.stumodeLableSelect.labelindex[i];

        arrayObj["value"] =  QString("%1").arg(struCnfg.stumodeLableList.labelinfo[i].labelID[index-1]).toStdString();
        arrayObj["labelType"] =  QString("%1").arg(struCnfg.stumodeLableList.labelinfo[i].labeltype[index-1]).toStdString();

        labellist.append(arrayObj);
        arrayObj.clear();
    }


    if(j == 0)
    {
        labellist = Json::Value(Json::arrayValue);

    }
    root["labelList"] = labellist;
    arrayObj["column"] = "used_Num";
    arrayObj["asc"] = false;

    arrayObj2.append(arrayObj);
    root["orders"] = arrayObj2;

    std::string str = info.write(root);

    if(myHttpFileClient->requestModelList(URL_MODEL_LIST + QString::number(struGsh.currentpageid) + "&size=" + QString::number(MAX_MODE_NUM_LIST) ,str))
    {

        return SUCCESS;
    }
    else
    {
        return SYSTEM_ERROR;
    }



}

//评价功能
int mqttMsgParaseThread::uploadModelEvaluate()
{
    n_uploadModelEvaluate = 0;


    Json::Value root;
    root["modelName"] = QString("%1").arg(struGsh.stumodelData.modelname[struGsh.stumodelData.checkindex]).toStdString();

    Json::FastWriter info;


    std::string str = info.write(root);

    if(myHttpFileClient->upLoadData(URL_MODEL_EVALUATE,str))
    {

        return SUCCESS;
    }
    else
    {
        return SYSTEM_ERROR;
    }



}

void mqttMsgParaseThread::uploadRemoteControl()
{
    n_uploadRemoteControlFlag = 0;
    onCmdReturn(remotelock_uuid,string("CMD_RemoteControl"),struGsh.remoteControlstatus);
}

int mqttMsgParaseThread::onModelParamsSet(string value)
{


    if(struCnfg.nUseIpcEnable == 1)
    {
        if(struIpcShare.allIpcModelLoadOk != 2 )
        {

            return IPC_CONNECT_FAIL;
        }
    }


    Json::Reader reader;
    Json::Value root;
    value ="{\"modelParams\":"+value+"}";

    qDebug()<<"onModelParamsSet"<<QString::fromStdString(value);

    if (reader.parse(value, root)) // reader将Json字符串解析到root，root将包含Json里所有子元素
    {
        int layernum = root["modelParams"].size();

        for(int i = 0; i < layernum; i++)
        {

            int layer =  root["modelParams"][i]["layerId"].asInt();
            int viewnum = root["modelParams"][i]["layerParams"].size();
            for(int j = 0; j < viewnum; j++)
            {

                int view = root["modelParams"][i]["layerParams"][j]["viewId"].asInt();
                int groupnum = root["modelParams"][i]["layerParams"][j]["viewParams"].size();
                for(int m = 0; m < groupnum; m++)
                {

                    int classnum =  root["modelParams"][i]["layerParams"][j]["viewParams"][m]["classCount"].asInt();
                    int group =  root["modelParams"][i]["layerParams"][j]["viewParams"][m]["groupId"].asInt();
                    for(int n =0; n < classnum; n++)
                    {

                        int classid = root["modelParams"][i]["layerParams"][j]["viewParams"][m]["classParams"][n]["classId"].asInt();

                        int state = root["modelParams"][i]["layerParams"][j]["viewParams"][m]["classParams"][n]["state"].asInt();

                        int classSens = root["modelParams"][i]["layerParams"][j]["viewParams"][m]["classParams"][n]["classSens"].asInt();

                        int classThres = root["modelParams"][i]["layerParams"][j]["viewParams"][m]["classParams"][n]["classThres"].asInt();

                        struCnfp.struCameraIpcClassGroupParams[view][group].classEnable[classid] = state;
                        struCnfp.struCameraIpcClassGroupParams[view][group].classSens[classid] = classSens;
                        struCnfp.struCameraIpcClassGroupParams[view][group].classThreshold[classid] = classThres;
                    }
                }
            }
        }
    }


    for(int i = 0;i < struCnfp.ipcModelClassCount[struGsh.nView][struGsh.nMatGroup]; i++){
        if(struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classMode[i] == 1){
            if(struCnfp.struCameraIpcClassGroupParams[struGsh.nView][struGsh.nMatGroup].classLabelBox[i] > 24){
                return CMD_PARA_ERROR;
            }
        }
    }

    switch(struCnfp.nIpcCameraClassSameMode)
    {
    case 0:
        break;
    case 1: //前后相同
        for(int i = 0; i < struCnfg.struLayerInfo[0].nViewChainTotal; i++)
        {
            for(int j = 0; j < struCnfg.struLayerInfo[0].stuViewInfo[i].nIdentifyGroupTotal; j++)
            {
                for(int m = 0; m < MAX_IPC_MODEL_CLASS; m++)
                {
                    struCnfp.struCameraIpcClassGroupParams[i][j].classEnable[m] = struCnfp.struCameraIpcClassGroupParams[i/2*2][j].classEnable[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classSens[m] = struCnfp.struCameraIpcClassGroupParams[i/2*2][j].classSens[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classThreshold[m] = struCnfp.struCameraIpcClassGroupParams[i/2*2][j].classThreshold[m];

                    struCnfp.struCameraIpcClassGroupParams[i][j].classLabelBox[m] = struCnfp.struCameraIpcClassGroupParams[i/2*2][j].classLabelBox[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classLabelBox2[m] = struCnfp.struCameraIpcClassGroupParams[i/2*2][j].classLabelBox2[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classTickMode[m] = struCnfp.struCameraIpcClassGroupParams[i/2*2][j].classTickMode[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classFixMode[m] = struCnfp.struCameraIpcClassGroupParams[i/2*2][j].classFixMode[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classMode[m] = struCnfp.struCameraIpcClassGroupParams[i/2*2][j].classMode[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classDelayMode[m] = struCnfp.struCameraIpcClassGroupParams[i/2*2][j].classDelayMode[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classHWRatio[m] = struCnfp.struCameraIpcClassGroupParams[i/2*2][j].classHWRatio[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classPosAdjust[m] = struCnfp.struCameraIpcClassGroupParams[i/2*2][j].classPosAdjust[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classBD[m] = struCnfp.struCameraIpcClassGroupParams[i/2*2][j].classBD[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classBX[m] = struCnfp.struCameraIpcClassGroupParams[i/2*2][j].classBX[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].selRerverse[m] = struCnfp.struCameraIpcClassGroupParams[i/2*2][j].selRerverse[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].selLimit1[m] = struCnfp.struCameraIpcClassGroupParams[i/2*2][j].selLimit1[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].selLimit2[m] = struCnfp.struCameraIpcClassGroupParams[i/2*2][j].selLimit2[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classANDEnable[m] = struCnfp.struCameraIpcClassGroupParams[i/2*2][j].classANDEnable[m];
                    memcpy(struCnfp.struCameraIpcClassGroupParams[i][j].className[m],struCnfp.struCameraIpcClassGroupParams[i/2*2][j].className[m],sizeof(struCnfp.struCameraIpcClassGroupParams[0][0].className[m]));
                    //qDebug()<<"view"<<i<<"group"<<j<<"model"<<m<<"checkd"<<struCnfp.struCameraIpcClassGroupParams[i][j].classEnable[m]<<struCnfp.struCameraIpcClassGroupParams[i/2*2][j].classEnable[m];
                }
            }
        }
        break;
    case 2:
        for(int i = 0; i < struCnfg.struLayerInfo[0].nViewChainTotal; i++)
        {
            for(int j = 0; j < struCnfg.struLayerInfo[0].stuViewInfo[i].nIdentifyGroupTotal; j++)
            {
                for(int m = 0; m < MAX_IPC_MODEL_CLASS; m++)
                {
                    struCnfp.struCameraIpcClassGroupParams[i][j].classEnable[m] = struCnfp.struCameraIpcClassGroupParams[0][0].classEnable[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classSens[m] = struCnfp.struCameraIpcClassGroupParams[0][0].classSens[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classThreshold[m] = struCnfp.struCameraIpcClassGroupParams[0][0].classThreshold[m];

                    struCnfp.struCameraIpcClassGroupParams[i][j].classLabelBox[m] = struCnfp.struCameraIpcClassGroupParams[0][0].classLabelBox[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classLabelBox2[m] = struCnfp.struCameraIpcClassGroupParams[0][0].classLabelBox2[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classTickMode[m] = struCnfp.struCameraIpcClassGroupParams[0][0].classTickMode[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classFixMode[m] = struCnfp.struCameraIpcClassGroupParams[0][0].classFixMode[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classMode[m] = struCnfp.struCameraIpcClassGroupParams[0][0].classMode[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classDelayMode[m] = struCnfp.struCameraIpcClassGroupParams[0][0].classDelayMode[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classHWRatio[m] = struCnfp.struCameraIpcClassGroupParams[0][0].classHWRatio[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classPosAdjust[m] = struCnfp.struCameraIpcClassGroupParams[0][0].classPosAdjust[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classBD[m] = struCnfp.struCameraIpcClassGroupParams[0][0].classBD[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classBX[m] = struCnfp.struCameraIpcClassGroupParams[0][0].classBX[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].selRerverse[m] = struCnfp.struCameraIpcClassGroupParams[0][0].selRerverse[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].selLimit1[m] = struCnfp.struCameraIpcClassGroupParams[0][0].selLimit1[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].selLimit2[m] = struCnfp.struCameraIpcClassGroupParams[0][0].selLimit2[m];
                    struCnfp.struCameraIpcClassGroupParams[i][j].classANDEnable[m] = struCnfp.struCameraIpcClassGroupParams[0][0].classANDEnable[m];
                    memcpy(struCnfp.struCameraIpcClassGroupParams[i][j].className[m],struCnfp.struCameraIpcClassGroupParams[0][0].className[m],sizeof(struCnfp.struCameraIpcClassGroupParams[0][0].className[m]));
                }
            }
        }
        break;
    }

    myFlow.configIpcAIPara(0,0);
    myFlow.configIpcClassParams(true,0,0);


    return SUCCESS;
}


int mqttMsgParaseThread::onModelParamsUpload()
{
    n_uploadModelParamsFlag = 0;
#if 1
    if (myMqttThread->m_connect_name == "") return SYSTEM_ERROR;

    Json::Value root;
    Json::Value precisionParams;
    Json::Value groupParam;
    Json::Value viewParam;
    Json::Value layerParam;
    Json::Value classParam;
    Json::Value arrayobj;
    Json::Value array2obj;
    Json::Value array3obj;
    Json::FastWriter info;
    for(int i=0;i<struCnfg.nLayerTotal;i++)
    {
        switch (i){
        case 0:
            layerParam["layerName"] = QString(g_myLan().lower_layer).toStdString();
            layerParam["layerId"] = 0;
            break;
        case 1:
            layerParam["layerName"] = QString(g_myLan().upper_layer).toStdString();
            layerParam["layerId"] = 1;
            break;
        }
        for( int k=0;  k< MAX_VIEW; k++ )
        {
            if (!struCnfg.struLayerInfo[i].nViewIsUsed[k]) continue;

            int viewId = struCnfg.struLayerInfo[i].nViewId[k];
            switch (viewId){
            case 0:
                viewParam["viewName"] = QString(g_myLan().ids_fm).toStdString();
                viewParam["viewId"] = 0;
                break;
            case 1:
                viewParam["viewName"] = QString(g_myLan().ids_bm).toStdString();
                viewParam["viewId"] = 1;
                break;
            case 2:
                viewParam["viewName"] = QString(g_myLan().ids_fs).toStdString();
                viewParam["viewId"] = 2;
                break;
            case 3:
                viewParam["viewName"] = QString(g_myLan().ids_bs).toStdString();
                viewParam["viewId"] = 3;
                break;
            case 4:
                viewParam["viewName"] = QString("II"+g_myLan().ids_fm).toStdString();
                viewParam["viewId"] = 4;
                break;
            case 5:
                viewParam["viewName"] = QString("II"+g_myLan().ids_bm).toStdString();
                viewParam["viewId"] = 5;
                break;
            case 6:
                viewParam["viewName"] = QString("II"+g_myLan().ids_fs).toStdString();
                viewParam["viewId"] = 6;
                break;
            case 7:
                viewParam["viewName"] = QString("II"+g_myLan().ids_bs).toStdString();
                viewParam["viewId"] = 7;
                break;
            default:
                break;
            }
            viewParam["viewGroup"] = struCnfg.struLayerInfo[i].stuViewInfo[k].nIdentifyGroupTotal;
            for(int j=0;j<struCnfg.struLayerInfo[i].stuViewInfo[k].nIdentifyGroupTotal;j++)
            {
                groupParam["groupId"] = j;
                QByteArray modelName(struCnfp.sCurModelName[k][j]);

                groupParam["modelName"] = QString(modelName).toStdString();
                groupParam["classCount"] = struCnfp.ipcModelClassCount[k][j];
                stu_ipc_class_sens GroupParams ;
                memcpy(&GroupParams, &struCnfp.struCameraIpcClassGroupParams[k][j],sizeof(stu_ipc_class_sens) );
                for(int m = 0; m < struCnfp.ipcModelClassCount[k][j]; m++)
                {
                    classParam["classId"] = m;
                    QByteArray name(GroupParams.className[m]);
                    classParam["className"] = QString(name).toStdString();
                    classParam["step"] = 1;
                    classParam["state"] = GroupParams.classEnable[m];
                    classParam["maxValue"] = 100;
                    classParam["minValue"] = 0;
                    classParam["classThres"] = GroupParams.classThreshold[m];
                    classParam["classSens"] = GroupParams.classSens[m];
                    precisionParams.append(classParam);
                    classParam.clear();
                }
                groupParam["classParams"] = precisionParams;
                precisionParams.clear();
                arrayobj.append(groupParam);
                groupParam.clear();
            }//end
            viewParam["viewParams"] = arrayobj;
            arrayobj.clear();
            array2obj.append(viewParam);
            viewParam.clear();
        }//end
        layerParam["layerParams"] = array2obj;
        array2obj.clear();
        array3obj.append(layerParam);
    }
    root["modelParams"] = array3obj;
    array3obj.clear();
    root["code"] = myMqttThread->m_connect_name;
    QString dateText = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    root["reportTime"] = QString(dateText).toStdString();
    root["reportFlag"] = reportFlag;
    std::string str = info.write(root);

    //qDebug()<<QString::fromStdString(str);
    if (!struGsh.bFlagMqttConnect)
    {
        root["reportFlag"] = reportFlag+2;
        str = info.write(root);
        cacheLocalRemoteInfo(URL_MODELPARAMS_UPLOAD,str);
        return NETWORK_ERR;
    }
    if (!myHttpFileClient->upLoadData(URL_MODELPARAMS_UPLOAD,str))
    {
        root["reportFlag"] = reportFlag+2;
        str = info.write(root);
        cacheLocalRemoteInfo(URL_MODELPARAMS_UPLOAD,str);
        return UPLOAD_ERR;
    }
    return SUCCESS;
#endif
}

/* 一键检修上报 */
void mqttMsgParaseThread::uploadOneKeyFixInfo()
{
    n_uploadOneKeyFixFlag = 0; //上传完成后 复位0
    if (myHttpFileClient->g_connectName == "")
    {
        return;
    }

    QSettings setting(CFG_APPSet,QSettings::IniFormat);
    QString temp;

    Json::Value root;
    root.clear();
    Json::FastWriter info;

    root["code"] = myMqttThread->m_connect_name;

    QMap<int,string> m_machineTypeMap;
    m_machineTypeMap.clear();
    m_machineTypeMap.insert(MACHINE_VERTICAL_ZL_CV,"ZL_LS");
    m_machineTypeMap.insert(MACHINE_LD_ZL_GP,"ZL_LD");
    m_machineTypeMap.insert(MACHINE_LD2_ZL_GP,"ZL_LDS");
    root["model"] = m_machineTypeMap.value(struCnfg.nMachineType);

    temp = setting.value("name","").toString();
    root["name"] = temp.toStdString();

    temp = setting.value("customerName","").toString();
    root["customerName"] = temp.toStdString();

    temp = setting.value("customPhone","").toString();
    root["mobile"]  = temp.toStdString();

    temp  = setting.value("Province","").toString();
    temp += setting.value("City","").toString();
    temp += setting.value("County","").toString();
    temp += setting.value("DetialPlace","").toString();
    root["address"]  = temp.toStdString();

    temp = setting.value("customQCnum","").toString();
    root["qc"]  = temp.toStdString();

    temp = setting.value("extModel","").toString();
    root["extModel"]  = temp.toStdString();
    /*---------------------------------------------------------------------*/


    QString dateText = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    root["reportTime"] = QString(dateText).toStdString();
    root["reportFlag"] = reportFlag;

    std::string str = info.write(root);
    qDebug()<<QString::fromStdString(str);

    if (!struGsh.bFlagMqttConnect)
    {
        //        root["reportFlag"] = reportFlag+2;
        //        str = info.write(root);
        //        cacheLocalRemoteInfo(URL_ONEKEYFIX_UPLOAD,str);
        return;
    }
    if (!myHttpFileClient->upLoadData(URL_ONEKEYFIX_UPLOAD,str))
    {
        //        root["reportFlag"] = reportFlag+2;
        //        str = info.write(root);
        //        cacheLocalRemoteInfo(URL_ONEKEYFIX_UPLOAD,str);
    }
}


int mqttMsgParaseThread::onSeriesBlowCounts(int total_time,int cmd_fre)
{
    int errorcode = 0;
    if( total_time <= 0 || cmd_fre <= 0 || total_time < cmd_fre
            || myFlow.myNetWork->udpthread->isrunning )
        return SYSTEM_ERROR;

    emit SeriesBlowCount(total_time, cmd_fre);

    return errorcode;
}

int mqttMsgParaseThread::offlineDetection(string cmd_value)
{
    QList<int> offlineresult;
    Json::Value resultlist;
    offlineresult.clear();
    Json::Value arrayObj;
    Json::Reader reader;
    arrayObj.clear();
    resultinfo.clear();
    resultlist.clear();
    int errorcode = 0;

    string cmd_string = "{\"cmd\":"+cmd_value+"}";
    reader.parse( cmd_string.c_str() , arrayObj ); // string 转换成　json数据
    logger()->info("offlineDetection cmd num:" + QString::number( arrayObj["cmd"].size() ) );

    int offlinedetectioncmdnum = arrayObj["cmd"].size();
    if(offlinedetectioncmdnum == 0 )
    {
        return CMD_PARA_ERROR;
    }

    struGsh.offlineDetection = 1;
    for(int i = 0; i < offlinedetectioncmdnum; i++)
    {
        resultlist.clear();
        msleep(30);
        string cmdstr = arrayObj["cmd"][i]["cmdCode"].asString();  // 解析
        qDebug()<<"cmd type:"<<QString::fromStdString( cmdstr );
        string value =  arrayObj["cmd"][i]["content"].asString();
        qDebug()<<"cmd value:"<<QString::fromStdString(value);
        if(cmdstr == ("CMD_BlowCountsUpload") )
        {
            errorcode = onBlowCountsUpload();
        }
        else if( cmdstr == ("CMD_CaptureImage") || cmdstr  == ("CMD_CaptureWave"))
        {
            errorcode = onCaptureImage(value,cmdstr);
        }
        else if(cmdstr == "CMD_LogfileUpload")
        {
            QDateTime  dateTime = QDateTime::currentDateTime();
            int year = dateTime.date().year();
            int month = dateTime.date().month();
            int day = dateTime.date().day();
            QString time_value = QString::number(year)+"-"+QString::number(month)+"-"
                    +QString::number(day);
            errorcode = uploadLogFile(time_value.toStdString());
        }
        else if(cmdstr == "CMD_CnffileUpload")
        {
            errorcode = onCnffileUpload();
            QString tarCmd = "cd /sdcard; rm -rf *.tgz"; // 清空所有压缩文件
            g_Runtime().mySystem(tarCmd);
        }
        else if(cmdstr == "CMD_PartsStatusInfoUpload")
        {
            errorcode = uploadPartsStatusInfo();
        }
        else if(cmdstr == "CMD_StatisticsUpload")
        {
            errorcode = UNKNOWN_CMD;
        }
        else if (cmdstr == string("CMD_WipeSpeedUpload"))
        {
            errorcode = onWipeSpeedUpload();
        }
        else if(cmdstr  == "CMD_CaptureAllImage")
        {
            if(myFlow.myNetWork->udpthread->isrunning)
            {
                errorcode = SYSTEM_ERROR;
                continue;
            }
            errorcode = onCaptureAllImage();
        }
        else if(cmdstr == "CMD_CaptureAllWave")
        {
            if(myFlow.myNetWork->udpthread->isrunning)
            {
                errorcode = SYSTEM_ERROR;
                continue;
            }
            errorcode = onCaptureAllWave();
        }
        else  // 未识别的指令
        {
            errorcode = MACHINE_MISMATCH;
        }

        resultlist["result"] = errorcode;
        if(errorcode == SUCCESS)
        {
            offlineresult.append(0);
        }
        else
        {
            offlineresult.append(1);
        }
        resultlist["cmdCode"] = cmdstr;
        resultlist["content"] = value;
        resultlist["uuid"] = m_uuid;
        resultinfo.append(resultlist);
    }
    Json::FastWriter info;
    std::string str = info.write(resultinfo);
    qDebug()<<QString::fromStdString(str);
    struGsh.offlineDetection = 0;
    if(offlineresult.count(0) == offlineresult.size())
    {
        return SUCCESS;
    }
    else if(offlineresult.count(1) == offlineresult.size())
    {
        return CAPTURE_ERROR;
    }
    else
    {
        return PARTIAL_SUCCESS;
    }
}
void mqttMsgParaseThread::onRemoteOneKeyTest(string cmd_value)
{
    onekeytest.clear();
    Json::Value arrayObj;
    Json::Value arrayObj1;
    int errorcode = 0;
    QString strVec = QString().fromStdString(cmd_value);
    qDebug()<<QString().fromStdString(cmd_value);;
    QStringList vecList = strVec.split(":");
    QString cmdstr;
    if (vecList.size() == 0)
    {
        return ;
    }
    struGsh.remoteOnekeytest = 1;
    for(int i = 0; i < vecList.size(); i++)
    {
        arrayObj["cmdName"] = vecList.at(i).toStdString();
        cmdstr = vecList.at(i);
        qDebug()<<cmdstr;
        if(cmdstr  == "CMD_CaptureAllImage")
        {
            if(myFlow.myNetWork->udpthread->isrunning)
            {
                arrayObj["cmdResult"] = -1;
                continue;
            }
            errorcode = onCaptureAllImage();
            if(errorcode == 0)
                arrayObj["cmdResult"] =  0;  // 执行成功
            else
                arrayObj["cmdResult"] = -1; // 执行失败
        }
        else if(cmdstr == "CMD_CaptureAllWave")
        {
            if(myFlow.myNetWork->udpthread->isrunning)
            {
                arrayObj["cmdResult"] = -1;
                continue;
            }
            errorcode = onCaptureAllWave();
            if(errorcode == 0)
                arrayObj["cmdResult"] =  0;  // 执行成功
            else
                arrayObj["cmdResult"] = -1; // 执行失败
        }
        else if(cmdstr == "CMD_LogfileUpload")
        {
            QDateTime  dateTime = QDateTime::currentDateTime();
            int year = dateTime.date().year();
            int month = dateTime.date().month();
            int day = dateTime.date().day();
            QString time_value = QString::number(year)+"-"+QString::number(month)+"-"
                    +QString::number(day);
            errorcode = uploadLogFile(time_value.toStdString());
            if(errorcode == 0)
                arrayObj["cmdResult"] =  0;  // 执行成功
            else
                arrayObj["cmdResult"] = -1; // 执行失败
        }
        else if(cmdstr == "CMD_CnffileUpload")
        {
            errorcode = onCnffileUpload();
            QString tarCmd = "cd /sdcard; rm -rf *.tgz"; // 清空所有压缩文件
            g_Runtime().mySystem(tarCmd);
            if(errorcode == SUCCESS)
                arrayObj["cmdResult"] =  0;
            else
                arrayObj["cmdResult"] = -1;
        }
        else if(cmdstr == "CMD_PartsStatusInfoUpload")
        {
            errorcode = uploadPartsStatusInfo();
            if(errorcode == SUCCESS)
                arrayObj["cmdResult"] =  0;
            else
                arrayObj["cmdResult"] = -1;
        }
        else
        {
            arrayObj["cmdResult"] = 1; // 未执行
        }
        arrayObj1.append(arrayObj);
        arrayObj.clear();
    }

    onekeytest["code"] = myMqttThread->m_connect_name;
    onekeytest["cmdUuid"] = m_uuid;
    onekeytest["cmdTest"] = arrayObj1;
    uploadOnekeytest();
    struGsh.remoteOnekeytest = 0;
    return ;
}
void mqttMsgParaseThread::uploadOnekeytest()
{
    int errorcode = 0;
    Json::FastWriter info;
    QString dateText = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    onekeytest["reportTime"] = QString(dateText).toStdString();
    onekeytest["reportFlag"] = reportFlag;
    std::string str = info.write(onekeytest);
    qDebug()<<QString::fromStdString(str);
    if (!struGsh.bFlagMqttConnect)
    {
        onekeytest["reportFlag"] = reportFlag+2;
        str = info.write(onekeytest);
        cacheLocalRemoteInfo(URL_ONEKEYTEST_UPLOAD,str);
        errorcode = SYSTEM_ERROR;
    }
    if (!myHttpFileClient->upLoadData(URL_ONEKEYTEST_UPLOAD,str))
    {
        if (!myHttpFileClient->upLoadData(URL_ONEKEYTEST_UPLOAD,str))
        {
            struGsh.bFlagMqttConnect = 0;
            onekeytest["reportFlag"] = reportFlag+2;
            str = info.write(onekeytest);
            cacheLocalRemoteInfo(URL_ONEKEYTEST_UPLOAD,str);
            errorcode = SYSTEM_ERROR;
        }
    }
    onCmdReturn(m_uuid,"CMD_RemoteOneKeyTest",errorcode);
    reportFlag = Normal_Send;
}
#endif
