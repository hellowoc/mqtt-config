#include "ipcinfowidget.h"
#include "ui_ipcinfowidget.h"
IPCInfoWidget::IPCInfoWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::IPCInfoWidget)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(10);
    ui->tableWidget->setColumnWidth(0,150);//电路板
    ui->tableWidget->setColumnWidth(1,250);//版本
    ui->tableWidget->setColumnWidth(2,150);//roiW/roiH/
    ui->tableWidget->setColumnWidth(3,150);//模型状态
    ui->tableWidget->setColumnWidth(4,250);//丢失行数/总行数
    ui->tableWidget->setColumnWidth(5,100);//剩余空间
    ui->tableWidget->setColumnWidth(6,150);//npu/cpu/mem
    ui->tableWidget->setColumnWidth(7,100);//temp
    ui->tableWidget->setColumnWidth(8,100);//freq
    ui->tableWidget->setColumnWidth(9,150);//piccnt
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setFocusPolicy(Qt::NoFocus);
    ui->tableWidget->setShowGrid(false);

    QHeaderView *header = ui->tableWidget->verticalHeader();
    header->setHidden(true);// 隐藏行号

    QHeaderView *horiHeader = ui->tableWidget->horizontalHeader();
    horiHeader->setFixedHeight(50);
    horiHeader->setSectionResizeMode(QHeaderView::Fixed);
    ui->search->hide();
    connect(myUpdateStatusThread,SIGNAL(sUpdateIpcInfo()),this,SLOT(updateALl()));
}

IPCInfoWidget::~IPCInfoWidget()
{
    delete ui;
}

void IPCInfoWidget::showPage(bool bshow)
{
    if(bshow){
        g_infoWidget().hide();
        updateVersionInfo();
        updateTableWidget();
    }
}

void IPCInfoWidget::retranslateUiWidget()
{
    ui->search->setText(g_myLan().recheck);
}

void IPCInfoWidget::updateVersionInfo()
{
    m_verinfoVec.clear();
    int hPixel,vPiexl;
    int offStat = 0;
    QString strName,strVer,strRoiInfo,strModelStat,strLineInfo,strFreeSpace,strCpuInfo,strTempInfo,strLineFreq,strPicCnt;

    for(int i = 0;i < MAX_VIEW;i++){
        if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[i]) continue;
        for(int j = 0;j < struCnfg.struLayerInfo[0].nViewUnitTotal[i];j++){
            if(struCnfg.struIpcCameraParams[struGsh.nLayer][i][j].ipcType != IPC_CAMERA_TYPE_NULL)
            {
                stu_ipc_info ipcInfo;
                memcpy(&ipcInfo,&struIpcShare.struIpcInfo[i][j],sizeof(ipcInfo));
                strName = myFlow.getUnitShowName(i,j);
                if(struCnfg.nXsystemBoard[i])
                    strVer = QString("%1\n%2").arg(QString::fromUtf8(ipcInfo.version)).arg(QString::fromUtf8(ipcInfo.versionLib));
                else
                    strVer = QString("%1\n%2\nv%3.%4-%5").arg(QString::fromUtf8(ipcInfo.version)).arg(QString::fromUtf8(ipcInfo.versionLib)).
                        arg(QString::number(ipcInfo.ugVer[0])).arg(QString::number(ipcInfo.ugVer[1])).arg(ipcInfo.ugDate[5],2,10,QChar('0'));

                strRoiInfo = QString("%1/%2").arg(ipcInfo.roiWidth).arg(ipcInfo.roiHeight);
                strModelStat = QString("%1/%2(%3)").arg(ipcInfo.modelStat).arg(ipcInfo.modelUploadStat).arg(ipcInfo.aliveNum );
                strLineInfo = QString("%1/%2").arg(ipcInfo.loseCamTotalLines).arg(ipcInfo.recvCamTotalLines);
                strFreeSpace = QString("%1").arg(ipcInfo.freeSize);
                strCpuInfo = QString("%1/%2/%3").arg(ipcInfo.npuUsed).arg(ipcInfo.cpuUsed).arg(ipcInfo.memUsed);
                strTempInfo = QString("%1").arg(ipcInfo.chipTemp);
                offStat = 0;
                strLineFreq = QString("%1").arg(ipcInfo.lineFreq);
                strPicCnt =  QString("%1").arg(ipcInfo.picCnt);
//                qDebug()<<i<<j<<struGsh.intefaceAlive <<struIpcShare.struIpcInfo[i][j].aliveRetryCnt<<(int)struIpcShare.struIpcReqStauts[i][j].ipcInfoReq;
                if(struGsh.intefaceAlive > 1 //arm-interface离线
                        || struIpcShare.struIpcInfo[i][j].aliveRetryCnt > 3 //cam-rk离线
                        || struIpcShare.struIpcReqStauts[i][j].ipcInfoReq == IPC_REQ_RETURN_NULL) //arm-cam离线
                    offStat = 1;

                IPCVersionInfo info(strName,strVer,strRoiInfo,strModelStat,strLineInfo,strFreeSpace,strCpuInfo,strTempInfo,strLineFreq,strPicCnt,offStat);
                m_verinfoVec.push_back(info);
            }
        }
    }
}

void IPCInfoWidget::updateTableWidget()
{
    QStringList list;
    list<<g_myLan().ids_board<<g_myLan().software_version+("(AI/LIB/UG)")<<"ROI(W/H)"<<g_myLan().ids_Type+("(R/U)")<<g_myLan().compression_ratio+"/"+g_myLan().speed\
       <<"F(MB)"<<"G/C/M(%)"<<"T"+QString::fromLocal8Bit("(°C)")<<g_myLan().sensor_fre<<"IMG";
    ui->tableWidget->setHorizontalHeaderLabels(list);
    ui->tableWidget->setRowCount(m_verinfoVec.size());
    for(int i = 0;i< m_verinfoVec.size();++i){
        ui->tableWidget->setItem(i,0, new QTableWidgetItem(m_verinfoVec.at(i).name));
        ui->tableWidget->setItem(i,1, new QTableWidgetItem(m_verinfoVec.at(i).ver));
        ui->tableWidget->setItem(i,2, new QTableWidgetItem(m_verinfoVec.at(i).roiInfo));
        ui->tableWidget->setItem(i,3, new QTableWidgetItem(m_verinfoVec.at(i).modelStat));
        ui->tableWidget->setItem(i,4, new QTableWidgetItem(m_verinfoVec.at(i).lineInfo));
        ui->tableWidget->setItem(i,5, new QTableWidgetItem(m_verinfoVec.at(i).freeSpace));
        ui->tableWidget->setItem(i,6, new QTableWidgetItem(m_verinfoVec.at(i).cpuInfo));
        ui->tableWidget->setItem(i,7, new QTableWidgetItem(m_verinfoVec.at(i).tempInfo));
        ui->tableWidget->setItem(i,8, new QTableWidgetItem(m_verinfoVec.at(i).lineFreq));
        ui->tableWidget->setItem(i,9, new QTableWidgetItem(m_verinfoVec.at(i).picCnt));

        for(int j= 0;j < 10;j++){
            ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            ui->tableWidget->item(i,j)->setFlags(Qt::ItemIsEnabled);
            if(j== 1)
            {
                ui->tableWidget->item(i,j)->setFont(QFont("微软雅黑",15));
                if(m_verinfoVec.at(i).ver.contains("-"))
                    ui->tableWidget->setRowHeight(i,60);
            }
            if(m_verinfoVec.at(i).offlineStat == 1)
            {
                ui->tableWidget->item(i,j)->setTextColor(Qt::red);
            }
        }
    }
    ui->label->setText(QString::number(struIpcShare.struIpcInfo[struGsh.nView][struGsh.nUnit].aliveNum)+"/"
            + QString::number(struIpcShare.struIpcInfo[struGsh.nView][struGsh.nUnit].aliveRetryCnt));
}
void IPCInfoWidget::updateALl()
{
    updateVersionInfo();
    updateTableWidget();
}

#if 1
void IPCInfoWidget::on_search_clicked()
{
    //xiewj:色选时不允许查询版本，as此时正在连续采图
    g_infoWidget().setLabelText(g_myLan().msg_require_version);
    g_infoWidget().delayShow();

    myFlow.getIpcInfo(true,0,0);

    g_infoWidget().hide();
    updateVersionInfo();
    updateTableWidget();

}
#endif
