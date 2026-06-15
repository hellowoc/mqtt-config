#include "engineerwidget.h"
#include "ui_engineerwidget.h"
#include <QToolButton>
#include "global.h"
#include "onekeyfixdialog.h"
EngineerWidget::EngineerWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::EngineerWidget)
{
    ui->setupUi(this);


}

EngineerWidget::~EngineerWidget()
{
    delete ui;
}


void EngineerWidget::retranslateUiWidget()
{
    ui->groupBox_date->setTitle(g_myLan().system_time);
    ui->label_year->setText(g_myLan().year);
    ui->label_month->setText(g_myLan().month);
    ui->label_day->setText(g_myLan().day);
    ui->label_hour->setText(g_myLan().hour);
    ui->label_minute->setText(g_myLan().minute);
    ui->label_second->setText(g_myLan().sec);
    ui->pushButton->setText(g_myLan().ids_system);
    ui->pushButton_2->setText(g_myLan().ids_background);
    ui->pushButton_3->setText(g_myLan().ids_image);
    ui->pushButton_4->setText(g_myLan().advance);
    ui->pushButton_5->setText(g_myLan().lighting);
    ui->radioButton_user->setText(g_myLan().ids_operator);
    ui->radioButton_engineer->setText(g_myLan().ids_engineer);
    ui->groupBox_level->setTitle(g_myLan().set_purview);
    ui->pushButton_6->setText(g_myLan().ids_calibration);
    ui->radioButton_factory->setText(g_myLan().factory_set);
    ui->pushButton_7->setText(g_myLan().dc_set);
    ui->pushButton_8->setText(g_myLan().bad_point);
    ui->plcCtrlBtn->setText(g_myLan().plc_ctrl);
    ui->pushButton_9->setText(g_myLan().ids_statisticsInfo);
}

void EngineerWidget::showPage(bool bShow)
{
    if(bShow)
    {
        updateTimeEdit();
        updateLevelRadioBtn();
        ui->plcCtrlBtn->setVisible(struCnfg.bautoplcctrlEnable);

        if(g_myLan().g_languageIndex != LANG_CHS)
        {
             ui->onekeyfix->hide();
        }
    }
    basewidget::showPage(bShow);
}

void EngineerWidget::receiveMsg(int msg1, int msg2, int msg3)
{
#ifdef Q_OS_UNIX
    char time[64];
    sprintf(time, "date -s \"%04d-%02d-%02d %02d:%02d:%02d\"", data[0], data[1], data[2],
            data[3], data[4], data[5]);
    system(time);
    popen("hwclock -w","r");
#endif
}


void EngineerWidget::on_pushButton_2_clicked()
{
    g_MainManager().ShowWidget(SM_BACKGROUND_SET_WIDGET);
}

void EngineerWidget::on_pushButton_3_clicked()
{
    g_MainManager().ShowWidget(SM_IMAGE_CAPTURE_WIDGET);
}

void EngineerWidget::on_pushButton_4_clicked()
{
    logger()->info("enter high level page");
    g_MainManager().ShowWidget(SM_FACTORY_SET_WIDGET);
}

void EngineerWidget::updateTimeEdit()
{
    QDateTime  dateTime = QDateTime::currentDateTime();

       data[0]          = dateTime.date().year();
       data[1]          = dateTime.date().month();
       data[2]          = dateTime.date().day();
       data[3]          = dateTime.time().hour();
       data[4]          = dateTime.time().minute();
       data[5]          = dateTime.time().second();
       ui->lineEditYear->setText(QString::number(data[0]));
       ui->lineEditYear->setMinMax(this,0,2100,0,&data[0]);
       ui->lineEditMonth->setText(QString::number(data[1]));
       ui->lineEditMonth->setMinMax(this,0,12,1,&data[1]);
       ui->lineEditDay->setText(QString::number(data[2]));
       ui->lineEditDay->setMinMax(this,0,31,2,&data[2]);

       ui->lineEditHour->setText(QString::number(data[3]));
       ui->lineEditHour->setMinMax(this,0,23,3,&data[3]);
       ui->lineEditMinute->setText(QString::number(data[4]));
       ui->lineEditMinute->setMinMax(this,0,59,4,&data[4]);
       ui->lineEditSecond->setText(QString::number(data[5]));
       ui->lineEditSecond->setMinMax(this,0,59,5,&data[5]);

       if((paramDelayCode.enable == 0xAA)||(paramDelayCode30Day.product30DayUseStartEnable == 0xAA))
       {
           ui->lineEditYear->setEnabled(false);
           ui->lineEditMonth->setEnabled(false);
           ui->lineEditDay->setEnabled(false);
           ui->lineEditHour->setEnabled(false);
           ui->lineEditMinute->setEnabled(false);
           ui->lineEditSecond->setEnabled(false);

       }else{
           ui->lineEditYear->setEnabled(true);
           ui->lineEditMonth->setEnabled(true);
           ui->lineEditDay->setEnabled(true);
           ui->lineEditHour->setEnabled(true);
           ui->lineEditMinute->setEnabled(true);
           ui->lineEditSecond->setEnabled(true);
       }
}

void EngineerWidget::updateLevelRadioBtn()
{
    ui->radioButton_engineer->setChecked(struGsh.bRunMode == RUNMODE_ENGINEER);
    ui->radioButton_user->setChecked(struGsh.bRunMode == RUNMODE_USER);
    ui->radioButton_factory->setChecked(struGsh.bRunMode == RUNMODE_FACTORY||struGsh.bRunMode == RUNMODE_ADMIN);

    ui->pushButton->setVisible(struGsh.bRunMode != RUNMODE_USER);
    ui->pushButton_2->setVisible(struGsh.bRunMode != RUNMODE_USER);
    ui->pushButton_3->setVisible(struGsh.bRunMode != RUNMODE_USER);
    ui->pushButton_4->setVisible(struGsh.bRunMode != RUNMODE_USER && struGsh.bRunMode != RUNMODE_ENGINEER);
    ui->pushButton_5->setVisible(struGsh.bRunMode != RUNMODE_USER);
    ui->pushButton_8->setVisible(struGsh.bRunMode != RUNMODE_USER && struCnfg.nDPMachine == 1);
    ui->pushButton_7->setVisible(struGsh.bRunMode != RUNMODE_USER);
    ui->pushButton_9->setVisible(struGsh.bRunMode != RUNMODE_USER && struCnfg.nDPMachine == 1);
}

void EngineerWidget::on_radioButton_engineer_clicked(bool checked)
{
    if(checked){
#ifdef Q_OS_UNIX
        if(struGsh.bRunMode == RUNMODE_ENGINEER)
        {
        }
        else
        {
            myInputMethod inputPanel;
            inputPanel.setType(KB_PD);
            inputPanel.setInputBtnText(g_myLan().ids_ok,g_myLan().ids_cancel,g_myLan().ids_delete,g_myLan().ids_space);
            if(inputPanel.exec() == QDialog::Accepted)
            {
                QString str = inputPanel.getText();
                if (str == "123.321"||str == "123321")
                {
                    struGsh.bFlagSecondPwdValid = 1;
                    struGsh.bRunMode = RUNMODE_ENGINEER;
                }
                else
                {
                    g_infoWidget().setLabelText(g_myLan().ids_wrongPassword);
                    g_infoWidget().delayShow();
                    myFlow.sleep(2);
                    g_infoWidget().hide();
                }
            }
        }
#else
        struGsh.bRunMode = RUNMODE_ENGINEER;
        struGsh.bFlagSecondPwdValid = 1;
        struGsh.bFlagAutowipe = 0;
#endif

    }
    updateLevelRadioBtn();
}

void EngineerWidget::on_radioButton_user_clicked(bool checked)
{
    if(checked){
        struGsh.bRunMode = RUNMODE_USER;
        struGsh.bFlagSecondPwdValid = 0;
    }
    updateLevelRadioBtn();
}

void EngineerWidget::on_pushButton_5_clicked()
{
//    if(struCnfg.nMachineType == MACHINE_LD2_ZL_GP)
//    {
        g_MainManager().ShowWidget(SM_DOUBLE_LIGHE_SET_WIDGET);
//    }
//    else
//    {
//        g_MainManager().ShowWidget(SM_LIGHE_SET_WIDGET);
//    }
}

void EngineerWidget::on_pushButton_6_clicked()
{
    myMessageBox *mesBox = new myMessageBox(MSG_QUES, g_myLan().cfm_ts_calibrate);
    int ret = mesBox->exec();
    if(ret == QDialog::Accepted){
        logger()->info("calibrate screen");
        mesBox->setText(g_myLan().ids_toSaveParameters);
        if(mesBox->exec() == QDialog::Accepted){
            g_infoWidget().setLabelText(g_myLan().save_parameters);
            g_infoWidget().delayShow();
            g_Runtime().save();
            myFlow.sleep(1);
            g_infoWidget().hide();
        }

        struGsh.poweroff_status = 1;
        QSettings setting(CFG_APPSet,QSettings::IniFormat);
        setting.setValue("poweroffstatus",struGsh.poweroff_status);
        setting.sync();

        delete mesBox;
        mesBox = nullptr;
        this->close();
        myFlow.msleep(100);
        QProcess::startDetached("/app/mycalibrate.sh");
        qApp->quit();
    }
}

void EngineerWidget::on_radioButton_factory_clicked(bool checked)
{
    if(checked){
#ifdef Q_OS_UNIX
        if(struGsh.bRunMode == RUNMODE_FACTORY)
        {
        }
        else
        {
            myInputMethod inputPanel;
            inputPanel.setType(KB_PD);

            inputPanel.setInputBtnText(g_myLan().ids_ok,g_myLan().ids_cancel,g_myLan().ids_delete,g_myLan().ids_space);
            if(inputPanel.exec() == QDialog::Accepted)
            {
                QString str = inputPanel.getText();
                if(PRODUCT_DEFAULT_VENDOR == PRODUCT_VENDOR_LV){
                    if (str == "LV123.321")
                    {
                        struGsh.bRunMode = RUNMODE_FACTORY;
                    }
                    else
                    {
                        g_infoWidget().setLabelText(g_myLan().ids_wrongPassword);
                        g_infoWidget().delayShow();
                        myFlow.sleep(2);
                        g_infoWidget().hide();
                    }
                }else{
                    QString dateText = QDateTime::currentDateTime().toString("h:mm:ss:yyyy:MM:dd");
                    QStringList list = dateText.split(":");
                    if (str == list.at(5)+list.at(0)+list.at(1))
                    {
                        struGsh.bRunMode = RUNMODE_FACTORY;
                    }
                    else
                    {
                        g_infoWidget().setLabelText(g_myLan().ids_wrongPassword);
                        g_infoWidget().delayShow();
                        myFlow.sleep(2);
                        g_infoWidget().hide();
                    }
                }
            }
        }
#else
        struGsh.bRunMode = RUNMODE_FACTORY;
        struGsh.bFlagAutowipe = 0;
#endif
    }
    updateLevelRadioBtn();
}

void EngineerWidget::on_pushButton_7_clicked()
{
    myEncrypt dlg;

    bool bFlagEna = true;

    if (paramDelayCode.enable != 0xAA) {
        bFlagEna = true;
    }
    else {
        bFlagEna = false;
    }
    int ret = dlg.exec();
    if (ret == QDialog::Accepted){
        if (bFlagEna) { //未加密过的机器，启动实时纪录系统时间
           /*启动定时保存参数功能*/
           QProcess *process = new QProcess;
           QStringList strList;
           strList<<"";
           process->startDetached("./MyTimer",strList);
        }
    }
}

void EngineerWidget::on_pushButton_8_clicked()
{
    g_MainManager().ShowWidget(SM_IPC_EJECT_WIDGET);
}

void EngineerWidget::on_onekeyfix_clicked()
{
    OneKeyDialog aiset;
    int ret = aiset.exec();
}
void EngineerWidget::on_plcCtrlBtn_clicked()
{
    myMessageBox *mesBox = new myMessageBox(MSG_QUES, g_myLan().cfm_open_plc_ctrl);
    int ret = mesBox->exec();
    if(ret == QDialog::Accepted){
        g_MainManager().ShowWidget(SM_PLC_CTRL_WIDGET);
        return;
    }
    else{
        return;
    }
}
void EngineerWidget::on_pushButton_9_clicked()
{
    logger()->info("enter IPC OUTPUT page");
    g_MainManager().ShowWidget(SM_IPC_OUTPUT_WIDGET);
}
