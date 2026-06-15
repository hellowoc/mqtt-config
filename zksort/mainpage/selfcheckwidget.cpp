#include "selfcheckwidget.h"
#include "ui_selfcheckwidget.h"
#include "global.h"
SelfCheckWidget::SelfCheckWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::SelfCheckWidget)
{
    ui->setupUi(this);
}

SelfCheckWidget::~SelfCheckWidget()
{
    delete ui;
}

void SelfCheckWidget::showPage(bool bshow)
{
    if(bshow){
        showCheckInfo();
        updateCheckInfo();
    }
}

void SelfCheckWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_VERSION){
        g_infoWidget().hide();
        showCheckInfo();
        updateCheckInfo();
        if(g_Runtime().g_selfCheckState){
            struCnfg.bFlagFirstrun = 1;
            //            if(struCnfg.nMachineType == MACHINE_LD_ZL_GP || struCnfg.nMachineType== MACHINE_LD2_ZL_GP){
            if(struCnfg.nLVMachine){
                g_MainManager().ShowWidget(SM_MAIN_PAGE_WIDGET);
            }
            else{
                g_MainManager().ShowWidget(SM_MAIN_PAGE_WIDGET_NEW);
            }
            //            }else{
            //                g_MainManager().ShowWidget(SM_MAIN_PAGE_WIDGET_NEW);
            //            }
        }
    }
}

void SelfCheckWidget::retranslateUiWidget()
{
    ui->label->setText(g_myLan().ids_selfcheck);
    ui->reCheck->setText(g_myLan().recheck);
    ui->enterMainPageBtn->setText(g_myLan().enter_system);
}

void SelfCheckWidget::on_enterMainPageBtn_clicked()
{
    //selfcheck error 手动进入主界面
    g_infoWidget().setLabelText(g_myLan().msg_system_init);
    g_infoWidget().delayShow();
    myFlow.initSendAllParams();
    myFlow.sleep(3);
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
    //    myMqttMsgParaseThread->n_uploadPartsStatusFlag = 1;
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
    if(struCnfg.nDefaultPlcCtrl == 1 || struCnfg.nPlcCtrl == 1)
    {
        g_MainManager().ShowWidget(SM_PLC_CTRL_WIDGET);
    }
}

void SelfCheckWidget::showCheckInfo()
{
    m_infoList.clear();
    m_infoStateVec.clear();
    int state;
    QString namestr("");
    state = 0;
    stru_version version;
    memcpy(&version,&struGsh.struVer,sizeof(version));

    namestr = g_myLan().ids_interfaceBoard;
    state = version.nInterfaceRes;
    m_infoList.append(namestr);
    m_infoStateVec.append(state);

    for(int n = 0;n < struCnfg.nLayerTotal;n++)//相机板链
    {
        for(int i = 0;i < struCnfg.struLayerInfo[n].nViewChainTotal;i++){
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[i]) continue;
            for(int j = 0;j < struCnfg.struLayerInfo[n].nViewUnitTotal[i];j++){
                namestr = myFlow.getUnitShowName(i,j);

                state = version.nUnitRes[n][i][j];
                m_infoList.append(namestr);
                m_infoStateVec.append(state);
            }
        }
    }

    for(int n = 0;n < struCnfg.nLayerTotal;n++)//阀板链
    {
        for(int i = 0;i < struCnfg.struLayerInfo[n].nVavleChainTotal;i++){
            for(int j = 0;j < struCnfg.struLayerInfo[n].nVavleBoardTotal[i];j++){
                switch(struCnfg.struLayerInfo[n].stuVavleInfo[i].nNameIndex){
                case MAIN_LAYER_VAVLE:
                    namestr = QString("%1 %2").arg(g_myLan().main_vavle).arg(j+1);
                    break;
                case FIRST_LAYER_VAVLE:
                    //namestr = QString("%1 %2").arg(g_myLan().vavle_type1).arg(j+1);
                    namestr = QString("%1%2 %3").arg(g_myLan().upper_layer).arg(g_myLan().main_vavle).arg(j+1);
                    break;
                case SECOND_LAYER_VAVLE:
                    //namestr = QString("%1 %2").arg(g_myLan().vavle_type2).arg(j+1);
                    namestr = QString("%1%2 %3").arg(g_myLan().lower_layer).arg(g_myLan().main_vavle).arg(j+1);
                    break;
                }
                state = version.nVavleRes[n][i][j];
                m_infoList.append(namestr);
                m_infoStateVec.append(state);
            }
        }
    }

    for(int j = 0;j < struCnfg.stuLayerLampCfg.nLampBoardTotal;j++){//灯光板
        namestr = QString("%1 %2").arg(g_myLan().ids_ccsBoard).arg(j+1);
        state = version.nLampRes[j];
        m_infoList.append(namestr);
        m_infoStateVec.append(state);
    }

    if(!struCnfg.nAllInOne)
    {
        for(int j = 0;j < struCnfg.nCtrlBoardTotal;j++){//控制板
            namestr = QString("%1 %2").arg(g_myLan().ids_controlBoard).arg(j+1);
            state = version.nCtrlRes[j];
            m_infoList.append(namestr);
            m_infoStateVec.append(state);
        }

        //智能控制板(一台机器一个远程控制板，一键开关机使用)
        if(struCnfg.bautoplcctrlEnable)
        {
            namestr = QString("%1 %2").arg(g_myLan().ids_autoControlBoard).arg(1);
            state = version.nAutoCtrlRes;
            m_infoList.append(namestr);
            m_infoStateVec.append(state);

            for(int j = 0;j < struCnfg.stuLayerLampCfg.nLampBoardTotal;j++){//灯光板
                namestr = QString("%1 %2").arg(g_myLan().ids_ccsBoard).arg(j+1);
                state = version.nLampRes[j];
                m_infoList.append(namestr);
                m_infoStateVec.append(state);
            }
        }

    }
}

void SelfCheckWidget::updateCheckInfo()
{
    if(m_infoList.isEmpty())
        return;
    if(m_infoList.count() != m_infoStateVec.count()){
        logger()->info("self check error");
        return;
    }
    while (ui->listWidget->count()>0) {
        QListWidgetItem *item = ui->listWidget->takeItem(0);
        if(item){
            delete item;
        }
    }
    ui->listWidget->clear();

    QString showInfoStr("");
    for(int i = 0;i < m_infoList.count();i++){
        showInfoStr = m_infoList[i];
        showInfoStr += QString("  ");
        if( m_infoStateVec[i] != 1){
            g_Runtime().g_selfCheckState = false;
            showInfoStr += g_myLan().abnormal;
            QListWidgetItem *item= new QListWidgetItem(showInfoStr,ui->listWidget);
            item->setTextColor(QColor(255,0,0));
        }else{
            showInfoStr += g_myLan().normal;
            QListWidgetItem *item= new QListWidgetItem(showInfoStr,ui->listWidget);
        }
    }
}

void SelfCheckWidget::on_reCheck_clicked()
{
    while (ui->listWidget->count()>0) {
        QListWidgetItem *item = ui->listWidget->takeItem(0);
        if(item){
            delete item;
        }
    }
    ui->listWidget->clear();
    g_infoWidget().setLabelText(g_myLan().msg_communicating);
    g_infoWidget().delayShow();
    g_Runtime().g_selfCheckState = true;
    myFlow.resetCommunication();
}
