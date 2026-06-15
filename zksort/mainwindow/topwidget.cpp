#include "topwidget.h"
#include "ui_topwidget.h"
#include "global.h"
#include "mysetdatetime.h"
TopWidget::TopWidget(QWidget *parent) :
    BaseUi(parent),
    ui(new Ui::TopWidget)
{
    ui->setupUi(this);

    ui->SysStateWidgetBtn->setText("");
    ui->saveBtn->setText("");
    ui->levelBtn->setText("");
    m_levelPageId.append(SM_USER_WIDGET);
    m_levelPageId.append(SM_MAIN_PAGE_WIDGET);
    m_levelPageId.append(SM_SELF_CHECK_WIDGET);
    m_levelPageId.append(SM_MACHINE_DCCRYPT_WIDGET);
    m_levelPageId.append(SM_SIMULATE_WIDGET);
    m_levelPageId.append(SM_MAIN_PAGE_WIDGET_NEW);
    ipcstatus[0] = ui->label;
    ipcstatus[1] = ui->label_2;
    ipcstatus[2] = ui->label_3;
    ipcstatus[3] = ui->label_4;
    ipcstatus[4] = ui->label_5;
    ipcstatus[5] = ui->label_6;
    ipcstatus[6] = ui->label_7;
    ipcstatus[7] = ui->label_8;
    ipcstatus[8] = ui->label_9;
    for(int i= 0; i < 9;i++){
        ipcstatus[i]->setText("");
        ipcstatus[i]->hide();
    }

    updateDateTime();
    connect(myUpdateStatusThread, SIGNAL(sUpdateSystemTimeSig()), this, SLOT(updateDateTime()));
    m_pTimer = new QTimer(this);
    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(timeOut()));
    reloadTitleMap();
    m_calibrateTimes = 0;
}

TopWidget::~TopWidget()
{
    delete ui;
}

void TopWidget::PageChanged(int pageId)
{
    updateIpcStatus();

    if(pageId == SM_PLC_CTRL_WIDGET)
     {
         ui->SysStateWidgetBtn->hide();
     }
     else
     {
         ui->SysStateWidgetBtn->show();
     }
    if(pageId == SM_MACHINE_DCCRYPT_WIDGET || pageId == SM_SELF_CHECK_WIDGET){
        ui->SysStateWidgetBtn->setEnabled(false);
    }else{
        ui->SysStateWidgetBtn->setEnabled(true);
    }
    if(pageId == SM_SYSSTATE_WIDGET){
        ui->timeLabel->setEnabled(true);
    }
    else
    {
        ui->timeLabel->setEnabled(false);
    }

    if(pageId == SM_REMOTE_CONTROL_WIDGET)
    {
        ui->saveBtn->hide();
        ui->SysStateWidgetBtn->hide();
    }
    else
    {
        ui->saveBtn->show();
        ui->SysStateWidgetBtn->show();
    }


    ui->pB_conveyor->setVisible(struCnfg.nLVMachine && (struCnfg.nMachineType == MACHINE_LD_ZL_GP || struCnfg.nMachineType== MACHINE_LD2_ZL_GP));
    update();
}

void TopWidget::showPage(bool bshow)
{
    retranslateUiWidget();

    updateIpcStatus();
    ui->levelBtn->hide();
    ui->netLbl->hide();
    ui->pB_conveyor->setVisible(struCnfg.nLVMachine && (struCnfg.nMachineType == MACHINE_LD_ZL_GP || struCnfg.nMachineType== MACHINE_LD2_ZL_GP));
    if(struGsh.bConveyorOnOff)
    {
        ui->pB_conveyor->setStyleSheet("QPushButton{min-width:50px;max-width:50px;min-height:50px;max-height:50px;"
                                       "background-color:rgba(0,0,0,0);border-image:url(:/white/conveyor_run.png)}");
    }
    else
    {
        ui->pB_conveyor->setStyleSheet("QPushButton{min-width:50px;max-width:50px;min-height:50px;max-height:50px;"
                                       "background-color:rgba(0,0,0,0);border-image:url(:/white/conveyor_stop.png)}");
    }
}

void TopWidget::retranslateUiWidget()
{
    reloadTitleMap();
}

void TopWidget::mousePressEvent(QMouseEvent *event)
{
    if(ui->widget_2->geometry().contains(event->pos())){
        m_calibrateTimes ++;
        if(!m_pTimer->isActive())
            m_pTimer->start(500);
    }
    else
         m_calibrateTimes = 0;
}

void TopWidget::mouseReleaseEvent(QMouseEvent *event)
{
//    m_pTimer->stop();
}

void TopWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
#ifdef Q_OS_WIN
    QString fileName =QApplication::applicationDirPath()+QString("/logo/top.bmp");
#else
    QString fileName = QString("/sdcard/logo/top.bmp");
#endif
    painter.drawPixmap(rect(),QPixmap(fileName),QRect());
}

void TopWidget::reloadTitleMap()
{
    m_pageId_TitleMap.clear();
    m_pageId_TitleMap[SM_SELF_CHECK_WIDGET] = g_myLan().ids_selfcheck;
    m_pageId_TitleMap[SM_USER_WIDGET] = g_myLan().user_mode;
    m_pageId_TitleMap[SM_ENGINEER_WIDGET] = g_myLan().ids_engineer;
    m_pageId_TitleMap[SM_FACTORY_SET_WIDGET] = g_myLan().ids_set;
    m_pageId_TitleMap[SM_SYSSTATE_WIDGET] = g_myLan().system_stat;
    m_pageId_TitleMap[SM_LANGUAGE_WIDGET] = g_myLan().language;
    m_pageId_TitleMap[SM_FEED_SET_WIDGET] = g_myLan().ids_feed;
    m_pageId_TitleMap[SM_WIPE_SET_WIDGET] = g_myLan().ids_cleaning;
    m_pageId_TitleMap[SM_VAVLE_TEST_WIDGET] = g_myLan().ids_valveTest;
    m_pageId_TitleMap[SM_MODE_MANAGE_WIDGET] = g_myLan().ids_modelManagement;
    m_pageId_TitleMap[SM_MODE_SET_WIDGET] = g_myLan().scheme_parameter;
    m_pageId_TitleMap[SM_SENSE_SET_WIDGET] = g_myLan().ids_select;
    m_pageId_TitleMap[SM_MACHINE_SET_WIDGET] = g_myLan().ids_machineModel;
    m_pageId_TitleMap[SM_CAMERA_SET_WIDGET] = g_myLan().camera_set;
    m_pageId_TitleMap[SM_GRAYSENSE_SET_WIDGET] = g_myLan().ids_select;
    m_pageId_TitleMap[SM_IMAGE_MANAGE_WIDGET] = g_myLan().ai_analysis;
    m_pageId_TitleMap[SM_BIG_SMALL_SENSE_SET_WIDGET] = g_myLan().arith_big_small;
    m_pageId_TitleMap[SM_CIRCLE_LONG_SENSE_SET_WIDGET] = g_myLan().arith_circle_long;
    m_pageId_TitleMap[SM_LONG_SHORT_SENSE_SET_WIDGET] = g_myLan().arith_long_short;
    m_pageId_TitleMap[SM_SVM_SENSE_SET_WIDGET] = g_myLan().arith_svm_a;
    m_pageId_TitleMap[SM_MAIN_PAGE_WIDGET] = g_myLan().page_home;
    m_pageId_TitleMap[SM_CAMERA_SIGNAL_WIDGET] = g_myLan().signal_set;
    m_pageId_TitleMap[SM_DIVIDE_PIXEL_WIDGET] = g_myLan().ids_devidePixel;
    m_pageId_TitleMap[SM_DIVIDE_PIXEL_SHOWALL_WIDGET] = g_myLan().ids_devidePixel;
    m_pageId_TitleMap[SM_EJECT_TIME_WIDGET] = g_myLan().valve_test;
    m_pageId_TitleMap[SM_LIGHE_SET_WIDGET] = g_myLan().lamp_control;
    m_pageId_TitleMap[SM_DOUBLE_LIGHE_SET_WIDGET] = g_myLan().lamp_control;
    m_pageId_TitleMap[SM_IMAGE_CAPTURE_WIDGET] = g_myLan().ai_image_capturing;
    m_pageId_TitleMap[SM_BACKGROUND_SET_WIDGET] = g_myLan().ids_background;
    m_pageId_TitleMap[SM_SIMULATE_WIDGET] = g_myLan().ai_simulate;
    m_pageId_TitleMap[SM_PIXEL_AJUST_WIDGET] = g_myLan().ids_devidePixel;
    m_pageId_TitleMap[SM_EJECT_WIDGET] = g_myLan().tick_params;
    m_pageId_TitleMap[SM_LAMP_BOARD_SET_WIDGET] = g_myLan().ids_ccsBoard;
    m_pageId_TitleMap[SM_IPC_MODEL_CHANGE_WIDGET] = g_myLan().ipc_modelChange;
    m_pageId_TitleMap[SM_IPC_MODEL_CLASS_INFO_WIDGET] = g_myLan().classType;
}

void TopWidget::updateIpcStatus()
{    
    QString title;
    if(struCnfg.nUseIpcEnable == 1)
    {
        switch(struIpcShare.allIpcModelLoadOk)
        {
        case 0:
            ui->titleLbl->setText("");
            break;
        case 1:
            ui->titleLbl->setText(g_myLan().waitingModel);
            break;
        case 2:
            title = QString::fromUtf8(struCnfg.stuProfileIndex[struCnfg.nProfile].sMaterialName);
            ui->titleLbl->setText(title);
            break;
        case 3:
        case 4:
        case 5:
        case 6:
            ui->titleLbl->setText(g_myLan().modelLoadingErr+QString::number(struIpcShare.allIpcModelLoadOk));
            break;
        }
    }
    else
    {
        title = QString::fromUtf8(struCnfg.stuProfileIndex[struCnfg.nProfile].sMaterialName);
        ui->titleLbl->setText(title);
    }
    ui->statusLabel->setVisible(struCnfg.nUseIpcEnable);
}

void TopWidget::on_levelBtn_clicked()
{
    g_Runtime().g_level2 = (g_Runtime().g_level2 == 1) ? 3 : 1;
    g_MainManager().SendCurWidgetMsg(MSG_LEVEL_CHANGE);
}

void TopWidget::updateDateTime()
{
    updateIpcStatus();
    QString title;
    int curTime = 0,ipcConnectFlag = 0;
    QString dateText = QDateTime::currentDateTime().toString("h:mm:ss \n yyyy/MM/dd");
    QStringList list = dateText.split(":");
    if(list.at(0) == "23" && list.at(1) == "59" && list.at(2).left(2) == "30")
    {
#ifdef Q_OS_UNIX
//        myMqttMsgParaseThread->n_uploadLogFileFlag = 1;
#endif
    }
    if(list.at(0) == "23" && list.at(1) == "59" && list.at(2).left(2) == "59")
    {
        myFlow.clearIpcCamBadPointsCount();

        for(int k = 0; k < MAX_IPC_MODEL_CLASS; k++)
        {
            struIpcShare.sCurModelLastClassBadPointsCount[k] = 0;
        }
    }

    ui->timeLabel->setText(dateText);
    ui->alarmLbl->setVisible(struGsh.bAlarmStatus && (struGsh.nCounter%2 == 0));


    ipcConnectFlag = 1;
    if(!struCnfg.nUseIpcEnable) goto end;
    for(int j = 0; j < struCnfg.struLayerInfo[0].nViewChainTotal; j++)
    {
        if(struCnfg.struLayerInfo[0].nViewIsUsed[j] == 1)
        {
            for(int k = 0; k <struCnfg.struLayerInfo[0].nViewUnitTotal[j]; k++)
            {
                if(struCnfg.struIpcCameraParams[0][j][k].ipcType != IPC_CAMERA_TYPE_NULL)
                {
//                    qDebug()<<struGsh.intefaceAlive<<struIpcShare.struIpcInfo[j][k].aliveRetryCnt<<struIpcShare.struIpcInfo[j][k].aliveRetryCnt;
                    if(struGsh.intefaceAlive > 1||struIpcShare.struIpcInfo[j][k].aliveRetryCnt > 3 ){
                        ipcConnectFlag = -1;
                        goto end;
                    }
                }
            }
        }
    }
end:
    struIpcShare.allIpcConnecOk = ipcConnectFlag;
    if(struIpcShare.allIpcConnecOk == 1)
        ui->statusLabel->setStyleSheet("QLabel{min-width:50px;min-height:50px;max-width:50px;max-height:50px;border-image:url(:/white/online.png);}");
    else if(struIpcShare.allIpcConnecOk == -1)
        ui->statusLabel->setStyleSheet("QLabel{min-width:50px;min-height:50px;max-width:50px;max-height:50px;border-image:url(:/white/offline_red.png);}");

    if(struCnfg.nAlarmValid[ALARM_IPC] && struIpcShare.allIpcConnecOk == -1 && struCnfg.bFlagFirstrun == 1)
        struGsh.nAlarmStautsCurrent[ALARM_IPC] = 1;
    else
        struGsh.nAlarmStautsCurrent[ALARM_IPC] = 0;
}

void TopWidget::on_SysStateWidgetBtn_clicked()
{
    g_MainManager().ShowWidget(SM_SYSSTATE_WIDGET);
}

void TopWidget::on_saveBtn_clicked()
{
    g_infoWidget().setLabelText(g_myLan().msg_saving_params);
    g_infoWidget().delayShow();
    g_Runtime().save();
    myFlow.sleep(1);
    g_infoWidget().hide();
}

void TopWidget::timeOut()
{
    m_pTimer->stop();
    if(m_calibrateTimes >= CALIBRATE_TIMES){
        g_MainManager().ShowWidget(SM_MAIN_PAGE_WIDGET_NEW);
    }
    m_calibrateTimes = 0;
//    if(m_calibrateTimes < CALIBRATE_TIMES)
//        m_calibrateTimes++;
//    else{
//        myMessageBox *mesBox = new myMessageBox(MSG_QUES, g_myLan().cfm_ts_calibrate);
//        int ret = mesBox->exec();
//        if(ret == QDialog::Accepted){
//            QProcess *process = new QProcess;
//            process->startDetached("./mycalibrate.sh");
//            system("/app/kmy.sh");
//        }
//        m_pTimer->stop();
//    }
}

void TopWidget::on_timeLabel_clicked()
{
//    if(paramDelayCode.enable == 0xAA)
//    {
//        return;
//    }

//    MySetDateTime setTime;
//    int ret = setTime.exec();
}
void TopWidget::on_pB_conveyor_clicked()
{
    myMessageBox msgBox(MSG_QUES, g_myLan().cfm_ld_state);
    int ret = msgBox.exec();
    if (ret == QDialog::Accepted)
    {
        struGsh.bConveyorOnOff = !struGsh.bConveyorOnOff;

        if(struGsh.bConveyorOnOff)
        {
            ui->pB_conveyor->setStyleSheet("QPushButton{min-width:50px;max-width:50px;min-height:50px;max-height:50px;"
                                            "background-color:rgba(0,0,0,0);border-image:url(:/white/conveyor_run.png)}");
        }
        else
        {
            ui->pB_conveyor->setStyleSheet("QPushButton{min-width:50px;max-width:50px;min-height:50px;max-height:50px;"
                                           "background-color:rgba(0,0,0,0);border-image:url(:/white/conveyor_stop.png)}");
        }
        myFlow.resetConveyorSpeed();
        myFlow.resetControlAlarmRG();
    }
}

