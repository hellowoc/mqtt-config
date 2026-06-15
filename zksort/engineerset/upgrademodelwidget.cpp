#include "upgrademodelwidget.h"
#include "ui_upgrademodelwidget.h"

upgradeModelWidget::upgradeModelWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::upgradeModelWidget)
{
    ui->setupUi(this);
    m_count = 0;
    m_nUpgradeTypeIndex = 0;
    ui->progressBar->setRange(0,100);
    m_nChainIndex = BOARD_TYPE_ALL_CAMERA;//针对ｘ系统升级
    m_functionflag = 0;
}

upgradeModelWidget::~upgradeModelWidget()
{
    delete ui;
}

void upgradeModelWidget::showPage(bool bshow)
{
    if(bshow){
        if (struCnfg.nMachineType == MACHINE_LD2_ZL_GP){
            viewList<<QString(g_myLan().lower_layer+g_myLan().ids_fm) <<QString(g_myLan().lower_layer+g_myLan().ids_bm)<<QString(g_myLan().lower_layer+g_myLan().ids_fs)<<QString(g_myLan().lower_layer+g_myLan().ids_bs)
                   <<QString(g_myLan().upper_layer+g_myLan().ids_fm) <<QString(g_myLan().upper_layer+g_myLan().ids_bm)<<QString(g_myLan().upper_layer+g_myLan().ids_fs)<<QString(g_myLan().upper_layer+g_myLan().ids_bs);
        }
        else{
            viewList<<g_myLan().ids_fm <<g_myLan().ids_bm<<g_myLan().ids_fs<<g_myLan().ids_bs
                   <<g_myLan().ids_fm <<g_myLan().ids_bm<<g_myLan().ids_fs<<g_myLan().ids_bs;

        }

        ui->comboBox_chain->setVisible(false);
        ui->label_2->setVisible(false);

        struGsh.bUpdateState = 1;
        updateAll();
    }
    else
        struGsh.bUpdateState = 0;
}

void upgradeModelWidget::receiveMsg(int msg1, int msg2, int msg3)
{

}

void upgradeModelWidget::retranslateUiWidget()
{
    updateAll();
    ui->checkFileBtn->setText(g_myLan().ids_checkUpdateFile);
    ui->startUpgradeBtn->setText(g_myLan().ids_startUpdate);

    QStringList list;
    list<<QString("RK Model")<<QString("X Model")<<QString("X System");
    ui->comboBox_function->addItems(list);
}

void upgradeModelWidget::on_comboBox_function_activated(int index)
{
    ui->FileListWidget->clear();
    m_functionflag = index;
}

//void upgradeModelWidget::on_comboBox_chute_activated(int index)
//{
//    m_nChuteIndex = index;
//}

void upgradeModelWidget::on_comboBox_chain_activated(int index)
{
    m_nChainIndex = index;
}

void upgradeModelWidget::appendUpdateResult(QString str,bool ok)
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
bool upgradeModelWidget::checkUpdateFile()
{
    if(m_functionflag == 0)
    {
        if (!ui->FileListWidget->currentItem())
        {
            ui->FileListWidget->clear();
            appendUpdateResult(g_myLan().ids_notSelectFile);
            return 0;
        }
        QString listinfo = ui->FileListWidget->currentItem()->text();
        ui->FileListWidget->clear();

        QStringList list = listinfo.split(" ");
        modeFileDataName = list.at(0).left(list.at(0).length());
        modeFileDataWholeName = QString("%1%2").arg(g_Runtime().getUsbPath()+"zkmodels/").arg(list.at(0));
#ifdef Q_OS_UNIX
//        if(myMqttMsgParaseThread->m_updateFileType == myMqttMsgParaseThread->UPDATE_IPCMODEL)
//        {
//            modeFileDataWholeName = QString("%1%2").arg("/sdcard/udisk/").arg(list.at(0));
//        }
#endif
        return 1;
    }
    else
    {
        if (!ui->FileListWidget->currentItem())
        {
            ui->FileListWidget->clear();
            appendUpdateResult(g_myLan().ids_notSelectFile);
            return 0;
        }
        QString listinfo = ui->FileListWidget->currentItem()->text();
        ui->FileListWidget->clear();

        QStringList list = listinfo.split(" ");
        modeFileDataName = list.at(0).left(list.at(0).length());

        if(m_functionflag == 1)
            modeFileDataWholeName = QString("%1%2").arg(g_Runtime().getUsbPath()+"zkmodels/").arg(list.at(0));
        else
            modeFileDataWholeName = QString("%1%2").arg(g_Runtime().getUsbPath()+"bin/").arg(list.at(0));


        qDebug("000 modeFileDataWholeName = %s", modeFileDataWholeName.toUtf8().data());

        QFile file(modeFileDataWholeName);
        int datalen = file.size();
        if(m_functionflag == 1)
        {
            if(datalen>5017753)
            {
                return 0;
            }
        }
        else if(m_functionflag == 2)
        {
            if(datalen<5017753)
            {
                return 0;
            }
        }

        return 1;
    }
}

void upgradeModelWidget::on_startUpgradeBtn_clicked()
{
    if(m_functionflag == 0)
    {
        int dataLen = 0, configLen = 0;
        int packetTotal = 1;
        int i,j,k, err=0;
        int totalTimeCost,curTimeCost;

        ui->progressBar->setValue(0);
        m_upgradeResList.clear();

        ui->startUpgradeBtn->setEnabled(false);
        ui->ipc1resultlabel->setText(g_myLan().ids_upgrading);
        m_count = 0;
        if (!checkUpdateFile())
        {
#ifdef Q_OS_UNIX
//            if(myMqttMsgParaseThread->m_updateFileType == myMqttMsgParaseThread->UPDATE_IPCMODEL)
//            {
//                myMqttMsgParaseThread->onCmdReturn(myMqttMsgParaseThread->m_uuid,myMqttMsgParaseThread->m_tmp,MODEL_FILE_MISMATCH);
//            }
#endif
            g_infoWidget().setLabelText(g_myLan().model_file_mismatch+"-100");
            g_infoWidget().delayShow();
            myFlow.sleep(2);
            g_infoWidget().hide();
            return;
        }

        QFile fileData(modeFileDataWholeName);


        if (fileData.open(QIODevice::ReadOnly))
        {
            dataLen = fileData.size();
            qDebug("111 modeFileDataWholeName is %s,filesize is %d", modeFileDataWholeName.toLatin1().data(),dataLen);

            //停止心跳
            struGsh.bUpdateState = 1;
            char* tmpModeldata = new char[dataLen];
            QDataStream dsData(&fileData);
            dsData.readRawData(tmpModeldata, dataLen);
            fileData.close();

            //组包添加模型-头
            unsigned char sendPacket[MAX_IPC_MODEL_NAME_LENGTH+18];
            struIpcShare.struIpcReqStauts[struGsh.nView][struGsh.nUnit].modelAddDataReq = IPC_REQ_RETURN_NULL;

            memset(sendPacket, 0, MAX_IPC_MODEL_NAME_LENGTH);
            memcpy(sendPacket, modeFileDataName.toLatin1().data(), modeFileDataName.length());
            sendPacket[64] = dataLen/(256*256*256);
            sendPacket[65] = (dataLen/(256*256))%256;
            sendPacket[66] = (dataLen%(256*256))/256;
            sendPacket[67] = dataLen%256;

            sendPacket[68] = 1;
            sendPacket[69] = 0;
            sendPacket[70] = sendPacket[64];
            sendPacket[71] = sendPacket[65];
            sendPacket[72] = sendPacket[66];
            sendPacket[73] = sendPacket[67];

            sendPacket[74] = PACKET_LEN/256;
            sendPacket[75] = PACKET_LEN%256;

            packetTotal = (dataLen%PACKET_LEN == 0) ? dataLen/PACKET_LEN : dataLen/PACKET_LEN+1;
            sendPacket[76] = packetTotal/256;
            sendPacket[77] = packetTotal%256;

            for(int i = 0; i < dataLen;i++)
            {
                err += tmpModeldata[i];
            }
            sendPacket[78] = err/(256*256*256);
            sendPacket[79] = (err/(256*256))%256;
            sendPacket[80] = (err%(256*256))/256;
            sendPacket[81] = err%256;

            struIpcShare.struIpcReqStauts[struGsh.nView][struGsh.nUnit].modelAddHeadReq = 0;
            g_Udp.pushUdpCmdData(CMD_UDP_IPC_MODEL_ADD_HEAD, true,0x09, 0xff, MAX_IPC_MODEL_NAME_LENGTH+18, sendPacket);

            //组包添加模型-数据
            g_infoWidget().setLabelText(g_myLan().ids_upgrading);
            g_infoWidget().delayShow();

            QString proecessStrDisp;
            int hasSendLen = 0;
            unsigned char* tmpPacket = new unsigned char[PACKET_LEN+3];

            totalTimeCost = packetTotal*(struCnfg.nUgSendInterval + 2); //ms
            curTimeCost = 0;
            //发送数据文件
            for(int ii = 0; ii < packetTotal; ii++)
            {
                memset(tmpPacket, 0, PACKET_LEN+3);

                tmpPacket[0] = 0;
                tmpPacket[1] = ii/256;
                tmpPacket[2] = ii%256;

                if(dataLen-hasSendLen >= PACKET_LEN) //至少还剩一个整包
                {
                    memcpy(tmpPacket+3, tmpModeldata+hasSendLen, PACKET_LEN);
                    g_Udp.pushUdpCmdData(CMD_UDP_IPC_MODEL_ADD_DATA, true,0x09, 0xff, PACKET_LEN+3, tmpPacket);
                    hasSendLen += PACKET_LEN;
                }
                else    //剩不到一个整包
                {
                    memcpy(tmpPacket+3, tmpModeldata+hasSendLen, dataLen-hasSendLen);
                    g_Udp.pushUdpCmdData(CMD_UDP_IPC_MODEL_ADD_DATA, true,0x09, 0xff, PACKET_LEN+3, tmpPacket);
                }
                if(ii%100 == 0)
                {
                    proecessStrDisp = QString::number((curTimeCost*100.0)/totalTimeCost,'f',1)+"%";
                    ui->progressBar->setValue((curTimeCost*100.0)/totalTimeCost);
                    ui->progressBar->setFormat(proecessStrDisp);
                }
                if((ii > 0)&&(ii%3 == 0))
                {
                    myFlow.msleep(6);
                    curTimeCost += 6;
                }
            }

            delete[] tmpPacket;
            delete[] tmpModeldata;

            for(int i = 0;i < packetTotal;i++ ){
                myFlow.msleep(struCnfg.nUgSendInterval);
                curTimeCost += struCnfg.nUgSendInterval;
                if(i%20 == 0){
                    proecessStrDisp = QString::number((curTimeCost*100.0)/totalTimeCost,'f',1)+"%";
                    ui->progressBar->setValue((curTimeCost*100.0)/totalTimeCost);
                    ui->progressBar->setFormat(proecessStrDisp);
                }
            }

            proecessStrDisp = "99.9%";
            ui->progressBar->setValue(99.9);
            ui->progressBar->setFormat(proecessStrDisp);

            //清升级状态标识
            for(j = 0; j < struCnfg.struLayerInfo[0].nViewChainTotal; j++)
            {
                if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
                for(k = 0; k <struCnfg.struLayerInfo[0].nViewUnitTotal[j]; k++)
                {
                    struIpcShare.struIpcInfo[j][k].modelUploadStat = 0;
                }
            }
            //上位机数据发完恢复心跳
            struGsh.bUpdateState = 0;
            QString upgradeResStr=g_myLan().upgrade_timeout;
            for(int ii = 0; ii < 30; ii++)
            {
                err = 0;
                myFlow.sleep(1);
                for(j = 0; j < struCnfg.struLayerInfo[0].nViewChainTotal; j++)
                {
                    if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
                    if(struCnfg.nXsystemBoard[j] == 1) continue;
                    for(k = 0; k <struCnfg.struLayerInfo[0].nViewUnitTotal[j]; k++)
                    {
                        if(struCnfg.struIpcCameraParams[i][j][k].ipcType == IPC_CAMERA_TYPE_NULL) continue;
                        if(struIpcShare.struIpcInfo[j][k].modelUploadStat == 4)
                        {
                            err = 4;
                            goto end;
                        }
                        if(struIpcShare.struIpcInfo[j][k].modelUploadStat != 3)
                        {
                            err = 1;
                            continue;
                        }
                    }
                }
                if(err == 0)
                    goto end;
            }
end:
            switch ( err) {
            case 0:
                proecessStrDisp = "100%";
                ui->progressBar->setValue(100);
                ui->progressBar->setFormat(proecessStrDisp);
                ui->ipc1resultlabel->setText(g_myLan().upgrade_success);
                break;
            case 1:
                ui->ipc1resultlabel->setText(g_myLan().upgrade_timeout);
                break;
            case 4:
                ui->ipc1resultlabel->setText(g_myLan().ids_upgradeFailed);
                break;
            default:
                break;
            }

            g_infoWidget().hide();
            updateAll();
            return;
        }
    }
    else
    {
        if(m_functionflag == 1)  //升级x系统模型
        {
            int dataLen = 0, configLen = 0;
            int packetTotal = 1;
            int i,j,k, err=0;

            ui->progressBar->setValue(0);
            ui->progressBar->setFormat(QString("0%"));

            ui->startUpgradeBtn->setEnabled(false);
            ui->ipc1resultlabel->setText(g_myLan().ids_upgrading);
            m_count = 0;
            if (!checkUpdateFile())
            {
                g_infoWidget().setLabelText(g_myLan().model_file_mismatch+"-100");
                g_infoWidget().delayShow();
                myFlow.sleep(2);
                g_infoWidget().hide();
                return;
            }

            QFile fileData(modeFileDataWholeName);


            if (fileData.open(QIODevice::ReadOnly))
            {
                dataLen = fileData.size();
                qDebug("111 modeFileDataWholeName is %s,filesize is %d", modeFileDataWholeName.toLatin1().data(),dataLen);

                //停止心跳
                struGsh.bUpdateState = 1;
                //清升级状态标识
                for(j = 0; j < struCnfg.struLayerInfo[0].nViewChainTotal; j++)
                {
                    if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;

                    for(k = 0; k <struCnfg.struLayerInfo[0].nViewUnitTotal[j]; k++)
                    {
                        struIpcShare.struIpcInfo[j][k].modelUploadStat = 0;
                    }
                }
                char* tmpModeldata = new char[dataLen];
                QDataStream dsData(&fileData);
                dsData.readRawData(tmpModeldata, dataLen);
                fileData.close();

                //组包添加模型-头
                unsigned char sendPacket[MAX_IPC_MODEL_NAME_LENGTH+18];
                struIpcShare.struIpcReqStauts[struGsh.nView][struGsh.nUnit].modelAddDataReq = IPC_REQ_RETURN_NULL;

                memset(sendPacket, 0, MAX_IPC_MODEL_NAME_LENGTH);
                memcpy(sendPacket, modeFileDataName.toLatin1().data(), modeFileDataName.length());
                sendPacket[64] = dataLen/(256*256*256);
                sendPacket[65] = (dataLen/(256*256))%256;
                sendPacket[66] = (dataLen%(256*256))/256;
                sendPacket[67] = dataLen%256;

                sendPacket[68] = 1;
                sendPacket[69] = 0;
                sendPacket[70] = sendPacket[64];
                sendPacket[71] = sendPacket[65];
                sendPacket[72] = sendPacket[66];
                sendPacket[73] = sendPacket[67];

                sendPacket[74] = PACKET_LEN/256;
                sendPacket[75] = PACKET_LEN%256;

                packetTotal = (dataLen%PACKET_LEN == 0) ? dataLen/PACKET_LEN : dataLen/PACKET_LEN+1;
                sendPacket[76] = packetTotal/256;
                sendPacket[77] = packetTotal%256;

                for(int i = 0; i < dataLen;i++)
                {
                    err += tmpModeldata[i];
                }
                sendPacket[78] = err/(256*256*256);
                sendPacket[79] = (err/(256*256))%256;
                sendPacket[80] = (err%(256*256))/256;
                sendPacket[81] = err%256;

                //组包添加模型-数据
                g_infoWidget().setLabelText(g_myLan().model_file_upgrading);
                g_infoWidget().delayShow();

                QString proecessStrDisp;
                int hasSendLen = 0;
                unsigned char* tmpPacket = new unsigned char[PACKET_LEN+3];


                g_Udp.pushUdpCmdData(CMD_UDP_IPC_MODEL_ADD_HEAD, true, m_nChainIndex, 0xff, MAX_IPC_MODEL_NAME_LENGTH+18, sendPacket);

                //                myFlow.msleep(200);

                for(int ii = 0; ii < packetTotal; ii++)
                {
                    memset(tmpPacket, 0, PACKET_LEN+3);

                    tmpPacket[0] = 0;
                    tmpPacket[1] = ii/256;
                    tmpPacket[2] = ii%256;

                    if(dataLen-hasSendLen >= PACKET_LEN) //至少还剩一个整包
                    {
                        memcpy(tmpPacket+3, tmpModeldata+hasSendLen, PACKET_LEN);
                        g_Udp.pushUdpCmdData(CMD_UDP_IPC_MODEL_ADD_DATA, true, m_nChainIndex, 0xff, PACKET_LEN+3, tmpPacket);
                        hasSendLen += PACKET_LEN;
                    }
                    else    //剩不到一个整包
                    {
                        memcpy(tmpPacket+3, tmpModeldata+hasSendLen, dataLen-hasSendLen);
                        g_Udp.pushUdpCmdData(CMD_UDP_IPC_MODEL_ADD_DATA, true, m_nChainIndex, 0xff, PACKET_LEN+3, tmpPacket);
                    }

                    if(ii%10 == 0)
                    {
                        proecessStrDisp = QString::number((ii*100)/packetTotal)+"%";
                        ui->progressBar->setValue((ii*100)/packetTotal);
                        ui->progressBar->setFormat(proecessStrDisp);
                    }

                    //                    if((ii > 0)&&(ii%3 == 0))
                    {
                        myFlow.msleep(2);
                    }
                }
                myFlow.msleep(100);

                proecessStrDisp = "99.9%";
                ui->progressBar->setValue(99.9);
                ui->progressBar->setFormat(proecessStrDisp);

                delete[] tmpPacket;
                delete[] tmpModeldata;

                myFlow.sleep(2);

                //上位机数据发完恢复心跳
                struGsh.bUpdateState = 0;
                int upgradeResult[MAX_VIEW][MAX_VIEW_UNIT_USE];
                for(int ii = 0; ii < 8; ii++)
                {
                    memset(upgradeResult, 0, sizeof(upgradeResult));
                    err = 0;
                    myFlow.sleep(5);
                    for(j = 0; j < struCnfg.struLayerInfo[0].nViewChainTotal; j++)
                    {
                        if(struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j] && struCnfg.nXsystemBoard[j])
                        {
                            for(k = 0; k <struCnfg.struLayerInfo[0].nViewUnitTotal[j]; k++)
                            {
                                upgradeResult[j][k]=3;

                                if(struIpcShare.struIpcInfo[j][k].modelUploadStat != 3)
                                {
                                    err = err + 1;
                                    upgradeResult[j][k]=4;
                                    continue;
                                }
                            }

                        }
                    }
                    if(err == 0)
                        goto end1;
                }
end1:
                switch ( err) {
                case 0:
                    proecessStrDisp = "100%";
                    ui->progressBar->setValue(100);
                    ui->progressBar->setFormat(proecessStrDisp);
                    ui->ipc1resultlabel->setText(g_myLan().upgrade_success);
                    break;
                default:  //非0表示有相机版升级失败
                    proecessStrDisp = "0%";
                    ui->progressBar->setValue(0);
                    ui->progressBar->setFormat(proecessStrDisp);
                    ui->ipc1resultlabel->setText(g_myLan().model_file_upgrade_fail);
                    break;
                }

                QString showInfoStr("");
                for(j = 0; j < struCnfg.struLayerInfo[0].nViewChainTotal; j++)
                {
                    if(struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j] && struCnfg.nXsystemBoard[j])
                    {
                        for(k = 0; k <struCnfg.struLayerInfo[0].nViewUnitTotal[j]; k++)
                        {
                            if(upgradeResult[j][k]==0) continue;
                            if(upgradeResult[j][k]==3)
                            {
                                showInfoStr = (viewList[j] + QString::number(k+1)+g_myLan().ids_upgradeSucceeded);
                                appendUpdateResult(showInfoStr);
                            }
                            if(upgradeResult[j][k]==4)
                            {
                                showInfoStr = (viewList[j] + QString::number(k+1)+g_myLan().ids_upgradeFailed);
                                appendUpdateResult(showInfoStr, false);
                            }
                        }
                    }
                }
                g_infoWidget().hide();
                ui->ipc1resultlabel->setText("");
                updateAll();
                return;
            }
        }
        else
        {
            int dataLen = 0, configLen = 0;
            int packetTotal = 1;
            int i,j,k, err=0,tempflag=0;

            int factorySetflag[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT];

            memset(factorySetflag, 0, sizeof(factorySetflag));

            ui->progressBar->setValue(0);
            ui->progressBar->setFormat(QString("0%"));

            ui->startUpgradeBtn->setEnabled(false);
            ui->ipc1resultlabel->setText(g_myLan().ids_upgrading);
            m_count = 0;
            if (!checkUpdateFile())
            {
                g_infoWidget().setLabelText(g_myLan().model_file_mismatch+"-100");
                g_infoWidget().delayShow();
                myFlow.sleep(2);
                g_infoWidget().hide();
                return;
            }

            unsigned char sendPacket1[MAX_IPC_MODEL_NAME_LENGTH+18];
            memset(sendPacket1, 0, MAX_IPC_MODEL_NAME_LENGTH);


            g_Udp.pushUdpCmdData(CMD_UDP_IPC_SET_FACTORY_MODE, true, m_nChainIndex, 0xff, MAX_IPC_MODEL_NAME_LENGTH, sendPacket1);
            myFlow.sleep(3);

            for(j = 0; j < struCnfg.struLayerInfo[0].nViewChainTotal; j++)
            {
                if(struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j] && struCnfg.nXsystemBoard[j])
                {
                    for(k = 0; k <struCnfg.struLayerInfo[0].nViewUnitTotal[j]; k++)
                    {
                        myFlow.getIpcInfo(false,j ,k);
                        myFlow.sleep(3);
                        if(QString::fromUtf8(struIpcShare.struIpcInfo[j][k].version).contains(QString("Fac")))
                        {
                            factorySetflag[0][j][k] = 1;
                            appendUpdateResult(viewList.at(j)+QString::number(k+1)+g_myLan().ids_setFactorySucceeded);
                        }
                        else
                            appendUpdateResult(viewList.at(j)+QString::number(k+1)+g_myLan().ids_setFactoryFailed, false);
                    }
                }
            }

            myFlow.sleep(7);

            QFile fileData(modeFileDataWholeName);

            if (fileData.open(QIODevice::ReadOnly))
            {
                dataLen = fileData.size();
                qDebug("111 modeFileDataWholeName is %s,filesize is %d", modeFileDataWholeName.toLatin1().data(),dataLen);

                //停止心跳
                struGsh.bUpdateState = 1;
                myFlow.sleep(6);
                //清升级状态标识
                for(j = 0; j < struCnfg.struLayerInfo[0].nViewChainTotal; j++)
                {
                    if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;

                    for(k = 0; k <struCnfg.struLayerInfo[0].nViewUnitTotal[j]; k++)
                    {
                        struIpcShare.struIpcInfo[j][k].modelUploadStat = 0;
                    }
                }
                char* tmpModeldata = new char[dataLen];
                QDataStream dsData(&fileData);
                dsData.readRawData(tmpModeldata, dataLen);
                fileData.close();

                //组包添加模型-头
                unsigned char sendPacket[MAX_IPC_MODEL_NAME_LENGTH+18];
                struIpcShare.struIpcReqStauts[struGsh.nView][struGsh.nUnit].modelAddDataReq = IPC_REQ_RETURN_NULL;

                memset(sendPacket, 0, MAX_IPC_MODEL_NAME_LENGTH);
                memcpy(sendPacket, modeFileDataName.toLatin1().data(), modeFileDataName.length());
                sendPacket[64] = dataLen/(256*256*256);
                sendPacket[65] = (dataLen/(256*256))%256;
                sendPacket[66] = (dataLen%(256*256))/256;
                sendPacket[67] = dataLen%256;

                sendPacket[68] = 1;
                sendPacket[69] = 0;
                sendPacket[70] = sendPacket[64];
                sendPacket[71] = sendPacket[65];
                sendPacket[72] = sendPacket[66];
                sendPacket[73] = sendPacket[67];

                sendPacket[74] = PACKET_LEN/256;
                sendPacket[75] = PACKET_LEN%256;

                packetTotal = (dataLen%PACKET_LEN == 0) ? dataLen/PACKET_LEN : dataLen/PACKET_LEN+1;
                sendPacket[76] = packetTotal/256;
                sendPacket[77] = packetTotal%256;

                for(int i = 0; i < dataLen;i++)
                {
                    err += tmpModeldata[i];
                }
                sendPacket[78] = err/(256*256*256);
                sendPacket[79] = (err/(256*256))%256;
                sendPacket[80] = (err%(256*256))/256;
                sendPacket[81] = err%256;

                //组包添加模型-数据
                g_infoWidget().setLabelText(g_myLan().model_file_upgrading);
                g_infoWidget().delayShow();

                QString proecessStrDisp;
                int hasSendLen = 0;
                unsigned char* tmpPacket = new unsigned char[PACKET_LEN+3];

                for(j = 0; j < struCnfg.struLayerInfo[0].nViewChainTotal; j++)
                {
                    if(struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j] && struCnfg.nXsystemBoard[j])
                    {
                        for(k = 0; k <struCnfg.struLayerInfo[0].nViewUnitTotal[j]; k++)
                        {
                            tempflag = tempflag + factorySetflag[0][j][k];
                        }
                    }
                }

                if(tempflag != 0)
                {

                    appendUpdateResult(g_myLan().ids_upgrade+QString("..."));

                    g_Udp.pushUdpCmdData(CMD_UDP_IPC_MODEL_ADD_HEAD, true, m_nChainIndex, 0xff, MAX_IPC_MODEL_NAME_LENGTH+18, sendPacket);

                    for(int ii = 0; ii < packetTotal; ii++)
                    {
                        memset(tmpPacket, 0, PACKET_LEN+3);

                        tmpPacket[0] = 0;
                        tmpPacket[1] = ii/256;
                        tmpPacket[2] = ii%256;

                        if(dataLen-hasSendLen >= PACKET_LEN) //至少还剩一个整包
                        {
                            memcpy(tmpPacket+3, tmpModeldata+hasSendLen, PACKET_LEN);
                            g_Udp.pushUdpCmdData(CMD_UDP_IPC_MODEL_ADD_DATA, true, m_nChainIndex, 0xff, PACKET_LEN+3, tmpPacket);
                            hasSendLen += PACKET_LEN;
                        }
                        else    //剩不到一个整包
                        {
                            memcpy(tmpPacket+3, tmpModeldata+hasSendLen, dataLen-hasSendLen);
                            g_Udp.pushUdpCmdData(CMD_UDP_IPC_MODEL_ADD_DATA, true, m_nChainIndex, 0xff, PACKET_LEN+3, tmpPacket);
                        }

                        if(ii%10 == 0)
                        {
                            proecessStrDisp = QString::number((ii*100)/packetTotal)+"%";
                            ui->progressBar->setValue((ii*100)/packetTotal);
                            ui->progressBar->setFormat(proecessStrDisp);
                        }
                        myFlow.msleep(2);
                    }
                    myFlow.msleep(100);
                }

                proecessStrDisp = "99.9%";
                ui->progressBar->setValue(99.9);
                ui->progressBar->setFormat(proecessStrDisp);

                delete[] tmpPacket;
                delete[] tmpModeldata;

                myFlow.sleep(2);
                //上位机数据发完恢复心跳
                struGsh.bUpdateState = 0;

                int upgradeResult[MAX_VIEW][MAX_VIEW_UNIT_USE];
                memset(upgradeResult, 0, sizeof(upgradeResult));

                if(tempflag == 0)
                    goto end2;

                for(int ii = 0; ii < 24; ii++)
                {
                    memset(upgradeResult, 0, sizeof(upgradeResult));
                    err = 0;
                    myFlow.sleep(5);
                    for(j = 0; j < struCnfg.struLayerInfo[0].nViewChainTotal; j++)
                    {
                        if(struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j] && struCnfg.nXsystemBoard[j])
                        {
                            for(k = 0; k <struCnfg.struLayerInfo[0].nViewUnitTotal[j]; k++)
                            {

                                if(factorySetflag[0][j][k] == 0) continue;

                                upgradeResult[j][k]=3;

                                if(QString::fromUtf8(struIpcShare.struIpcInfo[j][k].version).contains(QString("Fac")))
                                {
                                    err = err + 1;
                                    upgradeResult[j][k]=4;
                                    continue;
                                }
                            }
                        }
                    }
                    if(err == 0)
                        goto end2;
                }
end2:
                switch ( err) {
                case 0:
                    proecessStrDisp = "0%";
                    ui->progressBar->setValue(100);
                    ui->progressBar->setFormat(proecessStrDisp);
                    //                ui->ipc1resultlabel->setText(g_myLan().upgrade_success);
                    break;
                default:  //非0表示有相机版升级失败
                    proecessStrDisp = "0%";
                    ui->progressBar->setValue(0);
                    ui->progressBar->setFormat(proecessStrDisp);
                    //                ui->ipc1resultlabel->setText(g_myLan().model_file_upgrade_fail);
                    break;
                }

                QString showInfoStr("");
                for(j = 0; j < struCnfg.struLayerInfo[0].nViewChainTotal; j++)
                {
                    if(struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j] && struCnfg.nXsystemBoard[j])
                    {
                        for(k = 0; k <struCnfg.struLayerInfo[0].nViewUnitTotal[j]; k++)
                        {

                            if(factorySetflag[0][j][k] == 0) continue;
                            if(upgradeResult[j][k]==0) continue;
                            if(upgradeResult[j][k]==3)
                            {
                                showInfoStr = (viewList[j] + QString::number(k+1)+g_myLan().ids_upgradeSucceeded);
                                appendUpdateResult(showInfoStr);
                            }
                            if(upgradeResult[j][k]==4)
                            {
                                showInfoStr = (viewList[j] + QString::number(k+1)+g_myLan().ids_upgradeFailed);
                                appendUpdateResult(showInfoStr, false);
                            }
                        }
                    }
                }
                appendUpdateResult(g_myLan().ids_upgrade+g_myLan().Completed);
                ui->ipc1resultlabel->setText(" ");
                g_infoWidget().hide();
                updateAll();
                return;
            }
        }

    }
}


void upgradeModelWidget::timerEvent(QTimerEvent *event)
{    
    //    killTimer(event->timerId());
    //    if(id == timer_interface_wait_toFactory){
    //        myFlow.resetMachineConfigRG();
    //        id = timer_ask_interface_version ;
    //        startTimer(3000);

    //    }
}


void upgradeModelWidget::on_checkFileBtn_clicked()
{
    if(m_functionflag == 0)
    {
        QDir myDir;
#ifdef Q_OS_UNIX
//        if(myMqttMsgParaseThread->m_updateFileType != myMqttMsgParaseThread->UPDATE_IPCMODEL)
        if(1)
        {
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
            myDir.setPath(path+"zkmodels");
        }
        else
        {
            myDir.setPath("/sdcard/udisk");
        }
#else
        myDir.setPath(qApp->applicationDirPath()+"/zkmodels");
#endif

        if (!myDir.exists()) {
            qWarning("Cannot find the /udisk directory");
            return;
        }
        ui->FileListWidget->clear();
        QStringList filters;

        //    filters << "lib*"<<"*.dmb"<<"*.tgz"<<"ACS*";
        //    myDir.setNameFilters(filters);
        QFileInfoList list;
        list = myDir.entryInfoList(myDir.filter() | QDir::NoDotAndDotDot);

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
            if(filename.contains("(")|| filename.contains(")"))
                continue;
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
    else
    {
        QDir myDir;
#ifdef Q_OS_UNIX
        //    if(myMqttMsgParaseThread->m_updateFileType != myMqttMsgParaseThread->UPDATE_IPCMODEL)
        //    {
        ui->startUpgradeBtn->setEnabled(false);
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
        if(m_functionflag == 1)
            myDir.setPath(path+"zkmodels");
        else
            myDir.setPath(path+"bin");
        //    }
        //    else
        //    {
        //        myDir.setPath("/sdcard/udisk");
        //    }
#else
        if(m_functionflag == 1)
            myDir.setPath(qApp->applicationDirPath()+"/zkmodels");
        else
            myDir.setPath(qApp->applicationDirPath()+"/bin");
#endif

        if (!myDir.exists()) {
            qWarning("Cannot find the /udisk directory");
            //            g_infoWidget().setLabelText(g_myLan().msg_zkmodels_not_exist);
            //            g_infoWidget().showSecs();
            return;
        }
        ui->FileListWidget->clear();
        QStringList filters;

        filters << "*.bin";
        myDir.setNameFilters(filters);
        QFileInfoList list;
        list = myDir.entryInfoList(filters);

        int m_nFileNum = list.size();
        if (m_nFileNum < 1) {
            ui->startUpgradeBtn->setEnabled(false);
            g_infoWidget().setLabelText(g_myLan().No_file_detected);
            g_infoWidget().showSecs();
        } else {
            ui->startUpgradeBtn->setEnabled(true);
        }

        QString filename;
        float filesize;
        QDateTime filedate;
        QString str;
        int dataLen;

        filelist.clear();

        for(int i=0;i< list.size();i++)
        {


            filename = list.at(i).fileName();
            if(filename.contains("(")|| filename.contains(")"))
                continue;
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

            dataLen = list.at(i).size();
            if(m_functionflag == 1 && dataLen < 5017753)
            {
                filelist.append(str);
            }
            else if(m_functionflag == 2 && dataLen > 5017753)
            {
                filelist.append(str);
            }
        }

        ui->FileListWidget->insertItems(0, filelist);
        ui->FileListWidget->setCurrentRow(0);

    }
}

void upgradeModelWidget::updateAll()
{
    updateChainCombobox();
    //    updateChuteCombobox();
}

void upgradeModelWidget::updateChainCombobox()
{
    QStringList list;
    //   list<<g_myLan().front_rear_view;
    for(int i = 0;i< struCnfg.struLayerInfo[0].nViewChainTotal;i++)
    {
        //       if(struCnfg.struLayerInfo[0].nViewIsUsed[i])
        {
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
                //           case 4:
                //               list<<g_myLan().upper_layer + g_myLan().ids_fm;
                //               break;
                //           case 5:
                //               list<<g_myLan().upper_layer + g_myLan().ids_bm;
                //               break;
                //           case 6:
                //               list<<g_myLan().upper_layer + g_myLan().ids_fs;
                //               break;
                //           case 7:
                //               list<<g_myLan().upper_layer + g_myLan().ids_bs;
                //               break;
            default:
                break;
            }
        }
    }
    ui->comboBox_chain->clear();
    ui->comboBox_chain->addItems(list);
    ui->comboBox_chain->setCurrentIndex(m_nChainIndex);
}

//void upgradeModelWidget::updateChuteCombobox()
//{
//    QStringList list;
//    list<<g_myLan().ids_allChannel;

//    for(int i = 1;i< struCnfg.nChuteTotal+1;i++){
//        list<<QString::number(i);
//    }

//    ui->comboBox_chute->clear();
//    ui->comboBox_chute->addItems(list);
//    ui->comboBox_chute->setCurrentIndex(m_nChuteIndex);
//}

