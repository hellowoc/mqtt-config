#include "ipccamerasetwidget.h"
#include "ui_ipccamerasetwidget.h"
#include "globalflow.h"

IpcCameraSetWidget::IpcCameraSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::IpcCameraSetWidget)
{
    ui->setupUi(this);

    m_pViewBtn[0] = ui->pushButton;
    m_pViewBtn[1] = ui->pushButton_2;
    m_pViewBtn[2] = ui->pushButton_3;
    m_pViewBtn[3] = ui->pushButton_4;
    m_pViewBtn[4] = ui->pushButton_5;
    m_pViewBtn[5] = ui->pushButton_6;
    m_pViewBtn[6] = ui->pushButton_7;
    m_pViewBtn[7] = ui->pushButton_8;

    m_pLayerBtn[0] = ui->mainBtn;
    m_pLayerBtn[1] = ui->viceBtn;

    setSlotConnect();

    m_pIpcCamTitle[0] = ui->label_0;
    m_pIpcCamTitle[1] = ui->label_1;
    m_pIpcCamTitle[2] = ui->label_2;
    m_pIpcCamTitle[3] = ui->label_3;
    m_pIpcCamTitle[4] = ui->label_4;
    m_pIpcCamTitle[5] = ui->label_5;
    m_pIpcCamTitle[6] = ui->label_6;
    m_pIpcCamTitle[7] = ui->label_7;
    m_pIpcCamTitle[8] = ui->label_8;
    m_pIpcCamTitle[9] = ui->label_9;
    m_pIpcCamTitle[10] = ui->label_10;
    m_pIpcCamTitle[11] = ui->label_11;

    m_pIpcTypeCmx[0] = ui->comboBox_cam1_ipc;
    m_pIpcTypeCmx[1] = ui->comboBox_cam2_ipc;
    m_pIpcTypeCmx[2] = ui->comboBox_cam3_ipc;
    m_pIpcTypeCmx[3] = ui->comboBox_cam4_ipc;
    m_pIpcTypeCmx[4] = ui->comboBox_cam5_ipc;
    m_pIpcTypeCmx[5] = ui->comboBox_cam6_ipc;
    m_pIpcTypeCmx[6] = ui->comboBox_cam7_ipc;
    m_pIpcTypeCmx[7] = ui->comboBox_cam8_ipc;
    m_pIpcTypeCmx[8] = ui->comboBox_cam9_ipc;
    m_pIpcTypeCmx[9] = ui->comboBox_cam10_ipc;
    m_pIpcTypeCmx[10] = ui->comboBox_cam11_ipc;
    m_pIpcTypeCmx[11] = ui->comboBox_cam12_ipc;

    m_pIpcIpAddr[0] = ui->lineEdit_cam1_ip;
    m_pIpcIpAddr[1] = ui->lineEdit_cam2_ip;
    m_pIpcIpAddr[2] = ui->lineEdit_cam3_ip;
    m_pIpcIpAddr[3] = ui->lineEdit_cam4_ip;
    m_pIpcIpAddr[4] = ui->lineEdit_cam5_ip;
    m_pIpcIpAddr[5] = ui->lineEdit_cam6_ip;
    m_pIpcIpAddr[6] = ui->lineEdit_cam7_ip;
    m_pIpcIpAddr[7] = ui->lineEdit_cam8_ip;
    m_pIpcIpAddr[8] = ui->lineEdit_cam9_ip;
    m_pIpcIpAddr[9] = ui->lineEdit_cam10_ip;
    m_pIpcIpAddr[10] = ui->lineEdit_cam11_ip;
    m_pIpcIpAddr[11] = ui->lineEdit_cam12_ip;

    m_pIpcSavePicEnable[0] = ui->checkBox;
    m_pIpcSavePicEnable[1] = ui->checkBox_2;
    m_pIpcSavePicEnable[2] = ui->checkBox_3;
    m_pIpcSavePicEnable[3] = ui->checkBox_4;
    m_pIpcSavePicEnable[4] = ui->checkBox_5;
    m_pIpcSavePicEnable[5] = ui->checkBox_6;
    m_pIpcSavePicEnable[6] = ui->checkBox_7;
    m_pIpcSavePicEnable[7] = ui->checkBox_8;
    m_pIpcSavePicEnable[8] = ui->checkBox_9;
    m_pIpcSavePicEnable[9] = ui->checkBox_10;
    m_pIpcSavePicEnable[10] = ui->checkBox_11;
    m_pIpcSavePicEnable[11] = ui->checkBox_12;

    m_pIpcTypeGroupSigMapper = new QSignalMapper(this);
    for(int i = 0; i < CONST_MAX_CAMERA_PER_VIEW; i++){
        m_pIpcTypeGroupSigMapper->setMapping(m_pIpcTypeCmx[i], i);
        connect(m_pIpcTypeCmx[i],SIGNAL(activated(int)),m_pIpcTypeGroupSigMapper,SLOT(map()));
    }
    connect(m_pIpcTypeGroupSigMapper,SIGNAL(mapped(int)),this,SLOT(ipcTypeGroupClicked(int)));

    m_pIpcIpEditSigMapper = new QSignalMapper(this);
    for(int i = 0; i < CONST_MAX_CAMERA_PER_VIEW; i++){
        m_pIpcIpAddr[i]->setType(textType);
        m_pIpcIpAddr[i]->setObjectName("ipcIpAddr");
        m_pIpcIpEditSigMapper->setMapping(m_pIpcIpAddr[i], i);
        connect(m_pIpcIpAddr[i],SIGNAL(valueSetSignal()),m_pIpcIpEditSigMapper,SLOT(map()));
    }
    connect(m_pIpcIpEditSigMapper,SIGNAL(mapped(int)),this,SLOT(ipcIpEditClicked(int)));

    m_pIpcSavePicSigMapper = new QSignalMapper(this);
    for(int i = 0; i < CONST_MAX_CAMERA_PER_VIEW; i++){
        m_pIpcSavePicSigMapper->setMapping(m_pIpcSavePicEnable[i], i);
        connect(m_pIpcSavePicEnable[i],SIGNAL(clicked(bool)),m_pIpcSavePicSigMapper,SLOT(map()));
    }
    connect(m_pIpcSavePicSigMapper,SIGNAL(mapped(int)),this,SLOT(ipcSavePicEnabled(int)));

    m_pIpcCamPara[0][0] = ui->lineEdit_0_0;
    m_pIpcCamPara[1][0] = ui->lineEdit_1_0;
    m_pIpcCamPara[2][0] = ui->lineEdit_2_0;
    m_pIpcCamPara[3][0] = ui->lineEdit_3_0;
    m_pIpcCamPara[4][0] = ui->lineEdit_4_0;
    m_pIpcCamPara[5][0] = ui->lineEdit_5_0;
    m_pIpcCamPara[6][0] = ui->lineEdit_6_0;
    m_pIpcCamPara[7][0] = ui->lineEdit_7_0;
    m_pIpcCamPara[8][0] = ui->lineEdit_8_0;
    m_pIpcCamPara[9][0] = ui->lineEdit_9_0;
    m_pIpcCamPara[10][0] = ui->lineEdit_10_0;
    m_pIpcCamPara[11][0] = ui->lineEdit_11_0;

    m_pIpcCamPara[0][1] = ui->lineEdit_0_1;
    m_pIpcCamPara[1][1] = ui->lineEdit_1_1;
    m_pIpcCamPara[2][1] = ui->lineEdit_2_1;
    m_pIpcCamPara[3][1] = ui->lineEdit_3_1;
    m_pIpcCamPara[4][1] = ui->lineEdit_4_1;
    m_pIpcCamPara[5][1] = ui->lineEdit_5_1;
    m_pIpcCamPara[6][1] = ui->lineEdit_6_1;
    m_pIpcCamPara[7][1] = ui->lineEdit_7_1;
    m_pIpcCamPara[8][1] = ui->lineEdit_8_1;
    m_pIpcCamPara[9][1] = ui->lineEdit_9_1;
    m_pIpcCamPara[10][1] = ui->lineEdit_10_1;
    m_pIpcCamPara[11][1] = ui->lineEdit_11_1;

    m_pIpcCamPara[0][2] = ui->lineEdit_0_2;
    m_pIpcCamPara[1][2] = ui->lineEdit_1_2;
    m_pIpcCamPara[2][2] = ui->lineEdit_2_2;
    m_pIpcCamPara[3][2] = ui->lineEdit_3_2;
    m_pIpcCamPara[4][2] = ui->lineEdit_4_2;
    m_pIpcCamPara[5][2] = ui->lineEdit_5_2;
    m_pIpcCamPara[6][2] = ui->lineEdit_6_2;
    m_pIpcCamPara[7][2] = ui->lineEdit_7_2;
    m_pIpcCamPara[8][2] = ui->lineEdit_8_2;
    m_pIpcCamPara[9][2] = ui->lineEdit_9_2;
    m_pIpcCamPara[10][2] = ui->lineEdit_10_2;
    m_pIpcCamPara[11][2] = ui->lineEdit_11_2;

    m_pIpcCamPara[0][3] = ui->lineEdit_0_3;
    m_pIpcCamPara[1][3] = ui->lineEdit_1_3;
    m_pIpcCamPara[2][3] = ui->lineEdit_2_3;
    m_pIpcCamPara[3][3] = ui->lineEdit_3_3;
    m_pIpcCamPara[4][3] = ui->lineEdit_4_3;
    m_pIpcCamPara[5][3] = ui->lineEdit_5_3;
    m_pIpcCamPara[6][3] = ui->lineEdit_6_3;
    m_pIpcCamPara[7][3] = ui->lineEdit_7_3;
    m_pIpcCamPara[8][3] = ui->lineEdit_8_3;
    m_pIpcCamPara[9][3] = ui->lineEdit_9_3;
    m_pIpcCamPara[10][3] = ui->lineEdit_10_3;
    m_pIpcCamPara[11][3] = ui->lineEdit_11_3;

    m_pIpcCamPara[0][4] = ui->lineEdit_0_4;
    m_pIpcCamPara[1][4] = ui->lineEdit_1_4;
    m_pIpcCamPara[2][4] = ui->lineEdit_2_4;
    m_pIpcCamPara[3][4] = ui->lineEdit_3_4;
    m_pIpcCamPara[4][4] = ui->lineEdit_4_4;
    m_pIpcCamPara[5][4] = ui->lineEdit_5_4;
    m_pIpcCamPara[6][4] = ui->lineEdit_6_4;
    m_pIpcCamPara[7][4] = ui->lineEdit_7_4;
    m_pIpcCamPara[8][4] = ui->lineEdit_8_4;
    m_pIpcCamPara[9][4] = ui->lineEdit_9_4;
    m_pIpcCamPara[10][4] = ui->lineEdit_10_4;
    m_pIpcCamPara[11][4] = ui->lineEdit_11_4;

    m_curIpcIndex = 0;
    ui->label_v_move->hide();
    ui->label_v_width->hide();
    ui->label_ipcSavePic->hide();
    struGsh.ipcSavePicFlag = 0;
}

IpcCameraSetWidget::~IpcCameraSetWidget()
{
    delete ui;
}

void IpcCameraSetWidget::showPage(bool bshow)
{
    basewidget::showPage(bshow);
    if(bshow){
        updateIpcBtn();
        updateView();
    }
    else
    {
        if (struCnfg.nDPMachine == 1)
        {
//            updateIpcCameraId();
            myFlow.configIpcCamera();
        }
    }
}
void IpcCameraSetWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){
        if (msg2 == 51){
//            if ((struCnfg.nDPMachine == 1)&&(struCnfg.nIsCameraConpress == 0))
//            {
//                myFlow.configIpcCamera();
//            }
        }
    }
}

void IpcCameraSetWidget::OnViewChange(int view)
{
//    ui->widget->update();
    updateView();
}

void IpcCameraSetWidget::OnChuteChange()
{
    updateView();
}

void IpcCameraSetWidget::OnLayerChange()
{
//    ui->widget->update();
    updateView();
}

void IpcCameraSetWidget::updateCamState()
{
//    for(int i = 0;i < struCnfg.nIpcCamera;i++){
//        m_pIpcCamTitle[i]->setVisible(i<struIpcShare.struIpcInfo[m_curIpcIndex].cameraCount);
//        m_pIpcCamPara[i][0]->setVisible(i<struIpcShare.struIpcInfo[m_curIpcIndex].cameraCount);
//        m_pIpcCamPara[i][1]->setVisible(i<struIpcShare.struIpcInfo[m_curIpcIndex].cameraCount);
//        m_pIpcCamPara[i][2]->setVisible(i<struIpcShare.struIpcInfo[m_curIpcIndex].cameraCount);
//        m_pIpcCamPara[i][3]->setVisible(i<struIpcShare.struIpcInfo[m_curIpcIndex].cameraCount);

//        m_pIpcCamPara[i][0]->setText(QString::number(struCnfg.struIpcCameraParams[i].nHoriOffset));
//        m_pIpcCamPara[i][1]->setText(QString::number(struCnfg.struIpcCameraParams[i].nHoriWidth));
//        m_pIpcCamPara[i][2]->setText(QString::number(struCnfg.struIpcCameraParams[i].nVertOffset));
//        m_pIpcCamPara[i][3]->setText(QString::number(struCnfg.struIpcCameraParams[i].nVertWidth));

//        m_pIpcCamPara[i][0]->setMinMax(this,0,255,51,&struCnfg.struIpcCameraParams[i].nHoriOffset);
//        m_pIpcCamPara[i][1]->setMinMax(this,0,255,51,&struCnfg.struIpcCameraParams[i].nHoriWidth);
//        m_pIpcCamPara[i][2]->setMinMax(this,0,255,51,&struCnfg.struIpcCameraParams[i].nVertOffset);
//        m_pIpcCamPara[i][3]->setMinMax(this,0,255,51,&struCnfg.struIpcCameraParams[i].nVertWidth);
//    }
}
void IpcCameraSetWidget::ipcBtnClicked(int ipcindex)
{
    m_curIpcIndex = ipcindex;
    updateIpcBtn();
}

void IpcCameraSetWidget::ipcTypeGroupClicked(int cameraIndex)
{
    struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][cameraIndex].ipcType = m_pIpcTypeCmx[cameraIndex]->currentIndex();
    updateView();
}

void IpcCameraSetWidget::ipcIpEditClicked(int cameraIndex)
{
    QStringList ipList;
    QString ipStr;
    int tmpAddr[4] = {0};

    ipStr = m_pIpcIpAddr[cameraIndex]->text();
    ipList = ipStr.split(".");

    if(ipList.length() != 4)
    {
        g_infoWidget().setLabelText(g_myLan().dc_save_erro);
        g_infoWidget().delayShow();
        myFlow.sleep(2);
        g_infoWidget().hide();
        return;
    }

    for(int i = 0; i < 4; i++)
    {
        tmpAddr[i] = ipList.at(i).toInt(0, 10);
        if((tmpAddr[i] < 0) && (tmpAddr[i] > 255))
        {
            g_infoWidget().setLabelText(g_myLan().dc_save_erro);
            g_infoWidget().delayShow();
            myFlow.sleep(2);
            g_infoWidget().hide();
            break;
        }
    }

    for(int i = 0; i < 4; i++)
    {
        struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][cameraIndex].nIpAddr[i] = tmpAddr[i];
    }
    //23/6/21　重新启用，ip 1,2,3对应cmdid 0,1,2
    if(tmpAddr[3] > 0)
    {
        struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][cameraIndex].cameraId = tmpAddr[3]-1;
    }

    //相机ip是齐对上位机的唯一属性，不能出错
    myFlow.ipcResetCameraIpaddr(false, struGsh.nLayer, struGsh.nView, cameraIndex);
}

void IpcCameraSetWidget::ipcSavePicEnabled(int cameraIndex)
{
    struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][cameraIndex].nSaveImgEnable =
            (struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][cameraIndex].nSaveImgEnable == 0 ? 1 : 0);
    updateView();
}

void IpcCameraSetWidget::updateIpcBtn()
{
    if (m_curIpcIndex >= MAX_IPC_NUM)
        m_curIpcIndex = 0;
}
void IpcCameraSetWidget::retranslateUiWidget()
{
    for(int i = 0; i < CONST_MAX_CAMERA_PER_VIEW;i++)
        m_pIpcCamTitle[i]->setText(g_myLan().ids_camera+QString::number(i+1));

    ui->labelIpcCount->setText("IPC"+g_myLan().ipc_toalCount);
    ui->label_ip->setText("IP");
    ui->label_h_move->setText(g_myLan().ipc_h_move);
    ui->label_h_width->setText(g_myLan().ipc_h_width);
    ui->label_h_width_2->setText(g_myLan().ipc_h_width+"2");
    ui->label_v_move->setText(g_myLan().ipc_v_move);
    ui->label_v_width->setText(g_myLan().ipc_v_width);
    if(struGsh.ipcSavePicFlag == 1){
        ui->ipcSavePicBtn->setText(g_myLan().ai_image_capturing);
    } else{
        ui->ipcSavePicBtn->setText("IPC"+g_myLan().ids_getImage);
    }
}

void IpcCameraSetWidget::updateView()
{
    int i, j,nHoriWidth_1,nHoriWidth_2;

    ui->lineEditIpcCount->setText(QString::number(struCnfg.nIpcCount));
    ui->lineEditIpcCount->setMinMax(this,1,MAX_IPC_NUM,61,&struCnfg.nIpcCount);

    ui->label_ipc_num->hide();

    ui->comboBox_camera_same->clear();
    QStringList ipcCameraClassSameList;
    ipcCameraClassSameList<<g_myLan().all_seperate<<g_myLan().ids_masterSame<<g_myLan().ids_allSame;
    ui->comboBox_camera_same->addItems(ipcCameraClassSameList);
    ui->comboBox_camera_same->setCurrentIndex(struCnfp.nIpcCameraClassSameMode);

    ui->comboBox_cam1_ipc->clear();
    QStringList ipcTypeList;

    ipcTypeList<<"NULL"<<"IPC1";
    ui->comboBox_cam1_ipc->addItems(ipcTypeList);

    for(i = 0; i < CONST_MAX_CAMERA_PER_VIEW; i++)
    {
        m_pIpcCamTitle[i]->show();
        m_pIpcTypeCmx[i]->show();

        m_pIpcCamTitle[i]->show();
        m_pIpcTypeCmx[i]->show();
        m_pIpcIpAddr[i]->show();
        m_pIpcSavePicEnable[i]->show();
    }

    for(i = 0; i < CONST_MAX_CAMERA_PER_VIEW; i++)
    {
        if(i >= struCnfg.struLayerInfo[struGsh.nLayer].nViewUnitTotal[struGsh.nView])
        {
            m_pIpcCamTitle[i]->hide();
            m_pIpcTypeCmx[i]->hide();
            m_pIpcIpAddr[i]->hide();
            m_pIpcSavePicEnable[i]->hide();
            for(j = 0; j < 5; j++)
            {
                m_pIpcCamPara[i][j]->hide();
            }
        }
        else
        {
            m_pIpcIpAddr[i]->hide();
            for(j = 0; j < 5; j++)
            {
                m_pIpcCamPara[i][j]->hide();
            }
        }
    }

    for(i = 0; i < CONST_MAX_CAMERA_PER_VIEW; i++)
    {
        m_pIpcTypeCmx[i]->clear();
        m_pIpcTypeCmx[i]->addItems(ipcTypeList);
    }

    for(i = 0; i < struCnfg.struLayerInfo[struGsh.nLayer].nViewUnitTotal[struGsh.nView]; i++)
    {
        if(i >= CONST_MAX_CAMERA_PER_VIEW) continue;
        m_pIpcTypeCmx[i]->setCurrentIndex(struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][i].ipcType);
        m_pIpcSavePicEnable[i]->setChecked(struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][i].nSaveImgEnable);
    }

    for(i = 0; i < struCnfg.struLayerInfo[struGsh.nLayer].nViewUnitTotal[struGsh.nView] ; i++)
    {
        if(i >= CONST_MAX_CAMERA_PER_VIEW) continue;
        QString strIp = QString("%1.%2.%3.%4").arg(struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][i].nIpAddr[0])
                .arg(struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][i].nIpAddr[1])
                .arg(struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][i].nIpAddr[2])
                .arg(struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][i].nIpAddr[3]);
        m_pIpcIpAddr[i]->setText(strIp);
        if(struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][i].ipcType != IPC_CAMERA_TYPE_NULL)
        {
            m_pIpcIpAddr[i]->show();
        }
    }

    for(i = 0;i < struCnfg.struLayerInfo[struGsh.nLayer].nViewUnitTotal[struGsh.nView]; i++)
    {
        if(i >= CONST_MAX_CAMERA_PER_VIEW) continue;
        for(j = 0; j < 5; j++)
        {
            if(struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][i].ipcType != IPC_CAMERA_TYPE_NULL)
            {
                m_pIpcCamPara[i][j]->setVisible(j<2 || j>3);
            }
        }

        m_pIpcCamPara[i][0]->setText(QString::number(struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][i].nHoriOffset));
        m_pIpcCamPara[i][1]->setText(QString::number(struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][i].nHoriWidth));
        m_pIpcCamPara[i][2]->setText(QString::number(struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][i].nVertOffset));
        m_pIpcCamPara[i][3]->setText(QString::number(struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][i].nVertWidth));
        m_pIpcCamPara[i][4]->setText(QString::number(struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][i].nHoriWidth_2));

        m_pIpcCamPara[i][0]->setMinMax(this,0,2047,51,&struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][i].nHoriOffset);
        m_pIpcCamPara[i][1]->setMinMax(this,0,2047,51,&struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][i].nHoriWidth);
        m_pIpcCamPara[i][2]->setMinMax(this,0,2047,51,&struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][i].nVertOffset);
        m_pIpcCamPara[i][3]->setMinMax(this,0,2047,51,&struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][i].nVertWidth);
        m_pIpcCamPara[i][4]->setMinMax(this,0,2047,51,&struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][i].nHoriWidth_2);
    }
}

/* 获取该ipc起始的逻辑ip */
int IpcCameraSetWidget::getIpcBasicCamId(int ipcIndex)
{
    int i, j, k;
    int ipcBasicCamId = 257;
    int tmpIp = 0;

    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(!struCnfg.struLayerInfo[i].nViewIsUsed[j])
                continue;
            for(k = 0; k < struCnfg.struLayerInfo[i].nViewUnitTotal[j]; k++)
            {
                if(struCnfg.struIpcCameraParams[i][j][k].ipcType == IPC_CAMERA_TYPE_IPC1 + ipcIndex)
                {
                    tmpIp = struCnfg.struIpcCameraParams[i][j][k].nIpAddr[2]*256+struCnfg.struIpcCameraParams[i][j][k].nIpAddr[3];
                    if( tmpIp > 256 && struCnfg.struIpcCameraParams[i][j][k].nIpAddr[3] < 255)
                    {
                        if(tmpIp < ipcBasicCamId)
                        {
                            ipcBasicCamId = tmpIp;
                        }
                    }
                }
            }
        }
    }
    return ipcBasicCamId;
}

/* 早期ip随意配置时需考虑统一处理转换成camid(通过ipc ipaddr 偏移)　*/
void IpcCameraSetWidget::updateIpcCameraId()
{
    int m, i, j, k;
    int ipcBasicCamId = 0;
    for(m = 0; m < struCnfg.nIpcCount;m++)
    {
//        ipcBasicCamId = getIpcBasicCamId(m)%256;
        for(i = 0; i < struCnfg.nLayerTotal; i++)
        {
            for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
            {
                if(!struCnfg.struLayerInfo[i].nViewIsUsed[j])
                    continue;
                for(k = 0; k < struCnfg.struLayerInfo[i].nViewUnitTotal[j]; k++)
                {
                    if(struCnfg.struIpcCameraParams[i][j][k].ipcType == IPC_CAMERA_TYPE_IPC1 + m)
                    {
                        if(struCnfg.struIpcCameraParams[i][j][k].nIpAddr[3] > 0)
                        {
                            struCnfg.struIpcCameraParams[i][j][k].cameraId = struCnfg.struIpcCameraParams[i][j][k].nIpAddr[3] - 1;
                            //防止溢出
                            if(struCnfg.struIpcCameraParams[i][j][k].cameraId >= MAX_CAMERA_PER_IPC)
                                struCnfg.struIpcCameraParams[i][j][k].cameraId = 0;
                        }
                    }
                }
            }
        }
    }
}

void IpcCameraSetWidget::on_comboBox_camera_same_activated(int index)
{
    struCnfp.nIpcCameraClassSameMode = index;
}

void IpcCameraSetWidget::on_ipcSavePicBtn_clicked()
{
    //22/10/31 非色选状态，点击保存图片开始，需先把ROI全部设置成实际相机的最大值，再发送要保存的相机的地址图片。
    if(struGsh.bSortStart){
        g_infoWidget().setLabelText(g_myLan().msg_turn_off_first);
        g_infoWidget().delayShow();
        myFlow.sleep(1);
        g_infoWidget().hide();
        return;
    }

   if(struGsh.ipcSavePicFlag == 0){
       ui->ipcSavePicBtn->setText(g_myLan().ai_image_capturing);
       struGsh.ipcSavePicFlag = 1;
   } else{
       ui->ipcSavePicBtn->setText("IPC"+g_myLan().ids_getImage);
       struGsh.ipcSavePicFlag = 0;
   }

   myFlow.configIpcCamera();
   myFlow.ipcSaveImg(struGsh.ipcSavePicFlag);
}
