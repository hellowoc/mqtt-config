#include "sysversioninfowidget.h"
#include "ui_sysversioninfowidget.h"
#include "global.h"
#include "myfscheck.h"

SysVersionInfoWidget::SysVersionInfoWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::SysVersionInfoWidget)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setColumnWidth(0,250);
    ui->tableWidget->setColumnWidth(1,50);
    ui->tableWidget->setColumnWidth(2,200);
    ui->tableWidget->setColumnWidth(3,100);
    ui->tableWidget->setColumnWidth(4,150);
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setFocusPolicy(Qt::NoFocus);
    ui->tableWidget->setShowGrid(false);

    QHeaderView *header = ui->tableWidget->verticalHeader();
    header->setHidden(true);// 隐藏行号

    QHeaderView *horiHeader = ui->tableWidget->horizontalHeader();
    horiHeader->setFixedHeight(38);
    horiHeader->setSectionResizeMode(QHeaderView::Stretch);
    m_pMessage = NULL;

    shottimer = new QTimer;
    connect(shottimer,SIGNAL(timeout()),this, SLOT(getscreenshot()));

}

SysVersionInfoWidget::~SysVersionInfoWidget()
{
    delete ui;
}

void SysVersionInfoWidget::showPage(bool bshow)
{
    if(bshow){
        updateVersionInfo();
        updateTableWidget();

        if( struGsh.bScreenShotStatus ==1)
        {
            shottimer->start(1000);
        }
    }
}

void SysVersionInfoWidget::retranslateUiWidget()
{
    ui->search->setText(g_myLan().recheck);
}

void SysVersionInfoWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_VERSION){
#ifdef Q_OS_UNIX
//        myMqttMsgParaseThread->n_uploadPartsStatusFlag = 1;
#endif
        g_infoWidget().hide();
        updateVersionInfo();
        updateTableWidget();
    }
}

void SysVersionInfoWidget::getscreenshot()
{
    shottimer->stop();
    {
        int count = ui->tableWidget->rowCount()-1;
        int countid = 0;
        while(countid <= count)
        {
            ui->tableWidget->scrollTo(ui->tableWidget->model()->index(countid,0)); // 滑动滚轮下拉
            myFlow.sleep(1);
            myFlow.saveScreenshot();

            countid += 16;
        }

        ui->tableWidget->scrollTo(ui->tableWidget->model()->index(count,0));
        myFlow.sleep(1);
        myFlow.saveScreenshot();

        struGsh.bScreenShotStatus = 0;
    }
}

void SysVersionInfoWidget::updateVersionInfo()
{
    m_verinfoVec.clear();
    int state;
    QString namestr,valuestr,modestr,svnstr;
    QString m_type, m_typeIpc;
    QString m_mode, m_modeIpc;

    QString namestrIpc,valuestrIpc,modestrIpc,svnstrIpc;

    namestr = QString("Screen Version");
    state = 0;
    valuestr = QString(SCREEN_VERSION);
    modestr = "-";
    svnstr = "0";
#ifdef Q_OS_UNIX
    //    svnstr =  QString("%1-%2%3-%4%5%6").arg(struGsh.struMemoryStatus.nCurRunLevel).arg(struGsh.struMemoryStatus.nSDStatus[0]).arg(struGsh.struMemoryStatus.nSDStatus[1])\
    .arg(struGsh.struMemoryStatus.nEMMCStatus[0]).arg(struGsh.struMemoryStatus.nEMMCStatus[1]).arg(struGsh.struMemoryStatus.nEMMCStatus[2]);

    QString runstat,sdStat,emmcStat;
    runstat = QString("%1%2%3").arg(myfs.struMemoryStatus.nCurRunLevel[0])
            .arg(myfs.struMemoryStatus.nCurRunLevel[1]).arg(myfs.struMemoryStatus.nCurRunLevel[2]);
    sdStat = QString("%1%2'%3%4").arg(myfs.struMemoryStatus.nSDMountStatus[0])
            .arg(myfs.struMemoryStatus.nSDMountStatus[1])
            .arg(myfs.struMemoryStatus.nSDUsedStatus[0])
            .arg(myfs.struMemoryStatus.nSDUsedStatus[1]);
    emmcStat = QString("%1%2%3'%4%5%6").arg(myfs.struMemoryStatus.nEMMCMountStatus[0])
            .arg(myfs.struMemoryStatus.nEMMCMountStatus[1])
            .arg(myfs.struMemoryStatus.nEMMCMountStatus[2])
            .arg(myfs.struMemoryStatus.nEMMCUsedStatus[0])
            .arg(myfs.struMemoryStatus.nEMMCUsedStatus[1])
            .arg(myfs.struMemoryStatus.nEMMCUsedStatus[2]);
    svnstr = QString("%1-%2-%3").arg(runstat).arg(sdStat).arg(emmcStat);
#endif

    VersionInfo info(namestr,state,valuestr,modestr,svnstr);
    VersionInfo infoIpc(namestr,state,valuestr,modestr,svnstr);

    info.state = 1;
    m_verinfoVec.push_back(info);

    stru_version version;
    memcpy(&version,&struGsh.struVer,sizeof(version));

    namestr = g_myLan().ids_interfaceBoard;
    state = version.nInterfaceRes;

    m_type = myFlow.getFpgaVersionProduct(version.sInterface[4]);

    if(version.sInterface[2] == 0)
        m_mode = "(F)";
    else
        m_mode = "(U)";

    if(version.nInterfaceRes == -1)
    {
        valuestr = "-";
    }
    else if(version.nInterfaceRes == 0){
        if(version.sInterface[1] < 10)
            valuestr = QString("%1.0%2.%3%4").arg(QString::number(version.sInterface[0])).arg(QString::number(version.sInterface[1])).arg(m_type).arg(m_mode);
        else
            valuestr = QString("%1.%2.%3%4").arg(QString::number(version.sInterface[0])).arg(QString::number(version.sInterface[1])).arg(m_type).arg(m_mode);
    }
    else if(version.nInterfaceRes == 1){
        if(version.sInterface[1] < 10)
            valuestr = QString("%1.0%2.%3%4").arg(QString::number(version.sInterface[0])).arg(QString::number(version.sInterface[1])).arg(m_type).arg(m_mode);
        else
            valuestr = QString("%1.%2.%3%4").arg(QString::number(version.sInterface[0])).arg(QString::number(version.sInterface[1])).arg(m_type).arg(m_mode);
    }
    modestr = QString::number(version.sInterface[4]);
    svnstr = QString::number(version.sInterface[3]);
    info = VersionInfo(namestr,state,valuestr,modestr,svnstr);
    m_verinfoVec.push_back(info);

    for(int n = 0;n < struCnfg.nLayerTotal;n++)
    {
        for(int i = 0;i < MAX_VIEW;i++){
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[i]) continue;
            for(int j = 0;j < struCnfg.struLayerInfo[n].nViewUnitTotal[i];j++){
                namestr = myFlow.getUnitShowName(i,j);
                state = version.nUnitRes[n][i][j];
                m_type = myFlow.getFpgaVersionProduct(version.sUnit[n][i][j][4]);

                if(version.sUnit[n][i][j][2]== 0)
                    m_mode = "(F)";
                else if(version.sUnit[n][i][j][2]== 1)
                    m_mode = "(U1)";
                else if(version.sUnit[n][i][j][2]== 2)
                     m_mode = "(U2)";
                else
                    m_mode = "(U)";

                if(version.nUnitRes[n][i][j] == -1){
                    valuestr = "-";
                }else if(version.nUnitRes[n][i][j] == 0){
                    if(version.sUnit[n][i][j][1] < 10){
                        valuestr = QString("%1.0%2.%3%4").arg(version.sUnit[n][i][j][0]).arg(version.sUnit[n][i][j][1]).arg(m_type).arg(m_mode);
                    }else{
                        valuestr = QString("%1.%2.%3%4").arg(version.sUnit[n][i][j][0]).arg(version.sUnit[n][i][j][1]).arg(m_type).arg(m_mode);
                    }
                }else if(version.nUnitRes[n][i][j] == 1){
                    if(version.sUnit[n][i][j][1] < 10){
                        valuestr = QString("%1.0%2.%3%4").arg(version.sUnit[n][i][j][0]).arg(version.sUnit[n][i][j][1]).arg(m_type).arg(m_mode);
                    }else{
                        valuestr = QString("%1.%2.%3%4").arg(version.sUnit[n][i][j][0]).arg(version.sUnit[n][i][j][1]).arg(m_type).arg(m_mode);
                    }
                }
                if(struCnfg.nUseLowcfgSync && state != -1)
                {
                    valuestr.append(QString("-%1.%2").arg(version.sUnitSync[n][i][j][0]).arg(version.sUnitSync[n][i][j][1]));
                }

                svnstr = QString::number(struGsh.struCameraStatus[n][i][j].comunicationCode1, 16).toUpper();
                modestr = QString::number(version.sUnit[n][i][j][4]);
                info = VersionInfo(namestr,state,valuestr,modestr,svnstr);

                m_verinfoVec.push_back(info);

                if ((struCnfg.nDPMachine == 1)&&(struCnfp.nIsCameraConpress == 0) &&(struCnfp.nViewSensorType[0][i] != SENSOR_G2)
                        &&(struCnfp.nViewSensorType[0][i] != SENSOR_G3))
                {
//                    if(struCnfg.struIpcCameraParams[n][i][j].ipcType != IPC_CAMERA_TYPE_NULL)
//                    {
//                        namestrIpc = namestr+"-IPC";
//                        m_typeIpc = myFlow.getFpgaVersionProduct(version.sUnitIpc[n][i][j][4]);

//                        if(version.sUnitIpc[n][i][j][2]== 0)
//                            m_modeIpc = "(F)";
//                        else
//                            m_modeIpc = "(U)";

//                        if(version.nUnitRes[n][i][j] == -1){
//                            valuestrIpc = "-";
//                        }else if(version.nUnitRes[n][i][j] == 0){
//                            if(version.sUnitIpc[n][i][j][1] < 10){
//                                valuestr = QString("%1.0%2.%3%4").arg(version.sUnitIpc[n][i][j][0]).arg(version.sUnitIpc[n][i][j][1]).arg(m_typeIpc).arg(m_modeIpc);
//                            }else{
//                                valuestr = QString("%1.%2.%3%4").arg(version.sUnitIpc[n][i][j][0]).arg(version.sUnitIpc[n][i][j][1]).arg(m_typeIpc).arg(m_modeIpc);
//                            }
//                        }else if(version.nUnitRes[n][i][j] == 1){
//                            if(version.sUnitIpc[n][i][j][1] < 10){
//                                valuestr = QString("%1.0%2.%3%4").arg(version.sUnitIpc[n][i][j][0]).arg(version.sUnitIpc[n][i][j][1]).arg(m_typeIpc).arg(m_modeIpc);
//                            }else{
//                                valuestr = QString("%1.%2.%3%4").arg(version.sUnitIpc[n][i][j][0]).arg(version.sUnitIpc[n][i][j][1]).arg(m_typeIpc).arg(m_modeIpc);
//                            }
//                        }

//                        svnstr = QString::number(struGsh.struCameraStatus[n][i][j].comunicationCode1, 16).toUpper();
//                        modestrIpc = QString::number(version.sUnit[n][i][j][4]);
//                        infoIpc = VersionInfo(namestrIpc,state,valuestr,modestrIpc,svnstr);
//                        m_verinfoVec.push_back(infoIpc);
//                    }
                }
            }
        }
    }

    for(int n = 0;n < struCnfg.nLayerTotal;n++)
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
                m_type = myFlow.getFpgaVersionProduct(version.sVavle[n][i][j][4]);

                if(version.sVavle[n][i][j][2]== 0)
                    m_mode = "(F)";
                else
                    m_mode = "(U)";

                if(version.nVavleRes[n][i][j] == -1){
                    valuestr = "-";
                }else if(version.nVavleRes[n][i][j] == 0){
                    if(version.sVavle[n][i][j][1] < 10){
                        valuestr = QString("%1.0%2.%3%4").arg(version.sVavle[n][i][j][0]).arg(version.sVavle[n][i][j][1]).arg(m_type).arg(m_mode);
                    }else{
                        valuestr = QString("%1.%2.%3%4").arg(version.sVavle[n][i][j][0]).arg(version.sVavle[n][i][j][1]).arg(m_type).arg(m_mode);
                    }
                }else if(version.nVavleRes[n][i][j] == 1){
                    if(version.sVavle[n][i][j][1] < 10){
                        valuestr = QString("%1.0%2.%3%4").arg(version.sVavle[n][i][j][0]).arg(version.sVavle[n][i][j][1]).arg(m_type).arg(m_mode);
                    }else{
                        valuestr = QString("%1.%2.%3%4").arg(version.sVavle[n][i][j][0]).arg(version.sVavle[n][i][j][1]).arg(m_type).arg(m_mode);
                    }
                }
                svnstr = QString::number(version.sVavle[n][i][j][3]);
                modestr = QString::number(version.sVavle[n][i][j][4]);
                info =VersionInfo(namestr,state,valuestr,modestr,svnstr);
                m_verinfoVec.push_back(info);
            }
        }
    }

    if(struCnfg.bautoplcctrlEnable)
    {
        namestr = QString("%1 %2").arg(g_myLan().ids_autoControlBoard).arg(1);
        state = version.nAutoCtrlRes;
        m_type = myFlow.getFpgaVersionProduct(version.sAutoCtrlBoard[4]);
        if(version.sAutoCtrlBoard[2]== 0)
            m_mode = "(F)";
        else
            m_mode = "(U)";

        if(version.nAutoCtrlRes == -1){
            valuestr = "-";

        }else if(version.nAutoCtrlRes == 0){
            if(version.sAutoCtrlBoard[1] < 10){
                valuestr = QString("%1.0%2.%3%4").arg(version.sAutoCtrlBoard[0]).arg(version.sAutoCtrlBoard[1]).arg(m_type).arg(m_mode);
            }
            else
            {
                valuestr = QString("%1.%2.%3%4").arg(version.sAutoCtrlBoard[0]).arg(version.sAutoCtrlBoard[1]).arg(m_type).arg(m_mode);
            }
        }else if(version.nAutoCtrlRes == 1){
            if(version.sAutoCtrlBoard[1] < 10){
                valuestr = QString("%1.0%2.%3%4").arg(version.sAutoCtrlBoard[0]).arg(version.sAutoCtrlBoard[1]).arg(m_type).arg(m_mode);
            }
            else
            {
                valuestr = QString("%1.%2.%3%4").arg(version.sAutoCtrlBoard[0]).arg(version.sAutoCtrlBoard[1]).arg(m_type).arg(m_mode);
            }
        }
        svnstr = QString::number(version.sAutoCtrlBoard[3]);
        modestr = QString::number(version.sAutoCtrlBoard[4]);
        info = VersionInfo(namestr,state,valuestr,modestr,svnstr);
        m_verinfoVec.push_back(info);
    }


    if(!struCnfg.nAllInOne)
    {
        for(int j = 0;j < struCnfg.nCtrlBoardTotal;j++){

            namestr = QString("%1 %2").arg(g_myLan().ids_controlBoard).arg(j+1);

            state = version.nCtrlRes[j];
            m_type = myFlow.getFpgaVersionProduct(version.sCtrlBoard[j][4]);

            if(version.sCtrlBoard[j][2]== 0)
                m_mode = "(F)";
            else
                m_mode = "(U)";

            if(version.nCtrlRes[j] == -1){
                valuestr = "-";
            }else if(version.nCtrlRes[j] == 0){
                if(version.sCtrlBoard[j][1] < 10){
                    valuestr = QString("%1.0%2.%3%4").arg(version.sCtrlBoard[j][0]).arg(version.sCtrlBoard[j][1]).arg(m_type).arg(m_mode);
                }
                else
                {
                    valuestr = QString("%1.%2.%3%4").arg(version.sCtrlBoard[j][0]).arg(version.sCtrlBoard[j][1]).arg(m_type).arg(m_mode);
                }
            }else if(version.nCtrlRes[j] == 1){
                if(version.sCtrlBoard[j][1] < 10){
                    valuestr = QString("%1.0%2.%3%4").arg(version.sCtrlBoard[j][0]).arg(version.sCtrlBoard[j][1]).arg(m_type).arg(m_mode);
                }
                else
                {
                    valuestr = QString("%1.%2.%3%4").arg(version.sCtrlBoard[j][0]).arg(version.sCtrlBoard[j][1]).arg(m_type).arg(m_mode);
                }
            }
            svnstr = QString::number(version.sCtrlBoard[j][3]);
            modestr = QString::number(version.sCtrlBoard[j][4]);
            info = VersionInfo(namestr,state,valuestr,modestr,svnstr);
            m_verinfoVec.push_back(info);
        }

        if(struCnfg.accChecked)
        {
            for(int j = 0;j < struCnfg.nCtrlBoardTotal;j++){

                namestr = QString("%1 %2").arg(g_myLan().ids_accelerationBoard).arg(j+1);

                state = version.nAccelerationRes[j];
                m_type = myFlow.getFpgaVersionProduct(version.sCtrlBoard[j][4]);

                if(version.sAccelerationBoard[j][0]== 0)
                    m_mode = "(F)";
                else
                    m_mode = "(U)";

                if(version.nAccelerationRes[j] == -1){
                    valuestr = "-";
                }else if(version.nAccelerationRes[j] == 0){
                    if(version.sAccelerationBoard[j][2] < 10){
                        valuestr = QString("%1.0%2.%3%4").arg(version.sAccelerationBoard[j][1]).arg(version.sAccelerationBoard[j][2]).arg(m_type).arg(m_mode);
                    }
                    else
                    {
                        valuestr = QString("%1.%2.%3%4").arg(version.sAccelerationBoard[j][1]).arg(version.sAccelerationBoard[j][2]).arg(m_type).arg(m_mode);
                    }
                }else if(version.nAccelerationRes[j] == 1){
                    if(version.sAccelerationBoard[j][2] < 10){
                        valuestr = QString("%1.0%2.%3%4").arg(version.sAccelerationBoard[j][1]).arg(version.sAccelerationBoard[j][2]).arg(m_type).arg(m_mode);
                    }
                    else
                    {
                        valuestr = QString("%1.%2.%3%4").arg(version.sAccelerationBoard[j][1]).arg(version.sAccelerationBoard[j][2]).arg(m_type).arg(m_mode);
                    }
                }
                svnstr = QString::number(version.sAccelerationBoard[j][3]);
                modestr = QString::number(version.sCtrlBoard[j][4]);
                info = VersionInfo(namestr,state,valuestr,modestr,svnstr);
                m_verinfoVec.push_back(info);
            }
        }

        for(int j = 0;j < struCnfg.viBoardTotalCount;j++){

            namestr = QString("%1 %2").arg("V-I").arg(j+1);

            state = version.nCtrlRes[0];
            m_type = myFlow.getFpgaVersionProduct(version.sVIDetectBoard[j][4]);

            if(version.sVIDetectBoard[j][2]== 0)
                m_mode = "(F)";
            else
                m_mode = "(U)";

            if(version.nCtrlRes[0] == -1){
                valuestr = "-";
            }else if(version.nCtrlRes[0] == 0){
                if(version.sVIDetectBoard[j][1] < 10){
                    valuestr = QString("%1.0%2.%3%4").arg(version.sVIDetectBoard[j][0]).arg(version.sVIDetectBoard[j][1]).arg(m_type).arg(m_mode);
                }
                else
                {
                    valuestr = QString("%1.%2.%3%4").arg(version.sVIDetectBoard[j][0]).arg(version.sVIDetectBoard[j][1]).arg(m_type).arg(m_mode);
                }
            }else if(version.nCtrlRes[0] == 1){
                if(version.sVIDetectBoard[j][1] < 10){
                    valuestr = QString("%1.0%2.%3%4").arg(version.sVIDetectBoard[j][0]).arg(version.sVIDetectBoard[j][1]).arg(m_type).arg(m_mode);
                }
                else
                {
                    valuestr = QString("%1.%2.%3%4").arg(version.sVIDetectBoard[j][0]).arg(version.sVIDetectBoard[j][1]).arg(m_type).arg(m_mode);
                }
            }
            svnstr = QString::number(version.sVIDetectBoard[j][3]);
            modestr = QString::number(version.sVIDetectBoard[j][4]);
            info = VersionInfo(namestr,state,valuestr,modestr,svnstr);
            m_verinfoVec.push_back(info);
        }
    }

    for(int k = 0;k < struCnfg.stuLayerLampCfg.nLampBoardTotal;k++)
    {
        int j = k;
        if(struCnfg.nAllInOne){
            j = k+1;
        }
        namestr = QString("%1 %2").arg(g_myLan().ids_ccsBoard).arg(k+1);
        state = version.nLampRes[j];
        m_type = myFlow.getFpgaVersionProduct(version.sLampBoard[j][4]);

        if(version.sLampBoard[j][2]== 0)
            m_mode = "(F)";
        else
            m_mode = "(U)";
        if(version.nLampRes[j] == -1){
            valuestr = "-";
        }else if(version.nLampRes[j] == 0){
            if(version.sLampBoard[j][1] < 10){
                valuestr = QString("%1.0%2.%3%4").arg(version.sLampBoard[j][0]).arg(version.sLampBoard[j][1]).arg(m_type).arg(m_mode);
            }else{
                valuestr = QString("%1.%2.%3%4").arg(version.sLampBoard[j][0]).arg(version.sLampBoard[j][1]).arg(m_type).arg(m_mode);
            }
        }else if(version.nLampRes[j] == 1){
            if(version.sLampBoard[j][1] < 10){
                valuestr = QString("%1.0%2.%3%4").arg(version.sLampBoard[j][0]).arg(version.sLampBoard[j][1]).arg(m_type).arg(m_mode);
            }else{
                valuestr = QString("%1.%2.%3%4").arg(version.sLampBoard[j][0]).arg(version.sLampBoard[j][1]).arg(m_type).arg(m_mode);
            }
        }
        svnstr = QString::number(version.sLampBoard[j][3]);
        modestr = QString::number(version.sLampBoard[j][4]);
        info = VersionInfo(namestr,state,valuestr,modestr,svnstr);
        m_verinfoVec.push_back(info);
    }
}

void SysVersionInfoWidget::updateTableWidget()
{
    QStringList list;
    list<<g_myLan().ids_board<<g_myLan().list<<g_myLan().software_version<<g_myLan().version_custom<<g_myLan().version_factory;
    ui->tableWidget->setHorizontalHeaderLabels(list);
    ui->tableWidget->setRowCount(m_verinfoVec.size());
    for(int i = 0;i< m_verinfoVec.size();++i){
        ui->tableWidget->setItem(i,0, new QTableWidgetItem(m_verinfoVec.at(i).strName));
        static QString statestr;

        if(m_verinfoVec.at(i).state == -1){
            statestr = "-";
        }else if(m_verinfoVec.at(i).state == 0){
            statestr = "✘";
        }else if(m_verinfoVec.at(i).state == 1){
            statestr = "✔";
        }
        ui->tableWidget->setItem(i,1, new QTableWidgetItem(statestr));
        ui->tableWidget->setItem(i,2, new QTableWidgetItem(m_verinfoVec.at(i).value));
        ui->tableWidget->setItem(i,3, new QTableWidgetItem(m_verinfoVec.at(i).strMode));
        ui->tableWidget->setItem(i,4, new QTableWidgetItem(m_verinfoVec.at(i).strSvn));

        if(m_verinfoVec.at(i).state == -1){
            ui->tableWidget->item(i,1)->setTextColor(QColor(0,0,0));
        }else if(m_verinfoVec.at(i).state == 0){
            ui->tableWidget->item(i,1)->setTextColor(QColor(255,0,0));
        }else if(m_verinfoVec.at(i).state == 1){
            ui->tableWidget->item(i,1)->setTextColor(QColor(0,255,0));
        }
        for(int j= 0;j < 5;j++){
            ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            ui->tableWidget->item(i,j)->setFlags(Qt::ItemIsEnabled);
        }
    }
    ui->tableWidget->hideColumn(3);

    static const QDate buildDate = QLocale( QLocale::English ).toDate( QString(__DATE__).replace("  ", " 0"), "MMM dd yyyy");
    static const QTime buildTime = QTime::fromString(__TIME__, "hh:mm:ss");
    QString buildInfo = QString("%1 : %2 %3").arg("BT").arg(buildDate.toString("yyyy.MM.dd")).arg(buildTime.toString());

    if(paramDelayCode.enable == 0xAA)
    {
        QString strId = QString::fromUtf8(paramDelayCode.product_only_id_cur);
        buildInfo = buildInfo+"("+strId+")";
    }
    else
    {
        int acc = paramDelayCode30Day.product30DayUseAccHour;
        int err = paramDelayCode30Day.fpagCodeErrCount;

        if(paramDelayCode30Day.product30DayUseStartEnable == 0xFA)   //全解过的机器
        {
            buildInfo = buildInfo+QString("-(2088:%1)").arg(err);
        }
        else if(paramDelayCode30Day.product30DayUseStartEnable == 0xAA)   //临时100小时使用期限
        {
            buildInfo = buildInfo+QString("-(100:%1:%2)").arg(acc).arg(err);
        }
    }

#ifdef Q_OS_UNIX
    buildInfo +="\n";
    QProcess m_cmd;
    m_cmd.start("sh",QStringList()<<"-c"<<"cat /proc/sys/kernel/version");
    m_cmd.waitForFinished(-1);
    buildInfo += m_cmd.readAll().trimmed();
    m_cmd.close();
    ui->buildTimeLabel->setText(buildInfo);
#endif

}

#if 1
void SysVersionInfoWidget::on_search_clicked()
{
    //xiewj:色选时不允许查询版本，as此时正在连续采图
    if(struGsh.bSortStart)
    {
        g_infoWidget().setLabelText(g_myLan().msg_turn_off_first);
        g_infoWidget().delayShow();
        myFlow.sleep(2);
        g_infoWidget().hide();
        return;
    }
    g_infoWidget().setLabelText(g_myLan().msg_require_version);
    g_infoWidget().delayShow();

    struCnfg.nBoardStatusAlarm = myFlow.resetCommunication();
}
#endif
#if 0
// 下面用于测试版本查询功能
void SysVersionInfoWidget::on_search_clicked()
{
    int error = 0;
    int count = 0;

    while(error == 0)
    {
        g_infoWidget().setLabelText(g_myLan().getting_version+QString("count is %1").arg(count++));

        g_infoWidget().delayShow();

        struCnfg.nBoardStatusAlarm = myFlow.resetCommunication();

        if((struGsh.struVer.nInterfaceRes == -1)
                ||(struGsh.struVer.nCtrlRes == -1)
                ||(struGsh.struVer.nLampRes == -1))
        {
            error = 1;
        }

        for(int i = 0; i < struCnfg.nLayerTotal ; i++)
        {
            for(int j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
            {
                if(struGsh.struVer.nUnitRes[i][j] == -1)
                {
                    error = 1;
                }
            }
        }

        for(int i = 0; i < struCnfg.nLayerTotal ; i++)
        {
            for(int j = 0; j < struCnfg.struLayerInfo[i].nVavleChainTotal; j++)
            {
                if(struGsh.struVer.nVavleRes[i][j] == -1)
                {
                    error = 1;
                }
            }
        }
        myFlow.sleep(1);
    }

    qDebug("Version test count is %d", count);
}
#endif
