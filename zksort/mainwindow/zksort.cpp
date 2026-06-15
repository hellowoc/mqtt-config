#include "zksort.h"
#include "ui_zksort.h"
#include "global.h"
#include <QFile>
#include <QKeyEvent>
#include "downloaddialog.h"

zkApplication::zkApplication(int &argc, char **argv):
    QApplication(argc,argv)
{
}

ZKSort::ZKSort(QWidget *parent) :
    BaseUi(parent),
    ui(new Ui::ZKSort)
{
    ui->setupUi(this);

    ui->bottomWidget->showPage(true);
    ui->bottomWidget->hide();
    ui->topWidget->showPage(true);
    setWindowFlags(Qt::FramelessWindowHint);
    LoadSkin();
    g_MainManager().setStackedWidget(ui->ContainerStackedWidget,this);
    //dccrypt error

    QSettings setting(CFG_APPSet,QSettings::IniFormat);
    int onoff_cnts =  setting.value("onoffcnts",0).toInt();
    setting.setValue("onoffcnts",++onoff_cnts);
    setting.sync();
    logger()->info("power on: %1times",onoff_cnts);

    QByteArray homeEnv = qgetenv("TSLIB_CALIBFILE");
    if (!homeEnv.isEmpty()) {
        QString califile = g_Runtime().mySystemStr("cat "+homeEnv);
        logger()->info("CALIBFILE: %1", califile);
    } else {
        logger()->info("NO CALIBFILE");
    }

    mytimer = new QTimer(this);
    connect(mytimer,SIGNAL(timeout()),this,SLOT(dccryptCheck()));
    mytimer->start(100);
    mytimerWaitFpgaMode = new QTimer(this);
    connect(mytimerWaitFpgaMode,SIGNAL(timeout()),this,SLOT(waitSetFpgaMode()));
    m_hideBottomPageId.append(SM_MACHINE_DCCRYPT_WIDGET);
    m_hideBottomPageId.append(SM_SELF_CHECK_WIDGET);
    m_hideBottomPageId.append(SM_MAIN_PAGE_WIDGET_NEW);
    m_hideBottomPageId.append(SM_CREATE_MODE_WIDGET);
    m_hideBottomPageId.append(SM_MAIN_PAGE_WIDGET);
    m_hideBottomPageId.append(SM_SVM_IMAGE_WIDGET);
    m_hideTopPageId.append(SM_SVM_IMAGE_WIDGET);
}

ZKSort::~ZKSort()
{
    delete ui;
}

void ZKSort::communication()
{
    if(struGsh.serialPortOpenState == -1)
        return;
    for(int view = 0;view<MAX_VIEW;view++)
    {
        if(struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[view])
        {
            struGsh.nView = view;
            break;
        }
    }


    if(myFlow.startWithoutCnf == 1){
        logger()->info("start without cnf");
        myMessageBox *mesBox = new myMessageBox(MSG_QUES, g_myLan().msg_reset_machine_info);
         int ret = mesBox->exec();
         if(ret == QDialog::Accepted){
             startEnterMainPage();
             return;
         }
    }

    if(QString(SCREEN_VERSION).contains("test"))
    {
        g_infoWidget().setLabelText(g_myLan().Test_Version);
        g_infoWidget().delayShow();
        struGsh.bRunMode = RUNMODE_ADMIN;
    }
    else
    {
        g_infoWidget().setLabelText(g_myLan().msg_communicating);
        g_infoWidget().delayShow();

#ifdef Q_OS_UNIX
        if(1 == myFlow.dccryptCodeTimeGet())
        {
            myFlow.sleep(20);
        }
#endif
    }

    int ret2 = myFlow.resetCommunication();
    struCnfg.nBoardStatusAlarm = ret2;
//    if(struGsh.bFpgaModeAutoSwitchErr == 1){
//        myFlow.resetFpgaRunModeRG();
//        mytimerWaitFpgaMode->start(20000);
    if(struGsh.bInterModeSwitchErr || struGsh.bControlModeSwitchErr || struGsh.bLampAutoSwitchErr ||
            struGsh.bValveAutoSwitchErr || struGsh.bFpgaModeAutoSwitchErr)
    {
        if(struGsh.bInterModeSwitchErr == 1)
        {
            myFlow.resetInterfaceRunModeRG();
    }
        if(struGsh.bControlModeSwitchErr == 1)
        {
            myFlow.resetControlRunModeRG();
        }
        if(struGsh.bLampAutoSwitchErr == 1)
        {
            myFlow.resetLampRunModeRG();
        }
        if(struGsh.bValveAutoSwitchErr == 1)
        {
            myFlow.resetValveRunModeRG();
        }
        if(struGsh.bFpgaModeAutoSwitchErr == 1)
    {

            for(int i = 0 ; i < MAX_VIEW; i++)
            {
                if(struCnfg.struLayerInfo[0].nViewIsUsed[i] == 0)
                    continue;
                for (int j =0; j<struCnfg.struLayerInfo[0].nViewUnitTotal[i]; j++)
                {
                    int n = struCnfg.struLayerInfo[0].nViewUnitTotal[i]- j-1;
                    if(struGsh.cameraResult[0][i][n] == 1)
                    {
                        struGsh.struVer.sUnitbefore[0][i][n] = struGsh.struVer.sUnit[0][i][n][2]; // 记录可查到版本且用户模式不匹配的相机
                        myFlow.switchUserMirror(board_camera<<4|FACTORY_MODE,myFlow.getBoardAddr(board_camera,i+1),n+1,0);
                        logger()->info("view:%1 unit:%2 usermode:%3 is no match ",i,n,struGsh.struVer.sUnitbefore[0][i][n]);
                        qDebug()<<"change factory"<<i<<n;
                        myFlow.msleep(100);
                    }
                    else if(struGsh.cameraResult[0][i][n] == 0)
                    {
                        logger()->info("view:%1 unit:%2 is factorymode",i,n);
                    }
                    else if(struGsh.cameraResult[0][i][n] == -1)
                    {
                        logger()->info("view:%1 unit:%2 version unknown",i,n);
                    }
                }
            }
            myFlow.sleep(5);
        myFlow.resetFpgaRunModeRG();
            myFlow.sleep(10);
            myFlow.resetCommunication();
            if(struGsh.bFpgaModeAutoSwitchErr == 1)
            {
                qDebug()<<"resetFpgaBeforeRunModeRG";
                myFlow.resetFpgaBeforeRunModeRG();
            }
        }
        mytimerWaitFpgaMode->start(20000);
    }
    else{
        g_infoWidget().hide();
        if((CUSTOM == CUSTOM_YANSI  ||ret2 == 0||  struCnfg.nPlcCtrl == 1 || struCnfg.nDefaultPlcCtrl == 1))
        {
            startEnterMainPage();
        }
        else
        {
            g_MainManager().ShowWidget(SM_SELF_CHECK_WIDGET);
        }
    }
}

void ZKSort::waitSetFpgaMode()
{
    mytimerWaitFpgaMode->stop();
    if(struGsh.bFpgaModeAutoSwitchErr == 1)
        g_infoWidget().setLabelText(g_myLan().ids_reset);
    else if(struGsh.bFpgaModeAutoSwitchErr == 2)
        g_infoWidget().setLabelText(g_myLan().ChangeUserMode);
    myFlow.resetMachineConfigRG();
    myFlow.resetSensorConfigRG();
    int ret2 = myFlow.resetCommunication();
    struCnfg.nBoardStatusAlarm = ret2;
    g_infoWidget().hide();
    if(ret2 == 0 || struCnfg.nPlcCtrl == 1 || struCnfg.nDefaultPlcCtrl == 1)
    {
        startEnterMainPage();
    }
    else
    {
        g_MainManager().ShowWidget(SM_SELF_CHECK_WIDGET);
    }

}
void ZKSort::infoStartValve()
{
//    QTimer timer;
//    timer.setSingleShot(true);

//    myMessageBox msgBox(MSG_QUES, "?"+g_myLan().ids_cycleTest+"?");

//    QObject::connect(&timer, SIGNAL(timeout()), &msgBox, SLOT(hide()));
//    timer.start(5000);
//    int ret = msgBox.exec();

//    if (timer.isActive())
//    {
//        timer.stop();
//        if (ret == QDialog::Accepted){ //用户操作，同意进行
//            g_MainManager().ShowWidget(SM_VAVLE_TEST_WIDGET);
//            g_MainManager().SendWidgetMsg(SM_VAVLE_TEST_WIDGET,MSG_VALVE_TEST);
//        }
//    }
//    else{ //超时用户未操作，直接进行
//        g_MainManager().ShowWidget(SM_VAVLE_TEST_WIDGET);
//        g_MainManager().SendWidgetMsg(SM_VAVLE_TEST_WIDGET,MSG_VALVE_TEST);
//    }
}
void ZKSort::infoUpdateZKSort(QVariant m_unit)
{
    UpdateUnit unit = QVariant(m_unit).value<UpdateUnit>();

    QTimer timer;
    timer.setSingleShot(true);

    myMessageBox msgBox(MSG_QUES, g_myLan().accept_remote_upgrade);

    QObject::connect(&timer, SIGNAL(timeout()), &msgBox, SLOT(hide()));
    timer.start(10000);
    int ret = msgBox.exec();

    if (timer.isActive())
    {
        timer.stop();
        if (ret == QDialog::Accepted){ //用户操作，同意进行
            DownloadDialog dialog;
            dialog.setDownloadInfo(m_unit);
            dialog.exec();
        }
    }
    else{ //超时用户未操作，直接进行
        if(unit.force == 1)
        {
            DownloadDialog dialog;
            dialog.setDownloadInfo(m_unit);
            dialog.exec();
        }
        else{
#ifdef Q_OS_UNIX
//            myMqttMsgParaseThread->onCmdReturn(unit.m_uuid,unit.m_cmd,USERREJECT);
#endif
        }
    }
}
void ZKSort::infoUpdateIpcModel(QVariant m_unit)
{
    UpdateUnit unit = QVariant(m_unit).value<UpdateUnit>();

    QTimer timer;
    timer.setSingleShot(true);

    myMessageBox msgBox(MSG_QUES, g_myLan().accept_remote_upgrade+g_myLan().ids_Type);

    QObject::connect(&timer, SIGNAL(timeout()), &msgBox, SLOT(hide()));
    timer.start(10000);
    int ret = msgBox.exec();

    if (timer.isActive())
    {
        timer.stop();
        if (ret == QDialog::Accepted){ //用户操作，同意进行
            DownloadDialog dialog;
            dialog.setDownloadInfo(m_unit);
            dialog.exec();
        }
    }
    else{ //超时用户未操作，直接进行
#ifdef Q_OS_UNIX
//        myMqttMsgParaseThread->onCmdReturn(unit.m_uuid,unit.m_cmd,USERREJECT);
#endif
    }
}
void ZKSort::infoUploadIpcInfo()
{
    QTimer timer;
    timer.setSingleShot(true);

    struGsh.nIpcInfoUploadAccess = 0;

    myMessageBox msgBox(MSG_QUES, g_myLan().cfm_upload_ipcinfo);

    QObject::connect(&timer, SIGNAL(timeout()), &msgBox, SLOT(hide()));
    timer.start(5000);
    int ret = msgBox.exec();

    if (timer.isActive())
    {
        timer.stop();
        if (ret == QDialog::Accepted){ //用户操作，同意进行
            struGsh.nIpcInfoUploadAccess = 1;
        }
        else
            struGsh.nIpcInfoUploadAccess = 2;
    }
    else{ //超时用户未操作，直接进行
        struGsh.nIpcInfoUploadAccess = 1;
    }
}

void ZKSort::infoStartIpcFilterCottonClean()
{
//    myMessageBox msgBox(MSG_QUES, g_myLan().msg_ipc_clean_filter_cotton);
//    int ret = msgBox.exec();
//    if (ret == QDialog::Accepted){ //用户操作，同意进行

//    }
//    else{

//    }
    g_infoWidget().setLabelText(g_myLan().msg_ipc_clean_filter_cotton);
    myFlow.sleep(2);
    g_infoWidget().hide();
}

void ZKSort::PageChanged(const uint pageId)
{
    ui->topWidget->PageChanged(pageId);
    ui->bottomWidget->PageChanged(pageId);
    if(m_hideBottomPageId.contains(pageId)){
        ui->bottomWidget->hide();
    }else{
        ui->bottomWidget->show();
    }
    if(m_hideTopPageId.contains(pageId)){
        ui->topWidget->hide();
    }else{
        ui->topWidget->show();
    }
}

void ZKSort::setBottomEnable(bool enable)
{
    ui->bottomWidget->setValveEnable(enable);
}
void ZKSort::refreshBottomStatus()
{
    ui->bottomWidget->refreshFeedVavleStatus();
}

void ZKSort::showPage(bool bshow)
{

}

void ZKSort::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_F8){
        LoadSkin();
    }
    QWidget::keyPressEvent(e);
}

void ZKSort::retranslateUiWidget()
{

}

void ZKSort::LoadSkin()
{
#ifdef SKIP_TEST
    QString skinPath(qApp->applicationDirPath()+"/../resource/skin/qss/skin.qss");
#else
    QString skinPath;
    if(struCnfg.screenH == 600)
        skinPath = ":/white/skin/qss/skin_1024_600.qss";
    else if (struCnfg.screenH == 768)
        skinPath = ":/white/skin/qss/skin_1024_768.qss";
    else
        skinPath = ":/white/skin/qss/skin.qss";
#endif
    QFile file(skinPath);
    if (!file.open(QIODevice::ReadOnly))
        Q_ASSERT(false);
    QString styleSheet = file.readAll();
    qApp->setStyleSheet(styleSheet);
    file.close();
}

void ZKSort::startEnterMainPage()
{
    g_infoWidget().setLabelText(g_myLan().msg_system_init);
    g_infoWidget().delayShow();
    myFlow.initSendAllParams();
    myFlow.sleep(3);

    for(int j = 0;j < 15;j++)
    {
        myFlow.sleep(1);
        if(struIpcShare.allIpcModelLoadOk > 1)
        {
            break;
        }
    }

//#ifdef Q_OS_UNIX
//    if(struCnfg.nUseIpcEnable == 1 && struGsh.debugMode == 0)
//    {
//        for(int i = 0; i < struCnfg.nIpcCount; i++)
//        {
//            for(int j = 0;j < 15;j++)
//            {
//                myFlow.sleep(1);
//                if(struIpcShare.struIpcStatus[i].modelLoadStatus == 2 && struIpcShare.struIpcReqStauts[i].modelLoadReq == 2)
//                {
//                    myFlow.getIpcInfo(i);
//                    myFlow.getIpcStatus(i);
//                    myFlow.getIpcModelInfo(i);
//                    myFlow.getIpcHardwareInfo(i);
//                    break;
//                }
//            }
//        }
//    }
//    myMqttMsgParaseThread->n_uploadPartsStatusFlag = (myFlow.startWithoutCnf == 0);
//#endif

    g_MainManager().CreateWidgets();
    g_infoWidget().hide();
    struCnfg.bFlagFirstrun = 1;
    if(struCnfg.nLVMachine){
        g_MainManager().ShowWidget(SM_MAIN_PAGE_WIDGET);
    }
    else{
        g_MainManager().ShowWidget(SM_MAIN_PAGE_WIDGET_NEW);
    }
    if(struCnfg.nPlcCtrl == 1 || struCnfg.nDefaultPlcCtrl == 1)
    {
        g_MainManager().ShowWidget(SM_PLC_CTRL_WIDGET);
    }
}

void ZKSort::myWipeStartSlt()
{
    logger()->info("start wipe");
    struGsh.bFlagWiping = 1;
    g_infoWidget().setLabelText(g_myLan().msg_wiping);

    setEnabled(false);
    g_infoWidget().delayShow();

    myFlow.startWipe();

    g_infoWidget().hide();
    setEnabled(true);
    struGsh.bFlagWiping = 0;
}

void ZKSort::lostFpgaPowerOffSlt()
{
    struGsh.bLostFpgaBoard = 0;

    if(struGsh.bSortStart == 1)
    {
        g_infoWidget().setLabelText(g_myLan().msg_turning_off);

        g_infoWidget().delayShow();
        myFlow.onOff();
        g_infoWidget().hide();
    }
    //关机参数保存确认

    g_infoWidget().setLabelText(g_myLan().save_parameters);
    g_infoWidget().delayShow();

    g_Runtime().save();

    g_infoWidget().hide();

    g_infoWidget().setLabelText(g_myLan().ids_shutdown);
    g_infoWidget().delayShow();
#ifdef Q_OS_UNIX
//    myMqttMsgParaseThread->n_uploadLogFileFlag = 1;
    myFlow.sleep(1);
    struGsh.bFlagPowerOff = 1;
    system("shutdown -h now");
    myFlow.sleep(10);
#endif
    g_infoWidget().hide();
}

void ZKSort::alarmTurnOffFeedSlt()
{
    logger()->warn("alarm turnoff feed");
    struGsh.bFlagAutowipe = 0;      // 禁止清灰

    if(struCnfg.nPlcControl){

        if(struGsh.bAlarmFeedStaus[1] && !struGsh.struMachineStatus[0].bAlarmFeedLevel[1]&& struGsh.bSortStart) {
            struGsh.bSortStart = 0;
            myFlow.resetFeederRG();
            myFlow.sleep(struCnfg.nOnoffDelay);
            if(!struGsh.bStatEject)
                myFlow.resetSortOnOff(0, 0);
                myFlow.configIpcSortStart(0);
        }
        if(struGsh.bAlarmFeedStaus[2] &&!struGsh.struMachineStatus[0].bAlarmFeedLevel[2] && struGsh.bStatEject){
            struGsh.bStatEject = 0;
            if(!struGsh.bSortStart)
                myFlow.resetSortOnOff(0, 0);
            myFlow.vavleOnOff();
        }
    }
    else{
        if(struGsh.bSortStart == 1) {
            struGsh.bAlarmLastFeedoff = 1;
            struGsh.bSortStart = 0;
            struGsh.bStatEject = 0;
            myFlow.configIpcSortStart(0);

            myFlow.resetFeederRG();
            myFlow.sleep(struCnfg.nOnoffDelay);

            myFlow.resetSortOnOff(0, 0);
            myFlow.vavleOnOff();
        }
    }

    g_MainManager().SendCurWidgetMsg(MSG_OFF);
    ui->bottomWidget->refreshFeedVavleStatus();

    myFlow.resetNetWaveStart();
    struGsh.bFlagAutowipe = 1;      // 允许清灰

    struGsh.curAlarmCount++;        //气压报警次数累加
    struCnfg.ntotalAlarmCount++;
}

void ZKSort::alarmTurnOnFeedSlt()
{
    logger()->warn("alarm turnon feed");
    struGsh.bFlagAutowipe = 0;      // 禁止清灰

    if(struCnfg.nPlcControl){
        // 红外部分
        //开启振动器前发送明场校正命令
        if(struCnfg.nRealTimeCorrection)
        {
            myFlow.resetSortOnOff(0, 0);
            myFlow.cameraBrightCalibrateRG(true, struGsh.nLayer, struGsh.nView, struGsh.nUnit, 0);
            myFlow.sleep(2);
        }
        if(struGsh.bAlarmFeedStaus[2] &&struGsh.struMachineStatus[0].bAlarmFeedLevel[2] && !struGsh.bStatEject){
            struGsh.bStatEject = 1;
            if(!struGsh.bSortStart)
                myFlow.resetSortOnOff(1, 0);
            myFlow.vavleOnOff();
        }
        if(struGsh.bAlarmFeedStaus[1] &&struGsh.struMachineStatus[0].bAlarmFeedLevel[1] && !struGsh.bSortStart){
            struGsh.bSortStart = 1;
            myFlow.configIpcSortStart(1);

            if(!struGsh.bStatEject)
                myFlow.resetSortOnOff(1, 0);
            myFlow.resetFeederRG();
        }
    }
    else{
        struGsh.bAlarmLastFeedoff = 0;
        struGsh.bSortStart = 1;
        struGsh.bStatEject = 1;
        myFlow.configIpcSortStart(1);

        myFlow.resetSortOnOff(1, 0);
        myFlow.vavleOnOff();
        myFlow.resetFeederRG();
    }

    g_MainManager().SendCurWidgetMsg(MSG_OFF);
    ui->bottomWidget->refreshFeedVavleStatus();

    myFlow.resetNetWaveStart();

    struGsh.bFlagAutowipe = 1;      // 允许清灰
}

void ZKSort::alarmColorVoiceOnSlt()
{
    logger()->warn("alarm turn on color voice");

    myFlow.resetControlAlarmRG();
    struGsh.bAlarmColorVoiceLast = 1;
}

void ZKSort::alarmColorVoiceOffSlt()
{
    logger()->warn("alarm turn off color voice");

    myFlow.resetControlAlarmRG();
    struGsh.bAlarmColorVoiceLast = 2;
}

void ZKSort::warnDccryptSlt()
{
    if(struGsh.bSortStart == 1)
    {
        myFlow.onOff();
    }

    myDccrypt dlg;
    int ret = dlg.exec();
    if (ret == QDialog::Accepted) {
        g_infoWidget().setLabelText(g_myLan().ids_shutdown);
        g_infoWidget().delayShow();
        myFlow.sleep(3);

        g_infoWidget().hide();
        this->close();
#ifndef Q_OS_WIN
        myFlow.sleep(1);    
        struGsh.bFlagPowerOff = 1;
        system("shutdown -h now");
#endif
    }
}

void ZKSort::writeDccryptTimeSlt()
{
    struGsh.bUpdateState = 1;
    int waitTime = 50;
    char data[SEND_PACKET_DATA_SIZE];

    memset(data, 0, SEND_PACKET_DATA_SIZE);
    myFlow.interfaceEncryptAns = 0;

    mySerial.pushCom1CmdData(CMD_INT_ENCRYPT_PARAMS_REQUIRE, 0, BOARD_TYPE_INTERFACE, 0, data, 1);
    for(int tt = 0; tt < 30; tt++)
    {
        myFlow.msleep(waitTime);
        if(1 == myFlow.interfaceEncryptAns )
        {
            if( struGsh.struDccryptData.bDccryptGetError != ENCRYPT_FPGA_CODE_FORMAT_ERROR)
            {
                myFlow.writeDccryptBiggerTime();
                struGsh.bUpdateState = 0;
                return;
            }
            break;
        }
    }
//    writeSysTimeToEmmc();
    myFlow.writeFpgaDccryptTime();
    struGsh.bUpdateState = 0;
}

void ZKSort::write30DayDccryptTimeSlt()
{
    if(paramDelayCode30Day.product30DayUseAccHour < DCCRYPT_30DAY_ACC_HOUR+2)
    {
        paramDelayCode30Day.product30DayUseAccHour++;
        paramDelayCode30Day.product30DayUseAccHourWrite();
    }
}
void ZKSort::dccryptCheck()
{
    mytimer->stop();

    int onoff_cnts = 0;
    int curYear = QDateTime::currentDateTime().date().year();
    time_t sysTime,eepTime;
//    int eepStat = getSysTimeFromE2prom(&eepTime);
    int eepStat ;
    time(&sysTime);

    if(curYear < 2020 || eepStat< 0 || sysTime < eepTime)
    {
        QSettings setting(CFG_APPSet,QSettings::IniFormat);
        onoff_cnts =  setting.value("nobatonoffcnts",0).toInt();
        setting.setValue("nobatonoffcnts",++onoff_cnts);
        setting.sync();
        logger()->info("nobatonoffcnts: %1times",onoff_cnts);
    }
    else{
        QSettings setting(CFG_APPSet,QSettings::IniFormat);
        setting.setValue("nobatonoffcnts",0);
        setting.sync();
    }

    if(QString(SCREEN_VERSION).contains("test"))
    {
        communication();
    }
    else{
#ifdef Q_OS_UNIX
        if(paramDelayCode.delayCodeCheck()) {   //加密到期
            myDccrypt dlg;
            int ret = dlg.exec();
            if (ret == QDialog::Accepted) {
                communication();
            }
        }
        else
        {
            /*
             * 时间校验异常进行提示，满足７次即锁机，解密后可正常操作
            */
            if(onoff_cnts)
            {
                myMessageBox msgBox(MSG_WARN, g_myLan().ids_confirmTime+QString::number(onoff_cnts));
                msgBox.exec();
                if(onoff_cnts >= 7)
                {
                    myDccrypt dlg;
                    int ret = dlg.exec();
                }
            }
            else{
                if(paramDelayCode.enable == 0xAA)
                {
                    if((struGsh.struDccryptData.mCryptDayLast <= 7)&&(struGsh.struDccryptData.mCryptDayLast > 0))
                    {
                        QString tmpInfo = g_myLan().days_avaiable+QString(": %1").arg(struGsh.struDccryptData.mCryptDayLast);
                        myMessageBox msgBox(MSG_QUES, tmpInfo);

                        msgBox.exec();
                    }
                }
                else
                {
                    paramDelayCode30Day.get30DayInfofromEeprom();
                    paramDelayCode30Day.fpgaCodeErrCountGet();
                    paramDelayCode30Day.product30DayUseAccHourGet();

                    if((paramDelayCode30Day.product30DayUseStartEnable == 0xAA)&&(paramDelayCode30Day.product30DayUseOver == 0xAA))
                    {
                        myDccrypt30Day dlg30;
                        int ret2 = dlg30.exec();
                    }
                }
            }
            communication();
        }
#else
        startEnterMainPage();
#endif
    }


    //dccrypt ok
    //g_MainManager().ShowWidget(SM_SELF_CHECK_WIDGET);
    connect(myUpdateStatusThread, SIGNAL(sWipeStartSig()), this, SLOT(myWipeStartSlt()));
    connect(myUpdateStatusThread, SIGNAL(sLostFpgaPowerOffSig()), this, SLOT(lostFpgaPowerOffSlt()));
    connect(myUpdateStatusThread, SIGNAL(sAlarmTurnOffSig()), this, SLOT(alarmTurnOffFeedSlt()));
    connect(myUpdateStatusThread, SIGNAL(sAlarmTurnOnSig()), this, SLOT(alarmTurnOnFeedSlt()));

    connect(myUpdateStatusThread, SIGNAL(sAlarmColorVoiceOnSig()), this, SLOT(alarmColorVoiceOnSlt()));
    connect(myUpdateStatusThread, SIGNAL(sAlarmColorVoiceOffSig()), this, SLOT(alarmColorVoiceOffSlt()));

    connect(myUpdateStatusThread, SIGNAL(sDccryptWarningSig()), this, SLOT(warnDccryptSlt()));
    connect(myUpdateStatusThread, SIGNAL(sDccryptCodeWriteSig()), this, SLOT(writeDccryptTimeSlt()));
    connect(myUpdateStatusThread, SIGNAL(s30DayDccryptTimeWriteSig()), this, SLOT(write30DayDccryptTimeSlt()));
    connect(myUpdateStatusThread, SIGNAL(sInfoStartValve()), this, SLOT(infoStartValve()));

#ifdef Q_OS_UNIX
//    connect(myMqttMsgParaseThread,SIGNAL(cmd_updatezksort_ask(QVariant)),this,SLOT(infoUpdateZKSort(QVariant)));
//    connect(myMqttMsgParaseThread,SIGNAL(cmd_updateIpcModel_ask(QVariant)),this,SLOT(infoUpdateZKSort(QVariant)));
//    connect(myMqttMsgParaseThread,SIGNAL(cmd_uploadIpcInfo_ask()),this,SLOT(infoUploadIpcInfo()));
#endif

    connect(myUpdateStatusThread,SIGNAL(sInfoIpcStartFilterCottonClean()),this,SLOT(infoStartIpcFilterCottonClean()));
}
