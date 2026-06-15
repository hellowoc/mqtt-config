#include "imagecapturewidget.h"
#include "ui_imagecapturewidget.h"
#include "globalflow.h"
ImageCaptureWidget::ImageCaptureWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::ImageCaptureWidget)
{
    ui->setupUi(this);
    m_curFilePath = QString("");
    ui->lineEdit_time->setMinMax(this,1,25,1,&struGsh.nImageCapTimeout);
    ui->lineEdit_threshold->setMinMax(this,0,500,2,&struGsh.nImageCapTrigValue);
    ui->lineEdit_imgHeight->setMinMax(this,416,832,3,&myFlow.imgHeight);
    ui->lineEdit_imgCount->setMinMax(this,1,300,4,&struGsh.imgCaptureCnt);
    m_pViewBtn[0] = ui->pushButton;
    m_pViewBtn[1] = ui->pushButton_2;
    m_pViewBtn[2] = ui->pushButton_3;
    m_pViewBtn[3] = ui->pushButton_4;
    m_pViewBtn[4] = ui->pushButton_5;
    m_pViewBtn[5] = ui->pushButton_7;
    m_pViewBtn[6] = ui->pushButton_8;
    m_pViewBtn[7] = ui->pushButton_9;

    m_pChuteWidget = ui->chuteWidget;
    m_pLayerBtn[0] = ui->mainBtn;
    m_pLayerBtn[1] = ui->viceBtn;
    setSlotConnect();
    connect(myFlow.myNetWork->udpthread,SIGNAL(SignalImageRecvFinished(int,QString)),this,SLOT(updateImage(int,QString)));
    connect(myFlow.myNetWork->udpthread,SIGNAL(showContinuousImage(QString)),this,SLOT(updateContinuousImage(QString)));

    struGsh.bFilterBackground = 0;
    m_curCapType= 0;
    m_pCapBtn[0] = ui->captureBtn;
//    m_pCapBtn[1] = ui->syncCaptureBtn;
    m_pCapBtn[1] = ui->sequentialCapture;
    m_pCapBtn[2] = ui->sequentialSyncCapture;

    ui->syncCaptureBtn->hide();
    ui->sequentialCapture->setCheckable(true);
    ui->sequentialSyncCapture->setCheckable(true);
    ui->label_6->hide();
    ui->lineEdit_imgCount->hide();
    ui->pushButton_6->hide();
    ui->checkBox1->setCheckable(true);
}

ImageCaptureWidget::~ImageCaptureWidget()
{
    delete ui;
}

void ImageCaptureWidget::showPage(bool bshow)
{
    myFlow.myNetWork->udpStop();
    basewidget::showPage(bshow);
    if(bshow){
        struGsh.bUpdateState = 1;
        memset(struGsh.colorRGBTotal,0,sizeof(struGsh.colorRGBTotal));
        if(struGsh.bRunMode == RUNMODE_ENGINEER)
        {
            m_curCapType = 3;
        }
        updateCapBtnShow();
        m_curFilePath = "";
        //采图界面关闭统计信息查询
        m_autoFeedEnable = struCnfg.nAutoStaticsEnable;
        struCnfg.nAutoStaticsEnable = 0;
        g_Runtime().updateTmpImgList();
        if(!g_Runtime().nimageNameVec.isEmpty())
            m_curFilePath = g_Runtime().nimageNameVec[0];
        if(!m_curFilePath.isEmpty()){
            if (m_curFilePath.contains("png")){ //compatible with old png
                QImage img(myFlow.getSensorPixels(struGsh.nLayer,struGsh.nView)/2, myFlow.imgHeight,QImage::Format_ARGB32);
                img.load(m_curFilePath);
                ui->imagewidget->setImage(img,struGsh.nLayer,struGsh.nView,struGsh.nUnit);
            }
            else //bmp all here
                ui->imagewidget->setImage(m_curFilePath,struGsh.nLayer,struGsh.nView,struGsh.nUnit);
        }
        else{
            QImage img(1024,myFlow.imgHeight,QImage::Format_ARGB32);
            img.fill(Qt::white);
            ui->imagewidget->setImage(img,struGsh.nLayer,struGsh.nView,struGsh.nUnit);
        }
        ui->imagewidget->showPage(bshow);
        ui->lineEdit_time->setText(QString("%1").arg(struGsh.nImageCapTimeout));

        ui->lineEdit_threshold->setText(QString("%1").arg(struGsh.nImageCapTrigValue));
        ui->lineEdit_imgHeight->setText(QString("%1").arg(myFlow.imgHeight));
        ui->lineEdit_imgCount->setText(QString("%1").arg(struGsh.imgCaptureCnt));
        ui->checkBox1->setChecked(struGsh.bFilterBackground);

        myFlow.myNetWork->setUdpRecvType(CAPTURE_MULTI_IMAGE);

        struGsh.bContinuous = false;
        updateBtnState();
        if(struCnfg.nFreeSpace < 100.0){
            g_infoWidget().setLabelText(g_myLan().msg_memory_space_inadequate);
            g_infoWidget().showSecs();
        }
    }
    else{
        myFlow.myNetWork->udpStop();
        struGsh.bContinuous = false;
        ui->sequentialCapture->setChecked(false);
        ui->sequentialCapture->setText(g_myLan().continuous);
        ui->sequentialSyncCapture->setChecked(false);
        ui->sequentialSyncCapture->setText(g_myLan().continuous_sync);
        struGsh.nImageCaptureIsRun = 0;
        struCnfg.nAutoStaticsEnable = m_autoFeedEnable;
        struGsh.bUpdateState = 0;
    }
}

void ImageCaptureWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){
        if(msg2 == chute_change){
            basewidget::receiveMsg(msg1,msg2,msg3);
        }else if(msg2 == 1){

            //todo 超时时间 m_time
        }else if(msg2 == 2){
            //todo 更改阈值m_threshold
        }else if(msg2 == 3){
            if(myFlow.imgHeight != 416 && myFlow.imgHeight != 832){
                myFlow.imgHeight = 832;
                ui->lineEdit_imgHeight->setText(QString::number(myFlow.imgHeight));
            }
            ui->imagewidget->showPage(true);
        }
    }
}

void ImageCaptureWidget::retranslateUiWidget()
{
    ui->analysisBtn->setText(g_myLan().ai_analysis);
    ui->captureBtn->setText(g_myLan().ids_getImage);
    ui->pushButton_6->setText(g_myLan().ids_exportImage);
    ui->label_2->setText(g_myLan().ai_trigger_threshold);
    ui->imagewidget->retranslateUiWidget();
    ui->syncCaptureBtn->setText(g_myLan().ids_getImage2);
    ui->sequentialCapture->setText(g_myLan().continuous);
    ui->sequentialSyncCapture->setText(g_myLan().continuous_sync);
    ui->label_5->setText(g_myLan().ids_size);
    ui->label_6->setText(g_myLan().imgCapTureCount);
    ui->checkBox1->setText(g_myLan().filter_background);
    ui->label->setText(g_myLan().max_latency+"/s");
}

void ImageCaptureWidget::on_captureBtn_clicked()
{
    //远程采图中，直接退出（正常情况下会失效个2,3秒）
    if (myFlow.myNetWork->udpthread->isrunning || struGsh.nRemoteImageCaptureIsRun) return;
    struGsh.nImageCaptureIsRun = 1;
    g_infoWidget().setLabelText(g_myLan().ai_image_capturing);
    g_infoWidget().delayShow();

    myFlow.myNetWork->setUdpRecvType(CAPTURE_MULTI_IMAGE);
    if(struGsh.imgCaptureCnt > 1){
        //预先判断下剩余空间能不能够完成采图
        QStringList options;
        options << "-c" << "cat /proc/mounts |grep sdcard |sed -n '$p' |awk '{printf $1}'";
        QProcess process;
        process.start("/bin/bash", options);
        if(process.waitForFinished()){
            QString str = process.readAll();
            QStringList spaceList = g_Runtime().getUsbSpace(str);
            QString space = spaceList.at(3);
            float value = QString(space.remove(space.length()-1,1)).toFloat();
            space = spaceList.at(3);
            if(space.contains("G")) value *=1024;
            else if (space.contains("K")) value /=1024;

            if(value < 1.5*3.1*struGsh.imgCaptureCnt){
                g_infoWidget().setLabelText(g_myLan().msg_memory_space_inadequate);
                g_infoWidget().showSecs();
                return;
            }
        }
    }
    myFlow.myNetWork->udpStart();
    memset(struGsh.colorRGBTotal,0,sizeof(struGsh.colorRGBTotal));
}

void ImageCaptureWidget::on_analysisBtn_clicked()
{
    g_Runtime().updateTmpImgList();
    if(!g_Runtime().nimageNameVec.isEmpty())
        g_MainManager().ShowWidget(SM_IMAGE_MANAGE_WIDGET);
    else{
        myMessageBox msg(MSG_INFO,g_myLan().msg_capture_image_first,this);
        msg.exec();
    }
}

void ImageCaptureWidget::updateImage(int correctWaveLines,QString filePath)
{
    if (!struGsh.nImageCaptureIsRun) return;  //表示不是采图界面触发的采图指令（mqtt）
    g_infoWidget().hide();

    qDebug()<<correctWaveLines<<myFlow.imgHeight;

    if (correctWaveLines != myFlow.imgHeight )
    {
        if( filePath != "")
        {
            QString str = "captured: " + QString::number(correctWaveLines);
            g_infoWidget().setLabelText(str);
            g_infoWidget().delayShow();
            myFlow.sleep(2);
            g_infoWidget().hide();
            m_curFilePath = filePath;
            ui->imagewidget->setImage(filePath,struGsh.nLayer,struGsh.nView,struGsh.nUnit);
        }
        else
        {
            QString str = g_myLan().ai_image_capture_error + ":" + QString::number(correctWaveLines);
            g_infoWidget().setLabelText(str);
            g_infoWidget().delayShow();
            myFlow.sleep(2);
            g_infoWidget().hide();
            QImage img(myFlow.getSensorPixels(struGsh.nLayer,struGsh.nView)/2,myFlow.imgHeight,QImage::Format_ARGB32);
            img.fill(Qt::white);
            ui->imagewidget->setImage(img,struGsh.nLayer,struGsh.nView,struGsh.nUnit);
        }
    }else{
        m_curFilePath = filePath;
        ui->imagewidget->setImage(filePath,struGsh.nLayer,struGsh.nView,struGsh.nUnit);
    }
    ui->imagewidget->showPage();
    myFlow.myNetWork->udpStop();
}

void ImageCaptureWidget::updateContinuousImage(QString filePath)
{
    g_infoWidget().hide();
    ui->imagewidget->setImage(filePath,struGsh.nLayer,struGsh.nView,struGsh.nUnit);
    ui->imagewidget->showPage();
}

void ImageCaptureWidget::on_pushButton_6_clicked()
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

    int i = 0;
    myMessageBox msgBox(MSG_QUES, g_myLan().cfm_rename_image);
    int ret = msgBox.exec();
    QStringList list = m_curFilePath.split("/");
    QString name = list.last().split(".").first();
    QString targetName("");
    if(ret == QDialog::Accepted)
    {
        myInputMethod inputPanel;
        inputPanel.setTitleAndInput(g_myLan().input,name);
        inputPanel.setInputBtnText(g_myLan().ids_ok,g_myLan().ids_cancel,g_myLan().ids_delete,g_myLan().ids_space);
        if (inputPanel.exec() == QDialog::Accepted) {
            targetName = inputPanel.getText();
        }
    }
    else
    {
        return;
    }
    if(targetName.isEmpty()){
        targetName = name;
    }

    if(g_Runtime().copyFileToPath(m_curFilePath,QString(path+"%1.bmp").arg(targetName),true)){
        g_infoWidget().setLabelText(g_myLan().ids_exportImageFinished);
        g_infoWidget().delayShow();
        myFlow.sleep(2);
        g_infoWidget().hide();
    }else{
        g_infoWidget().setLabelText(g_myLan().ai_image_save_error);
        g_infoWidget().delayShow();
        myFlow.sleep(2);
        g_infoWidget().hide();
    }
}

void ImageCaptureWidget::on_syncCaptureBtn_clicked()
{
    g_infoWidget().setLabelText(g_myLan().ai_image_capturing);
    g_infoWidget().delayShow();
    struGsh.nImageCaptureIsRun = 1;
    if(struGsh.imgCaptureCnt > 1){
        //预先判断下剩余空间能不能够完成采图
        QStringList options;
        options << "-c" << "cat /proc/mounts |grep sdcard |sed -n '$p' |awk '{printf $1}'";
        QProcess process;
        process.start("/bin/bash", options);
        if(process.waitForFinished()){
            QString str = process.readAll();
            QStringList spaceList = g_Runtime().getUsbSpace(str);
            QString space = spaceList.at(3);
            float value = QString(space.remove(space.length()-1,1)).toFloat();
            space = spaceList.at(3);
            if(space.contains("G")) value *=1024;
            else if (space.contains("K")) value /=1024;

            if(value < 1.5*3.1*struGsh.imgCaptureCnt *2){
                g_infoWidget().setLabelText(g_myLan().msg_memory_space_inadequate);
                g_infoWidget().showSecs();
                return;
            }
        }
    }
    myFlow.myNetWork->setUdpRecvType(CAPTURE_MULTI_IMAGE_SYNC);
    myFlow.myNetWork->udpStart();
    memset(struGsh.colorRGBTotal,0,sizeof(struGsh.colorRGBTotal));
}


void ImageCaptureWidget::on_sequentialCapture_clicked(bool checked)
{
    if(checked){
        struGsh.bContinuous = true;
        g_MainManager().RefreshBottomStatus();
        struGsh.nImageCaptureIsRun = 1;
        myFlow.myNetWork->setUdpRecvType(CAPTURE_IAMGE_CONTINUOUS);
        myFlow.myNetWork->udpStart();
        ui->sequentialCapture->setText(g_myLan().ids_stop);

        memset(struGsh.colorRGBTotal,0,sizeof(struGsh.colorRGBTotal));
    }else{
        struGsh.bContinuous = false;
        myFlow.myNetWork->udpStop();
        ui->sequentialCapture->setText(g_myLan().continuous);
        g_infoWidget().setLabelText(g_myLan().ids_WaitAFewSeconds);
        g_infoWidget().delayShow();
        for( int i = 0 ; i <struGsh.nImageCapTimeout;i++ )
        {
            myFlow.sleep(1);
            if(struGsh.bNetContinuousCapture != 0)
                break;
        }
        g_infoWidget().hide();
        g_MainManager().RefreshBottomStatus();
    }
    updateBtnState();
}

void ImageCaptureWidget::updateBtnState()
{
   ui->captureBtn->setEnabled(!struGsh.bContinuous);
   ui->analysisBtn->setEnabled(!struGsh.bContinuous);
   ui->pushButton_6->setEnabled(!struGsh.bContinuous);
   ui->syncCaptureBtn->setEnabled(!struGsh.bContinuous);
   ui->pushButton_l->setEnabled(!struGsh.bContinuous);
   ui->pushButton_r->setEnabled(!struGsh.bContinuous);
}

void  ImageCaptureWidget::updateCapBtnShow()
{
    ui->analysisBtn->setVisible(struGsh.bRunMode >= RUNMODE_FACTORY);
    ui->label_5->setVisible(struGsh.bRunMode >= RUNMODE_FACTORY);
    ui->lineEdit_imgHeight->setVisible(struGsh.bRunMode >= RUNMODE_FACTORY);
    ui->label->setVisible(struGsh.bRunMode >= RUNMODE_FACTORY);
    ui->lineEdit_time->setVisible(struGsh.bRunMode >= RUNMODE_FACTORY);
    ui->checkBox1->setVisible(struGsh.bRunMode >= RUNMODE_FACTORY);
    ui->analysisBtn->setVisible(struGsh.bRunMode >= RUNMODE_FACTORY);

    for(int i = 0;i < MAX_CAP_TYPE;i++)
        m_pCapBtn[i]->setVisible(i == m_curCapType);
}

void ImageCaptureWidget::on_pushButton_l_clicked()
{
    if(m_curCapType > 0 )
        m_curCapType-- ;
    updateCapBtnShow();
}

void ImageCaptureWidget::on_pushButton_r_clicked()
{
    if(m_curCapType < MAX_CAP_TYPE - 1 )
        m_curCapType++ ;
    updateCapBtnShow();
}

void ImageCaptureWidget::on_sequentialSyncCapture_clicked(bool checked)
{
    if(checked){
        struGsh.bContinuous = true;
        g_MainManager().RefreshBottomStatus();
        struGsh.nImageCaptureIsRun = 1;
        myFlow.myNetWork->setUdpRecvType(CAPTURE_IAMGE_CONTINUOUS_SYNC);
        myFlow.myNetWork->udpStart();
        ui->sequentialSyncCapture->setText(g_myLan().ids_stop);
    }else{
        struGsh.bContinuous = false;
       // myFlow.myNetWork->udpStop();
        ui->sequentialSyncCapture->setText(g_myLan().continuous_sync);
        g_infoWidget().setLabelText(g_myLan().ids_WaitAFewSeconds);
        g_infoWidget().delayShow();
        for( int i = 0 ; i <struGsh.nImageCapTimeout*4+6;i++ )
        {
            myFlow.sleep(1);
            if(struGsh.bNetContinuousCapture == 1)
                break;
        }
        g_infoWidget().hide();
        g_MainManager().RefreshBottomStatus();
    }
    updateBtnState();
}

void ImageCaptureWidget::on_checkBox1_clicked(bool checked)
{
    struGsh.bFilterBackground = checked;
    ui->checkBox1->setChecked(struGsh.bFilterBackground);
}
