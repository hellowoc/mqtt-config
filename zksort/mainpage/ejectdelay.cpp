#include "ejectdelay.h"
#include "ui_ejectdelay.h"

ejectDelay::ejectDelay(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::ejectDelay)
{
    ui->setupUi(this);

    curView  = 0;
    curVavle = 0;
    curGroup = 0;

    m_delay = (float)(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].nEjectDelay/10.0);
    m_delay2 = (float)(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].nEjectDelayShape/10.0);
    m_delay3 = (float)(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].nEjectDelay3/10.0);
    m_delay4 = (float)(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].nEjectDelay4/10.0);
    m_duration = (float)(struCnfp.struGroupTick[struGsh.nLayer][struGsh.nVavle][struGsh.nMatGroup].nEjectDuration/10.0);
    m_duration2 = (float)(struCnfp.struGroupTick[struGsh.nLayer][struGsh.nVavle][struGsh.nMatGroup].nEjectDuration2/10.0);

    m_expandEnable = true;

    m_pLayerBtn[0] = ui->mainBtn;
    m_pLayerBtn[1] = ui->viceBtn;
    m_pGroupWidget = ui->groupWidget;

    m_pViewBtn[0]= ui->pushButton_0;
    m_pViewBtn[1]= ui->pushButton_1;
    m_pViewBtn[2]= ui->pushButton_4;
    m_pViewBtn[3]= ui->pushButton_5;
    m_pViewBtn[4]= ui->pushButton_6;
    m_pViewBtn[5]= ui->pushButton_7;
    m_pViewBtn[6]= ui->pushButton_8;
    m_pViewBtn[7]= ui->pushButton_9;

    setSlotConnect();

    shottimer = new QTimer;
    connect(shottimer,SIGNAL(timeout()),this, SLOT(getscreenshot()));

}

ejectDelay::~ejectDelay()
{
    delete ui;
}

void ejectDelay::showPage(bool bshow)
{
    if(bshow){
        //        ui->ejectDelayWidget->setTitle(g_myLan().ejectdelay"));
        //        ui->ejectDurationWidget->setTitle(g_myLan().ejectduration"));

        //        ui->expandTickCheckBox->setChecked(struCnfp.struGroupTick[struGsh.nLayer][struGsh.nVavle][struGsh.nMatGroup].nEjwidth == 1);

        curView = struGsh.nView;
        curGroup = struGsh.nMatGroup;
        curVavle = struGsh.nVavle;

        struGsh.nView = 0;
        struGsh.nMatGroup = 0;
        struGsh.nVavle = 0;

        basewidget::showPage(bshow);
        for(int i = 0; i < MAX_VIEW; i++)
        {
            if(struCnfg.nVavleTimeSetMode == 1)
            {
                m_pViewBtn[i]->hide();
            }
            else
            {
                m_pViewBtn[i]->setVisible((bool)struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[i]);
            }
        }

        updateGroup();

        if(struGsh.bScreenShotStatus == 1)
        {
            shottimer->start(1000);

        }
        //        updateGroupValue();
    }
    else
    {
        struGsh.nView = 0;
        struGsh.nMatGroup = 0;
        struGsh.nVavle = 0;
    }
}

void ejectDelay::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){
        curVavle = (struCnfg.nMachineType == MACHINE_LD2_ZL_GP) ? struGsh.nView/4 : 0;

        if(msg2 == Delay)
        {
            int tmpDelay = (int)((m_delay+0.01)*10);
            int mis = 0;

            if(struCnfg.nDPMachine){
                mis = tmpDelay-(struCnfp.struGroupTick[struGsh.nLayer][curVavle][struGsh.nMatGroup].nEjectDuration2+1)/2;
            }
            else
                mis = tmpDelay-struCnfp.struGroupTick[struGsh.nLayer][curVavle][struGsh.nMatGroup].nEjectDuration/2;

            if((mis < 0))
            {
                g_infoWidget().setLabelText(g_myLan().msg_delay_time_limit);
                g_infoWidget().delayShow();

                m_delay = (float)(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].nEjectDelay/10.0);
                updateGroup();

                myFlow.sleep(1);
                g_infoWidget().hide();
            }
            else
            {
                struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].nEjectDelay = tmpDelay;

                if((struCnfg.nVavleTimeSetMode == 1)&&(struGsh.nView == 0))
                {
                    for(int i = 1; i < struCnfg.struLayerInfo[struGsh.nLayer].nViewChainTotal; i++)
                    {
                        struCnfp.struGroupIdentify[struGsh.nLayer][i][struGsh.nMatGroup].nEjectDelay = struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].nEjectDelay;
                    }
                }
                myFlow.resetEjectDelayTimeRG();
            }
        }
        else if(msg2 == Delay2)
        {
            int tmpDelay2 = (int)((m_delay2+0.01)*10);

            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].nEjectDelayShape = tmpDelay2;

            if((struCnfg.nVavleTimeSetMode == 1)&&(struGsh.nView == 0))
            {
                for(int i = 1; i < struCnfg.struLayerInfo[struGsh.nLayer].nViewChainTotal; i++)
                {
                    struCnfp.struGroupIdentify[struGsh.nLayer][i][struGsh.nMatGroup].nEjectDelayShape = struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].nEjectDelayShape;
                }
            }
            myFlow.resetEjectDelayTimeRG();
        }
        else if(msg2 == Delay3)
        {
            int tmpDelay = (int)((m_delay3+0.01)*10);
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].nEjectDelay3 = tmpDelay;

            if((struCnfg.nVavleTimeSetMode == 1)&&(struGsh.nView == 0))
            {
                for(int i = 1; i < struCnfg.struLayerInfo[struGsh.nLayer].nViewChainTotal; i++)
                {
                    struCnfp.struGroupIdentify[struGsh.nLayer][i][struGsh.nMatGroup].nEjectDelay3 = struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].nEjectDelay3;
                }
            }
            myFlow.resetEjectDelayTimeRG();
        }
        else if(msg2 == Delay4)
        {
            int tmpDelay = (int)((m_delay4+0.01)*10);

            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].nEjectDelay4 = tmpDelay;

            if((struCnfg.nVavleTimeSetMode == 1)&&(struGsh.nView == 0))
            {
                for(int i = 1; i < struCnfg.struLayerInfo[struGsh.nLayer].nViewChainTotal; i++)
                {
                    struCnfp.struGroupIdentify[struGsh.nLayer][i][struGsh.nMatGroup].nEjectDelay4 = struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].nEjectDelay4;
                }
            }
            myFlow.resetEjectDelayTimeRG();
        }
        else if(msg2 == modelDelay)
        {
            int tmpDelay3 = (int)((m_modeldelay+0.01)*10);

            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].nEjectDelayModel = tmpDelay3;

            if((struCnfg.nVavleTimeSetMode == 1)&&(struGsh.nView == 0))
            {
                for(int i = 1; i < struCnfg.struLayerInfo[struGsh.nLayer].nViewChainTotal; i++)
                {
                    struCnfp.struGroupIdentify[struGsh.nLayer][i][struGsh.nMatGroup].nEjectDelayModel = struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].nEjectDelayModel;
                }
            }
            myFlow.resetEjectDelayTimeRG();
        }
        else if(msg2 == Duration)
        {
            int tmpDuration = (int)((m_duration+0.01)*10);

            //            if((struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].nEjectDelay-tmpDuration/2 < 0)
            //                ||(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].nEjectDelayShape-tmpDuration/2 < 0))
            //            {
            //                g_infoWidget().setLabelText(g_myLan().msg_delay_time_limit);
            //                g_infoWidget().delayShow();

            //                m_duration = (float)(struCnfp.struGroupTick[struGsh.nLayer][curVavle][struGsh.nMatGroup].nEjectDuration/10.0);
            //                updateGroup();

            //                myFlow.sleep(1);
            //                g_infoWidget().hide();
            //            }
            //            else
            //            {
            //2021/7/29 重吹时间要不小于吹气时间
            if(struCnfp.struGroupTick[struGsh.nLayer][curVavle][struGsh.nMatGroup].nEjectDuration2 < tmpDuration &&
                    1 == struCnfg.nDPMachine)
            {
                g_infoWidget().setLabelText(g_myLan().Reblow_time_not_less_blow_time);
                g_infoWidget().delayShow();

                m_duration = (float)(struCnfp.struGroupTick[struGsh.nLayer][curVavle][struGsh.nMatGroup].nEjectDuration/10.0);
                updateGroup();

                myFlow.sleep(1);
                g_infoWidget().hide();
            }
            else
            {
                if (tmpDuration == 0) tmpDuration = 1;
                struCnfp.struGroupTick[struGsh.nLayer][curVavle][struGsh.nMatGroup].nEjectDuration = tmpDuration;

                myFlow.resetEjectDurationTimeRG();
                myFlow.resetEjectDelayTimeRG(); //修改吹气时间，会影响延迟时间
            }
        }
        else if(msg2 == Duration2)
        {
            int tmpDuration2 = (int)((m_duration2+0.01)*10);
            //2021/7/29 重吹时间要大于吹气时间
            if(tmpDuration2 < struCnfp.struGroupTick[struGsh.nLayer][curVavle][struGsh.nMatGroup].nEjectDuration
                    || (tmpDuration2+1)/2 > struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].nEjectDelay
                    && 1 == struCnfg.nDPMachine )
            {
                g_infoWidget().setLabelText(g_myLan().Reblow_time_setting_error);
                g_infoWidget().delayShow();

                m_duration2 = (float)( struCnfp.struGroupTick[struGsh.nLayer][curVavle][struGsh.nMatGroup].nEjectDuration2 /10.0);
                updateGroup();

                myFlow.sleep(1);
                g_infoWidget().hide();
            }
            else
            {
                if (tmpDuration2 == 0) tmpDuration2 = 1;
                struCnfp.struGroupTick[struGsh.nLayer][curVavle][struGsh.nMatGroup].nEjectDuration2 = tmpDuration2;

                myFlow.resetEjectDurationTimeRG();
                myFlow.resetEjectDelayTimeRG(); //修改吹气时间，会影响延迟时间
            }
        }
        else if(msg2 == Wait)
        {
            struCnfp.struGroupTick[struGsh.nLayer][curVavle][struGsh.nMatGroup].nWaitTime = (int)((m_wait+0.01)*10);
            myFlow.resetEjectWaitTimeRG();
            myFlow.resetEjectDelayTimeRG(); //修改尾气时间，会影响延迟时间
        }
        else if(msg2 == Tail)
        {
            struCnfp.struGroupTick[struGsh.nLayer][curVavle][struGsh.nMatGroup].nTailTime = (int)((m_tail+0.01)*10);
            myFlow.resetEjectTailTimeRG();
        }
        else if(msg2 == valveDefendTime)
        {
            struCnfg.nValveDefendTime= (int)((m_vavleDefendTime+0.01)*10);
            struCnfg.nValveUndefendTime= (int)((m_vavleUndefendTime+0.01)*10);
            myFlow.resetVavleDefendTime();

        }
    }



}

void ejectDelay::retranslateUiWidget()
{
    ui->expandTickCheckBox->setText(g_myLan().ids_heavySelect);
}

void ejectDelay::OnGroupChange(int groupIndex)
{
    updateGroup();
    //    updateGroupValue();
}

void ejectDelay::OnViewChange(int view)
{
    //    curView = view;

    curVavle = (struCnfg.nMachineType == MACHINE_LD2_ZL_GP) ? struGsh.nView/4 : 0;
    updateGroup();
}

void ejectDelay::OnLayerChange()
{
    updateGroup();
    //    updateGroupValue();
}

void ejectDelay::getscreenshot()
{
    shottimer->stop();
    {
        if(struCnfg.nVavleTimeSetMode == 1)
        {
            struGsh.nMatGroup = 0;
            for(int j=0; j<struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[0].nIdentifyGroupTotal;j++)
            {
                groupBtnClick(j);
                updateGroupWidgetState();
                myFlow.sleep(1);
                myFlow.saveScreenshot();
            }
        }
        else
        {

            for(int i=0;i<MAX_VIEW;i++){
                if(struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[i])
                {
                    QMetaObject::invokeMethod(m_pViewBtn[i],"clicked", Qt::QueuedConnection);
                    myFlow.sleep(1);
                    myFlow.saveScreenshot();
                }
            }
        }
        myFlow.sleep(1);
        struGsh.bScreenShotStatus = 0;
    }
}

void ejectDelay::updateGroup()
{
    stu_group_identify &identify = struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup];

    m_delay = (float)(identify.nEjectDelay/10.0);
    if(struCnfg.nDPMachine){
        ui->ejectDelayWidget->setFloatParams(this,0,DELAY_DP, Delay, &m_delay);
        ui->ejectDelayWidget->setTitle(g_myLan().ids_normal+g_myLan().ids_delayTime);
    }
    else{
        ui->ejectDelayWidget->setFloatParams(this,0,DELAY, Delay, &m_delay);
        ui->ejectDelayWidget->setTitle(g_myLan().ids_delayTime);

    }
    ui->ejectDelayWidget->setValue(m_delay);

    m_delay2 = (float)(identify.nEjectDelayShape/10.0);
    if(struCnfg.nDPMachine){
        ui->ejectDelayWidget_2->setFloatParams(this,0,DELAY_DP, Delay2, &m_delay2);
        ui->ejectDelayWidget_2->setTitle(g_myLan().ids_delayTime2);
    }
    else{
        ui->ejectDelayWidget_2->setFloatParams(this,0,DELAY, Delay2, &m_delay2);
        ui->ejectDelayWidget_2->setTitle(g_myLan().ids_delayTime2);
    }
    ui->ejectDelayWidget_2->setValue(m_delay2);

    m_delay3 = (float)(identify.nEjectDelay3/10.0);
    if(struCnfg.nDPMachine){
        ui->ejectDelayWidget_3->setFloatParams(this,0,DELAY_DP, Delay3, &m_delay3);
        ui->ejectDelayWidget_3->setTitle(g_myLan().ids_delayTime+"3");
    }
    else{
        ui->ejectDelayWidget_3->setFloatParams(this,0,DELAY, Delay3, &m_delay3);
        ui->ejectDelayWidget_3->setTitle(g_myLan().ids_delayTime+"3");
    }
    ui->ejectDelayWidget_3->setValue(m_delay3);
    ui->ejectDelayWidget_3->setVisible(struCnfp.nSensorPixel[0][struGsh.nView]== SENSOR_PIXELS_2048_GTD);

    m_delay4 = (float)(identify.nEjectDelay4/10.0);
    if(struCnfg.nDPMachine){
        ui->ejectDelayWidget_4->setFloatParams(this,0,DELAY_DP, Delay4, &m_delay4);
        ui->ejectDelayWidget_4->setTitle(g_myLan().ids_delayTime+"4");
    }
    else{
        ui->ejectDelayWidget_4->setFloatParams(this,0,DELAY, Delay4, &m_delay4);
        ui->ejectDelayWidget_4->setTitle(g_myLan().ids_delayTime+"4");
    }
    ui->ejectDelayWidget_4->setValue(m_delay4);
    ui->ejectDelayWidget_4->setVisible(struCnfp.nSensorPixel[0][struGsh.nView] == SENSOR_PIXELS_2048_GTD);
    qDebug()<<struGsh.nLayer
           <<struGsh.nView
          <<struCnfp.nSensorPixel[0][struGsh.nView];



    m_modeldelay = (float)(identify.nEjectDelayModel/10.0);
    ui->ejectModelDelayWidget->setFloatParams(this,0.0,DELAY_DP, modelDelay, &m_modeldelay);
    ui->ejectModelDelayWidget->setTitle(g_myLan().ids_Type+g_myLan().ids_delayTime);
    ui->ejectModelDelayWidget->setValue(m_modeldelay);
    ui->ejectModelDelayWidget->setVisible(struCnfg.nDPMachine);

    stu_group_tick &tick = struCnfp.struGroupTick[struGsh.nLayer][curVavle][struGsh.nMatGroup];
    m_duration = (float)(tick.nEjectDuration/10.0);
    ui->ejectDurationWidget->setFloatParams(this,0,struCnfg.nEjectDurationMaxValue, Duration, &m_duration);
    ui->ejectDurationWidget->setTitle(g_myLan().ids_blowTime);
    ui->ejectDurationWidget->setValue(m_duration);

    m_duration2 = (float)(tick.nEjectDuration2/10.0);
    ui->ejectDuration2Widget->setFloatParams(this,0,struCnfg.nEjectDurationMaxValue, Duration2, &m_duration2);
    ui->ejectDuration2Widget->setTitle(g_myLan().ipc_blowTime2);
    ui->ejectDuration2Widget->setValue(m_duration2);
    ui->ejectDuration2Widget->setVisible(struCnfg.nDPMachine);


    m_wait = (float)(tick.nWaitTime/10.0);
    ui->waitTimeWidget->setFloatParams(this,0,15.0, Wait, &m_wait);
    ui->waitTimeWidget->setTitle(g_myLan().ids_waittime);
    ui->waitTimeWidget->setValue(m_wait);

    m_tail = (float)(tick.nTailTime/10.0);
    ui->tailTimeWidget->setFloatParams(this,0,15.0,Tail,&m_tail);
    ui->tailTimeWidget->setTitle(g_myLan().ids_tailtime);
    ui->tailTimeWidget->setValue(m_tail);

    ui->expandTickCheckBox->setChecked(tick.nEjwidth == 1);

    if(struCnfp.struGroupTick[struGsh.nLayer][curVavle][struGsh.nMatGroup].nEjwidth == 1)
    {
        ui->waitTimeWidget->setVisible(false);
        ui->tailTimeWidget->setVisible(false);
    }
    else
    {
        ui->waitTimeWidget->setVisible(true);
        ui->tailTimeWidget->setVisible(true);
    }


    m_vavleDefendTime = (float)(struCnfg.nValveDefendTime/10.0);
    m_vavleUndefendTime = (float)(struCnfg.nValveUndefendTime/10.0);

    ui->valveDefendTimeWidget->setFloatParams(this,10,10000, valveDefendTime, &m_vavleDefendTime);
    ui->valveDefendTimeWidget->setTitle(g_myLan().ids_valveDefendTime);
    ui->valveDefendTimeWidget->setValue(m_vavleDefendTime);
    ui->valveDefendTimeWidget->setVisible(struCnfg.nValveDefend);

    ui->valveUndefendTimeWidget->setFloatParams(this,0,10000, valveDefendTime, &m_vavleUndefendTime);
    ui->valveUndefendTimeWidget->setTitle(g_myLan().ids_valveUndefendTime);
    ui->valveUndefendTimeWidget->setValue(m_vavleUndefendTime);
    ui->valveUndefendTimeWidget->setVisible(struCnfg.nValveDefend);

}


void ejectDelay::on_expandTickCheckBox_clicked(bool checked)
{
    struCnfp.struGroupTick[struGsh.nLayer][curVavle][struGsh.nMatGroup].nEjwidth = (int)checked;
    myFlow.resetEjectVavleModeRG();
    myFlow.resetEjectDelayTimeRG(); //修改吹气方式，会影响延迟时间
    updateGroup();
}
