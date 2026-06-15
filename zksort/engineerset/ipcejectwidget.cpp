#include "ipcejectwidget.h"
#include "ui_ipcejectwidget.h"

enum
{
    msg_pixel_begin = 10,
    msg_pixel_end = 11
};

IpcEjectWidget::IpcEjectWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::IpcEjectWidget)
{
    ui->setupUi(this);
    m_pViewBtn[0] = ui->pushButton;
    m_pViewBtn[1] = ui->pushButton_2;
    m_pViewBtn[2] = ui->pushButton_3;
    m_pViewBtn[3] = ui->pushButton_4;
    m_pViewBtn[4] = ui->pushButton_6;
    m_pViewBtn[5] = ui->pushButton_7;
    m_pViewBtn[6] = ui->pushButton_8;
    m_pViewBtn[7] = ui->pushButton_9;
    m_pChuteWidget = ui->chuteWidget;
    m_pLayerBtn[0] = ui->mainBtn;
    m_pLayerBtn[1] = ui->viceBtn;
    setSlotConnect();

    myFlow.myNetWork->setUdpRecvType(CAPTURE_IPC_EJECT);

    connect(myFlow.myNetWork->udpthread,SIGNAL(SignalWaveRecvFinished()),this,SLOT(updateWave()));
    connect(myFlow.myNetWork->udpthread,SIGNAL(SignalSocketFailed()),this,SLOT(updateWave()));
    ui->widget->setDrawType(IPC_EJECT,true);
//    ui->pushButton_5->hide();
}

IpcEjectWidget::~IpcEjectWidget()
{
    delete ui;
}

void IpcEjectWidget::showPage(bool bshow)
{
    basewidget::showPage(bshow);
    if(bshow){
        ui->widget->setDrawType(IPC_EJECT,true);
        updateWave();
        updateValue();
        myFlow.myNetWork->setUdpRecvType(CAPTURE_IPC_EJECT);
//        myFlow.myNetWork->setUdpRecvType(CAPTURE_SINGLE_WAVE);
        myFlow.myNetWork->udpStart();
    }else{
        myFlow.myNetWork->udpStop();
    }
}

void IpcEjectWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == WAVE_MSG){
        updateWave();
    }else if(msg1 == MSG_DATA_CHANGED){
        if(msg2 == chute_change){
            basewidget::receiveMsg(msg1,msg2,msg3);
        }
        else {
            updateWave();
        }
    }
}

void IpcEjectWidget::OnViewChange(int view)
{
    updateWave();
    updateValue();
}

void IpcEjectWidget::OnChuteChange()
{
    updateValue();
}

void IpcEjectWidget::OnLayerChange()
{
    updateValue();
}

void IpcEjectWidget::retranslateUiWidget()
{
   ui->groupBox_4->setTitle("");
//   ui->startPix->setTitle(g_myLan().ids_startPixel);
//   ui->endPix->setTitle(g_myLan().ids_endPixel);
   ui->startPix->setTitle("L");
   ui->endPix->setTitle("R");
   ui->pushButton_5->setText(g_myLan().ids_exportReport);
   ui->pushButton_10->setText(g_myLan().clear);

}

void IpcEjectWidget::updateWave()
{
    ui->widget->update();
    int val = 0,total = 0;
    double per= 0.0;
    for(int i = 0; i < 1000;i++)
    {
        total += struGsh.sRowTotal[i];
        if( i > struCnfg.nIpcEndPixel[struGsh.nView][struGsh.nUnit])
            val += struGsh.sRowTotal[i];
    }
    if(total >0) per = val*100.0/total;
    ui->label->setText(QString::number(val)+"/"+QString::number(total) +"\n"
                       +"(" + QString::number(per,'f',3) +"%)");
}

void IpcEjectWidget::updateValue()
{
    ui->startPix->setParams(this,1,1024,msg_pixel_begin,&struCnfg.nIpcStartPixel[struGsh.nView][struGsh.nUnit]);
    ui->endPix->setParams(this,1,1024,msg_pixel_end,&struCnfg.nIpcEndPixel[struGsh.nView][struGsh.nUnit]);
    ui->startPix->setValue(struCnfg.nIpcStartPixel[struGsh.nView][struGsh.nUnit]);
    ui->endPix->setValue(struCnfg.nIpcEndPixel[struGsh.nView][struGsh.nUnit]);
}

void IpcEjectWidget::on_pushButton_5_clicked()
{
#if 0
    g_Runtime().mySystem("rm -f "+QString::fromLocal8Bit(IPC_EJECT_STASTICS));

    QSettings setting(IPC_EJECT_STASTICS,QSettings::IniFormat);
    for(int i =0; i < 1024;i++)
    {
        if (struGsh.sRowTotal[i] > 0)
        {
            setting.setValue(QString::number(i),struGsh.sRowTotal[i]);
        }
    }
    setting.sync();

    QString targetName;

    myInputMethod inputPanel;
    inputPanel.setTitleAndInput(g_myLan().input,"");
    inputPanel.setInputBtnText(g_myLan().ids_ok,g_myLan().ids_cancel,g_myLan().ids_delete,g_myLan().ids_space);
    if (inputPanel.exec() == QDialog::Accepted) {
        targetName = inputPanel.getText();
    }
    else{
        return;
    }
    if(targetName == "")
        targetName = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");

    if(g_Runtime().copyFileToPath(IPC_EJECT_STASTICS,QString("/sdcard/ts/%1.txt").arg(targetName),true)){
        g_infoWidget().setLabelText(g_myLan().ids_exportReport+"ok");
        g_infoWidget().delayShow();
        myFlow.sleep(2);
        g_infoWidget().hide();
    }else{
        g_infoWidget().setLabelText(g_myLan().ids_exportReport+"err");
        g_infoWidget().delayShow();
        myFlow.sleep(2);
        g_infoWidget().hide();
    }
#endif
    QString path = g_Runtime().getUsbPath();
    if (path != ""){
        QString cornPath = path+"Corn";
        g_Runtime().dirExist(cornPath);

        if(g_Runtime().copyFileToPath("/sdcard/ts/corndata.csv",cornPath+"/corndata.csv",true)){
            g_infoWidget().setLabelText(g_myLan().ids_exportReport+"ok");
            g_infoWidget().delayShow();
            myFlow.sleep(2);
            g_infoWidget().hide();
        }else{
            g_infoWidget().setLabelText(g_myLan().ids_exportReport+"err");
            g_infoWidget().delayShow();
            myFlow.sleep(2);
            g_infoWidget().hide();
        }
    }
}

void IpcEjectWidget::on_pushButton_10_clicked()
{
    myMessageBox msgBox(MSG_QUES, g_myLan().cfm_clean_sample+"?");
    int ret = msgBox.exec();
    if (ret != QDialog::Accepted) return;

    int i, j, k;
    int nInt=0, board, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];
    memset(data,0,sizeof(data));
    data[0] = 1;

    mySerial.pushCom1CmdData(CMD_CLEAR_HISTOGRAM, nInt, BOARD_TYPE_ALL_CAMERA, BOARD_TYPE_ALL_IN_CHAIN, data, 3);

    g_Runtime().mySystem("rm -f /sdcard/ts/corndata.csv");
    g_Runtime().mySystem("sync");
}
