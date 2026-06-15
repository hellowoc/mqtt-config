#include "ipctestnetwidget.h"
#include "ui_ipctestnetwidget.h"


const int columnCount = 4;

IPCTestNetWidget::IPCTestNetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::IPCTestNetWidget)
{
    ui->setupUi(this);

    m_curNetTestIpcNum = 0;

    ui->tableWidget->setColumnCount(columnCount);
    ui->tableWidget->setColumnWidth(0,200);
    ui->tableWidget->setColumnWidth(1,200);
    ui->tableWidget->setColumnWidth(2,200);
    ui->tableWidget->setColumnWidth(3,200);


    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setFocusPolicy(Qt::NoFocus);
    ui->tableWidget->setShowGrid(false);

    QHeaderView *header = ui->tableWidget->verticalHeader();
    header->setHidden(true);// 隐藏行号

    QHeaderView *horiHeader = ui->tableWidget->horizontalHeader();
    horiHeader->setFixedHeight(50);
    horiHeader->setSectionResizeMode(QHeaderView::Stretch);

    myTestNetThread  = new ipcTestNetThread;

    ui->testinglabel->hide();

    ui->starttestbtn->setEnabled(TRUE);
    ui->stoptestbtn->setEnabled(FALSE);
}

IPCTestNetWidget::~IPCTestNetWidget()
{
    delete ui;
}

void IPCTestNetWidget::showPage(bool bshow)
{
    if(bshow){
        updateTestNetInfo();
        updateTableWidget();
    }
    else
    {
        if (myTestNetThread->isRunning())
        {

            testIpcNetworkSend(m_curNetTestIpcNum, 0);

            myTestNetThread->stop();

            for(int i = 0; i < struCnfg.nIpcCount; i++)
            {
                myFlow.configIpcSortStop(i);
            }


            ui->starttestbtn->setEnabled(TRUE);
            ui->stoptestbtn->setEnabled(FALSE);
            ui->testinglabel->hide();

            struIpcShare.nTestNetworkIsRun[struGsh.nView][struGsh.nUnit] = 0;
        }
    }
}

void IPCTestNetWidget::retranslateUiWidget()
{
    ui->starttestbtn->setText(g_myLan().network_test+"1");
    ui->starttestbtn_2->setText(g_myLan().network_test+"2");
    ui->stoptestbtn->setText(g_myLan().ids_stop);
    ui->ipcStatusbtn->setText("IPC"+g_myLan().check);

    ui->moreInfobtn->setText(g_myLan().ids_more);

    ui->testinglabel->setText(g_myLan().checking);
}

void IPCTestNetWidget::receiveMsg(int msg1, int msg2, int msg3)
{

}

void IPCTestNetWidget::updateTestNetInfo()
{
    m_testinfoVec.clear();
    int total=0, lost=0,speed=0;
    QString namestr,cameraNameStr;


    for(int i = 0;i < struCnfg.nIpcCount;i++){
//        memcpy(&ipcstate, &struIpcShare.struIpcStatus[i],sizeof(ipcstate));
        namestr = QString("IPC%1").arg(i+1);
       total =  struIpcShare.struIpcTestNetInfo[i].screenIpcTotal;
       lost = struIpcShare.struIpcTestNetInfo[i].screenIpcLost;

        IPCTestNetInfo info(namestr,total, lost, 0);
        m_testinfoVec.push_back(info);

        for(int j = 0;j < struIpcShare.struIpcInfo[i].cameraCount;j++){
            cameraNameStr = QString(g_myLan().ids_camera+"%1").arg(j+1);
            total = struIpcShare.struIpcTestNetInfo[i].testTotalPacket[j];
            lost = struIpcShare.struIpcTestNetInfo[i].testLostPacket[j];
            speed = struIpcShare.struIpcTestNetInfo[i].netTransSpeed[j];

            IPCTestNetInfo info2(cameraNameStr, total, lost, speed);
            m_testinfoVec.push_back(info2);
        }
    }
}

void IPCTestNetWidget::updateTableWidget()
{
    QStringList list;
//    list<<g_myLan().ids_board<<g_myLan().state;
    list<<g_myLan().ids_board<<g_myLan().total_frame<<g_myLan().compression_ratio<<g_myLan().speed;
    ui->tableWidget->setHorizontalHeaderLabels(list);
    ui->tableWidget->setRowCount(m_testinfoVec.size());
    for(int i = 0;i< m_testinfoVec.size();++i){
        ui->tableWidget->setItem(i,0, new QTableWidgetItem(m_testinfoVec.at(i).strName));
        ui->tableWidget->setItem(i,1, new QTableWidgetItem(m_testinfoVec.at(i).total));
        ui->tableWidget->setItem(i,2, new QTableWidgetItem(m_testinfoVec.at(i).lost));
        ui->tableWidget->setItem(i,3, new QTableWidgetItem(m_testinfoVec.at(i).speed));

        for(int j = 0;j < columnCount;j++){
            ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            ui->tableWidget->item(i,j)->setFlags(Qt::ItemIsEnabled);
        }
    }

    QString hardwareInfoStr1;

    QString cpuStr = "IPC-1:  CPU:"+QString::number(struIpcShare.struIpcTestNetInfo[0].cpuUsed);
    QString memoryStr = "    Mem:"+QString::number(struIpcShare.struIpcTestNetInfo[0].memoryUsed);
    QString gpu0Str = "    GPUA:"+QString::number(struIpcShare.struIpcTestNetInfo[0].gpu0Used);     //集成显卡
    QString gpu1ryStr = "    GPU:"+QString::number(struIpcShare.struIpcTestNetInfo[0].gpu1Used);    //独立显卡

    QString recv1Str = "    recv:"+QString::number(struIpcShare.struIpcTestNetInfo[0].testScreenRecieved);
//    hardwareInfoStr1 = cpuStr+memoryStr+gpu0Str+gpu1ryStr;
    hardwareInfoStr1 = cpuStr+memoryStr+gpu1ryStr+recv1Str;
    ui->ipchardwarelabel->setText(hardwareInfoStr1);

    if(struCnfg.nIpcCount > 1)
    {
        QString hardwareInfoStr2;
        cpuStr = "IPC-2:  CPU:"+QString::number(struIpcShare.struIpcTestNetInfo[1].cpuUsed);
        memoryStr = "    Mem:"+QString::number(struIpcShare.struIpcTestNetInfo[1].memoryUsed);
        gpu0Str = "    GPUA:"+QString::number(struIpcShare.struIpcTestNetInfo[1].gpu0Used);
        gpu1ryStr = "    GPU:"+QString::number(struIpcShare.struIpcTestNetInfo[1].gpu1Used);

        QString recv2Str = "    recv:"+QString::number(struIpcShare.struIpcTestNetInfo[1].testScreenRecieved);
//        hardwareInfoStr2 = cpuStr+memoryStr+gpu0Str+gpu1ryStr;
        hardwareInfoStr2 = cpuStr+memoryStr+gpu1ryStr+recv2Str;
        ui->ipchardwarelabel_2->setText(hardwareInfoStr2);
        ui->ipchardwarelabel_2->show();

        ui->starttestbtn_2->show();
    }
    else
    {
        ui->ipchardwarelabel_2->hide();
        ui->starttestbtn_2->hide();
    }

    ui->ipchardwarelabel->hide();
    ui->ipchardwarelabel_2->hide();

}

/* ipc1错误帧测试 */
void IPCTestNetWidget::on_starttestbtn_clicked()
{
//    if(struGsh.bSortStart)
//    {
//        g_infoWidget().setLabelText(g_myLan().msg_turn_off_first);

//        g_infoWidget().delayShow();

//        myFlow.sleep(1);

//        g_infoWidget().hide();

//        return;
//    }

    if (!myTestNetThread->isRunning() && struIpcShare.nTestNetworkIsRun == 0)
    {
        myMessageBox msgBox(MSG_QUES, g_myLan().cfm_start_net_test+"1");

        int ret = msgBox.exec();

        if (ret == QDialog::Accepted)
        {
            struIpcShare.nTestNetworkIsRun = 1;

            myTestNetThread->start();

            myFlow.configIpcSortStart(0);

            struIpcShare.struIpcTestNetInfo[0].testScreenRecieved = 0;

            testIpcNetworkSend(0, 1);

            ui->starttestbtn->setEnabled(FALSE);
            ui->starttestbtn_2->setEnabled(FALSE);
            ui->stoptestbtn->setEnabled(TRUE);
            ui->ipcStatusbtn->setEnabled(FALSE);
            ui->moreInfobtn->setEnabled(FALSE);

            ui->testinglabel->show();
        }
    }

    updateTestNetInfo();
    updateTableWidget();
}

/* ipc2错误帧测试 */
void IPCTestNetWidget::on_starttestbtn_2_clicked()
{
//    if(struGsh.bSortStart)
//    {
//        g_infoWidget().setLabelText(g_myLan().msg_turn_off_first);

//        g_infoWidget().delayShow();

//        myFlow.sleep(1);

//        g_infoWidget().hide();

//        return;
//    }

    if (!myTestNetThread->isRunning() && struIpcShare.nTestNetworkIsRun == 0)
    {
        myMessageBox msgBox(MSG_QUES, g_myLan().cfm_start_net_test+"2");

        int ret = msgBox.exec();

        if (ret == QDialog::Accepted)
        {
            struIpcShare.nTestNetworkIsRun = 1;

            myTestNetThread->start();

            myFlow.configIpcSortStart(1);

            struIpcShare.struIpcTestNetInfo[1].testScreenRecieved = 0;
            testIpcNetworkSend(1, 1);

            ui->starttestbtn->setEnabled(FALSE);
            ui->starttestbtn_2->setEnabled(FALSE);
            ui->stoptestbtn->setEnabled(TRUE);
            ui->ipcStatusbtn->setEnabled(FALSE);
            ui->moreInfobtn->setEnabled(FALSE);

            ui->testinglabel->show();
        }
    }

    updateTestNetInfo();
    updateTableWidget();
}

void IPCTestNetWidget::on_stoptestbtn_clicked()
{
    if (myTestNetThread->isRunning())
    {
        myTestNetThread->stop();

        g_infoWidget().setLabelText(g_myLan().stop_testing);

        g_infoWidget().delayShow();

        testIpcNetworkSend(m_curNetTestIpcNum, 0);

        ui->testinglabel->hide();

        myFlow.sleep(5);

        for(int i = 0; i < struCnfg.nIpcCount; i++)
        {
            myFlow.configIpcSortStop(i);
        }

        g_infoWidget().hide();

        updateTestNetInfo();
        updateTableWidget();

        ui->starttestbtn->setEnabled(TRUE);
        ui->starttestbtn_2->setEnabled(TRUE);
        ui->stoptestbtn->setEnabled(FALSE);
        ui->ipcStatusbtn->setEnabled(TRUE);
        ui->moreInfobtn->setEnabled(TRUE);

    }
    struIpcShare.nTestNetworkIsRun = 0;
}

void IPCTestNetWidget::testIpcNetworkSend(int ipcIndex, int startStop)
{
    unsigned char data[16];
    memset(data, 0, 16);

    if(startStop == 1)
    {
        m_curNetTestIpcNum = ipcIndex;
    }

    data[0] = startStop%256;
    for(int i = 0; i < 1; i++)
    {
        g_Udp.pushUdpCmdData(CMD_UDP_IPC_TEST_NET_CRTL, true, struCnfg.nIpcIpAddrBase+ipcIndex, 0, 1, data);

        myFlow.msleep(500);
    }
}

void IPCTestNetWidget::on_ipcStatusbtn_clicked()
{
    unsigned char data[16];
    memset(data, 0, 16);

    myMessageBox msgBox(MSG_QUES, g_myLan().cfm_start_ipc_status_test);

    int ret = msgBox.exec();

    if (ret == QDialog::Accepted)
    {
        g_infoWidget().setLabelText(g_myLan().checking);

        g_infoWidget().delayShow();

        myFlow.sleep(1);

        for(int ipcIndex = 0; ipcIndex < struCnfg.nIpcCount; ipcIndex++)
        {
            myFlow.getIpcHardwareInfo(ipcIndex);
#if 0
            struIpcShare.struIpcReqStauts[ipcIndex].testNetHardwareInfoReq = IPC_REQ_RETURN_NULL;

            for(int i = 0; i < 1; i++)
            {
                g_Udp.pushUdpCmdData(CMD_UDP_IPC_REQ_HARDWARE_INFO, true, IPC_IP_ADDR_BASE+ipcIndex, 0, 1, data);

                for(int ii = 0; ii < 20; ii++)
                {
                    myFlow.msleep(400);
                    if(struIpcShare.struIpcReqStauts[ipcIndex].testNetHardwareInfoReq == 1)
                    {
                        break;
                    }
                }
            }

            struIpcShare.struIpcReqStauts[ipcIndex].testNetConfigFileReq = IPC_REQ_RETURN_NULL;

            for(int i = 0; i < 1; i++)
            {
                g_Udp.pushUdpCmdData(CMD_UDP_IPC_REQ_CONFIG_FILE, true, IPC_IP_ADDR_BASE+ipcIndex, 0, 1, data);

                for(int ii = 0; ii < 10; ii++)
                {
                    myFlow.msleep(500);
                    if(struIpcShare.struIpcReqStauts[ipcIndex].testNetConfigFileReq == 1)
                    {
                        break;
                    }
                }
            }
#endif
        }
        g_infoWidget().hide();
    }

#ifdef Q_OS_UNIX
    myMqttMsgParaseThread->n_uploadPartsStatusFlag = 1;
#endif
    updateTestNetInfo();
    updateTableWidget();
}

void IPCTestNetWidget::on_moreInfobtn_clicked()
{
    QString configStr;

    QString ipcCpuUsedStr = "cpu0(%)="+QString::number(struIpcShare.struIpcTestNetInfo[0].cpuUsed);
    QString ipcMemoryUsedStr = "  memory(%)="+QString::number(struIpcShare.struIpcTestNetInfo[0].memoryUsed);
    QString ipcGpuUsedStr = "  gpu0(%)="+QString::number(struIpcShare.struIpcTestNetInfo[0].gpu0Used);
    QString ipcCpuTempdStr = "  cpu0(℃)="+QString::number(struIpcShare.struIpcTestNetInfo[0].cpuTemp);
    QString ipcGpuTempdStr = "  gpu0(℃)="+QString::number(struIpcShare.struIpcTestNetInfo[0].gpuTemp);
    configStr += "\n";

    configStr += ipcCpuUsedStr+ipcMemoryUsedStr+ipcGpuUsedStr+ipcCpuTempdStr+ipcGpuTempdStr;

    if(struCnfg.nIpcCount > 1)
    {
        configStr += "\n";
        ipcCpuUsedStr = "cpu2(%)="+QString::number(struIpcShare.struIpcTestNetInfo[1].cpuUsed);
        ipcMemoryUsedStr = "  memory2(%)="+QString::number(struIpcShare.struIpcTestNetInfo[1].memoryUsed);
        ipcGpuUsedStr = "  gpu2(%)="+QString::number(struIpcShare.struIpcTestNetInfo[1].gpu0Used);
        ipcCpuTempdStr = "  cpu2(℃)="+QString::number(struIpcShare.struIpcTestNetInfo[1].cpuTemp);
        ipcGpuTempdStr = "  gpu2(℃)="+QString::number(struIpcShare.struIpcTestNetInfo[1].gpuTemp);
        configStr += "\n";
        configStr += ipcCpuUsedStr+ipcMemoryUsedStr+ipcGpuUsedStr+ipcCpuTempdStr+ipcGpuTempdStr;
    }

    QString cameraCountStr = "cameraCount="+QString::number(struIpcShare.struIpcTestNetInfo[0].struIpcConfigFileInfo.cameraCount);
    QString batchSizeStr = "  batchSize="+QString::number(struIpcShare.struIpcTestNetInfo[0].struIpcConfigFileInfo.batchSize);
    QString roiHeightStr = "  roiHeight="+QString::number(struIpcShare.struIpcTestNetInfo[0].struIpcConfigFileInfo.roiHeight);
    QString roiRepeatStr = "  roiRepeat="+QString::number(struIpcShare.struIpcTestNetInfo[0].struIpcConfigFileInfo.roiRepeat);
    QString nmsStr = "  nms="+QString::number(struIpcShare.struIpcTestNetInfo[0].struIpcConfigFileInfo.nms);
    QString resultProtoStr = "  resultProto="+QString::number(struIpcShare.struIpcTestNetInfo[0].struIpcConfigFileInfo.resultProto);

    QString ipCameraStr = "  ipCamera="+QString("%1").arg(struIpcShare.struIpcTestNetInfo[0].struIpcConfigFileInfo.ipCamera);
    QString ipToCameraStr = "  ipToCamera="+QString("%1").arg(struIpcShare.struIpcTestNetInfo[0].struIpcConfigFileInfo.ipToCamera);
    QString ipToPanelStr = "  ipToPanel="+QString("%1").arg(struIpcShare.struIpcTestNetInfo[0].struIpcConfigFileInfo.ipToPanel);

    configStr += "\n";
    configStr += cameraCountStr+batchSizeStr+roiHeightStr+roiRepeatStr+nmsStr+resultProtoStr+ipCameraStr+ipToCameraStr+ipToPanelStr;

    if(struCnfg.nIpcCount > 1)
    {
        configStr += "\n";
        configStr += "\n";

        cameraCountStr = "cameraCount="+QString::number(struIpcShare.struIpcTestNetInfo[1].struIpcConfigFileInfo.cameraCount);
        batchSizeStr = "  batchSize="+QString::number(struIpcShare.struIpcTestNetInfo[1].struIpcConfigFileInfo.batchSize);
        roiHeightStr = "  roiHeight="+QString::number(struIpcShare.struIpcTestNetInfo[1].struIpcConfigFileInfo.roiHeight);
        roiRepeatStr = "  roiRepeat="+QString::number(struIpcShare.struIpcTestNetInfo[1].struIpcConfigFileInfo.roiRepeat);
        nmsStr = "  nms="+QString::number(struIpcShare.struIpcTestNetInfo[1].struIpcConfigFileInfo.nms);
        resultProtoStr = "  resultProto="+QString::number(struIpcShare.struIpcTestNetInfo[1].struIpcConfigFileInfo.resultProto);

        ipCameraStr = "  ipCamera="+QString("%1").arg(struIpcShare.struIpcTestNetInfo[1].struIpcConfigFileInfo.ipCamera);
        ipToCameraStr = "  ipToCamera="+QString("%1").arg(struIpcShare.struIpcTestNetInfo[1].struIpcConfigFileInfo.ipToCamera);
        ipToPanelStr = "  ipToPanel="+QString("%1").arg(struIpcShare.struIpcTestNetInfo[1].struIpcConfigFileInfo.ipToPanel);

        configStr += cameraCountStr+batchSizeStr+roiHeightStr+roiRepeatStr+nmsStr+resultProtoStr+ipCameraStr+ipToCameraStr+ipToPanelStr;
    }

    myLongMessageBox msgBox(MSG_QUES, configStr);

    int ret = msgBox.exec();
    if (ret == QDialog::Accepted)
    {

    }
}

/* ****************************************************************************
 *
 *                        错误帧测试线程
 *
 *******************************************************************************/
ipcTestNetThread::ipcTestNetThread()
{
    stopped = FALSE;
    m_bIsFast = false;
}

void ipcTestNetThread::run(void)
{
    while (!stopped)
    {
        msleep(1000);
    }
    stopped = false;
    if (m_bIsFast){

    }
}

void ipcTestNetThread::stop()
{
    stopped = TRUE;
}

void ipcTestNetThread::setTestSpeed(bool isFast)
{
    m_bIsFast = isFast;
}
