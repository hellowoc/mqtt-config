#include "onekeyupdatefpgawidget.h"
#include "ui_onekeyupdatefpgawidget.h"

OneKeyUpdateFpgaWidget::OneKeyUpdateFpgaWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::OneKeyUpdateFpgaWidget)
{
    ui->setupUi(this);
    pUpdateBoardEnable[0] = ui->interfaceChk;
    pUpdateBoardEnable[1] = ui->frontMasterChk;
    pUpdateBoardEnable[2] = ui->backMasterChk;
    pUpdateBoardEnable[3] = ui->frontSlaveChk;
    pUpdateBoardEnable[4] = ui->backSlaveChk;
    pUpdateBoardEnable[5] = ui->valveChk;
    pUpdateBoardEnable[6] = ui->controlChk;
    pUpdateBoardEnable[7] = ui->lampChk;
    pUpdateBoardEnable[8] = ui->zdjcChk;
    pUpdateBoardEnable[9] = ui->dljcChk;

    pcurVersion[0] = ui->curVersionLbl_1;
    pcurVersion[1] = ui->curVersionLbl_2;
    pcurVersion[2] = ui->curVersionLbl_3;
    pcurVersion[3] = ui->curVersionLbl_4;
    pcurVersion[4] = ui->curVersionLbl_5;
    pcurVersion[5] = ui->curVersionLbl_6;
    pcurVersion[6] = ui->curVersionLbl_7;
    pcurVersion[7] = ui->curVersionLbl_8;
    pcurVersion[8] = ui->curVersionLbl_9;
    pcurVersion[9] = ui->curVersionLbl_10;

    pupdateVersion[0] = ui->updateVersionLbl_1;
    pupdateVersion[1] = ui->updateVersionLbl_2;
    pupdateVersion[2] = ui->updateVersionLbl_3;
    pupdateVersion[3] = ui->updateVersionLbl_4;
    pupdateVersion[4] = ui->updateVersionLbl_5;
    pupdateVersion[5] = ui->updateVersionLbl_6;
    pupdateVersion[6] = ui->updateVersionLbl_7;
    pupdateVersion[7] = ui->updateVersionLbl_8;
    pupdateVersion[8] = ui->updateVersionLbl_9;
    pupdateVersion[9] = ui->updateVersionLbl_10;


    pUpdateBoardEnableSigMapper = new QSignalMapper(this);

    for(int i = 0;i < MAX_BOARD_TYPE;i++){
        pUpdateBoardEnableSigMapper->setMapping(pUpdateBoardEnable[i],i);
        connect(pUpdateBoardEnable[i],SIGNAL(pressed()),pUpdateBoardEnableSigMapper,SLOT(map()));
    }

    connect(pUpdateBoardEnableSigMapper,SIGNAL(mapped(int)),this,SLOT(updateBoardEnable(int)));
    id = 0;
    ui->tarUpdateFileBtn->hide();
    ui->checkFileBtn->hide();
}

OneKeyUpdateFpgaWidget::~OneKeyUpdateFpgaWidget()
{
    delete ui;
}

void OneKeyUpdateFpgaWidget::showPage(bool bshow)
{
    if(bshow)
    {
        for(int i = 0; i < MAX_BOARD_TYPE; i++)
        {
            mUpdateBoardEnable[i] = 0;
            pUpdateBoardEnable[i]->setChecked(0);
        }
        ui->startBtn->setEnabled(true);
        ui->FileListWidget->clear();
        getZipFilePath();
        updateFurVersion();
        updateCurVersion();
    }
}

void OneKeyUpdateFpgaWidget::retranslateUiWidget()
{
    ui->interfaceChk->setText(g_myLan().ids_interfaceBoard);
    ui->frontMasterChk->setText(g_myLan().ids_frontMaster);
    ui->backMasterChk->setText(g_myLan().ids_backMaster);
    ui->frontSlaveChk->setText(g_myLan().ids_frontSlave);
    ui->backSlaveChk->setText(g_myLan().ids_backSlave);
    ui->valveChk->setText(g_myLan().ids_valveBoard);
    ui->controlChk->setText(g_myLan().ids_controlBoard);
    ui->lampChk->setText(g_myLan().ids_ccsBoard);
    ui->zdjcChk->setText(g_myLan().ids_accelerationBoard);
    ui->dljcChk->setText(g_myLan().ids_halogenBoard);
}

void OneKeyUpdateFpgaWidget::receiveMsg(int msg1, int msg2, int msg3)
{

}

void OneKeyUpdateFpgaWidget::updateBoardEnable(int index)
{
    if(mUpdateBoardEnable[index])
    {
        mUpdateBoardEnable[index] = 0;
    }
    else
    {
        mUpdateBoardEnable[index] = 1;
    }
}

void OneKeyUpdateFpgaWidget::updateCurVersion()
{
    m_verinfoVec.clear();
    int state;
    QString namestr,valuestr,modestr,svnstr;
    QString m_type;
    QString m_mode;

    stru_version version;
    memcpy(&version,&struGsh.struVer,sizeof(version));
    VersionInfo info(namestr,state,valuestr,modestr,svnstr);

    // 接口板
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

    // 相机板
    for(int i = 0;i < 4;i++){
        if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[i])
        {
            valuestr = "-";
            svnstr = QString::number(struGsh.struCameraStatus[0][i][0].comunicationCode1, 16).toUpper();
            modestr = QString::number(version.sUnit[0][i][0][4]);
            info = VersionInfo(namestr,state,valuestr,modestr,svnstr);
            m_verinfoVec.push_back(info);
        }
        else
        {
//            namestr = myFlow.getUnitShowName(i,0);
            state = version.nUnitRes[0][i][0];
            m_type = myFlow.getFpgaVersionProduct(version.sUnit[0][i][0][4]);

            if(version.sUnit[0][i][0][2]== 0)
                m_mode = "(F)";
            else
                m_mode = "(U)";

            if(version.nUnitRes[0][i][0] == -1){
                valuestr = "-";
            }else if(version.nUnitRes[0][i][0] == 0){
                if(version.sUnit[0][i][0][1] < 10){
                    valuestr = QString("%1.0%2.%3%4").arg(version.sUnit[0][i][0][0]).arg(version.sUnit[0][i][0][1]).arg(m_type).arg(m_mode);
                }else{
                    valuestr = QString("%1.%2.%3%4").arg(version.sUnit[0][i][0][0]).arg(version.sUnit[0][i][0][1]).arg(m_type).arg(m_mode);
                }
            }else if(version.nUnitRes[0][i][0] == 1){
                if(version.sUnit[0][i][0][1] < 10){
                    valuestr = QString("%1.0%2.%3%4").arg(version.sUnit[0][i][0][0]).arg(version.sUnit[0][i][0][1]).arg(m_type).arg(m_mode);
                }else{
                    valuestr = QString("%1.%2.%3%4").arg(version.sUnit[0][i][0][0]).arg(version.sUnit[0][i][0][1]).arg(m_type).arg(m_mode);
                }
            }

            svnstr = QString::number(struGsh.struCameraStatus[0][i][0].comunicationCode1, 16).toUpper();
            modestr = QString::number(version.sUnit[0][i][0][4]);
            info = VersionInfo(namestr,state,valuestr,modestr,svnstr);
            m_verinfoVec.push_back(info);
        }
    }

    // 阀板
    state = version.nVavleRes[0][0][0];
    m_type = myFlow.getFpgaVersionProduct(version.sVavle[0][0][0][4]);

    if(version.sVavle[0][0][0][2]== 0)
        m_mode = "(F)";
    else
        m_mode = "(U)";

    if(version.nVavleRes[0][0][0] == -1){
        valuestr = "-";
    }else if(version.nVavleRes[0][0][0] == 0){
        if(version.sVavle[0][0][0][1] < 10){
            valuestr = QString("%1.0%2.%3%4").arg(version.sVavle[0][0][0][0]).arg(version.sVavle[0][0][0][1]).arg(m_type).arg(m_mode);
        }else{
            valuestr = QString("%1.%2.%3%4").arg(version.sVavle[0][0][0][0]).arg(version.sVavle[0][0][0][1]).arg(m_type).arg(m_mode);
        }
    }else if(version.nVavleRes[0][0][0] == 1){
        if(version.sVavle[0][0][0][1] < 10){
            valuestr = QString("%1.0%2.%3%4").arg(version.sVavle[0][0][0][0]).arg(version.sVavle[0][0][0][1]).arg(m_type).arg(m_mode);
        }else{
            valuestr = QString("%1.%2.%3%4").arg(version.sVavle[0][0][0][0]).arg(version.sVavle[0][0][0][1]).arg(m_type).arg(m_mode);
        }
    }
    svnstr = QString::number(version.sVavle[0][0][0][3]);
    modestr = QString::number(version.sVavle[0][0][0][4]);
    info =VersionInfo(namestr,state,valuestr,modestr,svnstr);
    m_verinfoVec.push_back(info);

    if(!struCnfg.nAllInOne)
    {
        // 控制板
        namestr = QString("%1 %2").arg(g_myLan().ids_controlBoard).arg(1);
        state = version.nCtrlRes[0];
        m_type = myFlow.getFpgaVersionProduct(version.sCtrlBoard[0][4]);

        if(version.sCtrlBoard[0][2]== 0)
            m_mode = "(F)";
        else
            m_mode = "(U)";

        if(version.nCtrlRes[0] == -1){
            valuestr = "-";
        }else if(version.nCtrlRes[0] == 0){
            if(version.sCtrlBoard[0][1] < 10){
                valuestr = QString("%1.0%2.%3%4").arg(version.sCtrlBoard[0][0]).arg(version.sCtrlBoard[0][1]).arg(m_type).arg(m_mode);
            }
            else
            {
                valuestr = QString("%1.%2.%3%4").arg(version.sCtrlBoard[0][0]).arg(version.sCtrlBoard[0][1]).arg(m_type).arg(m_mode);
            }
        }else if(version.nCtrlRes[0] == 1){
            if(version.sCtrlBoard[0][1] < 10){
                valuestr = QString("%1.0%2.%3%4").arg(version.sCtrlBoard[0][0]).arg(version.sCtrlBoard[0][1]).arg(m_type).arg(m_mode);
            }
            else
            {
                valuestr = QString("%1.%2.%3%4").arg(version.sCtrlBoard[0][0]).arg(version.sCtrlBoard[0][1]).arg(m_type).arg(m_mode);
            }
        }
        svnstr = QString::number(version.sCtrlBoard[0][3]);
        modestr = QString::number(version.sCtrlBoard[0][4]);
        info = VersionInfo(namestr,state,valuestr,modestr,svnstr);
        m_verinfoVec.push_back(info);

        // 灯板
        namestr = QString("%1 %2").arg(g_myLan().ids_ccsBoard).arg(1);
        state = version.nLampRes[0];
        m_type = myFlow.getFpgaVersionProduct(version.sLampBoard[0][4]);

        if(version.sLampBoard[0][2]== 0)
            m_mode = "(F)";
        else
            m_mode = "(U)";
        if(version.nLampRes[0] == -1){
            valuestr = "-";
        }else if(version.nLampRes[0] == 0){
            if(version.sLampBoard[0][1] < 10){
                valuestr = QString("%1.0%2.%3%4").arg(version.sLampBoard[0][0]).arg(version.sLampBoard[0][1]).arg(m_type).arg(m_mode);
            }else{
                valuestr = QString("%1.%2.%3%4").arg(version.sLampBoard[0][0]).arg(version.sLampBoard[0][1]).arg(m_type).arg(m_mode);
            }
        }else if(version.nLampRes[0] == 1){
            if(version.sLampBoard[0][1] < 10){
                valuestr = QString("%1.0%2.%3%4").arg(version.sLampBoard[0][0]).arg(version.sLampBoard[0][1]).arg(m_type).arg(m_mode);
            }else{
                valuestr = QString("%1.%2.%3%4").arg(version.sLampBoard[0][0]).arg(version.sLampBoard[0][1]).arg(m_type).arg(m_mode);
            }
        }
        svnstr = QString::number(version.sLampBoard[0][3]);
        modestr = QString::number(version.sLampBoard[0][4]);
        info = VersionInfo(namestr,state,valuestr,modestr,svnstr);
        m_verinfoVec.push_back(info);

        // 震动检测板
        namestr = QString("%1 %2").arg(g_myLan().ids_accelerationBoard).arg(1);
        state = version.nAccelerationRes[0];
        m_type = myFlow.getFpgaVersionProduct(version.sCtrlBoard[0][4]);

        if(version.sAccelerationBoard[0][0]== 0)
            m_mode = "(F)";
        else
            m_mode = "(U)";

        if(version.nAccelerationRes[0] == -1){
            valuestr = "-";
        }else if(version.nAccelerationRes[0] == 0){
            if(version.sAccelerationBoard[0][2] < 10){
                valuestr = QString("%1.0%2.%3%4").arg(version.sAccelerationBoard[0][1]).arg(version.sAccelerationBoard[0][2]).arg(m_type).arg(m_mode);
            }
            else
            {
                valuestr = QString("%1.%2.%3%4").arg(version.sAccelerationBoard[0][1]).arg(version.sAccelerationBoard[0][2]).arg(m_type).arg(m_mode);
            }
        }else if(version.nAccelerationRes[0] == 1){
            if(version.sAccelerationBoard[0][2] < 10){
                valuestr = QString("%1.0%2.%3%4").arg(version.sAccelerationBoard[0][1]).arg(version.sAccelerationBoard[0][2]).arg(m_type).arg(m_mode);
            }
            else
            {
                valuestr = QString("%1.%2.%3%4").arg(version.sAccelerationBoard[0][1]).arg(version.sAccelerationBoard[0][2]).arg(m_type).arg(m_mode);
            }
        }
        svnstr = QString::number(version.sAccelerationBoard[0][3]);
        modestr = QString::number(version.sCtrlBoard[0][4]);
        info = VersionInfo(namestr,state,valuestr,modestr,svnstr);
        m_verinfoVec.push_back(info);

        // 电流检测板
        namestr = QString("%1 %2").arg(g_myLan().ids_halogenBoard).arg(1);
        state = version.nHalogenRes[0];
        m_type = myFlow.getFpgaVersionProduct(version.sCtrlBoard[0][4]);

        if(version.sHalogenBoard[0][0]== 0)
            m_mode = "(F)";
        else
            m_mode = "(U)";

        if(version.nHalogenRes[0] == -1){
            valuestr = "-";
        }else if(version.nHalogenRes[0] == 0){
            if(version.sHalogenBoard[0][2] < 10){
                valuestr = QString("%1.0%2.%3%4").arg(version.sHalogenBoard[0][1]).arg(version.sHalogenBoard[0][2]).arg(m_type).arg(m_mode);
            }
            else
            {
                valuestr = QString("%1.%2.%3%4").arg(version.sHalogenBoard[0][1]).arg(version.sHalogenBoard[0][2]).arg(m_type).arg(m_mode);
            }
        }else if(version.nHalogenRes[0] == 1){
            if(version.sHalogenBoard[0][2] < 10){
                valuestr = QString("%1.0%2.%3%4").arg(version.sHalogenBoard[0][1]).arg(version.sHalogenBoard[0][2]).arg(m_type).arg(m_mode);
            }
            else
            {
                valuestr = QString("%1.%2.%3%4").arg(version.sHalogenBoard[0][1]).arg(version.sHalogenBoard[0][2]).arg(m_type).arg(m_mode);
            }
        }
        svnstr = QString::number(version.sHalogenBoard[0][3]);
        modestr = QString::number(version.sCtrlBoard[0][4]);
        info = VersionInfo(namestr,state,valuestr,modestr,svnstr);
        m_verinfoVec.push_back(info);
    }

    for(int i =0; i<MAX_BOARD_TYPE; i++)
    {
        pcurVersion[i]->setText(m_verinfoVec.at(i).value);
    }
}

void OneKeyUpdateFpgaWidget::updateFurVersion()
{
    QString furFileDir = updateFileDir+"/";
    QDir allFileDir(furFileDir);
    QStringList allFurFileList = allFileDir.entryList();
    QString furVersion;
    m_furVersionVec.clear();
    m_updateVersionVec.clear();

    for(int i=0; i<MAX_BOARD_TYPE; i++)
    {
        m_furVersionVec.push_back("-");
    }

    for(int i=0; i<MAX_BOARD_TYPE; i++)
    {
        m_updateVersionVec.push_back("-");
    }

    // 接口板
    for(int i=0; i<allFurFileList.size(); i++)
    {
        if(allFurFileList[i].contains("INTERFACE"))
        {
            furVersion = getFurVersion(allFurFileList[i],PCB_interface);
            m_furVersionVec[0]=allFurFileList[i];
        }
    }

    // 相机板
    for(int j=0; j<4; j++)
    {
        if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j])
        {
            furVersion = "-";
            m_furVersionVec.push_back(furVersion);
        }
        else
        {
            for(int i=0; i<allFurFileList.size(); i++)
            {
                if(allFurFileList[i].contains("CAM"))
                {
                    if( (struCnfp.nViewSensorType[0][j] == SENSOR_T1)
                        && (allFurFileList[i].contains("T1")))
                    {
                        if(j == 0)
                        {
                            furVersion = getFurVersion(allFurFileList[i],PCB_front_master);
                        }
                        else if(j == 1)
                        {
                            furVersion = getFurVersion(allFurFileList[i],PCB_back_master);
                        }
                        else if(j == 2)
                        {
                            furVersion = getFurVersion(allFurFileList[i],PCB_front_slave);
                        }
                        else
                        {
                            furVersion = getFurVersion(allFurFileList[i],PCB_back_slave);
                        }

                        m_furVersionVec[j+1]=allFurFileList[i];
                    }
                    else if((struCnfp.nViewSensorType[0][j] == SENSOR_G2)
                             && (allFurFileList[i].contains("G2")))
                    {
                        if(j == 0)
                        {
                            furVersion = getFurVersion(allFurFileList[i],PCB_front_master);
                        }
                        else if(j == 1)
                        {
                            furVersion = getFurVersion(allFurFileList[i],PCB_back_master);
                        }
                        else if(j == 2)
                        {
                            furVersion = getFurVersion(allFurFileList[i],PCB_front_slave);
                        }
                        else
                        {
                            furVersion = getFurVersion(allFurFileList[i],PCB_back_slave);
                        }
                        m_furVersionVec[j+1]=allFurFileList[i];
                    }
//                    else if((struCnfg.struLayerInfo[struGsh.nLayer].nViewSensorType[j] == SENSOR_C1_3)
//                             && (allFurFileList[i].contains("C3")))
//                    {
//                        if(j == 0)
//                        {
//                            furVersion = getFurVersion(allFurFileList[i],PCB_front_master);
//                        }
//                        else if(j == 1)
//                        {
//                            furVersion = getFurVersion(allFurFileList[i],PCB_back_master);
//                        }
//                        else if(j == 2)
//                        {
//                            furVersion = getFurVersion(allFurFileList[i],PCB_front_slave);
//                        }
//                        else
//                        {
//                            furVersion = getFurVersion(allFurFileList[i],PCB_back_slave);
//                        }
//                        m_furVersionVec[j+1]=allFurFileList[i];
//                    }
//                    else if((struCnfg.struLayerInfo[struGsh.nLayer].nViewSensorType[j] == SENSOR_C1_1)
//                             && (allFurFileList[i].contains("C1")))
//                    {
//                        if(j == 0)
//                        {
//                            furVersion = getFurVersion(allFurFileList[i],PCB_front_master);
//                        }
//                        else if(j == 1)
//                        {
//                            furVersion = getFurVersion(allFurFileList[i],PCB_back_master);
//                        }
//                        else if(j == 2)
//                        {
//                            furVersion = getFurVersion(allFurFileList[i],PCB_front_slave);
//                        }
//                        else
//                        {
//                            furVersion = getFurVersion(allFurFileList[i],PCB_back_slave);
//                        }
//                        m_furVersionVec[j+1]=allFurFileList[i];
//                    }
//                    else if((struCnfg.struLayerInfo[struGsh.nLayer].nViewSensorType[j] == SENSOR_C1_2)
//                             && (allFurFileList[i].contains("C2")))
//                    {
//                        if(j == 0)
//                        {
//                            furVersion = getFurVersion(allFurFileList[i],PCB_front_master);
//                        }
//                        else if(j == 1)
//                        {
//                            furVersion = getFurVersion(allFurFileList[i],PCB_back_master);
//                        }
//                        else if(j == 2)
//                        {
//                            furVersion = getFurVersion(allFurFileList[i],PCB_front_slave);
//                        }
//                        else
//                        {
//                            furVersion = getFurVersion(allFurFileList[i],PCB_back_slave);
//                        }
//                        m_furVersionVec[j+1]=allFurFileList[i];
//                    }
                    else if((struCnfp.nViewSensorType[0][j] == SENSOR_D2)
                             && (allFurFileList[i].contains("D2")))
                    {
                        if(j == 0)
                        {
                            furVersion = getFurVersion(allFurFileList[i],PCB_front_master);
                        }
                        else if(j == 1)
                        {
                            furVersion = getFurVersion(allFurFileList[i],PCB_back_master);
                        }
                        else if(j == 2)
                        {
                            furVersion = getFurVersion(allFurFileList[i],PCB_front_slave);
                        }
                        else
                        {
                            furVersion = getFurVersion(allFurFileList[i],PCB_back_slave);
                        }
                        m_furVersionVec[j+1]=allFurFileList[i];
                    }
                }
            }
        }
    }


    // 阀板
    for(int i=0; i<allFurFileList.size(); i++)
    {
        if(allFurFileList[i].contains("VD"))
        {
            furVersion = getFurVersion(allFurFileList[i],PCB_valve);
            m_furVersionVec[5]=allFurFileList[i];
        }
    }

    // 控制板
    for(int i=0; i<allFurFileList.size(); i++)
    {
        if(allFurFileList[i].contains("CONTROL"))
        {
            furVersion = getFurVersion(allFurFileList[i],PCB_control);
            m_furVersionVec[6]=allFurFileList[i];
        }
    }

    // 灯板
    for(int i=0; i<allFurFileList.size(); i++)
    {
        if(allFurFileList[i].contains("LEDDRIVER"))
        {
            furVersion = getFurVersion(allFurFileList[i],PCB_lamp);
            m_furVersionVec[7]=allFurFileList[i];
        }
    }

    // 振动检测板
    for(int i=0; i<allFurFileList.size(); i++)
    {
        if(allFurFileList[i].contains("ZDJC"))
        {
            furVersion = getFurVersion(allFurFileList[i],PCB_control_zd);
            m_furVersionVec[8]=allFurFileList[i];
        }
    }

    // 电流检测板
    for(int i=0; i<allFurFileList.size(); i++)
    {
        if(allFurFileList[i].contains("DLJC"))
        {
            furVersion = getFurVersion(allFurFileList[i],PCB_control_dl);
            m_furVersionVec[9]=allFurFileList[i];
        }
    }

    for(int i =0; i<MAX_BOARD_TYPE; i++)
    {
        pupdateVersion[i]->setText(m_furVersionVec[i]);
    }
}

QString OneKeyUpdateFpgaWidget::getFurVersion(QString fileName,int boardType)
{
    m_updateVersionVec[boardType] = fileName;
    QString furFileName;
    fileName = fileName.right(15);
    if ( fileName.contains("V"))
    {
        QStringList vlist = fileName.split("V");
        QString vTmpStr = vlist.at(1);
        QStringList vTmpLst = vTmpStr.split(".");
        furFileName = "V"+vTmpLst.at(0) +"."+ vTmpLst.at(1);
        m_versionToUpgrade[boardType][0] = QString(vTmpLst.at(0)).toInt();
        m_versionToUpgrade[boardType][1] = QString(vTmpLst.at(1)).toInt();
    }
    else
    {
        furFileName = "-";
    }
    return furFileName;
}

void OneKeyUpdateFpgaWidget::on_tarUpdateFileBtn_clicked()
{

}

void OneKeyUpdateFpgaWidget::getZipFilePath()
{
    QString zipFileName = g_Runtime().getUsbPath() + specificFolder;
    QDir baseDir(zipFileName);
    if (!baseDir.exists()) {
        baseDir.mkpath("."); // 创建目标目录
    }
    QString targetDir = baseDir.filePath(g_Runtime().updateZipFileName);
    targetDir.chop(targetDir.length() - targetDir.lastIndexOf('.'));
    updateFileDir = targetDir;
}

void OneKeyUpdateFpgaWidget::on_checkFileBtn_clicked()
{
    ui->startBtn->setEnabled(true);
    ui->FileListWidget->clear();
    getZipFilePath();
    updateFurVersion();
}

bool OneKeyUpdateFpgaWidget::StringToHexGroup( char *OutHexBuffer, char *InStrBuffer, unsigned int strLength)
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

void OneKeyUpdateFpgaWidget::appendUpdateResult(QString str, int state)
{
    m_count++;
    QString date = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString log =QString::number(m_count)+"    "+date+"    "+str;
    QListWidgetItem *item= new QListWidgetItem(log,ui->FileListWidget);

    if(state == 0) // 失败
    {
        item->setTextColor(QColor(255,0,0));
    }
    else if(state == 1) // 成功
    {
        item->setTextColor(QColor(0,127,252));
    }
    else // 跳过
    {
        item->setTextColor(QColor(128,128,128));
    }
    ui->FileListWidget->scrollToBottom();
}

void OneKeyUpdateFpgaWidget::on_startBtn_clicked()
{
    ui->startBtn->setEnabled(false);
    finalInterface = 0;

    myFlow.switchUserMirror(PCB_interface<<4 |FACTORY_MODE,BOARD_TYPE_INTERFACE,0,0);
    id = timer1_interface_wait_toFactory ;
    startTimer(time_switchMode);
}

void OneKeyUpdateFpgaWidget::timerEvent(QTimerEvent *event)
{
    memset(m_nBoardFactoryOk,0,sizeof(m_nBoardFactoryOk));
    memset(m_nBoardUpdateOk,0,sizeof(m_nBoardUpdateOk));
    killTimer(event->timerId());
    if(id == timer1_interface_wait_toFactory){
        myFlow.resetMachineConfigRG();
        id = timer1_ask_interface_version ;
        startTimer(3000);

    }else if(id == timer1_ask_interface_version){
        mCurBoardType = PCB_interface;
        m_boardAddr = BOARD_TYPE_INTERFACE;

        for (int i = 0; i < 3; i++)
        {
            myFlow.readVersion(m_boardAddr,0);
            myFlow.msleep(500);
            if(boardSwitchFactory(PCB_interface,0,0))
            {
                appendUpdateResult(g_myLan().ids_interfaceBoard+g_myLan().ids_setFactorySucceeded,1);
                id = timer1_goto_next_board ;
                startTimer(2500);
                return;
            }
            else
            {
                appendUpdateResult(g_myLan().ids_interfaceBoard+g_myLan().ids_setFactoryFailed,0);
                return;
            }
        }

    }else if(id == timer1_others_wait_toFactory){
        if(mCurBoardType == PCB_front_master)//前主
        {
            id = timer1_ask_front_master_version;
            myFlow.resetMachineConfigRG();
            myFlow.resetSensorConfigRG();

        }
        else if(mCurBoardType == PCB_back_master)//后主
        {
            id = timer1_ask_back_master_version;
            myFlow.resetMachineConfigRG();
            myFlow.resetSensorConfigRG();

        }
        else if(mCurBoardType == PCB_front_slave)//前辅
        {
            id = timer1_ask_front_slave_version;
            myFlow.resetMachineConfigRG();
            myFlow.resetSensorConfigRG();

        }
        else if(mCurBoardType == PCB_back_slave)//后辅
        {
            id = timer1_ask_back_slave_version;
            myFlow.resetMachineConfigRG();
            myFlow.resetSensorConfigRG();

        }
        else if(mCurBoardType == PCB_valve)//阀板
        {
            id = timer1_ask_valve_version;
            myFlow.resetMachineConfigRG();
            myFlow.resetSensorConfigRG();

        }
        else if(mCurBoardType == PCB_control)//控制板
        {
            id = timer1_ask_control_version;
            myFlow.resetMachineConfigRG();
            myFlow.resetSensorConfigRG();

        }
        else if(mCurBoardType == PCB_lamp)//灯板
        {
            id = timer1_ask_lamp_version;
            myFlow.resetMachineConfigRG();
            myFlow.resetSensorConfigRG();

        }
        else if(mCurBoardType == PCB_control_zd)//振动检测
        {
            id = timer1_ask_zdjc_version;
            myFlow.resetMachineConfigRG();
            myFlow.resetSensorConfigRG();

        }
        else if(mCurBoardType == PCB_control_dl)// 电流检测
        {
            id = timer1_ask_dljc_version;
            myFlow.resetMachineConfigRG();
            myFlow.resetSensorConfigRG();
        }
        startTimer(3000);
        return;
    }else if(id == timer1_ask_front_master_version){
        int k,m,failCount=0;
        if (!struCnfg.struLayerInfo[0].nViewIsUsed[0])
        {
            appendUpdateResult(g_myLan().ids_frontMaster+g_myLan().Pass,2);
            id = timer1_goto_next_board ;
            startTimer(20);
            return;
        }
        else
        {
            for (m =0;m<struCnfg.struLayerInfo[0].nViewUnitTotal[0];m++)
            {
                for(k = 0; k < 10; k++)
                {
                    myFlow.readVersion(m_boardAddr,m+1);
                    myFlow.msleep(500);
                    //轮询每个通道板的版本信息
                    if(boardSwitchFactory(PCB_front_master,mCurBoardType,m))
                    {
                        appendUpdateResult(g_myLan().ids_frontMaster+QString::number(m+1)+g_myLan().ids_setFactorySucceeded,1);
                        m_nBoardFactoryOk[mCurBoardType][m] = 1;
                        break;
                    }
                }
                if (k == 10)
                {
                    failCount++;
                    appendUpdateResult(g_myLan().ids_frontMaster+QString::number(m+1)+g_myLan().ids_setFactoryFailed,0);
                }
                if(failCount < struCnfg.struLayerInfo[0].nViewUnitTotal[0])
                {
                    continue;
                }
                else
                {
                    id = timer1_goto_next_board ;
                    startTimer(20);
                    return;
                }
            }
        }

        id = timer1_tell_upgrade ;
        startTimer(20);
        return;
    }else if(id == timer1_ask_back_master_version){       
        int k,m,failCount=0;

        if (!struCnfg.struLayerInfo[0].nViewIsUsed[1])
        {
            appendUpdateResult(g_myLan().ids_backMaster+g_myLan().Pass,2);
            id = timer1_goto_next_board ;
            startTimer(20);
            return;
        }
        else
        {
            for (m =0;m<struCnfg.struLayerInfo[0].nViewUnitTotal[1];m++)
            {
                for(k = 0; k < 10; k++)
                {
                    myFlow.readVersion(m_boardAddr,m+1);
                    myFlow.msleep(500);
                    //轮询每个通道板的版本信息
                    if(boardSwitchFactory(PCB_back_master,mCurBoardType,m))
                    {
                        appendUpdateResult(g_myLan().ids_backMaster+QString::number(m+1)+g_myLan().ids_setFactorySucceeded,1);
                        m_nBoardFactoryOk[mCurBoardType][m] = 1;
                        break;
                    }
                }
                if (k == 10)
                {
                    failCount++;
                    appendUpdateResult(g_myLan().ids_backMaster+QString::number(m+1)+g_myLan().ids_setFactoryFailed,0);
                }
                if(failCount < struCnfg.struLayerInfo[0].nViewUnitTotal[1])
                {
                    continue;
                }
                else
                {
                    id = timer1_goto_next_board ;
                    startTimer(20);
                    return;
                }
            }
        }
        id = timer1_tell_upgrade ;
        startTimer(20);
        return;
    }else if(id == timer1_ask_front_slave_version){

        int k,m,failCount=0;

        if (!struCnfg.struLayerInfo[0].nViewIsUsed[2])
        {
            appendUpdateResult(g_myLan().ids_frontSlave+g_myLan().Pass,2);
            id = timer1_goto_next_board ;
            startTimer(20);
            return;
        }
        else
        {
            for (m =0;m<struCnfg.struLayerInfo[0].nViewUnitTotal[2];m++)
            {
                for(k = 0; k < 10; k++)
                {
                    myFlow.readVersion(m_boardAddr,m+1);
                    myFlow.msleep(500);
                    //轮询每个通道板的版本信息
                    if(boardSwitchFactory(PCB_front_slave,mCurBoardType,m))
                    {
                        appendUpdateResult(g_myLan().ids_frontSlave+QString::number(m+1)+g_myLan().ids_setFactorySucceeded,1);
                        m_nBoardFactoryOk[mCurBoardType][m] = 1;
                        break;
                    }
                }
                if (k == 10)
                {
                    failCount++;
                    appendUpdateResult(g_myLan().ids_frontSlave+QString::number(m+1)+g_myLan().ids_setFactoryFailed,0);
                }
                if(failCount < struCnfg.struLayerInfo[0].nViewUnitTotal[2])
                {
                    continue;
                }
                else
                {
                    id = timer1_goto_next_board ;
                    startTimer(20);
                    return;
                }
            }
        }
        id = timer1_tell_upgrade ;
        startTimer(20);
        return;
    }else if(id == timer1_ask_back_slave_version){
        int k,m,failCount=0;

        if (!struCnfg.struLayerInfo[0].nViewIsUsed[3])
        {
            appendUpdateResult(g_myLan().ids_backSlave+g_myLan().Pass,2);
            id = timer1_goto_next_board ;
            startTimer(20);
            return;
        }
        else
        {
            for (m =0;m<struCnfg.struLayerInfo[0].nViewUnitTotal[3];m++)
            {
                for(k = 0; k < 10; k++)
                {
                    myFlow.readVersion(m_boardAddr,m+1);
                    myFlow.msleep(500);
                    //轮询每个通道板的版本信息
                    if(boardSwitchFactory(PCB_back_slave,mCurBoardType,m))
                    {
                        appendUpdateResult(g_myLan().ids_backSlave+QString::number(m+1)+g_myLan().ids_setFactorySucceeded,1);
                        m_nBoardFactoryOk[mCurBoardType][m] = 1;
                        break;
                    }
                }
                if (k == 10)
                {
                    failCount++;
                    appendUpdateResult(g_myLan().ids_backSlave+QString::number(m+1)+g_myLan().ids_setFactoryFailed,0);
                }
                if(failCount < struCnfg.struLayerInfo[0].nViewUnitTotal[3])
                {
                    continue;
                }
                else
                {
                    id = timer1_goto_next_board ;
                    startTimer(20);
                    return;
                }
            }
        }

        id = timer1_tell_upgrade ;
        startTimer(20);
        return;
    }else if(id == timer1_ask_valve_version){
        int j,k,m,failCount=0;

        for(j = 0; j < struCnfg.struLayerInfo[0].nVavleChainTotal; j++)
        {
            for (m =0;m<struCnfg.struLayerInfo[0].nVavleBoardTotal[j];m++)
            {
                for(k = 0; k < 10; k++)
                {
                    myFlow.readVersion(m_boardAddr,m+1);
                    myFlow.msleep(500);
                    //轮询每个阀板的版本信息
                    if(boardSwitchFactory(PCB_valve,j+1,m))
                    {
                        appendUpdateResult(g_myLan().ids_valveBoard+QString::number(m+1)+g_myLan().ids_setFactorySucceeded,1);
                        m_nBoardFactoryOk[mCurBoardType][m] = 1;
                        break;
                    }
                }
                if (k == 10)
                {
                    failCount++;
                    appendUpdateResult(g_myLan().ids_valveBoard+QString::number(m+1)+g_myLan().ids_setFactoryFailed,0);
                }
                if(failCount < struCnfg.struLayerInfo[0].nVavleBoardTotal[j])
                {
                    continue;
                }
                else
                {
                    id = timer1_goto_next_board ;
                    startTimer(20);
                    return;
                }
            }
        }

        id = timer1_tell_upgrade ;
        startTimer(20);
        return;
    }else if(id == timer1_ask_control_version){
        int k,m,failCount=0;
        for (m =0;m<struCnfg.nCtrlBoardTotal;m++)
        {
            for(k = 0; k < 10; k++)
            {
                myFlow.readVersion(m_boardAddr,m+1);
                myFlow.msleep(500);
                //轮询每个控制板的版本信息
                if(boardSwitchFactory(PCB_control,1,m))
                {
                    appendUpdateResult(g_myLan().ids_controlBoard+QString::number(m+1)+g_myLan().ids_setFactorySucceeded,1);
                    m_nBoardFactoryOk[mCurBoardType][m] = 1;
                    break;
                }
            }
            if (k == 10)
            {
                failCount++;
                appendUpdateResult(g_myLan().ids_controlBoard+QString::number(m+1)+g_myLan().ids_setFactoryFailed,0);
            }
            if(failCount < struCnfg.nCtrlBoardTotal)
            {
                continue;
            }
            else
            {
                id = timer1_goto_next_board ;
                startTimer(20);
                return;
            }
        }

        id = timer1_tell_upgrade ;
        startTimer(20);
        return;
    }else if(id == timer1_ask_lamp_version){
        int k,m,failCount=0;

        for (m =0;m<struCnfg.stuLayerLampCfg.nLampBoardTotal;m++)
        {
            for(k = 0; k < 10; k++)
            {
                myFlow.readVersion(m_boardAddr,m+1);
                myFlow.msleep(500);
                //轮询每个阀板的版本信息
                if(boardSwitchFactory(PCB_lamp,1,m))
                {
                    appendUpdateResult(g_myLan().ids_ccsBoard+QString::number(m+1)+g_myLan().ids_setFactorySucceeded,1);
                    m_nBoardFactoryOk[mCurBoardType][m] = 1;
                    break;
                }
            }
            if (k == 10)
            {
                failCount++;
                appendUpdateResult(g_myLan().ids_ccsBoard+QString::number(m+1)+g_myLan().ids_setFactoryFailed,0);
            }
            if(failCount < struCnfg.stuLayerLampCfg.nLampBoardTotal)
            {
                continue;
            }
            else
            {
                id = timer1_goto_next_board ;
                startTimer(20);
                return;
            }
        }

        id = timer1_tell_upgrade ;
        startTimer(20);
        return;
    }else if(id == timer1_ask_zdjc_version){
        int k,m,failCount=0;
        for (m =0;m<struCnfg.nCtrlBoardTotal;m++)
        {
            for(k = 0; k < 10; k++)
            {
                myFlow.readVersion(BOARD_TYPE_CONTROL,m+1);
                myFlow.msleep(500);
                //轮询每个控制板的版本信息
                if(boardSwitchFactory(PCB_control_zd,1,m))
                {
                    appendUpdateResult(g_myLan().ids_accelerationBoard+QString::number(m+1)+g_myLan().ids_setFactorySucceeded,1);
                    m_nBoardFactoryOk[mCurBoardType][m] = 1;
                    break;
                }
            }
            if (k == 10)
            {
                failCount++;
                appendUpdateResult(g_myLan().ids_accelerationBoard+QString::number(m+1)+g_myLan().ids_setFactoryFailed,0);
            }
            if(failCount < struCnfg.nCtrlBoardTotal)
            {
                continue;
            }
            else
            {
                id = timer1_goto_next_board ;
                startTimer(20);
                return;
            }
        }

        id = timer1_tell_upgrade ;
        startTimer(20);
        return;
    }else if(id == timer1_ask_dljc_version){
//        int k,m,failCount=0;

//        for (m =0;m<struCnfg.halogenPlateNum;m++)
//        {
//            for(k = 0; k < 10; k++)
//            {
//                myFlow.readVersion(BOARD_TYPE_CONTROL,m+1);
//                myFlow.msleep(500);
//                //轮询每个控制板的版本信息
//                if(boardSwitchFactory(PCB_control_dl,1,m))
//                {
//                    appendUpdateResult(g_myLan().ids_halogenBoard+QString::number(m+1)+g_myLan().ids_setFactorySucceeded,1);
//                    m_nBoardFactoryOk[mCurBoardType][m] = 1;
//                    break;
//                }
//            }
//            if (k == 10)
//            {
//                failCount++;
//                appendUpdateResult(g_myLan().ids_halogenBoard+QString::number(m+1)+g_myLan().ids_setFactoryFailed,0);
//            }
//            if(failCount < struCnfg.halogenPlateNum)
//            {
//                continue;
//            }
//            else
//            {
//                id = timer1_goto_next_board ;
//                startTimer(20);
//                return;
//            }
//        }

//        id = timer1_tell_upgrade ;
//        startTimer(20);
        return;
    }else if(id == timer1_tell_upgrade){
        appendUpdateResult(g_myLan().ids_tellToUpgrade,2);
        if (mCurBoardType == PCB_interface)
        {
            myFlow.tellUpgrade(m_boardAddr,0,PCB_interface<<4|0);
        }
        else if(mCurBoardType == PCB_front_master)
        {
            myFlow.tellUpgrade(m_boardAddr,0,PCB_front_master<<4|0);
        }
        else if(mCurBoardType == PCB_back_master)
        {
            myFlow.tellUpgrade(m_boardAddr,0,PCB_back_master<<4|0);
        }
        else if(mCurBoardType == PCB_front_slave)
        {
            myFlow.tellUpgrade(m_boardAddr,0,PCB_front_slave<<4|0);
        }
        else if(mCurBoardType == PCB_back_slave)
        {
            myFlow.tellUpgrade(m_boardAddr,0,PCB_back_slave<<4|0);
        }
        else if(mCurBoardType == PCB_valve)
        {
            for(int j = 0; j < struCnfg.struLayerInfo[0].nVavleChainTotal; j++)
            {
                for (int m =0;m<struCnfg.struLayerInfo[0].nVavleBoardTotal[j];m++)
                {
                    myFlow.tellUpgrade(m_boardAddr,m,0);
                }
            }
        }
        else if(mCurBoardType == PCB_control)
        {
            for (int m =0;m<struCnfg.nCtrlBoardTotal;m++)
            {
                myFlow.tellUpgrade(m_boardAddr,m,0);
            }
        }
        else if(mCurBoardType == PCB_lamp)
        {
            for (int m =0;m<struCnfg.stuLayerLampCfg.nLampBoardTotal;m++)
            {
                myFlow.tellUpgrade(m_boardAddr,m,0);
            }
        }
        else if(mCurBoardType == PCB_control_zd)
        {
            for (int m =0;m<struCnfg.nCtrlBoardTotal;m++)
            {
                myFlow.tellUpgrade(m_boardAddr,m,0);
            }
        }
        else if(mCurBoardType == PCB_control_dl)
        {
//            for (int m =0;m<struCnfg.halogenPlateNum;m++)
//            {
//                myFlow.tellUpgrade(m_boardAddr,m,0);
//            }
        }
        id = timer1_send_file ;
        startTimer(2500);
        return;
    }else if(id == timer1_send_file){

        for(int i=0;i<3;i++){
            if (myFlow.isNetWorkOk())
                break;
        }
        QString updateFileName = updateFileDir+"/"+m_updateVersionVec[mCurBoardType];
        int isBinFile = 0;
        if (m_updateVersionVec[mCurBoardType].contains(".hex") || m_updateVersionVec[mCurBoardType].contains(".bit"))
        {
            QFile file(updateFileName);
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
            fileSize = getUpdateFileSize();

            qDebug()<<"file size:"<<fileSize;

            char beginData[8] = { 0xB9,0x53,0xB9,0x53,fileSize >> 24,(fileSize >> 16) % 256 ,(fileSize >> 8) % 65536, fileSize % 256 };
            myFlow.myNetWork->udpRestart();

            if(myFlow.myNetWork->udpSendTo(beginData,8) != 8)
            {
                perror("update: udp send error");
                appendUpdateResult(g_myLan().ids_sendFileFailed,0);
                return;
            }

            uint crcRet=0;
            appendUpdateResult(g_myLan().ids_sendData,2);
            int time = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000;

            if(!file.open(QFile::ReadOnly | QFile::Text))
            {
                qDebug() << " Could not open the file for reading";
                appendUpdateResult(g_myLan().ids_sendFileFailed,0);
                return;
            }

            int totalsize =0;
            int bufSize = 1024 ;
            char *sendData = (char*)malloc(bufSize);
//            while (!file.atEnd())
//            {
//               char byte;
//               qint64 bytesRead = file.read(&byte, 1);
//               if (bytesRead > 0) {
//                   // 输出字节值
//                   qDebug() << "Read byte:" << QString::number(static_cast<unsigned char>(byte), 16).toUpper();
//                   return;
//               }
//            }
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
                    appendUpdateResult(g_myLan().ids_sendFileFailed,0);
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
                    appendUpdateResult(g_myLan().ids_sendFileFailed,0);
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
                    appendUpdateResult(g_myLan().ids_sendFileFailed,0);
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
                appendUpdateResult(g_myLan().ids_sendFileFailed,0);
                return;
            }
            free(sendData);
            sendData = NULL;
            file.close();
        }
        else if(m_updateVersionVec[mCurBoardType].contains(".bin")){
            isBinFile = 1;
            FILE *f;

            if((f=fopen(updateFileName.toUtf8().data(), "rb")) != NULL){
                fseek(f, 0, SEEK_END);
                long fileSize = ftell(f);
                int tmpSize = getUpdateFileSize();
                int bufSize = 1024 ;
                int offset =bufSize- fileSize%bufSize;
                int mis = tmpSize-fileSize;

                fileSize = getUpdateFileSize();

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
                    appendUpdateResult(g_myLan().ids_sendFileFailed,0);
                    struGsh.bIsUpgrading = false;
                    return;
                }

                fseek(f, 0, SEEK_SET);
                char *sendData = new char[bufSize];
                int count = 0;
                uint crcRet=0;

                appendUpdateResult(g_myLan().ids_sendData,2);
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
                        appendUpdateResult(g_myLan().ids_sendFileFailed,0);
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
                        appendUpdateResult(g_myLan().ids_sendFileFailed,0);
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
                    appendUpdateResult(g_myLan().ids_sendFileFailed,0);
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
            if((f=fopen(updateFileName.toUtf8().data(), "rb")) != NULL){
                fseek(f, 0, SEEK_END);
                long fileSize = ftell(f);

                int mis = 16-fileSize%16;
                fileSize = getUpdateFileSize();
                if(m_updateVersionVec[mCurBoardType].contains(".sfc")) //22/5/11 16字节对齐，本文件需补８个字节
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
                    appendUpdateResult(g_myLan().ids_sendFileFailed,0);
                    return;
                }

                fseek(f, 0, SEEK_SET);
                int bufSize = 1024 ;
                char *sendData =(char*)malloc(bufSize);
                int count = 0;
                uint crcRet=0;

                appendUpdateResult(g_myLan().ids_sendData,2);
                int time = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000;
                while ((count = fread(sendData, 1, bufSize, f))>0)
                {
                    myFlow.msleep(2);
                    if(count < bufSize)
                    {
                        if(m_updateVersionVec[mCurBoardType].contains(".sfc"))
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
                        appendUpdateResult(g_myLan().ids_sendFileFailed,0);
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
                    appendUpdateResult(g_myLan().ids_sendFileFailed,0);
                    return;
                }
                fclose(f);
                free(sendData);
                sendData = NULL;
            }
        }

        appendUpdateResult(g_myLan().ids_upgrading,2);

        if(finalInterface == 1)
        {
            id = timer1_wait_30secs;
        }
        else
        {
            id = timer1_goto_next_board ;
        }

        if(mCurBoardType == PCB_front_master || mCurBoardType == PCB_back_master || mCurBoardType == PCB_front_slave
                || mCurBoardType == PCB_back_slave || mCurBoardType == PCB_interface)
        {
            if(isBinFile)
            {
                startTimer(120000);
            }
            else
            {
                startTimer(60000);
            }
        }
        else if(mCurBoardType == PCB_valve)
        {
            startTimer(30000);
        }
        else
        {
            startTimer(10000);
        }

        return;
    }
    else if(id == timer1_goto_next_board)
    {
        if(mCurBoardType == PCB_interface)//接口板跳转至前主
        {
            mCurBoardType = PCB_front_master;
            m_boardAddr = BOARD_CAMERA_1_FRONT_MAIN;
            qDebug()<<"mUpdateBoardEnable[1]"<<mUpdateBoardEnable[1];
            if(mUpdateBoardEnable[1] == 1)
            {
                myFlow.switchUserMirror(PCB_front_master<<4|FACTORY_MODE,struCnfg.struLayerInfo[struGsh.nLayer].nViewBoardType[mCurBoardType-1],0,0);
                id = timer1_others_wait_toFactory;
                startTimer(time_switchMode);
                return;
            }
            else
            {
                appendUpdateResult(g_myLan().ids_frontMaster+g_myLan().Pass,2);
                id = timer1_goto_next_board;
                startTimer(20);
                return;
            }

        }
        else if(mCurBoardType == PCB_front_master)//前主跳转至后主
        {
            mCurBoardType = PCB_back_master;
            m_boardAddr = BOARD_CAMERA_1_REAR_MAIN;

            qDebug()<<"mUpdateBoardEnable[2]"<<mUpdateBoardEnable[2];
            if(mUpdateBoardEnable[2] == 1)
            {
                myFlow.switchUserMirror(PCB_back_master<<4|FACTORY_MODE,struCnfg.struLayerInfo[struGsh.nLayer].nViewBoardType[mCurBoardType-1],0,0);
                id = timer1_others_wait_toFactory;
                startTimer(time_switchMode);
                return;
            }
            else
            {
                appendUpdateResult(g_myLan().ids_backMaster+g_myLan().Pass,2);
                id = timer1_goto_next_board;
                startTimer(20);
                return;
            }

        }
        else if(mCurBoardType == PCB_back_master)//后主跳转至前辅
        {
            mCurBoardType = PCB_front_slave;
            m_boardAddr = BOARD_CAMERA_1_FRONT_VICE;

            qDebug()<<"mUpdateBoardEnable[3]"<<mUpdateBoardEnable[3];
            if(mUpdateBoardEnable[3] == 1)
            {
                myFlow.switchUserMirror(PCB_front_slave<<4|FACTORY_MODE,struCnfg.struLayerInfo[struGsh.nLayer].nViewBoardType[mCurBoardType-1],0,0);
                id = timer1_others_wait_toFactory;
                startTimer(time_switchMode);
                return;
            }
            else
            {
                appendUpdateResult(g_myLan().ids_frontSlave+g_myLan().Pass,2);
                id = timer1_goto_next_board;
                startTimer(20);
                return;
            }

        }
        else if(mCurBoardType == PCB_front_slave)//前辅跳转至后辅
        {
            mCurBoardType = PCB_back_slave;
            m_boardAddr = BOARD_CAMERA_1_REAR_VICE;

            qDebug()<<"mUpdateBoardEnable[4]"<<mUpdateBoardEnable[4];
            if(mUpdateBoardEnable[4] == 1)
            {
                myFlow.switchUserMirror(PCB_back_slave<<4|FACTORY_MODE,struCnfg.struLayerInfo[struGsh.nLayer].nViewBoardType[mCurBoardType-1],0,0);
                id = timer1_others_wait_toFactory;
                startTimer(time_switchMode);
                return;
            }
            else
            {
                appendUpdateResult(g_myLan().ids_backSlave+g_myLan().Pass,2);
                id = timer1_goto_next_board;
                startTimer(20);
                return;
            }

        }
        else if(mCurBoardType == PCB_back_slave)//后辅跳转至阀板
        {
            mCurBoardType = PCB_valve;
            m_boardAddr = BOARD_VAVLE_LAYER_1;

            qDebug()<<"mUpdateBoardEnable[5]"<<mUpdateBoardEnable[5];
            if(mUpdateBoardEnable[5] == 1)
            {
                for (int m =0;m<struCnfg.struLayerInfo[0].nVavleBoardTotal[0];m++)
                {
                    myFlow.switchUserMirror(PCB_valve<<4|FACTORY_MODE,BOARD_VAVLE_LAYER_1,m,0);
                }
                id = timer1_others_wait_toFactory;
                startTimer(time_switchMode);
                return;
            }
            else
            {
                appendUpdateResult(g_myLan().ids_valveBoard+g_myLan().Pass,2);
                id = timer1_goto_next_board;
                startTimer(20);
                return;
            }


        }
        else if(mCurBoardType == PCB_valve)//阀板跳转至控制板
        {
            mCurBoardType = PCB_control;
            m_boardAddr = BOARD_TYPE_CONTROL;

            qDebug()<<"mUpdateBoardEnable[6]"<<mUpdateBoardEnable[6];
            if(mUpdateBoardEnable[6] == 1)
            {
                for (int m =0;m<struCnfg.nCtrlBoardTotal;m++)
                {
                    myFlow.switchUserMirror(PCB_control<<4|FACTORY_MODE,BOARD_TYPE_CONTROL,m,0);
                }
                id = timer1_others_wait_toFactory;
                startTimer(time_switchMode);
                return;
            }
            else
            {
                appendUpdateResult(g_myLan().ids_controlBoard+g_myLan().Pass,2);
                id = timer1_goto_next_board;
                startTimer(20);
                return;
            }
        }
        else if(mCurBoardType == PCB_control)//控制板跳转至灯板
        {
            mCurBoardType = PCB_lamp;
            m_boardAddr = BOARD_TYPE_LAMP;

            qDebug()<<"mUpdateBoardEnable[7]"<<mUpdateBoardEnable[7];
            if(mUpdateBoardEnable[7] == 1)
            {
                for (int m =0;m<struCnfg.stuLayerLampCfg.nLampBoardTotal;m++)
                {
                    myFlow.switchUserMirror(PCB_lamp<<4|FACTORY_MODE,BOARD_TYPE_LAMP,m,0);
                }
                id = timer1_others_wait_toFactory;
                startTimer(time_switchMode);
                return;
            }
            else
            {
                appendUpdateResult(g_myLan().ids_ccsBoard+g_myLan().Pass,2);
                id = timer1_goto_next_board;
                startTimer(20);
                return;
            }

        }
        else if(mCurBoardType == PCB_lamp)//灯板跳转至震动检测板
        {
            mCurBoardType = PCB_control_zd;
            m_boardAddr = BOARD_TYPE_CONTROL_ZD;

            qDebug()<<"mUpdateBoardEnable[8]"<<mUpdateBoardEnable[8];
            if(mUpdateBoardEnable[8] == 1)
            {
                for (int m =0;m<struCnfg.nCtrlBoardTotal;m++)
                {
                    myFlow.switchUserMirror(PCB_control_zd<<4|FACTORY_MODE,BOARD_TYPE_CONTROL,m,BOARD_TYPE_CONTROL_ZD);
                }
                id = timer1_others_wait_toFactory;
                startTimer(time_switchMode);
                return;
            }
            else
            {
                appendUpdateResult(g_myLan().ids_accelerationBoard+g_myLan().Pass,2);
                id = timer1_goto_next_board;
                startTimer(20);
                return;
            }

        }
        else if(mCurBoardType == PCB_control_zd)//振动检测板跳转至电流板
        {
            mCurBoardType = PCB_control_dl;
            m_boardAddr = BOARD_TYPE_CONTROL_DL;

            qDebug()<<"mUpdateBoardEnable[9]"<<mUpdateBoardEnable[9];
            if(mUpdateBoardEnable[9] == 1)
            {
                for (int m =0;m<struCnfg.nCtrlBoardTotal;m++)
                {
                    myFlow.switchUserMirror(PCB_control_dl<<4|FACTORY_MODE,BOARD_TYPE_CONTROL,m,BOARD_TYPE_CONTROL_DL);
                }
                id = timer1_others_wait_toFactory;
                startTimer(time_switchMode);
                return;
            }
            else
            {
                appendUpdateResult(g_myLan().ids_halogenBoard+g_myLan().Pass,2);
                mCurBoardType = 100;
                id = timer1_goto_next_board;
                startTimer(20);
                return;
            }

        }
        else
        {
            id = timer1_final_interface;
            startTimer(20);
            return;
        }
    }
    else if(id == timer1_final_interface)
    {
        mCurBoardType = PCB_interface;
        m_boardAddr = BOARD_TYPE_INTERFACE;
        finalInterface = 1;
        if(mUpdateBoardEnable[0] == 1)
        {
            id = timer1_tell_upgrade;
        }
        else
        {
            id = timer1_ask_other_version_after_upgrade;
        }
        startTimer(20);
        return;
    }
    else if (id == timer1_wait_30secs)
    {
        myFlow.resetMachineConfigRG();
        myFlow.resetSensorConfigRG();
        myFlow.msleep(3000);

        id = timer1_ask_interface_version_after_upgrade ;
        startTimer(20);

        return;
    }
    else if (id ==  timer1_ask_interface_version_after_upgrade)
    {
        appendUpdateResult(g_myLan().ids_upgradeResult,1);
        for (int i = 0; i < 3; i++)
        {
            myFlow.readVersion(BOARD_TYPE_INTERFACE,0);
            myFlow.msleep(500);

            if(boardUpdateSuccess(PCB_interface,0,0,0))
            {
                m_nBoardUpdateOk[0][0] = 1;
                appendUpdateResult(g_myLan().ids_interfaceBoard+g_myLan().ids_upgradeSucceeded,1);
                break;
            }
            else
            {
                appendUpdateResult(g_myLan().ids_interfaceBoard+g_myLan().ids_upgradeFailed,0);
            }
        }

        myFlow.resetMachineConfigRG();
        myFlow.resetSensorConfigRG();
        myFlow.msleep(3000);
        id = timer1_ask_other_version_after_upgrade ;
        startTimer(20);
        return;
    }
    else if (id == timer1_ask_other_version_after_upgrade)
    {
        int i,j,k,m;

        // 相机板
        for(j = 0;j < 4;j++)
        {
            if (!struCnfg.struLayerInfo[0].nViewIsUsed[j]) continue;
            if (!mUpdateBoardEnable[j+1]) continue;
            for (m =0;m<struCnfg.struLayerInfo[0].nViewUnitTotal[j];m++)
            {
//                if (m_nBoardFactoryOk[j+1][m])
                {
                    for(k = 0; k < 20; k++)
                    {
                        if(j == 0)
                        {
                            myFlow.readVersion(BOARD_CAMERA_1_FRONT_MAIN,m+1);
                            myFlow.msleep(500);
                            if(boardUpdateSuccess(PCB_front_master,j+1,m,0))
                            {
                                appendUpdateResult(g_myLan().ids_frontMaster+QString::number(m+1)+g_myLan().ids_upgradeSucceeded,1);
                                m_nBoardUpdateOk[j+1][m] = 1;
                                break;
                            }
                        }
                        else if(j == 1)
                        {
                            myFlow.readVersion(BOARD_CAMERA_1_REAR_MAIN,m+1);
                            myFlow.msleep(500);
                            if(boardUpdateSuccess(PCB_back_master,j+1,m,0))
                            {
                                appendUpdateResult(g_myLan().ids_backMaster+QString::number(m+1)+g_myLan().ids_upgradeSucceeded,1);
                                m_nBoardUpdateOk[j+1][m] = 1;
                                break;
                            }
                        }
                        else if(j == 2)
                        {
                            myFlow.readVersion(BOARD_CAMERA_1_FRONT_VICE,m+1);
                            myFlow.msleep(500);
                            if(boardUpdateSuccess(PCB_front_slave,j+1,m,0))
                            {
                                appendUpdateResult(g_myLan().ids_frontSlave+QString::number(m+1)+g_myLan().ids_upgradeSucceeded,1);
                                m_nBoardUpdateOk[j+1][m] = 1;
                                break;
                            }
                        }
                        else
                        {
                            myFlow.readVersion(BOARD_CAMERA_1_REAR_VICE,m+1);
                            myFlow.msleep(500);
                            if(boardUpdateSuccess(PCB_back_slave,j+1,m,0))
                            {
                                appendUpdateResult(g_myLan().ids_backSlave+QString::number(m+1)+g_myLan().ids_upgradeSucceeded,1);
                                m_nBoardUpdateOk[j+1][m] = 1;
                                break;
                            }
                        }
                    }
                    if(k == 20)
                    {
                        if(j == 0)
                        {
                            appendUpdateResult(g_myLan().ids_frontMaster+QString::number(m+1)+g_myLan().ids_upgradeFailed,0);
                        }else if(j == 1)
                        {
                            appendUpdateResult(g_myLan().ids_backMaster+QString::number(m+1)+g_myLan().ids_upgradeFailed,0);
                        }else if(j == 2)
                        {
                            appendUpdateResult(g_myLan().ids_frontSlave+QString::number(m+1)+g_myLan().ids_upgradeFailed,0);
                        }else
                        {
                            appendUpdateResult(g_myLan().ids_backSlave+QString::number(m+1)+g_myLan().ids_upgradeFailed,0);
                        }
                    }
                }
            }
        }

        // 阀板
        if(mUpdateBoardEnable[PCB_valve] == 1)
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
                        if(boardUpdateSuccess(PCB_valve,j+1,m,0))
                        {
                            appendUpdateResult(g_myLan().ids_valveBoard+QString::number(m+1)+g_myLan().ids_upgradeSucceeded,1);
                            m_nBoardUpdateOk[PCB_valve][m] = 1;
                            break;
                        }
                    }
                    if (k == 20)
                    {
                        appendUpdateResult(g_myLan().ids_valveBoard+QString::number(m+1)+g_myLan().ids_upgradeFailed,0);
                    }
                }
            }
        }

        // 控制板
        if(mUpdateBoardEnable[PCB_control] == 1)
        {
            for(m = 0; m < struCnfg.nCtrlBoardTotal; m++)
            {
                for(k = 0; k < 20; k++)
                {
                    myFlow.readVersion(BOARD_TYPE_CONTROL,m+1);
                    myFlow.msleep(500);
                    //轮询每个阀板的版本信息
                    if(boardUpdateSuccess(PCB_control,0,m,0))
                    {
                        appendUpdateResult(g_myLan().ids_controlBoard+QString::number(m+1)+g_myLan().ids_upgradeSucceeded,1);
                        m_nBoardUpdateOk[PCB_control][m] = 1;
                        break;
                    }
                }
                if (k == 20)
                {
                    appendUpdateResult(g_myLan().ids_controlBoard+QString::number(m+1)+g_myLan().ids_upgradeFailed,0);
                }
            }
        }

        // 灯板
        if(mUpdateBoardEnable[PCB_lamp] == 1)
        {
            for(m = 0; m < struCnfg.stuLayerLampCfg.nLampBoardTotal; m++)
            {
                for(k = 0; k < 20; k++)
                {
                    myFlow.readVersion(BOARD_TYPE_LAMP,m+1);
                    myFlow.msleep(500);
                    //轮询每个阀板的版本信息
                    if(boardUpdateSuccess(PCB_lamp,0,m,0))
                    {
                        appendUpdateResult(g_myLan().ids_ccsBoard+QString::number(m+1)+g_myLan().ids_upgradeSucceeded,1);
                        m_nBoardUpdateOk[PCB_lamp][m] = 1;
                        break;
                    }
                }
                if (k == 20)
                {
                    appendUpdateResult(g_myLan().ids_ccsBoard+QString::number(m+1)+g_myLan().ids_upgradeFailed,0);
                }
            }
        }

        // 震动检测板
        if(mUpdateBoardEnable[PCB_control_zd] == 1)
        {
            for(m = 0; m < struCnfg.nCtrlBoardTotal; m++)
            {
                for(k = 0; k < 20; k++)
                {
                    myFlow.readVersion(BOARD_TYPE_CONTROL,m+1);
                    myFlow.msleep(500);
                    //轮询每个阀板的版本信息
                    if(boardUpdateSuccess(PCB_control_zd,0,m,0))
                    {
                        appendUpdateResult(g_myLan().ids_accelerationBoard+QString::number(m+1)+g_myLan().ids_upgradeSucceeded,1);
                        m_nBoardUpdateOk[PCB_control_zd][m] = 1;
                        break;
                    }
                }
                if (k == 20)
                {
                    appendUpdateResult(g_myLan().ids_accelerationBoard+QString::number(m+1)+g_myLan().ids_upgradeFailed,0);
                }
            }
        }


        // 电流检测板
        if(mUpdateBoardEnable[PCB_control_dl] == 1)
        {
//            for(m = 0; m < struCnfg.halogenPlateNum; m++)
//            {
//                for(k = 0; k < 20; k++)
//                {
//                    myFlow.readVersion(BOARD_TYPE_CONTROL,m+1);
//                    myFlow.msleep(500);
//                    //轮询每个阀板的版本信息
//                    if(boardUpdateSuccess(PCB_control_dl,0,m,0))
//                    {
//                        appendUpdateResult(g_myLan().ids_halogenBoard+QString::number(m+1)+g_myLan().ids_upgradeSucceeded,1);
//                        m_nBoardUpdateOk[PCB_control_dl][m] = 1;
//                        break;
//                    }
//                }
//                if (k == 20)
//                {
//                    appendUpdateResult(g_myLan().ids_halogenBoard+QString::number(m+1)+g_myLan().ids_upgradeFailed,0);
//                }
//            }
        }

        if(mUpdateBoardEnable[PCB_interface])
        {
            return;
        }
        else
        {
            myFlow.switchUserMirror(USER_MODE,BOARD_TYPE_INTERFACE,0,0);
            myFlow.msleep(3000); // Factory chg to User need more time
            id = timer1_interface_wait_toUser ;
            startTimer(time_switchMode);
            return;
        }

    }
    else if (id == timer1_interface_wait_toUser)
    {
        myFlow.resetMachineConfigRG();
        myFlow.resetSensorConfigRG();
        myFlow.msleep(3000);
        id = timer1_ask_interface_version_after_others_upgrade ;
        startTimer(20);
        return;
    }

    else if (id == timer1_ask_interface_version_after_others_upgrade)
    {
        for (int i = 0; i < 3; i++)
        {
            myFlow.readVersion(BOARD_TYPE_INTERFACE,0);
            myFlow.msleep(500);
            if (struGsh.struVer.sInterface[2] == 1)
            {
                appendUpdateResult(g_myLan().ids_restoreInterfaceSucceeded,1);
                return;
            }
        }
        appendUpdateResult(g_myLan().ids_restoreInterfaceFailed+","+g_myLan().ids_upgradeFailed,0);
        return;
    }
    else if(id == timer1_stop_update)
    {
        appendUpdateResult(g_myLan().Abort_upgrade,0);
        return;
    }
}

bool OneKeyUpdateFpgaWidget::boardSwitchFactory(int boardIndex,int chainIndex, int chuteIndex)
{
    switch (boardIndex) {
    case PCB_interface:
        if(struGsh.struVer.nInterfaceRes == 1 && struGsh.struVer.sInterface[2] == FACTORY_MODE)
            return true;
        break;
    case PCB_front_master:
        if(struGsh.struVer.nUnitRes[0][0][chuteIndex] == 1 && struGsh.struVer.sUnit[0][0][chuteIndex][0] != 0 && struGsh.struVer.sUnit[0][0][chuteIndex][2] == FACTORY_MODE)
            return true;
        break;
    case PCB_back_master:
        if(struGsh.struVer.nUnitRes[0][1][chuteIndex] == 1 && struGsh.struVer.sUnit[0][1][chuteIndex][0] != 0 && struGsh.struVer.sUnit[0][1][chuteIndex][2] == FACTORY_MODE)
            return true;
        break;
    case PCB_front_slave:
        if(struGsh.struVer.nUnitRes[0][2][chuteIndex] == 1 && struGsh.struVer.sUnit[0][2][chuteIndex][0] != 0 && struGsh.struVer.sUnit[0][2][chuteIndex][2] == FACTORY_MODE)
            return true;
        break;
    case PCB_back_slave:
        if(struGsh.struVer.nUnitRes[0][3][chuteIndex] == 1 && struGsh.struVer.sUnit[0][3][chuteIndex][0] != 0 && struGsh.struVer.sUnit[0][3][chuteIndex][2] == FACTORY_MODE)
            return true;
        break;
    case PCB_valve:
        if(struGsh.struVer.nVavleRes[0][chainIndex-1][chuteIndex] == 1 && struGsh.struVer.sVavle[0][chainIndex-1][chuteIndex][0] != 0 && struGsh.struVer.sVavle[0][chainIndex-1][chuteIndex][2] == FACTORY_MODE)
            return true;
        break;
    case PCB_control:
        if(struGsh.struVer.nCtrlRes[chuteIndex] == 1 && struGsh.struVer.sCtrlBoard[chuteIndex][0] != 0 && struGsh.struVer.sCtrlBoard[chuteIndex][2] == FACTORY_MODE)
            return true;
        break;
    case PCB_lamp:
        if(struGsh.struVer.nLampRes[chuteIndex] == 1 && struGsh.struVer.sLampBoard[chuteIndex][0] != 0 && struGsh.struVer.sLampBoard[chuteIndex][2] == FACTORY_MODE)
            return true;
        break;
    case PCB_control_zd:
        if(struGsh.struVer.nAccelerationRes[chuteIndex] == 1 && struGsh.struVer.sAccelerationBoard[chuteIndex][0] != 0 && struGsh.struVer.sAccelerationBoard[chuteIndex][2] == FACTORY_MODE)
            return true;
        break;
    case PCB_control_dl:
        if(struGsh.struVer.nHalogenRes[chuteIndex] == 1 && struGsh.struVer.sAccelerationBoard[chuteIndex][0] != 0 && struGsh.struVer.sAccelerationBoard[chuteIndex][2] == FACTORY_MODE)
            return true;
        break;
    default:
        return false;
        break;
    }
    return false;
}

bool OneKeyUpdateFpgaWidget::boardUpdateSuccess(int boardIndex, int chainIndex, int chuteIndex, uchar versionToUpdate)
{
    switch (boardIndex) {
    case PCB_interface:
        if(struGsh.struVer.nInterfaceRes == 1 && struGsh.struVer.sInterface[2] == USER_MODE &&
            struGsh.struVer.sInterface[0] == m_versionToUpgrade[PCB_interface][0] &&
            struGsh.struVer.sInterface[1] == m_versionToUpgrade[PCB_interface][1])
            return true;
        break;
    case PCB_front_master:
        if(struGsh.struVer.nUnitRes[0][0][chuteIndex] == 1 && struGsh.struVer.sUnit[0][0][chuteIndex][2] == USER_MODE &&
            struGsh.struVer.sUnit[0][0][chuteIndex][0] == m_versionToUpgrade[PCB_front_master][0] &&
            struGsh.struVer.sUnit[0][0][chuteIndex][1] == m_versionToUpgrade[PCB_front_master][1])
            return true;
        break;
    case PCB_back_master:
        if(struGsh.struVer.nUnitRes[0][1][chuteIndex] == 1 && struGsh.struVer.sUnit[0][1][chuteIndex][2] == USER_MODE &&
            struGsh.struVer.sUnit[0][1][chuteIndex][0] == m_versionToUpgrade[PCB_back_master][0] &&
            struGsh.struVer.sUnit[0][1][chuteIndex][1] == m_versionToUpgrade[PCB_back_master][1])
            return true;
        break;
    case PCB_front_slave:
        if(struGsh.struVer.nUnitRes[0][2][chuteIndex] == 1 && struGsh.struVer.sUnit[0][2][chuteIndex][2] == USER_MODE &&
            struGsh.struVer.sUnit[0][2][chuteIndex][0] == m_versionToUpgrade[PCB_front_slave][0] &&
            struGsh.struVer.sUnit[0][2][chuteIndex][1] == m_versionToUpgrade[PCB_front_slave][1])
            return true;
        break;
    case PCB_back_slave:
        if(struGsh.struVer.nUnitRes[0][3][chuteIndex] == 1 && struGsh.struVer.sUnit[0][3][chuteIndex][2] == USER_MODE &&
            struGsh.struVer.sUnit[0][3][chuteIndex][0] == m_versionToUpgrade[PCB_back_slave][0] &&
            struGsh.struVer.sUnit[0][3][chuteIndex][1] == m_versionToUpgrade[PCB_back_slave][1])
            return true;
        break;
    case PCB_valve:
        if(struGsh.struVer.nVavleRes[0][chainIndex-1][chuteIndex] == 1 && struGsh.struVer.sVavle[0][chainIndex-1][chuteIndex][2] == USER_MODE &&
            struGsh.struVer.sVavle[0][chainIndex-1][chuteIndex][0] == m_versionToUpgrade[PCB_valve][0] &&
            struGsh.struVer.sVavle[0][chainIndex-1][chuteIndex][1] == m_versionToUpgrade[PCB_valve][1])
            return true;
        break;
    case PCB_control:
        if(struGsh.struVer.nCtrlRes[chuteIndex] == 1 && struGsh.struVer.sCtrlBoard[chuteIndex][2] == USER_MODE &&
            struGsh.struVer.sCtrlBoard[chuteIndex][0] == m_versionToUpgrade[PCB_control][0] &&
            struGsh.struVer.sCtrlBoard[chuteIndex][1] == m_versionToUpgrade[PCB_control][1])
            return true;
        break;
    case PCB_lamp:
        if(struGsh.struVer.nLampRes[chuteIndex] == 1 && struGsh.struVer.sLampBoard[chuteIndex][2] == USER_MODE &&
            struGsh.struVer.sLampBoard[chuteIndex][0] == m_versionToUpgrade[PCB_lamp][0] &&
            struGsh.struVer.sLampBoard[chuteIndex][1] == m_versionToUpgrade[PCB_lamp][1])
            return true;
        break;
    case PCB_control_zd:
        if(struGsh.struVer.nAccelerationRes[chuteIndex] == 1 && struGsh.struVer.sAccelerationBoard[chuteIndex][2] == USER_MODE &&
            struGsh.struVer.sAccelerationBoard[chuteIndex][0] == m_versionToUpgrade[PCB_control_zd][0] &&
            struGsh.struVer.sAccelerationBoard[chuteIndex][1] == m_versionToUpgrade[PCB_control_zd][1])
            return true;
        break;
    case PCB_control_dl:
        if(struGsh.struVer.nHalogenRes[chuteIndex] == 1 && struGsh.struVer.sHalogenBoard[chuteIndex][2] == USER_MODE &&
            struGsh.struVer.sHalogenBoard[chuteIndex][0] == m_versionToUpgrade[PCB_control_dl][0] &&
            struGsh.struVer.sHalogenBoard[chuteIndex][1] == m_versionToUpgrade[PCB_control_dl][1])
            return true;
        break;
    default:
        return false;
        break;
    }
    return false;
}

int OneKeyUpdateFpgaWidget::getUpdateFileSize()
{
    QString fileName = m_updateVersionVec[mCurBoardType];
    if(fileName.contains("SIZE"))
    {
        QStringList vlist = fileName.split("SIZE");
        QString fileSizeType = vlist.at(1);
        QStringList vTmpLst = fileSizeType.split("_");
        QString fileType = vTmpLst.at(0);
        if(fileType == "0")
        {
            return 3550368;
        }
        else if(fileType == "1")
        {
            return 678656;
        }
        else if(fileType == "2")
        {
            return 6291456;
        }
        else if(fileType == "3")
        {
            return 12581888;
        }
    }
    else
    {
        return 0;
    }
}

void OneKeyUpdateFpgaWidget::on_stopBtn_clicked()
{
    id = timer1_stop_update;
    startTimer(20);
//    g_MainManager().ShowWidget(SM_IPC_UPGRADE_MODEL_WIDGET);
//    QMetaObject::invokeMethod(upgradeModelWidget, "performButtonClick", Qt::QueuedConnection);
}

