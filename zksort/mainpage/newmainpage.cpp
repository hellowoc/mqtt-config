#include "newmainpage.h"
#include "ui_newmainpage.h"

NewMainPage::NewMainPage(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::NewMainPage)
{
    ui->setupUi(this);
    ui->runBtn->setCheckable(true);
    ui->senseBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->senseBtn->setFocusPolicy(Qt::NoFocus);
    ui->feedBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->feedBtn->setFocusPolicy(Qt::NoFocus);
    ui->wipeBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->wipeBtn->setFocusPolicy(Qt::NoFocus);
    ui->vavleBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->vavleBtn->setFocusPolicy(Qt::NoFocus);
    ui->modeBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->modeBtn->setFocusPolicy(Qt::NoFocus);
    ui->setBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->setBtn->setFocusPolicy(Qt::NoFocus);
    connect(mytcpServer,SIGNAL(updateui()), this, SLOT(updateUi()));


}

NewMainPage::~NewMainPage()
{
    delete ui;
}

void NewMainPage::showPage(bool bshow)
{
    if(bshow){
        updateUi();
        updateLevel();

        if(struCnfg.bMainpageShow)
        {
            connect(getThread,SIGNAL(sGetClassPointsFinished()),this,SLOT(updateshowdata()) );
            ui->widget_5->setTimer(true);
        }
    }
    else
    {
        if(struCnfg.bMainpageShow)
        {
            disconnect(getThread,SIGNAL(sGetClassPointsFinished()),this,SLOT(updateshowdata()) );
            ui->widget_5->setTimer(false);
        }


    }
}

void NewMainPage::retranslateUiWidget()
{
    ui->wheelBtn->setText(g_myLan().ids_feed);
    ui->shutdownBtn->setText(g_myLan().ids_shutdown);
    ui->senseBtn->setText(g_myLan().ids_select);
    ui->feedBtn->setText(g_myLan().ids_feed);
    ui->wipeBtn->setText(g_myLan().ids_cleaning);
    ui->vavleBtn->setText(g_myLan().ids_valve);
    ui->modeBtn->setText(g_myLan().ids_model);
    ui->setBtn->setText(g_myLan().ids_set);
}

void NewMainPage::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_LEVEL_CHANGE){
        updateLevel();
    }else if(msg1 == MSG_OFF){
        updateUi();
    }
}

void NewMainPage::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QString fileName;
#ifdef Q_OS_WIN
    fileName =QApplication::applicationDirPath()+QString("/logo/homepage.bmp");
#else
    fileName = QString("/sdcard/logo/homepage.bmp");
#endif
    // painter.drawPixmap(rect(),QPixmap(fileName),QRect(0,50,1024,718));
    if(!struCnfg.bMainpageShow)
        painter.drawPixmap(rect(),QPixmap(fileName),QRect(0,0,1024,struCnfg.screenH));
}

void NewMainPage::on_runBtn_clicked(bool checked)
{
    //    struGsh.updateIpcStatisdata = 1;
    if(struCnfg.nUseIpcEnable == 1)
    {
        if(struIpcShare.allIpcModelLoadOk != 2 )
        {
            ui->runBtn->setChecked(struGsh.bSortStart);
            g_infoWidget().setLabelText("IPC"+g_myLan().ids_cannotRun);
            g_infoWidget().showSecs();
            return;
        }
    }

    if(struCnfg.nPlcControl) {

        g_infoWidget().setLabelText("PLC"+g_myLan().remote_control+g_myLan().msg_applying);
        g_infoWidget().delayShow();
        myFlow.sleep(2);
        g_infoWidget().hide();
        return;
    }

    if(struGsh.bSortStart == 0)
    {
        if(struGsh.bAlarmStatus == 1)
        {
            g_infoWidget().setLabelText(g_myLan().ids_airpressureAlarm);
            g_infoWidget().delayShow();
            myFlow.sleep(2);
            g_infoWidget().hide();

            updateUi();
            return;
        }

        //        if(struCnfg.nUseIpcEnable == 1)
        //        {
        //            if(struIpcShare.allIpcModelLoadOk != 2 || struIpcShare.allIpcConnecOk == 0)
        //            {
        //                g_infoWidget().setLabelText(g_myLan().msg_ipc_poweroff+QString::number(struIpcShare.allIpcModelLoadOk));
        //                g_infoWidget().delayShow();
        //                myFlow.sleep(2);
        //                g_infoWidget().hide();

        //                updateUi();
        //                return;
        //            }
        //        }

        myMessageBox msgBox(MSG_QUES, g_myLan().cfm_turn_on);

        int ret = msgBox.exec();

        if (ret == QDialog::Accepted)
        {
            logger()->info("sort start");
            g_infoWidget().setLabelText(g_myLan().msg_turning_on);

            g_infoWidget().delayShow();
            myFlow.onOff();

            myFlow.sleep(1);
            g_infoWidget().hide();
            updateUi();
        }
        else
        {
            updateUi();
        }
    }
    else
    {
        logger()->info("sort stop");
        g_infoWidget().setLabelText(g_myLan().msg_turning_off);
        g_infoWidget().delayShow();
        myFlow.onOff();

        myFlow.sleep(1);
        g_infoWidget().hide();
        updateUi();
    }
}

void NewMainPage::on_wheelBtn_clicked(bool checked)
{

}

void NewMainPage::on_shutdownBtn_clicked()
{
    myMessageBox msgBox(MSG_QUES, g_myLan().ids_toShutdown);
    int ret = msgBox.exec();

    if (ret == QDialog::Accepted)
    {
        if(struGsh.bSortStart == 1)
        {
            g_infoWidget().setLabelText(g_myLan().msg_turning_off);

            g_infoWidget().delayShow();
            myFlow.onOff();
            g_infoWidget().hide();
        }
        struGsh.bUpdateState = 1;
        myFlow.shutdownIpc(true,0,0,0);

        g_infoWidget().setLabelText(g_myLan().msg_ipc_clean_filter_cotton);
        g_infoWidget().showSecs();

        //关机参数保存确认
        //    msgBox.setText(g_myLan().save_parameters);
        //    ret = msgBox.exec();
        //    if(ret == QDialog::Accepted)
        //    {
        g_infoWidget().setLabelText(g_myLan().save_parameters);
        g_infoWidget().delayShow();

        bool status = g_Runtime().save();

        g_infoWidget().hide();

        if(!status) {
            g_infoWidget().setLabelText(g_myLan().msg_params_save_error);
            g_infoWidget().delayShow();
            myFlow.sleep(2);
            g_infoWidget().hide();
            return;
        }
        if(struCnfg.nPowerCut == 1)
        {
            logger()->info("Plc Power Cut");
            char data[SEND_PACKET_RECTRL_BOARD_SIZE];
            memset(data, 0, SEND_PACKET_RECTRL_BOARD_SIZE);
            data[0] = ((int)(struCnfg.nDelayedShutdownTime))/256;
            data[1] = ((int)(struCnfg.nDelayedShutdownTime))%256;
            mySerial.pushCom2CmdData(CMD_UPPER_COMPUTER_OUTAGE_REQUEST, 0, BOARD_TYPE_CONTROL, 0x0e, data, 3);
            g_infoWidget().setLabelText(g_myLan().DelayPowerCut);
            g_infoWidget().delayShow();
            myFlow.sleep(3);
            g_infoWidget().hide();
        }
        g_infoWidget().setLabelText(g_myLan().ids_shutdown);
        g_infoWidget().delayShow();
#ifdef Q_OS_UNIX
//        myMqttMsgParaseThread->n_uploadLogFileFlag = 1;

        struGsh.poweroff_status = 1;
        QSettings setting(CFG_APPSet,QSettings::IniFormat);
        setting.setValue("poweroffstatus",struGsh.poweroff_status);
        setting.sync();
        myFlow.sleep(2);
        logger()->info("shutdown");
        struGsh.bFlagPowerOff = 1;
        system("shutdown -h now");
        myFlow.sleep(10);
#endif
        g_infoWidget().hide();
    }
}

void NewMainPage::on_senseBtn_clicked()
{
    g_MainManager().ShowWidget(SM_SENSE_SET_WIDGET);
}

void NewMainPage::on_feedBtn_clicked()
{
    g_MainManager().ShowWidget(SM_FEED_SET_WIDGET);
}

void NewMainPage::on_wipeBtn_clicked()
{
    g_MainManager().ShowWidget(SM_WIPE_SET_WIDGET);
}

void NewMainPage::on_vavleBtn_clicked()
{
    //g_MainManager().ShowWidget(SM_EJECT_TIME_WIDGET);
    g_MainManager().ShowWidget(SM_VAVLE_TEST_WIDGET);
}

void NewMainPage::on_modeBtn_clicked()
{
    g_MainManager().ShowWidget(SM_MODE_MANAGE_WIDGET);
}

void NewMainPage::on_setBtn_clicked()
{
    g_MainManager().ShowWidget(SM_ENGINEER_WIDGET);

}

void NewMainPage::updateUi()
{
    if(struGsh.bSortStart == 0){
        ui->runBtn->setText(g_myLan().ids_run);
    }else{
        ui->runBtn->setText(g_myLan().ids_stop);
    }
    ui->runBtn->setChecked(struGsh.bSortStart);
    //    ui->wheelBtn->setVisible(struCnfg.nMachineAttr == MACHINE_TYPE_BELT_DOUBLE);
    ui->wheelBtn->hide();
    ui->widget_5->setVisible(struCnfg.bMainpageShow == 1);

}


void NewMainPage::updateLevel()
{
    if(g_Runtime().g_level2 == 1){
        ui->senseBtn->hide();
        ui->feedBtn->hide();
        ui->wipeBtn->hide();
        ui->vavleBtn->hide();
        ui->modeBtn->hide();
        ui->setBtn->hide();
    }else if(g_Runtime().g_level2 == 2){
        ui->senseBtn->show();
        ui->feedBtn->show();
        ui->wipeBtn->hide();
        ui->vavleBtn->hide();
        ui->modeBtn->hide();
        ui->setBtn->hide();
    }else{
        ui->senseBtn->show();
        ui->feedBtn->show();
        ui->wipeBtn->show();
        //        ui->vavleBtn->show();
        ui->vavleBtn->setVisible(struGsh.bRunMode != RUNMODE_USER);
        ui->modeBtn->show();
        ui->setBtn->show();
    }
    ui->runBtn->setCheckable(!struCnfg.nPlcControl);

    if(CUSTOM == CUSTOM_YANSI){
        ui->wipeBtn->hide();
        ui->feedBtn->hide();
    }

}
void NewMainPage::updateshowdata()
{

    QMap<QString, double> front_data;
    QMap<QString, double> rear_data1;
    for(int i = 0; i < getThread->StaticClassNameList.size(); i++)
    {
        front_data.insert(getThread->StaticClassNameList.at(i), getThread->m_statictotalValueVec.at(i) );
    }
    ui->widget_2->setData(front_data,rear_data1);
    //    ui->widget_2->namelist = getThread->StaticClassNameList;
    //    for(int i = 0; i < getThread->StaticClassNameList.size(); i++)
    //    {
    //        name.append(getThread->StaticClassNameList.at(i));
    //        ui->widget_2->m_numList.append( getThread->m_statictotalValueVec.at(i) );
    //    }
    //    ui->widget_2->updateData();
}
