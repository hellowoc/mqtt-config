#include "updatefpgawidget.h"
#include "ui_updatefpgawidget.h"
#include "myfscheck.h"
#include <QRegExp>

#define DEBUG_0613 1

UpdateFpgaWidget::UpdateFpgaWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::UpdateFpgaWidget)
{
    ui->setupUi(this);
    m_count = 0;
    m_nBoardTypeIndex = 0;
    m_nChainIndex = 0;
    m_nChuteIndex = 0;
    m_nMirrorIndex= 0;
    m_view = 0;

    viewList<<QString(g_myLan().lower_layer+g_myLan().ids_fm) <<QString(g_myLan().lower_layer+g_myLan().ids_bm)<<QString(g_myLan().lower_layer+g_myLan().ids_fs)<<QString(g_myLan().lower_layer+g_myLan().ids_bs)
            <<QString(g_myLan().upper_layer+g_myLan().ids_fm) <<QString(g_myLan().upper_layer+g_myLan().ids_bm)<<QString(g_myLan().upper_layer+g_myLan().ids_fs)<<QString(g_myLan().upper_layer+g_myLan().ids_bs);
    viewMap[board_camera] = "";
    viewMap[board_ipc_camera]= "IPC";
    id = 0;
    interfaceNeedChgToUser = false;

    ui->BackBurnBtn->hide();
    ui->comboBox_mode->setVisible(false);
    ui->label_mode->hide();
    isBackBurn = 0;
    ui->pushButton->hide();
    ui->OneKeyUpdateBtn->hide();
}

UpdateFpgaWidget::~UpdateFpgaWidget()
{
    delete ui;
}

void UpdateFpgaWidget::showPage(bool bshow)
{
    if(bshow){
        viewList.clear();
        if (struCnfg.nMachineType == MACHINE_LD2_ZL_GP){
            viewList<<QString(g_myLan().lower_layer+g_myLan().ids_fm) <<QString(g_myLan().lower_layer+g_myLan().ids_bm)<<QString(g_myLan().lower_layer+g_myLan().ids_fs)<<QString(g_myLan().lower_layer+g_myLan().ids_bs)
                    <<QString(g_myLan().upper_layer+g_myLan().ids_fm) <<QString(g_myLan().upper_layer+g_myLan().ids_bm)<<QString(g_myLan().upper_layer+g_myLan().ids_fs)<<QString(g_myLan().upper_layer+g_myLan().ids_bs);
        }
        else{
            viewList<<g_myLan().ids_fm <<g_myLan().ids_bm<<g_myLan().ids_fs<<g_myLan().ids_bs
                    <<g_myLan().ids_fm <<g_myLan().ids_bm<<g_myLan().ids_fs<<g_myLan().ids_bs;

        }
        struGsh.bUpdateState = 1;
        updateAll();
    }
    else
        struGsh.bUpdateState = 0;
}

void UpdateFpgaWidget::receiveMsg(int msg1, int msg2, int msg3)
{

}

bool UpdateFpgaWidget::StringToHexGroup( char *OutHexBuffer, char *InStrBuffer, unsigned int strLength)
{
    unsigned int i, k=0;
    unsigned char HByte,LByte;
    if(strLength%2 !=0)
        return false;

    for(i=0; i<strLength; i=i+2)
    {
        if(InStrBuffer[i]>='0' && InStrBuffer[i]<='9')
        {
            HByte=InStrBuffer[i]-'0';
        }
        else if(InStrBuffer[i]>='A' && InStrBuffer[i]<='F')
        {
            HByte=InStrBuffer[i]-'A' +10;
        }
        else
        {
            HByte=InStrBuffer[i];
            qDebug("aaa");
            return false;
        }
        HByte=HByte <<4;
        HByte = HByte & 0xF0;
        if(InStrBuffer[i+1]>='0' && InStrBuffer[i+1]<='9')
        {
            LByte=InStrBuffer[i+1]-'0';
        }
        else if(InStrBuffer[i+1]>='A' && InStrBuffer[i+1]<='F')
        {
            LByte=InStrBuffer[i+1]-'A' +10;
        }
        else
        {
            LByte=InStrBuffer[i];

            qDebug("bbb");
            return false;
        }

        OutHexBuffer[k++]=(HByte |LByte);
    }
    return true;
}

void UpdateFpgaWidget::retranslateUiWidget()
{
    updateAll();
    ui->label_board->setText(g_myLan().ids_board);
    ui->label_chain->setText(g_myLan().ids_chain);
    ui->label_chute->setText(g_myLan().ids_channel);
    ui->label_mode->setText(g_myLan().ids_model);
    ui->checkFileBtn->setText(g_myLan().ids_checkUpdateFile);
    ui->startUpgradeBtn->setText(g_myLan().ids_startUpdate);
    ui->OneKeyUpdateBtn->setText(g_myLan().OneKey_Update);
}

void UpdateFpgaWidget::appendUpdateResult(QString str,bool ok)
{
    m_count++;
    QString date = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString log =QString::number(m_count)+"    "+date+"    "+str;
    QListWidgetItem *item= new QListWidgetItem(log,ui->FileListWidget);

    if(!ok)
        item->setTextColor(QColor(255,0,0));
    ui->FileListWidget->scrollToBottom();
}

/* 检查升级文件是否匹配 */
bool UpdateFpgaWidget::checkUpdateFile()
{
    if(m_nBoardTypeIndex == board_arm) return 1;

    QString str;
    switch(m_nBoardTypeIndex)
    {
    case board_interface:
        str="INTERFACE";
        break;
    case board_camera:
        str="CAM";
        break;
    case board_ipc_camera:
        str="CAM";
        break;
    case board_valve:
        str="VD";
        break;
    case board_control:
        str="CONTROL";
        break;
    case board_lamp:
        str="LEDDRIVER";
        break;
    case board_control_zd:
        str="ZDJC";
        break;
    case board_control_dl:
        str="DLJC";
        break;
    default:
        break;
    }

    if (!ui->FileListWidget->currentItem())
    {
        ui->FileListWidget->clear();
        appendUpdateResult(g_myLan().ids_notSelectFile);
        return 0;
    }
    QString listinfo = ui->FileListWidget->currentItem()->text();

    if(isBackBurn)
    {

    }
    else
    {
        ui->FileListWidget->clear();
    }


    if (!listinfo.contains(str))
    {
        appendUpdateResult(g_myLan().ids_wrongFileName);
        return 0;
    }
    QStringList list = listinfo.split(" ");
    fileName = QString("%1%2").arg(g_Runtime().getUsbPath()+"bin/").arg(list.at(0));
    QStringList list1 = list.at(0).split("_");
    QString fileName1 = list1.at(list1.length()-1);
    if ( fileName1.at(0) == 'V')
    {
        QStringList vlist = fileName1.remove("V").split(".");
        m_versionToUpgrade[0] = QString(vlist.at(0)).toInt();
        m_versionToUpgrade[1] = QString(vlist.at(1)).toInt();

        struGsh.struVer.sInterface[0] = m_versionToUpgrade[0];
        struGsh.struVer.sInterface[1] = m_versionToUpgrade[1];
    }
    else
    {
        appendUpdateResult(g_myLan().ids_wrongFileName);
        return 0;
    }
    qDebug("\t fileName = %s", fileName.toUtf8().data());
    return 1;
}

void UpdateFpgaWidget::on_startUpgradeBtn_clicked()
{
    qDebug()<<m_nBoardTypeIndex<<m_nChainIndex<<m_nChuteIndex<<m_nMirrorIndex;
    ui->startUpgradeBtn->setEnabled(false);
    m_count = 0;
    if (!checkUpdateFile())
        return;
    if(fileName.contains("APP1"))
        m_nMirrorIndex = 0;
    else if(fileName.contains("APP2"))
        m_nMirrorIndex = 1;

    if (m_nBoardTypeIndex == board_arm){
        myMessageBox *mesBox = new myMessageBox(MSG_QUES, g_myLan().cfm_upgrade_screen);
        int ret = mesBox->exec();
        if(ret == QDialog::Accepted){
            struGsh.poweroff_status = 1;
            QSettings setting(CFG_APPSet,QSettings::IniFormat);
            setting.setValue("poweroffstatus",struGsh.poweroff_status);
            setting.sync();
            logger()->info("update Arm board");

            delete mesBox;
            mesBox = nullptr;
            this->close();
            myFlow.msleep(100);
            QProcess::startDetached("/app/MyUpdate.sh");
            qApp->quit();
        }
        ui->startUpgradeBtn->setEnabled(true);
        return;
    }
    appendUpdateResult(g_myLan().ids_yourFileIs+fileName.split("/").last());
    appendUpdateResult(g_myLan().ids_startUpgrading);

    struGsh.bIsUpgrading = true;
    m_boardAddr = getBoardAddr(m_nBoardTypeIndex,m_nChainIndex);

    myFlow.switchUserMirror(m_nBoardTypeIndex<<4 |FACTORY_MODE,BOARD_TYPE_INTERFACE,0,0);
    id = timer_interface_wait_toFactory ;
    startTimer(time_switchMode);
}

void UpdateFpgaWidget::timerEvent(QTimerEvent *event)
{
    killTimer(event->timerId());
    if(id == timer_interface_wait_toFactory){
        myFlow.resetMachineConfigRG();
        id = timer_ask_interface_version ;
        startTimer(3000);

    }else if(id == timer_ask_interface_version){
        for (int i = 0; i < 3; i++)
        {
            myFlow.readVersion(BOARD_TYPE_INTERFACE,0);
            myFlow.msleep(500);

            if(boardSwitchFactory(board_interface,0,0))
            {
                appendUpdateResult(g_myLan().ids_interfaceBoard+g_myLan().ids_setFactorySucceeded);
                if (m_boardAddr == BOARD_TYPE_INTERFACE)
                {
                    id = timer_tell_upgrade ;
                    startTimer(20);
                    return;
                }
                if(m_nChainIndex == 0){
                    if(m_nBoardTypeIndex == board_camera || m_nBoardTypeIndex == board_ipc_camera ){
                        if(m_nChuteIndex == 0){
                            myFlow.switchUserMirror(m_nBoardTypeIndex<<4|FACTORY_MODE,BOARD_TYPE_ALL_CAMERA,m_nChuteIndex,0);
                        }
                        else{
                            for(int k = 0;k < MAX_VIEW;k++){
                                if (!struCnfg.struLayerInfo[0].nViewIsUsed[k]) continue;
                                //支持任意链的任意通道单独、整条链切换升级
                                myFlow.switchUserMirror(m_nBoardTypeIndex<<4|FACTORY_MODE,getBoardAddr(m_nBoardTypeIndex,k+1),m_nChuteIndex,0);
                            }
                        }
                    }
                    else if(m_nBoardTypeIndex == board_valve)
                    {
                        for(int j = 0; j < struCnfg.struLayerInfo[i].nVavleChainTotal; j++)
                        {
                            for (int m =0;m<struCnfg.struLayerInfo[0].nVavleBoardTotal[j];m++)
                            {
                                myFlow.switchUserMirror(m_nBoardTypeIndex<<4|FACTORY_MODE,getBoardAddr(m_nBoardTypeIndex,j+1),m,0);
                            }
                        }
                    }
                    else if(m_nBoardTypeIndex == board_control)
                    {
                        for (int m =0;m<struCnfg.nCtrlBoardTotal;m++)
                        {
                            myFlow.switchUserMirror(m_nBoardTypeIndex<<4|FACTORY_MODE,getBoardAddr(m_nBoardTypeIndex,1),m,0);
                        }
                    }
                    else if(m_nBoardTypeIndex == board_lamp)
                    {
                        for (int m =0;m<struCnfg.stuLayerLampCfg.nLampBoardTotal;m++)
                        {
                            myFlow.switchUserMirror(m_nBoardTypeIndex<<4|FACTORY_MODE,getBoardAddr(m_nBoardTypeIndex,1),m,0);
                        }
                    }
                    else if(m_nBoardTypeIndex == board_control_zd)
                    {
                        for (int m =0;m<struCnfg.nCtrlBoardTotal;m++)
                        {
                            myFlow.switchUserMirror(m_nBoardTypeIndex<<4|FACTORY_MODE,getBoardAddr(m_nBoardTypeIndex,1),m,BOARD_TYPE_CONTROL_ZD);
                        }
                    }
                    else if(m_nBoardTypeIndex == board_control_dl)
                    {
                        for (int m =0;m<struCnfg.nCtrlBoardTotal;m++)
                        {
                            myFlow.switchUserMirror(m_nBoardTypeIndex<<4|FACTORY_MODE,getBoardAddr(m_nBoardTypeIndex,1),m,BOARD_TYPE_CONTROL_DL);
                        }
                    }
                }else{
                    myFlow.switchUserMirror(m_nBoardTypeIndex<<4|FACTORY_MODE,getBoardAddr(m_nBoardTypeIndex,m_nChainIndex),m_nChuteIndex,0);
                }
                id = timer_others_wait_toFactory ;
                startTimer(time_switchMode);
                return;
            }
            else
            {
                appendUpdateResult(g_myLan().ids_interfaceBoard+g_myLan().ids_setFactoryFailed,false);
                return;
            }
        }
    }else if(id == timer_others_wait_toFactory){
        myFlow.resetMachineConfigRG();
        myFlow.resetSensorConfigRG();
        id = timer_ask_other_version ;
        startTimer(3000);
    }else if(id == timer_ask_other_version){
        memset(m_nBoardFactoryOk,0,sizeof(m_nBoardFactoryOk));

        int i,j,k,m;
        if(m_nChainIndex == 0) // all chain
        {
            if(m_nBoardTypeIndex == board_camera || m_nBoardTypeIndex == board_ipc_camera)
            {
                for(j = 0;j < MAX_VIEW;j++)
                {
                    if (!struCnfg.struLayerInfo[0].nViewIsUsed[j]) continue;
                    if(m_nChuteIndex == 0) //all chute
                    {
                        for (m =0;m<struCnfg.struLayerInfo[0].nViewUnitTotal[j];m++)
                        {
                            for(k = 0; k < 10; k++)
                            {
                                myFlow.readVersion(getBoardAddr(m_nBoardTypeIndex,j+1),m+1);
                                myFlow.msleep(500);
                                //轮询每个通道板的版本信息
                                if(boardSwitchFactory(m_nBoardTypeIndex,j+1,m))
                                {
                                    appendUpdateResult(viewList.at(j)+QString::number(m+1)+viewMap[m_nBoardTypeIndex]+g_myLan().ids_setFactorySucceeded);
                                    m_nBoardFactoryOk[j][m] = 1;
                                    break;
                                }
                            }
                            if (k == 10)
                            {
                                appendUpdateResult(viewList.at(j)+QString::number(m+1)+viewMap[m_nBoardTypeIndex]+g_myLan().ids_setFactoryFailed,false);
                            }
                        }
                    }
                    else// single chute
                    {
                        for(k = 0; k < 10; k++)
                        {
                            myFlow.readVersion(getBoardAddr(m_nBoardTypeIndex,j+1),m_nChuteIndex);
                            myFlow.msleep(500);
                            //轮询每个通道板的版本信息
                            if(boardSwitchFactory(m_nBoardTypeIndex,j+1,m_nChuteIndex-1))
                            {
                                appendUpdateResult(viewList.at(j)+QString::number(m_nChuteIndex)+viewMap[m_nBoardTypeIndex]+g_myLan().ids_setFactorySucceeded);
                                m_nBoardFactoryOk[j][m_nChuteIndex-1] = 1;
                                break;
                            }
                        }
                        if (k == 10)
                        {
                            appendUpdateResult(viewList.at(j)+QString::number(m_nChuteIndex)+viewMap[m_nBoardTypeIndex]+g_myLan().ids_setFactoryFailed,false);
                        }
                    }
                }
            }//end CAM
            else if(m_nBoardTypeIndex == board_valve)
            {
                for(j = 0; j < struCnfg.struLayerInfo[i].nVavleChainTotal; j++)
                {
                    for (m =0;m<struCnfg.struLayerInfo[0].nVavleBoardTotal[j];m++)
                    {
                        for(k = 0; k < 10; k++)
                        {
                            myFlow.readVersion(getBoardAddr(m_nBoardTypeIndex,j+1),m+1);
                            myFlow.msleep(500);
                            //轮询每个阀板的版本信息
                            if(boardSwitchFactory(m_nBoardTypeIndex,j+1,m))
                            {
                                appendUpdateResult(g_myLan().ids_valveBoard+QString::number(m+1)+g_myLan().ids_setFactorySucceeded);
                                m_nBoardFactoryOk[j][m] = 1;
                                break;
                            }
                        }
                        if (k == 10)
                        {
                            appendUpdateResult(g_myLan().ids_valveBoard+QString::number(m+1)+g_myLan().ids_setFactoryFailed,false);
                        }
                    }
                }
            }
            else if(m_nBoardTypeIndex == board_control)
            {
                for (m =0;m<struCnfg.nCtrlBoardTotal;m++)
                {
                    for(k = 0; k < 10; k++)
                    {
                        myFlow.readVersion(getBoardAddr(m_nBoardTypeIndex,1),m+1);
                        myFlow.msleep(500);
                        //轮询每个控制板的版本信息
                        if(boardSwitchFactory(m_nBoardTypeIndex,1,m))
                        {
                            appendUpdateResult(g_myLan().ids_controlBoard+QString::number(m+1)+g_myLan().ids_setFactorySucceeded);
                            m_nBoardFactoryOk[j][m] = 1;
                            break;
                        }
                    }
                    if (k == 10)
                    {
                        appendUpdateResult(g_myLan().ids_controlBoard+QString::number(m+1)+g_myLan().ids_setFactoryFailed,false);
                    }
                }
            }
            else if(m_nBoardTypeIndex == board_lamp)
            {
                for (m =0;m<struCnfg.stuLayerLampCfg.nLampBoardTotal;m++)
                {
                    for(k = 0; k < 10; k++)
                    {
                        myFlow.readVersion(getBoardAddr(m_nBoardTypeIndex,1),m+1);
                        myFlow.msleep(500);
                        //轮询每个阀板的版本信息
                        if(boardSwitchFactory(m_nBoardTypeIndex,1,m))
                        {
                            appendUpdateResult(g_myLan().ids_ccsBoard+QString::number(m+1)+g_myLan().ids_setFactorySucceeded);
                            m_nBoardFactoryOk[j][m] = 1;
                            break;
                        }
                    }
                    if (k == 10)
                    {
                        appendUpdateResult(g_myLan().ids_ccsBoard+QString::number(m+1)+g_myLan().ids_setFactoryFailed,false);
                    }
                }
            }
            else if(m_nBoardTypeIndex == board_control_zd)
            {
                for (m =0;m<struCnfg.nCtrlBoardTotal;m++)
                {
                    for(k = 0; k < 10; k++)
                    {
                        myFlow.readVersion(getBoardAddr(m_nBoardTypeIndex,1),m+1);
                        myFlow.msleep(500);
                        //轮询每个控制板的版本信息
                        if(boardSwitchFactory(m_nBoardTypeIndex,1,m))
                        {
                            appendUpdateResult(g_myLan().ids_accelerationBoard+QString::number(m+1)+g_myLan().ids_setFactorySucceeded);
                            m_nBoardFactoryOk[j][m] = 1;
                            break;
                        }
                    }
                    if (k == 10)
                    {
                        appendUpdateResult(g_myLan().ids_accelerationBoard+QString::number(m+1)+g_myLan().ids_setFactoryFailed,false);
                    }
                }
            }
            else if(m_nBoardTypeIndex == board_control_dl)
            {
                for (m =0;m<struCnfg.nCtrlBoardTotal;m++)
                {
                    for(k = 0; k < 10; k++)
                    {
                        myFlow.readVersion(getBoardAddr(m_nBoardTypeIndex,1),m+1);
                        myFlow.msleep(500);
                        //轮询每个控制板的版本信息
                        if(boardSwitchFactory(m_nBoardTypeIndex,1,m))
                        {
                            appendUpdateResult(g_myLan().ids_halogenBoard+QString::number(m+1)+g_myLan().ids_setFactorySucceeded);
                            m_nBoardFactoryOk[j][m] = 1;
                            break;
                        }
                    }
                    if (k == 10)
                    {
                        appendUpdateResult(g_myLan().ids_halogenBoard+QString::number(m+1)+g_myLan().ids_setFactoryFailed,false);
                    }
                }
            }
        }//end all chain
        else{ //single chain
            if(m_nBoardTypeIndex == board_camera || m_nBoardTypeIndex == board_ipc_camera){
                if(m_nChuteIndex == 0) //all chute
                {
                    for (m =0;m<struCnfg.struLayerInfo[0].nViewUnitTotal[m_view];m++)
                    {
                        for(k = 0; k < 10; k++)
                        {
                            myFlow.readVersion(m_boardAddr,m+1);
                            myFlow.msleep(500);
                            //轮询每个通道板的版本信息
                            if(boardSwitchFactory(m_nBoardTypeIndex,m_view+1,m))
                            {
                                appendUpdateResult(viewList.at(m_view)+QString::number(m+1)+viewMap[m_nBoardTypeIndex]+g_myLan().ids_setFactorySucceeded);
                                m_nBoardFactoryOk[m_view][m] = 1;
                                break;
                            }
                        }
                        if (k == 10)
                        {
                            appendUpdateResult(viewList.at(m_view)+QString::number(m+1)+viewMap[m_nBoardTypeIndex]+g_myLan().ids_setFactoryFailed,false);
                        }
                    }
                }
                else// single chute
                {
                    for(k = 0; k < 10; k++)
                    {
                        myFlow.readVersion(m_boardAddr,m_nChuteIndex);
                        myFlow.msleep(500);
                        //轮询每个通道板的版本信息
                        if(boardSwitchFactory(m_nBoardTypeIndex,m_view+1,m_nChuteIndex-1))
                        {
                            appendUpdateResult(viewList.at(m_view)+QString::number(m_nChuteIndex)+viewMap[m_nBoardTypeIndex]+g_myLan().ids_setFactorySucceeded);
                            m_nBoardFactoryOk[m_view][m_nChuteIndex-1] = 1;
                            break;
                        }
                    }
                    if (k == 10)
                    {
                        appendUpdateResult(viewList.at(m_view)+QString::number(m_nChuteIndex)+viewMap[m_nBoardTypeIndex]+g_myLan().ids_setFactoryFailed,false);
                        myFlow.switchUserMirror(USER_MODE,BOARD_TYPE_INTERFACE,0,0);
                        myFlow.msleep(3000); // Factory chg to User need more time
                        id = timer_interface_wait_toUser ;
                        startTimer(time_switchMode);
                        return;
                    }
                }
            }
        }
        id = timer_tell_upgrade ;
        startTimer(20);
        return;
    }else if(id == timer_tell_upgrade){
        appendUpdateResult(g_myLan().ids_tellToUpgrade);
        if (m_boardAddr == BOARD_TYPE_INTERFACE)
        {
            myFlow.tellUpgrade(m_boardAddr,m_nChuteIndex,m_nBoardTypeIndex<<4|m_nMirrorIndex);
            id = timer_send_file;
            startTimer(2500);
            return;
        }
        if(m_nChainIndex == 0){
            if(m_nBoardTypeIndex == board_camera || m_nBoardTypeIndex == board_ipc_camera){
                //2021/2/25 避免所有视单通道升级仅对前主起作用
                myFlow.tellUpgrade(BOARD_TYPE_ALL_CAMERA,0,m_nBoardTypeIndex<<4|m_nMirrorIndex);
            }
            else if(m_nBoardTypeIndex == board_valve)
            {
                for(int j = 0; j < struCnfg.struLayerInfo[0].nVavleChainTotal; j++)
                {
                    for (int m =0;m<struCnfg.struLayerInfo[0].nVavleBoardTotal[j];m++)
                    {
                        myFlow.tellUpgrade(getBoardAddr(m_nBoardTypeIndex,j+1),m,0);
                    }
                }
            }
            else if(m_nBoardTypeIndex == board_control)
            {
                for (int m =0;m<struCnfg.nCtrlBoardTotal;m++)
                {
                    myFlow.tellUpgrade(getBoardAddr(m_nBoardTypeIndex,1),m,0);
                }
            }
            else if(m_nBoardTypeIndex == board_lamp)
            {
                for (int m =0;m<struCnfg.stuLayerLampCfg.nLampBoardTotal;m++)
                {
                    myFlow.tellUpgrade(getBoardAddr(m_nBoardTypeIndex,1),m,0);
                }
            }
            else if(m_nBoardTypeIndex == board_control_zd)
            {
                for (int m =0;m<struCnfg.nCtrlBoardTotal;m++)
                {
                    myFlow.tellUpgrade(getBoardAddr(m_nBoardTypeIndex,1),m,0);
                }
            }
            else if(m_nBoardTypeIndex == board_control_dl)
            {
                for (int m =0;m<struCnfg.nCtrlBoardTotal;m++)
                {
                    myFlow.tellUpgrade(getBoardAddr(m_nBoardTypeIndex,1),m,0);
                }
            }
        }else{
            if (m_nChuteIndex == 0)
            {
                if(m_nBoardFactoryOk[m_view][m_nChuteIndex])
                    myFlow.tellUpgrade(m_boardAddr,m_nChuteIndex,m_nBoardTypeIndex<<4|m_nMirrorIndex);
            }
            else{
                if(m_nBoardFactoryOk[m_view][m_nChuteIndex-1])
                    myFlow.tellUpgrade(m_boardAddr,m_nChuteIndex,m_nBoardTypeIndex<<4|m_nMirrorIndex);
            }
        }
        id = timer_send_file ;
        startTimer(2500);
        return;
    }else if(id == timer_send_file){

        for(int i=0;i<3;i++){
            if (myFlow.isNetWorkOk())
                break;
        }
        getUpdateFileInfo();

        if (fileName.contains(".hex") || fileName.contains(".bit"))
        {
            QFile file(fileName);
            int count = 0;
            long fileSize = file.size();
            if (fileSize % 3)
            {
                fileSize= fileSize/3+1;
            }
            else
                fileSize = fileSize/3;

            fileSize += (16-fileSize%16);
            count = fileSize;
            fileSize = m_needFileSize;

            qDebug()<<"file size:"<<fileSize;

            char beginData[8] = { 0xB9,0x53,0xB9,0x53,fileSize >> 24,(fileSize >> 16) % 256 ,(fileSize >> 8) % 65536, fileSize % 256 };
            myFlow.myNetWork->udpRestart();

            if(myFlow.myNetWork->udpSendTo(beginData,8) != 8)
            {
                perror("update: udp send error");
                appendUpdateResult(g_myLan().ids_sendFileFailed);
                return;
            }

            uint crcRet=0;
            appendUpdateResult(g_myLan().ids_sendData);
            int time = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000;

            if(!file.open(QFile::ReadOnly | QFile::Text))
            {
                qDebug() << " Could not open the file for reading";
                appendUpdateResult(g_myLan().ids_sendFileFailed);
                return;
            }

            int totalsize =0;
            int bufSize = 1024 ;
            char *sendData = (char*)malloc(bufSize);
            QTextStream in(&file);
            while(!in.atEnd())
            {
                QString myText= "";
                for(int i=0;i<1024;i++){
                    QString str;
                    str = in.readLine();
                    myText.append(str.trimmed());
                }
                QString str(myText);

                QByteArray bytes = str.toLatin1();   //Qstring 转 QByteArray

                //字符串转成16进制数组
                if(!StringToHexGroup(sendData,bytes.data(),bytes.length())){
                    qDebug("StringToHexGroup error\n");
                    free(sendData);
                    sendData = NULL;
                    file.close();
                    appendUpdateResult(g_myLan().ids_sendFileFailed);
                    return;
                }
                //补全16字节下发
                int mis = (bytes.length()/2)%16;
                if(mis){
                    mis = 16 -mis;
                    qDebug()<<"str_length"<<str.length()<<"bytes_length"<<bytes.length()/2;
                    for(int i = 0;i<mis;i++){
                        sendData[bytes.length()/2+i] = 0xFF;
                    }
                    bytes.resize(bytes.length()+2*mis);
                }
                totalsize += bytes.length()/2 ;

                if (myFlow.myNetWork->udpSendTo(sendData, bytes.length()/2)!=bytes.length()/2){
                    qDebug()<<"\t send data error";

                    free(sendData);
                    sendData = NULL;
                    file.close();
                    appendUpdateResult(g_myLan().ids_sendFileFailed);
                    return;
                }
                mySerial.crcFpga2(sendData,bytes.length()/2,crcRet);

                myFlow.msleep(1);
            }
            while(totalsize <fileSize)
            {
                int sendSize = bufSize;
                if(fileSize - totalsize < bufSize){
                    int size = fileSize - totalsize;
                    int mis = size%16;
                    sendSize = mis+size;
                }
                for(int i = 0;i<sendSize;i++){
                    sendData[i] = 0xFF;
                }
                totalsize += sendSize;

                if (myFlow.myNetWork->udpSendTo(sendData,sendSize)!=sendSize){
                    qDebug()<<"\t send data error";

                    free(sendData);
                    sendData = NULL;
                    file.close();
                    appendUpdateResult(g_myLan().ids_sendFileFailed);
                    return;
                }
                mySerial.crcFpga2(sendData,sendSize,crcRet);

                myFlow.msleep(1);
            }

            qDebug()<<"total size :"<<totalsize;
            myFlow.msleep(1000);
            time = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000 - time;
            qDebug()<<"\t send data end cost "<<time<<"s";
            char crcData[2] = { (crcRet & 0xFF00) / 256 ,crcRet & 0xFF };
            printf("--%d,%d\n",crcData[0],crcData[1]);
            if (myFlow.myNetWork->udpSendTo(crcData, 2)!=2)
            {
                free(sendData);
                sendData = NULL;
                file.close();
                appendUpdateResult(g_myLan().ids_sendFileFailed);
                return;
            }
            free(sendData);
            sendData = NULL;
            file.close();
        }
        else if(fileName.contains(".bin") && fileName.contains("SIZE")){
            FILE *f;
            if((f=fopen(fileName.toUtf8().data(), "rb")) != NULL){
                fseek(f, 0, SEEK_END);
                long fileSize = ftell(f);
                int tmpSize = m_needFileSize;
                int bufSize = 1024 ;
                int offset =bufSize- fileSize%bufSize;
                int mis = tmpSize-fileSize;

                //fileSize = 8387584;
                fileSize = tmpSize;

                char beginData[8] = { 0xB9,0x53,0xB9,0x53,0,0,0,0};
                beginData[4] = fileSize>>24;
                beginData[5] = (fileSize>>16)%256;
                beginData[6] = (fileSize >> 8) % 65536;
                beginData[7] = fileSize % 256;
                myFlow.myNetWork->udpRestart();

                if(myFlow.myNetWork->udpSendTo(beginData,8) != 8)
                {
                    perror("update: udp send error");
                    fclose(f);
                    appendUpdateResult(g_myLan().ids_sendFileFailed);
                    struGsh.bIsUpgrading = false;
                    return;
                }

                fseek(f, 0, SEEK_SET);
                char *sendData = new char[bufSize];
                int count = 0;
                uint crcRet=0;

                appendUpdateResult(g_myLan().ids_sendData);
                int time = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000;
                while ((count = fread(sendData, 1, bufSize, f))>0)
                {
                    myFlow.msleep(2);
                    if(count < bufSize){
                        for(int i = 0; i< offset; i++)
                        {
                            if(count == bufSize) break;
                            sendData[count++] = 0xFF;
                        }
                    }
                    if (myFlow.myNetWork->udpSendTo(sendData, count)!=count)
                    {
                        qDebug()<<"\t send data error";
                        fclose(f);
                        delete[] sendData;
                        sendData = NULL;
                        appendUpdateResult(g_myLan().ids_sendFileFailed);
                        struGsh.bIsUpgrading = false;
                        return;
                    }
                    mySerial.crcFpga2(sendData,count,crcRet);
                }
                mis = mis-offset;//发送上个while循环没补齐的数据
                while(mis > 0){
                    myFlow.msleep(2);
                    memset(sendData,0xff,bufSize);
                    if((mis-offset> bufSize)){
                        count = bufSize;
                    }else{
                        count = mis;
                    }

                    if (myFlow.myNetWork->udpSendTo(sendData, count)!=count)
                    {
                        qDebug()<<"\t send data error";
                        fclose(f);
                        delete[] sendData;
                        sendData = NULL;
                        appendUpdateResult(g_myLan().ids_sendFileFailed);
                        struGsh.bIsUpgrading = false;
                        return;
                    }
                    mySerial.crcFpga2(sendData,count,crcRet);

                    mis-=count;
                }
                myFlow.msleep(1000);

                time = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000 - time;
                qDebug()<<"\t send data end cost "<<time<<"s";
                char crcData[2] = {0,0};
                crcData[0] = (crcRet & 0xFF00) / 256;
                crcData[1] = crcRet & 0xFF;
                printf("--%d,%d\n",crcData[0],crcData[1]);
                if (myFlow.myNetWork->udpSendTo(crcData, 2)!=2)
                {
                    fclose(f);
                    delete[] sendData;
                    sendData = NULL;
                    appendUpdateResult(g_myLan().ids_sendFileFailed);
                    struGsh.bIsUpgrading = false;
                    return;
                }
                fclose(f);
                delete[] sendData;
                sendData = NULL;
            }
        }
        else{
            FILE *f;
            if((f=fopen(fileName.toUtf8().data(), "rb")) != NULL){
                fseek(f, 0, SEEK_END);
                long fileSize = ftell(f);

                int mis = 16-fileSize%16;
                if(fileName.contains(".sfc")) //22/5/11 16字节对齐，本文件需补８个字节
                {
                    fileSize += mis;
                }
                else
                {
                    if (fileSize % 2)
                    {
                        fileSize++;
                    }
                }
                char beginData[8] = { 0xB9,0x53,0xB9,0x53,fileSize >> 24,(fileSize >> 16) % 256 ,(fileSize >> 8) % 65536, fileSize % 256 };
                myFlow.myNetWork->udpRestart();

                if(myFlow.myNetWork->udpSendTo(beginData,8) != 8)
                {
                    perror("update: udp send error");
                    fclose(f);
                    appendUpdateResult(g_myLan().ids_sendFileFailed);
                    return;
                }

                fseek(f, 0, SEEK_SET);
                int bufSize = 1024 ;
                char *sendData =(char*)malloc(bufSize);
                int count = 0;
                int cnt = 0;
                uint crcRet=0;

                appendUpdateResult(g_myLan().ids_sendData);
                int time = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000;
                while ((count = fread(sendData, 1, bufSize, f))>0)
                {
                    myFlow.msleep(2);
                    if(count < bufSize)
                    {
                        if(fileName.contains(".sfc"))
                        {
                            for(int i = 0; i< mis; i++)
                            {
                                if(count == bufSize) break;
                                sendData[count++] = 0xFF;
                            }
                        }
                        else if(count % 2)
                        {
                            qDebug()<<"count++"<<count;
                            sendData[count] = 0xFF;
                            count++;
                        }
                    }
                    if (myFlow.myNetWork->udpSendTo(sendData, count)!=count)
                    {
                        qDebug()<<"\t send data error";
                        fclose(f);
                        free(sendData);
                        sendData = NULL;
                        appendUpdateResult(g_myLan().ids_sendFileFailed);
                        return;
                    }
                    mySerial.crcFpga2(sendData,count,crcRet);
                }
                myFlow.msleep(1000);

                time = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000 - time;
                qDebug()<<"\t send data end cost "<<time<<"s";
                char crcData[2] = { (crcRet & 0xFF00) / 256 ,crcRet & 0xFF };
                printf("--%d,%d\n",crcData[0],crcData[1]);
                if (myFlow.myNetWork->udpSendTo(crcData, 2)!=2)
                {
                    fclose(f);
                    free(sendData);
                    sendData = NULL;
                    appendUpdateResult(g_myLan().ids_sendFileFailed);
                    return;
                }
                fclose(f);
                free(sendData);
                sendData = NULL;
            }
        }

        appendUpdateResult(g_myLan().ids_upgrading);

        id = timer_wait_30secs ;
         startTimer(m_needDelay);
        return;
    }
    else if (id == timer_wait_30secs)
    {
        myFlow.resetMachineConfigRG();
        myFlow.resetSensorConfigRG();
        myFlow.msleep(3000);

        if (m_boardAddr == BOARD_TYPE_INTERFACE)
        {
            id = timer_ask_interface_version_after_upgrade ;
            startTimer(20);
        }
        else
        {
            id = timer_ask_other_version_after_upgrade ;
            startTimer(20);
        }
        return;
    }
    else if (id ==  timer_ask_interface_version_after_upgrade)
    {
        for (int i = 0; i < 3; i++)
        {
            myFlow.readVersion(BOARD_TYPE_INTERFACE,0);
            myFlow.msleep(500);

            if(boardUpdateSuccess(m_nBoardTypeIndex,m_view+1,m_nChuteIndex,0))
            {
                appendUpdateResult(g_myLan().ids_interfaceBoard+g_myLan().ids_upgradeSucceeded);
                if(isBackBurn)
                {
                    appendUpdateResult(g_myLan().msg_reset_machine_info);
                }
                return;
            }
        }
        appendUpdateResult(g_myLan().ids_interfaceBoard+g_myLan().ids_upgradeFailed);
        return;
    }
    else if (id == timer_ask_other_version_after_upgrade)
    {
        memset(m_nBoardUpdateOk,0,sizeof(m_nBoardUpdateOk));

        int i,j,k,m;
        if(m_nChainIndex == 0){ //all chain
            if(m_nBoardTypeIndex == board_camera || m_nBoardTypeIndex == board_ipc_camera){
                for(j = 0;j < MAX_VIEW;j++)
                {
                    if (!struCnfg.struLayerInfo[0].nViewIsUsed[j]) continue;
                    if(m_nChuteIndex == 0) //all chute
                    {
                        for (m =0;m<struCnfg.struLayerInfo[0].nViewUnitTotal[j];m++)
                        {
                            if (m_nBoardFactoryOk[j][m])
                            {
                                for(k = 0; k < 20; k++)
                                {
                                    myFlow.readVersion(getBoardAddr(m_nBoardTypeIndex,j+1),m+1);

                                    myFlow.msleep(500);
                                    qDebug()<<struGsh.struVer.nUnitRes[0][j][m] << struGsh.struVer.sUnit[0][j][m][2];
                                    if(boardUpdateSuccess(m_nBoardTypeIndex,j+1,m,0))
                                    {
                                        appendUpdateResult(viewList.at(j)+QString::number(m+1)+viewMap[m_nBoardTypeIndex]+g_myLan().ids_upgradeSucceeded);
                                        m_nBoardUpdateOk[j][m] = 1;
                                        break;
                                    }
                                }
                                if(k == 20)
                                {
                                    appendUpdateResult(viewList.at(j)+QString::number(m+1)+viewMap[m_nBoardTypeIndex]+g_myLan().ids_upgradeFailed,false);
                                    interfaceNeedChgToUser = true;
                                }
                            }
                        }
                    }
                    else// single chute
                    {
                        if (m_nBoardFactoryOk[j][m_nChuteIndex-1])
                        {
                            for(k = 0; k < 20; k++)
                            {
                                myFlow.readVersion(getBoardAddr(m_nBoardTypeIndex,j+1),m_nChuteIndex);
                                myFlow.msleep(500);
                                if(boardUpdateSuccess(m_nBoardTypeIndex,j+1,m_nChuteIndex-1,0))
                                {
                                    appendUpdateResult(viewList.at(j)+QString::number(m_nChuteIndex)+viewMap[m_nBoardTypeIndex]+g_myLan().ids_upgradeSucceeded);
                                    m_nBoardUpdateOk[j][m_nChuteIndex-1] = 1;
                                    break;
                                }
                            }
                            if(k == 20)
                            {
                                appendUpdateResult(viewList.at(j)+QString::number(m_nChuteIndex)+viewMap[m_nBoardTypeIndex]+g_myLan().ids_upgradeFailed,false);
                                interfaceNeedChgToUser = true;
                            }
                        }
                    }
                }
            }//end CAM
            else if (m_nBoardTypeIndex == board_valve)
            {
                for(j = 0; j < struCnfg.struLayerInfo[0].nVavleChainTotal; j++)
                {
                    for (m =0;m<struCnfg.struLayerInfo[0].nVavleBoardTotal[j];m++)
                    {
                        for(k = 0; k < 20; k++)
                        {
                            myFlow.readVersion(BOARD_VAVLE_LAYER_1,m+1);
                            myFlow.msleep(500);
                            //轮询每个阀板的版本信息
                            if(boardUpdateSuccess(board_valve,j+1,m,0))
                            {
                                appendUpdateResult(g_myLan().ids_valveBoard+QString::number(m+1)+g_myLan().ids_upgradeSucceeded);
//                                m_nBoardUpdateOk[PCB_valve][m] = 1;
                                break;
                            }
                        }
                        if (k == 20)
                        {
                            appendUpdateResult(g_myLan().ids_valveBoard+QString::number(m+1)+g_myLan().ids_upgradeFailed,false);
                        }
                    }
                }
            }
            else if (m_nBoardTypeIndex == board_control)
            {
                for(m = 0; m < struCnfg.nCtrlBoardTotal; m++)
                {
                    for(k = 0; k < 20; k++)
                    {
                        myFlow.readVersion(BOARD_TYPE_CONTROL,m+1);
                        myFlow.msleep(500);
                        //轮询每个阀板的版本信息
                        if(boardUpdateSuccess(board_control,0,m,0))
                        {
                            appendUpdateResult(g_myLan().ids_controlBoard+QString::number(m+1)+g_myLan().ids_upgradeSucceeded);
//                            m_nBoardUpdateOk[PCB_control][m] = 1;
                            break;
                        }
                    }
                    if (k == 20)
                    {
                        appendUpdateResult(g_myLan().ids_controlBoard+QString::number(m+1)+g_myLan().ids_upgradeFailed,false);
                    }
                }
            }
            else if (m_nBoardTypeIndex == board_lamp)
            {
                for(m = 0; m < struCnfg.stuLayerLampCfg.nLampBoardTotal; m++)
                {
                    for(k = 0; k < 20; k++)
                    {
                        myFlow.readVersion(BOARD_TYPE_LAMP,m+1);
                        myFlow.msleep(500);
                        //轮询每个阀板的版本信息
                        if(boardUpdateSuccess(board_lamp,0,m,0))
                        {
                            appendUpdateResult(g_myLan().ids_ccsBoard+QString::number(m+1)+g_myLan().ids_upgradeSucceeded);
//                            m_nBoardUpdateOk[PCB_lamp][m] = 1;
                            break;
                        }
                    }
                    if (k == 20)
                    {
                        appendUpdateResult(g_myLan().ids_ccsBoard+QString::number(m+1)+g_myLan().ids_upgradeFailed,false);
                    }
                }
            }

        }//end all chain
        else{ //single chain
            if(m_nBoardTypeIndex == board_camera || m_nBoardTypeIndex == board_ipc_camera){

                if(m_nChuteIndex == 0) //all chute
                {
                    for (m =0;m<struCnfg.struLayerInfo[0].nViewUnitTotal[m_view];m++)
                    {
                        if (m_nBoardFactoryOk[m_view][m])
                        {
                            {
                                for(k = 0; k < 20; k++)
                                {
                                    myFlow.readVersion(m_boardAddr,m+1);
                                    myFlow.msleep(500);
                                    //轮询每个通道板的版本信息
                                    if(boardUpdateSuccess(m_nBoardTypeIndex,m_view+1,m,0))
                                    {
                                        appendUpdateResult(viewList.at(m_view)+QString::number(m+1)+viewMap[m_nBoardTypeIndex]+g_myLan().ids_upgradeSucceeded);
                                        m_nBoardUpdateOk[m_view][m] = 1;
                                        break;
                                    }
                                }
                                if(k == 20)
                                {
                                    appendUpdateResult(viewList.at(m_view)+QString::number(m+1)+viewMap[m_nBoardTypeIndex]+g_myLan().ids_upgradeFailed,false);
                                    interfaceNeedChgToUser = true;
                                }
                            }
                        }
                    }
                }
                else// single chute
                {
                    if (m_nBoardFactoryOk[m_view][m_nChuteIndex-1])
                    {
                        for(k = 0; k < 20; k++)
                        {
                            myFlow.readVersion(m_boardAddr,m_nChuteIndex);
                            myFlow.msleep(500);
                            //轮询每个通道板的版本信息
                            if(boardUpdateSuccess(m_nBoardTypeIndex,m_view+1,m_nChuteIndex-1,0))
                            {
                                appendUpdateResult(viewList.at(m_view)+QString::number(m_nChuteIndex)+viewMap[m_nBoardTypeIndex]+g_myLan().ids_upgradeSucceeded);
                                m_nBoardUpdateOk[m_view][m_nChuteIndex-1] = 1;
                                break;
                            }
                        }
                        if(k == 20)
                        {
                            appendUpdateResult(viewList.at(m_view)+QString::number(m_nChuteIndex)+g_myLan().ids_upgradeFailed,false);
                            interfaceNeedChgToUser = true;
                        }
//                        if ( m_nBoardUpdateOk[m_view][m_nChuteIndex-1] != 1){
//                            appendUpdateResult(viewList.at(m_nChainIndex-1)+QString::number(m_nChuteIndex)+g_myLan().ids_upgradeFailed);
//                            appendUpdateResult(g_myLan().ids_upgradeFailed);
//                            return;
//                        }
                    }
                }
            }
        }

        if(m_nChainIndex == 0){//all chain
            if(m_nBoardTypeIndex == board_camera || m_nBoardTypeIndex == board_ipc_camera){
                for(j = 0;j < MAX_VIEW;j++)
                {
                    if (!struCnfg.struLayerInfo[0].nViewIsUsed[j]) continue;
                    if(m_nChuteIndex == 0) //all chute
                    {
                        for (m = 0;m<struCnfg.struLayerInfo[0].nViewUnitTotal[j];m++)
                        {
                            if (m_nBoardFactoryOk[j][m] && !m_nBoardUpdateOk[j][m]){
//                                appendUpdateResult(g_myLan().ids_upgradeFailed,false);
                                interfaceNeedChgToUser = true;
                                break;
                            }
                        }
                        if (interfaceNeedChgToUser) break;
                    }
                    else// single chute
                    {
                        for(k = 0; k < 20; k++)
                        {
                            if(m_nBoardFactoryOk[j][m_nChuteIndex-1] && !m_nBoardUpdateOk[j][m_nChuteIndex-1]){
//                                appendUpdateResult(g_myLan().ids_upgradeFailed,false);
                                interfaceNeedChgToUser = true;
                                break;
                            }
                        }
                        if (interfaceNeedChgToUser) break;
                    }
                }
            }//end CAM
        }
        else{ //single chain
            if(m_nBoardTypeIndex == board_camera || m_nBoardTypeIndex == board_ipc_camera){

                if(m_nChuteIndex == 0) //all chute
                {
                    for (m =0;m<struCnfg.struLayerInfo[0].nViewUnitTotal[m_view];m++)
                    {
                        if(m_nBoardFactoryOk[m_view][m] && !m_nBoardUpdateOk[m_view][m]){
//                            appendUpdateResult(g_myLan().ids_upgradeFailed,false);
                            interfaceNeedChgToUser = true;
                        }
                    }
                }
                else// single chute
                {
                    if(m_nBoardFactoryOk[m_view][m_nChuteIndex-1] && !m_nBoardUpdateOk[m_view][m_nChuteIndex-1]){
//                        appendUpdateResult(g_myLan().ids_upgradeFailed,false);
                        interfaceNeedChgToUser = true;
                    }
                }
            }
        }
        if(interfaceNeedChgToUser && m_needDelay < 150*1000) //  重新检测升级结果
        {
            appendUpdateResult(g_myLan().recheck,true);
            id = timer_wait_30secs ;
            m_needDelay += 30*1000;
            startTimer(30*1000);
            return;
        }
        myFlow.switchUserMirror(USER_MODE,BOARD_TYPE_INTERFACE,0,0);
        myFlow.msleep(3000); // Factory chg to User need more time
        id = timer_interface_wait_toUser ;
        startTimer(time_switchMode);
        return;
    }
    else if (id == timer_interface_wait_toUser)
    {
//        myFlow.resetFpgaRunModeRG();
        myFlow.resetMachineConfigRG();
        myFlow.resetSensorConfigRG();
        myFlow.msleep(3000);
        id = timer_ask_interface_version_after_others_upgrade ;
        startTimer(20);
        return;
    }

    else if (id == timer_ask_interface_version_after_others_upgrade)
    {
        for (int i = 0; i < 3; i++)
        {
            myFlow.readVersion(BOARD_TYPE_INTERFACE,0);
            myFlow.msleep(500);
            if (struGsh.struVer.sInterface[2] == 1)
            {
                if (interfaceNeedChgToUser){
                    interfaceNeedChgToUser = false;
                    appendUpdateResult(g_myLan().ids_upgradeFailed,false);
                    return;
                }
                appendUpdateResult(g_myLan().ids_restoreInterfaceSucceeded);
                return;
            }
        }
        appendUpdateResult(g_myLan().ids_restoreInterfaceFailed+","+g_myLan().ids_upgradeFailed,false);
        return;
    }
}

void UpdateFpgaWidget::updateElecBoardCombobox()
{
    QStringList list;
    list<<g_myLan().ids_interfaceBoard<<g_myLan().ids_camera<<"IPC"+g_myLan().ids_camera<<g_myLan().ids_ipc
         <<g_myLan().ids_valveBoard<<g_myLan().ids_controlBoard<<g_myLan().ids_ccsBoard<<g_myLan().ids_accelerationBoard<<g_myLan().ids_halogenBoard;
    ui->comboBox_board->clear();
    ui->comboBox_board->addItems(list);
    ui->comboBox_board->setCurrentIndex(m_nBoardTypeIndex);
}

void UpdateFpgaWidget::updateChuteCombobox()
{
    QStringList list;
    list<<g_myLan().ids_allChannel;

//    if ( (2 == m_view || 3 == m_view|| 6 == m_view || 7 == m_view) && (struCnfg.bCrossMode2 == 1) )
//    {
//        for(int i = 1;i< struCnfg.nChuteTotal+2;i++){
//            list<<QString::number(i);
//        }
//    }
//    else if ( (1 == m_view || 0 == m_view|| 4 == m_view || 5 == m_view) && (struCnfg.bCrossMode1 == 1) )
//    {
//        for(int i = 1;i< struCnfg.nChuteTotal+2;i++){
//            list<<QString::number(i);
//        }
//    }
//    else
    {
        for(int i = 1;i< struCnfg.nChuteTotal+1;i++){
            list<<QString::number(i);
        }
    }

    ui->comboBox_chute->clear();
    ui->comboBox_chute->addItems(list);
    ui->comboBox_chute->setCurrentIndex(m_nChuteIndex);
}

void UpdateFpgaWidget::updateModeCombobox()
{
    QStringList list;
    list<<g_myLan().mode_1<<g_myLan().mode_2<<g_myLan().mode_3;
    ui->comboBox_mode->clear();
    ui->comboBox_mode->addItems(list);
    ui->comboBox_mode->setCurrentIndex(m_nMirrorIndex);
}

void UpdateFpgaWidget::updateChainCombobox()
{
    chainList.clear();
    QStringList list;
    list<<g_myLan().front_rear_view;
    for(int i = 0;i< struCnfg.struLayerInfo[0].nViewChainTotal;i++){
        if(struCnfg.struLayerInfo[0].nViewIsUsed[i])
        {
            chainList<<QString::number(i);
            if (struCnfg.nMachineType == MACHINE_LD2_ZL_GP){
                switch (i) {
                case 0:
                    list<<g_myLan().lower_layer+g_myLan().ids_fm;
                    break;
                case 1:
                    list<<g_myLan().lower_layer+g_myLan().ids_bm;
                    break;
                case 2:
                    list<<g_myLan().lower_layer+g_myLan().ids_fs;
                    break;
                case 3:
                    list<<g_myLan().lower_layer+g_myLan().ids_bs;
                    break;
                case 4:
                    list<<g_myLan().upper_layer+g_myLan().ids_fm;
                    break;
                case 5:
                    list<<g_myLan().upper_layer+g_myLan().ids_bm;
                    break;
                case 6:
                    list<<g_myLan().upper_layer+g_myLan().ids_fs;
                    break;
                case 7:
                    list<<g_myLan().upper_layer+g_myLan().ids_bs;
                    break;
                default:
                    break;
                }
            }
            else{
                switch (i) {
                case 0:
                    list<<g_myLan().ids_fm;
                    break;
                case 1:
                    list<<g_myLan().ids_bm;
                    break;
                case 2:
                    list<<g_myLan().ids_fs;
                    break;
                case 3:
                    list<<g_myLan().ids_bs;
                    break;
                default:
                    break;
                }
            }
        }
    }
    ui->comboBox_chain->clear();
    ui->comboBox_chain->addItems(list);
    ui->comboBox_chain->setCurrentIndex(m_nChainIndex);

    if (m_nChainIndex == 0)
    {
        ui->comboBox_chute->setCurrentIndex(0);
        ui->comboBox_chute->setEnabled(false);
    }
}

bool UpdateFpgaWidget::boardUpdateSuccess(int boardIndex, int chainIndex, int chuteIndex, uchar versionToUpdate)
{
    switch (boardIndex) {
    case board_interface:
        if(struGsh.struVer.nInterfaceRes == 1 && struGsh.struVer.sInterface[2] == USER_MODE &&
                struGsh.struVer.sInterface[0] == m_versionToUpgrade[0] &&
                struGsh.struVer.sInterface[1] == m_versionToUpgrade[1])
            return true;
        break;
    case board_camera:
        if(struGsh.struVer.nUnitRes[0][chainIndex-1][chuteIndex] == 1 && struGsh.struVer.sUnit[0][chainIndex-1][chuteIndex][2] == (m_nMirrorIndex+1) &&
                struGsh.struVer.sUnit[0][chainIndex-1][chuteIndex][0] == m_versionToUpgrade[0] &&
                struGsh.struVer.sUnit[0][chainIndex-1][chuteIndex][1] == m_versionToUpgrade[1])
            return true;
        break;
    case board_ipc_camera:
        if(struGsh.struVer.nUnitRes[0][chainIndex-1][chuteIndex] == 1 && struGsh.struVer.sUnitIpc[0][chainIndex-1][chuteIndex][2] == USER_MODE &&
                struGsh.struVer.sUnitIpc[0][chainIndex-1][chuteIndex][0] == m_versionToUpgrade[0] &&
                struGsh.struVer.sUnitIpc[0][chainIndex-1][chuteIndex][1] == m_versionToUpgrade[1])
            return true;
        break;
    case board_valve:
        if(struGsh.struVer.nVavleRes[0][chainIndex-1][chuteIndex] == 1 && struGsh.struVer.sVavle[0][chainIndex-1][chuteIndex][2] == USER_MODE &&
                struGsh.struVer.sVavle[0][chainIndex-1][chuteIndex][0] == m_versionToUpgrade[0] &&
                struGsh.struVer.sVavle[0][chainIndex-1][chuteIndex][1] == m_versionToUpgrade[1])
            return true;
        break;
    case board_control:
        if(struGsh.struVer.nCtrlRes[chuteIndex] == 1 && struGsh.struVer.sCtrlBoard[chuteIndex][2] == USER_MODE &&
                struGsh.struVer.sCtrlBoard[chuteIndex][0] == m_versionToUpgrade[0] &&
                struGsh.struVer.sCtrlBoard[chuteIndex][1] == m_versionToUpgrade[1])
            return true;
        break;
    case board_lamp:
        if(struGsh.struVer.nLampRes[chuteIndex] == 1 && struGsh.struVer.sLampBoard[chuteIndex][2] == USER_MODE &&
                struGsh.struVer.sLampBoard[chuteIndex][0] == m_versionToUpgrade[0] &&
                struGsh.struVer.sLampBoard[chuteIndex][1] == m_versionToUpgrade[1])
            return true;
        break;
    case board_control_zd:
        if(struGsh.struVer.nAccelerationRes[chuteIndex] == 1 && struGsh.struVer.sAccelerationBoard[chuteIndex][2] == USER_MODE &&
            struGsh.struVer.sAccelerationBoard[chuteIndex][0] == m_versionToUpgrade[0] &&
            struGsh.struVer.sAccelerationBoard[chuteIndex][1] == m_versionToUpgrade[1])
            return true;
        break;
    case board_control_dl:
        if(struGsh.struVer.nHalogenRes[chuteIndex] == 1 && struGsh.struVer.sHalogenBoard[chuteIndex][2] == USER_MODE &&
            struGsh.struVer.sHalogenBoard[chuteIndex][0] == m_versionToUpgrade[0] &&
            struGsh.struVer.sHalogenBoard[chuteIndex][1] == m_versionToUpgrade[1])
            return true;
        break;
    default:
        return false;
        break;
    }
    return false;
}

int UpdateFpgaWidget::getBoardAddr(int boardIndex, int chainIndex)
{
    m_view = 0;
    switch(boardIndex){
    case board_interface:
        m_boardAddr = BOARD_TYPE_INTERFACE;
        break;
    case board_camera:
    case board_ipc_camera:
        if(chainIndex == 0)
            return struCnfg.struLayerInfo[struGsh.nLayer].nViewBoardType[chainIndex];
        else{
            for(int i=0;i<chainList.size();i++)
            {
                if((chainIndex -1)== chainList.at(i).toInt())
                {
                    m_view = chainIndex - 1;
                }
            }
            qDebug()<<"m_view = "<<m_view;
            return struCnfg.struLayerInfo[struGsh.nLayer].nViewBoardType[m_view];
       }
        break;
    case board_valve:
        return struCnfg.struLayerInfo[struGsh.nLayer].nVavleBoardType[0];
        break;
    case board_control:
        return struCnfg.nCtrlBoardType;
        break;
    case board_lamp:
        return struCnfg.nLampBoardType;
        break;
    case board_control_zd:
        return struCnfg.nCtrlBoardType;
        break;
    case board_control_dl:
        return struCnfg.nCtrlBoardType;
        break;
    }
    return m_boardAddr;
}

bool UpdateFpgaWidget::boardSwitchFactory(int boardIndex,int chainIndex, int chuteIndex)
{
    switch (boardIndex) {
    case board_interface:
        if(struGsh.struVer.nInterfaceRes == 1 && struGsh.struVer.sInterface[2] == FACTORY_MODE)
            return true;
        break;
    case board_camera:
        if(struGsh.struVer.nUnitRes[0][chainIndex-1][chuteIndex] == 1 && struGsh.struVer.sUnit[0][chainIndex-1][chuteIndex][0] != 0 && struGsh.struVer.sUnit[0][chainIndex-1][chuteIndex][2] == FACTORY_MODE)
            return true;
        break;
    case board_ipc_camera:
        if(struGsh.struVer.nUnitRes[0][chainIndex-1][chuteIndex] == 1 && struGsh.struVer.sUnitIpc[0][chainIndex-1][chuteIndex][0] != 0 && struGsh.struVer.sUnitIpc[0][chainIndex-1][chuteIndex][2] == FACTORY_MODE)
            return true;
        break;
    case board_valve:
        if(struGsh.struVer.nVavleRes[0][chainIndex-1][chuteIndex] == 1 && struGsh.struVer.sVavle[0][chainIndex-1][chuteIndex][0] != 0 && struGsh.struVer.sVavle[0][chainIndex-1][chuteIndex][2] == FACTORY_MODE)
            return true;
        break;
    case board_control:
        if(struGsh.struVer.nCtrlRes[chuteIndex] == 1 && struGsh.struVer.sCtrlBoard[chuteIndex][0] != 0 && struGsh.struVer.sCtrlBoard[chuteIndex][2] == FACTORY_MODE)
            return true;
        break;
    case board_lamp:
        if(struGsh.struVer.nLampRes[chuteIndex] == 1 && struGsh.struVer.sLampBoard[chuteIndex][0] != 0 && struGsh.struVer.sLampBoard[chuteIndex][2] == FACTORY_MODE)
            return true;
        break;
    case board_control_zd:
        if(struGsh.struVer.nAccelerationRes[chuteIndex] == 1 && struGsh.struVer.sAccelerationBoard[chuteIndex][0] != 0 && struGsh.struVer.sAccelerationBoard[chuteIndex][2] == FACTORY_MODE)
            return true;
        break;
    case board_control_dl:
        if(struGsh.struVer.nHalogenRes[chuteIndex] == 1 && struGsh.struVer.sAccelerationBoard[chuteIndex][0] != 0 && struGsh.struVer.sAccelerationBoard[chuteIndex][2] == FACTORY_MODE)
            return true;
        break;
    default:
        return false;
        break;
    }
    return false;
}

void UpdateFpgaWidget::on_comboBox_board_activated(int index)
{
//    if(index == 0 ){
//        m_nChuteIndex = 0;
//        ui->comboBox_chute->setCurrentIndex(0);
//        ui->comboBox_chute->setEnabled(false);
//    }else{
//        ui->comboBox_chute->setEnabled(true);
//    }
//#if 1
//    if(index == 1|| index == 2 || index == 3){
//        m_nChuteIndex = 0;
//        if (m_nChainIndex == 0){
//            ui->comboBox_chute->setCurrentIndex(0);
//            ui->comboBox_chute->setEnabled(false);
//        }else
//            ui->comboBox_chute->setEnabled(true);
//        ui->comboBox_mode->setCurrentIndex(0);
//        ui->comboBox_mode->setEnabled(true);
//    }else{
//        ui->comboBox_mode->setEnabled(false);
//    }
//#endif
//    if(index == 4|| index == 5 || index == 6)
//    {
//        m_nChuteIndex = 0;
//        m_nChainIndex = 0;
//        ui->comboBox_chain->setCurrentIndex(0);
//        ui->comboBox_chain->setEnabled(false);
//        ui->comboBox_chute->setCurrentIndex(0);
//        ui->comboBox_chute->setEnabled(false);
//        ui->comboBox_mode->setCurrentIndex(0);
//        ui->comboBox_mode->setEnabled(false);
//    }

    if(index == 0)
    {
        ui->BackBurnBtn->setEnabled(true);
    }
    else
    {
        ui->BackBurnBtn->setEnabled(false);
    }
#if 0
    if(index == 1|| index == 2 || index == 3){
        m_nChuteIndex = 0;
        if (m_nChainIndex == 0){
            ui->comboBox_chute->setCurrentIndex(0);
            ui->comboBox_chute->setEnabled(false);
        }else
            ui->comboBox_chute->setEnabled(true);
        ui->comboBox_mode->setCurrentIndex(0);
        ui->comboBox_mode->setEnabled(false);
    }else{
        ui->comboBox_mode->setEnabled(true);
    }
#endif
    m_nBoardTypeIndex = index;
    ui->comboBox_board->setCurrentIndex(m_nBoardTypeIndex);
    on_checkFileBtn_clicked();
}

void UpdateFpgaWidget::on_comboBox_chute_activated(int index)
{
    m_nChuteIndex = index;
}

void UpdateFpgaWidget::on_comboBox_chain_activated(int index)
{
    m_nChainIndex = index;
    if (m_nChainIndex == 0)
    {
        m_nChuteIndex = 0;
        ui->comboBox_chute->setCurrentIndex(0);
        ui->comboBox_chute->setEnabled(false);
    }
    else
    {
        ui->comboBox_chute->setEnabled(true);
    }
    on_comboBox_chute_activated(0);
    updateChuteCombobox();
}

void UpdateFpgaWidget::on_comboBox_mode_activated(int index)
{
    m_nMirrorIndex = index;
}

void UpdateFpgaWidget::on_checkFileBtn_clicked()
{

    qDebug()<<m_nBoardTypeIndex<<m_nChainIndex<<m_nChuteIndex<<m_nMirrorIndex;

    ui->startUpgradeBtn->setEnabled(true);
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

    QString folder ="";
    if(m_nBoardTypeIndex != board_arm)
        folder = "/bin";

#ifdef Q_OS_UNIX
    QDir myDir(path+folder);
#else
    QDir myDir(qApp->applicationDirPath()+folder);
#endif

    if (!myDir.exists()) {
        qWarning("Cannot find the /udisk directory");
        return;
    }
    ui->FileListWidget->clear();
    QStringList filters;
    switch(m_nBoardTypeIndex)
    {
    case board_interface:
        filters << "*INTERFACE*.bin"<<"*INTERFACE*.art"<<"*INTERFACE*.rpd"<<"*INTERFACE*[aA][pP][pP]*.hex"<<"*INTERFACE*.bit";
        break;
    case board_camera:
//        filters << "*CAM*.bin"<<"*CAM*.art"<<"*CAM*.rpd"<<"*CAM*Process*[aA][pP][pP]*.hex"<<"*CAM*.sfc";
        filters << "*CAM*.bin"<<"*CAM*.art"<<"*CAM*.rpd"<<"*CAM*[aA][pP][pP]*.hex"<<"*CAM*.sfc";
        break;
    case board_ipc_camera:
        filters << "*CAM*.bin"<<"*CAM*.art"<<"*CAM*.rpd"<<"*CAM*Driver*[aA][pP][pP]*.hex"<<"*CAM*.sfc";
        break;
    case board_arm:
        filters << "*.bin"<<"zksort"<<"resource.rcc"<<"lang"<<"fonts";
        break;
    case board_valve:
        filters << "*VD*.hex";
        break;
    case board_control:
        filters << "*CONTROL*.hex";
        break;
    case board_lamp:
        filters << "*LEDDRIVER*.hex";
        break;
    case board_control_zd:
        filters << "*ZDJC*.hex";
        break;
    case board_control_dl:
        filters << "*DLJC*.hex";
        break;
    default:
        filters << "*INTERFACE*.bin"<<"*INTERFACE*.art"<<"*INTERFACE*.rpd"<<"*INTERFACE*.hex"<<"*INTERFACE*.bit";
        break;
    }
    myDir.setNameFilters(filters);
    QFileInfoList list;
    list = myDir.entryInfoList(filters);

    int m_nFileNum = list.size();
    if (m_nFileNum < 1) {
        ui->startUpgradeBtn->setEnabled(false);
    } else {
        ui->startUpgradeBtn->setEnabled(true);
    }

    QString filename;
    float filesize;
    QDateTime filedate;
    QString str;

    filelist.clear();

    for(int i=0;i< list.size();i++)
    {
        filename = list.at(i).fileName();
        filesize = list.at(i).size();
        filedate = list.at(i).lastModified();
        if (filesize > 1024*1024)
        {
            filesize = filesize /1024 /1024;
            str= filename + "        "+filedate.toString("yyyy/MM/dd hh:mm:ss")+ "        "+QString::number(filesize,'f',1)+"M";
        }
        else
        {
            filesize = filesize/1024;
            str = filename + "        "+filedate.toString("yyyy/MM/dd hh:mm:ss")+ "        "+QString::number(filesize,'f',1)+"K";
        }

        filelist.append(str);
    }

    ui->FileListWidget->insertItems(0, filelist);
    ui->FileListWidget->setCurrentRow(0);
}

void UpdateFpgaWidget::updateAll()
{
    updateElecBoardCombobox();
    updateChainCombobox();
    updateChuteCombobox();
    updateModeCombobox();
}

void UpdateFpgaWidget::on_OneKeyUpdateBtn_clicked()
{
    g_MainManager().ShowWidget(SM_SELECT_UPDATE_FILE_WIDGET);
}


void UpdateFpgaWidget::on_BackBurnBtn_clicked()
{
    isBackBurn = 0;
    // 移到此处方便检查文件;
    checkUpdateFile();

    ui->FileListWidget->clear();
    ui->startUpgradeBtn->setEnabled(false);
    ui->BackBurnBtn->setEnabled(false);
    myInputMethod inputPanel;
    inputPanel.setType(KB_PD);
    inputPanel.setInputBtnText(g_myLan().ids_ok,g_myLan().ids_cancel,g_myLan().ids_delete,g_myLan().ids_space);
    if(inputPanel.exec() == QDialog::Accepted)
    {
        QString str = inputPanel.getText();
        if ( str == "ZK603011" )           
        {
            myMessageBox msgBox(MSG_QUES, g_myLan().cfm_perform);
            int ret = msgBox.exec();
            if (ret == QDialog::Accepted)
            {
                appendUpdateResult(g_myLan().ids_startUpgrading);
                QVector<QStringList> fileNameLst = getBackBurnFileName();
                QVector<QString> flatVector = flattenQVector(fileNameLst);

                if(flatVector.size() < 2)
                {
                    g_infoWidget().setLabelText(g_myLan().ids_notSelectFile);
                    g_infoWidget().showSecs();
                    return;
                }

                myFlow.switchUserMirror(0<<4 |FACTORY_MODE,BOARD_TYPE_INTERFACE,0,0);
                myFlow.msleep(6000);

                myFlow.resetMachineConfigRG();
                myFlow.msleep(3000);

                myFlow.readVersion(BOARD_TYPE_INTERFACE,0);
                myFlow.msleep(500);

                if(boardSwitchFactory(board_interface,0,0))
                {
                    appendUpdateResult(g_myLan().ids_interfaceBoard+g_myLan().ids_setFactorySucceeded);
                    for (int i = 0; i < flatVector.size(); ++i) {

                        appendUpdateResult(g_myLan().ids_tellToUpgrade);
                        myFlow.tellUpgrade(BOARD_TYPE_INTERFACE,0,0);
                        myFlow.msleep(2500);

                        // 测试网络
                        for(int j=0;j<3;j++){
                            if (myFlow.isNetWorkOk())
                                break;
                        }
                        sendUpdateFile(flatVector[i]);
                        myFlow.msleep(60000);

                        myFlow.readVersion(BOARD_TYPE_INTERFACE,0);
                        myFlow.msleep(500);
                        if(queryBoardVersion(flatVector[i]))
                        {
                            appendUpdateResult(g_myLan().ids_interfaceBoard+g_myLan().ids_upgradeSucceeded);
                            continue;
                        }
                        else
                        {
                            appendUpdateResult(g_myLan().ids_interfaceBoard+g_myLan().ids_upgradeFailed,false);
                            return;
                        }
                    }

                    isBackBurn = 1;

                    m_count = 0;
                    struGsh.bIsUpgrading = true;
                    m_boardAddr = getBoardAddr(m_nBoardTypeIndex,m_nChainIndex);
                    myFlow.switchUserMirror(m_nBoardTypeIndex<<4 |FACTORY_MODE,BOARD_TYPE_INTERFACE,0,0);
                    id = timer_interface_wait_toFactory ;
                    startTimer(time_switchMode);

                }
                else
                {
                    appendUpdateResult(g_myLan().ids_interfaceBoard+g_myLan().ids_setFactoryFailed,false);
                }
            }
            else
            {
                ui->BackBurnBtn->setEnabled(true);
            }
        }
        else
        {
            ui->BackBurnBtn->setEnabled(true);
            g_infoWidget().setLabelText(g_myLan().ids_wrongPassword);
            g_infoWidget().delayShow();
            myFlow.sleep(2);
            g_infoWidget().hide();
        }
    }
    else
    {
        ui->BackBurnBtn->setEnabled(true);
    }


}

bool UpdateFpgaWidget::queryBoardVersion(QString fileName)
{
    QString version = extractVersion(fileName);
    qDebug()<<"version"<<version;
    qDebug()<<"struGsh.struVer.sInterface[0]"<<struGsh.struVer.sInterface[0];
    qDebug()<<"struGsh.struVer.sInterface[1]"<<struGsh.struVer.sInterface[1];

    if((version.contains(QString::number(struGsh.struVer.sInterface[0])))
            && (version.contains(QString::number(struGsh.struVer.sInterface[1]))))
    {
        return true;
    }
    else
    {
        return false;
    }
}

QString UpdateFpgaWidget::extractVersion(const QString &fileName) {
    // 定义正则表达式以匹配 'V255.255' 形式的版本号
    QRegExp versionRegex("V\\d+\\.\\d+");

    // 查找匹配
    if (versionRegex.indexIn(fileName) != -1) {
        return versionRegex.cap(0);  // 返回匹配的字符串
    } else {
        return QString();  // 如果没有匹配，返回空字符串
    }
}

QVector<QString> UpdateFpgaWidget::flattenQVector(const QVector<QStringList> &nestedVector) {
    QVector<QString> flatVector;

    // 遍历 QVector<QStringList>
    for (int i = 0; i < nestedVector.size(); ++i) {
        const QStringList &currentList = nestedVector[i];

        // 遍历 QStringList 并将元素添加到 flatVector
        for (int j = 0; j < currentList.size(); ++j) {
            flatVector.append(currentList[j]);
        }
    }

    return flatVector;
}

void UpdateFpgaWidget::sendUpdateFile(QString fileName)
{
    QFile file(fileName);
    int count = 0;
    long fileSize = file.size();
    if (fileSize % 3)
    {
        fileSize= fileSize/3+1;
    }
    else
        fileSize = fileSize/3;

    fileSize += (16-fileSize%16);
    count = fileSize;
    fileSize = 3550368;


    qDebug()<<"file size:"<<fileSize;

    char beginData[8] = { 0xB9,0x53,0xB9,0x53,fileSize >> 24,(fileSize >> 16) % 256 ,(fileSize >> 8) % 65536, fileSize % 256 };
    myFlow.myNetWork->udpRestart();

    if(myFlow.myNetWork->udpSendTo(beginData,8) != 8)
    {
        perror("update: udp send error");
        appendUpdateResult(g_myLan().ids_sendFileFailed);
        return;
    }

    uint crcRet=0;
    appendUpdateResult(g_myLan().ids_sendData);
    int time = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000;

    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << " Could not open the file for reading";
        appendUpdateResult(g_myLan().ids_sendFileFailed);
        return;
    }

    int totalsize =0;
    int bufSize = 1024 ;
    char *sendData = (char*)malloc(bufSize);
    QTextStream in(&file);
    while(!in.atEnd())
    {
        QString myText= "";
        for(int i=0;i<1024;i++){
            QString str;
            str = in.readLine();
            myText.append(str.trimmed());
        }
        QString str(myText);

        QByteArray bytes = str.toLatin1();   //Qstring 转 QByteArray

        //字符串转成16进制数组
        if(!StringToHexGroup(sendData,bytes.data(),bytes.length())){
            qDebug("StringToHexGroup error\n");
            free(sendData);
            sendData = NULL;
            file.close();
            appendUpdateResult(g_myLan().ids_sendFileFailed);
            return;
        }
        //补全16字节下发
        int mis = (bytes.length()/2)%16;
        if(mis){
            mis = 16 -mis;
            qDebug()<<"str_length"<<str.length()<<"bytes_length"<<bytes.length()/2;
            for(int i = 0;i<mis;i++){
                sendData[bytes.length()/2+i] = 0xFF;
            }
            bytes.resize(bytes.length()+2*mis);
        }
        totalsize += bytes.length()/2 ;

        if (myFlow.myNetWork->udpSendTo(sendData, bytes.length()/2)!=bytes.length()/2){
            qDebug()<<"\t send data error";

            free(sendData);
            sendData = NULL;
            file.close();
            appendUpdateResult(g_myLan().ids_sendFileFailed);
            return;
        }
        mySerial.crcFpga2(sendData,bytes.length()/2,crcRet);

        myFlow.msleep(1);
    }
    while(totalsize <fileSize)
    {
        int sendSize = bufSize;
        if(fileSize - totalsize < bufSize){
            int size = fileSize - totalsize;
            int mis = size%16;
            sendSize = mis+size;
        }
        for(int i = 0;i<sendSize;i++){
            sendData[i] = 0xFF;
        }
        totalsize += sendSize;

        if (myFlow.myNetWork->udpSendTo(sendData,sendSize)!=sendSize){
            qDebug()<<"\t send data error";

            free(sendData);
            sendData = NULL;
            file.close();
            appendUpdateResult(g_myLan().ids_sendFileFailed);
            return;
        }
        mySerial.crcFpga2(sendData,sendSize,crcRet);

        myFlow.msleep(1);
    }

    qDebug()<<"total size :"<<totalsize;
    myFlow.msleep(1000);
    time = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000 - time;
    qDebug()<<"\t send data end cost "<<time<<"s";
    char crcData[2] = { (crcRet & 0xFF00) / 256 ,crcRet & 0xFF };
    printf("--%d,%d\n",crcData[0],crcData[1]);
    if (myFlow.myNetWork->udpSendTo(crcData, 2)!=2)
    {
        free(sendData);
        sendData = NULL;
        file.close();
        appendUpdateResult(g_myLan().ids_sendFileFailed);
        return;
    }
    free(sendData);
    sendData = NULL;
    file.close();

    appendUpdateResult(g_myLan().ids_upgrading);
}

QVector<QStringList> UpdateFpgaWidget::getBackBurnFileName()
{
    QString sourceDirPath = g_Runtime().getUsbPath() + "/fac";
    QDir sourceDir(sourceDirPath);

    if (!sourceDir.exists()) {
        g_infoWidget().setLabelText(g_myLan().msg_usb_insert);
        g_infoWidget().showSecs();
        return QVector<QStringList>();
    }

    // 定义需要搜索的模式列表
    QStringList patterns;
    patterns << "*INTERFACE*app0*V255.255*";
    patterns << "*INTERFACE*fac*";
//    patterns << "*INTERFACE*app1*";

    QVector<QStringList> extractedFiles(3);

    // 针对每个模式进行文件搜索并按顺序存储
    for (int i = 0; i < patterns.size(); ++i) {
        QStringList fileList = sourceDir.entryList(QStringList() << patterns[i], QDir::Files);

        foreach (const QString &fileName, fileList) {
            QString sourceFilePath = sourceDir.absoluteFilePath(fileName);
            extractedFiles[i] << sourceFilePath;
            qDebug() << "Found:" << sourceFilePath;
        }
    }

    return extractedFiles;
}

void UpdateFpgaWidget::getUpdateFileInfo()
{
    if(fileName.contains("SIZE0"))
    {
        m_needFileSize = 3550368;
        m_needDelay = 60*1000;
    }
    else if(fileName.contains("SIZE1"))
    {
        m_needFileSize = 678656;
        m_needDelay = 30*1000;
    }
    else if(fileName.contains("SIZE2"))
    {
        m_needFileSize = 6291456;
        m_needDelay = 120*1000;
    }
    else if(fileName.contains("SIZE3"))
    {
        m_needFileSize = 12581888;
        m_needDelay = 130*1000;
    }
    else
    {
        m_needFileSize = 3550368;
        m_needDelay = 60*1000;
    }
    m_needDelay += 30*1000;
    m_needDelay = 90*1000;
}

void UpdateFpgaWidget::on_pushButton_clicked()
{
    g_MainManager().ShowWidget(SM_FPGA_SET_WIDGET);
    return;
}
