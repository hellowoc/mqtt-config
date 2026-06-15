#include "imagefastcapturewidget.h"
#include "ui_imagefastcapturewidget.h"
#include "globalflow.h"

MyFastNetWorkThread *myFastNetThread;

ImageFastCaptureWidget::ImageFastCaptureWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::ImageFastCaptureWidget)
{
    ui->setupUi(this);
    m_curFilePath = QString("");
    ui->lineEdit_time->setMinMax(this,1,25,1,&struGsh.nImageCapTimeout);
    ui->lineEdit_threshold->setMinMax(this,0,500,2,&struGsh.nImageCapTrigValue);
    ui->lineEdit_imgHeight->setMinMax(this,416,832,3,&myFlow.imgHeight);
    ui->lineEdit_imgCount->setMinMax(this,1,300,4,&struGsh.imgCaptureCnt);
    ui->lineEdit_sendDelay->setMinMax(this,200,1000,5,&struGsh.nImageCapDelayTime);

    ui->lineEdit_thresArea->setMinMax(this,0,65535,6,&struCnfg.trigArea);
    ui->lineEdit_cutHeight->setMinMax(this,0,415,7,&struCnfg.cutHeight);
    ui->lineEdit_imgDim->setMinMax(this,1,4,8,&struCnfg.imgDim);
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
    connect(myFastNetThread,SIGNAL(SignalImageRecvFinished(QStringList,QStringList)),this,SLOT(updateImage(QStringList,QStringList)));
    connect(myFastNetThread,SIGNAL(SignalImageRecvFull(int,int)),this,SLOT(doImageRecvFull(int,int)));
    connect(myFastNetThread,SIGNAL(SignalFRImageRecvOver()),this,SLOT(doFRImageRecvOver()));

    struGsh.bFilterBackground = 0;
    m_curCapType= 0;
    m_pCapBtn[0] = ui->captureBtn;
    m_pCapBtn[1] = ui->syncCaptureBtn;
    m_pCapBtn[2] = ui->targetCaptureBtn;

    ui->captureBtn->setCheckable(true);
    ui->syncCaptureBtn->setCheckable(true);
    ui->targetCaptureBtn->setCheckable(true);
    ui->label_6->hide();
    ui->lineEdit_imgCount->hide();
    ui->checkBox1->setCheckable(true);
    ui->label_3->setText("");
    ui->label_7->hide();
    ui->label_8->hide();
    ui->label_9->hide();
    ui->lineEdit_thresArea->hide();
    ui->lineEdit_cutHeight->hide();
    ui->lineEdit_imgDim->hide();
}

ImageFastCaptureWidget::~ImageFastCaptureWidget()
{
    delete ui;
}

void ImageFastCaptureWidget::showPage(bool bshow)
{
    basewidget::showPage(bshow);
    if(bshow){
        //myFastNetThread->isrunning = false;
        //myFastNetThread->setUdpRecvType(FAST_CAPTURE_IMAGE);
        struGsh.bUpdateState = 1;
        updateCapBtnShow();
        ui->lineEdit_time->setText(QString("%1").arg(struGsh.nImageCapTimeout));
        ui->lineEdit_threshold->setText(QString("%1").arg(struGsh.nImageCapTrigValue));
        ui->lineEdit_imgHeight->setText(QString("%1").arg(myFlow.imgHeight));
        ui->lineEdit_imgCount->setText(QString("%1").arg(struGsh.imgCaptureCnt));
        ui->lineEdit_sendDelay->setText(QString("%1").arg(struGsh.nImageCapDelayTime));

        ui->lineEdit_thresArea->setText(QString("%1").arg(struCnfg.trigArea));
        ui->lineEdit_cutHeight->setText(QString("%1").arg(struCnfg.cutHeight));
        ui->lineEdit_imgDim->setText(QString("%1").arg(struCnfg.imgDim));
        ui->checkBox1->setChecked(struGsh.bFilterBackground);
        ui->checkBox->setChecked(struCnfg.autoTargetImage);
        struGsh.bContinuous = false;

        ui->targetCaptureBtn->setChecked(myFastNetThread->isrunning == true
                && myFastNetThread->currenttype == FAST_TARGET_CAPTURE_IMAGE);

        updateBtnState();
        if(struCnfg.nFreeSpace < 100.0){
            g_infoWidget().setLabelText(g_myLan().msg_memory_space_inadequate);
            g_infoWidget().showSecs();
        }
    }
    else{
        if( myFastNetThread->isrunning == true ){
            qDebug()<<"captured terminate - show false";
        }

        if(struCnfg.bMainpageShow == 0 && struCnfg.autoTargetImage == 0)
            myFastNetThread->isrunning = false;

        if( myFastNetThread->currenttype == FAST_CAPTURE_IMAGE)
            myFastNetThread->udpSendTransStartStop(myFastNetThread->Stop,FAST_CAPTURE_IMAGE,struGsh.nView,struGsh.nUnit);
        else if( myFastNetThread->currenttype == FAST_FR_CAPTURE_IMAGE)
            myFastNetThread->udpSendTransStartStop(myFastNetThread->Stop,FAST_FR_CAPTURE_IMAGE,BOARD_TYPE_ALL_CAMERA,BOARD_TYPE_ALL_IN_CHAIN);

        struGsh.bContinuous = false;
        ui->captureBtn->setChecked(false);
        ui->syncCaptureBtn->setChecked(false);
        struGsh.bUpdateState = 0;
    }
}

void ImageFastCaptureWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){
        if(msg2 == chute_change){
            basewidget::receiveMsg(msg1,msg2,msg3);
            if(myFastNetThread->isrunning){
                myFastNetThread->isrunning = false;
                myFastNetThread->udpSendTransStartStop(myFastNetThread->Stop,FAST_FR_CAPTURE_IMAGE,BOARD_TYPE_ALL_CAMERA,BOARD_TYPE_ALL_IN_CHAIN);
                g_infoWidget().setLabelText(g_myLan().stopFRCapturing);
                g_infoWidget().delayShow();
                myFlow.sleep(2);
                g_infoWidget().hide();
                ui->captureBtn->setChecked(false);
                ui->syncCaptureBtn->setChecked(false);
                setlrBtnEnabled(true);
            }
        }else if(msg2 == 1){

            //todo 超时时间 m_time
        }else if(msg2 == 2){
            //todo 更改阈值m_threshold
        }else if(msg2 == 3){
            if(myFlow.imgHeight != 416 && myFlow.imgHeight != 832 && myFlow.imgHeight != 1088){
                myFlow.imgHeight = 832;
                ui->lineEdit_imgHeight->setText(QString::number(myFlow.imgHeight));
            }
        }
    }
}
void ImageFastCaptureWidget::setlrBtnEnabled(bool stat)
{
    ui->pushButton_l->setEnabled(stat);
    ui->pushButton_r->setEnabled(stat);
}

void ImageFastCaptureWidget::retranslateUiWidget()
{
    ui->captureBtn->setText(g_myLan().ids_getImage);
    ui->label_2->setText(g_myLan().ai_trigger_threshold);
    ui->syncCaptureBtn->setText(g_myLan().front_rear_view);
    ui->label_4->setText(g_myLan().delay);
    ui->label_5->setText(g_myLan().ids_size);
    ui->label_6->setText(g_myLan().imgCapTureCount);
    ui->checkBox1->setText(g_myLan().filter_background);
    ui->label->setText(g_myLan().max_latency+"/s");
    ui->analysisBtn->setText(g_myLan().ai_analysis);
    ui->checkBox2->setText("rectify");
    ui->checkBox->setText("Auto");
}

void ImageFastCaptureWidget::updateImage(QStringList viewList, QStringList saveImgCntList)
{
    QString str;
    QStringList list;
    int view;
    for(int i = 0; i< viewList.size();i++)
    {
        view = viewList.at(i).toInt();
        switch (myFastNetThread->currenttype)
        {
        case FAST_CAPTURE_IMAGE:
            str += "V"+viewList.at(i)+g_myLan().imgCapTureCount + ":" + saveImgCntList.at(i);
            str +="\n";
            break;
        case FAST_FR_CAPTURE_IMAGE:
            list = saveImgCntList.at(i).split("-");
            str += "V"+viewList.at(i)+" ";
            for(int j = 0;j < struCnfg.struLayerInfo[struGsh.nLayer].nViewUnitTotal[view];j++)
            {
                if(struCnfg.struIpcCameraParams[struGsh.nLayer][view][j].nSaveImgEnable)
                {
                    str += QString::number(j+1)+":"+list.at(j)+
                            "("+QString::number(struGsh.nCamCurImgCnt[view][j])+"/"+
                            QString::number(struGsh.nCamMaxImgCnt[view][j])+")"+" ";
                }

            }
//            str += "V"+viewList.at(i)+g_myLan().imgCapTureCount + ":" + saveImgCntList.at(i)+
//                    "("+QString::number(struGsh.nCamCurImgCnt[view][struGsh.nUnit])+"/"+
//                    QString::number(struGsh.nCamMaxImgCnt[view][struGsh.nUnit])+")";
            str +="\n";
            break;
        case FAST_TARGET_CAPTURE_IMAGE:
            list = saveImgCntList.at(i).split("-");
            str += "V"+viewList.at(i)+" ";
            for(int j = 0;j < struCnfg.struLayerInfo[struGsh.nLayer].nViewUnitTotal[view];j++)
            {
                if(struCnfg.struIpcCameraParams[struGsh.nLayer][view][j].nSaveImgEnable)
                {
                    str += QString::number(j+1)+":"+list.at(j)+ " ";
                }
            }
            str +="\n";
            break;
        }
    }
    ui->label_3->setText(str);
}
void ImageFastCaptureWidget::doImageRecvFull(int view, int unit)
{
    QString str;
//    myFastNetThread->udpSendTransStartStop(myFastNetThread->Stop,FAST_FR_CAPTURE_IMAGE,BOARD_TYPE_ALL_CAMERA);
//    myFastNetThread->isrunning = false;
    if(struCnfg.nFreeSpace <= 100.0){
        str = g_myLan().msg_memory_space_inadequate;
    }
    else{
        str = "V"+QString::number(view)+"Full";
    }
    g_infoWidget().setLabelText(str);
    g_infoWidget().delayShow();
    myFlow.sleep(2);
    g_infoWidget().hide();
    ui->syncCaptureBtn->setChecked(false);
}
void ImageFastCaptureWidget::doFRImageRecvOver()
{
    myFastNetThread->udpSendTransStartStop(myFastNetThread->Stop,FAST_FR_CAPTURE_IMAGE,BOARD_TYPE_ALL_CAMERA,BOARD_TYPE_ALL_IN_CHAIN);
    myFastNetThread->isrunning = false;
    g_infoWidget().setLabelText(g_myLan().stopFRCapturing);
    g_infoWidget().delayShow();
    myFlow.sleep(2);
    g_infoWidget().hide();
    ui->syncCaptureBtn->setChecked(false);
}

//单视连续采图
void ImageFastCaptureWidget::on_captureBtn_clicked(bool checked)
{
    struGsh.nImageCaptureIsRun = checked;

    if(checked){
        setlrBtnEnabled(false);
        myFastNetThread->setUdpRecvType(FAST_CAPTURE_IMAGE,struGsh.nLayer,struGsh.nView,struGsh.nUnit);
        myFastNetThread->isrunning = true;
        myFastNetThread->start();
    }else{
        myFastNetThread->udpSendTransStartStop(myFastNetThread->Stop,FAST_CAPTURE_IMAGE,struGsh.nView,struGsh.nUnit);
        myFastNetThread->isrunning = false;
        g_infoWidget().setLabelText(g_myLan().stopCapturing);
        g_infoWidget().delayShow();
        myFlow.sleep(2);
        g_infoWidget().hide();
        setlrBtnEnabled(true);
    }
}

//前后视采图
void ImageFastCaptureWidget::on_syncCaptureBtn_clicked(bool checked)
{
    struGsh.nImageCaptureIsRun = checked;

    if(checked){
        setlrBtnEnabled(false);
        myFastNetThread->setUdpRecvType(FAST_FR_CAPTURE_IMAGE,struGsh.nLayer,struGsh.nView,struGsh.nUnit);
        myFastNetThread->isrunning = true;
        myFastNetThread->start();
    }else{
        qDebug()<<"sync captured terminate - hand control";
        myFastNetThread->udpSendTransStartStop(myFastNetThread->Stop,FAST_FR_CAPTURE_IMAGE,BOARD_TYPE_ALL_CAMERA,BOARD_TYPE_ALL_IN_CHAIN);
        myFastNetThread->isrunning = false;
        g_infoWidget().setLabelText(g_myLan().stopFRCapturing);
        g_infoWidget().delayShow();
        myFlow.sleep(2);
        g_infoWidget().hide();
        setlrBtnEnabled(true);
    }
}

void ImageFastCaptureWidget::updateBtnState()
{
   ui->captureBtn->setEnabled(!struGsh.bContinuous);
   ui->syncCaptureBtn->setEnabled(!struGsh.bContinuous);
   ui->pushButton_l->setEnabled(!struGsh.bContinuous);
   ui->pushButton_r->setEnabled(!struGsh.bContinuous);
}

void  ImageFastCaptureWidget::updateCapBtnShow()
{
    for(int i = 0;i < MAX_CAP_TYPE;i++)
        m_pCapBtn[i]->setVisible(i == m_curCapType);
    ui->label_4->setVisible(m_pCapBtn[0]->isVisible());
    ui->lineEdit_sendDelay->setVisible(m_pCapBtn[0]->isVisible());
    ui->fastParaAwidget->setVisible(m_curCapType== 0 || m_curCapType == 1);
    ui->fastParaBwidget->setVisible(m_curCapType== 2);
}

void ImageFastCaptureWidget::on_pushButton_l_clicked()
{
    if(m_curCapType > 0 )
        m_curCapType-- ;
    updateCapBtnShow();
}

void ImageFastCaptureWidget::on_pushButton_r_clicked()
{
    if(m_curCapType < 2 )
        m_curCapType++ ;
    updateCapBtnShow();
}

void ImageFastCaptureWidget::on_checkBox1_clicked(bool checked)
{
    struGsh.bFilterBackground = checked;
    ui->checkBox1->setChecked(struGsh.bFilterBackground);
}

void ImageFastCaptureWidget::on_analysisBtn_clicked()
{
    g_Runtime().updateTmpImgList();
    if(!g_Runtime().nimageNameVec.isEmpty())
        g_MainManager().ShowWidget(SM_IMAGE_MANAGE_WIDGET);
    else{
        myMessageBox msg(MSG_INFO,g_myLan().msg_capture_image_first,this);
        msg.exec();
    }
}

void ImageFastCaptureWidget::on_targetCaptureBtn_clicked(bool checked)
{
    struGsh.nImageCaptureIsRun = checked;

    QString str;
    if(checked){
        setlrBtnEnabled(false);
        myFastNetThread->setUdpRecvType(FAST_TARGET_CAPTURE_IMAGE,struGsh.nLayer,struGsh.nView,struGsh.nUnit);
        myFastNetThread->isrunning = true;
        myFastNetThread->start();
    }else{
        qDebug()<<"target captured terminate - hand control";
        myFastNetThread->udpSendTransStartStop(myFastNetThread->Stop,FAST_TARGET_CAPTURE_IMAGE,BOARD_TYPE_ALL_CAMERA,BOARD_TYPE_ALL_IN_CHAIN);
        myFastNetThread->isrunning = false;
        while(myRingQueue.get_size()){
            str = QString("Fig save:%1").arg(myRingQueue.get_count());
            g_infoWidget().setLabelText(str);
            g_infoWidget().delayShow();
            myFlow.msleep(100);
        }
        myFlow.sleep(2);
        g_infoWidget().hide();
        setlrBtnEnabled(true);
    }
}

void ImageFastCaptureWidget::on_checkBox_clicked(bool checked)
{
    struCnfg.autoTargetImage = (int)checked;
    ui->checkBox->setChecked(struCnfg.autoTargetImage);
}
