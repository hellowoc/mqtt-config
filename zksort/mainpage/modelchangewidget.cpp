#include "modelchangewidget.h"
#include "ui_modelchangewidget.h"
#include "mylistwidgetitem.h"
#include <QDateTime>
#include "global.h"
#include "renameprofiledialog.h"
#include "messagedialog.h"
#include "myinputmethod.h"
#include "bus/mqttsrv.h"
ModelChangeWidget::ModelChangeWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::ModelChangeWidget)
{
    ui->setupUi(this);
    m_pGroupWidget = ui->groupWidget;

    m_plistWidget = ui->listWidget;
    m_plistWidget->setIconSize(QSize(40,40));
    m_pViewBtn[0] = ui->pushButton;
    m_pViewBtn[1] = ui->pushButton_2;
    m_pViewBtn[2] = ui->pushButton_3;
    m_pViewBtn[3] = ui->pushButton_4;
    m_pViewBtn[4] = ui->pushButton_5;
    m_pViewBtn[5] = ui->pushButton_6;
    m_pViewBtn[6] = ui->pushButton_7;
    m_pViewBtn[7] = ui->pushButton_8;

    m_pChuteWidget = ui->chuteWidget;
    m_pLayerBtn[0] = ui->mainBtn;
    m_pLayerBtn[1] = ui->viceBtn;
    setSlotConnect();
    retranslateUiWidget();

    ui->replaceBtn->hide();
    ui->ipcSelectComboBox->hide();
    ui->rebootIpcBtn->hide();
    ui->roiHLbl->hide();
    ui->roiHLineEdit->hide();
    ui->roiWLbl->hide();
    ui->roiWLineEdit->hide();
    ui->roiRLbl->hide();
    ui->roiRLineEdit->hide();

    m_curIpcIndex = 0;
    m_curModelIndex = 0;
    //m_modelSame = true;
    ui->chuteWidget->setVisible(false);
}

ModelChangeWidget::~ModelChangeWidget()
{
    delete ui;
}

void ModelChangeWidget::showPage(bool bshow)
{
    basewidget::showPage(bshow);
    if(bshow){
        m_moreBtnUsed = true;

        struGsh.isInModelchangePage = 1;
        updateGroupWidgetState();
        updateViewButtonVisible();
        updateViewBtnState();
        updateChuteNum();
        doModelPreSelect();
        updateListWidget();
        updateAllBtnShow();
    }
    else
    {
        struGsh.isInModelchangePage = 0;
    }
}
void ModelChangeWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){
        if(msg2 == chute_change){
            basewidget::receiveMsg(msg1,msg2,msg3);
        }
        //        else if(msg2 == 0){
        //            if(struCnfp.nIpcRoiHeight[struGsh.nView]>0){
        //                struCnfp.nIpcRoiRepeatf[struGsh.nView] =(float)struCnfp.nIpcRoiRepeat[struGsh.nView]/(float)struCnfp.nIpcRoiHeight[struGsh.nView];
        //                qDebug()<< struCnfp.nIpcRoiRepeatf[struGsh.nView]<<struCnfp.nIpcRoiRepeat[struGsh.nView]<<struCnfp.nIpcRoiHeight[struGsh.nView];
        //                if(struCnfp.nDualLayerModelSame){
        //                    for(int i = 0; i< MAX_VIEW;i++){
        //                         struCnfp.nIpcRoiRepeatf[i] = struCnfp.nIpcRoiRepeatf[struGsh.nView];
        //                    }
        //                }
        //                myFlow.configIpcAIPara(0,0);
        //            }
        //        }
    }
}

void ModelChangeWidget::OnViewChange(int view)
{
    updateListWidget();
}

void ModelChangeWidget::OnChuteChange()
{
    updateListWidget();
}

void ModelChangeWidget::OnLayerChange()
{
    updateListWidget();
}

void ModelChangeWidget::OnGroupChange(int groupIndex)
{
    stru_iden_group_info groupInfo;

    memcpy(&groupInfo,&struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].stuIdenGupInfo[struGsh.nMatGroup],sizeof(groupInfo));

    int nUnitAddr = myFlow.getIdentifyGroupAddr(0, struGsh.nView,struGsh.nMatGroup, 0)%64;

    struGsh.nUnit = nUnitAddr;
    updateListWidget();
}

void ModelChangeWidget::retranslateUiWidget()
{
    m_strModePre = g_myLan().ids_new;
    ui->label->setText(g_myLan().cur_model);
    ui->applyBtn->setText(g_myLan().ids_apply);
    ui->recheckBtn->setText(g_myLan().ids_query);
    //    ui->addmodelbtn->setText(g_myLan().add_model);
    ui->addmodelbtn->setText("AI+");
    ui->rebootIpcBtn->setText(g_myLan().ipc_reboot);
    ui->delBtn->setText(g_myLan().ids_delete);
    ui->pushButton_9->setText(g_myLan().ids_remote_download);
}

void ModelChangeWidget::doModelPreSelect()
{
    //只在lgs dp机型上可选使用，此处二次确认是为了避免参数不统一
    //    if(struCnfg.nDPMachine && struCnfg.nUseIpcEnable && struCnfp.nDualLayerModelSame == 0)
    //        //        if(struCnfg.nDPMachine && struCnfg.nUseIpcEnable && struCnfg.nMachineType == MACHINE_LD2_ZL_GP && struCnfp.nDualLayerModelSame == 0)
    //    {
    //        //m_modelSame = false;
    //    }
    //    else
    //    {
    //        //struCnfp.nDualLayerModelSame = 1;
    //        //m_modelSame = true;
    //    }



}

void ModelChangeWidget::updateAllBtnShow()
{
    ui->ipcSelectComboBox->setMinimumSize(100, 50);
    ui->ipcSelectComboBox->clear();

    ui->addmodelbtn->setVisible(struGsh.bRunMode > RUNMODE_USER);
    ui->pushButton_9->setVisible(g_myLan().g_languageIndex == LANG_CHS || g_myLan().g_languageIndex == LANG_CHT);
}

void ModelChangeWidget::updateListWidget()
{
    QString strName;
    QString stat;

    //    ui->roiHLineEdit->setText(QString::number(struCnfp.nIpcRoiHeight[struGsh.nView] ));
    //    ui->roiWLineEdit->setText(QString::number(struCnfp.nIpcRoiWidth[struGsh.nView] ));
    //    ui->roiRLineEdit->setText(QString::number(struCnfp.nIpcRoiRepeat[struGsh.nView] ));
    //    ui->roiHLineEdit->setMinMax(this,0,416,0,&struCnfp.nIpcRoiHeight[struGsh.nView] );
    //    ui->roiWLineEdit->setMinMax(this,0,1024,0,&struCnfp.nIpcRoiWidth[struGsh.nView] );
    //    ui->roiRLineEdit->setMinMax(this,0,struCnfp.nIpcRoiHeight[struGsh.nView] /4,0,&struCnfp.nIpcRoiRepeat[struGsh.nView] );

    int count = m_plistWidget->count();
    for(int i = 0;i< count;i++){
        delete m_plistWidget->takeItem(0);
    }
    m_plistWidget->clear();
    m_curModelIndex = 0;
    for(int i = 0; i < struIpcShare.struIpcModelAble[struGsh.nView][struGsh.nUnit].modelCount; i++){
        QByteArray arrayName(struIpcShare.struIpcModelAble[struGsh.nView][struGsh.nUnit].modelInfo[i].modelname);

        if(memcmp(struIpcShare.struIpcModelAble[struGsh.nView][struGsh.nUnit].modelInfo[i].modelname, struCnfp.sCurModelName[struGsh.nView][struGsh.nMatGroup], sizeof(struCnfp.sCurModelName[struGsh.nView][struGsh.nMatGroup])) == 0)
        {
            //            schemeListItem->setIcon(QIcon(":/white/icon_sort.png"));
            if (struIpcShare.allIpcModelLoadOk == 2)
                stat = "✔";
            else
                stat = "✘";


            strName = stat+ QString("%1").arg(i+1,2,10,QChar('0'))+"."+QString::fromUtf8(arrayName);
            m_curModelIndex = i;
        }
        else
        {
            strName =QString("%1").arg(i+1,2,10,QChar('0'))+"."+QString::fromUtf8(arrayName);
        }

        QString dateString = QString(" [%1]").arg(struIpcShare.struIpcModelAble[struGsh.nView][struGsh.nUnit].modelInfo[i].modelDate);

        myListWidgetItem *schemeListItem = new myListWidgetItem(strName+"\r\n"+dateString,
                                                                QIcon(""),
                                                                QSize(m_plistWidget->width()-20,40));
        m_plistWidget->addItem(schemeListItem);
    }
    if(m_plistWidget->count() > m_curModelIndex)
        m_plistWidget->setCurrentRow(m_curModelIndex);

    updateApplyBtnState();
}

void ModelChangeWidget::updateApplyBtnState()
{
    ui->label_2->setText(QString::fromUtf8(struCnfp.sCurModelName[struGsh.nView][struGsh.nMatGroup]));
}

void ModelChangeWidget::apply()
{
    logger()->info("change model");
    g_infoWidget().setLabelText(g_myLan().apply_model);
    g_infoWidget().delayShow();

    m_curModelIndex = m_plistWidget->currentRow();


    struCnfp.nHasSaveModel = 1;

    int startView = 0, endView = 0;
    //if(m_modelSame)
    if(struCnfp.nDualLayerModelSame == 1) // 整机相同
    {
        startView = 0;
        endView   = MAX_VIEW;

        for(int i = startView; i< endView;i++)
        {
            for(int j = 0; j < MAX_GROUP_IDENTIFY; j++)
            {
                memset(struCnfp.sCurModelName[i][j], 0, MAX_IPC_MODEL_NAME_LENGTH);
                memset(struCnfp.sCurModelDate[i][j], 0, MAX_IPC_MODEL_DATE_LENGTH);

                memcpy(struCnfp.sCurModelName[i][j], struIpcShare.struIpcModelAble[struGsh.nView][struGsh.nUnit].modelInfo[m_curModelIndex].modelname, sizeof(struCnfp.sCurModelName[0][0]));
                memcpy(struCnfp.sCurModelDate[i][j], struIpcShare.struIpcModelAble[struGsh.nView][struGsh.nUnit].modelInfo[m_curModelIndex].modelDate, sizeof(struCnfp.sCurModelDate[0][0]));
            }
        }
    }
    else if(struCnfp.nDualLayerModelSame == 2) // 前后相同 可分次
    {
        if(struGsh.nView %2 == 0)
        {
            startView = struGsh.nView;
            endView   = struGsh.nView+1;
        }
        else
        {
            startView = struGsh.nView-1;
            endView   = struGsh.nView;
        }

        int k = struGsh.nMatGroup;
        int nUnitAddr = myFlow.getIdentifyGroupAddr(0, struGsh.nView, k, 0);

        if(nUnitAddr>128)
            nUnitAddr = myFlow.getIdentifyGroupAddr(0, struGsh.nView, k, 1)%64;
        else
            nUnitAddr = myFlow.getIdentifyGroupAddr(0, struGsh.nView, k, 0)%64;

        //        for(int i = startView; i<= endView;i++)
        //        {
        //            for(int j = 0; j < struCnfg.struLayerInfo[0].stuViewInfo[i].nIdentifyGroupTotal ; j++)
        //            {
        //                memset(struCnfp.sCurModelName[i][j], 0, MAX_IPC_MODEL_NAME_LENGTH);
        //                memset(struCnfp.sCurModelDate[i][j], 0, MAX_IPC_MODEL_DATE_LENGTH);

        //                memcpy(struCnfp.sCurModelName[i][j], struIpcShare.struIpcModelAble[struGsh.nView][nUnitAddr].modelInfo[m_curModelIndex].modelname, sizeof(struCnfp.sCurModelName[0][0]));
        //                memcpy(struCnfp.sCurModelDate[i][j], struIpcShare.struIpcModelAble[struGsh.nView][nUnitAddr].modelInfo[m_curModelIndex].modelDate, sizeof(struCnfp.sCurModelDate[0][0]));

        //                qDebug()<<"view"<<i<<"group"<<j<<"unit"<<nUnitAddr<<QString::fromUtf8(struCnfp.sCurModelName[i][k]);
        //            }
        //        }

        for(int i = startView; i<= endView;i++)
        {
            //for(int j = 0; j < struCnfg.struLayerInfo[0].stuViewInfo[i].nIdentifyGroupTotal ; j++)
            {
                memset(struCnfp.sCurModelName[i][k], 0, MAX_IPC_MODEL_NAME_LENGTH);
                memset(struCnfp.sCurModelDate[i][k], 0, MAX_IPC_MODEL_DATE_LENGTH);

                memcpy(struCnfp.sCurModelName[i][k], struIpcShare.struIpcModelAble[struGsh.nView][nUnitAddr].modelInfo[m_curModelIndex].modelname, sizeof(struCnfp.sCurModelName[0][0]));
                memcpy(struCnfp.sCurModelDate[i][k], struIpcShare.struIpcModelAble[struGsh.nView][nUnitAddr].modelInfo[m_curModelIndex].modelDate, sizeof(struCnfp.sCurModelDate[0][0]));
            }
        }

        if(myFlow.checkIpcModel() == 1)
        {
            g_infoWidget().setLabelText(g_myLan().Other_Group_error);
            g_infoWidget().delayShow();
            myFlow.sleep(1);
            g_infoWidget().hide();
            return;
        }

    }
    else if(struCnfp.nDualLayerModelSame == 3) // 前后分开 单视相同
    {
        startView = struGsh.nView;
        endView   = struGsh.nView+1;

        for(int i = startView; i< endView;i++)
        {
            for(int j = 0; j < MAX_GROUP_IDENTIFY; j++)
            {

                memset(struCnfp.sCurModelName[i][j], 0, MAX_IPC_MODEL_NAME_LENGTH);
                memset(struCnfp.sCurModelDate[i][j], 0, MAX_IPC_MODEL_DATE_LENGTH);

                memcpy(struCnfp.sCurModelName[i][j], struIpcShare.struIpcModelAble[struGsh.nView][struGsh.nUnit].modelInfo[m_curModelIndex].modelname, sizeof(struCnfp.sCurModelName[0][0]));
                memcpy(struCnfp.sCurModelDate[i][j], struIpcShare.struIpcModelAble[struGsh.nView][struGsh.nUnit].modelInfo[m_curModelIndex].modelDate, sizeof(struCnfp.sCurModelDate[0][0]));
            }
        }
    }
    else if(struCnfp.nDualLayerModelSame == 4) //全部独立
    {
        startView = struGsh.nView;
        endView   = struGsh.nView+1;
        for(int i = startView; i< endView;i++)
        {
            // for(int k = 0; k < struCnfg.struLayerInfo[0].stuViewInfo[startView].nIdentifyGroupTotal; k++)
            {
                int k = struGsh.nMatGroup;

                memset(struCnfp.sCurModelName[i][k], 0, MAX_IPC_MODEL_NAME_LENGTH);
                memset(struCnfp.sCurModelDate[i][k], 0, MAX_IPC_MODEL_DATE_LENGTH);

                int nUnitAddr = myFlow.getIdentifyGroupAddr(0, i, k, 0);
                if(nUnitAddr>128)
                    nUnitAddr = myFlow.getIdentifyGroupAddr(0, i, k, 1)%64;
                else
                    nUnitAddr = myFlow.getIdentifyGroupAddr(0, i, k, 0)%64;

                memcpy(struCnfp.sCurModelName[i][k], struIpcShare.struIpcModelAble[struGsh.nView][nUnitAddr].modelInfo[m_curModelIndex].modelname, sizeof(struCnfp.sCurModelName[i][0]));
                memcpy(struCnfp.sCurModelDate[i][k], struIpcShare.struIpcModelAble[struGsh.nView][nUnitAddr].modelInfo[m_curModelIndex].modelDate, sizeof(struCnfp.sCurModelDate[i][0]));

                qDebug()<<i<<QString::fromUtf8(struCnfp.sCurModelName[i][k]);
            }
        }
    }

    struIpcShare.allIpcModelLoadOk = 0; //开始等待模型加载
    struGsh.nIpcLoadCnt++;
    //myFlow.shutdownIpc(true,0,0,1);

    myFlow.resetIpcParams(0xff,0xff);

    switch (struIpcShare.allIpcModelLoadOk) {
    case 0:
        g_infoWidget().setLabelText(g_myLan().apply_model+"not satrt!");
        break;
    case 1:
        g_infoWidget().setLabelText(g_myLan().apply_model+"timeout!");
        break;
    case 2:
        g_infoWidget().setLabelText(g_myLan().apply_model+"ok!");
        break;
    case 3:
    case 4:
    case 5:
    case 6:
        g_infoWidget().setLabelText(g_myLan().apply_model+"err!"+QString::number(struIpcShare.allIpcModelLoadOk));
        break;
    default:
        break;
    }
    //模型加载完，查询到类别信息后，可以建表，并对ipc含杂清理
    if(struIpcShare.allIpcModelLoadOk == 2)
    {
        if(struCnfg.nAutoStaticsEnable == 1)
        {
            QByteArray modelName(struCnfp.sCurModelName[0][0]);   // 整机模型相同
            logger()->info("create table %1",g_Runtime().modelFullName);
            g_Runtime().modelFullName = QString(modelName);
            g_Runtime().modelFullNametest = QString(modelName) + "_Count";
            g_Runtime().modelFullName_Area = QString(modelName) + "_Area";
            g_Runtime().modelFullName_Frame = QString(modelName) + "_Frame";
            g_Runtime().modelFullNameList.clear();
            MySqlite mysql;
            for(int i = 0; i < struCnfg.struLayerInfo[0].nViewChainTotal; i++)
            {
                if(struCnfg.struLayerInfo[0].nViewIsUsed[i] == 1)
                {
                    for(int j = 0; j <struCnfg.struLayerInfo[0].nViewUnitTotal[i]; j++)
                    {
                        if(struCnfg.struIpcCameraParams[0][i][j].ipcType != IPC_CAMERA_TYPE_NULL)
                        {
                            mysql.creatstaticTable(g_Runtime().modelFullNametest + QString("_%1_%2").arg(i).arg(j),i, j);
                            mysql.creatstaticTable(g_Runtime().modelFullName_Area + QString("_%1_%2").arg(i).arg(j),i, j);
                            mysql.creatstaticTable(g_Runtime().modelFullName_Frame + QString("_%1_%2").arg(i).arg(j),i, j);
                        }
                    }
                }
            }
        }

        if( struGsh.strumachineset.targetCount != 0 && myFlow.getModelTargetValue(true,struGsh.nView,struGsh.nUnit) )
        {
            int tmpvalue = 0;
            for( tmpvalue = 0; tmpvalue < struGsh.strumachineset.targetCount; tmpvalue++)
            {
                if(struIpcShare.sCurModelTargetValue[struGsh.nView][struGsh.nUnit] == struGsh.strumachineset.targetlightValue[tmpvalue])
                {
                    if(struCnfp.bCurCal != tmpvalue)
                    {
                        g_infoWidget().setLabelText(g_myLan().Switch + g_myLan().ids_lampSystem+ QString::number( struGsh.strumachineset.targetlightValue[tmpvalue]));
                        struCnfp.bCurCal = tmpvalue;

                        myFlow.cameraChangeCalibrate();
                        myFlow.resetCamera();
                        /* 发送相机板IP */
                        if(struCnfg.nDPMachine)
                            myFlow.ipcResetCameraIpaddr(true, 0, 0, 0);
                        /* 发送控制板参数 */
                        myFlow.resetControl();
                        /* 发送色选参数 */
                        //  myFlow.resetSortParams();
                        /* 发送整机相机校正使能 */
                        myFlow.cameraWaveCaliEnable();
                        /* 发送灵敏度校正使能 */
                        myFlow.resetSensUpdateEnable();
                    }
                    else
                    {
                        g_infoWidget().setLabelText(g_myLan().NoChange);
                    }
                    break;
                }
            }
            if(tmpvalue == struGsh.strumachineset.targetCount)
            {
                g_infoWidget().setLabelText(g_myLan().Model_Nomatch_calibration);
            }
            myFlow.sleep(1);
        }
    }
    //加载完新模型需要清0
    myFlow.clearIpcCamBadPointsCount();
    updateListWidget();
    g_infoWidget().hide();
}

void ModelChangeWidget::on_listWidget_currentRowChanged(int currentRow)
{
    //    ui->delBtn->setEnabled(currentRow != struCnfg.nProfile);
}

void ModelChangeWidget::on_applyBtn_clicked()
{
    if(struGsh.bSortStart == 1)
    {
        g_infoWidget().setLabelText(g_myLan().msg_turn_off_first);
        g_infoWidget().delayShow();

        myFlow.sleep(3);
        g_infoWidget().hide();
        return;
    }
    myMessageBox msgBox(MSG_QUES, g_myLan().choose_model);
    if(msgBox.exec() == QDialog::Accepted){
        apply();
    }
    updateListWidget();
}

//void ModelChangeWidget::on_lockBtn_clicked()
//{
//    struCnfg.stuProfileIndex[m_plistWidget->currentRow()].nProfileLock = (struCnfg.stuProfileIndex[m_plistWidget->currentRow()].nProfileLock == 0) ? 1 : 0;
//    updateListWidget();
//}


void ModelChangeWidget::on_recheckBtn_clicked()
{
    g_infoWidget().setLabelText(g_myLan().check_model);
    g_infoWidget().delayShow();

    myFlow.getIpcModelEnable(true,struGsh.nView,struGsh.nUnit);
    g_infoWidget().hide();

    updateListWidget();
}

void ModelChangeWidget::on_replaceBtn_clicked()
{

}

void ModelChangeWidget::on_delBtn_clicked()
{
    m_curModelIndex = m_plistWidget->currentRow();
    if (memcmp(struCnfp.sCurModelName[struGsh.nView][struGsh.nMatGroup], struIpcShare.struIpcModelAble[struGsh.nView][struGsh.nUnit].modelInfo[m_curModelIndex].modelname,
               sizeof(struCnfp.sCurModelName[struGsh.nView][struGsh.nMatGroup])))
    {
        char string[64];
        char escaped[64];
        memset(escaped,0,sizeof(escaped));
        memcpy(string, struIpcShare.struIpcModelAble[struGsh.nView][struGsh.nUnit].modelInfo[m_curModelIndex].modelname,sizeof(string));
        for (int i = 0,j = 0; string[i] != '\0'; ++i)
        {
            printf("%c ",string[i]);
            if (string[i] == '(' || string[i] == ')')
            {
                escaped[j++] = '\\';
            }
            escaped[j++] = string[i];
        }

        myMessageBox msgBox(MSG_QUES, g_myLan().ids_delete+g_myLan().cur_model);
        int ret = msgBox.exec();
        if (ret == QDialog::Accepted){ //用户操作，同意进行
            g_infoWidget().setLabelText(g_myLan().ipc_verificating);
            g_infoWidget().delayShow();
            if(struCnfg.nXsystemBoard[struGsh.nView] == 0)  //RK
            {
                myFlow.deleteIpcmodel(true,0,struIpcShare.struIpcModelAble[struGsh.nView][struGsh.nUnit].modelInfo[m_curModelIndex].modelname); //默认整机删除
                myFlow.msleep(500);
            }
            else
            {
                myFlow.deleteXsysmodel(true,0,escaped); //默认整机删除
                myFlow.msleep(500);
            }
            myFlow.getIpcModelEnable(true,struGsh.nView,struGsh.nUnit);
            g_infoWidget().hide();
            updateListWidget();
        }
    }
}

void ModelChangeWidget::on_ipcSelectComboBox_activated(int index)
{
    m_curIpcIndex = index;
    updateListWidget();
}

void ModelChangeWidget::on_addmodelbtn_clicked()
{
    g_MainManager().ShowWidget(SM_IPC_UPGRADE_MODEL_WIDGET);
}

void ModelChangeWidget::on_rebootIpcBtn_clicked()
{
    myMessageBox msgBox(MSG_QUES, g_myLan().cfm_ipc_reboot);
    int ret = msgBox.exec();

    if (ret == QDialog::Accepted){ //用户操作，同意进行
        myFlow.shutdownIpc(true,0,0,1);
        ui->rebootIpcBtn->setEnabled(false);
    }
}

void ModelChangeWidget::updateIpcRestartBtnEnable(int view,int unit)
{
    ui->rebootIpcBtn->setEnabled(true);
}

void ModelChangeWidget::on_pushButton_9_clicked()
{
    g_MainManager().ShowWidget(SM_IPC_REMOTE_DOWNLOAD_MODEL_WIDGET);
}
