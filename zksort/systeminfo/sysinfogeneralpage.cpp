#include "sysinfogeneralpage.h"

SysInfoGeneralPage::SysInfoGeneralPage(QWidget *parent ): QWidget(parent)
{
    CreateGeneralPage();

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(TimeOutSlt()));
    timer->start(1000);
}

void SysInfoGeneralPage::TimeOutSlt()
{
    updateGeneralPage();
    updateDateTime();
}

void SysInfoGeneralPage::updateDateTime()
{
    QString  str;
    QDateTime  dateTime = QDateTime::currentDateTime();

    data[0]          = dateTime.date().year();
    data[1]          = dateTime.date().month();
    data[2]          = dateTime.date().day();
    data[3]          = dateTime.time().hour();
    data[4]          = dateTime.time().minute();
    data[5]          = dateTime.time().second();


    str.append(QString("%1/%2/%3 %4:%5:%6").arg(data[0]).arg(data[1])\
            .arg(data[2]).arg(data[3]).arg(data[4]).arg(data[5]));

    currentTime->setStyleSheet("color:black");
    currentTime->setText(str);
}

void SysInfoGeneralPage::CreateGeneralPage()
{
    QString str = "";

    //本次运行时间
    thisRunLabel    = new QLabel(str,this);
    thisRunLabel->setMaximumHeight(30);

    //累计运行时间
    totalRunLabel   = new QLabel(str,this);
    totalRunLabel->setMaximumHeight(30);

    currentTime = new QLabel(str,this);
    currentTime->setMaximumHeight(30);

    //供料状态
    feedLabel       = new QLabel(str,this);
    feedLabel->setMaximumHeight(30);

    //喷阀状态
    ejectLabel      = new QLabel(str,this);
    ejectLabel->setMaximumHeight(30);

    //气压报警
    pressLabel  =  new QLabel(str,this);
    pressLabel->setMaximumHeight(30);

    //风机报警
    blowerLabel  =  new QLabel(str,this);
    blowerLabel->setMaximumHeight(30);

    //灯报警
    lampLabel  =  new QLabel(str   ,this);
    lampLabel->setMaximumHeight(30);

    //皮带状态
    beltStatus[0]  =  new QLabel(str   ,this);
    beltStatus[0]->setMaximumHeight(30);

    beltStatus[1]  =  new QLabel(str   ,this);
    beltStatus[1]->setMaximumHeight(30);

    generalVBLayout = new QVBoxLayout(this);
    generalVBLayout->setSpacing(11);
    verticalSpacer_1 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    generalVBLayout->addWidget(thisRunLabel);
    generalVBLayout->addWidget(totalRunLabel);
    generalVBLayout->addWidget(currentTime);
    generalVBLayout->addWidget(feedLabel);
    generalVBLayout->addWidget(ejectLabel);
    generalVBLayout->addWidget(pressLabel);
    generalVBLayout->addWidget(blowerLabel);
    generalVBLayout->addWidget(lampLabel);
    generalVBLayout->addWidget(beltStatus[0]);
    generalVBLayout->addWidget(beltStatus[1]);
    generalVBLayout->addItem(verticalSpacer_1);

    updateGeneralPage();
}


void SysInfoGeneralPage::updateGeneralPage()
{
    QString str;

    //本次运行时间
    str = QString("%1: %2:%3:%4").arg(g_myLan().ids_thisRunTime).arg(struGsh.nCounter/3600).arg((struGsh.nCounter%3600)/60).arg(struGsh.nCounter%60);
    thisRunLabel->setText(str);

    //累计运行时间
    str = QString("%1: %2:%3:%4").arg(g_myLan().ids_totalRunTime).arg(struCnfg.nCounter/3600).arg((struCnfg.nCounter%3600)/60).arg(struCnfg.nCounter%60);
    totalRunLabel->setText(str);

    //当前系统时间
    updateDateTime();

    //运行状态
    if (struGsh.bSortStart){
        str = QString("%1: %2").arg(g_myLan().ids_feedState).arg(g_myLan().on);
        feedLabel->setStyleSheet("color:red");
    }
    else {
        str = QString("%1: %2").arg(g_myLan().ids_feedState).arg(g_myLan().off);
        feedLabel->setStyleSheet("color:black");
    }
    feedLabel->setText(str);

    //喷阀状态
    if (struGsh.bStatEject){
        str = QString("%1: %2").arg(g_myLan().eject_stat).arg(g_myLan().on);
        ejectLabel->setStyleSheet("color:red");
    }
    else {
        str = QString("%1: %2").arg(g_myLan().eject_stat).arg(g_myLan().off);
        ejectLabel->setStyleSheet("color:black");
    }
     ejectLabel->setText(str);

     //气压报警
     if (1) {
         str = QString("%1: %2").arg(g_myLan().ids_airpressureAlarm).arg(g_myLan().abnormal);
         pressLabel->setStyleSheet("color:red");
     }
     else {
         str = QString("%1: %2").arg(g_myLan().ids_airpressureAlarm).arg(g_myLan().normal);
         pressLabel->setStyleSheet("color:black");
     }
     pressLabel->setText(str);


    beltStatus[1]->setText(str);

    /* 显示或屏蔽报警状态   */
    if(struCnfg.nAlarmValid[0]) {
        pressLabel->show();
    }
    else {
        pressLabel->hide();
    }
    if(struCnfg.nAlarmValid[1]) {
        blowerLabel->show();
    }
    else {
        blowerLabel->hide();
    }

    if(struCnfg.nAlarmValid[2]) {
        lampLabel->show();
    }
    else {
        lampLabel->hide();
    }
    if(struCnfg.nAlarmValid[3]) {
        beltStatus[0]->show();
        beltStatus[1]->show();
    }
    else {
        beltStatus[0]->hide();
        beltStatus[1]->hide();
    }
}
