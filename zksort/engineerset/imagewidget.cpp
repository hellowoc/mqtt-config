#include "imagewidget.h"
#include "ui_imagewidget.h"
#include <QMouseEvent>
#include <QTime>
#include "global.h"
#include "aisetdialog.h"

ImageWidget::ImageWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::ImageWidget)
{
    ui->setupUi(this);

    m_bSimulate = false;
    m_bIsPurSim = false;
    m_bRGB = true;
    m_widgetType = captureType;
    m_simpleType = NoUse;
    m_imageWidth = IMAGE_WIDTH;
    m_imageHeight = myFlow.imgHeight;

    ImageLabelHeight = (struCnfg.screenH == 768?416:280);
    m_pColorRadioBtn[0] = ui->radioBtn_origin;
    m_pColorRadioBtn[1] = ui->radioBtn_rgb;
    m_pColorRadioBtn[2] = ui->radioBtn_r;
    m_pColorRadioBtn[3] = ui->radioBtn_g;
    m_pColorRadioBtn[4] = ui->radioBtn_b;
    m_pColorRadioBtn[5] = ui->radioBtn_n;
    m_pColorRadioBtn[6] = ui->radioBtn_rgn;
    m_pColorRadioBtn[7] = ui->radioBtn_rbn;
    m_pColorRadioBtn[8] = ui->radioBtn_gbn;
    m_pColorTypeBtnMapper = new QSignalMapper(this);
    for(int i = 0;i < 9;i++){
        m_pColorTypeBtnMapper->setMapping(m_pColorRadioBtn[i],i);
        connect(m_pColorRadioBtn[i],SIGNAL(clicked()),m_pColorTypeBtnMapper,SLOT(map()));
    }
    connect(m_pColorTypeBtnMapper,SIGNAL(mapped(int)),this,SLOT(colorTypeRadioBtnClicked(int)));

    m_ZoomValue = 1;
    if(m_imageWidth*m_ZoomValue < ImageLabelWidth)
        m_CenterX = m_imageWidth/2;
    else
        m_CenterX = ImageLabelWidth/2;
    m_CenterY = ImageLabelWidth/2;

    m_matPixCount = 0;
    m_badPixCount = 0;

    ui->m_saturationSlider->setRange(0, 200);
    ui->m_saturationSlider->setValue(100);
    ui->m_saturationSlider->setTickPosition(QSlider::TicksBelow);
    ui->m_saturationSlider->setTickInterval(20);
    QString sliderStyle =
            "QSlider::groove:horizontal {"
            "    border: 1px solid #bbb;"
            "    background: white;"
            "    height: 10px;"  // 轨道高度（调粗核心）
            "    border-radius: 8px;"
            "}"
            "QSlider::handle:horizontal {"
            "    background: #666;"
            "    border: 1px solid #555;"
            "    width: 25px;"   // 手柄宽度
            "    height: 25px;"  // 手柄高度
            "    margin-top: -5px;"  // 拆分margin为单独属性，Qt4解析更友好
            "    margin-bottom: -5px;"
            "    border-radius: 15px;"
            "}"
/*            "QSlider::handle:horizontal:hover {"
            "    background: #333;"
            "}"*/;
    ui->m_saturationSlider->setStyleSheet(sliderStyle);
    m_timer = new QTimer(this);
    m_timer->setInterval(200);
    m_timer->setSingleShot(true); // 重复触发
    ui->m_saturationSlider->hide();
    ui->m_saturationValue->hide();

    connect(ui->m_saturationSlider, SIGNAL(valueChanged(int)), this, SLOT(m_saturationSlider_valueChanged(int )));
    connect(m_timer,SIGNAL(timeout()), this, SLOT(m_saturationSliderTimeOut()));
    connect(ui->m_saturationSlider, SIGNAL(sliderReleased()), this, SLOT(m_saturationSlider_released()));

    ui->m_saturationWidget->setParams(this,0,200,200, &struCnfg.imgSaturationFactor);
}

ImageWidget::~ImageWidget()
{
    delete ui;
}

void ImageWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){
        if(msg2 == 200 && struCnfg.imgSaturationEnable){
            m_saturationSliderTimeOut();
        }

        if(m_bSimulate){
            updateSvmSimulateEditValue();
            updateImage();
        }
    }
}

void ImageWidget::showPage(bool bshow)
{
    m_PointVec[0].clear();
    m_PointVec[1].clear();
    m_NoSlectPointVec[0].clear();
    m_NoSlectPointVec[1].clear();
    m_SamplePointVec[0].clear();
    m_SamplePointVec[1].clear();
    m_PointNumVec[0].clear();
    m_PointNumVec[1].clear();
    m_NoSlectPointNumVec[0].clear();
    m_NoSlectPointNumVec[1].clear();

    struGsh.nShapeSingleGoodSampCount = 0;//图片取样完未保存退出返回则丢失
    struGsh.nShapeSingleBadSampCount = 0;
    m_matPixCount = 0;
    m_badPixCount = 0;

    if(bshow){
        ui->m_saturationWidget->setValue(struCnfg.imgSaturationFactor);
        ui->m_saturationWidget->setVisible(struCnfg.imgSaturationEnable);
        ui->widget_3->setVisible(struGsh.bRunMode != RUNMODE_ENGINEER);
        ui->checkBox_2->setVisible(struGsh.bRunMode != RUNMODE_ENGINEER );
        ui->simulateCheckBox->setVisible(struGsh.bRunMode != RUNMODE_ENGINEER);
        ui->widget->setVisible(m_widgetType == analyzeType && struGsh.m_bManulStudy[0] ==1);
        ui->widget_2->setVisible(m_widgetType == analyzeType && struGsh.ntrainComplete);

        int dim = struCnfp.nSensorDim[m_imageLayer][m_imageView];
        ui->radioBtn_n->setVisible(dim>3);
        ui->radioBtn_rbn->setVisible(dim>3);
        ui->radioBtn_gbn->setVisible(dim>3);
        ui->radioBtn_rgn->setVisible(dim>3);
        m_colorType = rgb;
        m_pColorRadioBtn[m_colorType]->setChecked(true);


//        ui->m_saturationSlider->setVisible(struCnfg.imgSaturationEnable && m_widgetType == analyzeType);
//        ui->m_saturationValue->setVisible(struCnfg.imgSaturationEnable && m_widgetType == analyzeType);
//        ui->m_saturationSlider->setValue(struCnfg.imgSaturationFactor);
//        ui->m_saturationValue->setText(QString::number(struCnfg.imgSaturationFactor-100));

        ui->checkBox_No_Bg->setChecked(false);
        ui->checkBox_No_Bg->setVisible(m_widgetType != captureType);
        ui->revokeBtn->setVisible( m_widgetType == analyzeType);
        ui->checkBox->setVisible(m_widgetType != captureType);
        ui->checkBox_2->setVisible(m_widgetType == simulateType || (m_widgetType == analyzeType && struGsh.ntrainComplete));
        ui->checkBox_2->setChecked(m_bIsPurSim);
        ui->simulateCheckBox->setVisible(m_widgetType == simulateType || (m_widgetType == analyzeType && struGsh.ntrainComplete));
        ui->simulateCheckBox->setChecked(m_bSimulate);
        ui->pushButton->setVisible(m_widgetType == captureType||m_widgetType == analyzeType);
        updateShowPage();

        //    QString style = "QLabel{min-width:768px;max-width:768px;min-height:"+QString::number(myFlow.imgHeight)+
        //            "px;max-height:400px;"+QString::number(myFlow.imgHeight)+"px;background-color:gray;}";
        //    ui->imageLabel->setStyleSheet(style);
    }
}

void ImageWidget::updateSvmSimulateEditValue()
{
    ui->svmRowEdit->setMinMax(this,1,32,-1,&struGsh.nSvmRow);
    ui->svmRowEdit->setText(QString::number(struGsh.nSvmRow));
    ui->svmPerEdit->setMinMax(this,1,struGsh.nSvmRow*struGsh.nSvmRow,-1,&struGsh.nSvmPercent);
    ui->svmPerEdit->setText(QString::number(struGsh.nSvmPercent));
    ui->svmSensEdit->setMinMax(this,1,200,-1,&struGsh.nSvmSimulateSens);
    ui->svmSensEdit->setText(QString::number(struGsh.nSvmSimulateSens));
    ui->svmCutEdit->setMinMax(this,0,200,-1,&struGsh.nSvmCut);
    ui->svmCutEdit->setText(QString::number(struGsh.nSvmCut));
    ui->svmExpandLabel->setVisible(struGsh.ntrainComplete && struCnfg.n_svmType == 2);
    ui->svmExpandEdit->setVisible(struGsh.ntrainComplete && struCnfg.n_svmType == 2 );
    ui->svmExpandEdit->setMinMax(this,1,10000,-1,&struGsh.nSvmExpandValue);
    ui->svmExpandEdit->setText(QString::number(struGsh.nSvmExpandValue));
    ui->svmBrightLimitEdit->setMinMax(this,0,255,-1,&struGsh.nSvmBrightLimit);
    ui->svmBrightLimitEdit->setText(QString::number(struGsh.nSvmBrightLimit));
}

void ImageWidget::setImage(QImage image,int layer,int view,int unit)
{
    if(m_imageWidth != image.width())
          m_imageWidth = image.width();
      if(m_imageHeight != image.height())
          m_imageHeight = image.height();

      memset(oriBuf,0,sizeof(oriBuf));
      memset(bgPoint,0,sizeof(bgPoint));
      m_image = image;
      m_imageLayer = layer;
      m_imageView = view;
      m_imageUnit = unit;
      uchar* line;
      int oriIndex;

      for(int i=0; i<m_imageHeight; i++){
          oriIndex = (i*m_imageWidth)*4;
          line = image.scanLine(i);
          memcpy(oriBuf+oriIndex,line,m_imageWidth*4);
          //if(!struGsh.bContinuous){
              for(int j=0; j<m_imageWidth; j++){
                  QRgb rgb = image.pixel(j,i);
                  uchar r =qRed(rgb);
                  uchar g = qGreen(rgb);
                  uchar b = qBlue(rgb);
                  uchar n = qAlpha(rgb);
                  if(myFlow.imagePixelIsBackground(r, g, b, n, m_imageLayer, m_imageView,struGsh.nTdMode))
                      continue;
                  int sensornSplice =  struCnfp.nImageSplice[m_imageLayer][m_imageView];
                  sensornSplice = (sensornSplice == 0)? 1 : 2;
                  if(struGsh.nImgTdMode == 2 && struCnfp.nSensorPixel[0][struGsh.nView] == SENSOR_PIXELS_2048_XTD
                          && struCnfp.nViewSensorType[0][struGsh.nView] == SENSOR_G2)
                  {
                      sensornSplice = 2;
                  }
                  if(j < struCnfg.struLayerInfo[m_imageLayer].stuCameraInfo[m_imageView].nChannelBegin[m_imageUnit]/sensornSplice \
                                  || j > struCnfg.struLayerInfo[m_imageLayer].stuCameraInfo[m_imageView].nChannelEnd[m_imageUnit]/sensornSplice)
                      continue;//像元划分部分直接作为背景处理，节省计算时间
                  bgPoint[i*m_imageWidth+j] = PIXEL_MATTER;

                  struGsh.colorRGBTotal[0][r]++;
                  struGsh.colorRGBTotal[1][g]++;
                  struGsh.colorRGBTotal[2][b]++;
              }
          //}
      }
      memcpy(chgBuf, oriBuf, m_imageWidth*m_imageHeight*4);
      memcpy(disBuf, oriBuf, m_imageWidth*m_imageHeight*4);
}

void ImageWidget::setImage( QString path, int layer, int view, int unit)
{
    memset(oriBuf,0,sizeof(oriBuf));
    memset(bgPoint,0,sizeof(bgPoint));

    if (!path.contains(".bmp")) return;

    QFile myfile(path);
    myfile.open(QIODevice::ReadOnly);
    QByteArray head1 = myfile.read(sizeof(BITMAPFILEHEADER));
    QByteArray head2 = myfile.read(sizeof(BITMAPINFOHEADER));
    BITMAPINFOHEADER info;
    memcpy(&info,head2,sizeof(BITMAPINFOHEADER));

    int type = (info.biHeight&(0x80000000))>>31;
    m_imageHeight= (type == 1) ? info.biHeight*(-1) :info.biHeight;
    m_imageWidth = info.biWidth;
    qDebug()<<path<<m_imageWidth<<m_imageHeight<<info.biBitCount<<type;

    memcpy(oriBuf,myfile.read(myfile.size()-54).data(),myfile.size()-54);
    if(info.biBitCount == 24){
        memcpy(chgBuf, oriBuf, m_imageWidth*m_imageHeight*4);
        for(int i=0; i<m_imageHeight; i++){
            for(int j=0; j<m_imageWidth; j++){
                if(type == 1) //正常存图
                {
                    oriBuf[i*m_imageWidth*4+j*4+0] = chgBuf[i*m_imageWidth*3+j*3+0];
                    oriBuf[i*m_imageWidth*4+j*4+1] = chgBuf[i*m_imageWidth*3+j*3+1];
                    oriBuf[i*m_imageWidth*4+j*4+2] = chgBuf[i*m_imageWidth*3+j*3+2];
                    oriBuf[i*m_imageWidth*4+j*4+3] = 255;
                }
                else{
                    oriBuf[i*m_imageWidth*4+j*4+0] = chgBuf[(m_imageHeight-i-1)*m_imageWidth*3+j*3+0];
                    oriBuf[i*m_imageWidth*4+j*4+1] = chgBuf[(m_imageHeight-i-1)*m_imageWidth*3+j*3+1];
                    oriBuf[i*m_imageWidth*4+j*4+2] = chgBuf[(m_imageHeight-i-1)*m_imageWidth*3+j*3+2];
                    oriBuf[i*m_imageWidth*4+j*4+3] = 255;
                }
            }
        }
    }
    //---按刘博士要求的格式转换之前的bmp图片，高度为-hight---
//    if(type == 0){
//        myFlow.saveBmp(oriBuf,QString(path).toLatin1().data(),m_imageWidth,info.biHeight);
//    }

    ui->namelabel->setText(myFlow.getImageShowName(path));
    QImage image(oriBuf,m_imageWidth,m_imageHeight,QImage::Format_ARGB32);

    m_image = image;
    m_tempimage = image;
    m_oriImage = image;
    m_imageLayer = layer;
    m_imageView = view;
    m_imageUnit = unit;
    int oriIndex;
    int sensornSplice = struCnfp.nImageSplice[m_imageLayer][m_imageView];
    sensornSplice = (sensornSplice == 0)? 1 : 2;
    if(struGsh.nImgTdMode  == 2 && struCnfp.nSensorPixel[0][struGsh.nView] == SENSOR_PIXELS_2048_XTD
            && struCnfp.nViewSensorType[0][struGsh.nView] == SENSOR_G2)
    {
        sensornSplice = 2;
    }

    for(int i=0; i<m_imageHeight; i++){
        //if(!struGsh.bContinuous){
            for(int j=0; j<m_imageWidth; j++){
                uchar r = *(oriBuf+i*m_imageWidth*4+j*4+2);
                uchar g = *(oriBuf+i*m_imageWidth*4+j*4+1);
                uchar b = *(oriBuf+i*m_imageWidth*4+j*4+0);
                uchar n = *(oriBuf+i*m_imageWidth*4+j*4+3);
                if(j < struCnfg.struLayerInfo[m_imageLayer].stuCameraInfo[m_imageView].nChannelBegin[m_imageUnit]/sensornSplice \
                        || j > struCnfg.struLayerInfo[m_imageLayer].stuCameraInfo[m_imageView].nChannelEnd[m_imageUnit]/sensornSplice){
                    bgPoint[i*m_imageWidth+j] = PIXEL_BACKGROUND;
                    continue;//像元划分部分直接作为背景处理，节省计算时间
                }
                if(myFlow.imagePixelIsBackground(r, g, b, n, m_imageLayer, m_imageView,struGsh.nImgTdMode)){
                    bgPoint[i*m_imageWidth+j] = PIXEL_BACKGROUND;
                    continue;
                }
                bgPoint[i*m_imageWidth+j] = PIXEL_MATTER;

                struGsh.colorRGBTotal[0][r]++;
                struGsh.colorRGBTotal[1][g]++;
                struGsh.colorRGBTotal[2][b]++;
            }
        //}
    }
    memcpy(srcBuf, oriBuf, m_imageWidth*m_imageHeight*4);
    memcpy(chgBuf, oriBuf, m_imageWidth*m_imageHeight*4);
    memcpy(disBuf, oriBuf, m_imageWidth*m_imageHeight*4);
    myfile.close();

    if(struCnfg.imgSaturationEnable){
        m_saturationSliderTimeOut();
    }
}

void ImageWidget::updateImage()
{
    int dim = struCnfp.nSensorDim[m_imageLayer][m_imageView];
    ui->radioBtn_n->setVisible(dim>3);
    m_tempimage = m_image;

    memcpy(chgBuf, oriBuf, m_imageHeight*m_imageWidth*4);
    if(m_bSimulate){
        displaySimulate();
    }
    showGrayColor();
    QImage img(chgBuf, m_imageWidth, m_imageHeight, QImage::Format_ARGB32);
    m_tempimage = img;
    updateImageSize();
}

void ImageWidget::setWidgetType(ImageType type)
{
    m_widgetType = type;
}

void ImageWidget::mousePressEvent(QMouseEvent *event)
{
    if(!ui->imageLabel->geometry().contains(event->pos()))
        return;

    posStart = event->pos()-ui->imageLabel->pos();
    if(m_imageWidth*m_ZoomValue < ImageLabelWidth && m_imageHeight *m_ZoomValue < ImageLabelHeight){
        m_StartOriPoint.setX((posStart.x()-m_imageWidth*m_ZoomValue/2)/m_ZoomValue+m_CenterX);
        m_StartOriPoint.setY((posStart.y()-m_imageHeight*m_ZoomValue/2)/m_ZoomValue+m_CenterY);
    }else{
        if(m_imageWidth*m_ZoomValue >= ImageLabelWidth)
            m_StartOriPoint.setX((posStart.x()-ImageLabelWidth/2)/m_ZoomValue+m_CenterX);
        else
            m_StartOriPoint.setX((posStart.x()-m_imageWidth*m_ZoomValue/2)/m_ZoomValue+m_CenterX);
        if(m_imageHeight *m_ZoomValue >= ImageLabelHeight)
            m_StartOriPoint.setY((posStart.y()-ImageLabelHeight/2)/m_ZoomValue+m_CenterY);
        else
            m_StartOriPoint.setY((posStart.y()-m_imageHeight*m_ZoomValue/2)/m_ZoomValue+m_CenterY);
    }
//    if(m_imageWidth*m_ZoomValue < ImageLabelWidth){
//        m_StartOriPoint.setX((posStart.x()-m_imageWidth*m_ZoomValue/2)/m_ZoomValue+m_CenterX);
//        m_StartOriPoint.setY((posStart.y()-ImageLabelHeight/2)/m_ZoomValue+m_CenterY);
//    }else{
//        m_StartOriPoint.setX((posStart.x()-ImageLabelWidth/2)/m_ZoomValue+m_CenterX);
//        m_StartOriPoint.setY((posStart.y()-ImageLabelHeight/2)/m_ZoomValue+m_CenterY);
//    }
    updatePixColor();
}

void ImageWidget::adapteImageCenterPos()
{
    if(m_imageWidth*m_ZoomValue < ImageLabelWidth && m_imageHeight*m_ZoomValue < ImageLabelHeight){

    }else{
        if(m_imageWidth*m_ZoomValue < ImageLabelWidth){
            //        m_CenterX = m_imageWidth*m_ZoomValue/2;
        }else{
            if(m_CenterX < ImageLabelWidth/2/m_ZoomValue){
                m_CenterX = ImageLabelWidth/2/m_ZoomValue;
            }else if(m_CenterX > m_imageWidth-ImageLabelWidth/2/m_ZoomValue){
                m_CenterX = m_imageWidth-ImageLabelWidth/2/m_ZoomValue;
                if(ImageLabelWidth/2%m_ZoomValue != 0){
                    m_CenterX = m_imageWidth-ImageLabelWidth/2/m_ZoomValue-m_ZoomValue;
                }
            }
        }

        if(m_imageHeight*m_ZoomValue < ImageLabelHeight){
            //        m_CenterY = m_imageHeight*m_ZoomValue/2;
        } else {
            if(m_CenterY < ImageLabelHeight/2/m_ZoomValue){
                m_CenterY = ImageLabelHeight/2/m_ZoomValue;
            } else if( m_CenterY>m_imageHeight-ImageLabelHeight/2/m_ZoomValue){
                m_CenterY = m_imageHeight-ImageLabelHeight/2/m_ZoomValue;
                if(ImageLabelHeight/2%m_ZoomValue != 0){
                    m_CenterY = m_imageHeight-ImageLabelHeight/2/m_ZoomValue-m_ZoomValue;
                }
            }
        }
    }
}

void ImageWidget::updateZoomValueText()
{
    ui->label_zoom->setText(QString("%1X").arg(QString::number((float)m_ZoomValue,'f',1)));
}

typedef struct QNode{
    int data;
    struct QNode *next;
}QNode;

typedef struct Queue{
    struct QNode* first;
    struct QNode* last;
}Queue;

void PushQueue(Queue *queue, int data){
    QNode *p = NULL;
    p = (QNode*)malloc(sizeof(QNode));
    p->data = data;
    if(queue->first == NULL){
        queue->first = p;
        queue->last = p;
        p->next = NULL;
    }
    else{
        p->next = NULL;
        queue->last->next = p;
        queue->last = p;
    }
}

int PopQueue(Queue *queue){
    QNode *p = NULL;
    int data;
    if(queue->first == NULL){
        return -1;
    }
    p = queue->first;
    data = p->data;
    if(queue->first->next == NULL){
        queue->first = NULL;
        queue->last = NULL;
    }
    else{
        queue->first = p->next;
    }
    free(p);
    return data;
}

static int NeighborDirection[8][2] = {{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}};

void SearchNeighbor(unsigned char *bitmap, int width, int height, int *labelmap,
                    int labelIndex, int pixelIndex, Queue *queue){
    int searchIndex, i, length;
    labelmap[pixelIndex] = labelIndex;
    length = width * height;
    for(i = 0;i < 8;i++){
        searchIndex = pixelIndex + NeighborDirection[i][0] * width + NeighborDirection[i][1];
        if(searchIndex > 0 && searchIndex < length &&
                bitmap[searchIndex] == PIXEL_MATTER && labelmap[searchIndex] == 0){
            labelmap[searchIndex] = labelIndex;
            PushQueue(queue, searchIndex);
        }
    }
}

int ConnectedComponentLabeling(unsigned char *bitmap, int width, int height, int *labelmap){
    int cx, cy, index, popIndex, labelIndex = 0;
    Queue *queue = NULL;
    queue = (Queue*)malloc(sizeof(Queue));
    queue->first = NULL;
    queue->last = NULL;
    memset(labelmap, 0, width * height);
    for(cy = 1; cy < height - 1; cy++){
        for(cx = 1; cx < width - 1; cx++){
            index = cy * width + cx;
            if(bitmap[index] == PIXEL_MATTER && labelmap[index] == 0){
                labelIndex++;
                SearchNeighbor(bitmap, width, height, labelmap, labelIndex, index, queue);

                popIndex = PopQueue(queue);
                while(popIndex > -1){
                    SearchNeighbor(bitmap, width, height, labelmap, labelIndex, popIndex, queue);
                    popIndex = PopQueue(queue);
                }
            }
        }
    }
    free(queue);
    return labelIndex;
}

void ImageWidget::judgeGreyDiscolorPropotion(quint8 index)//使用比例算法
{
    int i,j;
    int sensMin = 0, sensMax = 0,svmsense = 0, nedge =0;
    float tmpValue = 0.0;
    int tmpColor;

    int unitSens = 0;
    static int nr,ng,nb,nrg,ngb,nrb,nrr,ngg,nbb,ncon;
    int labelIndex = 0;
    stu_color_arith colorArith = struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struGreyColor[index];
    stu_svm svmArith = struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[index-ARITH_SVM_A];
    switch(index){
    case ARITH_GREY_A:
    case ARITH_GREY_B:
    case ARITH_GREY_C:
    case ARITH_GREY_D:
        unitSens = struCnfp.struUnitSens[m_imageLayer][m_imageView][m_imageUnit][0].greyColor[index];
        nedge = colorArith.nEdge;
        if(colorArith.nMode == 0)
        {
            sensMin = 0;
            sensMax = colorArith.nSensLow+unitSens;
        }
        else
        {
            sensMin = ARITH_GREY_MAX_SENS-(colorArith.nSensUp+struCnfp.struUnitSens[m_imageLayer][m_imageView][m_imageUnit][0].greyColor[index]);
            sensMax = ARITH_GREY_MAX_SENS;
        }

        break;
    case ARITH_DISCOLOR_A:
    case ARITH_DISCOLOR_B:
    case ARITH_CROSS:
        unitSens = struCnfp.struUnitSens[m_imageLayer][m_imageView][m_imageUnit][0].greyColor[index];
        nedge = colorArith.nEdge;
        if(colorArith.nMode == 0)
        {
            sensMin = 0;
            sensMax = colorArith.nSensLow+unitSens;
        }
        else
        {
            sensMin = ARITH_COLOR_MAX_SENS-(colorArith.nSensUp+unitSens);
            sensMax = ARITH_COLOR_MAX_SENS;
        }
        break;
    case ARITH_SVM_A:
    case ARITH_SVM_B:
    case ARITH_SVM_C:
    case ARITH_SVM_D:
        unitSens = struCnfp.struUnitSens[m_imageLayer][m_imageView][m_imageUnit][0].svm[index-ARITH_SVM_A];
        nedge = svmArith.nEdge;
        sensMin = 0;
        svmsense = svmArith.nSens+unitSens;

        break;
    }

    memset(matSingleTotalPixels,0,sizeof(matSingleTotalPixels));
    memset(matSingleBadPixels,0,sizeof(matSingleBadPixels));
    memset(MatBordPoints,0,sizeof(MatBordPoints));
    for(j=0;j<m_imageHeight;j++)
    {
        for(i=0;i<m_imageWidth;i++)
        {
            if(pixelIsEdge(j,i,nedge)) continue;
            labelIndex = Percent_Label_Buffer[j*m_imageWidth+i];

            if(labelIndex >= 1)
            {
                int rr=srcBuf[4*j*m_imageWidth+4*i+2];
                int gg=srcBuf[4*j*m_imageWidth+4*i+1];
                int bb=srcBuf[4*j*m_imageWidth+4*i];
                int nn=srcBuf[4*j*m_imageWidth+4*i+3];

                matSingleTotalPixels[labelIndex-1]++;

                if(index <= ARITH_SVM_D){
                    if(index <= ARITH_GREY_D)   //灰度算法
                    {
                        switch(colorArith.nMain)
                        {
                        case BACKGROUND_COLOR_RED:
                            if((rr > sensMin)&&(rr <= sensMax))
                            {
                                matSingleBadPixels[labelIndex-1]++;
                            }
                            break;
                        case BACKGROUND_COLOR_GREEN:
                            if((gg > sensMin)&&(gg <= sensMax))
                            {
                                matSingleBadPixels[labelIndex-1]++;
                            }
                            break;
                        case BACKGROUND_COLOR_BLUE:
                            if((bb > sensMin)&&(bb <= sensMax))
                            {
                                matSingleBadPixels[labelIndex-1]++;
                            }
                            break;
                        case BACKGROUND_COLOR_BLACK_WHITE:
                            if((rr > sensMin)&&(gg > sensMin)&&(bb > sensMin)&&(rr <= sensMax)&&(gg <= sensMax)&&(bb <= sensMax)){
                                matSingleBadPixels[labelIndex-1]++;
                            }
                            break;
                        case BACKGROUND_COLOR_GREY:
                            if(((rr+gg+bb)/3 > sensMin) && ((rr+gg+bb)/3 <= sensMax)){

                                matSingleBadPixels[labelIndex-1]++;
                            }
                            break;
                        case BACKGROUND_COLOR_NIR:
                            if((nn > sensMin)&&(nn <= sensMax))
                            {
                                matSingleBadPixels[labelIndex-1]++;
                            }
                            break;
                        }
                    }
                    else if(index <= ARITH_CROSS)  //色差算法
                    {
                        //不满足亮度下限的直接判定为好点
                        int dim = struCnfp.nSensorDim[m_imageLayer][m_imageView];
                        switch(dim){
                        case 1:
                            if(rr < colorArith.lightLimit)
                                continue;
                            break;
                        case 2:
                            if((rr+gg) < colorArith.lightLimit*dim)
                                continue;
                            break;
                        case 3:
                            if((rr+gg+bb) < colorArith.lightLimit*dim)
                                continue;
                            break;
                        case 4:
                            if((rr+gg+bb+nn) < colorArith.lightLimit*dim)
                                continue;
                            break;
                        default:
                            break;
                        }

                        switch(colorArith.nDiffColor)
                        {
                        case 0://红绿
                                tmpValue = rr*ARITH_COLOR_MAX_SENS/(1.0*(rr+gg));
                                if((tmpValue > sensMin)&&(tmpValue <= sensMax))
                                {
                                    matSingleBadPixels[labelIndex-1]++;
                                }

                            break;
                        case 1://红蓝
                                tmpValue = rr*ARITH_COLOR_MAX_SENS/(1.0*(rr+bb));
                                if((tmpValue > sensMin)&&(tmpValue <= sensMax))
                                {
                                    matSingleBadPixels[labelIndex-1]++;
                                }
                            break;
                        case 2://绿蓝
                                tmpValue = gg*ARITH_COLOR_MAX_SENS/(1.0*(gg+bb));
                                if((tmpValue > sensMin)&&(tmpValue <= sensMax))
                                {
                                    matSingleBadPixels[labelIndex-1]++;
                                }
                            break;
                        case 3://红红外
                                tmpValue = rr*ARITH_COLOR_MAX_SENS/(1.0*(rr+nn));
                                if((tmpValue > sensMin)&&(tmpValue <= sensMax))
                                {
                                    matSingleBadPixels[labelIndex-1]++;
                                }
                            break;
                        case 4://绿红
                                tmpValue = gg*ARITH_COLOR_MAX_SENS/(1.0*(gg+nn));
                                if((tmpValue > sensMin)&&(tmpValue <= sensMax))
                                {
                                    matSingleBadPixels[labelIndex-1]++;
                                }
                            break;
                        case 5://蓝红外
                                tmpValue = bb*ARITH_COLOR_MAX_SENS/(1.0*(bb+nn));
                                if((tmpValue > sensMin)&&(tmpValue <= sensMax))
                                {
                                    matSingleBadPixels[labelIndex-1]++;
                                }
                            break;
                        }
                    }else if(index <= ARITH_SVM_D){
                        int svmIndex = index-ARITH_SVM_A;
                        nr = struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[svmIndex].nQuoR[0] == 0?struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[svmIndex].nQuoR[1]:struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[svmIndex].nQuoR[1]*(-1);
                        ng = struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[svmIndex].nQuoG[0] == 0?struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[svmIndex].nQuoG[1]:struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[svmIndex].nQuoG[1]*(-1);
                        nb = struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[svmIndex].nQuoB[0] == 0?struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[svmIndex].nQuoB[1]:struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[svmIndex].nQuoB[1]*(-1);
                        nrg = struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[svmIndex].nQuoRG[0] == 0?struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[svmIndex].nQuoRG[1]:struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[svmIndex].nQuoRG[1]*(-1);
                        ngb = struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[svmIndex].nQuoGB[0] == 0?struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[svmIndex].nQuoGB[1]:struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[svmIndex].nQuoGB[1]*(-1);
                        nrb = struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[svmIndex].nQuoRB[0] == 0?struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[svmIndex].nQuoRB[1]:struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[svmIndex].nQuoRB[1]*(-1);
                        nrr = struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[svmIndex].nQuoRR[0] == 0?struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[svmIndex].nQuoRR[1]:struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[svmIndex].nQuoRR[1]*(-1);
                        ngg = struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[svmIndex].nQuoGG[0] == 0?struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[svmIndex].nQuoGG[1]:struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[svmIndex].nQuoGG[1]*(-1);
                        nbb = struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[svmIndex].nQuoBB[0] == 0?struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[svmIndex].nQuoBB[1]:struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[svmIndex].nQuoBB[1]*(-1);
                        ncon = struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[svmIndex].nQuoCons[0] == 0?struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[svmIndex].nQuoCons[1]:struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[svmIndex].nQuoCons[1]*(-1);


                        int nSignCons = struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[svmIndex].nQuoCons[0];
                        int tmpSvmCons = ncon;
                        int unitSens = struCnfp.struUnitSens[m_imageLayer][m_imageView][m_imageArithGroup][0].svm[svmIndex];
                        int svmsense = struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[svmIndex].nSens+unitSens;
                        int nSvmCons;
                        if(struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[svmIndex].nModeSvm == 1) { //反选时，变换符号位标志
                            nSignCons = (nSignCons == 1) ? 0:1;
                        }
                       if(nSignCons == 1) {//综合正反选后，常数项为负数
                           /* 灵敏度越大，常数项绝对值越大，选的越多 */
                           if (svmsense >= 100) {
                               nSvmCons = tmpSvmCons*(1+(svmsense-100)/100.0);
                           }
                           else {   // 灵敏度越小，常数项绝对值越小，选的越小
                               nSvmCons = tmpSvmCons*(1-(100-svmsense)/100.0);
                           }
                       }
                       else {
                           /* 灵敏度越大，常数项越小，选的越多 */
                           if (svmsense >= 100) {
                               nSvmCons = tmpSvmCons*(1-(svmsense-100)/100.0);
                           }
                           else {   // 灵敏度越小，常数项越大，选的越少
                               nSvmCons = tmpSvmCons*(1+(100-svmsense)/100.0);
                           }
                       }
                       if(struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[svmIndex].nModeSvm == 0){//pos
                           if((nr*r+ng*g+nb*b+nrg*r*g+ngb*g*b+nrb*r*b+nrr*r*r+ngg*g*g+nbb*b*b+nSvmCons) < 0){
                               matSingleBadPixels[labelIndex-1]++;
                           }
                       }else{
                           if((nr*r+ng*g+nb*b+nrg*r*g+ngb*g*b+nrb*r*b+nrr*r*r+ngg*g*g+nbb*b*b+nSvmCons) > 0){
                               matSingleBadPixels[labelIndex-1]++;
                           }
                       }
                    }
                }else if(index == ARITH_LONG_SHORT || index == ARITH_CIRCLE_LONG){//获取上下左右的边界点坐标，用于计算选长选短和选圆选长
                    //left
                    if(MatBordPoints[labelIndex-1].left_x == 0 && MatBordPoints[labelIndex-1].left_y ==0){
                        MatBordPoints[labelIndex-1].left_x = i;
                        MatBordPoints[labelIndex-1].left_y = j;
                    }else{
                        if(MatBordPoints[labelIndex-1].left_x > i){
                            MatBordPoints[labelIndex-1].left_x = i;
                            MatBordPoints[labelIndex-1].left_y = j;
                        }
                    }

                    //top
                    if(MatBordPoints[labelIndex-1].top_x == 0 && MatBordPoints[labelIndex-1].top_y ==0){
                        MatBordPoints[labelIndex-1].top_x = i;
                        MatBordPoints[labelIndex-1].top_y = j;
                    }else{
                        if(MatBordPoints[labelIndex-1].top_y > j){
                            MatBordPoints[labelIndex-1].top_x = i;
                            MatBordPoints[labelIndex-1].top_y = j;
                        }
                    }

                    //right
                    if(MatBordPoints[labelIndex-1].right_x == 0 && MatBordPoints[labelIndex-1].right_y ==0){
                        MatBordPoints[labelIndex-1].right_x = i;
                        MatBordPoints[labelIndex-1].right_y = j;
                    }else{
                        if(MatBordPoints[labelIndex-1].right_x < i){
                            MatBordPoints[labelIndex-1].right_x = i;
                            MatBordPoints[labelIndex-1].right_y = j;
                        }
                    }

                    //bottom
                    if(MatBordPoints[labelIndex-1].bottom_x == 0 && MatBordPoints[labelIndex-1].bottom_y ==0){
                        MatBordPoints[labelIndex-1].bottom_x = i;
                        MatBordPoints[labelIndex-1].bottom_y = j;
                    }else{
                        if(MatBordPoints[labelIndex-1].bottom_y < j){
                            MatBordPoints[labelIndex-1].bottom_x = i;
                            MatBordPoints[labelIndex-1].bottom_y = j;
                        }
                    }
                }
            }
        }
    }

    static int matLength = 0;
    static int matCircleArea = 0;
    /* 判断坏物料索引 */
    for(j = 0; j < m_matCount; j++)
    {
        if(matSingleTotalPixels[j] <= 0)
        {
            continue;
        }

        if(matSingleGoodOrBad[j] == -1)
            continue;//已经被标记为坏物料的不进行重复计算
        if(index <= ARITH_CROSS){//gray arith
            float propotionSenses = (matSingleBadPixels[j]*100)/(matSingleTotalPixels[j]*1.0);

            if(propotionSenses > colorArith.fPropotionSensLow && propotionSenses < colorArith.fPropotionSensUp)
            {
                matSingleGoodOrBad[j] = -1;	//-1表示坏物料
                m_badMatCount++;
            }
        }else if(index <= ARITH_SVM_D){
            if((matSingleBadPixels[j]*100)/(matSingleTotalPixels[j]*1.0) >  svmArith.fPropotionSensLow)
            {
                matSingleGoodOrBad[j] = -1;	//-1表示坏物料
                m_badMatCount++;
            }
        }
        else
        {
            if(index == ARITH_BIG_SMALL){//shape arith big_small
                stu_big_small bigsmall = struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struBigSmall[0];
                unitSens = struCnfp.struUnitSens[m_imageLayer][m_imageView][m_imageUnit][0].bigSmall[0];
                if(bigsmall.nMode){
                    if(matSingleTotalPixels[j] > struCnfg.nBigSmallArithMaxValue-(bigsmall.nAreaUp+unitSens)){
                        matSingleGoodOrBad[j] = -1;
                        m_badMatCount++;
                    }
                }else{
                    if(matSingleTotalPixels[j] < (bigsmall.nAreaLow+unitSens)){
                        matSingleGoodOrBad[j] = -1;
                        m_badMatCount++;
                    }
                }
            }else if(index == ARITH_LONG_SHORT){//shape arith long_short
                stu_long_short longShort = struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struLongShort[0];
                unitSens = struCnfp.struUnitSens[m_imageLayer][m_imageView][m_imageUnit][0].longShort[0];
                matLength = myFlow.getShapeLongShort(MatBordPoints[j].left_x,MatBordPoints[j].left_y,MatBordPoints[j].top_x,MatBordPoints[j].top_y,\
                                                         MatBordPoints[j].right_x,MatBordPoints[j].right_y,MatBordPoints[j].bottom_x,MatBordPoints[j].bottom_y);
                if(longShort.nLengthMode){//long
                    if(matLength > ARITH_LENGTH_MAX_VALUE-(longShort.nLengthUp+unitSens)){
                        matSingleGoodOrBad[j] = -1;
                        m_badMatCount++;
                    }
                }else{//short
                    if(matLength < (longShort.nLengthLow+unitSens)){
                        matSingleGoodOrBad[j] = -1;
                        m_badMatCount++;
                    }
                }
            }else if(index == ARITH_CIRCLE_LONG){//shape arith circle_long
                stu_circle_long circlelong = struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struCircleLong[0];
                unitSens = struCnfp.struUnitSens[m_imageLayer][m_imageView][m_imageUnit][0].circleLong[0];
                matCircleArea = myFlow.getShapeCircleLong(MatBordPoints[j].left_x,MatBordPoints[j].left_y,MatBordPoints[j].top_x,MatBordPoints[j].top_y,\
                                                         MatBordPoints[j].right_x,MatBordPoints[j].right_y,MatBordPoints[j].bottom_x,MatBordPoints[j].bottom_y);
                if(circlelong.nMode == 0){//long
                    if((float)matSingleTotalPixels[j]/(float)matCircleArea < ((float)circlelong.nCiloLow+(float)unitSens)/ARITH_LONG_CIRCLE_MAX_VALUE){
                        matSingleGoodOrBad[j] = -1;
                        m_badMatCount++;
                    }
                }else{//circle
                    if((float)matSingleTotalPixels[j]/(float)matCircleArea  > 1.0-((float)circlelong.nCilosUp+(float)unitSens)/ARITH_LONG_CIRCLE_MAX_VALUE){
                        matSingleGoodOrBad[j] = -1;
                        m_badMatCount++;
                    }
                }
            }
        }
    }

    for(j=0;j<m_imageHeight;j++)
    {
        for(i=0;i<m_imageWidth;i++)
        {
            labelIndex = Percent_Label_Buffer[j*m_imageWidth+i]-1;

            if((Percent_Label_Buffer[j*m_imageWidth+i] >= 1) && (matSingleTotalPixels[labelIndex] > 15))//像素点对应物料的索引大于1 物料总的像素点大于15
            {
                if(matSingleGoodOrBad[labelIndex] == -1)    //坏物料用红色标识
                {
                    chgBuf[4*j*m_imageWidth+4*i+2]=255;
                    chgBuf[4*j*m_imageWidth+4*i+1]=0;
                    chgBuf[4*j*m_imageWidth+4*i]=0;
                }
            }
        }
    }
}

void ImageWidget::displaySimulate()
{
    if(m_bSimulate)
    {
#ifdef Q_OS_UNIX
        g_infoWidget().setLabelText(g_myLan().ai_simulating);
        g_infoWidget().show();
        myFlow.msleep(20);
#endif
    }
    uchar *pPixelIsBad = (uchar *)malloc(m_imageHeight*m_imageWidth);
    bool *pPixelIsBad2 = (bool *)malloc(m_imageHeight*m_imageWidth);
    memset(pPixelIsBad, 0, m_imageWidth*m_imageHeight);
    memset(pPixelIsBad2, 0, m_imageWidth*m_imageHeight);
    m_imageArithGroup = myFlow.getIdentifyGroupIndex(m_imageLayer,m_imageView,m_imageUnit);
    memset(Percent_Label_Buffer,0,sizeof(Percent_Label_Buffer));
    memset(matSingleGoodOrBad,0,sizeof(matSingleGoodOrBad));
    m_badMatCount = 0;
    m_matCount = ConnectedComponentLabeling(bgPoint, m_imageWidth, m_imageHeight, Percent_Label_Buffer);

    if(!m_bSimulate & struGsh.m_bManulStudy[1]) //非仿真界面，手动智能取样专用
    {
        free(pPixelIsBad);
        free(pPixelIsBad2);
        return;
    }
    if(m_widgetType == analyzeType){//svm 分析页面,只仿真svm
        colorSimulate(pPixelIsBad,ARITH_SVM_A);
        if(m_bIsPurSim){//只显示所有坏点
            for(int i=0; i<m_imageHeight; i++) {
                for(int j=0; j<m_imageWidth; j++) {
                    if(pPixelIsBad[i*m_imageWidth+j] == PIXEL_MATTER_BAD) {
                        chgBuf[(i*m_imageWidth+j)*4] = 0;
                        chgBuf[(i*m_imageWidth+j)*4+1] = 0;
                        chgBuf[(i*m_imageWidth+j)*4+2] = 255;
                    }//end if
                }//end j
            }//end i
        }else{
            changeImgBySim(pPixelIsBad,ARITH_SVM_A);//使用尺寸算法
        }
    }else{//仿真页面仿真所有使能算法
        for(int arithIndex = 0;arithIndex<= TEA_ARITH_COLOR_5;arithIndex++){
            if(struCnfp.nGroupArithEnable[m_imageLayer][m_imageView][m_imageArithGroup][arithIndex]){
                if(arithIndex <= ARITH_SVM_D || (arithIndex >= ARITH_HSV && arithIndex <= ARITH_HSV_C ) || arithIndex == ARITH_SAT){
                    colorSimulate(pPixelIsBad,arithIndex);
                    if(m_bIsPurSim){//只显示所有坏点
                        for(int i=0; i<m_imageHeight; i++) {
                            for(int j=0; j<m_imageWidth; j++) {
                                if(pPixelIsBad[i*m_imageWidth+j] == PIXEL_MATTER_BAD) {
                                    chgBuf[(i*m_imageWidth+j)*4] = 0;
                                    chgBuf[(i*m_imageWidth+j)*4+1] = 0;
                                    chgBuf[(i*m_imageWidth+j)*4+2] = 255;
                                }//end if
                            }//end j
                        }//end i
                    }else{
                        if(arithIndex <= ARITH_CROSS){
                            if(struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struGreyColor[arithIndex].nPropotionEnable ==0 \
                                    && struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struGreyColor[arithIndex].nLineDefectEnable ==0){
                                changeImgBySim(pPixelIsBad,arithIndex);//使用尺寸算法
                            }else{
                                judgeGreyDiscolorPropotion(arithIndex);//使用比例算法
                            }
                        }else if(arithIndex >= ARITH_SVM_A && arithIndex <= ARITH_SVM_D){
                            if(struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[arithIndex-ARITH_SVM_A].nPropotionEnable ==0 \
                                    && struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struSvm[arithIndex-ARITH_SVM_A].nLineDefectEnable ==0){

                                changeImgBySim(pPixelIsBad,arithIndex);//使用尺寸算法
                            }else{
                                judgeGreyDiscolorPropotion(arithIndex);//使用比例算法
                            }
                        }else if((arithIndex >= ARITH_HSV && arithIndex <= ARITH_HSV_C)|| arithIndex ==  ARITH_SAT){
                            changeImgBySim(pPixelIsBad,arithIndex);//使用尺寸算法
                        }
                    }
                }

                if(arithIndex == ARITH_BIG_SMALL || arithIndex == ARITH_LONG_SHORT || arithIndex == ARITH_CIRCLE_LONG){
                    judgeGreyDiscolorPropotion(arithIndex);
                }

                if(arithIndex == ARITH_WHEAT_SPROUT)
                {
                    stru_wheat_sprout wheatsprout = struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struWheatSprout;
#ifndef Q_OS_WIN
                    Wheat wheat;
                    float h0 = wheatsprout.nH0/10.0;
                    float h1 = wheatsprout.nH1/10.0;
//                    wheat.Wheat_Main(srcBuf,(bool*)bgPoint,pPixelIsBad2,m_imageWidth,m_imageHeight,wheatsprout.nLow,wheatsprout.nSense,wheatsprout.nPercent,h0,h1);
#endif
                    for(int i=0; i<m_imageHeight; i++) {
                        for(int j=0; j<m_imageWidth; j++) {
                            if(pPixelIsBad2[i*m_imageWidth+j] == 1) {
                                chgBuf[(i*m_imageWidth+j)*4] = 0;
                                chgBuf[(i*m_imageWidth+j)*4+1] = 0;
                                chgBuf[(i*m_imageWidth+j)*4+2] = 255;
                            }//end if
                        }
                    }
                }

                if(arithIndex >= TEA_ARITH_COLOR && arithIndex <= TEA_ARITH_COLOR_4){
                    int index = arithIndex-TEA_ARITH_COLOR;
#if 0
                    uchar *pPixelIsBad_tea = (uchar *)malloc(m_imageHeight*m_imageWidth);
                    if(!pPixelIsBad_tea){
                        qDebug()<<"pPixelIsBad_tea malloc err";
                        return;
                    }
                    memset(pPixelIsBad_tea, 0, m_imageWidth*m_imageHeight);
#ifdef Q_OS_UNIX
                    stu_tea_color color = struCnfp.struGroupIdentify[m_imageLayer][m_imageView][m_imageArithGroup].struTeaColor[index];

                    Tea converter;

                    u_int16_t tc = converter.Float_to_fp16(color.tc);
                    u_int16_t ec = converter.Float_to_fp16(color.ec);
                    u_int16_t ac = converter.Float_to_fp16(color.ac);
                    u_int16_t sense = converter.Float_to_fp16(color.sense);
                    quint8 data[16];
                    memset(data,0,16);

                    data[1] = tc/256;
                    data[2] = tc%256;
                    data[3] = ec/256;
                    data[4] = ec%256;
                    data[5] = ac/256;
                    data[6] = ac%256;

                    data[7] = sense/256;
                    data[8] = sense%256;
                    data[9] = color.nRow-1;
                    data[10] = (color.nPercent-1)/256;
                    data[11] = (color.nPercent-1)%256;
                    data[13] = color.nEdge;
                    data[14] = color.nInhibit;
                    data[12] = color.nMode;
#endif
                    uchar bgPoint_ivt[IMAGE_WIDTH*IMAGE_HEIGHT];
                    memset(bgPoint_ivt,0,sizeof(bgPoint_ivt));

                    for(int i=0; i<m_imageHeight; i++) {
                        for(int j=0; j<m_imageWidth; j++) {
                            if(bgPoint[i*m_imageWidth+j] == 0) {
                                bgPoint_ivt[i*m_imageWidth+j] = 1;
                            }
                        }
                    }
#ifdef Q_OS_UNIX
                    //TEA_Signed(const unsigned char *pInData, const bool *pBkgData, unsigned char *pOutFlag, int nHeight, int nWidth, const unsigned char *Data_16Bytes);
                    converter.TEA_Signed(srcBuf,(bool*)bgPoint_ivt,pPixelIsBad_tea,m_imageHeight,m_imageWidth,data);
                    for(int i=0; i<m_imageHeight; i++) {
                        for(int j=0; j<m_imageWidth; j++) {
                            if(pPixelIsBad_tea[i*m_imageWidth+j] == 1) {
                                chgBuf[(i*m_imageWidth+j)*4] = 0;
                                chgBuf[(i*m_imageWidth+j)*4+1] = 0;
                                chgBuf[(i*m_imageWidth+j)*4+2] = 255;
                            }//end if
                        }
                    }
#endif

                    free(pPixelIsBad_tea);

#endif
                }
            }
        }
    }
    free(pPixelIsBad);
    free(pPixelIsBad2);
#ifdef Q_OS_UNIX
    g_infoWidget().delayHide();
#endif
}

void ImageWidget::colorSimulate(uchar *pStatus,int arithIndex)
{
    int cnstNum = 1000000;
    int nr = struGsh.m_svmcoefs[0]*cnstNum;
    int ng = struGsh.m_svmcoefs[1]*cnstNum;
    int nb = struGsh.m_svmcoefs[2]*cnstNum;
    int nrg = struGsh.m_svmcoefs[3]*cnstNum;
    int ngb = struGsh.m_svmcoefs[4]*cnstNum;
    int nrb = struGsh.m_svmcoefs[5]*cnstNum;
    int nrr= struGsh.m_svmcoefs[6]*cnstNum;
    int ngg = struGsh.m_svmcoefs[7]*cnstNum;
    int nbb = struGsh.m_svmcoefs[8]*cnstNum;
    int ncon = struGsh.m_svmcoefs[9]*cnstNum;;
    int nSvmCons = 0;

    m_matPixCount = 0;
    m_badPixCount = 0;
    int group = myFlow.getIdentifyGroupIndex(m_imageLayer,m_imageView,m_imageUnit);
    bool bIsBad;
    for(int i=0; i<m_imageHeight; i++){
        for(int j=0; j<m_imageWidth; j++){
            bIsBad = false;
            uchar b = srcBuf[(i*m_imageWidth+j)*4];
            uchar g = srcBuf[(i*m_imageWidth+j)*4+1];
            uchar r = srcBuf[(i*m_imageWidth+j)*4+2];
            uchar n = srcBuf[(i*m_imageWidth+j)*4+3];
            if(bgPoint[i*m_imageWidth+j] == PIXEL_BACKGROUND)
                continue;
            /* 设置物料点的属性 */
            pStatus[i*m_imageWidth+j] = PIXEL_MATTER;
            m_matPixCount++;
            if(m_widgetType == analyzeType && struGsh.ntrainComplete ){//学习成功后可看临时仿真效果
                /* 灵敏度越大，常数项越小，选的越多 */
                if(ncon < 0) {
                    nSvmCons = ncon*(1+(struGsh.nSvmSimulateSens-100)/100.0);
                }else{
                    nSvmCons = ncon*(1+(100-struGsh.nSvmSimulateSens)/100.0);
                }
                //智能模式３，需要对常数项放大
                if (struCnfg.n_svmType == 2){
                    nSvmCons = struGsh.nSvmExpandValue*ncon*((struGsh.nSvmSimulateSens-100)/100.0);
                }

                if((nr*r+ng*g+nb*b+nrg*r*g+ngb*g*b+nrb*r*b+nrr*r*r+ngg*g*g+nbb*b*b+nSvmCons) > 0
                        || r <struGsh.nSvmBrightLimit || g <struGsh.nSvmBrightLimit || b <struGsh.nSvmBrightLimit){
                    bIsBad = false;
                }else{
                    bIsBad = true;
                }

            }else{
                bIsBad = myFlow.imagePixelIsBad(r,g,b,n,m_imageLayer, m_imageView, group,m_imageUnit,arithIndex);
            }

            if(bIsBad){
                m_badPixCount++;
                pStatus[i*m_imageWidth+j] = PIXEL_MATTER_BAD;
            }
        }
    }
//    updatePixelCountLbl();
}

void ImageWidget::changeImgBySim(uchar *pStatus,int arithIndex)
{
    int group = myFlow.getIdentifyGroupIndex(m_imageLayer,m_imageView,m_imageUnit);
    bool bNeedChg;
    int nBadPixelNum;
    int row =0,col=0,per=0,edge=0;
    if(m_widgetType == analyzeType){
        row = struGsh.nSvmRow;
        per = struGsh.nSvmPercent;
        edge = struGsh.nSvmCut;
    }else{
        if(arithIndex <= ARITH_CROSS){//grey
            row= struCnfp.struGroupIdentify[m_imageLayer][m_imageView][group].struGreyColor[arithIndex].nRow;
            per = struCnfp.struGroupIdentify[m_imageLayer][m_imageView][group].struGreyColor[arithIndex].nPercent;
            edge = struCnfp.struGroupIdentify[m_imageLayer][m_imageView][group].struGreyColor[arithIndex].nEdge;
        }else if(arithIndex <= ARITH_SVM_D && arithIndex >= ARITH_SVM_A){//svm
            row= struCnfp.struGroupIdentify[m_imageLayer][m_imageView][group].struSvm[arithIndex-ARITH_SVM_A].nRow;
            per = struCnfp.struGroupIdentify[m_imageLayer][m_imageView][group].struSvm[arithIndex-ARITH_SVM_A].nPercent;
            edge = struCnfp.struGroupIdentify[m_imageLayer][m_imageView][group].struSvm[arithIndex-ARITH_SVM_A].nEdge;
        }else if(arithIndex <= ARITH_HSV_C && arithIndex >= ARITH_HSV){
            row= struCnfp.struGroupIdentify[m_imageLayer][m_imageView][group].struHsv[arithIndex-ARITH_HSV].nRow;
            per = struCnfp.struGroupIdentify[m_imageLayer][m_imageView][group].struHsv[arithIndex-ARITH_HSV].nPercent;
            edge = 0;
        }else if(arithIndex == ARITH_SAT){
            row= struCnfp.struGroupIdentify[m_imageLayer][m_imageView][group].struColorSat.nRow;
            per = struCnfp.struGroupIdentify[m_imageLayer][m_imageView][group].struColorSat.nPercent;
            edge = 0;
        }
    }
    col = row;//列暂时没有使用，均使用row
    /* 根据设置的行列纯度进行仿真 */
    for(int i=0; i<m_imageHeight; i++) {
        for(int j=0; j<m_imageWidth; j++) {
            if(pStatus[i*m_imageWidth+j] != PIXEL_BACKGROUND) {
                if(pixelIsEdge(j,i,edge)) continue;
                bNeedChg = true;
                nBadPixelNum = 0;
                int nRowBegin = i-(row-1)/2-(row-1)%2;
                int nRowEnd = i+(row-1)/2;
                int nColBegin = j-(col-1)/2-(col-1)%2;
                int nColEnd = j+(col-1)/2;

                if(nColBegin < 0)
                    nColBegin = 0;
                if(nColEnd > m_imageWidth-1)
                    nColEnd = m_imageWidth-1;
                if(nRowBegin < 0)
                    nRowBegin = 0;
                if(nRowEnd > m_imageHeight-1)
                    nRowEnd = m_imageHeight-1;

                for(int m=nRowBegin; m<=nRowEnd; m++) {
                    for(int n=nColBegin; n<=nColEnd; n++) {
                        if(pStatus[m*m_imageWidth+n] == PIXEL_MATTER_BAD)
                            nBadPixelNum++;
                    }//end n
                }//end m

                if(nBadPixelNum < per)
                    bNeedChg = false;

                if(bNeedChg) {
                    chgBuf[(i*m_imageWidth+j)*4] = 0;
                    chgBuf[(i*m_imageWidth+j)*4+1] = 0;
                    chgBuf[(i*m_imageWidth+j)*4+2] = 255;
                }
            }
        }//end j
    }//end i
    //end if
}

void ImageWidget::showGrayColor()
{
    for(int i=0; i<m_imageHeight; i++) {
        for(int j=0; j<m_imageWidth; j++) {
            if(m_widgetType == simulateType){
                if((chgBuf[(i*m_imageWidth+j)*4] == 0)&&(chgBuf[(i*m_imageWidth+j)*4+1] == 0)&&(chgBuf[(i*m_imageWidth+j)*4+2] == 255))
                {
                    chgBuf[(i*m_imageWidth+j)*4+3] = 255;
                    continue;
                }
            }
            switch(m_colorType){
            case rgb:
                break;
            case r:
                chgBuf[(i*m_imageWidth+j)*4] = oriBuf[(i*m_imageWidth+j)*4+2];
                chgBuf[(i*m_imageWidth+j)*4+1] = oriBuf[(i*m_imageWidth+j)*4+2];
                break;
            case g:
                chgBuf[(i*m_imageWidth+j)*4] = oriBuf[(i*m_imageWidth+j)*4+1];
                chgBuf[(i*m_imageWidth+j)*4+2] = oriBuf[(i*m_imageWidth+j)*4+1];
                break;
            case b:
                chgBuf[(i*m_imageWidth+j)*4+1] = oriBuf[(i*m_imageWidth+j)*4];
                chgBuf[(i*m_imageWidth+j)*4+2] = oriBuf[(i*m_imageWidth+j)*4];
                break;
            case n:
                chgBuf[(i*m_imageWidth+j)*4] = oriBuf[(i*m_imageWidth+j)*4+3];
                chgBuf[(i*m_imageWidth+j)*4+1] = oriBuf[(i*m_imageWidth+j)*4+3];
                chgBuf[(i*m_imageWidth+j)*4+2] = oriBuf[(i*m_imageWidth+j)*4+3];
                break;
            case nrg:
                chgBuf[(i*m_imageWidth+j)*4] = oriBuf[(i*m_imageWidth+j)*4+1];
                chgBuf[(i*m_imageWidth+j)*4+1] = oriBuf[(i*m_imageWidth+j)*4+2];
                chgBuf[(i*m_imageWidth+j)*4+2] = oriBuf[(i*m_imageWidth+j)*4+3];
                break;
            case nrb:
                chgBuf[(i*m_imageWidth+j)*4] = oriBuf[(i*m_imageWidth+j)*4];
                chgBuf[(i*m_imageWidth+j)*4+1] = oriBuf[(i*m_imageWidth+j)*4+2];
                chgBuf[(i*m_imageWidth+j)*4+2] = oriBuf[(i*m_imageWidth+j)*4+3];
                break;
            case ngb:
                chgBuf[(i*m_imageWidth+j)*4] = oriBuf[(i*m_imageWidth+j)*4];
                chgBuf[(i*m_imageWidth+j)*4+1] = oriBuf[(i*m_imageWidth+j)*4+1];
                chgBuf[(i*m_imageWidth+j)*4+2] = oriBuf[(i*m_imageWidth+j)*4+3];
                break;
            }
            if(m_colorType != origin)
                chgBuf[(i*m_imageWidth+j)*4+3] = 255;
        }
    }
    memcpy(disBuf, chgBuf, m_imageWidth*m_imageHeight*4);
}

void ImageWidget::updateSimpleBtnState()
{
    if(m_simpleType == Good){
        ui->goodTypeBtn->setChecked(true);
    }else if(m_simpleType == Bad){
        ui->badTypeBtn->setChecked(true);
    }
}

void ImageWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_bMoveImage)
        return;

    if(!ui->imageLabel->geometry().contains(event->pos())){
        m_tempimage = m_image;
        updateImageSize();
        return;
    }
    m_image = m_tempimage;
    posTmp = event->pos()-ui->imageLabel->pos();
    if(m_tempSelectNum > 0)
    {
        m_PointNumVec[m_simpleType].append(m_tempSelectNum);
        m_NoSlectPointNumVec[m_simpleType].append(m_tempNoSelectNum);
    }
    m_PointVec[m_simpleType]+=m_tempSelectPointVec;
    m_NoSlectPointVec[m_simpleType]+=m_tempNoSelectPointVec;
}

void ImageWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(m_widgetType == analyzeType )
        m_tempimage = m_image;
    if(!ui->imageLabel->geometry().contains(event->pos()))
        return;

    posTmp = event->pos()-ui->imageLabel->pos();

    if(m_bMoveImage){
        float posXoffset = posTmp.x()-posStart.x();
        float posYoffset = posTmp.y()-posStart.y();
        posStart = posTmp;
        m_CenterX = m_CenterX-posXoffset/m_ZoomValue;
        m_CenterY = m_CenterY-posYoffset/m_ZoomValue;

        adapteImageCenterPos();
        if(m_widgetType == captureType )
            showGrayColor();
        else{
            if(m_widgetType == analyzeType && (struGsh.m_bAutoStudy[0] | struGsh.m_bAutoStudy[1])){
                showGrayColor();
                QImage img = QImage(chgBuf,m_imageWidth,m_imageHeight,QImage::Format_ARGB32);
                m_tempimage = img;
            }else{
            }
        }
        updateImageSize();
        return;
    }
    if(m_imageWidth*m_ZoomValue < ImageLabelWidth && m_imageHeight *m_ZoomValue < ImageLabelHeight){
        m_EndOriPoint.setX((posTmp.x()-m_imageWidth*m_ZoomValue/2)/m_ZoomValue+m_CenterX);
        m_EndOriPoint.setY((posTmp.y()-m_imageHeight*m_ZoomValue/2)/m_ZoomValue+m_CenterY);
    }else{
        if(m_imageWidth*m_ZoomValue >= ImageLabelWidth)
            m_EndOriPoint.setX((posTmp.x()-ImageLabelWidth/2)/m_ZoomValue+m_CenterX);
        else
            m_EndOriPoint.setX((posTmp.x()-m_imageWidth*m_ZoomValue/2)/m_ZoomValue+m_CenterX);
        if(m_imageHeight *m_ZoomValue >= ImageLabelHeight)
            m_EndOriPoint.setY((posTmp.y()-ImageLabelHeight/2)/m_ZoomValue+m_CenterY);
        else
            m_EndOriPoint.setY((posTmp.y()-m_imageHeight*m_ZoomValue/2)/m_ZoomValue+m_CenterY);
    }

//    if(m_imageWidth*m_ZoomValue < ImageLabelWidth){
//        m_EndOriPoint.setX((posTmp.x()-m_imageWidth*m_ZoomValue/2)/m_ZoomValue+m_CenterX);
//        m_EndOriPoint.setY((posTmp.y()-ImageLabelHeight/2)/m_ZoomValue+m_CenterY);
//    }else{
//        m_EndOriPoint.setX((posTmp.x()-ImageLabelWidth/2)/m_ZoomValue+m_CenterX);
//        m_EndOriPoint.setY((posTmp.y()-ImageLabelHeight/2)/m_ZoomValue+m_CenterY);
//    }
    m_OriRectWidth = m_EndOriPoint.x()-m_StartOriPoint.x();
    m_OriRectHeight = m_EndOriPoint.y()-m_StartOriPoint.y();



    m_tempSelectPointVec.clear();
    m_tempSelectNum = 0;
    m_tempNoSelectPointVec.clear();
    m_tempNoSelectNum = 0;

    for(int i=0; i<m_OriRectWidth; i++){
        for(int j=0; j<m_OriRectHeight; j++){
            int x = m_StartOriPoint.x()+i;
            int y = m_StartOriPoint.y()+j;
            if(bgPoint[y*m_imageWidth+x] == PIXEL_BACKGROUND)
                continue;
            if(pixelIsSelected(x,y))
                continue;
            if(struGsh.m_bEdge)
                if(pixelIsEdge(x,y))
                    continue;
            if(pixelIsOutThreshold(x,y))
                continue;
            if(m_simpleType == Good){
                m_tempimage.setPixel(x,y,qRgb(0,255,0));
            }else if(m_simpleType == Bad){
                m_tempimage.setPixel(x,y,qRgb(255,0,0));
            }else{
                m_tempimage.setPixel(x,y,qRgb(255,255,0));
            }
            if ( i % (struGsh.svmSample[0]+1) != 0){
                m_tempNoSelectPointVec.append(QPoint(x,y));
                m_tempNoSelectNum++;
                continue;
            }
            if (j % (struGsh.svmSample[1]+1) != 0){
                m_tempNoSelectPointVec.append(QPoint(x,y));
                m_tempNoSelectNum++;
                continue;
            }
            m_tempSelectPointVec.append(QPoint(x,y));
            m_tempSelectNum++;
        }
    }
    updateImageSize();
}

void ImageWidget::retranslateUiWidget()
{
    ui->checkBox_No_Bg->setText(g_myLan().ids_background);
    ui->radioBtn_origin->setText(g_myLan().origin);
//    ui->radioBtn_rgb->setText(g_myLan().colorful);
    ui->radioBtn_r->setText(g_myLan().ids_red);
    ui->radioBtn_g->setText(g_myLan().ids_green);
    ui->radioBtn_b->setText(g_myLan().ids_blue);
    ui->radioBtn_n->setText(g_myLan().ids_n);
    ui->radioBtn_rgb->setText(struCnfp.sD2ImgModeName[0]);
    ui->radioBtn_rgn->setText(struCnfp.sD2ImgModeName[1]);
    ui->radioBtn_rbn->setText(struCnfp.sD2ImgModeName[2]);
    ui->radioBtn_gbn->setText(struCnfp.sD2ImgModeName[3]);
    ui->checkBox->setText(g_myLan().movefix);
    ui->saveSampleBtn->setText(g_myLan().ai_add_sample);
    ui->goodTypeBtn->setText(g_myLan().ai_good);
    ui->badTypeBtn->setText(g_myLan().ai_bad);
    ui->revokeBtn->setText(g_myLan().ai_reselect_last);
    ui->checkBox_2->setText(g_myLan().show_bad_all);
    ui->simulateCheckBox->setText(g_myLan().ai_simulate);
    ui->cleanSampleBtn->setText(g_myLan().clean_sample);
    ui->svmCutLabel->setText(g_myLan().edge_cut);
    ui->svmRowLabel->setText(g_myLan().ids_size);
    ui->svmPerLabel->setText(g_myLan().ids_purity);
    ui->svmSensLabel->setText(g_myLan().ids_select);
    ui->svmExpandLabel->setText(g_myLan().lamp_coef);
    ui->svmBrightLimitLabel->setText(g_myLan().light_limit);
    ui->imgDataBtn->setText(g_myLan().sample_analysis+"1");
    ui->imgDataBtn_2->setText(g_myLan().sample_analysis+"2");
    ui->imgDataBtn_3->setText(g_myLan().sample_analysis+"3");
    ui->pushButton->setText(g_myLan().sample_analysis);
    ui->lastPicBtn->setText(g_myLan().last_page);
    ui->nextPicBtn->setText(g_myLan().next_page);

    ui->m_saturationWidget->setTitle("S");
    ui->sampleClassBtn->setText(g_myLan().ids_sampleDetails);
}

void ImageWidget::updateImageSize()
{
    int m_minW = qMin(m_imageWidth*m_ZoomValue,ImageLabelWidth);
    int m_minH = qMin(m_imageHeight*m_ZoomValue,ImageLabelHeight);

    if(m_minW < ImageLabelWidth && m_minH < ImageLabelHeight){
       QImage GrabImage = m_tempimage.scaled(m_minW,m_minH,Qt::KeepAspectRatio);
        ui->imageLabel->setPixmap(QPixmap::fromImage(GrabImage));
    }else{
        int x  = m_CenterX-m_minW/m_ZoomValue/2;
        int y = m_CenterY-m_minH/m_ZoomValue/2;
        int w = m_minW/m_ZoomValue;
        int h = m_minH/m_ZoomValue;
        QImage GrabImage = m_tempimage.copy(x,y,w,h);
       // QImage GrabImage = m_tempimage.copy(m_CenterX-ImageLabelWidth/m_ZoomValue/2,m_CenterY-myFlow.imgHeight/m_ZoomValue/2,ImageLabelWidth/m_ZoomValue,myFlow.imgHeight/m_ZoomValue);
        if(m_ZoomValue != 1)
            GrabImage = GrabImage.scaled(ImageLabelWidth,ImageLabelHeight,Qt::KeepAspectRatio);
        ui->imageLabel->setPixmap(QPixmap::fromImage(GrabImage));
    }
    ui->imageLabel->setFixedWidth(ImageLabelWidth);
    ui->imageLabel->setFixedHeight(ImageLabelHeight);
    ui->imageLabel->setAlignment(Qt::AlignCenter);

#if 0

    if(m_imageWidth*m_ZoomValue < ImageLabelWidth){
        QImage GrabImage = m_tempimage.copy(m_CenterX-m_imageWidth/m_ZoomValue/2,m_CenterY-ImageLabelHeight/m_ZoomValue/2,m_imageWidth*m_ZoomValue,ImageLabelHeight/m_ZoomValue);
        ui->imageLabel->setFixedWidth(m_imageWidth*m_ZoomValue);
        ui->imageLabel->setPixmap(QPixmap::fromImage(GrabImage));
    }else{
        int x  = m_CenterX-ImageLabelWidth/m_ZoomValue/2;
        int y = m_CenterY-ImageLabelHeight/m_ZoomValue/2;
        int w = ImageLabelWidth/m_ZoomValue;
        int h = ImageLabelHeight/m_ZoomValue;
        QImage GrabImage = m_tempimage.copy(x,y,w,h);
       // QImage GrabImage = m_tempimage.copy(m_CenterX-ImageLabelWidth/m_ZoomValue/2,m_CenterY-myFlow.imgHeight/m_ZoomValue/2,ImageLabelWidth/m_ZoomValue,myFlow.imgHeight/m_ZoomValue);
        if(m_ZoomValue != 1)
            GrabImage = GrabImage.scaled(ImageLabelWidth,ImageLabelHeight,Qt::KeepAspectRatio);
        ui->imageLabel->setFixedWidth(ImageLabelWidth);
        ui->imageLabel->setPixmap(QPixmap::fromImage(GrabImage));
    }
#endif
}
void ImageWidget::updateShowPage()
{
    qDebug()<<"updateShowPage";
    memcpy(chgBuf, oriBuf, m_imageWidth*m_imageHeight*4);
    m_simpleType = Good;
    m_tempimage = m_image;
    m_ZoomValue = 1;
    if(m_imageWidth*m_ZoomValue < ImageLabelWidth)
        m_CenterX = m_imageWidth/2;
    else
        m_CenterX = ImageLabelWidth/2;
    if(m_imageHeight*m_ZoomValue < ImageLabelHeight)
        m_CenterY = m_imageHeight/2;
    else
        m_CenterY = ImageLabelHeight/2;

//    if(m_CenterY >= m_imageHeight){
//        m_CenterY = ImageLabelHeight/2;
//    }
//    if(m_CenterX < m_imageWidth/2 || m_CenterX >= m_imageWidth)
//        m_CenterX = m_imageWidth/2;

    updateSvmSimulateEditValue();

    if(m_widgetType == captureType){
        m_bMoveImage = true;
        showGrayColor();
        QImage img = QImage(disBuf,m_imageWidth,m_imageHeight,QImage::Format_ARGB32);
        m_tempimage = img;
    }else if(m_widgetType == simulateType){
        if(m_bSimulate)
            displaySimulate();
        showGrayColor();
        QImage img = QImage(disBuf,m_imageWidth,m_imageHeight,QImage::Format_ARGB32);
        m_tempimage = img;
    }else{
        QImage img = QImage(disBuf,m_imageWidth,m_imageHeight,QImage::Format_ARGB32);
        m_tempimage = img;
        m_image = img;

        if(m_bSimulate && struGsh.ntrainComplete)
            displaySimulate();
        updatePixColor();
        showGrayColor();
    }
    updateImageSize();
    updateZoomValueText();
    updateSimpleBtnState();
}

void ImageWidget::on_enlarge_clicked()
{
    if(m_ZoomValue < 10)
        m_ZoomValue++;
    adapteImageCenterPos();
    updateImageSize();
    updateZoomValueText();
}

void ImageWidget::on_reduce_clicked()
{
    if(m_ZoomValue>1)
        m_ZoomValue--;
    adapteImageCenterPos();
    updateImageSize();
    updateZoomValueText();
}

void ImageWidget::on_checkBox_clicked()
{
    m_bMoveImage = ui->checkBox->isChecked();
}

void ImageWidget::on_revokeBtn_clicked()
{
    if(!m_PointNumVec[m_simpleType].isEmpty()){
        int lastSelectPointNum = m_PointNumVec[m_simpleType][m_PointNumVec[m_simpleType].size()-1];
        int lastNoSelectPointNum = m_NoSlectPointNumVec[m_simpleType][m_NoSlectPointNumVec[m_simpleType].size()-1];
        QPoint point;
        for(int i = m_PointVec[m_simpleType].count()-lastSelectPointNum;i<m_PointVec[m_simpleType].count();i++){
            point = m_PointVec[m_simpleType][i];
            m_tempimage.setPixel(point,m_oriImage.pixel(point));
        }
        for(int i = m_NoSlectPointVec[m_simpleType].count()-lastNoSelectPointNum;i<m_NoSlectPointVec[m_simpleType].count();i++){
            point = m_NoSlectPointVec[m_simpleType][i];
            m_tempimage.setPixel(point,m_oriImage.pixel(point));
        }

        m_PointVec[m_simpleType].resize(m_PointVec[m_simpleType].count()-lastSelectPointNum);
        m_NoSlectPointVec[m_simpleType].resize(m_NoSlectPointVec[m_simpleType].count()-lastNoSelectPointNum);
        m_PointNumVec[m_simpleType].pop_back();
        m_NoSlectPointNumVec[m_simpleType].pop_back();

        updateImageSize();
        m_image = m_tempimage;
    }
}

void ImageWidget::colorTypeRadioBtnClicked(int index)
{
    m_colorType = (colorType)index;
    memcpy(chgBuf, oriBuf, m_imageHeight*m_imageWidth*4);

    if(m_widgetType == simulateType){
        if(m_bSimulate){
            displaySimulate();
        }
    }
    showGrayColor();
    QImage img = QImage(chgBuf,m_imageWidth,m_imageHeight,QImage::Format_ARGB32);
    m_tempimage = img;
    updateImageSize();
}

void ImageWidget::on_goodTypeBtn_clicked(bool checked)
{
    if(checked)
        m_simpleType = Good;
}

void ImageWidget::on_badTypeBtn_clicked(bool checked)
{
    if(checked)
        m_simpleType = Bad;
}

void ImageWidget::updatePixelCountLbl()
{
    int x = m_StartOriPoint.x();
    int y = m_StartOriPoint.y();
    uchar r=srcBuf[4*y*m_imageWidth+4*x+2];
    uchar g=srcBuf[4*y*m_imageWidth+4*x+1];
    uchar b=srcBuf[4*y*m_imageWidth+4*x];
    uchar i=srcBuf[4*y*m_imageWidth+4*x+3];
    QColor pixelColor(r,g,b);
    int h,s,v;
    pixelColor.getHsv(&h,&s,&v);
    QString hsv = QString("H:%1,S:%2,V:%3").arg(h).arg(s).arg(v);
    QString badNumStr = "";
    if(m_widgetType == simulateType){
        if(m_bSimulate){
            if(m_bIsPurSim){
                badNumStr = QString("%1-%2").arg(QString::number(m_badPixCount)).arg(QString::number(m_matPixCount-m_badPixCount));
            }
        }
//        ui->label_showpixelnum->setText(hsv+" "+badNumStr);
    }

}

void ImageWidget::on_checkBox_2_clicked(bool checked)
{
    m_bIsPurSim = checked;
    updateImage();
}

void ImageWidget::on_simulateCheckBox_clicked(bool checked)
{
    m_bSimulate = checked;
    ui->checkBox_No_Bg->setChecked(false);
    updateImage();
}

/* 判断图像中像素点为原始数据点还是被标记的点 */
bool ImageWidget::pixelIsSelected(int x,int y)
{
    QRgb rgb = m_tempimage.pixel(x,y);
    if(rgb == qRgb(255,0,0)||rgb == qRgb(0,255,0)||rgb == qRgb(0,0,255))
        return true;
    return false;
}

bool ImageWidget::pixelIsOutThreshold(int x, int y)
{
    if(r+g ==0 || r+b == 0 || g+b == 0) return true;

   QRgb rgb = m_image.pixel(x,y);
   uchar r = qRed(rgb);
   uchar g = qGreen(rgb);
   uchar b = qBlue(rgb);
   float rg = (float)r/(r+g)*100.0;
   float rb = (float)r/(r+b)*100.0;
   float gb = (float)g/(g+b)*100.0;

   if(m_simpleType == Good)
   {
       if((r < struGsh.svmThreshold[0][0]) || (r > struGsh.svmThreshold[0][1]) || (g < struGsh.svmThreshold[1][0]) \
               || (g > struGsh.svmThreshold[1][1]) || (b < struGsh.svmThreshold[2][0]) || (b > struGsh.svmThreshold[2][1])){
           return true;
       }

       if( (rg < struGsh.svmDisThreshold[0][0]) || (rg > struGsh.svmDisThreshold[0][1]) || (rb < struGsh.svmDisThreshold[1][0]) \
               || (rb > struGsh.svmDisThreshold[1][1]) || (gb < struGsh.svmDisThreshold[2][0]) || (gb > struGsh.svmDisThreshold[2][1])){
           return true;
       }
   }
   else if(m_simpleType == Bad)
   {
       if((r < struGsh.svmThreshold[0][2]) || (r > struGsh.svmThreshold[0][3]) || (g < struGsh.svmThreshold[1][2]) \
               || (g > struGsh.svmThreshold[1][3]) || (b < struGsh.svmThreshold[2][2]) || (b > struGsh.svmThreshold[2][3])){
           return true;
       }

       if( (rg < struGsh.svmDisThreshold[0][2]) || (rg > struGsh.svmDisThreshold[0][3]) || (rb < struGsh.svmDisThreshold[1][2]) \
               || (rb > struGsh.svmDisThreshold[1][3]) || (gb < struGsh.svmDisThreshold[2][2]) || (gb > struGsh.svmDisThreshold[2][3])){
           return true;
       }
   }

    return false;
}

/* 优化计算时判断像素点是否为冗余点 */
bool ImageWidget::pixelIsReSelected(uchar r, uchar g, uchar b)
{
    QPoint point;
    int x,y;
    for(int i = 0;i < 2;i++){
        if(m_PointVec[i].count() > 0){
            for(int j = 0;j < m_PointVec[i].count();j++){
                point = m_PointVec[i][j];
                x = point.x();
                y = point.y();
                if(r == srcBuf[(y*m_imageWidth+x)*4+2] && g == srcBuf[(y*m_imageWidth+x)*4+1]&& b == srcBuf[(y*m_imageWidth+x)*4])
                    return true;
            }
        }
    }
    return false;
}

/* 优化取样时判断像素点是否为边缘点 */
bool ImageWidget::pixelIsEdge(int x, int y,int cnt)
{
    if (cnt == 0) return false;

    if(x==0 || y==0 || x==IMAGE_WIDTH-1 || y==myFlow.imgHeight-1)
        return false;
    static int tmpX,tmpY;
    for(int i=-1*cnt; i<=1*cnt; i++){
        for(int j=-1*cnt; j<=1*cnt; j++){
            if(i!=0 || j!=0){
                tmpX = x+i;
                tmpY = y+j;
                if(bgPoint[tmpY*m_imageWidth+tmpX] == PIXEL_BACKGROUND){
                    return true;
                }
            }
        }
    }
    return false;
}

void ImageWidget::on_saveSampleBtn_clicked()
{
    QPoint point;
    int h,s,v;
    int x,y,tr,tg,tb,num = 0;
    float trg,trb,tgb;
    if(m_PointVec[0].count() == 0 && m_PointVec[1].count() ==0){
        g_infoWidget().setLabelText(g_myLan().cfm_ai_mark_sample);
        g_infoWidget().showSecs(2);
        return;
    }
    g_infoWidget().setLabelText(g_myLan().ai_add_sample);
    g_infoWidget().show();
    myFlow.msleep(10);

    memset(struGsh.nGoodSamp,0,sizeof(struGsh.nGoodSamp));
    memset(struGsh.nBadSamp,0,sizeof(struGsh.nBadSamp));
    struGsh.nGoodSampCount = 0;
    struGsh.nBadSampCount = 0;
    struGsh.nShapeSingleGoodSampCount = 0;
    struGsh.nShapeSingleBadSampCount = 0;

    qint64 hsvHTotal = 0,hsvSTotal = 0,hsvVTotal = 0,rTotal = 0,gTotal = 0,bTotal = 0;
    float rgTotal = 0,rbTotal = 0,gbTotal = 0;
    int m_hsvHMin = 359,m_hsvHMax = 0,m_hsvSMin = 255,m_hsvSMax = 0,m_hsvVMin =255,m_hsvVMax = 0;
    int count = m_PointVec[m_simpleType].count();
    for(int i = 0;i < 9;i++)
    {
        g_Runtime().m_imgDataMap[0][i].clear();
        g_Runtime().m_imgDataMap[1][i].clear();
    }

//    updateShapeSample();

    if(struGsh.m_bManulStudy[0]){
        if(m_PointVec[0].count()>0){
            for(int i = 0;i < m_PointVec[0].count();i++){
                if(struGsh.nGoodSampCount >= MAX_SAMPLE_NUM){
                    logger()->warn("good sample is out of memory");
                    break;
                }
                point = m_PointVec[0][i];
                x = point.x();
                y = point.y();

                if(struGsh.m_bEdge)
                    if(pixelIsEdge(x,y))//剔除边界点
                        continue;
                SampleObj obj = {srcBuf[(y*m_imageWidth+x)*4+2],srcBuf[(y*m_imageWidth+x)*4+1],srcBuf[(y*m_imageWidth+x)*4]};
                if(g_Runtime().m_sampleMap[0].contains(obj)){
                    continue;
                }
                g_Runtime().m_sampleMap[0].insert(obj,1);
            }
        }
        if(m_PointVec[1].count()>0){
            for(int i = 0;i < m_PointVec[1].count();i++){
                if(struGsh.nBadSampCount >= MAX_SAMPLE_NUM){
                    logger()->warn("bad sample is out of memory");
                    break;
                }
                point = m_PointVec[1][i];
                x = point.x();
                y = point.y();
                if(struGsh.m_bEdge)
                    if(pixelIsEdge(x,y))//剔除边界点
                        continue;
                SampleObj obj = {srcBuf[(y*m_imageWidth+x)*4+2],srcBuf[(y*m_imageWidth+x)*4+1],srcBuf[(y*m_imageWidth+x)*4]};

                if (struGsh.m_bGoodBadDiff){ //好坏相异
                    if(g_Runtime().m_sampleMap[0].contains(obj))
                        continue;
                }

                if(struGsh.m_bRMRedtmdancy){ //去除冗余
                    int tr = srcBuf[(y*m_imageWidth+x)*4+2];
                    int tg = srcBuf[(y*m_imageWidth+x)*4+1];
                    int tb = srcBuf[(y*m_imageWidth+x)*4+0];

                    for(int k1 = -1; k1 <= 1; k1++)
                    {
                        for(int k2 = -1; k2 <= 1; k2++)
                        {
                            for(int k3 = -1; k3 <= 1; k3++)
                            {
                                SampleObj objTmp1 = {tr+k1, tg+k2, tb+k3};
                                if(g_Runtime().m_sampleMap[1].contains(objTmp1)){
                                    continue;
                                }
                            }
                        }
                    }
                }
                g_Runtime().m_sampleMap[1].insert(obj,0);
            }
        }

        QMap<SampleObj,int>::const_iterator var;
        int i = 0;
        hsvHTotal = 0;
        hsvSTotal = 0;
        hsvVTotal = 0;
        rTotal = 0;
        gTotal = 0;
        bTotal = 0;
        rgTotal = 0;
        rbTotal = 0;
        gbTotal = 0;

        for (var = g_Runtime().m_sampleMap[1].constBegin(); var !=g_Runtime().m_sampleMap[1].constEnd(); ++var,i++) {
            tr = var.key().r;
            tg = var.key().g;
            tb = var.key().b;

            myFlow.getHsv(tr,tg,tb,h,s,v);

            trg = tr*100.0/((float)tr+(float)tg);
            trb = tr*100.0/((float)tr+(float)tb);
            tgb = tg*100.0/((float)tg+(float)tb);

            hsvHTotal += h/10;
            hsvSTotal += s;
            hsvVTotal += v;

            rTotal += tr;
            gTotal += tg;
            bTotal += tb;
            rgTotal += trg;
            gbTotal += tgb;
            rbTotal += trb;

            num = g_Runtime().m_imgDataMap[1][0].value(h);
            g_Runtime().m_imgDataMap[1][0].insert(h,num == 0?1:num+1);

            num = g_Runtime().m_imgDataMap[1][1].value(s);
            g_Runtime().m_imgDataMap[1][1].insert(s,num == 0?1:num+1);

            num = g_Runtime().m_imgDataMap[1][2].value(v);
            g_Runtime().m_imgDataMap[1][2].insert(v,num == 0?1:num+1);

            num = g_Runtime().m_imgDataMap[1][3].value(tr);
            g_Runtime().m_imgDataMap[1][3].insert(tr,num == 0?1:num+1);

            num = g_Runtime().m_imgDataMap[1][4].value(tg);
            g_Runtime().m_imgDataMap[1][4].insert(tg,num == 0?1:num+1);

            num = g_Runtime().m_imgDataMap[1][5].value(tb);
            g_Runtime().m_imgDataMap[1][5].insert(tb,num == 0?1:num+1);

            num = g_Runtime().m_imgDataMap[1][6].value(trg);
            g_Runtime().m_imgDataMap[1][6].insert(trg,num == 0?1:num+1);

            num = g_Runtime().m_imgDataMap[1][7].value(trb);
            g_Runtime().m_imgDataMap[1][7].insert(trb,num == 0?1:num+1);

            num = g_Runtime().m_imgDataMap[1][8].value(tgb);
            g_Runtime().m_imgDataMap[1][8].insert(tgb,num == 0?1:num+1);

            struGsh.nBadSamp[struGsh.nBadSampCount*3] = var.key().r;
            struGsh.nBadSamp[struGsh.nBadSampCount*3+1] = var.key().g;
            struGsh.nBadSamp[struGsh.nBadSampCount*3+2] = var.key().b;

            struGsh.nBadSampCount++;
            if(struGsh.nBadSampCount> 0){
                struGsh.nAvgH[1] = hsvHTotal/struGsh.nBadSampCount;
                struGsh.nAvgS[1] = hsvSTotal/struGsh.nBadSampCount;
                struGsh.nAvgV[1] = hsvVTotal/struGsh.nBadSampCount;
                struGsh.nAvgR[1] = rTotal/struGsh.nBadSampCount;
                struGsh.nAvgG[1] = gTotal/struGsh.nBadSampCount;
                struGsh.nAvgB[1] = bTotal/struGsh.nBadSampCount;
                struGsh.nAvgRG[1] = rgTotal/struGsh.nBadSampCount;
                struGsh.nAvgRB[1] = rbTotal/struGsh.nBadSampCount;
                struGsh.nAvgGB[1] = gbTotal/struGsh.nBadSampCount;
            }
        }
        hsvHTotal = 0;
        hsvSTotal = 0;
        hsvVTotal = 0;
        rTotal = 0;
        gTotal = 0;
        bTotal = 0;
        rgTotal = 0;
        rbTotal = 0;
        gbTotal = 0;
        for (var = g_Runtime().m_sampleMap[0].constBegin(); var !=g_Runtime().m_sampleMap[0].constEnd(); ++var,i++) {
            tr = var.key().r;
            tg = var.key().g;
            tb = var.key().b;

            myFlow.getHsv(tr,tg,tb,h,s,v);

            trg = tr*100.0/((float)tr+(float)tg);
            trb = tr*100.0/((float)tr+(float)tb);
            tgb = tg*100.0/((float)tg+(float)tb);

            hsvHTotal += h/10;
            hsvSTotal += s;
            hsvVTotal += v;

            rTotal += tr;
            gTotal += tg;
            bTotal += tb;
            rgTotal += trg;
            gbTotal += tgb;
            rbTotal += trb;

            num = g_Runtime().m_imgDataMap[0][0].value(h);
            g_Runtime().m_imgDataMap[0][0].insert(h,num == 0?1:num+1);

            num = g_Runtime().m_imgDataMap[0][1].value(s);
            g_Runtime().m_imgDataMap[0][1].insert(s,num == 0?1:num+1);

            num = g_Runtime().m_imgDataMap[0][2].value(v);
            g_Runtime().m_imgDataMap[0][2].insert(v,num == 0?1:num+1);

            num = g_Runtime().m_imgDataMap[0][3].value(tr);
            g_Runtime().m_imgDataMap[0][3].insert(tr,num == 0?1:num+1);

            num = g_Runtime().m_imgDataMap[0][4].value(tg);
            g_Runtime().m_imgDataMap[0][4].insert(tg,num == 0?1:num+1);

            num = g_Runtime().m_imgDataMap[0][5].value(tb);
            g_Runtime().m_imgDataMap[0][5].insert(tb,num == 0?1:num+1);

            num = g_Runtime().m_imgDataMap[0][6].value(trg);
            g_Runtime().m_imgDataMap[0][6].insert(trg,num == 0?1:num+1);

            num = g_Runtime().m_imgDataMap[0][7].value(trb);
            g_Runtime().m_imgDataMap[0][7].insert(trb,num == 0?1:num+1);

            num = g_Runtime().m_imgDataMap[0][8].value(tgb);
            g_Runtime().m_imgDataMap[0][8].insert(tgb,num == 0?1:num+1);

            struGsh.nGoodSamp[struGsh.nGoodSampCount*3] = tr;
            struGsh.nGoodSamp[struGsh.nGoodSampCount*3+1] = tg;
            struGsh.nGoodSamp[struGsh.nGoodSampCount*3+2] = tb;

            struGsh.nGoodSampCount++;
            if(struGsh.nGoodSampCount> 0){
                struGsh.nAvgH[0] = hsvHTotal/struGsh.nGoodSampCount;
                struGsh.nAvgS[0] = hsvSTotal/struGsh.nGoodSampCount;
                struGsh.nAvgV[0] = hsvVTotal/struGsh.nGoodSampCount;
                struGsh.nAvgR[0] = rTotal/struGsh.nGoodSampCount;
                struGsh.nAvgG[0] = gTotal/struGsh.nGoodSampCount;
                struGsh.nAvgB[0] = bTotal/struGsh.nGoodSampCount;
                struGsh.nAvgRG[0] = rgTotal/struGsh.nGoodSampCount;
                struGsh.nAvgRB[0] = rbTotal/struGsh.nGoodSampCount;
                struGsh.nAvgGB[0] = gbTotal/struGsh.nGoodSampCount;
            }
        }

        g_infoWidget().delayHide();
        updateSampleCount();
    }//end color


    m_PointNumVec[0].clear();
    m_PointNumVec[1].clear();
    m_PointVec[0].clear();
    m_PointVec[1].clear();
    m_NoSlectPointNumVec[0].clear();
    m_NoSlectPointNumVec[1].clear();
    m_NoSlectPointVec[0].clear();
    m_NoSlectPointVec[1].clear();
}

void ImageWidget::updateSampleCount()
{
    ui->rgblabel->setText(QString("%1:%2,%3:%4").arg(g_myLan().good_number).arg(struGsh.nGoodSampCount).arg(g_myLan().bad_number).arg(struGsh.nBadSampCount));
    if(struGsh.m_bManulStudy[1]){
        ui->rgblabel->setText(QString("%1:%2/%3,%4:%5/%6").arg(g_myLan().good_number).arg(struGsh.nShapeSingleGoodSampCount).arg(struGsh.nShapeGoodSampCount).\
                           arg(g_myLan().bad_number).arg(struGsh.nShapeSingleBadSampCount).arg(struGsh.nShapeBadSampCount));
    }
}

void ImageWidget::updatePixColor()
{
    int x = m_StartOriPoint.x();
    int y = m_StartOriPoint.y();
    uchar r=srcBuf[4*y*m_imageWidth+4*x+2];
    uchar g=srcBuf[4*y*m_imageWidth+4*x+1];
    uchar b=srcBuf[4*y*m_imageWidth+4*x];
    uchar i=srcBuf[4*y*m_imageWidth+4*x+3];
    QColor pixelColor(r,g,b);
    QString posColor;
    switch (struCnfp.nSensorDim[m_imageLayer][m_imageView]) {
    case 1:
    {
        QString posStr1("%1:%2");
        posColor =  posStr1.arg(g_myLan().ids_red).arg(r);
    }
        break;
    case 2:
    {
        QString posStr1("r:%1, g:%2");
        posColor =  posStr1.arg(r).arg(g);
    }
        break;
    case 3:
    {
        QString posStr1("r:%1, g:%2, b:%3");
        posColor =  posStr1.arg(r).arg(g).arg(b);
    }
        break;
    case 4:
    {
        QString posStr1("r:%1, g:%2, b:%3, i:%4");
        posColor =  posStr1.arg(r).arg(g).arg(b).arg(i);
    }
        break;
    default:
        break;
    }

    QString rect("(%1,%2)");
    rect = rect.arg(m_StartOriPoint.x()).arg(m_StartOriPoint.y());
    ui->pos_press->setText(rect);
    float rg = (float)r*100.0/((float)r+(float)g);
    QString rgstr =  QString(" R/G(%1%) ").arg(QString::number(rg,'f',1));

    float rb = (float)r*100.0/((float)r+(float)b);
    QString rbstr = QString("R/B(%1%) ").arg(QString::number(rb,'f',1));

    float gb = (float)g*100.0/((float)g+(float)b);
    QString gbstr =  QString("G/B(%1%)").arg(QString::number(gb,'f',1));

//    updatePixelCountLbl();

    int h,s,v;
    float h_new;
    myFlow.getHsv(r,g,b,h,s,v);

    float t,e,a;
#ifdef Q_OS_UNIX
//    Tea converter;
//    converter.getTEA(r,g,b,&t,&e,&a);
#endif
    QString teaStr = QString(" T:%1,E:%2,A:%3").arg(FloatToPercentQString(t)).arg(FloatToPercentQString(e)).arg(FloatToPercentQString(a));

    h_new = h/10.0;
    QString type=g_myLan().ids_background;
    if(bgPoint[y*m_imageWidth+x] == PIXEL_MATTER){
        if(chgBuf[(y*m_imageWidth+x)*4] == 0 && chgBuf[(y*m_imageWidth+x)*4+1] == 0 && chgBuf[(y*m_imageWidth+x)*4+2] == 255)
            type = g_myLan().ai_bad;
        else
            type = g_myLan().ai_good;
    }
    QString showStr;
    showStr = posColor+rgstr+rbstr+gbstr+QString("H:%1,S:%2,V:%3 ").arg(h_new).arg(s).arg(v)+teaStr+" -"+type;
    if(m_widgetType == analyzeType  && !struGsh.m_bAutoStudy){
        QString matcnt=QString(" %1:%2,%3:%4").arg(g_myLan().ai_good).arg(struGsh.nGoodSampCount).arg(g_myLan().ai_bad).arg(struGsh.nBadSampCount);
        showStr = posColor+rgstr+rbstr+gbstr+QString("H:%1,S:%2,V:%3 ").arg(h_new).arg(s).arg(v)+teaStr+matcnt;
    }
    ui->rgblabel->setText(showStr);
}

void ImageWidget::on_cleanSampleBtn_clicked()
{
    myMessageBox msgBox(MSG_QUES, g_myLan().cfm_clean_sample);
    g_Runtime().m_sampleMap[0].clear();
    g_Runtime().m_sampleMap[1].clear();
    int ret = msgBox.exec();
    if (ret == QDialog::Accepted)
    {
        memset(struGsh.nGoodSamp,0,sizeof(struGsh.nGoodSamp));
        memset(struGsh.nBadSamp,0,sizeof(struGsh.nBadSamp));
        struGsh.nGoodSampCount = 0;
        struGsh.nBadSampCount= 0;
        m_SamplePointVec[0].clear();
        m_SamplePointVec[1].clear();
        m_PointNumVec[0].clear();
        m_PointNumVec[1].clear();
        m_PointVec[0].clear();
        m_PointVec[1].clear();
        m_NoSlectPointNumVec[0].clear();
        m_NoSlectPointNumVec[1].clear();
        m_NoSlectPointVec[0].clear();
        m_NoSlectPointVec[1].clear();

        memset(struGsh.nShapeGoodSamp,0,sizeof(struGsh.nShapeGoodSamp));
        memset(struGsh.nShapeBadSamp,0,sizeof(struGsh.nShapeBadSamp));
        struGsh.nShapeSingleGoodSampCount = 0;
        struGsh.nShapeSingleBadSampCount = 0;
        struGsh.nShapeGoodSampCount = 0;
        struGsh.nShapeBadSampCount = 0;

        showGrayColor();
        QImage img = QImage(disBuf,m_imageWidth,m_imageHeight,QImage::Format_ARGB32);
        m_image = img;
    }
    updatePixColor();
    m_tempimage = m_image;
    updateImageSize();
}

void ImageWidget::on_imgDataBtn_clicked()
{
    g_MainManager().ShowWidget(SM_IMAEG_DATA_RGB_CHART_WIDGET);
}

void ImageWidget::on_imgDataBtn_2_clicked()
{
    g_MainManager().ShowWidget(SM_IMAGE_DATA_CHART_WIDGET);
}

void ImageWidget::on_imgDataBtn_3_clicked()
{
    g_MainManager().ShowWidget(SM_IMAEG_DATA_DISCOLOR_CHART_WIDGET);
}

void ImageWidget::on_checkBox_No_Bg_clicked(bool checked)
{
    if(checked){
        uchar value = 0;
        for(int i=0; i<m_imageHeight; i++) {
            for(int j=0; j<m_imageWidth; j++) {
                value = bgPoint[i*m_imageWidth+j];
                if(value == PIXEL_BACKGROUND){
                    chgBuf[(i*m_imageWidth+j)*4] = 255;
                    chgBuf[(i*m_imageWidth+j)*4+1] = 255;
                    chgBuf[(i*m_imageWidth+j)*4+2] = 255;
                }
            }//end j
        }//end i
        showGrayColor();
        QImage img = QImage(disBuf,m_imageWidth,m_imageHeight,QImage::Format_ARGB32);
        m_tempimage = img;
        updateImageSize();
        updateZoomValueText();
        updateSimpleBtnState();
    }else{
        updateShowPage();
    }
}

void ImageWidget::on_pushButton_clicked()
{
    int matCount = 0;
    int totalRValue = 0;
    int totalGValue = 0;
    int totalBValue = 0;
    for(int i=0; i<256; i++){
        totalRValue += struGsh.colorRGBTotal[0][i]*i;
        totalGValue += struGsh.colorRGBTotal[1][i]*i;
        totalBValue += struGsh.colorRGBTotal[2][i]*i;
        matCount += struGsh.colorRGBTotal[0][i];
    }
    if(matCount != 0){
        struGsh.nMatAvgR = totalRValue/matCount;
        struGsh.nMatAvgG = totalGValue/matCount;
        struGsh.nMatAvgB = totalBValue/matCount;
    }else{
        struGsh.nMatAvgR = 0;
        struGsh.nMatAvgG = 0;
        struGsh.nMatAvgB = 0;
    }
    g_MainManager().ShowWidget(SM_IMAGE_INFO_RGB_WIDGET);
}

void ImageWidget::on_sampleClassBtn_clicked()
{
    g_MainManager().ShowWidget(SM_SVM_SAMPLE_WIDGET);
}

void ImageWidget::on_lastPicBtn_clicked()
{
    g_Runtime().updateTmpImgList();
    if( struGsh.nImageIndex > 0){
        struGsh.nImageIndex --;
        int layer,view,nUnit;
        g_Runtime().getCurImageLayerAndView(layer,view,nUnit,struGsh.nImageIndex);
        setImage(g_Runtime().nimageNameVec[struGsh.nImageIndex],layer,view,nUnit);
        updateShowPage();
    }
}

void ImageWidget::on_nextPicBtn_clicked()
{
    g_Runtime().updateTmpImgList();
    if( struGsh.nImageIndex < g_Runtime().nimageNameVec.size()-1){
        struGsh.nImageIndex ++;
        int layer,view,nUnit;
        g_Runtime().getCurImageLayerAndView(layer,view,nUnit,struGsh.nImageIndex);
        setImage(g_Runtime().nimageNameVec[struGsh.nImageIndex],layer,view,nUnit);
        updateShowPage();
    }
}

void ImageWidget::m_saturationSlider_valueChanged(int value)
{
    if(m_image.isNull()) return;
    struCnfg.imgSaturationFactor = value;

//    if (!m_timer->isActive()) {
//        m_timer->start();
//    }
}
void ImageWidget::m_saturationSlider_released()
{
//    if(m_timer->isActive())
//        m_timer->stop(); // 停止定时器
    ui->m_saturationValue->setText(QString::number(struCnfg.imgSaturationFactor-100));; // 立即更新
    m_saturationSliderTimeOut();
}

void ImageWidget::m_saturationSliderTimeOut()
{
    ui->m_saturationValue->setText(QString::number(struCnfg.imgSaturationFactor-100));
    // 计算饱和度缩放因子（100% 对应 1.0，0% 对应 0.0，200% 对应 2.0）
    double saturationFactor = struCnfg.imgSaturationFactor / 100.0;
    memcpy(oriBuf, srcBuf,sizeof(oriBuf));

    uchar* src = m_oriImage.bits();
    uchar* dst = m_tempimage.bits();
    uchar* ori = oriBuf;

    int n = m_imageWidth * m_imageHeight;
    #define CLAMP(c) (((c) < 0) ? 0 : ((c) > 255) ? 255 : (c))
    for (int i = 0; i < n; i++) {
        int b = src[0];
        int g = src[1];
        int r = src[2];

        int y = (r * 299 + g * 587 + b * 114) / 1000;

        r = CLAMP(y + (r - y) * saturationFactor);
        g = CLAMP(y + (g - y) * saturationFactor);
        b = CLAMP(y + (b - y) * saturationFactor);

        dst[0] = b;
        dst[1] = g;
        dst[2] = r;

        ori[0] = b;
        ori[1] = g;
        ori[2] = r;

        src += 4;
        dst += 4;
        ori += 4;
    }
    #undef CLAMP
    m_image = m_tempimage;
    showGrayColor();
    updateImageSize();
    updateZoomValueText();
    updateSimpleBtnState();
}
