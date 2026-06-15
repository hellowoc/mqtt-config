#include "mywavewidget.h"
#include <QPen>
#include <QTime>
#include <QPainter>
#include "globalparams.h"
#include "globalflow.h"

#if(LCD_WIDTH == 1024)
const int detailLength = 250;
#else
const int detailLength = 188;
#endif
enum {
    WAVE_RED = 0,
    WAVE_GREEN,
    WAVE_BLUE,
    WAVE_INF,
    WAVE_IPC_EJECT,
    WAVE_HUE,
    WAVE_VAVUE
};
MyWaveWidget::MyWaveWidget(QWidget *parent) : QWidget(parent)
{
    //    config = g_Config::getInstance();
    //    m_nDrawType = TYPE_WAVE;
    m_bDrawBoarder = false;
    m_bDrawMultiWave = false;

    setMinValueX(0);
    setMinValueY(0);
    setMaxValueY(255);

    switch (struCnfp.nSensorPixel[struGsh.nLayer][struGsh.nView]) {
    case SENSOR_PIXELS_256:
        setMaxValueX(256);
        break;
    case SENSOR_PIXELS_512:
        setMaxValueX(512);
        break;
    case SENSOR_PIXELS_1024:
    case SENSOR_PIXELS_1024_TD:
        setMaxValueX(1024);
        break;
    case SENSOR_PIXELS_2048:
    case SENSOR_PIXELS_2048_GTD:
        setMaxValueX(2048);
        break;
    case SENSOR_PIXELS_2048_TD:
    case SENSOR_PIXELS_2048_XTD:
        if(struGsh.nCamTdMode == 2)
            setMaxValueX(1024);
        else
            setMaxValueX(2048);
        break;
    default:
        setMaxValueX(2048);
        break;
    }

    setTickAttr();
    font.setPixelSize(16);

    if(LCD_WIDTH == 1024)
        setBorder(50,30,11,50); // 设置坐标轴的左右上下边界
    else
        setBorder(40,20,5,25);  // 设置坐标轴的左右上下边界
}

MyWaveWidget::~MyWaveWidget()
{
}

/* 根据分辨率来设置X坐标 */
void MyWaveWidget::setXByData()
{
    int maxValue;

    switch (struCnfp.nSensorPixel[struGsh.nLayer][struGsh.nView]) {
    case SENSOR_PIXELS_256:
        maxValue = 256;
        break;
    case SENSOR_PIXELS_512:
        maxValue = 512;
        break;
    case SENSOR_PIXELS_1024:
    case SENSOR_PIXELS_1024_TD:
        maxValue = 1024;
        break;
    case SENSOR_PIXELS_2048:
    case SENSOR_PIXELS_2048_GTD:
        maxValue = 2048;
        break;
    case SENSOR_PIXELS_2048_TD:
    case SENSOR_PIXELS_2048_XTD:
        if(struGsh.nCamTdMode == 2)
            maxValue = 1024;
        else
            maxValue = 2048;
        break;
    default:
        setMaxValueX(2048);
        break;
    }

    if(struGsh.nPageSm == SM_IPC_EJECT_WIDGET ||(m_nDrawType == IPC_EJECT) )
    {
        maxValue = struGsh.sRowTotal[0];
        for(int i=1;i<1000;i++){
            if( maxValue < struGsh.sRowTotal[i])
                maxValue = struGsh.sRowTotal[i];
        }
        setMaxValueX(1024);
        setMaxValueY(maxValue);
        setTickAttr();
        return;
    }
    setMaxValueX(maxValue);     // 设置x轴上的最大值
    setTickAttr();
}

/* 设置x轴上的最小值 */
void MyWaveWidget::setMinValueX(int value)
{
    minValueX = value;
}

/* 设置x轴上的最大值 */
void MyWaveWidget::setMaxValueX(int value)
{
    maxValueX = value;
}

/* 设置y轴上的最小值 */
void MyWaveWidget::setMinValueY(int value)
{
    minValueY = value;
}

/* 设置y轴上的最大值 */
void MyWaveWidget::setMaxValueY(int value)
{
    maxValueY = value;
    if(struGsh.nPageSm == SM_IPC_EJECT_WIDGET ||(m_nDrawType == IPC_EJECT) )
    {
        if(value <= 45) maxValueY = 50;
        else if(value < 90) maxValueY = 100;
        else if(value < 180) maxValueY = 200;
        else if(value < 360) maxValueY = 400;
        else if(value < 900) maxValueY = 1000;
        else if(value < 4500) maxValueY = 5000;
        else if(value < 18000) maxValueY = 20000;
        else maxValueY = 70000;
    }
}

/* 设置坐标轴的边界 */
void MyWaveWidget::setBorder(int left, int right, int up, int down)
{
    leftBorder = left;
    rightBorder = right;
    upBorder = up;
    downBorder = down;
}

/* 获取左边界 */
int MyWaveWidget::getLeftBorder()
{
    return leftBorder;
}

/* 获取右边界 */
int MyWaveWidget::getRightBorder()
{
    return rightBorder;
}

/* 获取上边界 */
int MyWaveWidget::getUpBorder()
{
    return upBorder;
}

/* 获取下边界 */
int MyWaveWidget::getDownBorder()
{
    return downBorder;
}

/* 设置标记属性 */
void MyWaveWidget::setTickAttr()
{
    setTickValue();
    setTickNum();
}

/* 设置标记的数目 */
void MyWaveWidget::setTickNum()
{
    majorXNum = (maxValueX-minValueX)/majorTickValueX;  // 长刻度数量
    minorXNum = (maxValueX-minValueX)/minorTickValueX;  // 短刻度数量
    majorYNum = (maxValueY-minValueY)/majorTickValueY;
    minorYNum = (maxValueY-minValueY)/minorTickValueY;
}

/* 计算记号上的值 */
void MyWaveWidget::setTickValue()
{
    switch(maxValueX-minValueX) {
    case 60:
        majorTickValueX = 5;   // 长刻度单位
        minorTickValueX = 1;   // 短刻度单位
        break;
    case 512:
        majorTickValueX = 64;   // 长刻度单位
        minorTickValueX = 16;   // 短刻度单位
        break;
    case 1024:
        majorTickValueX = 128;  // 长刻度单位
        minorTickValueX = 32;   // 短刻度单位
        break;
    case 2048:
        majorTickValueX = 256;  // 长刻度单位
        minorTickValueX = 64;   // 短刻度单位
        break;
    default:
        majorTickValueX = 50;   // 长刻度单位
        minorTickValueX = 10;   // 短刻度单位
        break;
    }

    switch(maxValueY) {
    case 50:
        majorTickValueY = 10;
        minorTickValueY = 2;
        break;
    case 100:
        majorTickValueY = 20;
        minorTickValueY = 4;
        break;
    case 200:
        majorTickValueY = 40;
        minorTickValueY = 8;
        break;
    case 255:
        majorTickValueY = 50;
        minorTickValueY = 10;
        break;
    case 400:
        majorTickValueY = 80;
        minorTickValueY = 16;
        break;
    case 1000:
        majorTickValueY = 200;
        minorTickValueY = 40;
        break;
    case 5000:
        majorTickValueY = 1000;
        minorTickValueY = 200;
        break;
    case 20000:
        majorTickValueY = 4000;
        minorTickValueY = 800;
        break;
    case 70000:
        majorTickValueY = 14000;
        minorTickValueY = 2800;
        break;

    default:
        majorTickValueY = 50;
        minorTickValueY = 10;
        break;
    }
}

/* 从串口处更新波形信号数据 */
void MyWaveWidget::getNewdata()
{
    int i;

    //    for(i = 0; i < struCnfc.nSensorPixelLevel[struGsh.nLevel]; i++){
    //        struGsh.sRowRed[i]      = struGsh.sRgbRow[10+i*3];
    //        struGsh.sRowGreen[i]    = struGsh.sRgbRow[10+i*3+1];
    //        struGsh.sRowBlue[i]     = struGsh.sRgbRow[10+i*3+2];
    //    }

    //    if(struCnfc.nSensorPixelLevel[struGsh.nLevel] == SENSOR_PIXELS_1024) {
    //        /* 1024分辨率时，附加红外相机数据 */
    //        memcpy(struGsh.sRowInf, struGsh.sRgbRow+10+struCnfc.nSensorPixelLevel[struGsh.nLevel]*3, 512);
    //    }
}

/* 设置绘制的类型 */
void MyWaveWidget::setDrawType(WavePage type, bool showSrc)
{
    m_nDrawType = type;
    sourceShow = showSrc;
}

/* 绘制吹气次数 */
void MyWaveWidget::drawEjectTimes(QPainter &painter)
{
    //    drawSingleTimes(painter, FRONT_VIEW);
    //    drawSingleTimes(painter, REAR_VIEW);
    //    struGsh.mutex->lock();
    //    memset(struGsh.pEjectTimes, 0, MAX_EJECTOR*2*sizeof(int));
    //    struGsh.mutex->unlock();
}

/* 绘制单视的吹气次数波形 */
void MyWaveWidget::drawSingleTimes(QPainter &painter, int flag)
{
    int *pData = NULL;
    QPen pen;
    pen.setWidthF(2);
    //    switch(flag){
    //    case FRONT_VIEW:
    //        pData = struGsh.pEjectTimes;
    //        pen.setColor(Qt::red);
    //        break;
    //    case REAR_VIEW:
    //        pData = struGsh.pEjectTimes+MAX_EJECTOR;
    //        pen.setColor(Qt::blue);
    //        break;
    //    default:
    //        break;
    //    }

    //    bool isFirstpoint = true;
    //    QRect rect = this->rect();
    //    rect.adjust(leftBorder,upBorder,-rightBorder,-downBorder);
    //    painter.setClipRect(rect);
    //    painter.setPen(pen);

    //    QPointF currentPoint, lastPoint;
    //    int tmp = maxValueX-minValueX+1;
    //    for(int i=1; i<tmp; i++)
    //    {
    //        if(isFirstpoint){
    //            lastPoint.setX(leftBorder+i/valueXPerPixel);
    //            lastPoint.setY(rect.height()+upBorder-pData[i-1]/valueYPerPixel);
    //            isFirstpoint = false;
    //        }
    //        else
    //        {
    //            currentPoint.setX(leftBorder+i/valueXPerPixel);
    //            currentPoint.setY(rect.height()+upBorder-pData[i-1]/valueYPerPixel);
    //            painter.drawLine(lastPoint,currentPoint);
    //            lastPoint = currentPoint;
    //        }
    //    }
}

/* 设置边界是否显示 */
void MyWaveWidget::setBorderShow(bool bFlag)
{
    m_bDrawBoarder = bFlag;
}

/* 获取边界是否显示 */
bool MyWaveWidget::getBorderShow()
{
    return m_bDrawBoarder;
}

/* 设置是否绘制重叠波形标志位 */
void MyWaveWidget::setMultiWaveFlag(bool bFlag)
{
    m_bDrawMultiWave = bFlag;
    m_pixmap = QPixmap();
}

/* 设置需要绘制的图像 */
void MyWaveWidget::setPixmap(QPixmap pixmap)
{
    m_pixmap = pixmap;
}

void MyWaveWidget::drawDividePix(QPainter &painter)
{
    int curPixel = myFlow.getSensorPixels(struGsh.nLayer,struGsh.nView);
    //    int x1=leftBorder+struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nChannelBegin[struGsh.nUnit]/valueXPerPixel;
    int x1=struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nChannelBegin[struGsh.nUnit];
    x1 = getModifiedChannalXPixel(x1);
    int y1= rect().height();

    int x2 = x1;
    int y2 = 0;

    painter.setPen(QColor(0,0,0));
    painter.drawLine(x1,y1,x2,y2);

    //    x1 = leftBorder+struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nChannelEnd[struGsh.nUnit]/valueXPerPixel;
    x1 = struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nChannelEnd[struGsh.nUnit];
    x1 = getModifiedChannalXPixel(x1);
    x2 = x1;
    painter.setPen(QColor(0,0,0));
    painter.drawLine(x1,y1,x2,y2);


    for(int k = 0; k <struCnfg.struLayerInfo[0].stuViewInfo[struGsh.nView].nIdentifyGroupTotal; k++)
    {
       // if(struCnfg.struLayerInfo[0].stuViewInfo[struGsh.nView].stuIdenGupInfo[k].nUnitGupSplice[struGsh.nUnit])
        {
            x1=struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nChannelBegin2[struGsh.nUnit];
            x1 = getModifiedChannalXPixel(x1);
            x2 = x1;
            painter.setPen(QColor(0,0,0));
            painter.drawLine(x1,y1,x2,y2);

            x1 = struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nChannelEnd2[struGsh.nUnit];
            x1 = getModifiedChannalXPixel(x1);
            x2 = x1;
            painter.setPen(QColor(0,0,0));
            painter.drawLine(x1,y1,x2,y2);
        }

    }
    // 画三段分界线
    if (m_nDrawType == Divide_PIXEL && !sourceShow )
    {
        if (curPixel < 1024)
            return;
        QPen dotline(Qt::black,1,Qt::DotLine);
        painter.setPen(dotline);

        x1 = leftBorder+detailLength;
        x2 = x1;
        painter.drawLine(x1,y1,x2,y2);

        x1 = leftBorder+detailLength*2;
        x2 = x1;
        painter.drawLine(x1,y1,x2,y2);

        x1 = leftBorder+detailLength*3;
        x2 = x1;
        painter.drawLine(x1,y1,x2,y2);

        QPen dotline2(Qt::red,1,Qt::DotLine);
        painter.setPen(dotline2);
        x1 = leftBorder+detailLength*3/2.0;
        x2 = x1;
        painter.drawLine(x1,y1,x2,y2);
    }
}

void MyWaveWidget::drawIpcEject(QPainter &painter)
{
    QRect rect = this->rect();
    rect.adjust(leftBorder,upBorder,-rightBorder,-downBorder);
    int x1=struCnfg.nIpcStartPixel[struGsh.nView][struGsh.nUnit]/valueXPerPixel+leftBorder;
    int y1= rect.height()+upBorder;
    //    int y1= 1000/valueYPerPixel+upBorder;
    int x2 = x1;
    int y2 = upBorder;

    painter.setPen(QColor(0,0,0));
    painter.drawLine(x1,y1,x2,y2);

    x1=struCnfg.nIpcEndPixel[struGsh.nView][struGsh.nUnit]/valueXPerPixel+leftBorder;
    x2 = x1;
    painter.setPen(QColor(0,0,0));
    painter.drawLine(x1,y1,x2,y2);
}


/* 覆写绘制事件　*/
void MyWaveWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setFont(font);
    if(PRODUCT_DEFAULT_VENDOR == PRODUCT_VENDOR_LV){
        painter.setBrush(QColor(0x67,0x7F,0x99));
        painter.drawRoundedRect(this->rect(),10,10);
    }

    m_drawSignalType++;
    if(m_drawSignalType%3 == 1){
        QPen pen;
        if(!struCnfp.bColorRestoreEnable){
            pen.setWidth(6);
            pen.setColor(Qt::red);
            painter.setPen(pen);
            painter.setBrush(QColor(Qt::red));
            painter.drawEllipse(width()-8,5,5,5);
        }
        else{
            pen.setWidth(6);
            pen.setColor(Qt::yellow);
            painter.setPen(pen);
            painter.setBrush(QColor(Qt::yellow));
            painter.drawEllipse(width()-8,5,5,5);
        }
    }else{
        QPen pen;
        pen.setWidth(6);
        pen.setColor(Qt::white);
        painter.setPen(pen);
        painter.setBrush(QColor(Qt::white));
        painter.drawEllipse(width()-8,5,5,5);
    }
    /* 若绘制重叠波形则无需重绘坐标轴 */
    paintAxis(painter);
    if(m_bDrawMultiWave) {
        painter.drawPixmap(leftBorder, upBorder, width()-leftBorder-rightBorder,
                           height()-upBorder-downBorder, m_pixmap);
    }

    drawWanted(painter);
}

/* 绘制整体的坐标轴 */
void MyWaveWidget::paintAxis(QPainter &painter)
{
    if(width()<leftBorder+rightBorder || height()<upBorder+downBorder)
        return;

    /* 绘制波形时需更新x轴显示,绘制吹气次数时需更新y轴显示 */
    //    if(m_nDrawType == TYPE_WAVE) {
    setXByData();
    //    } else {
    //        setYByData();
    //    }

    if (m_nDrawType == Divide_PIXEL && !sourceShow ){
        if (myFlow.getSensorPixels(struGsh.nLayer,struGsh.nView)<1024)
            paintAxisX(painter);
        else
            paitDivideAxisX(painter);
    }else{
        paintAxisX(painter);
    }
    paintAxisY(painter);
}

/* 绘制坐标轴x */
void MyWaveWidget::paintAxisX(QPainter &painter)
{
    qreal majorTickX, minorTickX;
    QPen pen_axis(Qt::black, 2);
    QPen pen_gridline(Qt::black, 1, Qt::DotLine);
    if(PRODUCT_DEFAULT_VENDOR == PRODUCT_VENDOR_LV){
        pen_gridline.setColor(QColor(255,255,255));
    }
    QPen pen_majortick(Qt::black, 2);
    QPen pen_minortick(Qt::black, 1);
    QPen pen_border(Qt::yellow, 1);

    valueXPerPixel = ((qreal)(maxValueX-minValueX)/(qreal)(width()-leftBorder-rightBorder));
    QPointF leftDownPoint(leftBorder, height()-downBorder);
    QPointF rightDownPoint(width()-rightBorder, height()-downBorder);
    /* 记录下tick点的坐标 */
    QPointF tmp(leftDownPoint);
    int pixelSize = painter.fontInfo().pixelSize();
    qreal halfTextLength = ((((qreal)width()-leftBorder-rightBorder))/(majorXNum-1))/3.0;
    QRect textRect;

    painter.setPen(pen_axis);
    painter.drawLine(leftDownPoint,rightDownPoint);

    /* 1.绘制主标记及纵轴虚线 */
    for(int i=1; i<=majorXNum; i++)
    {
        painter.setPen(pen_majortick);
        majorTickX = (qreal)(i*majorTickValueX/valueXPerPixel);
        tmp.setX(leftBorder+majorTickX);
        /* 绘制tick */
        painter.drawLine(tmp,tmp+QPointF(0,-8));
        textRect.setTopLeft(QPoint(tmp.x()-halfTextLength,tmp.y()+10));
        textRect.setSize(QSize(2*halfTextLength,pixelSize));
        /* 绘制X轴tick上的值 */
        painter.drawText(textRect, QString("%1")
                         .arg(minValueX+i*majorTickValueX),QTextOption(Qt::AlignHCenter));
        painter.setPen(pen_gridline);
        /* 绘制坐标轴中的网格 */
        painter.drawLine(tmp,QPointF(tmp.x(),upBorder));
    }
    tmp.setX((qreal)(leftBorder+(maxValueX-minValueX)/valueXPerPixel));
    painter.drawLine(tmp,QPointF(tmp.x(),upBorder));

    /* 2.绘制辅标记 */
    painter.setPen(pen_minortick);
    for(int i=1; i<=minorXNum; i++)
    {
        minorTickX = (qreal)(i*minorTickValueX/valueXPerPixel);
        tmp.setX(leftBorder+minorTickX);
        painter.drawLine(tmp,tmp+QPointF(0,-5));
    }
}

/* 绘制坐标轴y */
void MyWaveWidget::paintAxisY(QPainter &painter)
{
    qreal majorTickY, minorTickY;
    QPen pen_axis(Qt::black, 2);
    QPen pen_gridline(Qt::black, 1, Qt::DotLine);
    if(PRODUCT_DEFAULT_VENDOR == PRODUCT_VENDOR_LV){
        pen_gridline.setColor(QColor(255,255,255));
    }
    QPen pen_majortick(Qt::black, 2);
    QPen pen_minortick(Qt::black, 1);

    valueYPerPixel = ((qreal)(maxValueY-minValueY)/(qreal)(height()-upBorder-downBorder));
    QPointF leftDownPoint(leftBorder,height()-downBorder);
    QPointF leftUpPoint(leftBorder,upBorder);
    QPointF tmp(leftDownPoint);
    QRect textRect;
    qreal halfTextHeight = ((((qreal)height()-upBorder-downBorder))/(majorYNum-1))/3.0;

    painter.setPen(pen_axis);
    painter.drawLine(leftDownPoint,leftUpPoint);

    /* 绘制主标记及横轴虚线 */
    for(int i=1; i<=majorYNum; i++)
    {
        painter.setPen(pen_majortick);
        majorTickY = (qreal)(i*majorTickValueY/valueYPerPixel);
        tmp.setY(height()-downBorder-majorTickY);
        painter.drawLine(tmp, tmp+QPointF(8,0));

        textRect.setTopLeft(QPoint(tmp.x()-40,tmp.y()-halfTextHeight));
        textRect.setSize(QSize(35,2*halfTextHeight));
        textRect.setTopLeft(QPoint(tmp.x()-40,tmp.y()-halfTextHeight));
        textRect.setSize(QSize(35,2*halfTextHeight));

        painter.drawText(textRect,QString("%1")
                         .arg(minValueY+i*majorTickValueY),QTextOption(Qt::AlignCenter));
        painter.setPen(pen_gridline);
        painter.drawLine(tmp, QPointF(width()-rightBorder,tmp.y()));
    }

    textRect.setTopLeft(QPoint(leftBorder-40,height()-downBorder-halfTextHeight));
    textRect.setSize(QSize(35,2*halfTextHeight));

    painter.drawText(textRect,QString("0"),QTextOption(Qt::AlignCenter));
    /* 绘制辅标记 */
    painter.setPen(pen_minortick);
    for(int i=1; i<=minorYNum; i++)
    {
        minorTickY = (qreal)(i*minorTickValueY/valueYPerPixel);
        tmp.setY(height()-downBorder-minorTickY);
        painter.drawLine(tmp,tmp+QPointF(5,0));
    }
}

/* 根据数据设置纵轴坐标 */
void MyWaveWidget::setYByData()
{
    int maxValue = 0;
    //    for(int i=0; i<MAX_EJECTOR*2; i++) {
    //        if(struGsh.pEjectTimes[i] > maxValue) {
    //            maxValue = struGsh.pEjectTimes[i];
    //        }
    //    }

    if(maxValue < 50) {
        setMaxValueY(50);
    } else if(maxValue>=50 && maxValue<100) {
        setMaxValueY(100);
    } else if(maxValue>=100 && maxValue<200) {
        setMaxValueY(200);
    } else if(maxValue>=200 && maxValue<400) {
        setMaxValueY(400);
    }

    setTickAttr();
}

/* 绘制想要的波形 */
void MyWaveWidget::drawWanted(QPainter &painter)
{
    //    if(m_nDrawType == TYPE_WAVE) {
    //        if(struCnfe.nProduct != OPT_PRODUCT_ANCOO_RS) {
    //            getNewdata();
    drawColorWave(painter);
    //        } else {
    //            drawGrayWave(painter);
    //        }
    //    } else if(m_nDrawType == TYPE_EJECT_TIMES){
    //        drawEjectTimes(painter);
    //    }
}

void MyWaveWidget::drawBackgroundValue(QPainter &painter)
{
    QRect rect = this->rect();
    rect.adjust(leftBorder,upBorder,-rightBorder,-downBorder);
    painter.setClipRect(rect);

    int x1 = leftBorder;
    int x2 = width()-rightBorder;
    int y1 = rect.height()+upBorder-(struCnfp.nBackgroundColorValueUp[struGsh.nLayer][struGsh.nView][struGsh.nTdMode])/valueYPerPixel;
    int y2 = rect.height()+upBorder-struCnfp.nBackgroundColorValueLow[struGsh.nLayer][struGsh.nView][struGsh.nTdMode]/valueYPerPixel;

    painter.setPen(QColor(0,255,255));
    painter.drawLine(x1,y1,x2,y1);

    //    x1 = leftBorder+struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nChannelEnd[struGsh.nUnit]/valueXPerPixel;
    //    x2 = x1;
    painter.setPen(QColor(255,255,0));
    painter.drawLine(x1,y2,x2,y2);
}

void MyWaveWidget::drawIrBadPoint(QPainter &painter)
{
    //当前找红外相机要的是传感器上还是下的数据
    int updown = struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nNirUpdownEnable;

    if(struGsh.nSignalDispColor == 1)
    {
        int x1=leftBorder+(struGsh.nIrCurPointIndex[updown]-1)/valueXPerPixel;
        int y1= rect().height();

        int x2 = x1;
        int y2 = 0;

        painter.setPen(QColor(0,0,0));
        painter.drawLine(x1,y1,x2,y2);
    }
    else if(struGsh.nSignalDispColor == 2)
    {
        int x1=leftBorder+(struGsh.nIrCurPointIndex[updown]-1)/valueXPerPixel;
        int y1= rect().height();

        int x2 = x1;
        int y2 = 0;

        painter.setPen(QColor(0,0,0));
        painter.drawLine(x1,y1,x2,y2);
    }
    else if(struGsh.nSignalDispColor == 3)
    {
        int x1=leftBorder+(struGsh.nIrCurPointIndex[updown]-1)/valueXPerPixel;
        int y1= rect().height();

        int x2 = x1;
        int y2 = 0;

        painter.setPen(QColor(0,0,0));
        painter.drawLine(x1,y1,x2,y2);
    }
    else if(struGsh.nSignalDispColor == 4)
    {
        int x1=leftBorder+(struGsh.nIrCurPointIndex[updown]-1)/valueXPerPixel;
        int y1= rect().height();

        int x2 = x1;
        int y2 = 0;

        painter.setPen(QColor(0,0,0));
        painter.drawLine(x1,y1,x2,y2);
    }
}

/* 绘制彩色波形 */
void MyWaveWidget::drawColorWave(QPainter &painter)
{
    if(struGsh.nPageSm == SM_IPC_EJECT_WIDGET ||(m_nDrawType == IPC_EJECT) )
    {
        drawIpcEject(painter);
        drawSingleWave(painter, WAVE_IPC_EJECT);
        return;
    }

    int dim = struCnfp.nSensorDim[struGsh.nLayer][struGsh.nView];
    switch(dim)
    {
    case 1:
        drawSingleWave(painter, WAVE_RED);
        break;
    case 2:
        if(struGsh.nSignalDispColor == 0)
        {
            drawSingleWave(painter, WAVE_RED);
            drawSingleWave(painter, WAVE_GREEN);
        }
        else if(struGsh.nSignalDispColor == 1)
        {
            drawSingleWave(painter, WAVE_RED);
        }
        else if(struGsh.nSignalDispColor == 2)
        {
            drawSingleWave(painter, WAVE_GREEN);
        }
        break;
    case 3:
        drawSingleWave(painter, WAVE_RED);
        drawSingleWave(painter, WAVE_GREEN);
        drawSingleWave(painter, WAVE_BLUE);
        if(struCnfp.nBackgroundColor[struGsh.nLayer][struGsh.nView][struGsh.nTdMode] == BACKGROUND_COLOR_YELLOW)
        {
            drawSingleWave(painter, WAVE_HUE);
            //            drawSingleWave(painter, WAVE_VAVUE);
        }
        break;
    case 4:
        drawSingleWave(painter, WAVE_RED);
        drawSingleWave(painter, WAVE_GREEN);
        drawSingleWave(painter, WAVE_BLUE);
        drawSingleWave(painter, WAVE_INF);
        if(struCnfp.nBackgroundColor[struGsh.nLayer][struGsh.nView][struGsh.nTdMode] == BACKGROUND_COLOR_YELLOW)
        {
            drawSingleWave(painter, WAVE_HUE);
            //            drawSingleWave(painter, WAVE_VAVUE);
        }
        break;
    }

    if(struGsh.nPageSm == SM_BACKGROUND_SET_WIDGET)
    {
        //        if(struCnfp.nBackgroundColor[struGsh.nLayer][struGsh.nView] == BACKGROUND_COLOR_GREY)
        {
            drawBackgroundValue(painter);
        }
    }
    if(struGsh.nPageSm == SM_IR_BAD_POINT_WIDGET)
    {
        drawIrBadPoint(painter);
    }
    if(m_nDrawType == Divide_PIXEL)
        drawDividePix(painter);
    //    if(m_nDrawType == IR_BadPoints)
    //        drawIrBadPoint(painter);
    //    if (struCnfe.nArithmeticSeq[ARITH_INFRA] == 1) {
    //        drawSingleWave(painter, WAVE_INF);
    //    }
}

/* 绘制灰度波形 */
void MyWaveWidget::drawGrayWave(QPainter &painter)
{
    //    if(!struGsh.bIsAssist) {
    drawSingleWave(painter, 0);
    //    } else {
    //        drawSingleWave(painter, WAVE_INF_RS);
    //    }
}

/* 根据标志位显示单一的波形 */
void MyWaveWidget::drawSingleWave(QPainter &painter, int flag)
{
    uchar *pData;
    int *pIpcData;
    int lastGoodPoint = 0;  //红外坏点纠错的上一个好点信号

    int quo = 1;
    QPen pen;
    pen.setWidth(1);
    switch(flag){
    case WAVE_RED:
        pData = struGsh.sRowRed;
        pen.setColor(Qt::red);
        break;
    case WAVE_GREEN:
        pData = struGsh.sRowGreen;
        pen.setColor(Qt::green);
        break;
    case WAVE_BLUE:
        pData = struGsh.sRowBlue;
        pen.setColor(Qt::blue);
        break;
    case WAVE_INF:
        pData = struGsh.sRowInf;
        pen.setColor(Qt::yellow);
        break;
    case IPC_EJECT:
        pIpcData = struGsh.sRowTotal;
        pen.setColor(Qt::blue);
        break;
    case WAVE_HUE:
        pData = struGsh.sRowHue;
        pen.setColor(Qt::darkMagenta);
        break;
    case WAVE_VAVUE:
        pData = struGsh.sRowVaule;
        pen.setColor(Qt::black);
        break;
        //    case WAVE_GRAY:
        //        pData = struGsh.sRgbRow+10;
        //        if(struGsh.nUnit%2 == 0) {
        //            pen.setColor(Qt::red);
        //        } else {
        //            pen.setColor(Qt::blue);
        //        }
        //        break;
        //    case WAVE_INF_RS:
        //        pData = struGsh.sRgbRow+10+struCnfc.nSensorPixelLevel[struGsh.nLevel];
        //        pen.setColor(Qt::yellow);
        //        break;
    default:
        break;
    }

    //    bool isFirstpoint = true;
    QRect rect = this->rect();
    rect.adjust(leftBorder,upBorder,-rightBorder,-downBorder);
    painter.setClipRect(rect);
    painter.setPen(pen);

    int curPixel = myFlow.getSensorPixels(struGsh.nLayer,struGsh.nView);
    if (m_nDrawType == Divide_PIXEL && !sourceShow &&  curPixel >= 1024)
    {
        QPolygonF polyLine(detailLength*3); //横坐标实际的像素点数
        double x;
        double y;
        if(struGsh.nNirDotCorrectived == 0)
        {
            for(int i=0; i< (detailLength*3); i++)
            {
                x=leftBorder+i;
                if (i < detailLength)
                    y=rect.height()+upBorder-pData[i/quo]/valueYPerPixel;
                else if (i >= detailLength && i<detailLength *2){
                    y=rect.height()+upBorder-pData[(i-detailLength+(curPixel/2-detailLength/2))/quo]/valueYPerPixel;
                }
                else if (i >= (detailLength*2)&& i<(detailLength*3)){
                    y=rect.height()+upBorder-pData[(i-detailLength*2+(curPixel-detailLength))/quo]/valueYPerPixel;  // 455 + 569 =1024
                }
                polyLine[i]=QPointF(x,y);
            }
        }
        painter.drawPolyline(polyLine);
    }
    else{
        QPolygonF polyLine(maxValueX);
        double x;
        double y;
        //    QPointF currentPoint, lastPoint;
        if(struGsh.nNirDotCorrectived == 0)
        {
            for(int i=0; i<maxValueX; i++)
            {
                x=leftBorder+i/valueXPerPixel;
                if(flag == WAVE_IPC_EJECT)                {
                    y=rect.height()+upBorder-pIpcData[i/quo]/valueYPerPixel;
                }
                else{
                    y=rect.height()+upBorder-pData[i/quo]/valueYPerPixel;
                }
                polyLine[i]=QPointF(x,y);
            }
        }
        else
        {
            for(int i=0; i<maxValueX; i++)
            {
                if(flag == WAVE_RED)
                {
                    if(struGsh.nNirDotCorrectiveRed[struGsh.nView][struGsh.nUnit][struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nNirUpdownEnable][i] == 0)
                    {
                        lastGoodPoint = i;
                        x=leftBorder+i/valueXPerPixel;
                        y=rect.height()+upBorder-pData[i/quo]/valueYPerPixel;
                        polyLine[i]=QPointF(x,y);
                    }
                    else
                    {
                        x=leftBorder+i/valueXPerPixel;
                        y=rect.height()+upBorder-pData[lastGoodPoint/quo]/valueYPerPixel;
                        polyLine[i]=QPointF(x,y);
                    }
                }
                else if(flag == WAVE_GREEN)
                {
                    if(struGsh.nNirDotCorrectiveGreen[struGsh.nView][struGsh.nUnit][struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[struGsh.nView].nNirUpdownEnable][i] == 0)
                    {
                        lastGoodPoint = i;
                        x=leftBorder+i/valueXPerPixel;
                        y=rect.height()+upBorder-pData[i/quo]/valueYPerPixel;
                        polyLine[i]=QPointF(x,y);
                    }
                    else
                    {
                        x=leftBorder+i/valueXPerPixel;
                        y=rect.height()+upBorder-pData[lastGoodPoint/quo]/valueYPerPixel;
                        polyLine[i]=QPointF(x,y);
                    }
                }
                else if(flag == WAVE_BLUE)
                {
                    x=leftBorder+i/valueXPerPixel;
                    y=rect.height()+upBorder-pData[lastGoodPoint/quo]/valueYPerPixel;
                    polyLine[i]=QPointF(x,y);
                }
                else if(flag == WAVE_INF)
                {
                    x=leftBorder+i/valueXPerPixel;
                    y=rect.height()+upBorder-pData[lastGoodPoint/quo]/valueYPerPixel;
                    polyLine[i]=QPointF(x,y);
                }
                else if(flag == WAVE_HUE)
                {
                    x=leftBorder+i/valueXPerPixel;
                    y=rect.height()+upBorder-pData[lastGoodPoint/quo]/valueYPerPixel;
                    polyLine[i]=QPointF(x,y);
                }
                else if(flag == WAVE_VAVUE)
                {
                    x=leftBorder+i/valueXPerPixel;
                    y=rect.height()+upBorder-pData[lastGoodPoint/quo]/valueYPerPixel;
                    polyLine[i]=QPointF(x,y);
                }
                else if(flag == WAVE_IPC_EJECT)
                {
                    x=leftBorder+i/valueXPerPixel;
                    y=rect.height()+upBorder-pIpcData[lastGoodPoint/quo]/valueYPerPixel;
                    polyLine[i]=QPointF(x,y);

                }
            }
        }

        painter.drawPolyline(polyLine);
    }

    //    free(pData);
}

/* 绘制坐标轴x */
void MyWaveWidget::paitDivideAxisX(QPainter &painter)
{
    qreal majorTickX, minorTickX;
    QPen pen_axis(Qt::black, 2);
    QPen pen_gridline(Qt::black, 1, Qt::DotLine);
    QPen pen_majortick(Qt::black, 2);
    QPen pen_minortick(Qt::black, 1);
    QPen pen_border(Qt::yellow, 1);




    QPointF leftDownPoint(leftBorder, height()-downBorder);
    QPointF rightDownPoint(width()-rightBorder, height()-downBorder);
    /* 记录下tick点的坐标 */
    QPointF tmp(leftDownPoint);
    int pixelSize = painter.fontInfo().pixelSize();
    QRect textRect;
    int curPixel = myFlow.getSensorPixels(struGsh.nLayer,struGsh.nView);

    valueXPerPixel = ((qreal)(maxValueX-minValueX)/(qreal)(width()-leftBorder-rightBorder));
    painter.setPen(pen_axis);
    painter.drawLine(leftDownPoint,rightDownPoint);

    painter.setPen(pen_majortick);
    tmp.setX(leftBorder+detailLength);
    painter.drawLine(tmp,tmp+QPointF(0,-8));
    textRect.setTopLeft(QPoint(tmp.x()-50,tmp.y()+10));
    textRect.setSize(QSize(10*10,pixelSize));
    /* 绘制X轴tick上的值 */
    painter.drawText(textRect, QString("%1|%2").arg(detailLength).arg(curPixel/2-detailLength/2),QTextOption(Qt::AlignHCenter));

    tmp.setX(leftBorder+detailLength*2);
    painter.drawLine(tmp,tmp+QPointF(0,-8));
    textRect.setTopLeft(QPoint(tmp.x()-50,tmp.y()+10));
    textRect.setSize(QSize(10*10,pixelSize));
    /* 绘制X轴tick上的值 */
    painter.drawText(textRect, QString("%1|%2").arg(detailLength/2+curPixel/2).arg(curPixel-detailLength),QTextOption(Qt::AlignHCenter));

    tmp.setX(leftBorder+detailLength*3);
    painter.drawLine(tmp,tmp+QPointF(0,-8));
    textRect.setTopLeft(QPoint(tmp.x()-50,tmp.y()+10));
    textRect.setSize(QSize(10*10,pixelSize));
    /* 绘制X轴tick上的值 */
    painter.drawText(textRect, QString("%1").arg(curPixel),QTextOption(Qt::AlignHCenter));

    tmp.setX(leftBorder+detailLength*3/2);
    painter.drawLine(tmp,tmp+QPointF(0,-8));
    textRect.setTopLeft(QPoint(tmp.x()-50,tmp.y()+10));
    textRect.setSize(QSize(10*10,pixelSize));
    /* 绘制X轴tick上的值 */
    painter.drawText(textRect, QString("%1").arg(curPixel/2),QTextOption(Qt::AlignHCenter));
}
/* 0---------------248-----------372.5----------497-----------------746 //屏0-746
 * 0-------------248|387----------511---------635|774--------------1023 // 0-1023
 * 0-------------248|899-------- 1023--------1137|1798-------------2047 // 0-1023
 */
int MyWaveWidget::getModifiedChannalXPixel(int pixel)
{
    if (m_nDrawType != Divide_PIXEL || sourceShow) return (leftBorder+pixel/valueXPerPixel);

    int curPixel = myFlow.getSensorPixels(struGsh.nLayer,struGsh.nView);
    if (curPixel < 1024){
        return leftBorder+pixel/valueXPerPixel;
    }else{
        if(pixel <= detailLength)
            return leftBorder+pixel;
        else if (pixel >detailLength && pixel < (curPixel/2-detailLength/2))
            return leftBorder+detailLength;
        else if (pixel >= (curPixel/2-detailLength/2)&& pixel <=(curPixel/2+detailLength/2))
            return leftBorder+(detailLength+(pixel-(curPixel/2-detailLength/2)));
        else if (pixel > (curPixel/2+detailLength/2)&& pixel <(curPixel-detailLength))
            return  leftBorder+detailLength*2;
        else if (pixel >= (curPixel-detailLength)&& pixel <curPixel)
            return leftBorder+(detailLength*2+(pixel-(curPixel-detailLength)));
    }
}

void MyWaveWidget::resetAxisYValue()
{
    setMinValueY(0);
    setMaxValueY(2000);
}

void MyWaveWidget::chgAxisYValue()
{
    long pData[MAX_EJECTOR];
    memset(pData,0,sizeof(pData));
    long maxValue = 0;
    long minValue = 0;
    for(int i= 0; i<maxValueX; i++)
    {
        if(pData[i] > maxValue)
        {
            maxValue = pData[i];
        }
        if(pData[i] < minValue)
        {
            minValue = pData[i];
        }
    }
    setMaxValueY(maxValue/500*500+500);
    setMinValueY(minValue/500*500);
}
