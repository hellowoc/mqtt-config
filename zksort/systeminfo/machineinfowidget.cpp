#include "machineinfowidget.h"
#include "ui_machineinfowidget.h"
#include "mysetdatetime.h"
MachineInfoWidget::MachineInfoWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::MachineInfoWidget)
{
    ui->setupUi(this);

    feedLevelLabel[0] = ui->feedlevellabel1;
    feedLevelLabel[1] = ui->feedlevellabel2;
    feedLevelLabel[2] = ui->feedlevellabel3;
    feedLevelLabel[3] = ui->feedlevellabel4;
    feedLevelLabel[4] = ui->feedlevellabel5;
    feedLevelLabel[5] = ui->feedlevellabel6;
    feedLevelLabel[6] = ui->feedlevellabel7;
    feedLevelLabel[7] = ui->feedlevellabel8;

    tempSensorLabel[0] = ui->ctrlboardTempSensorValue;
    tempSensorLabel[1] = ui->tempSensorValue1;
    tempSensorLabel[2] = ui->tempSensorValue2;
    tempSensorLabel[3] = ui->tempSensorValue3;
    tempSensorLabel[4] = ui->tempSensorValue4;

    myTimer = new QTimer(this);
    connect(myTimer,SIGNAL(timeout()),this,SLOT(TimeOutSlt()));

    ui->pushButton->hide();
    ui->ipcalarmlabel->hide();
}

MachineInfoWidget::~MachineInfoWidget()
{
    delete ui;
}

void MachineInfoWidget::showPage(bool bshow)
{
    if (bshow){
        myTimer->start(1000);
        updateInfo();

    }
    else{
        myTimer->stop();
    }
}

void MachineInfoWidget::retranslateUiWidget()
{
    updateInfo();
}

void MachineInfoWidget::TimeOutSlt()
{
    updateInfo();
}

void MachineInfoWidget::updateInfo()
{
    QString str = "";

    //本次运行时间
    str = QString("%1 : %2:%3:%4").arg(g_myLan().ids_thisRunTime).arg(struGsh.nCounter/3600).arg((struGsh.nCounter%3600)/60).arg(struGsh.nCounter%60);
    ui->thisRunLabel->setText(str);

    //累计运行时间
    str = QString("%1 : %2:%3:%4").arg(g_myLan().ids_totalRunTime).arg(struCnfg.nCounter/3600).arg((struCnfg.nCounter%3600)/60).arg(struCnfg.nCounter%60);
    ui->totalRunLabel->setText(str);

    //IP
    QSettings setting(CFG_APPSet,QSettings::IniFormat);
    setting.beginGroup("eth0");
    QString ipStr0 =  setting.value("ipStr").toString();
    setting.endGroup();
    setting.beginGroup("eth1");
    QString ipStr1 =  setting.value("ipStr").toString();
    setting.endGroup();

    if(paramDelayCode30Day.product30DayUseStartEnable == 0xFA)   //全解过的机器
    {
        ui->label->show();
        QString code =  setting.value("delaycode").toString();
        ui->label->setText(code);
    }
    else
    {
        ui->label->hide();
    }
    setting.sync();

    str = "IP1 : "+ ipStr0;
    ui->IP1Label->setText(str);
    str = "IP2 : " +ipStr1;
    ui->IP2Label->setText(str);

    //下料状态
    if(struGsh.bSortStart == 0)
    {
        str = QString("%1 : %2").arg(g_myLan().ids_feedState).arg(g_myLan().ids_stop);
    }
    else
    {
        str = QString("%1 : %2").arg(g_myLan().ids_feedState).arg(g_myLan().ids_run);
    }
    ui->feedStatusLabel->setText(str);

    //供料报警
    if(0)
    {
        str = QString("%1 : %2").arg(g_myLan().ids_feedAlarm).arg(g_myLan().abnormal);
        ui->feedAlarmLabel->setStyleSheet("color:red");
    }
    else
    {
        str = QString("%1 : %2").arg(g_myLan().ids_feedAlarm).arg(g_myLan().normal);
        ui->feedAlarmLabel->setStyleSheet("color:blue");
    }
    ui->feedAlarmLabel->setText(str);

    //清灰报警
    if(0)
    {
        str = QString("%1 : %2").arg(g_myLan().ids_cleaningAlarm).arg(g_myLan().abnormal);
        ui->wipeAlarmLabel->setStyleSheet("color:red");
    }
    else
    {
        str = QString("%1 : %2").arg(g_myLan().ids_cleaningAlarm).arg(g_myLan().normal);
        ui->wipeAlarmLabel->setStyleSheet("color:blue");
    }
    ui->wipeAlarmLabel->setText(str);

    //气压报警
    if (struGsh.bAlarmStatus)
    {
        str = QString("%1 : %2").arg(g_myLan().ids_airpressureAlarm).arg(g_myLan().abnormal);
        ui->pressureLabel->setStyleSheet("color:red");
    }
    else
    {
        str = QString("%1 : %2").arg(g_myLan().ids_airpressureAlarm).arg(g_myLan().normal);
        ui->pressureLabel->setStyleSheet("color:blue");
    }
    ui->pressureLabel->setText(str);

    int hour = struCnfg.ntotalFeedTime/3600;
    int minute = struCnfg.ntotalFeedTime%3600/60;
    int second = struCnfg.ntotalFeedTime%60;
    QString timeFormatStr = QString("%1:%2:%3").arg(hour).arg(minute).arg(second);
    str = QString("%1 : %2").arg(g_myLan().total_feed_time).arg(timeFormatStr);

    ui->totalFeedTimeLabel->setText(str);

    str = QString("%1 : %2").arg(g_myLan().cur_pressure_alarm_times).arg(struGsh.curAlarmCount);
    ui->curpressureAlarmNum->setText(str);

    str = QString("%1 : %2").arg(g_myLan().total_pressure_alarm_times).arg(struCnfg.ntotalAlarmCount);
    ui->totalPressureLabel->setText(str);
    //料位报警
    if (struGsh.nAlarmStautsCurrent[ALARM_FEED_LEVEL] == 1)
    {
        str = QString("%1 : %2").arg(g_myLan().ids_materialLevelAlarm).arg(g_myLan().abnormal);
        ui->levelAlarmLabel->setStyleSheet("color:red");
    }
    else
    {
        str = QString("%1 : %2").arg(g_myLan().ids_materialLevelAlarm).arg(g_myLan().normal);
        ui->levelAlarmLabel->setStyleSheet("color:blue");
    }
    ui->levelAlarmLabel->setText(str);

#ifdef Q_OS_UNIX
    ui->mqttConnectLabel->setVisible(!myHttpFileClient->g_connectName.isEmpty());
#else
    ui->mqttConnectLabel->hide();
#endif
    if(struGsh.bFlagMqttConnect)
        ui->mqttConnectLabel->setText("Mqtt : "+g_myLan().msg_network_success);
    else
        ui->mqttConnectLabel->setText("Mqtt : "+g_myLan().msg_network_fail);

    //工控机报警
    if (struGsh.bAlarmIpcStatus[0]||struGsh.bAlarmIpcStatus[1])
    {
        str = QString("%1 : %2").arg("IPC").arg(g_myLan().abnormal);
        ui->ipcalarmlabel->setStyleSheet("color:red");
    }
    else
    {
        str = QString("%1 : %2").arg("IPC").arg(g_myLan().normal);
        ui->ipcalarmlabel->setStyleSheet("color:blue");
    }
    ui->ipcalarmlabel->setText(str);

    /* 显示或屏蔽报警状态   */
    if (struCnfg.nAlarmValid[ALARM_FEED])
    {
        ui->feedAlarmLabel->show();
    }
    else
    {
        ui->feedAlarmLabel->hide();
    }
    if (struCnfg.nAlarmValid[ALARM_WIPE])
    {
        ui->wipeAlarmLabel->show();
    }
    else
    {
        ui->wipeAlarmLabel->hide();
    }
    if(struCnfg.nAlarmValid[ALARM_PRESSURE])
    {
        ui->pressureLabel->show();
    }
    else
    {
        ui->pressureLabel->hide();
    }
    if(struCnfg.nAlarmValid[ALARM_FEED_LEVEL])
    {
        ui->levelAlarmLabel->show();
    }
    else
    {
        ui->levelAlarmLabel->hide();
    }


    if(struCnfg.struAlarmConfig.nAirPressureAlarmType == 1)
    {
        str = QString("%1:").arg(g_myLan().pressure_value)+ QString::number(struGsh.struMachineStatus[struGsh.nLayer].nPressureValue,'f',3);
        ui->pressureValue->setText(str);
    }
    else
    {
        ui->pressureValue->hide();
    }

    str = QString("%1: %2").arg(g_myLan().gasConsumption).arg( struGsh.struMachineStatus[struGsh.nLayer].nMachineUseAir);
    ui->machineUseAir->setText(str);
    ui->machineUseAir->hide();

    for(int i = 0; i < CTRL_ALARM_MAX_TEMP_SENSOR; i++)
    {
        if(struCnfg.struAlarmConfig.nTemptureAlarmEnable[i] == 1)
        {
            tempSensorLabel[i]->show();
            str = QString("%1:").arg(struCnfg.struAlarmConfig.nTemptureAlarmName[i])+ QString::number(struGsh.struMachineStatus[struGsh.nLayer].nTempSensorValue[i],'f',1);

            if(struGsh.struMachineStatus[struGsh.nLayer].nTempSensorValue[i] > struCnfg.struAlarmConfig.nTemptureAlarmLimit[i])
            {
                tempSensorLabel[i]->setStyleSheet("color:red");
            }
            else
            {
                tempSensorLabel[i]->setStyleSheet("color:blue");
            }
            tempSensorLabel[i]->setText(str);
        }
        else
        {
            tempSensorLabel[i]->hide();
        }
    }

    for(int i = 0; i < CTRL_ALARM_MAX_FEED_SENSOR; i++)
    {
        if(struCnfg.struAlarmConfig.nFeederLevelAlarmEnable[i] == 1)
        {
            feedLevelLabel[i]->show();
            if(struGsh.struMachineStatus[struGsh.nLayer].bAlarmFeedLevel[i] == 1)
            {
                str = QString("%1 : %2").arg(struCnfg.struAlarmConfig.nFeederLevelAlarmName[i]).arg(g_myLan().abnormal);
                feedLevelLabel[i]->setStyleSheet("color:red");
            }
            else
            {
                str = QString("%1 : %2").arg(struCnfg.struAlarmConfig.nFeederLevelAlarmName[i]).arg(g_myLan().normal);
                feedLevelLabel[i]->setStyleSheet("color:blue");
            }
            feedLevelLabel[i]->setText(str);
        }
        else
        {
            feedLevelLabel[i]->hide();
        }
    }


    if(struGsh.strumachineset.targetCount != 0)
    {
        ui->label_2->show();
        QString tmp;
        tmp += QString(struGsh.strumachineset.machinetype);

        for(int i = 0; i < struGsh.strumachineset.targetCount; i++)
        {
            tmp += ":" + QString::number(struGsh.strumachineset.targetlightValue[i]);

        }
        ui->label_2->setText(tmp);
    }
    else
    {
        ui->label_2->hide();
    }

    if(g_Runtime().checkUsbExist()){
        QString path = g_Runtime().getUsbPath();
        if (path != ""){
            ui->label_3->setText("USB:"+QString::number(struCnfg.nFreeSpace) + "M");
        }
        else
            ui->label_3->setText("SD:"+QString::number(struCnfg.nFreeSpace)+ "M");
    }
    else
    {
        ui->label_3->setText("SD:"+QString::number(struCnfg.nFreeSpace)+ "M");
    }

#if 0
#ifdef Q_OS_UNIX
    if(g_Runtime().checkUsbExist()){
        QString bmpCnt="0";
        QStringList sdSpace;
        QString path = g_Runtime().getUsbPath();
        if (path != ""){
            QString fileName = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
            QString filePath;
            QString imagePath = path+"IMG/"+fileName.left(8);
            QFileInfo fileinfo(imagePath);
            if(fileinfo.exists()){
                QProcess m_cmd;
                QString cmd = "ls -lR "+imagePath+"|grep ^-|grep .bmp|wc -l";
                m_cmd.start("sh",QStringList()<<"-c"<<cmd);
                m_cmd.waitForFinished();
                bmpCnt = QLatin1String(m_cmd.readAll());
                bmpCnt.replace("\n","");

                sdSpace = g_Runtime().getUsbSpace("/dev/sd");
                if (sdSpace.size() > 5)
                    ui->sdLabel->setText("SD: "+bmpCnt+"  "+sdSpace.at(2)+"/"+sdSpace.at(1));
            }

            //        m_cmd.start("df -h");
            //        m_cmd.waitForFinished();
            //        while (!m_cmd.atEnd()) {
            //            QString result = QLatin1String(m_cmd.readLine());
            //            if (result.startsWith("/dev/sd")) {
            //                result.replace("\n","");
            //                result.replace(QRegExp("( ){1,}")," ");
            //                sdSpace = result.split(" ");
            //            }
            //        }
        }
        else{
            ui->sdLabel->setText("");
        }
    }
    else{
        else{
            ui->sdLabel->setText("");
        }
    }
#endif
#endif


}

void MachineInfoWidget::on_pushButton_clicked()
{
    if(g_Runtime().checkUsbExist()){
        QString bmpCnt="0";
        QStringList sdSpace;
        QString path = g_Runtime().getUsbPath();
        if (path != ""){
            QString fileName = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
            QString filePath;
            QString imagePath = path+"IMG/"+fileName.left(8);
            QFileInfo fileinfo(imagePath);
            if(fileinfo.exists()){
                g_infoWidget().setLabelText(g_myLan().ids_query+"...");
                QProcess m_cmd;
                QString cmd = "ls -lR "+imagePath+"|grep ^-|grep .bmp|wc -l";
                m_cmd.start("sh",QStringList()<<"-c"<<cmd);
                m_cmd.waitForFinished();
                bmpCnt = QLatin1String(m_cmd.readAll());
                bmpCnt.replace("\n","");

                sdSpace = g_Runtime().getUsbSpace("/dev/sd");
                if (sdSpace.size() > 5){
                    g_infoWidget().setLabelText("SD: "+bmpCnt+"  "+sdSpace.at(2)+"/"+sdSpace.at(1));
                    g_infoWidget().showSecs();
                }
                //                m_cmd.start("df -h");
                //                m_cmd.waitForFinished();
                //                while (!m_cmd.atEnd()) {
                //                    QString result = QLatin1String(m_cmd.readLine());
                //                    if (result.startsWith("/dev/sd")) {
                //                        result.replace("\n","");
                //                        result.replace(QRegExp("( ){1,}")," ");
                //                        sdSpace = result.split(" ");
                //                    }
                //                }
            }
        }
    }
}
