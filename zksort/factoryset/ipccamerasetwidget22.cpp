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

    m_pLayerBtn[0] = ui->mainBtn;
    m_pLayerBtn[1] = ui->viceBtn;

    setSlotConnect();

    m_pIpcCamTitle[0] = ui->label_0;
    m_pIpcCamTitle[1] = ui->label_1;
    m_pIpcCamTitle[2] = ui->label_2;
    m_pIpcCamTitle[3] = ui->label_3;
    m_pIpcCamTitle[4] = ui->label_4;
    m_pIpcCamTitle[5] = ui->label_5;

    m_pIpcTypeCmx[0] = ui->comboBox_cam1_ipc;
    m_pIpcTypeCmx[1] = ui->comboBox_cam2_ipc;
    m_pIpcTypeCmx[2] = ui->comboBox_cam3_ipc;
    m_pIpcTypeCmx[3] = ui->comboBox_cam4_ipc;
    m_pIpcTypeCmx[4] = ui->comboBox_cam5_ipc;
    m_pIpcTypeCmx[5] = ui->comboBox_cam6_ipc;

    m_pIpcIpAddr[0] = ui->lineEdit_cam1_ip;
    m_pIpcIpAddr[1] = ui->lineEdit_cam2_ip;
    m_pIpcIpAddr[2] = ui->lineEdit_cam3_ip;
    m_pIpcIpAddr[3] = ui->lineEdit_cam4_ip;
    m_pIpcIpAddr[4] = ui->lineEdit_cam5_ip;
    m_pIpcIpAddr[5] = ui->lineEdit_cam6_ip;

//    ui->pushButton->setCheckable(false);
//    ui->pushButton_2->hide();
//    ui->pushButton_3->hide();

//    m_pIpcBtnGroup = new QButtonGroup(this);
//    m_pIpcBtnGroup->addButton(ui->pushButton,0);
//    m_pIpcBtnGroup->addButton(ui->pushButton_2,1);
//    m_pIpcBtnGroup->addButton(ui->pushButton_3,2);

    m_pIpcTypeGroupSigMapper = new QSignalMapper(this);
    for(int i = 0; i < CONST_MAX_CAMERA_PER_VIEW; i++){
        m_pIpcTypeGroupSigMapper->setMapping(m_pIpcTypeCmx[i], i);
        connect(m_pIpcTypeCmx[i],SIGNAL(activated(int)),m_pIpcTypeGroupSigMapper,SLOT(map()));
    }
    connect(m_pIpcTypeGroupSigMapper,SIGNAL(mapped(int)),this,SLOT(ipcTypeGroupClicked(int)));

    m_pIpcIpEditSigMapper = new QSignalMapper(this);
    for(int i = 0; i < CONST_MAX_CAMERA_PER_VIEW; i++){
        m_pIpcIpAddr[i]->setType(textType);
        m_pIpcIpEditSigMapper->setMapping(m_pIpcIpAddr[i], i);
        connect(m_pIpcIpAddr[i],SIGNAL(valueSetSignal()),m_pIpcIpEditSigMapper,SLOT(map()));
    }
    connect(m_pIpcIpEditSigMapper,SIGNAL(mapped(int)),this,SLOT(ipcIpEditClicked(int)));

    m_pIpcCamPara[0][0] = ui->lineEdit_0_0;
    m_pIpcCamPara[1][0] = ui->lineEdit_1_0;
    m_pIpcCamPara[2][0] = ui->lineEdit_2_0;
    m_pIpcCamPara[3][0] = ui->lineEdit_3_0;
    m_pIpcCamPara[4][0] = ui->lineEdit_4_0;
    m_pIpcCamPara[5][0] = ui->lineEdit_5_0;


    m_pIpcCamPara[0][1] = ui->lineEdit_0_1;
    m_pIpcCamPara[1][1] = ui->lineEdit_1_1;
    m_pIpcCamPara[2][1] = ui->lineEdit_2_1;
    m_pIpcCamPara[3][1] = ui->lineEdit_3_1;
    m_pIpcCamPara[4][1] = ui->lineEdit_4_1;
    m_pIpcCamPara[5][1] = ui->lineEdit_5_1;

    m_pIpcCamPara[0][2] = ui->lineEdit_0_2;
    m_pIpcCamPara[1][2] = ui->lineEdit_1_2;
    m_pIpcCamPara[2][2] = ui->lineEdit_2_2;
    m_pIpcCamPara[3][2] = ui->lineEdit_3_2;
    m_pIpcCamPara[4][2] = ui->lineEdit_4_2;
    m_pIpcCamPara[5][2] = ui->lineEdit_5_2;

    m_pIpcCamPara[0][3] = ui->lineEdit_0_3;
    m_pIpcCamPara[1][3] = ui->lineEdit_1_3;
    m_pIpcCamPara[2][3] = ui->lineEdit_2_3;
    m_pIpcCamPara[3][3] = ui->lineEdit_3_3;
    m_pIpcCamPara[4][3] = ui->lineEdit_4_3;
    m_pIpcCamPara[5][3] = ui->lineEdit_5_3;

    m_curIpcIndex = 0;
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
//        updateCamState();

        updateView();
    }
}
void IpcCameraSetWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){
        if (msg2 == 51){
            myFlow.configIpcCamera();
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
    if(tmpAddr[3] > 0)
    {
        switch(struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][cameraIndex].ipcType)
        {
        case IPC_CAMERA_TYPE_NULL:
            break;
        case IPC_CAMERA_TYPE_IPC1:
            struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][cameraIndex].cameraId = tmpAddr[3]-1;
            break;
        case IPC_CAMERA_TYPE_IPC2:
//            struCnfg.struIpcCameraParams[i][j][k].cameraId      = IPC_IP_IPC2_ADDR_BASE+k;
            struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][cameraIndex].cameraId = tmpAddr[3]-IPC_IP_IPC2_ADDR_BASE-1;
            break;
        }
//        qDebug("struCnfg.struIpcCameraParams[%d][%d][%d].cameraId = %d", struGsh.nLayer, struGsh.nView, cameraIndex,struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][cameraIndex].cameraId);

//        struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][cameraIndex].cameraId = tmpAddr[3]-1;
    }

    myFlow.ipcResetCameraIpaddr(false, struGsh.nLayer, struGsh.nView, cameraIndex);
}


void IpcCameraSetWidget::updateIpcBtn()
{
    if (m_curIpcIndex >= MAX_IPC_NUM)
        m_curIpcIndex = 0;
//    m_pIpcBtnGroup->button(m_curIpcIndex)->setChecked(true);
//    for(int i = 0;i <struCnfg.nIpcCount;i++){
//        m_pIpcBtnGroup->button(i)->setVisible(true);
//    }
}
void IpcCameraSetWidget::retranslateUiWidget()
{
//    ui->checkBox_bias->setText(g_myLan().camera_signal_bias_enable);
//    ui->checkBox_cali->setText(g_myLan().camera_signal_cali_enable);
//    ui->label->setText(g_myLan().ids_color);
//    ui->groupBox->setTitle(g_myLan().ids_threshold);
//    ui->groupBox_2->setTitle(g_myLan().ids_ratio);
//    ui->label_4->setText(g_myLan().ids_uplimit);
//    ui->label_3->setText(g_myLan().ids_lowlimit);
//    ui->label_5->setText(g_myLan().ids_uplimit);
//    ui->label_6->setText(g_myLan().ids_lowlimit);
}

void IpcCameraSetWidget::updateView()
{
    int i, j;

    ui->lineEditIpcCount->setText(QString::number(struCnfg.nIpcCount));
    ui->lineEditIpcCount->setMinMax(this,1,2,61,&struCnfg.nIpcCount);

    ui->comboBox_cam1_ipc->clear();
    QStringList ipcTypeList;

    ipcTypeList<<"NULL"<<"IPC1"<<"IPC2";
    ui->comboBox_cam1_ipc->addItems(ipcTypeList);

    for(i = 0; i < CONST_MAX_CAMERA_PER_VIEW; i++)
    {
        m_pIpcCamTitle[i]->show();
        m_pIpcTypeCmx[i]->show();

        m_pIpcCamTitle[i]->show();
        m_pIpcTypeCmx[i]->show();
        m_pIpcIpAddr[i]->show();
    }

    for(i = 0; i < CONST_MAX_CAMERA_PER_VIEW; i++)
    {
        if(i >= struCnfg.struLayerInfo[struGsh.nLayer].nViewUnitTotal[struGsh.nView])
        {
            m_pIpcCamTitle[i]->hide();
            m_pIpcTypeCmx[i]->hide();
            m_pIpcIpAddr[i]->hide();
            for(j = 0; j < 4; j++)
            {
                m_pIpcCamPara[i][j]->hide();
            }
        }
        else
        {
//            m_pIpcCamTitle[i]->hide();
//            m_pIpcTypeCmx[i]->hide();
            m_pIpcIpAddr[i]->hide();
            for(j = 0; j < 4; j++)
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
        m_pIpcTypeCmx[i]->setCurrentIndex(struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][i].ipcType);
    }

    for(i = 0; i < struCnfg.struLayerInfo[struGsh.nLayer].nViewUnitTotal[struGsh.nView] ; i++)
    {
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


    for(i = 0;i < struCnfg.struLayerInfo[struGsh.nLayer].nViewUnitTotal[struGsh.nView]; i++){
//        m_pIpcCamTitle[i]->setVisible(i<struIpcShare.struIpcInfo[m_curIpcIndex].cameraCount);
        for(j = 0; j < 4; j++)
        {
            if(struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][i].ipcType != IPC_CAMERA_TYPE_NULL)
            {
                 m_pIpcCamPara[i][j]->show();
            }
        }
//        m_pIpcCamPara[i][0]->setVisible(struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][i].ipcType != IPC_CAMERA_TYPE_NULL);
//        m_pIpcCamPara[i][1]->setVisible(struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][i].ipcType != IPC_CAMERA_TYPE_NULL);
//        m_pIpcCamPara[i][2]->setVisible(struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][i].ipcType != IPC_CAMERA_TYPE_NULL);
//        m_pIpcCamPara[i][3]->setVisible(struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][i].ipcType != IPC_CAMERA_TYPE_NULL);

        m_pIpcCamPara[i][0]->setText(QString::number(struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][i].nHoriOffset));
        m_pIpcCamPara[i][1]->setText(QString::number(struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][i].nHoriWidth));
        m_pIpcCamPara[i][2]->setText(QString::number(struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][i].nVertOffset));
        m_pIpcCamPara[i][3]->setText(QString::number(struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][i].nVertWidth));

        m_pIpcCamPara[i][0]->setMinMax(this,0,2047,51,&struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][i].nHoriOffset);
        m_pIpcCamPara[i][1]->setMinMax(this,0,2047,51,&struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][i].nHoriWidth);
        m_pIpcCamPara[i][2]->setMinMax(this,0,2047,51,&struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][i].nVertOffset);
        m_pIpcCamPara[i][3]->setMinMax(this,0,2047,51,&struCnfg.struIpcCameraParams[struGsh.nLayer][struGsh.nView][i].nVertWidth);
    }
}

#if 0   //待系统确定，再写
void IpcCameraSetWidget::updateIpcCameraId()
{
    int i, j, k;
    int tmpId[MAX_IPC_NUM] = 0;

    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        tmpId[i] = 0;
    }

    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            for(k = 0; k < struCnfg.struLayerInfo[i].nViewUnitTotal[j]; k++)
            {
                switch(struCnfg.struIpcCameraParams[i][j][k].ipcType)
                {
                case IPC_CAMERA_TYPE_IPC1:
                    struCnfg.struIpcCameraParams[i][j][k].cameraId = tmpId[0]++;
                    tmpId[0]++;
                    break;
                case IPC_CAMERA_TYPE_IPC2:
                    struCnfg.struIpcCameraParams[i][j][k].cameraId = tmpId[1]++;
                    tmpId[1]++;
                    break;
                case IPC_CAMERA_TYPE_IPC3:
                    struCnfg.struIpcCameraParams[i][j][k].cameraId = tmpId[2]++;
                    tmpId[2]++;
                    break;
                default:
                    break;
                }
            }
        }
    }
}
#endif
