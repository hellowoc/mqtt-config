#include "modemanagewidget.h"
#include "ui_modemanagewidget.h"
#include "mylistwidgetitem.h"
#include <QDateTime>
#include "global.h"
#include "renameprofiledialog.h"
#include "messagedialog.h"
#include "myinputmethod.h"
#include "bus/mqttsrv.h"
ModeManageWidget::ModeManageWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::ModeManageWidget)
{
    ui->setupUi(this);
    m_plistWidget = ui->listWidget;
    m_plistWidget->setIconSize(QSize(40,40));
    retranslateUiWidget();

    shottimer = new QTimer;
    connect(shottimer,SIGNAL(timeout()),this, SLOT(getscreenshot()));
    ui->presetBtn->hide();
}

ModeManageWidget::~ModeManageWidget()
{
    delete ui;
}

void ModeManageWidget::showPage(bool bshow)
{
    if(bshow){
        samelist.clear();
        ui->comboBox->clear();
        samelist<<g_myLan().model_same_all<<g_myLan().ids_Type+g_myLan().ids_masterSame<<g_myLan().model_same_view<<g_myLan().model_same_group;
        ui->comboBox->addItems(samelist);
        ui->comboBox->setCurrentIndex(struCnfp.nDualLayerModelSame-1);
        m_moreBtnUsed = false;
        ui->checkBox->setVisible(false);
        ui->checkBox->setChecked(struCnfp.nDualLayerModelSame);
        updateListWidget();
        updateAllBtnShow();
        ui->ModelChangeBtn->setVisible(struCnfg.nDPMachine);

        if( struGsh.bScreenShotStatus ==1)
        {
            shottimer->start(1000);
        }
    }
}

void ModeManageWidget::retranslateUiWidget()
{
    m_strModePre = g_myLan().ids_new;
    ui->label->setText(g_myLan().scheme_cur);
    ui->newBtn->setText(g_myLan().ids_new);
    ui->copyBtn->setText(g_myLan().ids_copy);
    ui->applyBtn->setText(g_myLan().ids_apply);
    ui->delBtn->setText(g_myLan().ids_delete);
    ui->renameBtn->setText(g_myLan().ids_rename);
    ui->exportBtn->setText(g_myLan().ids_export);
    ui->importBtn->setText(g_myLan().ids_import);
    ui->sMoreBtn->setText(g_myLan().ids_more);
    ui->backupBtn->setText(g_myLan().scheme_backup);
    ui->recoveryBtn->setText(g_myLan().ids_restore);
    ui->ModelChangeBtn->setText(g_myLan().ipc_modelChange);
    ui->lockBtn->setText(g_myLan().ids_lock);
    ui->invalidClearBtn->setText(g_myLan().rm_redundancy);
    ui->checkBox->setText(g_myLan().dualLayerModelSame);
    ui->presetBtn->setText(g_myLan().usePreSetCnf);
    //    ui->lockBtn->setIcon(QIcon(":/white/icon_lock.png"));
}

void ModeManageWidget::getscreenshot()
{
    shottimer->stop();
    myFlow.sleep(2);
    myFlow.saveScreenshot();
    struGsh.bScreenShotStatus = 0 ;
}

void ModeManageWidget::on_newBtn_clicked()
{
    if(struGsh.bSortStart == 1)
    {
        g_infoWidget().setLabelText(g_myLan().msg_turn_off_first);
        g_infoWidget().delayShow();

        myFlow.sleep(3);
        g_infoWidget().hide();
        return;
    }

    if(memcmp(&struCnfp,&_t_struCnfp,sizeof(struCnfp))){
        myMessageBox msgBox(MSG_QUES, g_myLan().cfm_save_current_scheme);
        if(msgBox.exec() == QDialog::Accepted){
            myFlow.saveProfile();
        }
    }
    QString smaterialName = g_myLan().ids_new;
    QByteArray sMaterialArr = smaterialName.toUtf8();

    memset(struCnfg.stuProfileIndex[struCnfg.nProfileTotal].sMaterialName, 0,
            sizeof(struCnfg.stuProfileIndex[struCnfg.nProfileTotal].sMaterialName));
    memcpy(struCnfg.stuProfileIndex[struCnfg.nProfileTotal].sMaterialName,
            sMaterialArr.data(), sMaterialArr.length());
    QDateTime time = QDateTime::currentDateTime();
    QString tmpTime = time.toString("yyyyMMddhhmmsszzz");
    QByteArray array = tmpTime.toUtf8();
    const char *strName = array.data();

    memset(struCnfg.stuProfileIndex[struCnfg.nProfileTotal].sProfileName, 0, sizeof(struCnfg.stuProfileIndex[struCnfg.nProfileTotal].sProfileName));
    memcpy(struCnfg.stuProfileIndex[struCnfg.nProfileTotal].sProfileName, strName, array.length());

    /* 改变当前方案索引号 */
    struCnfg.nProfile = struCnfg.nProfileTotal;
    struCnfg.nProfileTotal++;
    myFlow.saveGlobal();

    g_infoWidget().setLabelText(g_myLan().ids_apply);
    g_infoWidget().delayShow();
    myFlow.createProfile();
    myFlow.saveProfile();
    /* 重新初始化方案 */
    myFlow.resetSortParams();
    g_infoWidget().hide();
    //    myMessageBox msgBox(MSG_QUES,  g_myLan().cfm_scheme_create);
    //    int ret = msgBox.exec();
    //    if(ret == QDialog::Accepted)
    //        g_MainManager().ShowWidget(SM_CREATE_MODE_WIDGET);
    updateListWidget();
}

void ModeManageWidget::on_delBtn_clicked()
{
    if(m_plistWidget->currentRow() == struCnfg.nProfile)
        return;
    if(struCnfg.stuProfileIndex[m_plistWidget->currentRow()].nProfileLock )
        return;
    myMessageBox msgBox(MSG_QUES,  g_myLan().ids_toDeleteModel);
    int ret = msgBox.exec();

    if (ret == QDialog::Accepted){
        if (m_plistWidget->currentItem()) {

            QByteArray fileNameArray(struCnfg.stuProfileIndex[m_plistWidget->currentRow()].sProfileName);
            QString fileName = QString("%1%2").arg(CNF_MODE_DIR).arg(QString::fromUtf8(fileNameArray));
            QFileInfo file(fileName);
            if(file.exists()){
                QFile::remove(fileName);
                QFile::remove(fileName+"_2");
            }

            /* update scheme m_plistWidget status*/
            for (int i = m_plistWidget->currentRow();i < m_plistWidget->count()-1;i++)
            {
                memset(struCnfg.stuProfileIndex[i].sMaterialName,0,sizeof(struCnfg.stuProfileIndex[i].sMaterialName));
                memset(struCnfg.stuProfileIndex[i].sProfileName,0,sizeof(struCnfg.stuProfileIndex[i].sProfileName));

                memcpy(struCnfg.stuProfileIndex[i].sMaterialName,
                       struCnfg.stuProfileIndex[i+1].sMaterialName,
                        sizeof(struCnfg.stuProfileIndex[i+1].sMaterialName));
                memcpy(struCnfg.stuProfileIndex[i].sProfileName,
                       struCnfg.stuProfileIndex[i+1].sProfileName,
                        sizeof(struCnfg.stuProfileIndex[i+1].sProfileName));
                struCnfg.stuProfileIndex[i].nProfileLock =struCnfg.stuProfileIndex[i+1].nProfileLock ;
            }
            /* set current scheme */
            if(m_plistWidget->currentRow() < struCnfg.nProfile)
                struCnfg.nProfile--;
            struCnfg.nProfileTotal--;

            myFlow.saveGlobal();
        }
        updateListWidget();
    }
}

void ModeManageWidget::updateListWidget()
{
    QString strScheme;

    /* clear m_plistWidget item */
    int count = m_plistWidget->count();
    for(int i = 0;i< count;i++){
        delete m_plistWidget->takeItem(0);
    }
    m_plistWidget->clear();

    /* scheme list widget adds items */
    for(int i = 0; i < struCnfg.nProfileTotal; i++){
        QByteArray array(struCnfg.stuProfileIndex[i].sMaterialName);
        strScheme =QString::number(i+1)+QString::fromUtf8(array);
        QFileInfo file(QString(CNF_MODE_DIR)+struCnfg.stuProfileIndex[i].sProfileName);
        QString dateString = QString(" [%1]").arg(file.lastModified().toString("yyyy/MM/dd hh:mm:ss"));

        myListWidgetItem *schemeListItem = new myListWidgetItem(strScheme+dateString,
                                                                //                                                                QIcon(":/white/icon_sort.png"),
                                                                QIcon(""),
                                                                QSize(m_plistWidget->width()-45,25*2));
        if(struCnfg.stuProfileIndex[i].nProfileLock == 1)
        {
            schemeListItem->setIcon(QIcon(":/white/icon_lock.png"));
        }
        //        schemeListItem->setData(Qt::DisplayRole,strScheme+dateString);


        if (i == struCnfg.nProfile){
            if(PRODUCT_DEFAULT_VENDOR == PRODUCT_VENDOR_LV){
                schemeListItem->setBackgroundColor(QColor(59,147,106));
            }else{
                schemeListItem->setBackgroundColor(Qt::green);
            }
        }
        m_plistWidget->addItem(schemeListItem);
    }

    m_plistWidget->setCurrentRow(struCnfg.nProfile);
    ui->delBtn->setEnabled(false);//不提供删除当前使用的方案的功能
    updateApplyBtnState();

}

void ModeManageWidget::updateApplyBtnState()
{
    ui->label_2->setText(QString::fromUtf8(struCnfg.stuProfileIndex[struCnfg.nProfile].sMaterialName));
}

void ModeManageWidget::apply()
{
    int lastProfile = struCnfg.nProfile;
    g_infoWidget().setLabelText(g_myLan().msg_applying);
    g_infoWidget().delayShow();

    myFlow.saveProfile();
    struCnfg.nProfile = m_plistWidget->currentRow();
    ui->label_2->setText(QString::fromUtf8(struCnfg.stuProfileIndex[struCnfg.nProfile].sMaterialName));

    logger()->info("profile change %1 to %2",QString::fromUtf8(struCnfg.stuProfileIndex[lastProfile].sMaterialName),
                   QString::fromUtf8(struCnfg.stuProfileIndex[struCnfg.nProfile].sMaterialName));

    //切换方案的时候会自动切换模型，此时需对前一个模型含杂清理
    if(lastProfile != struCnfg.nProfile)
    {
        if(struCnfg.nAutoStaticsEnable == 1 && struCnfg.nUseIpcEnable == 1 && struCnfp.nHasSaveModel == 1)
        {
            myFlow.clearIpcCamBadPointsCount();
        }
    }
    myFlow.getProfile();

    // 切换相机用户
    myFlow.SwitchAllViewUserMode();
    /* 发送相机参数 */
    myFlow.resetCamera();
    /* 发送相机板IP */
    if(struCnfg.nDPMachine)
        myFlow.ipcResetCameraIpaddr(true, 0, 0, 0);

    /* 发送控制板参数 */
    myFlow.resetControl();
    /* 发送色选参数 */
    myFlow.resetSortParams();
    /* 发送整机相机校正使能 */
    myFlow.cameraWaveCaliEnable();
    /* 发送灵敏度校正使能 */
    myFlow.resetSensUpdateEnable();
    /* 关闭下位机色选 */
    myFlow.resetSortOnOff(0, 0);	//turn off sort
    myFlow.configIpcSortStart(0);
    /* 关闭下位机喷阀 */
    myFlow.vavleOnOff();
    //延迟缓冲时间
    if(struCnfg.nDPMachine)
        myFlow.setIpcDelayBufRowCount();

    myFlow.colorRestoreEnable();

    /* 加载模型 */
    //    for(int j = 0; j < struCnfg.struLayerInfo[0].nViewChainTotal; j++)
    //    {
    //        if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
    //        for(int k = 0; k < struCnfg.struLayerInfo[0].nViewUnitTotal[j]; k++)
    //        {
    //            g_Udp.loadIpcModelSlt(j,k);
    //        }
    //    }
    //    myFlow.sleep(3);
    //    for(int i=0;i < 30;i++)
    //    {
    //        if(struIpcShare.allIpcModelLoadOk == 2)
    //            break;
    //        myFlow.sleep(1);
    //    }

    if(struCnfg.nDPMachine && struCnfg.nUseIpcEnable)
    {
        struIpcShare.allIpcModelLoadOk = 0; //开始等待模型加载
        struGsh.nIpcLoadCnt++;
        myFlow.resetIpcParams(0xff,0xff);
        for(int i = 0; i< 50;i++)
        {
            myFlow.msleep(200);
            switch (struIpcShare.allIpcModelLoadOk) {
            case 0:
                break;
            case 1:
                g_infoWidget().setLabelText(g_myLan().apply_model+"timeout!");
                goto end;
            case 2:
                g_infoWidget().setLabelText(g_myLan().apply_model+"ok!");
                goto end;
            case 3:
            case 4:
            case 5:
            case 6:
                g_infoWidget().setLabelText(g_myLan().apply_model+"err!"+QString::number(struIpcShare.allIpcModelLoadOk));
                goto end;
            default:
                break;
            }
        }
end:
        myFlow.sleep(1);
    }


#ifdef Q_OS_UNIX
//    myMqttMsgParaseThread->n_uploadSchemeInfoFlag = 1;
#endif


    g_infoWidget().hide();
    returnBack();
}

void ModeManageWidget::on_listWidget_currentRowChanged(int currentRow)
{
    ui->delBtn->setEnabled(currentRow != struCnfg.nProfile);
}

void ModeManageWidget::on_applyBtn_clicked()
{
    //    if(struCnfg.nProfile == m_plistWidget->currentRow())
    //        apply();
    if(struGsh.bSortStart == 1)
    {
        g_infoWidget().setLabelText(g_myLan().msg_turn_off_first);
        g_infoWidget().delayShow();

        myFlow.sleep(3);
        g_infoWidget().hide();
        return;
    }

    myMessageBox msgBox(MSG_QUES, g_myLan().cfm_select_current_scheme);
    if(msgBox.exec() == QDialog::Accepted){
        apply();
    }
}

void ModeManageWidget::on_renameBtn_clicked()
{
    if (struCnfg.stuProfileIndex[m_plistWidget->currentRow()].nProfileLock)
    {
        g_infoWidget().setLabelText(g_myLan().ids_cannotRenameModelInLocked);
        g_infoWidget().showSecs();
        return;
    }
    myMessageBox msgBox(MSG_QUES, g_myLan().cfm_scheme_rename);
    int ret = msgBox.exec();
    int currentRow = m_plistWidget->currentRow();

    if(ret == QDialog::Accepted)
    {
        myInputMethod inputPanel;
        inputPanel.setTitleAndInput(g_myLan().input, QString::fromUtf8(struCnfg.stuProfileIndex[currentRow].sMaterialName));
        inputPanel.setInputBtnText(g_myLan().ids_ok,g_myLan().ids_cancel,g_myLan().ids_delete,g_myLan().ids_space);
        if (inputPanel.exec() == QDialog::Accepted) {
            const char *strName = inputPanel.getText().toUtf8();
            memset(struCnfg.stuProfileIndex[currentRow].sMaterialName, 0,
                   sizeof(struCnfg.stuProfileIndex[currentRow].sMaterialName));
            memcpy(struCnfg.stuProfileIndex[currentRow].sMaterialName,strName,strlen(strName));
        }
    }
    updateListWidget();
}

void ModeManageWidget::on_copyBtn_clicked()
{
    int currentRow = m_plistWidget->currentRow();
    if (m_plistWidget->currentItem()) {
        for (int i =m_plistWidget->count() ;i > m_plistWidget->currentRow() ;i--)
        {
            memset(struCnfg.stuProfileIndex[i].sMaterialName,0,sizeof(struCnfg.stuProfileIndex[i].sMaterialName));
            memset(struCnfg.stuProfileIndex[i].sProfileName,0,sizeof(struCnfg.stuProfileIndex[i].sProfileName));

            memcpy(struCnfg.stuProfileIndex[i].sMaterialName,
                   struCnfg.stuProfileIndex[i-1].sMaterialName,
                    sizeof(struCnfg.stuProfileIndex[i-1].sMaterialName));
            memcpy(struCnfg.stuProfileIndex[i].sProfileName,
                   struCnfg.stuProfileIndex[i-1].sProfileName,
                    sizeof(struCnfg.stuProfileIndex[i-1].sProfileName));

            struCnfg.stuProfileIndex[i].nProfileLock =struCnfg.stuProfileIndex[i-1].nProfileLock ;
        }
        QByteArray oldArr(struCnfg.stuProfileIndex[currentRow].sMaterialName);
        oldArr+= "_copy";
        memset(struCnfg.stuProfileIndex[currentRow+1].sMaterialName, 0,
                sizeof(struCnfg.stuProfileIndex[currentRow+1].sMaterialName));
        memcpy(struCnfg.stuProfileIndex[currentRow+1].sMaterialName,oldArr.data(),oldArr.length());



        QString dir = QString(CNF_MODE_DIR);
        QString oriFileName = QString(struCnfg.stuProfileIndex[currentRow].sProfileName);

        /* copy selected profile */
        QDateTime time = QDateTime::currentDateTime();
        QString tmpTime = time.toString("yyyyMMddhhmmss");
        const char *strName = tmpTime.toLatin1();
        memset(struCnfg.stuProfileIndex[currentRow+1].sProfileName, 0,
                sizeof(struCnfg.stuProfileIndex[currentRow+1].sProfileName));
        memcpy(struCnfg.stuProfileIndex[currentRow+1].sProfileName, strName, strlen(strName));
        QString desFileName = QString(struCnfg.stuProfileIndex[currentRow+1].sProfileName);
        g_Runtime().copyFileToPath(dir+oriFileName,  dir+desFileName,true);
        if(!g_Runtime().copyFileToPath(dir+oriFileName,  dir+desFileName,true)) {
            qDebug("ERROR : Copy profile error!");
        }
        if(!g_Runtime().copyFileToPath(dir+oriFileName,  dir+desFileName+"_2",true)) {
            qDebug("ERROR : Copy profile error!");
        }
        struCnfg.nProfileTotal++;
        if(currentRow<struCnfg.nProfile)
            struCnfg.nProfile++;

        myFlow.saveGlobal();
        updateListWidget();
    }
}

void ModeManageWidget::on_lockBtn_clicked()
{
    struCnfg.stuProfileIndex[m_plistWidget->currentRow()].nProfileLock = (struCnfg.stuProfileIndex[m_plistWidget->currentRow()].nProfileLock == 0) ? 1 : 0;
    updateListWidget();
}

void ModeManageWidget::on_exportBtn_clicked()
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

    myMessageBox msgBox(MSG_QUES, g_myLan().cfm_export_parameter);

    int ret = msgBox.exec();

    if (ret == QDialog::Accepted)
    {
        myMessageBox msgBox2(MSG_QUES, g_myLan().together_export_image);

        ret = msgBox2.exec();

        if (ret == QDialog::Accepted)
        {

            g_infoWidget().setLabelText(g_myLan().msg_exporting_params);
            g_infoWidget().delayShow();

            QString tarCmd = "rm -f " +path+"cnf_bmp.tgz" ;
            g_Runtime().mySystem(tarCmd);

            tarCmd = "cd  /sdcard; tar -czvf " +path + "cnf_bmp.tgz   cnf/  bmp/" ;

            if(struGsh.debugMode)
                tarCmd += " logs/ ";
            g_Runtime().mySystem(tarCmd);
            myFlow.sleep(1);
            g_infoWidget().hide();
        }
        else
        {
            g_infoWidget().setLabelText(g_myLan().msg_exporting_params);
            g_infoWidget().delayShow();
#ifdef Q_OS_UNIX
            //            QString str = QString("%1%2").arg("cp /sdcard/cnf -fr ").arg(path);
            //            const char *cmd = QString(str).toLocal8Bit().constData();
            //            system(cmd);
            //            system("sync");

            QString tarCmd = "rm -f " +path+"cnf.tgz" ;
            g_Runtime().mySystem(tarCmd);

            tarCmd = "cd  /sdcard; tar -czvf " +path + "cnf.tgz   cnf/" ;
            if(struGsh.debugMode)
                tarCmd += " logs/ ";
            g_Runtime().mySystem(tarCmd);

            QString str = QString("%1%2").arg("cp /sdcard/logs -fr ").arg(path);
            const char *cmd2 = QString(str).toLocal8Bit().constData();
            system(cmd2);
            system("sync");
#endif
            myFlow.sleep(1);
            g_infoWidget().hide();
        }
    }
}

void ModeManageWidget::on_importBtn_clicked()
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

    myMessageBox msgBox(MSG_QUES, g_myLan().cfm_import_parameter);

    int ret = msgBox.exec();

    if (ret == QDialog::Accepted)
    {
        logger()->info("import cnf");
        QFileInfo file1(path + "cnf_bmp.tgz");
        QFileInfo file2(path + "cnf.tgz");

        if(file1.exists() && file1.size()>0)
        {
            g_infoWidget().setLabelText(g_myLan().msg_importing_params);

            g_infoWidget().delayShow();

            QString tarCmd = "cd " + path + "; tar -xzvf cnf_bmp.tgz -C  /sdcard/" ;
            g_Runtime().mySystem(tarCmd);
#ifdef Q_OS_UNIX
            struGsh.poweroff_status = 1;
            QSettings setting(CFG_APPSet,QSettings::IniFormat);
            setting.setValue("poweroffstatus",struGsh.poweroff_status);
            setting.sync();

            tarCmd = "rm -rf /sdcard/cnf_bak/*";
            g_Runtime().mySystem(tarCmd);

            system("sync");
            this->close();
            myFlow.msleep(100);
            QProcess::startDetached("/app/myrestart.sh");
            qApp->quit();
#endif
        }
        else if(file2.exists() && file2.size()>0)
        {
            g_infoWidget().setLabelText(g_myLan().msg_importing_params);
            g_infoWidget().delayShow();

            QString tarCmd = "cd  " + path + "; tar -xzvf cnf.tgz -C  /sdcard/" ;
            g_Runtime().mySystem(tarCmd);
#ifdef Q_OS_UNIX
            struGsh.poweroff_status = 1;
            QSettings setting(CFG_APPSet,QSettings::IniFormat);
            setting.setValue("poweroffstatus",struGsh.poweroff_status);
            setting.sync();

            tarCmd = "rm -rf /sdcard/cnf_bak/*";
            g_Runtime().mySystem(tarCmd);

            system("sync");
            this->close();
            myFlow.msleep(100);
            QProcess::startDetached("/app/myrestart.sh");
            qApp->quit();
#endif
        }
        else
        {
            g_infoWidget().setLabelText(g_myLan().msg_importing_params);
            g_infoWidget().delayShow();

#ifdef Q_OS_UNIX
            QString str = QString("%1%2%3").arg("cp ").arg(path).arg(+"cnf -fr /sdcard/");
            const char *cmd = QString(str).toLocal8Bit().constData();
            system(cmd);
            g_Runtime().mySystem("");
            system("sync");
         struGsh.poweroff_status = 1;
            QSettings setting(CFG_APPSet,QSettings::IniFormat);
            setting.setValue("poweroffstatus",struGsh.poweroff_status);
            setting.sync();
            str = "rm -rf /sdcard/cnf_bak/*";
            g_Runtime().mySystem(str);
            this->close();
            myFlow.msleep(100);
            QProcess::startDetached("/app/myrestart.sh");
            qApp->quit();
#endif
        }
    }
}

void ModeManageWidget::updateAllBtnShow()
{
    ui->exportBtn->setVisible(m_moreBtnUsed &(struGsh.bRunMode != RUNMODE_USER));
    ui->importBtn->setVisible(m_moreBtnUsed &(struGsh.bRunMode != RUNMODE_USER));
    ui->backupBtn->setVisible(m_moreBtnUsed &(struGsh.bRunMode != RUNMODE_USER));
    ui->recoveryBtn->setVisible(m_moreBtnUsed &(struGsh.bRunMode != RUNMODE_USER));
    ui->lockBtn->setVisible(m_moreBtnUsed &(struGsh.bRunMode != RUNMODE_USER));
    ui->newBtn->setVisible(struGsh.bRunMode != RUNMODE_USER);
    ui->delBtn->setVisible(struGsh.bRunMode != RUNMODE_USER);
    ui->renameBtn->setVisible(struGsh.bRunMode != RUNMODE_USER);
    ui->copyBtn->setVisible(struGsh.bRunMode != RUNMODE_USER);
    ui->sMoreBtn->setVisible(struGsh.bRunMode != RUNMODE_USER);
    ui->recoveryAllBtn->setVisible(m_moreBtnUsed &(struGsh.bRunMode != RUNMODE_USER));
}

void ModeManageWidget::on_backupBtn_clicked()
{
    myMessageBox msgBox(MSG_QUES,  g_myLan().cfm_backup_to_local);
    int ret = msgBox.exec();

    if (ret == QDialog::Accepted){
        if (m_plistWidget->currentItem()) {

            g_Runtime().dirExist(CNF_BAK_DIR);
            QByteArray fileNameArray(struCnfg.stuProfileIndex[m_plistWidget->currentRow()].sProfileName);
            QString fileName = QString("%1%2").arg(CNF_MODE_DIR).arg(QString::fromUtf8(fileNameArray));
            QString fileName_bak = QString("%1%2").arg(CNF_BAK_DIR).arg(QString::fromUtf8(fileNameArray));

            bool ok  = g_Runtime().copyFileToPath(fileName+"_2",fileName_bak+"_2",true);
            bool ok1 = g_Runtime().copyFileToPath(fileName,fileName_bak,true);
            if (ok && ok1)
            {
                g_infoWidget().setLabelText(g_myLan().msg_backup_to_local_success);
                g_infoWidget().showSecs(1);
            }
            else
            {
                g_infoWidget().setLabelText(g_myLan().msg_backup_to_local_error);
                g_infoWidget().showSecs(1);
            }
        }
    }
}

void ModeManageWidget::on_recoveryBtn_clicked()
{
    myMessageBox msgBox(MSG_QUES, g_myLan().cfm_restore_from_local);
    int ret = msgBox.exec();

    if (ret == QDialog::Accepted){
        if (m_plistWidget->currentItem()) {

            QByteArray fileNameArray(struCnfg.stuProfileIndex[m_plistWidget->currentRow()].sProfileName);
            QString fileName = QString("%1%2").arg(CNF_MODE_DIR).arg(QString::fromUtf8(fileNameArray));
            QString fileName_bak = QString("%1%2").arg(CNF_BAK_DIR).arg(QString::fromUtf8(fileNameArray));

            bool ok  = g_Runtime().copyFileToPath(fileName_bak+"_2",fileName+"_2",true);
            bool ok1 = g_Runtime().copyFileToPath(fileName_bak,fileName,true);

            if(m_plistWidget->currentRow() == struCnfg.nProfile)
            {
                myFlow.getProfile();
            }
            if (ok && ok1)
            {
                g_infoWidget().setLabelText(g_myLan().ids_restoreFinished);
                g_infoWidget().showSecs(1);
            }
            else
            {
                g_infoWidget().setLabelText(g_myLan().msg_restore_to_local_error);
                g_infoWidget().showSecs(1);
            }

        }
    }
}

void ModeManageWidget::on_sMoreBtn_clicked()
{
    m_moreBtnUsed = (m_moreBtnUsed == true) ? false : true;
    updateAllBtnShow();
}

void ModeManageWidget::on_ModelChangeBtn_clicked()
{
    g_MainManager().ShowWidget(SM_IPC_MODEL_CHANGE_WIDGET);
}

void ModeManageWidget::on_invalidClearBtn_clicked()
{
    int i,j;
    QDir dir(CNF_MODE_DIR);
    QFileInfoList list = dir.entryInfoList();
    for (i = 0; i < list.size(); i++) {
        QFileInfo fileInfo = list.at(i);
        QString filename = fileInfo.fileName();
        if(filename.startsWith("20") && !filename.contains("_2")){
            for(j = 0; j < struCnfg.nProfileTotal; j++){
                if (filename == QString::fromLatin1(struCnfg.stuProfileIndex[j].sProfileName))
                    break;
            }
            if(j == struCnfg.nProfileTotal){
                QFile::remove(CNF_MODE_DIR+filename);
                QFile::remove(CNF_MODE_DIR+filename+"_2");
            }
        }
    }
    system("sysctl -w vm.drop_caches=3 > /dev/zero");
}

void ModeManageWidget::on_checkBox_clicked(bool checked)
{
    struCnfp.nDualLayerModelSame = (int)checked;
}

void ModeManageWidget::on_comboBox_activated(int index)
{
    struCnfp.nDualLayerModelSame = index+1;

    if(struCnfp.nDualLayerModelSame == 3 || struCnfp.nDualLayerModelSame == 4)
        struCnfg.nIpcCameraClassSameMode = 0;
    else if(struCnfp.nDualLayerModelSame == 1 || struCnfp.nDualLayerModelSame == 2)
        struCnfg.nIpcCameraClassSameMode = 1;

}

void ModeManageWidget::on_presetBtn_clicked()
{
    myMessageBox msgBox(MSG_QUES, g_myLan().cfmClearAllCurrentCnf);
    int ret = msgBox.exec();

    if (ret == QDialog::Accepted){
        logger()->info("use preset cnf");
        g_infoWidget().setLabelText(g_myLan().msg_applying);
        g_infoWidget().delayShow();
        system("cp /sdcard/cnf -fr /sdcard/cnf_bf_preset");
        system("sync");
        system("rm /sdcard/cnf/* -fr");
        system("sync");
        system("tar zxf /app/preb/LS/cnf.tgz -C /sdcard");
        system("sync");
        g_infoWidget().hide();
        this->close();
        myFlow.msleep(100);
        QProcess::startDetached("/app/myrestart.sh");
        qApp->quit();
    }
}

void ModeManageWidget::on_recoveryAllBtn_clicked()
{
    myMessageBox msgBox(MSG_QUES, g_myLan().cfm_restore_from_local);
    int ret = msgBox.exec();

    if (ret == QDialog::Accepted){

        myFlow.checkCompressParams();

        struGsh.poweroff_status = 1;
        QSettings setting(CFG_APPSet,QSettings::IniFormat);
        setting.setValue("poweroffstatus",struGsh.poweroff_status);
        setting.sync();

        system("sync");
        this->close();
        myFlow.msleep(100);
        QProcess::startDetached("/app/myrestart.sh");
        qApp->quit();


        //        if (m_plistWidget->currentItem()) {

        //            QByteArray fileNameArray(struCnfg.stuProfileIndex[m_plistWidget->currentRow()].sProfileName);
        //            QString fileName = QString("%1%2").arg(CNF_MODE_DIR).arg(QString::fromUtf8(fileNameArray));
        //            QString fileName_bak = QString("%1%2").arg(CNF_BAK_DIR).arg(QString::fromUtf8(fileNameArray));

        //            bool ok  = g_Runtime().copyFileToPath(fileName_bak+"_2",fileName+"_2",true);
        //            bool ok1 = g_Runtime().copyFileToPath(fileName_bak,fileName,true);

        //            if(m_plistWidget->currentRow() == struCnfg.nProfile)
        //            {
        //                myFlow.getProfile();
        //            }
        //            if (ok && ok1)
        //            {
        //                g_infoWidget().setLabelText(g_myLan().ids_restoreFinished);
        //                g_infoWidget().showSecs(1);
        //            }
        //            else
        //            {
        //                g_infoWidget().setLabelText(g_myLan().msg_restore_to_local_error);
        //                g_infoWidget().showSecs(1);
        //            }

        //        }
    }
}
