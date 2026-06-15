#include "svmimagewidget.h"
#include "ui_svmimagewidget.h"
#include <QMouseEvent>
#include <QTime>
#include "setsvmusedialog.h"
#include "global.h"
const int markArray[4]={1,3,5,7};
SvmImageWidget::SvmImageWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::SvmImageWidget)
{
    ui->setupUi(this);
    m_bIsPurSim = true;
    m_bRGB = true;
    m_imageWidth = IMAGE_WIDTH;
    m_imageHeight = ImageLabelHeight;
    m_imageIndex = 0;
    ui->listWidget->setMinimumHeight(110);
    ui->listWidget->setIconSize(QSize(188,100));
    ui->listWidget->setViewMode(QListView::IconMode);
    ui->listWidget->setMovement(QListView::Static);
    ui->listWidget->setFlow(QListView::TopToBottom);
    m_pThread = new SvmLearnThread(this);
    connect(m_pThread,SIGNAL(finished()),this,SLOT(learnFinished()));
    m_pSimulateThread = new SvmSimulateThread(this);
    connect(m_pSimulateThread,SIGNAL(finished()),this,SLOT(simulateFinished()));
    m_pSvmMarkThread = new SvmMarkThread(this);
    connect(m_pSvmMarkThread,SIGNAL(finished()),this,SLOT(markFinished()));
    m_simpleType = Good;
    // g_Runtime().m_sampleMap[Good].clear();
    // g_Runtime().m_sampleMap[Bad].clear();
    m_colorType = rgb;
    m_markIndex = 1;
    marking = false;
    ui->checkBox_svmNew->hide();
    m_bSimulate = false;
    m_bNoBackground = false;
}

SvmImageWidget::~SvmImageWidget()
{
    if(chgBuf){
        free(chgBuf);
        chgBuf = NULL;
    }
    if(oriBuf){
        free(oriBuf);
        oriBuf = NULL;
    }
    if(disBuf){
        free(disBuf);
        disBuf = NULL;
    }
    if(bgPoint){
        free(bgPoint);
        bgPoint = NULL;
    }
    delete ui;
}


void SvmImageWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){
        if(m_bSimulate){
            updateSvmSimulateEditValue();
            displaySimulate();
            showGrayColor();
            m_rgbnImage = QImage(chgBuf,m_imageWidth,m_imageHeight,QImage::Format_ARGB32);
            m_tempimage = m_rgbnImage;
            updateImageSize();
        }
    }
}

void SvmImageWidget::displaySimulate()
{
    g_infoWidget().setLabelText(g_myLan().ai_simulating);
    g_infoWidget().show();
    myFlow.msleep(20);
    m_bNoBackground = false;
    ui->checkBox_No_Bg->setChecked(m_bNoBackground);
    m_pSimulateThread->start();
}
void SvmImageWidget::showPage(bool bshow)
{
    m_PointVec[0].clear();
    m_PointVec[1].clear();
    m_PointNumVec[0].clear();
    m_PointNumVec[1].clear();

    if(bshow){
        struGsh.m_isNewTrain = 1; //默认就使用张涛的新库了
        ui->checkBox_svmNew->setChecked(struGsh.m_isNewTrain == 1);
        QStringList marklist,svmTypelist,colorTypeList;
        marklist<<"L0"<<"L1"<<"L2"<<"L3";
        svmTypelist<<"1"<<"2"<<"3";
        colorTypeList<<"origin"<<"rgb"<<"r"<<"g"<<"b"<<"n"<<"nrg"<<"nrb"<<"ngb";
        ui->comboBoxMarkIndex->clear();
        ui->comboBoxMarkIndex->addItems(marklist);
        ui->comboBoxMarkIndex->setCurrentIndex(m_markIndex);
        ui->comboBoxSvmType->clear();
        ui->comboBoxSvmType->addItems(svmTypelist);
        ui->comboBoxSvmType->setCurrentIndex(struCnfg.n_svmType);
        ui->comboBoxImageType->clear();
        ui->comboBoxImageType->addItems(colorTypeList);
        ui->comboBoxImageType->setCurrentIndex(m_colorType);
        ui->comboBoxImageType->setVisible(m_imageDim == 4);

        ui->checkBox_No_Bg->setVisible(struGsh.bRunMode >= RUNMODE_FACTORY);
        ui->simulateCheckBox->setChecked(m_bSimulate);
        if(m_bSimulate)
            ui->checkBox_No_Bg->setChecked(false);
        g_Runtime().updateTmpImgList();
        updateImageList();
		if(!g_Runtime().nimageNameVec.isEmpty()){
			if(m_imageIndex < g_Runtime().nimageNameVec.count()){
				updateCurrentImage(m_imageIndex,true);
			}
		}
        connect(ui->listWidget,SIGNAL(clicked(QModelIndex)),this,SLOT(imageListItemSelect(QModelIndex)));
        updateShowPage();

    }else{
        disconnect(ui->listWidget,SIGNAL(clicked(QModelIndex)),this,SLOT(imageListItemSelect(QModelIndex)));//清空list前需要断开信号槽连接
        while (ui->listWidget->count()>0) {
           QListWidgetItem *item = ui->listWidget->takeItem(0);
           if(item){
               delete item;
           }
        }
        if(chgBuf){
            free(chgBuf);
            chgBuf = NULL;
        }
        if(oriBuf){
            free(oriBuf);
            oriBuf = NULL;
        }
        if(disBuf){
            free(disBuf);
            disBuf = NULL;
        }
        if(bgPoint){
            free(bgPoint);
            bgPoint = NULL;
        }
    }
}

void SvmImageWidget::setImage( QString path, int layer, int view, int unit)
{
    if (!path.contains(".bmp"))
        return;
    QFile myfile(path);
    myfile.open(QIODevice::ReadOnly);
    QByteArray head1 = myfile.read(sizeof(BITMAPFILEHEADER));
    QByteArray head2 = myfile.read(sizeof(BITMAPINFOHEADER));
    BITMAPINFOHEADER info;
    memcpy(&info,head2,sizeof(BITMAPINFOHEADER));

    int type = (info.biHeight&(0x80000000))>>31;
    m_imageHeight= (type == 1) ? info.biHeight*(-1) :info.biHeight;
    m_imageWidth = info.biWidth;

    if(oriBuf) {
        free(oriBuf);
        oriBuf = NULL;
    }
   if(chgBuf) {
       free(chgBuf);
       chgBuf = NULL;
   }
   if(disBuf){
       free(disBuf);
       disBuf = NULL;
   }
   if(bgPoint){
       free(bgPoint);
       bgPoint = NULL;
   }
    oriBuf = (uchar *)malloc(m_imageWidth*m_imageHeight*4);
    chgBuf = (uchar *)malloc(m_imageWidth*m_imageHeight*4);
    disBuf = (uchar *)malloc(m_imageWidth*m_imageHeight*4);
    bgPoint = (uchar *)malloc(m_imageWidth*m_imageHeight);
    memset(oriBuf,0,m_imageWidth*m_imageHeight*4);
    memset(bgPoint,0,m_imageWidth*m_imageHeight);

    memcpy(oriBuf,myfile.read(myfile.size()-54).data(),myfile.size()-54);
    memcpy(chgBuf, oriBuf, m_imageWidth*m_imageHeight*4);

    qDebug()<<m_imageWidth<<m_imageHeight<<info.biBitCount<<type<<m_colorType;
    m_imageDim = 4;
    if(info.biBitCount == 24){
        m_imageDim = 3;
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
        memcpy(chgBuf, oriBuf, m_imageWidth*m_imageHeight*4);
    }
    else{
        showGrayColor();
    }

    ui->comboBoxImageType->setVisible(m_imageDim == 4);
    //直接对第四维统一处理，可能会影响到真实四维图片
    QImage image(chgBuf,m_imageWidth,m_imageHeight,QImage::Format_ARGB32);
    m_image = image;
    m_tempimage = image;
    m_oriImage = image;
    m_rgbnImage = image;

    m_imageLayer = layer;
    m_imageView = view;
    m_imageUnit = unit;

    if(m_bSimulate){
        for(int i=0; i<m_imageHeight; i++){
            for(int j=0; j<m_imageWidth; j++){
                uchar r = *(oriBuf+i*m_imageWidth*4+j*4+2);
                uchar g = *(oriBuf+i*m_imageWidth*4+j*4+1);
                uchar b = *(oriBuf+i*m_imageWidth*4+j*4+0);
                uchar n = *(oriBuf+i*m_imageWidth*4+j*4+3);
                if(j < struCnfg.struLayerInfo[m_imageLayer].stuCameraInfo[m_imageView].nChannelBegin[m_imageUnit] \
                        || j > struCnfg.struLayerInfo[m_imageLayer].stuCameraInfo[m_imageView].nChannelEnd[m_imageUnit]) {
                    bgPoint[i*m_imageWidth+j] = PIXEL_BACKGROUND;
                    continue;
                }
                if(myFlow.imagePixelIsBackground(r, g, b, n, m_imageLayer, m_imageView,struGsh.nImgTdMode)){
                    bgPoint[i*m_imageWidth+j] = PIXEL_BACKGROUND;
                    continue;
                }
            }
        }
        displaySimulate();
    }
    else{
        markAndInitImage();
    }

    myfile.close();
}

void SvmImageWidget::adapteImageCenterPos()
{
    if(m_imageWidth*m_ZoomValue < ImageLabelWidth){
        m_CenterX = m_imageWidth*m_ZoomValue/2;
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
    if(m_CenterY < ImageLabelHeight/2/m_ZoomValue){
        m_CenterY = ImageLabelHeight/2/m_ZoomValue;
    }else if( m_CenterY>m_imageHeight-ImageLabelHeight/2/m_ZoomValue){
        m_CenterY = m_imageHeight-ImageLabelHeight/2/m_ZoomValue;
        if(ImageLabelHeight/2%m_ZoomValue != 0){
            m_CenterY = m_imageHeight-ImageLabelHeight/2/m_ZoomValue-m_ZoomValue;
        }

    }
}

void SvmImageWidget::updateZoomValueText()
{
    ui->reduce->setEnabled(m_ZoomValue > MinZoom);
    ui->enlarge->setEnabled(m_ZoomValue < MaxZoom);
}

void SvmImageWidget::showGrayColor()
{
    for(int i=0; i<m_imageHeight; i++) {
        for(int j=0; j<m_imageWidth; j++) {
            switch(m_colorType){
            case rgb:
                chgBuf[(i*m_imageWidth+j)*4] = oriBuf[(i*m_imageWidth+j)*4];
                chgBuf[(i*m_imageWidth+j)*4+1] = oriBuf[(i*m_imageWidth+j)*4+1];
                chgBuf[(i*m_imageWidth+j)*4+2] = oriBuf[(i*m_imageWidth+j)*4+2];
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

void SvmImageWidget::updateSamplesBtnState()
{
    if(m_simpleType == Good){
        ui->goodTypeBtn->setChecked(true);
    }else if(m_simpleType == Bad){
        ui->badTypeBtn->setChecked(true);
    }
}

void SvmImageWidget::updateSvmSimulateEditValue()
{
    ui->applyBtn->setVisible(struGsh.ntrainComplete);
    ui->svmRowEdit->setMinMax(this,1,32,-1,&struGsh.nSvmRow);
    ui->svmRowEdit->setText(QString::number(struGsh.nSvmRow));
    ui->svmPerEdit->setVisible(struGsh.ntrainComplete  && struGsh.bRunMode >= RUNMODE_FACTORY);
    ui->svmPerEdit->setMinMax(this,1,struGsh.nSvmRow*struGsh.nSvmRow,-1,&struGsh.nSvmPercent);
    ui->svmPerEdit->setText(QString::number(struGsh.nSvmPercent));
    ui->svmSensEdit->setVisible(struGsh.ntrainComplete  && struGsh.bRunMode >= RUNMODE_FACTORY);
    ui->svmSensEdit->setMinMax(this,0,300,-1,&struGsh.nSvmSimulateSens);
    ui->svmSensEdit->setText(QString::number(struGsh.nSvmSimulateSens));
    ui->svmRowEdit->setVisible(struGsh.ntrainComplete  && struGsh.bRunMode >= RUNMODE_FACTORY);
//    ui->simulateCheckBox->setVisible(struGsh.ntrainComplete  && struGsh.bRunMode >= RUNMODE_FACTORY);
}
void SvmImageWidget::mousePressEvent(QMouseEvent *event)
{
    if(!ui->imageLabel->geometry().contains(event->pos()))
        return;
    posStart = event->pos()-ui->imageLabel->pos();
    if(m_imageWidth*m_ZoomValue < ImageLabelWidth){
        m_StartOriPoint.setX((posStart.x()-m_imageWidth*m_ZoomValue/2)/m_ZoomValue+m_CenterX);
        m_StartOriPoint.setY((posStart.y()-ImageLabelHeight/2)/m_ZoomValue+m_CenterY);
    }else{
        m_StartOriPoint.setX((posStart.x()-ImageLabelWidth/2)/m_ZoomValue+m_CenterX);
        m_StartOriPoint.setY((posStart.y()-ImageLabelHeight/2)/m_ZoomValue+m_CenterY);
    }
}

void SvmImageWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_bMoveImage| marking| m_bSimulate)
    {
        updatePixColor();
        return;
    }
    if(!ui->imageLabel->geometry().contains(event->pos())){
        return;
    }
    posTmp = event->pos()-ui->imageLabel->pos();
    int index = m_StartOriPoint.y()*m_imageWidth+m_StartOriPoint.x();
    if(!m_MatMap.contains(index))
    {
       ui->label->setText(QString("<font color=red>%1</font>").arg(g_myLan().BG_points_cannot_be_marked));
    }
    else
    {
        if(m_MatMap[index] != NoUse){
            if(m_MatMap[index] == Good){
                ui->label->setText(QString("<font color=red>%1</font>").arg(g_myLan().Already_marked_as_good));
            }else if(m_MatMap[index]== Bad){
                ui->label->setText(QString("<font color=red>%1</font>").arg(g_myLan().Already_marked_as_bad));
            }
        }else{
            ui->label->setText(g_myLan().ids_succeeded);
//            markMatColor();
            m_pSvmMarkThread->start();
            return;
        }
    }
    updateImageSize();
    updatePixColor();
    updateBtnState();
}


SvmMarkThread::SvmMarkThread(SvmImageWidget *parent)
{
   m_pParent = parent;
}

void SvmMarkThread::run()
{
    struGsh.bUpdateState = 1;
    m_pParent->marking = true;
    m_pParent->markMatColor();
}

void SvmImageWidget::markFinished()
{
    updateImageSize();
    updatePixColor();
    updateBtnState();
    marking = false;
    struGsh.bUpdateState = 0;
}

void SvmImageWidget::markMatColor()
{
    int x = m_StartOriPoint.x();
    int y = m_StartOriPoint.y();
    uchar r=oriBuf[4*y*m_imageWidth+4*x+2];
    uchar g=oriBuf[4*y*m_imageWidth+4*x+1];
    uchar b=oriBuf[4*y*m_imageWidth+4*x];
    uchar n=oriBuf[4*y*m_imageWidth+4*x+3];
    QList<int> keyList = m_MatMap.keys();//存放的就是QMap的key值
    int matX,matY,red,green,blue,nir,dist1,dist2,dist3,dist4;
    int realY;
    QPoint matPoint;
    m_tempSelectNum = 0;
    int c_num = 0;
    for(int i=0;i<keyList.size();i++)
    {
        if(m_MatMap[keyList[i]] == NoUse){
            c_num++;
            matX = keyList[i]%m_imageWidth;
            matY = keyList[i]/m_imageWidth;
            matPoint = QPoint(matX,matY);
            realY = matY*m_imageWidth;
//            if(!m_PointVec[m_simpleType].contains(matPoint))
            {
                red = oriBuf[realY*4+matX*4+2];
                green = oriBuf[realY*4+matX*4+1];
                blue = oriBuf[realY*4+matX*4];
                nir = oriBuf[realY*4+matX*4+3];
                dist1 = qAbs(red - r);
                dist2 = qAbs(green - g);
                dist3 = qAbs(blue - b);
                dist4 = qAbs(nir - n);

                if((dist1 <= markArray[m_markIndex]) && (dist2 <= markArray[m_markIndex])
                        && (dist3 <= markArray[m_markIndex])&& (dist4 <= markArray[m_markIndex]))
                {
                    m_PointVec[m_simpleType].append(matPoint);
                    m_tempSelectNum++;
                    if(m_simpleType == Good)
                    {
                        m_MatMap.insert(realY+matX,Good);
                        m_tempimage.setPixel(matPoint,qRgba(0,255,0,255));
                    } else{
                        m_MatMap.insert(realY+matX,Bad);
                        m_tempimage.setPixel(matPoint,qRgba(255,0,0,255));
                    }
                }
            }

        }
    }
    if(m_tempSelectNum > 0)
        m_PointNumVec[m_simpleType].append(m_tempSelectNum);
}

void SvmImageWidget::mouseMoveEvent(QMouseEvent *event)
{
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
        updateImageSize();
        return;
    }
}

void SvmImageWidget::retranslateUiWidget()
{
    ui->checkBox_No_Bg->setText(g_myLan().ids_background);
    ui->checkBox->setText(g_myLan().movefix);
    ui->saveSampleBtn->setText(g_myLan().ai_add_sample);
    ui->goodTypeBtn->setText(g_myLan().ai_good);
    ui->badTypeBtn->setText(g_myLan().ai_bad);
    ui->revokeBtn->setText(g_myLan().ai_reselect_last);
    ui->cleanSampleBtn->setText(g_myLan().clean_sample);
    ui->svmLearnBtn->setText(g_myLan().ai_calculate);
    ui->applyBtn->setText(g_myLan().ids_apply);
    ui->returnBtn->setText(g_myLan().ids_back);
    ui->importSampleBtn->setText(g_myLan().ids_sampleDetails);
    ui->label_mark->setText(g_myLan().ids_setSample);
    ui->simulateCheckBox->setText(g_myLan().ai_simulate);
}

void SvmImageWidget::updateBtnState()
{
    if(m_PointVec[0].size() > 0 || m_PointVec[1].size()>0){
        ui->saveSampleBtn->setEnabled(true);
    }else{
        ui->saveSampleBtn->setEnabled(false);
    }

    if(struGsh.nGoodSampCount > 0 || struGsh.nBadSampCount > 0){
        ui->cleanSampleBtn->setEnabled(true);
    }else{
        ui->cleanSampleBtn->setEnabled(false);
    }
    if(struGsh.nGoodSampCount > 512 && struGsh.nBadSampCount > 512){
        ui->svmLearnBtn->setEnabled(true);
    }else{
        ui->svmLearnBtn->setEnabled(false);
    }
    if(m_PointNumVec[m_simpleType].isEmpty()){
        ui->revokeBtn->setEnabled(false);
    }else{
        ui->revokeBtn->setEnabled(true);
    }
    ui->applyBtn->setVisible(struGsh.ntrainComplete);
    QString str = QString(" %1:%2,%3:%4").arg(g_myLan().ai_good).arg(struGsh.nGoodSampCount).arg(g_myLan().ai_bad).arg(struGsh.nBadSampCount);
    QString samplestr = QString("<font color=green>%1</font>").arg(str);
    ui->label_sample->setText(samplestr);
}

void SvmImageWidget::updateImageSize()
{
#if 0
    if(m_imageWidth*m_ZoomValue < ImageLabelWidth){
        QImage GrabImage = m_tempimage.copy(m_CenterX-m_imageWidth/m_ZoomValue/2,m_CenterY-ImageLabelHeight/m_ZoomValue/2,m_imageWidth*m_ZoomValue,ImageLabelHeight/m_ZoomValue);
        ui->imageLabel->setFixedWidth(m_imageWidth*m_ZoomValue);
        ui->imageLabel->setPixmap(QPixmap::fromImage(GrabImage));
    }else{
        ui->imageLabel->setFixedWidth(ImageLabelWidth);
        QImage GrabImage = m_tempimage.copy(m_CenterX-ImageLabelWidth/m_ZoomValue/2,m_CenterY-ImageLabelHeight/m_ZoomValue/2,ImageLabelWidth/m_ZoomValue,ImageLabelHeight/m_ZoomValue);
        if(m_ZoomValue != 1){
            QImage img(ImageLabelWidth,ImageLabelHeight,QImage::Format_ARGB32);
            for(int y = 0;y < ImageLabelHeight;y++)
                for(int x = 0; x <ImageLabelWidth;x++){
                    img.setPixel(x,y,GrabImage.pixel(x/m_ZoomValue,y/m_ZoomValue));
                }
            ui->imageLabel->setPixmap(QPixmap::fromImage(img));
        }else
            ui->imageLabel->setPixmap(QPixmap::fromImage(GrabImage));
    }
#endif
    if(m_imageWidth*m_ZoomValue < ImageLabelWidth){
           QImage GrabImage = m_tempimage.copy(m_CenterX-m_imageWidth/m_ZoomValue/2,m_CenterY-ImageLabelHeight/m_ZoomValue/2,m_imageWidth*m_ZoomValue,ImageLabelHeight/m_ZoomValue);
           ui->imageLabel->setFixedWidth(m_imageWidth*m_ZoomValue);
           ui->imageLabel->setFixedHeight(ImageLabelHeight);
           ui->imageLabel->setPixmap(QPixmap::fromImage(GrabImage));
       }else{
           QImage GrabImage = m_tempimage.copy(m_CenterX-ImageLabelWidth/m_ZoomValue/2,m_CenterY-ImageLabelHeight/m_ZoomValue/2,ImageLabelWidth/m_ZoomValue,ImageLabelHeight/m_ZoomValue);
           if(m_ZoomValue != 1)
               GrabImage = GrabImage.scaled(ImageLabelWidth,ImageLabelHeight,Qt::KeepAspectRatio);
           ui->imageLabel->setFixedWidth(ImageLabelWidth);
           ui->imageLabel->setFixedHeight(ImageLabelHeight);
           ui->imageLabel->setPixmap(QPixmap::fromImage(GrabImage));
       }
}

void SvmImageWidget::on_enlarge_clicked()
{
    if(m_ZoomValue < MaxZoom)
        m_ZoomValue = m_ZoomValue*2;
    adapteImageCenterPos();
    updateImageSize();
    updateZoomValueText();
}

void SvmImageWidget::on_reduce_clicked()
{
    if(m_ZoomValue>MinZoom)
        m_ZoomValue = m_ZoomValue/2;
    adapteImageCenterPos();
    updateImageSize();
    updateZoomValueText();
}


void SvmImageWidget::on_checkBox_clicked()
{
    m_bMoveImage = ui->checkBox->isChecked();
}

void SvmImageWidget::on_revokeBtn_clicked()
{
    if(!m_PointNumVec[m_simpleType].isEmpty()){
        int lastSelectPointNum = m_PointNumVec[m_simpleType][m_PointNumVec[m_simpleType].size()-1];
        QPoint point;
        for(int i = m_PointVec[m_simpleType].count()-lastSelectPointNum;i<m_PointVec[m_simpleType].count();i++){
            point = m_PointVec[m_simpleType][i];
            m_MatMap[point.y()*m_imageWidth+point.x()] = NoUse;
            m_tempimage.setPixel(point,m_rgbnImage.pixel(point));
        }

        m_PointVec[m_simpleType].resize(m_PointVec[m_simpleType].count()-lastSelectPointNum);
        m_PointNumVec[m_simpleType].pop_back();
        updateImageSize();
        m_image = m_tempimage;
    }
    updateBtnState();
}

void SvmImageWidget::on_goodTypeBtn_clicked(bool checked)
{
    if(checked)
        m_simpleType = Good;
    updateBtnState();
}

void SvmImageWidget::on_badTypeBtn_clicked(bool checked)
{
    if(checked)
        m_simpleType = Bad;
    updateBtnState();
}

void SvmImageWidget::updatePixelCountLbl()
{
    int x = m_StartOriPoint.x();
    int y = m_StartOriPoint.y();
    uchar r=oriBuf[4*y*m_imageWidth+4*x+2];
    uchar g=oriBuf[4*y*m_imageWidth+4*x+1];
    uchar b=oriBuf[4*y*m_imageWidth+4*x];
    uchar i=oriBuf[4*y*m_imageWidth+4*x+3];
    QString badNumStr = "";
}


bool SvmImageWidget::pixelIsOutThreshold(int x, int y)
{

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

      }
   else if(m_simpleType == Bad)
   {
       if((r < struGsh.svmThreshold[0][2]) || (r > struGsh.svmThreshold[0][3]) || (g < struGsh.svmThreshold[1][2]) \
               || (g > struGsh.svmThreshold[1][3]) || (b < struGsh.svmThreshold[2][2]) || (b > struGsh.svmThreshold[2][3])){
           return true;
       }

      }

    return false;
}

/* 优化计算时判断像素点是否为冗余点 */
bool SvmImageWidget::pixelIsReSelected(uchar r, uchar g, uchar b)
{
    QPoint point;
    int x,y;
    for(int i = 0;i < 2;i++){
        if(m_PointVec[i].count() > 0){
            for(int j = 0;j < m_PointVec[i].count();j++){
                point = m_PointVec[i][j];
                x = point.x();
                y = point.y();
                if(r == oriBuf[(y*m_imageWidth+x)*4+2] && g == oriBuf[(y*m_imageWidth+x)*4+1]&& b == oriBuf[(y*m_imageWidth+x)*4])
                    return true;
            }
        }
    }
    return false;
}

/* 优化取样时判断像素点是否为边缘点 */
bool SvmImageWidget::pixelIsEdge(int x, int y)
{
    if(x==0 || y==0 || x==IMAGE_WIDTH-1 || y==IMAGE_HEIGHT-1)
        return false;
    static int tmpX,tmpY;
    for(int i=-1; i<=1; i++){
        for(int j=-1; j<=1; j++){
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

void SvmImageWidget::on_saveSampleBtn_clicked()
{
    QPoint point;
    int x = 1,y = 1,tr = 0,tg = 0,tb = 0,tn = 0;
    if(m_PointVec[Good].count() == 0 && m_PointVec[Bad].count() ==0){
        g_infoWidget().setLabelText(g_myLan().cfm_ai_mark_sample);
        g_infoWidget().showSecs(2);
        return;
    }

    memset(struGsh.nGoodSamp,0,sizeof(struGsh.nGoodSamp));
    memset(struGsh.nBadSamp,0,sizeof(struGsh.nBadSamp));
    struGsh.nGoodSampCount = 0;
    struGsh.nBadSampCount = 0;
    if(m_PointVec[Good].count()>0){
        for(int i = 0;i < m_PointVec[Good].count();i++){
            point = m_PointVec[Good][i];
            x = point.x();
            y = point.y();

            SampleObj obj = {oriBuf[(y*m_imageWidth+x)*4+2],oriBuf[(y*m_imageWidth+x)*4+1],oriBuf[(y*m_imageWidth+x)*4],oriBuf[(y*m_imageWidth+x)*4+3]};
            if(g_Runtime().m_sampleMap[Good].contains(obj)){
                continue;
            }
            g_Runtime().m_sampleMap[Good].insert(obj,Good);
        }
    }

    if(m_PointVec[Bad].count()>0){
        for(int i = 0;i < m_PointVec[Bad].count();i++){

            point = m_PointVec[Bad][i];
            x = point.x();
			y = point.y();
            if(pixelIsEdge(x,y))//默认bad剔除边界点
				continue;
            SampleObj obj = {oriBuf[(y*m_imageWidth+x)*4+2],oriBuf[(y*m_imageWidth+x)*4+1],oriBuf[(y*m_imageWidth+x)*4],oriBuf[(y*m_imageWidth+x)*4+3]};
            g_Runtime().m_sampleMap[1].insert(obj,Bad);
        }
    }

    QMap<SampleObj,int>::const_iterator var;
    setSamplesFileName(Bad);
    if(m_badSamplesFile.open(QIODevice::ReadWrite|QIODevice::Append|QIODevice::Text))
    {
        QTextStream out(&m_badSamplesFile);
        QString tempString("");
        for (var = g_Runtime().m_sampleMap[Bad].constBegin(); var !=g_Runtime().m_sampleMap[Bad].constEnd(); ++var) {
            if(struGsh.nBadSampCount >= MAX_SAMPLE_NUM-1){
                ui->label->setText(g_myLan().Bad_material_enough);
                break;
            }
            tr = var.key().r;
            tg = var.key().g;
            tb = var.key().b;
            tn = var.key().n;
            struGsh.nBadSamp[struGsh.nBadSampCount*4] = tr;
            struGsh.nBadSamp[struGsh.nBadSampCount*4+1] = tg;
            struGsh.nBadSamp[struGsh.nBadSampCount*4+2] = tb;
            struGsh.nBadSamp[struGsh.nBadSampCount*4+3] = tn;
            struGsh.nBadSampCount++;
            tempString+=QString::number(tr);
            tempString+=QString(" ");

            tempString+=QString::number(tg);
            tempString+=QString(" ");

            tempString+=QString::number(tb);
            tempString+=QString(" ");

            tempString+=QString::number(tn);
            tempString+=QString(" ");
            tempString += QString::number(Bad);
            out<<tempString<<"\n";
            tempString.clear();
        }
        m_badSamplesFile.close();
    }else{
        ui->label->setText(g_myLan().Save_sample_failed);  //717#保存样本失败
    }

    setSamplesFileName(Good);
    if(m_goodSamplesFile.open(QIODevice::ReadWrite|QIODevice::Append|QIODevice::Text))
    {
        QTextStream out(&m_goodSamplesFile);
        QString tempString("");
        for (var = g_Runtime().m_sampleMap[Good].constBegin(); var !=g_Runtime().m_sampleMap[Good].constEnd(); ++var) {
            if(struGsh.nGoodSampCount >= MAX_SAMPLE_NUM-1){
                ui->label->setText(g_myLan().Good_material_enough);
                break;
            }
            tr = var.key().r;
            tg = var.key().g;
            tb = var.key().b;
            tn = var.key().n;
            struGsh.nGoodSamp[struGsh.nGoodSampCount*4] = tr;
            struGsh.nGoodSamp[struGsh.nGoodSampCount*4+1] = tg;
            struGsh.nGoodSamp[struGsh.nGoodSampCount*4+2] = tb;
            struGsh.nGoodSamp[struGsh.nGoodSampCount*4+3] = tn;
            struGsh.nGoodSampCount++;
            tempString+=QString::number(tr);
            tempString+=QString(" ");

            tempString+=QString::number(tg);
            tempString+=QString(" ");

            tempString+=QString::number(tb);
            tempString+=QString(" ");

            tempString+=QString::number(tn);
            tempString+=QString(" ");

            tempString += QString::number(Good);
            out<<tempString<<"\n";
            tempString.clear();
        }
            m_goodSamplesFile.close();
    }
    else
    {
        ui->label->setText(g_myLan().Save_sample_failed);  //717#保存样本失败
    }

    for(int i = 0;i<2;i++){
        m_PointNumVec[i].clear();
        m_PointVec[i].clear();
    }

    updateImageSize();
    updatePixColor();
    updateBtnState();
}
void SvmImageWidget::setSamplesFileName(SamplesType mode)
{
    QString SampleFileName("");
    SampleFileName = getSamplesFilePath(mode,struGsh.nView,0,true);
	QFile::remove(SampleFileName);
    if(mode == Good){
        m_goodSamplesFile.setFileName(SampleFileName);
    }else{
        m_badSamplesFile.setFileName(SampleFileName);
    }
}

void SvmImageWidget::markAndInitImage()
{
    int sensorSplice = struCnfp.nImageSplice[m_imageLayer][m_imageView];
    sensorSplice = (sensorSplice == 0)?1:2;
    if(struGsh.nImgTdMode  == 2 && struCnfp.nSensorPixel[0][struGsh.nView] == SENSOR_PIXELS_2048_XTD
            && struCnfp.nViewSensorType[0][struGsh.nView] == SENSOR_G2)
    {
        sensorSplice = 2;
    }
    m_MatMap.clear();
    for(int i=0; i<m_imageHeight; i++){
        if(!struGsh.bContinuous){
            for(int j=0; j<m_imageWidth; j++){
                uchar r = *(oriBuf+i*m_imageWidth*4+j*4+2);
                uchar g = *(oriBuf+i*m_imageWidth*4+j*4+1);
                uchar b = *(oriBuf+i*m_imageWidth*4+j*4+0);
                uchar n = *(oriBuf+i*m_imageWidth*4+j*4+3);
                if(j < struCnfg.struLayerInfo[m_imageLayer].stuCameraInfo[m_imageView].nChannelBegin[m_imageUnit]/sensorSplice \
                                || j > struCnfg.struLayerInfo[m_imageLayer].stuCameraInfo[m_imageView].nChannelEnd[m_imageUnit]/sensorSplice) {
                    bgPoint[i*m_imageWidth+j] = PIXEL_BACKGROUND;

                    if(m_bNoBackground){
                        m_tempimage.setPixel(j,i,Qt::white);
                    }
                    continue;
                }
                if(myFlow.imagePixelIsBackground(r, g, b, n, m_imageLayer, m_imageView,struGsh.nImgTdMode)){
                    bgPoint[i*m_imageWidth+j] = PIXEL_BACKGROUND;
                    if(m_bNoBackground){
                        m_tempimage.setPixel(j,i,Qt::white);
                    }
                    continue;
                }
                SampleObj objTmp1 = {r, g, b, n};
                if(g_Runtime().m_sampleMap[Bad].contains(objTmp1)){
                    m_MatMap.insert(i*m_imageWidth+j,Bad);
                    m_tempimage.setPixel(j,i,qRgba(255,0,0,255));
                    continue;
                }else if(g_Runtime().m_sampleMap[Good].contains(objTmp1)){
                    m_MatMap.insert(i*m_imageWidth+j,Good);
                    m_tempimage.setPixel(j,i,qRgba(0,255,0,255));
                    continue;
                }
                m_MatMap.insert(i*m_imageWidth+j,NoUse);
            }
        }
    }
}

void SvmImageWidget::updatePixColor()
{
    int x = m_StartOriPoint.x();
    int y = m_StartOriPoint.y();
    uchar r=oriBuf[4*y*m_imageWidth+4*x+2];
    uchar g=oriBuf[4*y*m_imageWidth+4*x+1];
    uchar b=oriBuf[4*y*m_imageWidth+4*x];
    uchar i=oriBuf[4*y*m_imageWidth+4*x+3];
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
        QString posStr1("r:%1,g:%2,b:%3");
        posColor =  posStr1.arg(r).arg(g).arg(b);
    }
        break;
    case 4:
    {
        QString posStr1("r:%1,g:%2,b:%3,i:%4");
        posColor =  posStr1.arg(r).arg(g).arg(b).arg(i);
    }
        break;
    default:
        break;
    }

    QString rect("(X:%1,Y:%2)");
    rect = rect.arg(m_StartOriPoint.x()).arg(m_StartOriPoint.y());
    ui->pos_press->setText(rect);
    float rg = (float)r*100.0/((float)r+(float)g);
    QString rgstr =  QString(" R/G(%1%) ").arg(QString::number(rg,'f',1));

    float rb = (float)r*100.0/((float)r+(float)b);
    QString rbstr = QString("R/B(%1%) ").arg(QString::number(rb,'f',1));

    float gb = (float)g*100.0/((float)g+(float)b);
    QString gbstr =  QString("G/B(%1%)").arg(QString::number(gb,'f',1));


    int h,s,v;
    myFlow.getHsv((int)r,(int)g,(int)b,h,s,v);
    QString showStr = posColor+QString(" H:%1,S:%2,V:%3").arg(h).arg(s).arg(v);
    ui->rgblabel->setText(showStr);
}

void SvmImageWidget::on_cleanSampleBtn_clicked()
{
    myMessageBox msgBox(MSG_QUES, g_myLan().cfm_clean_sample);
    int ret = msgBox.exec();
    if (ret == QDialog::Accepted)
    {
        g_Runtime().m_sampleMap[0].clear();
        g_Runtime().m_sampleMap[1].clear();
        memset(struGsh.nGoodSamp,0,sizeof(struGsh.nGoodSamp));
        memset(struGsh.nBadSamp,0,sizeof(struGsh.nBadSamp));
        struGsh.nGoodSampCount = 0;
        struGsh.nBadSampCount= 0;
        m_PointNumVec[0].clear();
        m_PointNumVec[1].clear();
        m_PointVec[0].clear();
        m_PointVec[1].clear();

        QList<int> keyList = m_MatMap.keys();//存放的就是QMap的key值
        for(int i=0;i<keyList.size();i++)
        {
            m_MatMap[keyList[i]] = NoUse;
        }
        m_image = m_oriImage;

        memcpy(chgBuf, oriBuf, m_imageWidth*m_imageHeight*4);
        showGrayColor();;
        QImage img(chgBuf, m_imageWidth, m_imageHeight, QImage::Format_ARGB32);
        m_tempimage = img;
//        m_tempimage = m_image;
    }
    updatePixColor();
    updateImageSize();
    updateBtnState();
}

void SvmImageWidget::learnFinished()
{
    g_infoWidget().hide();
    myMessageBox msg(MSG_INFO,g_myLan().ai_analysis);
    struGsh.ntrainComplete = (m_studyResult == tr_succeeded)?true:false;
    switch(m_studyResult){
    case tr_succeeded:
        msg.setText(g_myLan().ai_success);
        printf("svmcoefs:sense:%f r:%f g:%f b:%f rg;%f gb:%f rb:%f rr:%f gg:%f bb:%f\n",struGsh.m_svmcoefs[9],struGsh.m_svmcoefs[0],struGsh.m_svmcoefs[1]\
                        ,struGsh.m_svmcoefs[2],struGsh.m_svmcoefs[3],struGsh.m_svmcoefs[4],struGsh.m_svmcoefs[5],struGsh.m_svmcoefs[6],struGsh.m_svmcoefs[7],struGsh.m_svmcoefs[8]);
        break;
    case tr_unsupported_type:
        msg.setText(QString("AI type")+g_myLan().ai_unsupported);
        break;
    case tr_no_good_data:
        msg.setText(g_myLan().ai_err_no_good);
        break;
    case tr_no_bad_data:
        msg.setText(g_myLan().ai_err_no_bad);
        break;
    case tr_null_data_ptr:
        msg.setText(g_myLan().ai_err_no_set);
        break;
    case tr_failed_write_file:
        msg.setText(g_myLan().ai_err_fail_write);
        break;
    case tr_no_config_file:
        msg.setText(g_myLan().ai_err_no_config_file);
        break;
    case tr_unknown_error:
        msg.setText(g_myLan().ai_unknow_err);
        break;
    default:
        break;

    }
    updateSvmSimulateEditValue();
    //学习完自动应用前后视的当前智能算法
//    for(int view = struGsh.nView/2*2;view <= struGsh.nView/2*2+1;view++)
//        if(struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[view]){
//            for(int times = 0;times < struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].nIdentifyGroupTotal;times++){
//                printf("apply view%d time%d svm%d\n",view, times, struGsh.nArithIndex-ARITH_SVM_A);
//                myFlow.setSvmParas(0,view,times,struGsh.nArithIndex-ARITH_SVM_A);
//                myFlow.resetMaterial(struGsh.nLayer,view,times,false,struGsh.nArithIndex,struGsh.nArithIndex-ARITH_SVM_A);
//            }
//        }
//    }
    msg.exec();
}

svm_result SvmImageWidget::svmtrain(const int* dataGood, int countGood, const int* dataBad, int countBad, svm_type type, QString dirTool, double coefs[COEF_COUNT])
{   
	if (countGood == 0)
	{
		return (svm_result)tr_no_good_data;
	}
	if (countBad == 0)
	{
		return (svm_result)tr_no_bad_data;
	}
#ifdef Q_OS_UNIX
	QString dirTrainTool = dirTool + "/train";
	//    QString dirScaleTool = dirTool + "/svm-scale";
	//    QString dirPredictTool =  dirTool + "/predict";
#else
	QString dirTrainTool = dirTool + "/train.exe";
	//    QString dirScaleTool = dirTool + "/svm-scale.exe";
	//    QString dirPredictTool =  dirTool + "/predict.exe";
#endif
	if(!QFile::exists(dirTrainTool))
	{
		return (svm_result)tr_no_config_file;
	}

	QDir *temp = new QDir;
	bool exist = temp->exists(dirTool + "/tempdata/");
	if(!exist)
	{
		temp->mkdir(dirTool + "/tempdata/");
	}

	QString traindatapath = dirTool + "/tempdata/traindata.txt";
	QFile file(traindatapath);
	if(!file.open(QIODevice::WriteOnly))
	{
		return (svm_result)tr_failed_write_file;
	}
	int r = 0;
    int g = 0;
    int b = 0;
    int n = 0;

	QString lineGood;
	QString lineBad;
	QTextStream instream(&file);
	for(int i = 0; i < countGood; i++)
	{
        r = dataGood[4*i];
        g = dataGood[4*i + 1];
        b = dataGood[4*i + 2];
        n = dataGood[4*i + 3];

		if(type == 1){
			lineGood = QString("1 1:%1 2:%2 3:%3 4:%4 5:%5 6:%6 7:%7 8:%8 9:%9").arg(r).arg(g).arg(b).arg(r*g).arg(g*b).arg(r*b).arg(r*r).arg(g*g).arg(b*b);
		}
		else if(type == 2){
            lineGood = QString("1 1:%1 2:%2 3:%3 4:%4").arg(r).arg(g).arg(b).arg(n);
		}
		else{
			lineGood = QString("1 1:%1 2:%2 3:%3 4:%4 5:%5 6:%6").arg(r).arg(g).arg(b).arg(r*g).arg(g*b).arg(r*b);
		}
		instream << lineGood <<endl;
	}
	for(int i = 0; i < countBad; i++)
    {
        r = dataBad[4*i];
        g = dataBad[4*i + 1];
        b = dataBad[4*i + 2];
        n = dataBad[4*i + 3];

		if(type == 1){
			lineBad = QString("-1 1:%1 2:%2 3:%3 4:%4 5:%5 6:%6 7:%7 8:%8 9:%9").arg(r).arg(g).arg(b).arg(r*g).arg(g*b).arg(r*b).arg(r*r).arg(g*g).arg(b*b);
		}
		else if(type == 2){
            lineBad = QString("-1 1:%1 2:%2 3:%3 4:%4").arg(r).arg(g).arg(b).arg(n);
		}
		else{
			lineBad = QString("-1 1:%1 2:%2 3:%3 4:%4 5:%5 6:%6").arg(r).arg(g).arg(b).arg(r*g).arg(g*b).arg(r*b);
		}
		instream << lineBad <<endl;
	}
	file.close();
	QString modelpath = dirTool + "/tempdata/model.txt";
	QString cmd = dirTrainTool + " -s 2 -B 1 -e 0.0001 -p 0.0001 " + traindatapath + " " + modelpath;
	QProcess *tProcess = new QProcess();
	tProcess->start(cmd);
	tProcess->waitForStarted(-1);
	tProcess->waitForFinished(-1);
	tProcess->close();

	QFile modelfile(modelpath);
	modelfile.open(QIODevice::ReadOnly);
	int row = 1;
	memset(coefs,0,sizeof(double)*COEF_COUNT);
	while(!modelfile.atEnd())
	{
		QByteArray line = modelfile.readLine();
		QString str(line);

		if (row >= 7)
		{
			coefs[row-7]=str.toDouble();
		}   

		row = row + 1;
	}   

	if (type==0){
		coefs[9]=coefs[6];
		coefs[6]=0;
	}   
	if(type==2){
        coefs[9]=coefs[4];
	}

	modelfile.close();

	//删除临时文件
#ifdef Q_OS_UNIX
    system("rm /sdcard/bin/tempdata/*.txt");
    system("sync");
#endif
	return (svm_result)tr_succeeded;
}

SvmLearnThread::SvmLearnThread(SvmImageWidget *parent)
{
   m_pParent = parent;
}

void SvmLearnThread::run()
{
    struGsh.bUpdateState = 1;
#ifdef Q_OS_UNIX
      if(!g_Runtime().dirExist("/sdcard/bin")){
         system("cp -fr /app/svm* /sdcard/bin/");
         system("cp -fr /app/train /sdcard/bin/");
         system("sync");
      }    
      else{
         if (!QFile::exists("/sdcard/bin/train"))
         {
            system("cp -fr /app/train /sdcard/bin/");
            system("sync");
         }
      }    
  #endif
      printf("svmNewTrain:%d,svmType:%d\n",struGsh.m_isNewTrain,struCnfg.n_svmType);
#ifdef Q_OS_UNIX
         if(struGsh.m_isNewTrain){
//            m_pParent->m_studyResult = m_pParent->svmtrain(struGsh.nGoodSamp,struGsh.nGoodSampCount,struGsh.nBadSamp,struGsh.nBadSampCount,(svm_type)struCnfg.n_svmType,"/sdcard/bin",struGsh.m_svmcoefs);
         }else{
//             m_pParent->m_studyResult = train(struGsh.nGoodSamp,struGsh.nGoodSampCount,struGsh.nBadSamp,struGsh.nBadSampCount,(svm_type)struCnfg.n_svmType,"/sdcard/bin",struGsh.m_svmcoefs);
         }
#else
         if (struGsh.m_isNewTrain){
            m_pParent->m_studyResult = m_pParent->svmtrain(struGsh.nGoodSamp,struGsh.nGoodSampCount,struGsh.nBadSamp,struGsh.nBadSampCount,(svm_type)struCnfg.n_svmType,qApp->applicationDirPath(),struGsh.m_svmcoefs);
         }
         else{
            m_pParent->m_studyResult = train(struGsh.nGoodSamp,struGsh.nGoodSampCount,struGsh.nBadSamp,struGsh.nBadSampCount,(svm_type)struCnfg.n_svmType,qApp->applicationDirPath().toUtf8().data(),struGsh.m_svmcoefs);
         }
#endif
    struGsh.bUpdateState = 0;
}

void SvmImageWidget::on_checkBox_No_Bg_clicked(bool checked)
{
    m_bNoBackground = checked;
    if(checked){
        m_image = m_tempimage;
        uchar value = 0;
        for(int i=0; i<m_imageHeight; i++) {
            for(int j=0; j<m_imageWidth; j++) {
                value = bgPoint[i*m_imageWidth+j];
                if(value == PIXEL_BACKGROUND){
                    m_tempimage.setPixel(j,i,Qt::white);
                }
            }//end j
        }//end i
    }else{
        m_tempimage = m_image;
    }
    updateImageSize();
    updateZoomValueText();
    updateSamplesBtnState();
}
void SvmImageWidget::updateCurrentImage(int row,bool show)
{
    ui->listWidget->setCurrentRow(row);
    int layer =0,view= 0,unit = 0;
    g_Runtime().getCurImageLayerAndView(layer,view,unit,row);
    QString imagePath = g_Runtime().nimageNameVec[row];
    if (imagePath.contains("bmp")){
        myFlow.getImageShowName(imagePath);
        setImage(imagePath,layer,view,unit);
    }
}

void SvmImageWidget::updateShowPage()
{
    ui->importSampleBtn->setVisible(struGsh.bRunMode >= RUNMODE_FACTORY);
    ui->svmSimulte->setVisible(struGsh.bRunMode >= RUNMODE_FACTORY);
    ui->label_mark->setVisible(struGsh.bRunMode >= RUNMODE_FACTORY);
    ui->comboBoxMarkIndex->setVisible(struGsh.bRunMode >= RUNMODE_FACTORY);
    ui->comboBoxSvmType->setVisible(struGsh.bRunMode >= RUNMODE_FACTORY);

    m_ZoomValue = 1;
    if(m_imageWidth*m_ZoomValue < ImageLabelWidth)
        m_CenterX = m_imageWidth/2;
    else
        m_CenterX = ImageLabelWidth/2;
    m_CenterY = ImageLabelHeight/2;
	m_StartOriPoint.setX(m_CenterX);
    m_StartOriPoint.setY(m_CenterY);
    m_bMoveImage = true;
    ui->checkBox->setChecked(m_bMoveImage);
    ui->checkBox_No_Bg->setChecked(m_bNoBackground);
    if(!g_Runtime().nimageNameVec.isEmpty()){
        updatePixColor();
        updateImageSize();
    }
    updateZoomValueText();
    updateSamplesBtnState();
    updateBtnState();
    updateSvmSimulateEditValue();
}
void SvmImageWidget::imageListItemSelect(QModelIndex index)
{
    if(!m_PointNumVec[0].isEmpty() ||!m_PointNumVec[1].isEmpty()){
        ui->listWidget->setCurrentRow(m_imageIndex);
        update();
        g_infoWidget().setLabelText(g_myLan().Marked_material_not_saved);
        g_infoWidget().delayShow();

        myFlow.sleep(2);

        g_infoWidget().hide();
        return;
    }
    m_imageIndex = index.row();
    updateCurrentImage(index.row());

    updateShowPage();
}

void SvmImageWidget::updateImageList()
{
    int i =0;
    if(!g_Runtime().nimageNameVec.isEmpty()){
        while (i < g_Runtime().nimageNameVec.count()) {
            bool bIsImg = true;
            QImage  image;
#ifdef Q_OS_UNIX
            QString imagePath = g_Runtime().nimageNameVec[i]+"_copy";
#else
            QString imagePath = g_Runtime().nimageNameVec[i];
#endif
            bIsImg = image.load(imagePath);
            if(!bIsImg){
                bIsImg = image.load(g_Runtime().nimageNameVec[i],"bmp");
            }
            QString name = myFlow.getImageShowName(imagePath);
            if (bIsImg) {
                image = image.scaled(146,65);
                QListWidgetItem *item = new QListWidgetItem(QPixmap::fromImage(image),name,ui->listWidget,i);
                item->setTextAlignment(Qt::AlignHCenter);
            }
            i++;
        }
    }
}

void SvmImageWidget::on_svmLearnBtn_clicked()
{
    m_studyResult = tr_unknown_error;
    memset(struGsh.m_svmcoefs,0,sizeof(struGsh.m_svmcoefs));
    memset(struGsh.m_Shapecoef,0,sizeof(struGsh.m_Shapecoef));
    g_infoWidget().setLabelText(g_myLan().ai_calculate);
    g_infoWidget().delayShow();
    m_pThread->start();
}

void SvmImageWidget::on_applyBtn_clicked()
{
    SetSvmUseDialog dlg;
    if(dlg.exec() == QDialog::Accepted){
        g_infoWidget().setLabelText(g_myLan().ids_succeeded);
        g_infoWidget().delayShow();

        myFlow.sleep(2);

        g_infoWidget().hide();
    }
}

QString SvmImageWidget::getSamplesFilePath(SamplesType mode, int view,int svmIndex,bool all)
{
    QString strSamplePath;
    QString oriFileName = QString(struCnfg.stuProfileIndex[struCnfg.nProfile].sProfileName);
    QString sampleDirPath =QString(CNF_MODE_DIR)+QString("svm/");
    if(all)
        sampleDirPath+=QString("all/");
    else{
        sampleDirPath+=oriFileName;

        if(svmIndex == 0){
            sampleDirPath+=QString("/%1layer/svmA/").arg(view);
        }else{
            sampleDirPath+=QString("/%1layer/svmB/").arg(view);
        }
    }
    QDir dir(sampleDirPath);
    if(!dir.exists())
        dir.mkpath(dir.absolutePath());

    strSamplePath = sampleDirPath+QString("zksort");
    if(mode == Good)
    {
        strSamplePath = strSamplePath + QString("_good.txt");
    }
    else if(mode == Bad)
    {
        strSamplePath = strSamplePath + QString("_bad.txt");
    }
    return strSamplePath;
}

int SvmImageWidget::readSamplesNumsByType(SamplesType mode,int layer,int svmIndex)
{
    QString strSampleFile("");
    QByteArray ba;
    strSampleFile =getSamplesFilePath(mode,layer,svmIndex,false);

    ba = strSampleFile.toUtf8();
    const char* pFile = ba.data();
    return readSamplesNums(mode,pFile);
}

int SvmImageWidget::readSamplesNums(SamplesType mode,const char *file)
{
    QString lineStr;
    QStringList lineList;
    FILE *fp = fopen(file, "r");
    if(fp == NULL)
    {
        return 0;
    }

    int cnt = 0, ind = 0;
    char line[20];

    while(fgets(line, 20, fp))
    {
        cnt ++;
        if( strlen( line ) == 1 )	// line中只有一个\n
        {
            fclose( fp );
            fp = NULL;
            return 0;
        }

        lineStr = QString::fromLocal8Bit(line,20);
        lineList = lineStr.split(" ");

        if(lineList.size() > 3){
            SampleObj obj = {lineList.at(0).toInt(0,10),lineList.at(1).toInt(0,10),lineList.at(2).toInt(0,10),lineList.at(3).toInt(0,10)};
            g_Runtime().m_sampleMap[mode].insert(obj,mode);
            if(mode == Bad){
                struGsh.nBadSamp[struGsh.nBadSampCount*4] = obj.r;
                struGsh.nBadSamp[struGsh.nBadSampCount*4+1] = obj.g;
                struGsh.nBadSamp[struGsh.nBadSampCount*4+2] = obj.b;
                struGsh.nBadSamp[struGsh.nBadSampCount*4+3] = obj.n;
                struGsh.nBadSampCount++;
            }else{
                struGsh.nGoodSamp[struGsh.nGoodSampCount*4] = obj.r;
                struGsh.nGoodSamp[struGsh.nGoodSampCount*4+1] = obj.g;
                struGsh.nGoodSamp[struGsh.nGoodSampCount*4+2] = obj.b;
                struGsh.nGoodSamp[struGsh.nGoodSampCount*4+3] = obj.n;
                struGsh.nGoodSampCount++;
            }
        }else{
            fclose( fp );
            fp = NULL;
            return 0;
        }


        ind++;
    }

    fclose( fp );
    fp = NULL;
    return ind;
}

void SvmImageWidget::on_importSampleBtn_clicked()
{
    g_MainManager().ShowWidget(SM_SVM_SAMPLE_WIDGET);
}

void SvmImageWidget::simulateFinished()
{
    g_infoWidget().delayHide();
//    showGrayColor();
    QImage img = QImage(chgBuf,m_imageWidth,m_imageHeight,QImage::Format_ARGB32);
    m_tempimage = img;
    updateImageSize();
    updateZoomValueText();
}

void SvmImageWidget::colorSimulate(uchar *pStatus)
{
    m_matPixCount = 0;
    m_badPixCount = 0;
    bool bIsBad;

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
    /* 灵敏度越大，常数项越小，选的越多 */
    if(ncon < 0) {
        nSvmCons = ncon*(4.0/const_max_svm*struGsh.nSvmSimulateSens-1);
    }else{
        nSvmCons = ncon*(3-4.0/const_max_svm*struGsh.nSvmSimulateSens);
    }

    uchar r,g,b,n;
    for(int i=0; i<m_imageHeight; i++){
        for(int j=0; j<m_imageWidth; j++){
            bIsBad = false;
            b = oriBuf[(i*m_imageWidth+j)*4];
            g = oriBuf[(i*m_imageWidth+j)*4+1];
            r = oriBuf[(i*m_imageWidth+j)*4+2];
            n = oriBuf[(i*m_imageWidth+j)*4+3];
            if(bgPoint[i*m_imageWidth+j] == PIXEL_BACKGROUND)
                continue;
            //智能模式３以后就给多维智能用了
            if(struCnfg.n_svmType == 2){
                if((nr*r+ng*g+nb*b+nrg*n+nSvmCons) < 0){
                    bIsBad =  true;
                }else{
                    bIsBad = false;
                }
            }
            else{
                if((nr*r+ng*g+nb*b+nrg*r*g+ngb*g*b+nrb*r*b+nrr*r*r+ngg*g*g+nbb*b*b+nSvmCons) < 0){
                    bIsBad =  true;
                }else{
                    bIsBad = false;
                }
            }
            if(bIsBad){
                pStatus[i*m_imageWidth+j] = PIXEL_MATTER_BAD;
            }
        }
    }
}

void SvmImageWidget::changeImgBySimulate(uchar *pBadAll,uchar *pPixBadOut)
{
    bool bNeedChg;
    int nBadPixelNum;
	int row = 0,col = 0,per = 0;
	row = struGsh.nSvmRow;
	per = struGsh.nSvmPercent;
	col = row;//列暂时没有使用，均使用row
	/* 根据设置的行列纯度进行仿真 */
	for(int i=0; i<m_imageHeight; i++) {
		for(int j=0; j<m_imageWidth; j++) {
			if(bgPoint[i*m_imageWidth+j] != PIXEL_BACKGROUND) {
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
						if(pBadAll[m*m_imageWidth+n] == PIXEL_MATTER_BAD)
							nBadPixelNum++;
					}//end n
				}//end m

				if(nBadPixelNum >= per){
                    pPixBadOut[i*m_imageWidth+j] = 2;
				}
			}
		}//end j
	}//end i
}

SvmSimulateThread::SvmSimulateThread(SvmImageWidget *parent)
{
	m_pParent = parent;
}

void SvmSimulateThread::run()
{
    int m_imageWidth = m_pParent->m_imageWidth;
    int m_imageHeight = m_pParent->m_imageHeight;

	uchar *pPixelIsBad = (uchar *)malloc(m_imageHeight*m_imageWidth);
	memset(pPixelIsBad, 0, m_imageWidth*m_imageHeight);

    m_pParent->showGrayColor();
//    memcpy(chgBuf, oriBuf, m_imageWidth*m_imageHeight*4);
	uchar *pPixelIsBadall= (uchar *)malloc(m_imageHeight*m_imageWidth);
	memset(pPixelIsBadall, 0, m_imageWidth*m_imageHeight);
    m_pParent->colorSimulate(pPixelIsBadall);
    m_pParent->changeImgBySimulate(pPixelIsBadall,pPixelIsBad);//使用尺寸算法
	free(pPixelIsBadall);
	pPixelIsBadall = NULL;

	uchar value = 0;
	for(int i=0; i<m_imageHeight; i++) {
		for(int j=0; j<m_imageWidth; j++) {
			value = pPixelIsBad[i*m_imageWidth+j];
            if(value == 2){
                chgBuf[(i*m_imageWidth+j)*4] = 0;
                chgBuf[(i*m_imageWidth+j)*4+1] = 0;
                chgBuf[(i*m_imageWidth+j)*4+2] = 255;
            }
		}//end j
	}//end i

	free(pPixelIsBad);
	pPixelIsBad = NULL;
}

void SvmImageWidget::on_simulateCheckBox_clicked(bool checked)
{
    m_bSimulate = checked;

    if(m_bSimulate){
        displaySimulate();
    }
    showGrayColor();
    m_rgbnImage = QImage(chgBuf,m_imageWidth,m_imageHeight,QImage::Format_ARGB32);
    m_tempimage = m_rgbnImage;
    updateImageSize();
}

void SvmImageWidget::on_returnBtn_clicked()
{
   returnBack();
}

void SvmImageWidget::on_comboBoxMarkIndex_activated(int index)
{
   m_markIndex = index;
}

void SvmImageWidget::on_comboBoxSvmType_activated(int index)
{
   struCnfg.n_svmType = index;
}

void SvmImageWidget::on_checkBox_svmNew_clicked(bool checked)
{
   struGsh.m_isNewTrain = (int)checked;
}

void SvmImageWidget::on_comboBoxImageType_activated(int index)
{
    m_colorType = (colorType)index;

    if(m_bSimulate){
        displaySimulate();
    }
    showGrayColor();
    m_rgbnImage = QImage(chgBuf,m_imageWidth,m_imageHeight,QImage::Format_ARGB32);
    m_tempimage = m_rgbnImage;
    updateImageSize();
}
