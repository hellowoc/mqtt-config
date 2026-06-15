#include "mainpagewidget.h"
#include "ui_mainpagewidget.h"
#include <QPainter>
#include "global.h"
MainPageWidget::MainPageWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::MainPageWidget)
{
    ui->setupUi(this);
}

MainPageWidget::~MainPageWidget()
{
    delete ui;
}

void MainPageWidget::showPage(bool bshow)
{
    if(bshow){
        updateUi();
        updateLevel2();
        updateFeederState();
    }
}

void MainPageWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_LEVEL_CHANGE){
        updateLevel2();
        updateFeederState();
    }else if(msg1 == MSG_OFF){
        updateFeederState();
    }
}

void MainPageWidget::retranslateUiWidget()
{
    setText();
}

void MainPageWidget::updateLevel2()
{
    ui->runBtn->show();
    ui->shutDownBtn->show();
    ui->senseSetBtn->show();
    ui->feedSetBtn->show();
    ui->cleanBtn->show();
    ui->valveBtn->show();
    ui->modeBtn->show();
    ui->setBtn->show();

    ui->valveBtn->setEnabled(struGsh.bRunMode != RUNMODE_USER);
}

void MainPageWidget::setText()
{
    updateFeederState();
    ui->shutDownBtn->updateText(g_myLan().ids_shutdown);
    ui->senseSetBtn->updateText(g_myLan().ids_select);
    ui->feedSetBtn->updateText(g_myLan().ids_feed);
    ui->cleanBtn->updateText(g_myLan().ids_cleaning);
    ui->valveBtn->updateText(g_myLan().ids_valve);
    ui->modeBtn->updateText(g_myLan().ids_model);
    ui->setBtn->updateText(g_myLan().ids_set);
}

void MainPageWidget::updateFeederState()
{
    if(struGsh.bSortStart == 0){
        //ui->runBtn->setPixFileStr(":/white/run_normal.png",":/white/run_down.png",4);
        ui->runBtn->updateText(g_myLan().ids_run);
        ui->runBtn->updateRun(false);
    }else{
        //ui->runBtn->setPixFileStr(":/white/stop_normal.png",":/white/stop_down.png",4);
        ui->runBtn->updateText(g_myLan().ids_stop);
        ui->runBtn->updateRun(true);
    }
}

void MainPageWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QString fileName;
#ifdef Q_OS_WIN
    fileName =QApplication::applicationDirPath()+QString("/logo/homepage.bmp");
#else
    fileName = QString("/sdcard/logo/homepage.bmp");
#endif
    painter.drawPixmap(rect(),QPixmap(fileName),QRect(0,50,1024,718));
}

void MainPageWidget::updateUi()
{
    ui->cleanBtn->setPixFileStr(QString(":/white/cleaning_normal.png"),":/white/cleaning_down.png",2);
    ui->senseSetBtn->setPixFileStr(":/white/select_normal.png",":/white/select_down.png",1);
    ui->modeBtn->setPixFileStr(":/white/memory_normal.png",":/white/memory_down.png",3);
    ui->runBtn->setPixFileStr(":/white/run_normal.png",":/white/run_down.png",4);
    ui->feedSetBtn->setPixFileStr(":/white/feed_normal.png",":/white/feed_down.png",5);
    ui->valveBtn->setPixFileStr(":/white/valve_normal.png",":/white/valve_down.png",6);
    ui->setBtn->setPixFileStr(":/white/set_normal.png",":/white/set_down.png",7);
    ui->shutDownBtn->setPixFileStr(":/white/shutdown_normal.png",":/white/shutdown_down.png",8);
}

void MainPageWidget::on_senseSetBtn_clicked()
{
    g_MainManager().ShowWidget(SM_SENSE_SET_WIDGET);
}

void MainPageWidget::on_cleanBtn_clicked()
{
     g_MainManager().ShowWidget(SM_WIPE_SET_WIDGET);
}

void MainPageWidget::on_feedSetBtn_clicked()
{
    g_MainManager().ShowWidget(SM_FEED_SET_WIDGET);
}

void MainPageWidget::on_valveBtn_clicked()
{
    //g_MainManager().ShowWidget(SM_EJECT_TIME_WIDGET);
    g_MainManager().ShowWidget(SM_VAVLE_TEST_WIDGET);
}

void MainPageWidget::on_setBtn_clicked()
{
    g_MainManager().ShowWidget(SM_ENGINEER_WIDGET);
}

void MainPageWidget::on_shutDownBtn_clicked()
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

            myFlow.sleep(1);
            g_infoWidget().hide();
        }

        if(struCnfg.nUseIpcEnable == 1)
        {
//            for(int i = 0; i < struCnfg.nIpcCount; i++)
//            {
//                myFlow.shutdownIpc(i);
//            }
            g_infoWidget().setLabelText(g_myLan().msg_ipc_clean_filter_cotton);
            g_infoWidget().showSecs();
        }

        //关机参数保存确认
        msgBox.setText(g_myLan().save_parameters);
        ret = msgBox.exec();
        if(ret == QDialog::Accepted)
        {
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
        }
        g_infoWidget().setLabelText(g_myLan().ids_shutdown);
        g_infoWidget().delayShow();
        myFlow.sleep(2);

        g_infoWidget().hide();
        this->close();
#ifndef Q_OS_WIN
        struGsh.bFlagPowerOff = 1;
        system("shutdown -h now");
#endif
    }
}

void MainPageWidget::on_runBtn_clicked()
{
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

            updateFeederState();
            return;
        }

        if(struCnfg.nUseIpcEnable == 1)
        {
            if(struIpcShare.allIpcModelLoadOk!= 2 || struIpcShare.allIpcConnecOk == 0)
            {
                g_infoWidget().setLabelText(g_myLan().msg_ipc_poweroff);
                g_infoWidget().delayShow();
                myFlow.sleep(2);
                g_infoWidget().hide();

                updateFeederState();
                return;
            }
        }

        myMessageBox msgBox(MSG_QUES, g_myLan().ids_toRun);

        int ret = msgBox.exec();

        if (ret == QDialog::Accepted)
        {
            logger()->info("sort start");
            g_infoWidget().setLabelText(g_myLan().msg_turning_on);

            g_infoWidget().delayShow();
            myFlow.onOff();

            myFlow.sleep(1);
            g_infoWidget().hide();
            updateFeederState();
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
        updateFeederState();
    }
}

void MainPageWidget::on_modeBtn_clicked()
{
    g_MainManager().ShowWidget(SM_MODE_MANAGE_WIDGET);
}
