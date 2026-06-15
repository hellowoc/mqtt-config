#include "machinefunction.h"
#include "globalflow.h"
#include "mqttsrv.h"
#include "ui_machinefunction.h"
#include <QFile>
#include "qrencode.h"
#include "setsvmusedialog.h"

MachineFunction::MachineFunction(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::MachineFunction)
{
    ui->setupUi(this);
    QStringList list;
    list<<"1"<<"2"<<"3"<<"4";
    ui->comboBox->addItems(list);
    ui->oldBtn->setCheckable(true);
    ui->lineEdit->setType(textType);
    ui->d2Mode1Edit->setType(textType);
    ui->d2Mode2Edit->setType(textType);
    ui->d2Mode3Edit->setType(textType);
    ui->d2Mode4Edit->setType(textType);

    m_pbtnGroup = new QButtonGroup(this);
    m_pbtnGroup->addButton(ui->pushButton_3,machineModel);
    m_pbtnGroup->addButton(ui->pushButton_4,alarm);
    m_pbtnGroup->addButton(ui->pushButton_5,flow_set);
    m_pbtnGroup->addButton(ui->pushButton_6,recover_set);
    m_pbtnGroup->addButton(ui->pushButton_7,svm_set);
    m_pbtnGroup->addButton(ui->pushButton_8,param_set);
    m_pbtnGroup->addButton(ui->pushButton_9,acc);
    m_pbtnGroup->addButton(ui->pushButton_10,softfeed);
    m_pbtnGroup->addButton(ui->pushButton_11,btn_PLC);
    m_pbtnGroup->addButton(ui->btn_valve,btn_valve);
    m_pbtnGroup->addButton(ui->btn_filter,btn_filter);
    m_pbtnGroup->addButton(ui->btn_wipe,btn_wipe);
    m_pbtnGroup->addButton(ui->btn_lamp,btn_lamp);


    for(int i = 0;i< m_pbtnGroup->buttons().count();i++){
        m_pbtnGroup->button(i)->setCheckable(true);
        connect(m_pbtnGroup->button(i),SIGNAL(clicked(bool)),this,SLOT(btn_clicked(bool)));
    }
    m_pbtnGroup->button(svm_set)->hide();
    m_pbtnGroup->button(softfeed)->hide();

    ui->cmboxPort->clear();
    QStringList comlist;
    comlist<<"COM2"<<"COM1";
    ui->cmboxPort->addItems(comlist);
    ui->pushButton_3->setChecked(true);
    ui->stackedWidget->setCurrentIndex(0);

    QSettings setting(CFG_APPSet,QSettings::IniFormat);
    setting.setValue("testIp","cloud.chinaamd.cn");
    setting.sync();
    //    QSettings setting(CFG_APPSet,QSettings::IniFormat);
    //    QString testip = setting.value("testIp","cloud.chinaamd.cn").toString();
    //    setting.setValue("testIp",testip);
    //    setting.sync();
    ui->lvMachine->setVisible(false);
    ui->encryptBtn->setVisible(false);
    ui->autoImageCheckBox->setVisible(false);
    ui->pushButton->setObjectName("levelSenseBtn");
    ui->wheelRun->hide();
    ui->sprayCheckbox->hide();
    ui->bmMachine->hide();

    ui->sensortypecmx->setEnabled(false);
    ui->balmodecomboBox->setEnabled(false);
    ui->balmodecomboBox->clear();
    QStringList balModeList;
    //    balModeList<<g_myLan().mode_1<<g_myLan().mode_2<<g_myLan().mode_3<<g_myLan().mode_4;
    balModeList<<g_myLan().mode_1<<g_myLan().mode_2<<g_myLan().mode_3<<"mode_4";
    ui->balmodecomboBox->addItems(balModeList);

    ui->sensortypecmx->clear();
    QStringList sensTypelist;
    //    sensTypelist<<"NULL"<< g_myLan().ids_radarEnable<<g_myLan().level_sensor;
    sensTypelist<<"NULL"<< "radarEnable"<<g_myLan().level_sensor;
    ui->sensortypecmx->addItems(sensTypelist);
    QStringList alarmCmdList;
    alarmCmdList<<"0035"<<"0324";
    ui->alarmCmdTypeCombobox->addItems(alarmCmdList);
    ui->upsCheckBox->hide();
    alarmCmdList.clear();
    alarmCmdList<<"0"<<"1"<<"2"<<"3"<<"4"<<"5";
    ui->crossModeCombox->addItems(alarmCmdList);

    lightid[0] = ui->label_31;
    lightid[1] = ui->label_32;
    lightid[2] = ui->label_33;
    lightid[3] = ui->label_34;
    lightid[4] = ui->label_35;
    lightid[5] = ui->label_36;
    lightid[6] = ui->label_37;
    lightid[7] = ui->label_38;

    lightmode[0] = ui->modeEdit;
    lightmode[1] = ui->modeEdit_2;
    lightmode[2] = ui->modeEdit_3;
    lightmode[3] = ui->modeEdit_4;
    lightmode[4] = ui->modeEdit_5;
    lightmode[5] = ui->modeEdit_6;
    lightmode[6] = ui->modeEdit_7;
    lightmode[7] = ui->modeEdit_8;

    lightlength[0] = ui->lengthEdit;
    lightlength[1] = ui->lengthEdit_2;
    lightlength[2] = ui->lengthEdit_3;
    lightlength[3] = ui->lengthEdit_4;
    lightlength[4] = ui->lengthEdit_5;
    lightlength[5] = ui->lengthEdit_6;
    lightlength[6] = ui->lengthEdit_7;
    lightlength[7] = ui->lengthEdit_8;

    ui->lowcfg_sync->hide();
}

MachineFunction::~MachineFunction()
{
    delete ui;
}

void MachineFunction::showPage(bool bshow)
{
    if(bshow){
        ui->ledit_realtime->setMinMax(this,5,3600,0,&struCnfg.sTotalStaticsGetDuration);
        ui->ledit_realtime->setText(QString::number(struCnfg.sTotalStaticsGetDuration));
        ui->ledit_realtime_ratio->setFloatMinMax(this,0.1,99.9,1,&struCnfp.ipcBadPointscoff,false);
        ui->ledit_realtime_ratio->setText(QString::number(struCnfp.ipcBadPointscoff));
        ui->bigSmallMaxLineedit->setMinMax(this,255,65535,1,&struCnfg.nBigSmallArithMaxValue);
        ui->bigSmallMaxLineedit->setText(QString::number(struCnfg.nBigSmallArithMaxValue));
        ui->edgeCutMaxLineedit->setMinMax(this,1,14,1,&struCnfg.nEdgeCutMaxValue);
        ui->edgeCutMaxLineedit->setText(QString::number(struCnfg.nEdgeCutMaxValue));
        ui->impuredMaxLineedit->setMinMax(this,1,32,1,&struCnfg.nImpuredMaxValue);
        ui->impuredMaxLineedit->setText(QString::number(struCnfg.nImpuredMaxValue));
        ui->ejectDurationMaxLineedit->setFloatMinMax(this,1,54.0,1,&struCnfg.nEjectDurationMaxValue);
        ui->ejectDurationMaxLineedit->setText(QString::number(struCnfg.nEjectDurationMaxValue));

        ui->ledit_valve->setMinMax(this,0,65535,2,&struCnfg.stuUseLimitCfg.valve_life);
        ui->ledit_filter->setMinMax(this,0,65535,3,&struCnfg.stuUseLimitCfg.filter_life);
        ui->ledit_wipe->setMinMax(this,0,65535,4,&struCnfg.stuUseLimitCfg.wipe_life);
        ui->ledit_lamp->setMinMax(this,0,65535,5,&struCnfg.stuUseLimitCfg.lamp_life);
        ui->ledit_onoffDelay->setMinMax(this,0,30,7,&struCnfg.nOnoffDelay);
        ui->ledit_onoffDelay->setText(QString::number(struCnfg.nOnoffDelay));

        ui->ledit_onoffDelay_2->setMinMax(this,0,30,7,&struCnfg.nOnoffDelay_2);
        ui->ledit_onoffDelay_2->setText(QString::number(struCnfg.nOnoffDelay_2));

        ui->d2Mode1Edit->setText(struCnfp.sD2ImgModeName[0]);
        ui->d2Mode2Edit->setText(struCnfp.sD2ImgModeName[1]);
        ui->d2Mode3Edit->setText(struCnfp.sD2ImgModeName[2]);
        ui->d2Mode4Edit->setText(struCnfp.sD2ImgModeName[3]);

        /* 均衡供料参数 */
        memcpy(&m_feedAuto, &struCnfg.stuFeedAutoCtrl, sizeof(struCnfg.stuFeedAutoCtrl));

        ui->containDeptyEdit->setMinMax(this,0,1999,8,&m_feedAuto.riceContainerDepth);
        ui->emptyposiEdit->setMinMax(this,0,1999,9,&m_feedAuto.levelEmptyPosi);
        ui->levelmoreposiEdit->setMinMax(this,0,1000,10,&m_feedAuto.levelMorePosi);
        ui->fullposiEdit->setMinMax(this,0,500,11,&m_feedAuto.levelFullPosi);

        ui->feedclearvalueEdit->setMinMax(this,0,100,12,&m_feedAuto.feedClearModeValue);
        ui->feedemptyvalueEdit->setMinMax(this,0,99,13,&m_feedAuto.feedEmpytLowLimitValue);
        ui->fullcontinueedit->setMinMax(this,1,10,14,&m_feedAuto.feedBalAdjustStep);    //调节步长
        ui->balpartcountedit->setMinMax(this,1,1999,15,&m_feedAuto.levelLowPosi);
        ui->levelstatchangewaitedit->setMinMax(this,0,100,16,&m_feedAuto.feedLevelChangeWaitTime);
        ui->feedprefullvalueEdit->setMinMax(this,0,100,17,&m_feedAuto.feedPreFullConstValue);
        ui->prefullcontinueedit->setMinMax(this,0,100,18,&m_feedAuto.feedBalAdjustDuration);    //步长周期
        ui->flowbaluplimitEdit->setMinMax(this,1,100,19,&m_feedAuto.posiChgThres);
        ui->feedbaluplimitEdit->setMinMax(this,0,100,20,&m_feedAuto.posiChgWait);
        ui->feedbalupflowEdit->setMinMax(this,0,100,21,&m_feedAuto.balUpFeedflow);
        ui->feedballowflowEdit->setMinMax(this,0,100,22,&m_feedAuto.balLowFeedflow);
        ui->imgSaturationEnable->setChecked(struCnfg.imgSaturationEnable);

        updateoldBtnState();
        updateViceFeedState();
        updateAutoImageCaptureState();
        updateUseIpcEnableState();
        updateVavleTimeSetModeState();
        updateArithParamsSetByTimes();
        updateFeedCtrlTpye();
        updateConnectName();

        updatePressureAlarmTypeCombox();
        updateFlowStaticsEnableState();

        updatePartsUsedInfo();
        updatePortUsedCombox();

        updateSpray();
        updateWheelType();
        updateVibType();
//        updateCrossCam();
//        updateCrossMode1();
//        updateCrossMode2();
        updateValveSiteState();

        updateValveType();
        updateVibFreq();
        updateVibFreq2();
        updateBigVibPos();
        updateBigVibEnable();

        updateSVMPara();
        updateAllInOne();
        updateValveDefend();
        updateScreenSet();
        updateSGTimeoutSet();
        updateBalanceCtrl();
        updateSelectModeSet();

        updateConnectState(mytcpServer->isconnected);

        bool bVisiable = ((struCnfg.nMachineType == MACHINE_LD2_ZL_GP) || (struCnfg.nMachineType == MACHINE_LD_ZL_GP))?true:false;
        ui->vibMode->setVisible(bVisiable);
        ui->label_vib->setVisible(bVisiable);
        ui->sprayCheckbox->setVisible(bVisiable);
        //ui->wheelRun->setVisible(bVisiable);
        ui->checkBox->setChecked(struGsh.debugMode);
        ui->lvMachine->setChecked(struCnfg.nLVMachine);
        ui->plcControl->setChecked(struCnfg.nPlcControl);
        ui->dpMachine->setChecked(struCnfg.nDPMachine);
        ui->bmMachine->setChecked(struCnfg.nUseBMModule);
        ui->lowcfg_sync->setChecked(struCnfg.nUseLowcfgSync);
        ui->camParaInCnfCheckBox->setChecked(struCnfg.camParaInCnf);
        ui->alarmCmdTypeCombobox->setCurrentIndex(struCnfg.alarmCmdType);

        ui->lampcheckBox->setChecked(struCnfg.blampctrlEnable);
        ui->AutoCtrlcheckBox->setChecked(struCnfg.bautoplcctrlEnable);
        ui->lampcheckBox->setCheckable(struCnfg.bautoplcctrlEnable);
        ui->DefaultPlccheckBox->setChecked(struCnfg.nDefaultPlcCtrl);
        ui->PowerCutcheckBox->setChecked(struCnfg.nPowerCut);
        if(paramDelayCode.enable == 0xAA)
        {
            ui->moreBtn->hide();
        }
        else
        {
            ui->moreBtn->show();
        }
        if(paramDelayCode.uiLanguage == 0xAA)
        {
            ui->checkBoxChs->setChecked(false);
        }
        else
        {
            ui->checkBoxChs->setChecked(true);
        }
        ui->checkBoxChs->hide();
        ui->label_26->setVisible(g_myLan().g_languageIndex == LANG_CHS || g_myLan().g_languageIndex == LANG_CHT);
        ui->comboBox_2->setVisible(g_myLan().g_languageIndex == LANG_CHS || g_myLan().g_languageIndex == LANG_CHT);
        ui->label_27->setVisible(g_myLan().g_languageIndex == LANG_CHS || g_myLan().g_languageIndex == LANG_CHT);
        ui->comboBox_5->setVisible(g_myLan().g_languageIndex == LANG_CHS || g_myLan().g_languageIndex == LANG_CHT);
        ui->accCheckbox->setChecked(struCnfg.accChecked);
        ui->crossModeCombox->setCurrentIndex(struCnfg.bCrossMode1);
        ui->crossCheckbox->hide();
        ui->crossMode1Checkbox->hide();
        ui->crossMode2Checkbox->hide();
        ui->ledit_turnRatio->setMinMax(this,0,100,3,&struCnfg.overRatio);
        ui->ledit_turnRatio->setText(QString::number(struCnfg.overRatio));
        ui->ledit_AreaThres->setMinMax(this,0,65535,3,&struCnfg.areaThres);
        ui->ledit_AreaThres->setText(QString::number(struCnfg.areaThres));
        ui->ledit_turnRatioTime->setMinMax(this,0,65535,3,&struCnfg.overRatioTime);
        ui->ledit_turnRatioTime->setText(QString::number(struCnfg.overRatioTime));
        ui->lbl_AreaThres->setVisible(struGsh.debugMode);
        ui->lbl_turnRatiotime->setVisible(struGsh.debugMode);
        ui->lbl_turnRatio->setVisible(struGsh.debugMode);
        ui->ledit_turnRatio->setEnabled(struGsh.debugMode);
        ui->ledit_turnRatioTime->setEnabled(struGsh.debugMode);
        ui->checkBox_2->setChecked(struCnfg.bMainpageShow);
        ui->logolightnum->setTitle("LOGO"+ g_myLan().ids_light);
        ui->logolightnum->setValue(struCnfg.bLogoLightNum);
        ui->logolightnum->setParams(this, 0, MAX_LAMP_PER_BOARD-2, 2, &struCnfg.bLogoLightNum);
        updateLogoLight();

    }
}

void MachineFunction::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){
        switch(msg2)
        {
        case 0:
            ui->ledit_realtime->setText(QString::number(struCnfg.sTotalStaticsGetDuration));
            break;
        case 1:
            ui->bigSmallMaxLineedit->setText(QString::number(struCnfg.nBigSmallArithMaxValue));
            ui->edgeCutMaxLineedit->setText(QString::number(struCnfg.nEdgeCutMaxValue));
            ui->impuredMaxLineedit->setText(QString::number(struCnfg.nImpuredMaxValue));
            ui->ejectDurationMaxLineedit->setText(QString::number(struCnfg.nEjectDurationMaxValue));
            break;
        case 2:
            updateLogoLight();
        case -1:
            myFlow.resetSystemControlRG();
        }
    }
}

void MachineFunction::retranslateUiWidget()
{
    ui->viceFeederCheckBox->setText(g_myLan().vice_feeder);
    ui->feederCtrlType1CheckBox->setText(g_myLan().feederCtrlType1);
    ui->encryptBtn->setText(g_myLan().dc_set);
    ui->checkBox_flow->setText(g_myLan().ids_statisticsInfo);
    ui->checkBox->setText(g_myLan().ids_factoryModel);

    ui->pressurAlarmLbl->setText(g_myLan().pressure_alarm_type);
    presAlarmTypeList.clear();
    presAlarmTypeList<<g_myLan().pressure_alarm_switch<<g_myLan().pressure_alarm_digi;
    alarmVoiceTypeList.clear();
    alarmVoiceTypeList<<g_myLan().ids_disable<<g_myLan().lighting+g_myLan().sound<<g_myLan().lighting<<g_myLan().sound;

    ui->delayTimeSetByTimescheckBox->setText(g_myLan().set_delay_time_by_time);
    ui->identifyParamsSetByTimescheckBox->setText(g_myLan().arith_params_set_alike);
    ui->lbl_realtime->setText(g_myLan().realtime_statistical_query_cycle);
    ui->resetModeName->setText(g_myLan().one_click_recovery);
    ui->label->setText(g_myLan().alarm_method);
    ui->bigSmallMaxLabel->setText(g_myLan().arith_big_small_limit);
    ui->edgeCutMaxLabel->setText(g_myLan().edge_cut_max_limit);
    ui->impuredMaxLabel->setText(g_myLan().impured_max_limit);
    ui->ejectDurationMaxLabel->setText(g_myLan().eject_duration_max_limit);

    ui->changelogoBtn->setText(g_myLan().ui_style);
    ui->pushButton->setText(g_myLan().level_sensor);
    ui->tempSensorBtn->setText(g_myLan().temperature_sensor);
    ui->pushButton_3->setText(g_myLan().ids_machineModel);
    ui->pushButton_4->setText(g_myLan().ids_alarm);
    ui->pushButton_5->setText(g_myLan().flow_set);

    ui->emmcp1UgBtn->setText(g_myLan().recover_part);
    ui->emmcUgBtn->setText(g_myLan().recover_all);
    ui->ugCancelBtn->setText(g_myLan().recover_cancel);
    ui->pushButton_6->setText(g_myLan().recovery_set);
    ui->pushButton_7->setText("SVM "+g_myLan().ids_set);
    ui->pushButton_8->setText(g_myLan().ids_paramName+g_myLan().ids_set);
    ui->pushButton_9->setText("ACC");
    ui->pushButton_10->setText(g_myLan().ids_intelligent_control);
    ui->accelerationBtn->setText(g_myLan().ids_vib_assist_control);

    ui->portLbl->setText(g_myLan().port);

    ui->sensorNameRestoreBtn->setText(g_myLan().sensorNameRestore);

    ui->crossCheckbox->setText(g_myLan().crossCAM);
    ui->crossMode1Checkbox->setText(g_myLan().mode_1);
    ui->crossMode2Checkbox->setText(g_myLan().mode_2);
    ui->vavleSite->setText(g_myLan().vavle+g_myLan().ids_position);
    ui->vibUseEnable->setText(g_myLan().big_feeder);
    ui->label_17->setText(g_myLan().feeder_freq);

    ui->groupBox->hide();
    //    ui->groupBox->setTitle(g_myLan().life_time);
    ui->label_4->setText(g_myLan().ids_usefultime_valve);
    ui->label_5->setText(g_myLan().ids_usefultime_filter);
    ui->label_6->setText(g_myLan().ids_usefultime_clean);
    ui->label_7->setText(g_myLan().ids_light);
    ui->label_8->setText(g_myLan().day);
    ui->label_9->setText(g_myLan().day);
    ui->label_10->setText(g_myLan().day);
    ui->label_11->setText(g_myLan().times);
    ui->btn_filter->setText(g_myLan().recovery_set);
    ui->btn_valve->setText(g_myLan().recovery_set);
    ui->btn_wipe->setText(g_myLan().recovery_set);
    ui->btn_lamp->setText(g_myLan().recovery_set);
    ui->label_vib->setText(g_myLan().related_mode);
    ui->vibUseEnable->setText(g_myLan().big_feeder);
    ui->label_12->setText(g_myLan().big_feeder_pos);
    ui->label_17->setText(g_myLan().big_feeder_freq);
    ui->label_16->setText(g_myLan().feeder_freq);

    ui->lvMachine->setText("LV"+g_myLan().ids_machineModel);
    ui->lblValveType->setText(g_myLan().vavle+g_myLan().type);
    ui->pushButton_2->setText(g_myLan().ids_apply);
    ui->lbl_onoffDelay->setText(g_myLan().feeder+g_myLan().ids_off+g_myLan().ids_cleaningTime);
    ui->lbl_onoffDelay_2->setText(g_myLan().feeder+g_myLan().ids_turnOn+g_myLan().ids_cleaningTime);

    ui->moreBtn->setText(g_myLan().ids_more);

    ui->sprayCheckbox->setText(g_myLan().spray);
    ui->wheelRun->setText(g_myLan().wheelRun);
    ui->ipcEnableCheckBox->setText("IPC "+g_myLan().ids_enable);
    ui->autoImageCheckBox->setText(g_myLan().ids_smart+g_myLan().ids_image);
    ui->allInOne->setText(g_myLan().ids_allInOne);
    ui->checkBoxChs->setText(g_myLan().languageChange);
    ui->valveDefend->setText(g_myLan().ids_valveDefend);
    ui->screenSetCheckBox->setText(g_myLan().ids_ScreenSet);

    ui->alarmVoiceTestBtn->setText(g_myLan().alarm+g_myLan().check);
    ui->lbl_realtime_ratio->setText(g_myLan().ids_statisticsInfo+g_myLan().lamp_coef);
    ui->lowcfg_sync->setText(g_myLan().lowCfg_sync);
    ui->camParaInCnfCheckBox->setText("camIncnf");

    ui->autoApplyBtn->setText(g_myLan().ids_apply);
    ui->alarmCmd->setText(g_myLan().ids_versionValue);

    ui->updateWifiBtn->setText(g_myLan().update_lib);
    ui->pushButton_12->setText(g_myLan().export_screenshot_file);
    ui->AutoCtrlcheckBox->setText(g_myLan().AutoCtrlEnable);
    ui->PowerCutcheckBox->setText(g_myLan().PowerCut);
    ui->DefaultPlccheckBox->setText(g_myLan().DefaultPlcEnable);
    ui->lampcheckBox->setText(g_myLan().lamp_control);
    ui->pushButton_11->setText(g_myLan().plc_ctrl_set);

    ui->lbl_cmdSendTimeout->setText(g_myLan().InstructTimeout);
    ui->lbl_ugSendTimeout->setText(g_myLan().UpgradeFileTimeout);
    ui->lbl_ugSendInterval->setText(g_myLan().ForwardingInterval);
    ui->lbl_waitTimeout->setText(g_myLan().PacketTimeout);
    ui->restoreBtn->setText(g_myLan().restore);

    ui->pushButton_11->setText(g_myLan().Intelligent_Set);
    ui->tabWidget->setTabText(0,g_myLan().plc_ctrl_set);
    ui->tabWidget->setTabText(1,g_myLan().TCP_Set);
    ui->label_28->setText(g_myLan().server_ip);
    ui->label_25->setText(g_myLan().server_port);
    ui->connectButton->setText(g_myLan().connect);
    ui->disconnectButton->setText(g_myLan().disconnect);

    ui->screenShot->setText(g_myLan().Screenshot);
    ui->label_29->setText(g_myLan().ids_byChannel);
    ui->ageAll->setText(g_myLan().ids_byMachine);
    ui->label_26->setText(g_myLan().ids_byMachine);
    ui->imgSaturationEnable->setText(g_myLan().arith_sat);
    ui->label_30->setText(g_myLan().crossCAM);

    ui->label_mode->setText(g_myLan().mode);
    ui->label_length->setText(g_myLan().ids_length);

}

void MachineFunction::btn_clicked(bool check)
{
    if(check)
    {
        for(int i = btn_valve;i < END;i++)
        {
            if ((QPushButton *)sender() == m_pbtnGroup->button(i))
            {
                switch (i) {
                case btn_valve:
                    struCnfg.stuUseLimitCfg.valve_start_use = struCnfg.nCounter;
                    break;
                case btn_filter:
                    struCnfg.stuUseLimitCfg.filter_start_use = struCnfg.nCounter;
                    break;
                case btn_wipe:
                    struCnfg.stuUseLimitCfg.wipe_start_use = 0;
                    break;
                case btn_lamp:
                    struCnfg.stuUseLimitCfg.lamp_start_use = struCnfg.nCounter;
                    break;
                default:
                    break;
                }
            }
        }
        ui->stackedWidget->setCurrentIndex(m_pbtnGroup->id((QPushButton*)sender()));

        ui->cmboxPort->setStyleSheet("QComboBox::item { font:30px \"Arial\",height: 45px; }");
        ui->alarmCmdTypeCombobox->setStyleSheet("QComboBox::item { font:30px \"Arial\", height: 45px; }");
    }
}


void MachineFunction::on_oldBtn_clicked(bool checked)
{
    struGsh.ageTestEnable = (int)checked;
    myFlow.resetAgeTestFre(struGsh.ageTestEnable);
}
void MachineFunction::on_ageAll_clicked(bool checked)
{
    struGsh.ageTestAll = (int)checked;
    updateAgeAllState();
    myFlow.resetAgeTestFre(struGsh.ageTestEnable);
}
void MachineFunction::on_comboBox_activated(int index)
{
    struGsh.ageTestFre = index;
    myFlow.resetAgeTestFre(struGsh.ageTestEnable);
}

void MachineFunction::updateAgeChute()
{
    ui->ageChute->setMinMax(this,1,struCnfg.struLayerInfo[0].nVavleBoardTotal[0],-2,&struGsh.ageTestChute);
    ui->ageChute->setText(QString::number(struGsh.ageTestChute));
}

void MachineFunction::updateAgeAllState()
{
    ui->ageAll->setChecked(struGsh.ageTestAll == 1);
}

void MachineFunction::updateoldBtnState()
{
    if(ui->oldBtn->isChecked()){
        ui->oldBtn->setText(g_myLan().ids_enable);
    }else{
        ui->oldBtn->setText(g_myLan().ids_disable);
    }
    updateAgeChute();
    updateAgeAllState();
}

void MachineFunction::updateViceFeedState()
{
    ui->viceFeederCheckBox->setChecked(struCnfg.nFeedViceTotal == 1);
}

void MachineFunction::updateAutoImageCaptureState()
{
    ui->autoImageCheckBox->setChecked(struCnfg.nAutoImageCaptureEnable == 1);
}

void MachineFunction::updateUseIpcEnableState()
{
    ui->ipcEnableCheckBox->setChecked(struCnfg.nUseIpcEnable == 1);
    ui->dpMachine->setChecked(struCnfg.nDPMachine);
}

void MachineFunction::updateVavleTimeSetModeState()
{
    ui->delayTimeSetByTimescheckBox->setChecked(struCnfg.nVavleTimeSetMode == 1);
}

void MachineFunction::updateArithParamsSetByTimes()
{
    ui->identifyParamsSetByTimescheckBox->setChecked(struCnfg.nEnableDispArithSetByTimes == 1);
}

void MachineFunction::updateFeedCtrlTpye()
{
    ui->feederCtrlType1CheckBox->setChecked(struCnfg.nFeedControlType1 == 1);
}

void MachineFunction::updateConnectName()
{
    QSettings setting(CFG_APPSet,QSettings::IniFormat);
    QString connectName = setting.value("connectName","").toString();
    ui->lineEdit->setText(connectName);
    if (connectName != "")
    {
        GenerateQRcode(connectName);
    }
}

void MachineFunction::updateValveDefend()
{
    ui->valveDefend->setChecked(struCnfg.nValveDefend == 1);
}

void MachineFunction::updatePressureAlarmTypeCombox()
{
    ui->pressureAlarmType->show();
    ui->pressureAlarmType->clear();
    ui->pressureAlarmType->addItems(presAlarmTypeList);
    ui->pressureAlarmType->setCurrentIndex(struCnfg.struAlarmConfig.nAirPressureAlarmType);

    ui->alarmVoiceTypeCombobox->clear();
    ui->alarmVoiceTypeCombobox->addItems(alarmVoiceTypeList);
    ui->alarmVoiceTypeCombobox->setCurrentIndex(struCnfg.struAlarmConfig.nLightVoiceAlarmType);
}

void MachineFunction::on_encryptBtn_clicked()
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

void MachineFunction::on_viceFeederCheckBox_clicked()
{
    struCnfg.nFeedViceTotal = !struCnfg.nFeedViceTotal;
    updateViceFeedState();
}

void MachineFunction::on_lineEdit_textChanged(const QString &arg1)
{
    if (arg1 != "")
    {
        QSettings setting(CFG_APPSet,QSettings::IniFormat);
        setting.setValue("connectName",arg1);
        setting.sync();
        GenerateQRcode(arg1);
    }
}

void MachineFunction::GenerateQRcode(QString tempstr)
{
#if 0
    QRcode *qrcode; //二维码数据
    //QR_ECLEVEL_Q 容错等级
    qrcode = QRcode_encodeString(tempstr.toStdString().c_str(), 2, QR_ECLEVEL_Q, QR_MODE_8, 1);
    qint32 temp_width=ui->label_3->width(); //二维码图片的大小
    qint32 temp_height=ui->label_3->height();
    qint32 qrcode_width = qrcode->width > 0 ? qrcode->width : 1;
    double scale_x = (double)temp_width / (double)qrcode_width; //二维码图片的缩放比例
    double scale_y =(double) temp_height /(double) qrcode_width;
    QImage mainimg=QImage(temp_width,temp_height,QImage::Format_ARGB32);
    QPainter painter(&mainimg);
    QColor background(Qt::white);
    painter.setBrush(background);
    painter.setPen(Qt::NoPen);
    painter.drawRect(0, 0, temp_width, temp_height);
    QColor foreground(Qt::black);
    painter.setBrush(foreground);
    for( qint32 y = 0; y < qrcode_width; y ++)
    {
        for(qint32 x = 0; x < qrcode_width; x++)
        {
            unsigned char b = qrcode->data[y * qrcode_width + x];
            if(b & 0x01)
            {
                QRectF r(x * scale_x, y * scale_y, scale_x, scale_y);
                painter.drawRects(&r, 1);
            }
        }
    }
    QPixmap mainmap=QPixmap::fromImage(mainimg);
    ui->label_3->setPixmap(mainmap);
    ui->label_3->setVisible(true);
#endif
}

void MachineFunction::on_delayTimeSetByTimescheckBox_clicked(bool checked)
{
    struCnfg.nVavleTimeSetMode = checked;
}

void MachineFunction::on_identifyParamsSetByTimescheckBox_clicked(bool checked)
{
    struCnfg.nEnableDispArithSetByTimes = checked;
}

void MachineFunction::on_feederCtrlType1CheckBox_clicked(bool checked)
{
    struCnfg.nFeedControlType1 = checked;
}

void MachineFunction::on_checkBox_flow_clicked(bool checked)
{
    if(checked)
        struCnfg.nAutoStaticsEnable = 1;
    else
        struCnfg.nAutoStaticsEnable = 0;
    updateFlowStaticsEnableState();
}

void MachineFunction::updateFlowStaticsEnableState()
{
    if(struCnfg.nAutoStaticsEnable == 1)
        ui->checkBox_flow->setChecked(true);
    else
        ui->checkBox_flow->setChecked(false);
}

void MachineFunction::updatePartsUsedInfo()
{
    if(struCnfg.nHasUps == 1)
    {
        ui->upsCheckBox->setChecked(true);
    }
    else
    {
        ui->upsCheckBox->setChecked(false);
    }

    ui->ledit_valve->setText(QString::number(struCnfg.stuUseLimitCfg.valve_life));
    ui->ledit_filter->setText(QString::number(struCnfg.stuUseLimitCfg.filter_life));
    ui->ledit_wipe->setText(QString::number(struCnfg.stuUseLimitCfg.wipe_life));
    ui->ledit_lamp->setText(QString::number(struCnfg.stuUseLimitCfg.lamp_life));
}

void MachineFunction::updatePortUsedCombox()
{
    QSettings setting(CFG_APPSet,QSettings::IniFormat);
    int value = setting.value("portName",2).toInt();
    int index = (value == 2)? 0 : 1;
    ui->cmboxPort->setCurrentIndex(index);
}

void MachineFunction::updateSpray()
{
    ui->sprayCheckbox->setChecked(struCnfg.nSpray == 1);
}

void MachineFunction::updateVibType()
{
    ui->vibMode->clear();
    QStringList list;
    list<<g_myLan().rice_mode<<g_myLan().material_mode<<g_myLan().synchronization<<g_myLan().independent;
    ui->vibMode->addItems(list);
    ui->vibMode->setCurrentIndex(struCnfg.nVibType);
}

void MachineFunction::updateCrossCam()
{
//    ui->crossCheckbox->setChecked(struCnfg.bCrossCam == 1);
}

void MachineFunction::updateCrossMode1()
{
//    ui->crossMode1Checkbox->setVisible(struCnfg.bCrossCam);
//    ui->crossMode1Checkbox->setChecked(struCnfg.bCrossMode1);
}

void MachineFunction::updateCrossMode2()
{
//    ui->crossMode2Checkbox->setVisible(struCnfg.bCrossCam);
//    ui->crossMode2Checkbox->setChecked(struCnfg.bCrossMode2);
}

void MachineFunction::updateWheelType()
{
    ui->wheelRun->setChecked(struCnfg.nWheelRunType == 1);
}

void MachineFunction::on_pressureAlarmType_activated(int index)
{
    struCnfg.struAlarmConfig.nAirPressureAlarmType = index;
    if(struCnfg.struAlarmConfig.nAirPressureAlarmType == 0)   //使用压力开关
    {
        struCnfg.struAlarmConfig.nFeederLevelAlarmEnable[CTRL_ALARM_MAX_FEED_SENSOR-1] = 0;
    }
    myFlow.resetSystemControlRG();
}

void MachineFunction::on_resetModeName_clicked()
{
    myMessageBox msgBox(MSG_QUES,  g_myLan().cfm_reset_current_scheme_materialname);
    int ret = msgBox.exec();

    if (ret == QDialog::Accepted){
        myFlow.resetSortNamesByLan();
    }
}

void MachineFunction::on_pushButton_clicked()
{
    logger()->info("enter feeder sensor setting page");
    g_MainManager().ShowWidget(SM_FEEDER_SENSOR_SET_WIDGET);
}

void MachineFunction::on_tempSensorBtn_clicked()
{
    logger()->info("enter temperature sensor setting page");
    g_MainManager().ShowWidget(SM_TEMP_SENSOR_SET_WIDGET);
}

void MachineFunction::on_alarmVoiceTypeCombobox_activated(int index)
{
    struCnfg.struAlarmConfig.nLightVoiceAlarmType = index;
}

void MachineFunction::on_sensorNameRestoreBtn_clicked()
{
    myFlow.resetSensorNamesByLan();
}

void MachineFunction::on_changelogoBtn_clicked()
{
    if(!g_Runtime().checkUsbExist()){
        g_infoWidget().setLabelText(g_myLan().msg_usb_insert);
        g_infoWidget().showSecs();
        return;
    }
    QString path = g_Runtime().getUsbPath();
    if (path == ""){
        g_infoWidget().setLabelText(g_myLan().msg_usb_not_recognized);
        g_infoWidget().showSecs();
        return;
    }

    QDir logoDir(path+"logo");
    if(!logoDir.exists()){
        g_infoWidget().setLabelText(g_myLan().msg_usb_logofile_not_exist);
        g_infoWidget().showSecs();
        return;
    }
    g_Runtime().dirExist("/sdcard/logo/");
#ifdef Q_OS_UNIX
    g_Runtime().copyFileToPath(path+"logo/top.bmp","/media/mmcblk1p2/logo/top.bmp",true);
    g_Runtime().copyFileToPath(path+"logo/homepage.bmp","/media/mmcblk1p2/logo/homepage.bmp",true);

    g_Runtime().copyFileToPath(path+"logo/top.bmp","/media/mmcblk0p1/logo/top.bmp",true);
    g_Runtime().copyFileToPath(path+"logo/homepage.bmp","/media/mmcblk0p1/logo/homepage.bmp",true);
    g_Runtime().copyFileToPath(path+"logo/boot.bmp","/sdcard/logo/boot.bmp",true);

    QString cmd = "cp -f "+path+"logo/*.ini"+ " /app/lang/";
    system(QString(cmd).toLocal8Bit().constData());
    system("sync");
#endif
    g_infoWidget().setLabelText(g_myLan().msg_usb_logofile_replace_success);
    g_infoWidget().showSecs();
}

void MachineFunction::on_cmboxPort_activated(int index)
{
    int value = (index == 0)? 2 : 1;
    QSettings setting(CFG_APPSet,QSettings::IniFormat);
    setting.setValue("portName",value);
    setting.sync();
}

void MachineFunction::on_emmcUgBtn_clicked()
{
    QFile file("/media/mmcblk0p1/upgrade");
    myMessageBox *mesBox = new myMessageBox(MSG_QUES, g_myLan().cfm_recover_all);
    int ret = mesBox->exec();
    if(ret == QDialog::Accepted){
#ifdef Q_OS_UNIX
        QFile f_file("/dev/mmcblk1");
        if (f_file.exists())
        {
            if(!file.exists())
            {
                char cmd[64];
                sprintf(cmd, "cd /media/mmcblk0p1/;touch upgrade");
                system(cmd);
                system("sync");
            }
        }

#endif
    }
    if (file.exists())
    {
        g_infoWidget().setLabelText(g_myLan().recover_all_cfg_success);
        g_infoWidget().showSecs();
    }
}

void MachineFunction::on_emmcp1UgBtn_clicked()
{
    QFile file("/media/mmcblk0p1/upgradep1");
    myMessageBox *mesBox = new myMessageBox(MSG_QUES, g_myLan().cfm_recover_part);
    int ret = mesBox->exec();
    if(ret == QDialog::Accepted){
#ifdef Q_OS_UNIX
        QFile f_file("/dev/mmcblk1");
        if (f_file.exists())
        {
            if(!file.exists())
            {
                char cmd[64];
                sprintf(cmd, "cd /media/mmcblk0p1/;touch upgradep1");
                system(cmd);
                system("sync");
            }
        }

#endif
    }
    if (file.exists())
    {
        g_infoWidget().setLabelText(g_myLan().recover_part_cfg_success);
        g_infoWidget().showSecs();
    }
}

void MachineFunction::on_ugCancelBtn_clicked()
{
#ifdef Q_OS_UNIX
    QFile f_file("/dev/mmcblk1");
    if (f_file.exists())
    {
        system("rm -fr /media/mmcblk0p1/upgrade*");
    }
#endif
}

void MachineFunction::on_upsCheckBox_clicked()
{
    struCnfg.nHasUps = (struCnfg.nHasUps == 0) ? 1 : 0;

    if(struCnfg.nHasUps == 1)
    {
        ui->upsCheckBox->setChecked(true);
    }
    else
    {
        ui->upsCheckBox->setChecked(false);
    }
}


void MachineFunction::on_sprayCheckbox_clicked(bool checked)
{
    struCnfg.nSpray = (int)checked;
    updateSpray();
}


void MachineFunction::on_vibMode_activated(int index)
{
    struCnfg.nVibType = index;
    updateVibType();
    myFlow.resetSystemControlRG();
}

void MachineFunction::on_wheelRun_clicked(bool checked)
{
    struCnfg.nWheelRunType = (int)checked;
    updateWheelType();
}

void MachineFunction::on_checkBox_clicked(bool checked)
{
    struGsh.debugMode = checked;
    QSettings setting(DEBUG_MODE_Set,QSettings::IniFormat);
    setting.setValue("debugmode",(int)checked);
    setting.sync();
}

void MachineFunction::on_crossMode1Checkbox_clicked(bool checked)
{
//    struCnfg.bCrossMode1 = (int)checked;
//    myFlow.resetCrossCam();
}

void MachineFunction::on_crossCheckbox_clicked(bool checked)
{
//    struCnfg.bCrossCam = (int)checked;
//    struCnfg.bCrossMode1 &= struCnfg.bCrossCam;
//    struCnfg.bCrossMode2 &= struCnfg.bCrossCam;
//    updateCrossMode1();
//    updateCrossMode2();
//    myFlow.resetCrossCam();
}

void MachineFunction::on_crossMode2Checkbox_clicked(bool checked)
{
//    struCnfg.bCrossMode2 = (int)checked;
//    myFlow.resetCrossCam();
}

void MachineFunction::updateValveSiteState()
{
    ui->vavleSite->setChecked(struCnfg.nValveSite == 1);
}

void MachineFunction::on_vavleSite_clicked(bool checked)
{
    struCnfg.nValveSite = (int)checked;
}

void MachineFunction::on_autoImageCheckBox_clicked()
{
    struCnfg.nAutoImageCaptureEnable = (struCnfg.nAutoImageCaptureEnable == 0) ? 1: 0;
    updateAutoImageCaptureState();
}

void MachineFunction::on_ipcEnableCheckBox_clicked()
{
    struCnfg.nUseIpcEnable = (struCnfg.nUseIpcEnable == 0) ? 1 : 0;
    updateUseIpcEnableState();
}

void MachineFunction::on_ledit_onoffDelay_textChanged(const QString &arg1)
{
    struCnfg.nOnoffDelay = QString(arg1).toInt();
}

void MachineFunction::on_comboBox_valveType_activated(int index)
{
    struCnfg.nValveType = index;
    updateValveType();
    myFlow.resetMachineConfigRG();
}

void MachineFunction::updateValveType()
{
    ui->comboBox_valveType->clear();
    QStringList list;
    list<<g_myLan().shortDrive<<g_myLan().longDrive;
    ui->comboBox_valveType->addItems(list);
    ui->comboBox_valveType->setCurrentIndex(struCnfg.nValveType);
}

void MachineFunction::updateVibFreq()
{
    ui->comboBox_vibFreq->setCurrentIndex(struCnfg.nVibFreq);
}

void MachineFunction::updateVibFreq2()
{
    ui->comboBox_vibFreq_2->setCurrentIndex(struCnfg.nVibFreq_2);
}

void MachineFunction::updateBigVibPos()
{
    ui->vibPosEdit->setMinMax(this,1,12,-1,&struCnfg.nBigVibPos);
    ui->vibPosEdit->setText(QString::number(struCnfg.nBigVibPos));
}

void MachineFunction::updateBigVibEnable()
{
    ui->vibUseEnable->setChecked(struCnfg.bigVibIsUse == 1);
    ui->vibPosEdit->setVisible(struCnfg.bigVibIsUse == 1);
    ui->comboBox_vibFreq_2->setVisible(struCnfg.bigVibIsUse == 1);
    ui->label_12->setVisible(struCnfg.bigVibIsUse == 1);
    ui->label_17->setVisible(struCnfg.bigVibIsUse == 1);
}

void MachineFunction::on_vibUseEnable_clicked(bool checked)
{
    struCnfg.bigVibIsUse = (int)checked;
    updateBigVibEnable();
    myFlow.resetSystemControlRG();
}

void MachineFunction::on_comboBox_vibFreq_activated(int index)
{
    struCnfg.nVibFreq = index;
    updateVibFreq();
    myFlow.resetSystemControlRG();
}

void MachineFunction::on_comboBox_vibFreq_2_activated(int index)
{
    struCnfg.nVibFreq_2 = index;
    updateVibFreq2();
    myFlow.resetSystemControlRG();
}

void MachineFunction::updateSVMPara()
{
    ui->ledit_C->setType(textType);
    ui->ledit_R->setType(textType);
    ui->ledit_G->setType(textType);
    ui->ledit_B->setType(textType);
    ui->ledit_RG->setType(textType);
    ui->ledit_GB->setType(textType);
    ui->ledit_RB->setType(textType);
    ui->ledit_RR->setType(textType);
    ui->ledit_GG->setType(textType);
    ui->ledit_BB->setType(textType);

    ui->ledit_C->setText(QString::number(struGsh.m_svmcoefs[9]));
    ui->ledit_R->setText(QString::number(struGsh.m_svmcoefs[0]));
    ui->ledit_G->setText(QString::number(struGsh.m_svmcoefs[1]));
    ui->ledit_B->setText(QString::number(struGsh.m_svmcoefs[2]));
    ui->ledit_RG->setText(QString::number(struGsh.m_svmcoefs[3]));
    ui->ledit_GB->setText(QString::number(struGsh.m_svmcoefs[4]));
    ui->ledit_RB->setText(QString::number(struGsh.m_svmcoefs[5]));
    ui->ledit_RR->setText(QString::number(struGsh.m_svmcoefs[6]));
    ui->ledit_GG->setText(QString::number(struGsh.m_svmcoefs[7]));
    ui->ledit_BB->setText(QString::number(struGsh.m_svmcoefs[8]));
}

void MachineFunction::on_pushButton_2_clicked()
{
    struGsh.m_svmcoefs[9] = ui->ledit_C->text().toDouble();
    struGsh.m_svmcoefs[0] = ui->ledit_R->text().toDouble();
    struGsh.m_svmcoefs[1] = ui->ledit_G->text().toDouble();
    struGsh.m_svmcoefs[2] = ui->ledit_B->text().toDouble();
    struGsh.m_svmcoefs[3] = ui->ledit_RG->text().toDouble();
    struGsh.m_svmcoefs[4] = ui->ledit_GB->text().toDouble();
    struGsh.m_svmcoefs[5] = ui->ledit_RB->text().toDouble();
    struGsh.m_svmcoefs[6] = ui->ledit_RR->text().toDouble();
    struGsh.m_svmcoefs[7] = ui->ledit_GG->text().toDouble();
    struGsh.m_svmcoefs[8] = ui->ledit_BB->text().toDouble();

    SetSvmUseDialog dlg;
    dlg.exec();
}

void MachineFunction::on_lvMachine_clicked(bool checked)
{
    struCnfg.nLVMachine = checked;
    QSettings setting(CFG_APPSet,QSettings::IniFormat);
    if(checked)
        setting.setValue("testIp","www.lauffer-cloud.com");
    else
        setting.setValue("testIp","cloud.chinaamd.cn");
    setting.sync();
}

void MachineFunction::on_plcControl_clicked(bool checked)
{
    struCnfg.nPlcControl = checked;
}

void MachineFunction::on_moreBtn_clicked()
{
    myInputMethod inputPanel;
    inputPanel.setType(KB_PD);
    inputPanel.setInputBtnText(g_myLan().ids_ok,g_myLan().ids_cancel,g_myLan().ids_delete,g_myLan().ids_space);
    if(inputPanel.exec() == QDialog::Accepted)
    {
        QString str = inputPanel.getText();
        if ((PRODUCT_DEFAULT_VENDOR == PRODUCT_VENDOR_ZK && str == "ZK.GD.2021.LANG")||(PRODUCT_DEFAULT_VENDOR == PRODUCT_VENDOR_LV && str == "LV.123.321.LANG"))
        {
            ui->checkBoxChs->show();
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

void MachineFunction::on_checkBoxChs_clicked()
{
    myMessageBox msgBox(MSG_QUES, g_myLan().checkLanChange);

    int ret = msgBox.exec();

    if (ret == QDialog::Accepted)
    {
        g_infoWidget().setLabelText(g_myLan().msg_applying);
        g_infoWidget().delayShow();

        paramDelayCode.uiLanguage = (paramDelayCode.uiLanguage == 0xAA) ? 0xFF : 0xAA;
        paramDelayCode.setUiLanguage();

        myFlow.sleep(2);
        g_infoWidget().hide();

        if(paramDelayCode.uiLanguage == 0xAA)
        {
            logger()->info("change machine outside");
            ui->checkBoxChs->setChecked(false);
        }
        else
        {
            logger()->info("change machine inside");
            ui->checkBoxChs->setChecked(true);
        }
    }
}

void MachineFunction::on_dpMachine_clicked(bool checked)
{
    struCnfg.nDPMachine = (int)checked;
    updateUseIpcEnableState();

    myFlow.imgHeight = (struCnfg.nDPMachine == 1) ? IMAGE_HEIGHT : IMAGE_HEIGHT_NOT_DP;
    logger()->info("dp machine :%1",checked);
}

void MachineFunction::updateAllInOne()
{
    ui->allInOne->setChecked(struCnfg.nAllInOne);
}

void MachineFunction::on_allInOne_clicked(bool checked)
{
    struCnfg.nAllInOne = (checked) ? 1:0;
}

void MachineFunction::on_ledit_onoffDelay_2_textChanged(const QString &arg1)
{
    struCnfg.nOnoffDelay_2 = QString(arg1).toInt();
}

void MachineFunction::on_valveDefend_clicked(bool checked)
{
    struCnfg.nValveDefend = (int)checked;
    updateValveDefend();
    myFlow.resetMachineConfigRG();
}

void MachineFunction::updateScreenSet()
{
    ui->screenSetCheckBox->setChecked(struCnfg.nScreenSet);
}

void MachineFunction::updateSGTimeoutSet()
{
    ui->ledit_cmdSendTimeout->setMinMax(this,0,255,0,&struCnfg.nCmdSendTimeout);
    ui->ledit_cmdSendTimeout->setText(QString::number(struCnfg.nCmdSendTimeout));
    ui->ledit_ugSendTimeout->setMinMax(this,0,255,0,&struCnfg.nUGSendTimeout);
    ui->ledit_ugSendTimeout->setText(QString::number(struCnfg.nUGSendTimeout));
    ui->ledit_ugSendInterval->setMinMax(this,0,255,0,&struCnfg.nUgSendInterval);
    ui->ledit_ugSendInterval->setText(QString::number(struCnfg.nUgSendInterval));
    ui->ledit_waitTimeout->setMinMax(this,0,255,0,&struCnfg.nWaitTimeout);
    ui->ledit_waitTimeout->setText(QString::number(struCnfg.nWaitTimeout));

}

void MachineFunction::on_screenSetCheckBox_clicked(bool checked)
{
    struCnfg.nScreenSet = (checked) ? 1:0;
}

void MachineFunction::on_alarmVoiceTestBtn_clicked()
{
    char data[SEND_PACKET_DATA_SIZE];
    memset(data, 0, SEND_PACKET_DATA_SIZE);
    //data[0] = (struCnfg.nControlAlarmSet && struGsh.nAlarmSetData0);

    switch(struCnfg.struAlarmConfig.nLightVoiceAlarmType)
    {
    case 0: //不使能
        //mySerial.pushCom1CmdData(CMD_CTRL_ALARM_SET, 0, BOARD_TYPE_CONTROL, BOARD_TYPE_ALL_IN_CHAIN, data, 3);   //默认第一个控制板清灰
        break;
    case 1: //使能灯光+声音
        data[1] = 0x09;
        break;
    case 2: //仅灯光
        data[1] = 0x01;

        break;
    case 3: //仅声音
        data[1] = 0x08;
        break;
    default:
        break;
    }
    for(int i  =0 ; i < struCnfg.bLogoLightNum; i++)
    {

        data[3+i*2] = struCnfg.bLogoLightMode[i];
        data[4+i*2] = struCnfg.bLogoLightLength[i];
    }


    mySerial.pushCom1CmdData(CMD_CTRL_ALARM_SET, 0, BOARD_TYPE_CONTROL, BOARD_TYPE_ALL_IN_CHAIN, data, 3);   //默认第一个控制板清灰
}

void MachineFunction::on_bmMachine_clicked(bool checked)
{
    struCnfg.nUseBMModule = (int)checked;
    if(checked)
        struCnfg.nIpcIpAddrBase = 1;
    else
        struCnfg.nIpcIpAddrBase = 101;

}

void MachineFunction::on_d2Mode1Edit_textChanged(const QString &arg1)
{
    const char *strName = arg1.toUtf8().constData();
    if(strlen(strName) < sizeof(struCnfp.sD2ImgModeName[0]))
    {
        memset(struCnfp.sD2ImgModeName[0],0,sizeof(struCnfp.sD2ImgModeName[0]));
        memcpy(struCnfp.sD2ImgModeName[0],strName,strlen(strName));
    }
}

void MachineFunction::on_d2Mode2Edit_textChanged(const QString &arg1)
{
    const char *strName = arg1.toUtf8().constData();
    if(strlen(strName) < sizeof(struCnfp.sD2ImgModeName[1]))
    {
        memset(struCnfp.sD2ImgModeName[1],0,sizeof(struCnfp.sD2ImgModeName[1]));
        memcpy(struCnfp.sD2ImgModeName[1],strName,strlen(strName));
    }
}

void MachineFunction::on_d2Mode3Edit_textChanged(const QString &arg1)
{
    const char *strName = arg1.toUtf8().constData();
    if(strlen(strName) < sizeof(struCnfp.sD2ImgModeName[2]))
    {
        memset(struCnfp.sD2ImgModeName[2],0,sizeof(struCnfp.sD2ImgModeName[2]));
        memcpy(struCnfp.sD2ImgModeName[2],strName,strlen(strName));
    }
}

void MachineFunction::on_d2Mode4Edit_textChanged(const QString &arg1)
{
    const char *strName = arg1.toUtf8().constData();
    if(strlen(strName) < sizeof(struCnfp.sD2ImgModeName[3]))
    {
        memset(struCnfp.sD2ImgModeName[3],0,sizeof(struCnfp.sD2ImgModeName[3]));
        memcpy(struCnfp.sD2ImgModeName[3],strName,strlen(strName));
    }

}

void MachineFunction::on_lowcfg_sync_clicked(bool checked)
{
    struCnfg.nUseLowcfgSync = (int)checked;
}

void MachineFunction::on_restoreBtn_clicked()
{
    myMessageBox msgBox(MSG_QUES, g_myLan().restore+"?");

    int ret = msgBox.exec();
    if (ret == QDialog::Accepted)
    {
        struCnfg.nCmdSendTimeout = 10;//ms
        struCnfg.nUGSendTimeout = 25; //s
        struCnfg.nUgSendInterval = 8; //ms
        struCnfg.nWaitTimeout = 100;  //ms
    }
}

void MachineFunction::on_camParaInCnfCheckBox_clicked(bool checked)
{
    struCnfg.camParaInCnf = (int)checked;
}



void MachineFunction::updateBalanceCtrl()
{
    ui->checkBox_radar->setChecked(m_feedAuto.isHasRadar == 1);
    ui->checkBox_radar->hide();

    ui->containDeptyEdit->setText(QString::number(m_feedAuto.riceContainerDepth));
    ui->emptyposiEdit->setText(QString::number(m_feedAuto.levelEmptyPosi));
    ui->levelmoreposiEdit->setText(QString::number(m_feedAuto.levelMorePosi));
    ui->fullposiEdit->setText(QString::number(m_feedAuto.levelFullPosi));
    ui->feedclearvalueEdit->setText(QString::number(m_feedAuto.feedClearModeValue));
    ui->feedemptyvalueEdit->setText(QString::number(m_feedAuto.feedEmpytLowLimitValue));
    ui->fullcontinueedit->setText(QString::number(m_feedAuto.feedBalAdjustStep));
    ui->balpartcountedit->setText(QString::number(m_feedAuto.levelLowPosi));

    ui->levelstatchangewaitedit->setText(QString::number(m_feedAuto.feedLevelChangeWaitTime));

    ui->feedprefullvalueEdit->setText(QString::number(m_feedAuto.feedPreFullConstValue));
    ui->prefullcontinueedit->setText(QString::number(m_feedAuto.feedBalAdjustDuration));

    ui->flowbaluplimitEdit->setText(QString::number(m_feedAuto.posiChgThres));
    ui->feedbaluplimitEdit->setText(QString::number(m_feedAuto.posiChgWait));
    ui->feedbalupflowEdit->setText(QString::number(m_feedAuto.balUpFeedflow));
    ui->feedballowflowEdit->setText(QString::number(m_feedAuto.balLowFeedflow));

    ui->balmodecomboBox->setCurrentIndex(m_feedAuto.autoFeedMode);
    ui->sensortypecmx->setCurrentIndex(m_feedAuto.isHasRadar);

    switch(m_feedAuto.autoFeedMode)
    {
    case AUTO_FEED_MODE_0:
        ui->moreposilabel->show();
        ui->levelmoreposiEdit->show();
        ui->feedclearLabel->show();
        ui->feedclearvalueEdit->show();
        ui->balpartcountlabel->hide();
        ui->balpartcountedit->hide();
        break;
    case AUTO_FEED_MODE_1:
        ui->moreposilabel->hide();
        ui->levelmoreposiEdit->hide();
        ui->feedclearLabel->hide();
        ui->feedclearvalueEdit->hide();
        ui->balpartcountlabel->show();
        ui->balpartcountedit->show();
        break;
    case AUTO_FEED_MODE_2:
        ui->moreposilabel->hide();
        ui->levelmoreposiEdit->hide();
        ui->feedclearLabel->hide();
        ui->feedclearvalueEdit->hide();
        ui->balpartcountlabel->hide();
        ui->balpartcountedit->hide();
        break;
    case AUTO_FEED_MODE_3:

        ui->fullposiLabel->show();
        ui->emptyposiEdit->show();
        ui->fullposiEdit->show();
        ui->containDeptyEdit->show();

        ui->moreposilabel->show();
        ui->levelmoreposiEdit->show();
        ui->feedclearLabel->show();
        ui->feedclearvalueEdit->show();
        ui->balpartcountlabel->show();
        ui->balpartcountedit->show();

        ui->feedballowflowLabel->show();
        ui->feedbalupflowLabel->show();
        ui->feedballowflowEdit->show();
        ui->feedbalupflowEdit->show();
        break;
    }

    if(m_feedAuto.isHasRadar == 2)  //料位开关模式，默认模式2，且单个目标值
    {
        ui->balpartcountedit->setEnabled(false);

        ui->fullposiLabel->hide();
        ui->fullposiEdit->hide();
        ui->moreposilabel->hide();
        ui->levelmoreposiEdit->hide();

        ui->emptyposilabel->hide();
        ui->emptyposiEdit->hide();
        ui->containDeptylabel->hide();
        ui->containDeptyEdit->hide();
    }
    else{
        ui->fullposiLabel->show();
        ui->fullposiEdit->show();
        ui->moreposilabel->show();
        ui->levelmoreposiEdit->show();

        ui->emptyposilabel->show();
        ui->emptyposiEdit->show();
        ui->containDeptylabel->show();
        ui->containDeptyEdit->show();

        ui->balpartcountedit->setEnabled(true);
    }

    ui->sensortypecmx->setEnabled(false);
    ui->balmodecomboBox->setEnabled(false);

}

void MachineFunction::on_autoApplyBtn_clicked()
{
    int err = 0;

    m_feedAuto.autoFeedMode = AUTO_FEED_MODE_3;
    m_feedAuto.isHasRadar = 1;

    if(m_feedAuto.levelEmptyPosi  <= m_feedAuto.levelMorePosi) //
    {
        err = 1;
    }
    if(m_feedAuto.levelEmptyPosi  >= m_feedAuto.riceContainerDepth) //
    {
        err = 1;
    }
    if(m_feedAuto.autoFeedMode == AUTO_FEED_MODE_0)
    {
        if(m_feedAuto.levelMorePosi <= m_feedAuto.levelFullPosi) //
        {
            err = 1;
        }
    }

    if(err == 1)
    {
        g_infoWidget().setLabelText(g_myLan().msg_compute_error);
        g_infoWidget().delayShow();
        memcpy(&m_feedAuto, &struCnfg.stuFeedAutoCtrl, sizeof(struCnfg.stuFeedAutoCtrl));
        myFlow.sleep(2);
        g_infoWidget().hide();
    }
    else
    {
        g_infoWidget().setLabelText(g_myLan().msg_applying);
        g_infoWidget().delayShow();
        memcpy(&struCnfg.stuFeedAutoCtrl, &m_feedAuto, sizeof(m_feedAuto));
        myFlow.sleep(2);
        g_infoWidget().hide();
    }
    updateBalanceCtrl();
}

void MachineFunction::on_balmodecomboBox_activated(int index)
{
    m_feedAuto.autoFeedMode = index;
    updateBalanceCtrl();
}

void MachineFunction::on_sensortypecmx_activated(int index)
{
    m_feedAuto.isHasRadar = index;

    if(m_feedAuto.isHasRadar == 2)  //料位开关模式，默认模式2，且单个目标值
    {
        m_feedAuto.autoFeedMode = AUTO_FEED_MODE_1;
        m_feedAuto.mode1FeedTargetPartCount = 1;
    }
    updateBalanceCtrl();
}
void MachineFunction::on_accelerationBtn_clicked()
{
    if(struCnfg.accChecked)
        g_MainManager().ShowWidget(SM_ACCELERATION_SENSOR_SET_WIDGET);
}

void MachineFunction::on_alarmCmdTypeCombobox_activated(int index)
{
    struCnfg.alarmCmdType = index;
}

void MachineFunction::on_accCheckbox_clicked(bool checked)
{
    struCnfg.accChecked = checked;
}

void MachineFunction::on_updateWifiBtn_clicked()
{
    if(!g_Runtime().checkUsbExist()){
        g_infoWidget().setLabelText(g_myLan().msg_usb_insert);
        g_infoWidget().showSecs();
        return;
    }
    QString path = g_Runtime().getUsbPath();
    if (path == ""){
        g_infoWidget().setLabelText(g_myLan().msg_usb_not_recognized);
        g_infoWidget().showSecs();
        return;
    }


#ifdef Q_OS_UNIX

    QString cmd = "rm  -rf /sdcard/wifi";
    if(!g_Runtime().mySystem(cmd))
    {

        g_infoWidget().setLabelText(g_myLan().ids_upgradeFailed);
        g_infoWidget().showSecs();
        return;
    }


    // 更新WIFI文件
    cmd = "unzip "+path+"wifi.zip "+ " -d /sdcard/";
    if(!g_Runtime().mySystem(cmd))
    {

        g_infoWidget().setLabelText(g_myLan().ids_upgradeFailed);
        g_infoWidget().showSecs();
        return;
    }

    // 更新内核文件
    cmd = "cp -f "+path + "uImage" + "  /media/mmcblk1p1/1-emmc-uImage";
    if(!g_Runtime().mySystem(cmd))
    {

        g_infoWidget().setLabelText(g_myLan().ids_upgradeFailed);
        g_infoWidget().showSecs();
        return;
    }

    struGsh.poweroff_status = 1;
    QSettings setting(CFG_APPSet,QSettings::IniFormat);
    setting.setValue("poweroffstatus",struGsh.poweroff_status);
    setting.sync();
    //        g_infoWidget().setLabelText(g_myLan().ids_upgradeFailed);
    //        g_infoWidget().showSecs();
    //        return;
    //    }

#endif

    g_infoWidget().setLabelText(g_myLan().ids_importFinished);
    g_infoWidget().showSecs();

}

void MachineFunction::on_pushButton_12_clicked()
{
    if(!g_Runtime().checkUsbExist()){
        g_infoWidget().setLabelText(g_myLan().msg_usb_insert);
        g_infoWidget().showSecs();
        return;
    }
    QString path = g_Runtime().getUsbPath();
    if (path == ""){
        g_infoWidget().setLabelText(g_myLan().msg_usb_not_recognized);
        g_infoWidget().showSecs();
        return;
    }


    QSettings setting(CFG_APPSet,QSettings::IniFormat);
    QString connectName = setting.value("connectName","").toString();

    if(connectName == "")
        return;

#ifdef Q_OS_UNIX
    char *cmd = "ls /sys/class/scsi_device/";
    char buf[128];
    FILE *fp;

    if ((fp = popen( cmd , "r")) == NULL)
    {
        g_infoWidget().setLabelText("Error");
        g_infoWidget().delayShow();
        myFlow.sleep(2);
        g_infoWidget().hide();
        return;
    }
    memset(buf, 0 ,sizeof(buf));
    fread(buf, 1, 7, fp);

    // 未查出U盘信息，判空
    if(buf != NULL){
        if(strlen(buf) == 0)
        {
            g_infoWidget().setLabelText(g_myLan().cfm_insert_udisk);
            g_infoWidget().delayShow();


            myFlow.sleep(2);
            g_infoWidget().hide();
            return;
        }
        else
        {
            g_infoWidget().setLabelText(g_myLan().ids_exportImage);
            g_infoWidget().delayShow();

            QString cmdstr = QString("cp -fr /sdcard/") + connectName + "  " + path;
            char *cmd1 = QString(cmdstr).toLatin1().data();
            system(cmd1);



            system("sync");
            myFlow.sleep(1);

            g_infoWidget().hide();
        }
    }
    else
    {
        return;
    }
    g_infoWidget().setLabelText(g_myLan().ids_exportImageFinished);
    g_infoWidget().delayShow();

    myFlow.sleep(2);

    g_infoWidget().hide();
#endif
}
void MachineFunction::updateSelectModeSet()
{

    QString temp = QString("%1").arg(struCnfg.stumodeLableList.labelname[0]);

    if(temp == "")
    {
        ui->comboBox_2->hide();
        ui->comboBox_5->hide();
        ui->label_26->hide();
        ui->label_27->hide();
        return;
    }

    ui->label_26->setText(struCnfg.stumodeLableList.labelname[0]);
    ui->label_27->setText(struCnfg.stumodeLableList.labelname[4]);

    QStringList templist ;
    ui->comboBox_2->clear();
    ui->comboBox_5->clear();
    for(int i =0;  i < MAX_MODE_BIGLABLE_NUM ; i++)
    {
        if(i >= struCnfg.stumodeLableList.labelnum)
            break;

        if(i == 0 )   // 产品线
        {
            templist.clear();
            templist.append("");
            for(int j = 0; j < MAX_MODELABLE_NUM ; j++)
            {
                if(j >= struCnfg.stumodeLableList.labelinfo[i].labelnum)
                    break;

                templist.append(QString("%1").arg(struCnfg.stumodeLableList.labelinfo[i].labelname[j]));

            }
            ui->comboBox_2->addItems(templist);
            ui->comboBox_2->setCurrentIndex(struCnfg.stumodeLableSelect.labelindex[i]);
        }

        if(i == 4) // 地区
        {

            templist.clear();
            templist.append("");
            for(int j = 0; j < MAX_MODELABLE_NUM ; j++)
            {
                if(j >= struCnfg.stumodeLableList.labelinfo[i].labelnum)
                    break;

                templist.append(QString("%1").arg(struCnfg.stumodeLableList.labelinfo[i].labelname[j]));
            }
            ui->comboBox_5->addItems(templist);
            ui->comboBox_5->setCurrentIndex(struCnfg.stumodeLableSelect.labelindex[i]);
        }

    }
}

void MachineFunction::on_comboBox_2_activated(int index)
{
    struCnfg.stumodeLableSelect.labelindex[0] = index; // 产品线
}

void MachineFunction::on_comboBox_5_activated(int index)
{
    struCnfg.stumodeLableSelect.labelindex[4] = index; //  地区
}
void MachineFunction::on_AutoCtrlcheckBox_clicked(bool checked)
{
    struCnfg.bautoplcctrlEnable = (int)checked;
    ui->lampcheckBox->setCheckable(checked);
    update();
}
void MachineFunction::on_PowerCutcheckBox_clicked(bool checked)
{
    struCnfg.nPowerCut =  (int)checked;
}
void MachineFunction::on_DefaultPlccheckBox_clicked(bool checked)
{
    struCnfg.nDefaultPlcCtrl = (int)checked;
}
void MachineFunction::on_lampcheckBox_clicked(bool checked)
{
    struCnfg.blampctrlEnable = (int)checked;
}

void MachineFunction::on_screenShot_clicked()
{

    QSettings setting(CFG_APPSet,QSettings::IniFormat);
    QString connectName = setting.value("connectName","").toString();


    if(connectName == "")
    {
        g_infoWidget().setLabelText(g_myLan().Please_cfm_machine_number);
        g_infoWidget().delayShow();
        myFlow.sleep(5);
        g_infoWidget().hide();

        return;
    }
    // 确认当前用户权限
    myMessageBox msgBox_1(MSG_QUES,  g_myLan().cfm_screen_shot);
    int ret = msgBox_1.exec();
    if(ret == QDialog::Accepted)
    {

        g_Runtime().mySystem("rm -rf /sdcard/"+connectName);

        system("sync");

        myFlow.sleep(1);
        struGsh.bScreenShotId = Project;

        while(struGsh.bScreenShotId < MaxNum)
        {
            struGsh.bScreenShotStatus = 1;

            if(struGsh.bScreenShotId == Project )
            {

                g_MainManager().ShowWidget(SM_MODE_MANAGE_WIDGET);
            }
            else if(struGsh.bScreenShotId == SoftVersion)
            {
                g_MainManager().ShowWidget(SM_SYSSTATE_WIDGET);
            }
            else if(struGsh.bScreenShotId == Sense)
            {
                g_MainManager().ShowWidget(SM_SENSE_SET_WIDGET);
            }
            else if(struGsh.bScreenShotId == Arith)
            {
                g_MainManager().ShowWidget(SM_IDENTIFY_WIDGET);
            }
            else if(struGsh.bScreenShotId == LampConfig)
            {
                g_MainManager().ShowWidget(SM_LAMP_BOARD_SET_WIDGET);
            }
            else if(struGsh.bScreenShotId == LightConfig)
            {
                g_MainManager().ShowWidget(SM_DOUBLE_LIGHE_SET_WIDGET);
            }
            else if(struGsh.bScreenShotId == Backgroud)
            {
                g_MainManager().ShowWidget(SM_BACKGROUND_SET_WIDGET);
            }
            else if(struGsh.bScreenShotId == DelayTime)
            {
                g_MainManager().ShowWidget(SM_EJECT_TIME_WIDGET);
            }
            else if(struGsh.bScreenShotId == MachineSet)
            {
                g_MainManager().ShowWidget(SM_MACHINE_SET_WIDGET);
            }
            else if(struGsh.bScreenShotId == CameraWave)
            {
                g_MainManager().ShowWidget(SM_CAMERA_SIGNAL_WIDGET);
            }
            else if(struGsh.bScreenShotId == CameraSet)
            {
                g_MainManager().ShowWidget(SM_CAMERA_SET_WIDGET);
            }
            else if(struGsh.bScreenShotId == ModelSense)
            {
                g_MainManager().ShowWidget(SM_IPC_CLASS_PARAMS_WIDGET);
            }


            while(struGsh.bScreenShotStatus == 1)
                myFlow.sleep(1);

            struGsh.bScreenShotId++ ;

        }

        g_MainManager().ShowWidget(SM_MAIN_PAGE_WIDGET_NEW);

        struGsh.bScreenShotId = None;

        g_infoWidget().setLabelText( g_myLan().screen_shot_finish);
        g_infoWidget().showSecs(2);
    }





}



void MachineFunction::updateConnectState(bool isConnected)
{

    if(isConnected)
        ui->connectStatus->setText(g_myLan().Server_connection+":"+ g_myLan().ids_run);
    else
        ui->connectStatus->setText(g_myLan().Server_connection+":"+ g_myLan().ids_stop);

    ui->connectButton->setEnabled(!isConnected);
    ui->disconnectButton->setEnabled(isConnected);
    ui->serverAddressEdit->setEnabled(false);
    ui->serverAddressEdit->setText(mytcpServer->IPforConnect);

    ui->serverPortEdit->setType(textType);
    ui->serverPortEdit->setText(mytcpServer->PORTforConnect);

}

void MachineFunction::on_connectButton_clicked()
{
    mytcpServer->onConnectButtonPressed();
    updateConnectState(mytcpServer->isconnected);
}

void MachineFunction::on_disconnectButton_clicked()
{
    mytcpServer->onDisconnectButtonPressed();
    logger()->info("Tcp connect off");
    updateConnectState(mytcpServer->isconnected);
}

void MachineFunction::on_serverPortEdit_textChanged(const QString &arg1)
{
    if(arg1 != "")
    {
        mytcpServer->PORTforConnect = arg1;
        QSettings setting(CFG_APPSet,QSettings::IniFormat);
        setting.setValue("tcpPort",arg1);

    }
}
void MachineFunction::on_crossModeCombox_activated(int index)
{
    struCnfg.bCrossMode1 = index;
    myFlow.resetCrossCam();
}

void MachineFunction::on_ledit_turnRatio_textChanged(const QString &arg1)
{
    struCnfg.overRatio = QString(arg1).toInt();
}

void MachineFunction::on_ledit_turnRatioTime_textChanged(const QString &arg1)
{
     struCnfg.overRatioTime = QString(arg1).toInt();
}

void MachineFunction::on_ledit_AreaThres_textChanged(const QString &arg1)
{
    struCnfg.areaThres = QString(arg1).toInt();

}
void MachineFunction::on_imgSaturationEnable_clicked(bool checked)
{
     struCnfg.imgSaturationEnable = checked;
}

void MachineFunction::on_checkBox_2_clicked(bool checked)
{
    struCnfg.bMainpageShow = checked;
}
void MachineFunction::updateLogoLight()
{
    for(int i  = 0 ; i < MAX_LAMP_PER_BOARD; i++)
    {
        if(i >=  struCnfg.bLogoLightNum)
        {
            lightid[i]->hide();
            lightmode[i]->hide();
            lightlength[i]->hide();
        }
        else
        {
            lightid[i]->setText(QString::number(i+1));
            lightid[i]->show();

            lightmode[i]->setMinMax(this, 0,5, 2, &struCnfg.bLogoLightMode[i]);
            lightmode[i]->setText(QString::number(struCnfg.bLogoLightMode[i]));
            lightmode[i]->show();

            lightlength[i]->setMinMax(this, 0,255, 2, &struCnfg.bLogoLightLength[i]);
            lightlength[i]->setText(QString::number(struCnfg.bLogoLightLength[i]));
            lightlength[i]->show();
        }
    }

    if(struCnfg.bLogoLightNum == 0)
    {
        ui->label_mode->hide();
        ui->label_length->hide();
    }
}
