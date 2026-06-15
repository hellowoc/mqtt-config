#include "bottomwidget.h"
#include "ui_bottomwidget.h"
#include "global.h"
BottomWidget::BottomWidget(QWidget *parent) :
    BaseUi(parent),
    ui(new Ui::BottomWidget)
{
    ui->setupUi(this);
    setWindowOpacity(0);
    m_showSimulatePageId.append(SM_SENSE_SET_WIDGET);
    m_showSimulatePageId.append(SM_BIG_SMALL_SENSE_SET_WIDGET);
    m_showSimulatePageId.append(SM_CIRCLE_LONG_SENSE_SET_WIDGET);
    m_showSimulatePageId.append(SM_GRAYSENSE_SET_WIDGET);
    m_showSimulatePageId.append(SM_DISCOLOR_SENSE_SET_WIDGET);
    m_showSimulatePageId.append(SM_LONG_SHORT_SENSE_SET_WIDGET);
    m_showSimulatePageId.append(SM_SVM_SENSE_SET_WIDGET);
    m_showSimulatePageId.append(SM_SIMULATE_WIDGET);
    m_showSimulatePageId.append(SM_COLOR_SAT_SENSE_SET_WIDGET);
    m_showSimulatePageId.append(SM_COLOR_HUE_SENSE_SET_WIDGET);
    m_showSimulatePageId.append(SM_COLOR_SCALE_SENSE_SET_WIDGET);
    m_showSimulatePageId.append(SM_MILDEW_SENSE_SET_WIDGET);
    m_showSimulatePageId.append(SM_OUTLINE_SENSE_SET_WIDGET);
    m_showSimulatePageId.append(SM_LINE_SENSE_SET_WIDGET);
    m_showSimulatePageId.append(SM_BUD_SENSE_SET_WIDGET);
    m_showSimulatePageId.append(SM_ROUND_SENSE_SET_WIDGET);
    m_showSimulatePageId.append(SM_COLOR_VALUE_SENSE_SET_WIDGET);
    m_showSimulatePageId.append(SM_HSV_SENSE_SET_WIDGET);
    m_showSimulatePageId.append(SM_SPL_MILDEW_SENSE_SET_WIDGET);
    m_showSimulatePageId.append(SM_WHITE_PEANUT_SENSE_SET_WIDGET);
    m_showSimulatePageId.append(SM_SHAPE_SVM_SENSE_SET_WIDGET);
    m_showSimulatePageId.append(SM_PEPPER_HANDLE_SENSE_SET_WIDGET);
    m_showSimulatePageId.append(SM_SUPPRESS_LARGE_SENSE_SET_WIDGET);

    connect(ui->widget,SIGNAL(switchToggledSignal(bool)),this,SLOT(onFeedOnOffSlt(bool)));
    connect(ui->widget_2,SIGNAL(switchToggledSignal(bool)),this,SLOT(onVavleOnOffSlt(bool)));

    m_showSimulate = false;
    ui->simulateBtn->setCheckable(true);  
    ui->imgPageCut->hide();
}

BottomWidget::~BottomWidget()
{
    delete ui;
}

void BottomWidget::showPage(bool bshow)
{
    retranslateUiWidget();
}
void BottomWidget::PageChanged(int pageId)
{
    /* 返回按钮 */
    if(pageId == SM_MAIN_PAGE_WIDGET_NEW ||pageId == SM_MAIN_PAGE_WIDGET || pageId == SM_SIMULATE_WIDGET){
        ui->backBtn->hide();
    }
    else
    {
        ui->backBtn->show();
    }

    /* 仿真按钮 */
    if(m_showSimulatePageId.contains(pageId))
    {
        ui->simulateBtn->show();
        if(struCnfg.nDPMachine)
            ui->ipcClassBtn->show();
        //ui->moreBtn->show();
        if(pageId == SM_SENSE_SET_WIDGET)
        {
            //ui->moreBtn->setEnabled(true);
            ui->moreBtn->show();
        }
        else
        {
            //ui->moreBtn->setEnabled(false);
            ui->moreBtn->hide();
        }

    }else{
        ui->simulateBtn->hide();
        ui->ipcClassBtn->hide();
        ui->moreBtn->hide();
    }

    /* 喷阀自检按钮 */
    if(pageId == SM_VAVLE_TEST_WIDGET)
    {
        ui->vavletest->show();
        ui->moreBtn_2->show();
        ui->moreBtn_2->setEnabled(true);
    }
    else
    {
        ui->vavletest->hide();
        ui->moreBtn_2->hide();
    }

    /* 喷阀&下料开关按钮 */
    if((pageId == SM_MAIN_PAGE_WIDGET)||(pageId == SM_VAVLE_TEST_WIDGET)){
        ui->widget->hide();
        ui->widget_2->hide();
    }else{
        ui->widget->show();
        ui->widget_2->show();

        refreshFeedVavleStatus();
    }
    /* 加密和喷阀自检界面，不使能 */
    if(pageId == SM_MACHINE_DCCRYPT_WIDGET || pageId == SM_SELF_CHECK_WIDGET
            || struCnfg.nPlcControl){
        ui->widget->setEnabled(false);
        ui->widget_2->setEnabled(false);
    }else{
        ui->widget->setEnabled(true);
        ui->widget_2->setEnabled(true);
    }

    if (struGsh.bRunMode == RUNMODE_USER){
        ui->moreBtn->hide();
        ui->simulateBtn->hide();
    }
    else if(struGsh.bRunMode == RUNMODE_ENGINEER){
        ui->moreBtn_2->hide();
    }

    if (pageId == SM_SIMULATE_WIDGET){
        ui->imgPageCut->show();
    }
    else{
        ui->imgPageCut->hide();
    }
    if(pageId == SM_REMOTE_CONTROL_WIDGET)
    {
        ui->widget->hide();
        ui->widget_2->hide();
        ui->imgPageCut->hide();
        ui->moreBtn->hide();
        ui->moreBtn_2->hide();
        ui->simulateBtn->hide();
        ui->backBtn->hide();
        ui->vavletest->hide();
    }
    if(pageId == SM_PLC_CTRL_WIDGET)
    {
        ui->widget->hide();
        ui->widget_2->hide();
        ui->imgPageCut->hide();
        ui->moreBtn->hide();
        ui->moreBtn_2->hide();
        ui->simulateBtn->hide();
        ui->vavletest->hide();
        ui->ipcClassBtn->hide();
    }
}

void BottomWidget::setValveEnable(bool enable)
{
    ui->widget->setEnabled(enable);
    ui->widget_2->setEnabled(enable);
}
void BottomWidget::retranslateUiWidget()
{
    ui->simulateBtn->setText(g_myLan().ai_simulate);
    ui->widget->setText(g_myLan().feeder);
    ui->widget_2->setText(g_myLan().ids_valve);

    ui->moreBtn->setText(g_myLan().ids_more);
    ui->moreBtn_2->setText(g_myLan().ids_more);
    ui->backBtn->setText(g_myLan().ids_back);
    ui->vavletest->setText(g_myLan().ids_valveTest);
    ui->imgPageCut->setText(g_myLan().ids_image);
    ui->ipcClassBtn->setText(g_myLan().ipc_model);
}

void BottomWidget::onFeedOnOffSlt(bool onOff)
{
    if(struGsh.bSortStart == 0)
    {
        if(struGsh.bAlarmStatus == 1)
        {
            g_infoWidget().setLabelText(g_myLan().ids_airpressureAlarm);
            g_infoWidget().delayShow();
            myFlow.sleep(2);
            g_infoWidget().hide();

            refreshFeedVavleStatus();
            return;
        }
//        if(struCnfg.nUseIpcEnable == 1)
//        {
//            if(struIpcShare.allIpcModelLoadOk!= 2 || struIpcShare.allIpcConnecOk == 0)
//            {
//                g_infoWidget().setLabelText(g_myLan().checkIPCOnOff);
//                g_infoWidget().delayShow();
//                myFlow.sleep(2);
//                g_infoWidget().hide();

//                refreshFeedVavleStatus();
//                return;
//            }
//        }

        myMessageBox msgBox(MSG_QUES, g_myLan().cfm_turn_on);

        int ret = msgBox.exec();

        if (ret == QDialog::Accepted)
        {
            logger()->info("sort start");
            g_infoWidget().setLabelText(g_myLan().msg_turning_on);
        }
        else
        {
            refreshFeedVavleStatus();

            myFlow.resetNetWaveStart();

            return;
        }
    }
    else
    {
        g_infoWidget().setLabelText(g_myLan().msg_turning_off);
        logger()->info("sort stop");
    }
    g_infoWidget().delayShow();
    myFlow.onOff();

//    if (!myQueryStatisticThread->isNeedInserting)
//    {
//        MySqlite mysql;
//        mysql.updateTable();
//        mysql.clearStagedThroughout();
//        myQueryStatisticThread->isNeedInserting = false;
//        struGsh.nStaticCounter = 0;
//    }
//    if(myQueryStatisticThread->isQuerying)
//    {
//        myQueryStatisticThread->isQuerying = false;
//        myFlow.msleep(200);
//    }
//    if (myQueryStatisticThread->isNeedInserting)
//    {
//        myFlow.msleep(500);
//    }

    myFlow.sleep(1);

    refreshFeedVavleStatus();

    myFlow.resetNetWaveStart();

    g_infoWidget().hide();
}

void BottomWidget::onVavleOnOffSlt(bool onOff)
{
    struGsh.bStatEject = onOff;

    logger()->info("vavle onoff-%1", onOff);
    refreshFeedVavleStatus();

    myFlow.vavleOnOff();

    myFlow.resetNetWaveStart();
}

void BottomWidget::refreshFeedVavleStatus()
{
    if(struGsh.bSortStart == 0)
    {
        ui->widget->setToggle(false);
    }
    else
    {
        ui->widget->setToggle(true);
    }

    if(struGsh.bStatEject == 0)
    {
        ui->widget_2->setToggle(false);
    }
    else
    {
        ui->widget_2->setToggle(true);
    }
    ui->backBtn->setVisible(!struGsh.bContinuous);

}

void BottomWidget::on_simulateBtn_clicked()
{
    g_Runtime().updateTmpImgList();
    if(g_Runtime().nimageNameVec.isEmpty())
    {
        g_infoWidget().setLabelText(g_myLan().msg_capture_image_first);
        g_infoWidget().showSecs(3);
        ui->simulateBtn->setChecked(m_showSimulate);
        return;
    }
    if(!m_showSimulate){
        g_MainManager().ShowWidget(SM_SIMULATE_WIDGET);

    }else{
        g_MainManager().returnBackPage();
    }
    m_showSimulate = !m_showSimulate;
    ui->simulateBtn->setChecked(m_showSimulate);
    ui->simulateBtn->setText((m_showSimulate)?(g_myLan().ids_back):(g_myLan().ai_simulate));
}

void BottomWidget::on_backBtn_clicked()
{
    if(struGsh.nPageSm == SM_PLC_CTRL_WIDGET)
    {
        myMessageBox *mesBox = new myMessageBox(MSG_QUES, g_myLan().cfm_close_plc_ctrl);
        int ret = mesBox->exec();
        if(ret == QDialog::Accepted){
        g_MainManager().returnBackPage();
        }
    }
    else
    {
        g_MainManager().returnBackPage();
    }
    if(struGsh.bScreenShotId < MaxNum)
        struGsh.bScreenShotId = MaxNum; // 停止截屏
}

void BottomWidget::on_moreBtn_clicked()
{
    if (struGsh.bRunMode != RUNMODE_USER)
        g_MainManager().ShowWidget(SM_IDENTIFY_WIDGET);
}

void BottomWidget::on_moreBtn_2_clicked()
{
    struGsh.nLayer = 0;
    struGsh.nView = 0;
    struGsh.nMatGroup = 0;
    g_MainManager().ShowWidget(SM_EJECT_WIDGET);

//    if(struGsh.bFlagFirstPwdValid)
//    {
//        struGsh.nLayer = 0;
//        struGsh.nView = 0;
//        struGsh.nMatGroup = 0;
//        g_MainManager().ShowWidget(SM_EJECT_WIDGET);
//    }
//    else
//    {
//        myInputMethod inputPanel;
//        inputPanel.setType(KB_PD);
//        inputPanel.setInputBtnText(g_myLan().ids_ok,g_myLan().ids_cancel,g_myLan().ids_delete,g_myLan().ids_space);
//        if(inputPanel.exec() == QDialog::Accepted)
//        {
//            QString str = inputPanel.getText();
//            if (str == "9527")
//            {
//                struGsh.bFlagFirstPwdValid = 1;
//                struGsh.nLayer = 0;
//                struGsh.nView = 0;
//                struGsh.nMatGroup = 0;
//                g_MainManager().ShowWidget(SM_EJECT_WIDGET);
//            }
//            else
//            {
//                g_infoWidget().setLabelText(g_myLan().ids_wrongPassword);
//                g_infoWidget().delayShow();
//                myFlow.sleep(2);
//                g_infoWidget().hide();
//            }
//        }
//    }
}

void BottomWidget::on_vavletest_clicked()
{
    g_MainManager().ShowWidget(SM_EJECT_TIME_WIDGET);
//    if(struGsh.bSortStart == 0)
//    {
//        g_MainManager().ShowWidget(SM_VAVLE_TEST_WIDGET);
//    }
//    else
//    {
//        g_infoWidget().setLabelText(g_myLan().msg_turn_off_first);
//        g_infoWidget().delayShow();

//        myFlow.sleep(3);
//        g_infoWidget().hide();
//    }
}

void BottomWidget::on_imgPageCut_clicked()
{
    g_MainManager().ShowWidget(SM_IMAGE_CAPTURE_WIDGET);
}
void BottomWidget::on_ipcClassBtn_clicked()
{
    g_MainManager().ShowWidget(SM_IPC_CLASS_PARAMS_WIDGET);
}
