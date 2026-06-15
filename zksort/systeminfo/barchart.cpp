#include "barchart.h"
#include <QDateTime>
#include <QPen>
#include <QTime>
#include <QPainter>
#include "globalparams.h"
#include "global.h"
#include <QDebug>

BarChart::BarChart(QWidget *parent ) : QWidget(parent)
{
    m_bDrawBoarder = false;

    setMinValueX(0);
    setMinValueY(0);
    setMaxValueX(256);
    setMaxValueY(2000);


    setTickAttr();
    font.setPixelSize(16);
    setBorder(30,20,20,40); // 设置坐标轴的左右上下边界
}
BarChart::~BarChart()
{
}

/* 根据实际算法个数来设置X坐标 */
void BarChart::setXByData(int valueX)
{
    setMaxValueX(valueX);     // 设置x轴上的最大值

    setTickAttr();
}

/* 根据数据设置Y坐标 */
void BarChart::setYByData(int valueY)
{
    if (valueY>100000)
        setMaxValueY((valueY/10000+1)*10000);
    else if (valueY >10000)
        setMaxValueY((valueY/1000+1)*1000);
    else if(valueY >1000)
        setMaxValueY((valueY/100+1)*100);
    else if(valueY >100)
        setMaxValueY((valueY/100+1)*100);
    else if(valueY >50)
        setMaxValueY((valueY/50+1)*50);
    else if(valueY >10)
        setMaxValueY(50);
    else
        setMaxValueY(10);
    setMinValueY(0);
    setTickAttr();
}

/* 设置x轴上的最小值 */
void BarChart::setMinValueX(int value)
{
    minValueX = value;
}

/* 设置x轴上的最大值 */
void BarChart::setMaxValueX(int value)
{
    maxValueX = value;
}

/* 设置y轴上的最小值 */
void BarChart::setMinValueY(int value)
{
    minValueY = value;
}

/* 设置y轴上的最大值 */
void BarChart::setMaxValueY(int value)
{
    maxValueY = value;
}

/* 设置坐标轴的边界 */
void BarChart::setBorder(int left, int right, int up, int down)
{
    leftBorder = left;
    rightBorder = right;
    upBorder = up;
    downBorder = down;
}

/* 获取左边界 */
int BarChart::getLeftBorder()
{
    return leftBorder;
}

/* 获取右边界 */
int BarChart::getRightBorder()
{
    return rightBorder;
}

/* 获取上边界 */
int BarChart::getUpBorder()
{
    return upBorder;
}

/* 获取下边界 */
int BarChart::getDownBorder()
{
    return downBorder;
}

/* 设置标记属性 */
void BarChart::setTickAttr()
{
    setTickValue();
    setTickNum();
}

/* 设置标记的数目 */
void BarChart::setTickNum()
{
    majorXNum = (maxValueX-minValueX)/majorTickValueX;  // 长刻度数量
    minorXNum = (maxValueX-minValueX)/minorTickValueX;  // 短刻度数量


    majorYNum = (maxValueY-minValueY)/majorTickValueY;
    minorYNum = (maxValueY-minValueY)/minorTickValueY;
}

/* 计算记号上的值 */
void BarChart::setTickValue()
{
    if(m_dataIndex <=1 )
    {
        setBorder(30,20,20,40);
    }else{
        setBorder(70,20,20,40);
    }
    switch(maxValueX-minValueX) {
    case 60:
        majorTickValueX = 5;   // 长刻度单位
        minorTickValueX = 1;   // 短刻度单位
        break;
    case 100:
        majorTickValueX = 20;  // 长刻度单位
        minorTickValueX = 5;   // 短刻度单位
        break;
    case 256:
        majorTickValueX = 32;
        minorTickValueX = 8;
        break;
    case 360:
        majorTickValueX = 64;
        minorTickValueX = 16;
        break;
    case 512:
        majorTickValueX = 64;   // 长刻度单位
        minorTickValueX = 16;   // 短刻度单位
        break;
    case 1024:
        majorTickValueX = 200;  // 长刻度单位
        minorTickValueX = 50;   // 短刻度单位
        break;
    case 2048:
        majorTickValueX = 256;  // 长刻度单位
        minorTickValueX = 64;   // 短刻度单位
        break;
    case 4096:
        majorTickValueX = 512;  // 长刻度单位
        minorTickValueX = 128;   // 短刻度单位
        break;
    default:
        majorTickValueX = 50;   // 长刻度单位
        minorTickValueX = 10;   // 短刻度单位
        break;
    }

    switch ( maxValueY - minValueY) {
    case 0:
        majorTickValueY = 100;
        minorTickValueY = 50;
        break;
    case 2000:
        majorTickValueY = 500;
        minorTickValueY = 250;
        break;
    case 4000:
        majorTickValueY = 1000;
        minorTickValueY = 500;
        break;
    default:
        majorTickValueY = (maxValueY - minValueY) /5;
        minorTickValueY =  (maxValueY - minValueY) /5;
        break;
    }

    valueXPerPixel = ((qreal)(maxValueX-minValueX)/(qreal)(width()-leftBorder-rightBorder));
    valueYPerPixel = ((qreal)(maxValueY-minValueY)/(qreal)(height()-upBorder-downBorder));

}
/* 从数据库更新所需显示数据 */
void BarChart::getNewdata(StatisticType type)
{
    qDebug()<<"BarChart::getnewdata"<<type;
//    g_infoWidget().setLabelText("repainting...");
//    g_infoWidget().delayShow();
    myQueryStatisticThread->clearDataMap();
    if(myQueryStatisticThread->isQuerying)
    {
        qDebug()<<"querying...";
        myQueryStatisticThread->isQuerying = false;
        myFlow.msleep(200);

    }
    if (myQueryStatisticThread->isNeedInserting)
    {
        myFlow.msleep(500);
    }
    if (myQueryStatisticThread->isFinished())
    {
        qDebug()<<"finished...";
    }
    myQueryStatisticThread->setQueryInfo(m_nDrawStatisticType,m_nDrawQueryType,m_nDrawViewType);

    myQueryStatisticThread->updateCurrentAriths();
    myQueryStatisticThread->updateCurrentChutes();
    myQueryStatisticThread->isQuerying = true;
    myQueryStatisticThread->start();

    qDebug()<<"running...";
}

/* 设置绘制的类型 */
void BarChart::setDrawType(StatisticType type,QueryType type1,ViewType type2)
{
    m_nDrawStatisticType = type;
    m_nDrawQueryType     = type1;
    m_nDrawViewType      = type2;
}

void BarChart::setDrawType(dataType type,int dataIndex)
{
   m_type = type;
   m_dataIndex = dataIndex;
}

/* 设置边界是否显示 */
void BarChart::setBorderShow(bool bFlag)
{
    m_bDrawBoarder = bFlag;
}

/* 获取边界是否显示 */
bool BarChart::getBorderShow()
{
    return m_bDrawBoarder;
}

/* 覆写绘制事件　*/
void BarChart::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setFont(font);

    paintAxis(painter);

    drawWanted(painter);
}

/* 绘制整体的坐标轴 */
void BarChart::paintAxis(QPainter &painter)
{
    if(width()<leftBorder+rightBorder || height()<upBorder+downBorder)
        return;

    int valueX,valueY = 0;
    switch (m_type) {
    case hsv_h:
        valueX = 360;
        break;
    case hsv_s:
    case hsv_v:
    case rgb_r:
    case rgb_g:
    case rgb_b:
    case matRGB_r:
    case matRGB_g:
    case matRGB_b:
        valueX = 256;
        break;
    case discolor_rg:
    case discolor_rb:
    case discolor_gb:
        valueX = 100;
        break;
    default:
        break;
    }

    if(m_dataIndex <= 1){
        QMap<int,int>::iterator it;
        for(it = g_Runtime().m_imgDataMap[m_dataIndex][m_type].begin();it != g_Runtime().m_imgDataMap[m_dataIndex][m_type].end();++it){
            if(it.value() > valueY){
                valueY = it.value();
            }
        }
    }else{
        for(int i=0; i<256; i++){
            if(struGsh.colorRGBTotal[m_type-matRGB_r][i] > valueY){
                valueY = struGsh.colorRGBTotal[m_type-matRGB_r][i];
            }
        }
    }

    setXByData(valueX);

    setYByData(valueY);
    paintAxisX(painter);
    paintAxisY(painter);

}

/* 绘制坐标轴x */
void BarChart::paintAxisX(QPainter &painter)
{
    qreal majorTickX, minorTickX;
    QPen pen_axis(Qt::black, 2);
    QPen pen_majortick(Qt::black, 2);

    QPointF leftDownPoint(leftBorder, height()-downBorder);
    QPointF rightDownPoint(width()-rightBorder, height()-downBorder);
    /* 记录下tick点的坐标 */
    QPointF tmp(leftDownPoint);
    int pixelSize = painter.fontInfo().pixelSize();
    qreal halfTextLength = (((qreal)width()-leftBorder-rightBorder))/majorXNum/3.0;
    QRect textRect;

    painter.setPen(pen_axis);
    painter.drawLine(leftDownPoint,rightDownPoint);

    /* 1.绘制主标记及纵轴虚线 */

    painter.setPen(pen_majortick);
    for(int i=0; i<=majorXNum; i++)
    {
        majorTickX = (qreal)(i*majorTickValueX/valueXPerPixel);
        tmp.setX(leftBorder+majorTickX);
        /* 绘制tick */
        painter.drawLine(tmp,tmp+QPointF(0,-8));
        textRect.setTopLeft(QPoint(tmp.x()-halfTextLength,tmp.y()+10));
        textRect.setSize(QSize(2*halfTextLength,pixelSize));
        /* 绘制X轴tick上的值 */
        painter.drawText(textRect, QString("%1")
                         .arg(minValueX+i*majorTickValueX),QTextOption(Qt::AlignHCenter));

    }

}

/* 绘制坐标轴y */
void BarChart::paintAxisY(QPainter &painter)
{
    qreal majorTickY, minorTickY;
    QPen pen_axis(Qt::black, 2);
    QPen pen_gridline(Qt::black, 1, Qt::DotLine);
    QPen pen_majortick(Qt::black, 2);
    QPen pen_minortick(Qt::black, 1);


    QPointF leftDownPoint(leftBorder,height()-downBorder);
    QPointF leftUpPoint(leftBorder,upBorder);
    QPointF tmp(leftDownPoint);
    QRect textRect,avgTextRect;
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

        if(m_dataIndex <= 1){
            textRect.setTopLeft(QPoint(tmp.x()-30,tmp.y()-halfTextHeight));
            textRect.setSize(QSize(30,2*halfTextHeight));
        }else{
            textRect.setTopLeft(QPoint(tmp.x()-76,tmp.y()-halfTextHeight));
            textRect.setSize(QSize(80,2*halfTextHeight));
        }

        long value = minValueY+i*majorTickValueY;
        QString valueStr=(value<10000)?QString::number(value):QString::number(value,'e',2);
        painter.drawText(textRect,valueStr,QTextOption(Qt::AlignCenter));
    }

//    textRect.setTopLeft(QPoint(leftBorder-45,height()-downBorder-halfTextHeight));
//    textRect.setSize(QSize(45,2*halfTextHeight));

//    painter.drawText(textRect,QString("0"),QTextOption(Qt::AlignCenter));


    textRect.setTopLeft(QPoint(width()-60,10));
    textRect.setBottomRight(QPoint(width()-15,30));

    avgTextRect.setTopLeft(QPoint(width()-80,30));
    avgTextRect.setBottomRight(QPoint(width()-20,50));
    switch(m_type){
    case hsv_h:
//        painter.setPen(QPen(QColor(62, 147, 192)));
    painter.drawText(textRect,QString("H"),QTextOption(Qt::AlignCenter));
    painter.drawText(avgTextRect,QString("Avg:")+QString::number(struGsh.nAvgH[m_dataIndex]),QTextOption(Qt::AlignCenter));
        break;
    case hsv_s:
//        painter.setPen(QPen(QColor(180, 147, 192)));
    painter.drawText(textRect,QString("S"),QTextOption(Qt::AlignCenter));
    painter.drawText(avgTextRect,QString("Avg:")+QString::number(struGsh.nAvgS[m_dataIndex]),QTextOption(Qt::AlignCenter));
        break;
    case hsv_v:
//        painter.setPen(QPen(QColor(185, 185, 192)));
    painter.drawText(textRect,QString("V"),QTextOption(Qt::AlignCenter));
    painter.drawText(avgTextRect,QString("Avg:")+QString::number(struGsh.nAvgV[m_dataIndex]),QTextOption(Qt::AlignCenter));
        break;
    case rgb_r:
//        painter.setPen(QPen(QColor(255,20,20)));
    painter.drawText(textRect,QString("R"),QTextOption(Qt::AlignCenter));
    painter.drawText(avgTextRect,QString("Avg:")+QString::number(struGsh.nAvgR[m_dataIndex]),QTextOption(Qt::AlignCenter));
        break;
    case rgb_g:
//        painter.setPen(QPen(QColor(185,180,190)));
    painter.drawText(textRect,QString("G"),QTextOption(Qt::AlignCenter));
    painter.drawText(avgTextRect,QString("Avg:")+QString::number(struGsh.nAvgG[m_dataIndex]),QTextOption(Qt::AlignCenter));
        break;
    case rgb_b:
//        painter.setPen(QPen(QColor(2, 2, 255)));
        painter.drawText(textRect,QString("B"),QTextOption(Qt::AlignCenter));
        painter.drawText(avgTextRect,QString("Avg:")+QString::number(struGsh.nAvgB[m_dataIndex]),QTextOption(Qt::AlignCenter));
        break;
    case discolor_rg:
//        painter.setPen(QPen(QColor(2, 2, 255)));
        painter.drawText(textRect,QString("RG"),QTextOption(Qt::AlignCenter));
        painter.drawText(avgTextRect,QString("Avg:")+QString::number(struGsh.nAvgRG[m_dataIndex],'f',1),QTextOption(Qt::AlignCenter));
        break;
    case discolor_rb:
//        painter.setPen(QPen(QColor(2, 2, 255)));
        painter.drawText(textRect,QString("RB"),QTextOption(Qt::AlignCenter));
        painter.drawText(avgTextRect,QString("Avg:")+QString::number(struGsh.nAvgRB[m_dataIndex],'f',1),QTextOption(Qt::AlignCenter));
        break;
    case discolor_gb:
//        painter.setPen(QPen(QColor(2, 2, 255)));
        painter.drawText(textRect,QString("GB"),QTextOption(Qt::AlignCenter));
        painter.drawText(avgTextRect,QString("Avg:")+QString::number(struGsh.nAvgGB[m_dataIndex],'f',1),QTextOption(Qt::AlignCenter));
        break;
    case matRGB_r:
        painter.drawText(textRect,QString("R"),QTextOption(Qt::AlignCenter));
        painter.drawText(avgTextRect,QString("Avg:")+QString::number(struGsh.nMatAvgR),QTextOption(Qt::AlignCenter));
        break;
    case matRGB_g:
        painter.drawText(textRect,QString("G"),QTextOption(Qt::AlignCenter));
        painter.drawText(avgTextRect,QString("Avg:")+QString::number(struGsh.nMatAvgG),QTextOption(Qt::AlignCenter));
        break;
    case matRGB_b:
        painter.drawText(textRect,QString("B"),QTextOption(Qt::AlignCenter));
        painter.drawText(avgTextRect,QString("Avg:")+QString::number(struGsh.nMatAvgB),QTextOption(Qt::AlignCenter));
        break;
    }

    /* 绘制辅标记 */
    painter.setPen(pen_minortick);

    for(int i=1; i<=minorYNum; i++)
    {
        minorTickY = (qreal)(i*minorTickValueY/valueYPerPixel);
        tmp.setY(height()-downBorder-minorTickY);
        painter.drawLine(tmp,tmp+QPointF(10,0));
    }

}


/* 绘制想要的波形 */
void BarChart::drawWanted(QPainter &painter)
{

    qreal curWidth,graphHeight,x,y;
    QRect rect = this->rect();
    rect.adjust(leftBorder,upBorder,-rightBorder,-downBorder);
    painter.setBrush(Qt::black);

    qreal halfTextLength = (((qreal)width()-leftBorder-rightBorder))/majorXNum/3.0;
    for(int i=0; i< maxValueX; i++)
    {
        x = leftBorder+i/valueXPerPixel;
        curWidth = 2;

        if(m_dataIndex <= 1){
            graphHeight = (g_Runtime().m_imgDataMap[m_dataIndex][m_type].value(i) - minValueY)/valueYPerPixel;
            y = rect.height()+upBorder-(g_Runtime().m_imgDataMap[m_dataIndex][m_type].value(i) - minValueY)/valueYPerPixel;
        }else{
            graphHeight = (struGsh.colorRGBTotal[m_type-matRGB_r][i] - minValueY)/valueYPerPixel;
            y = rect.height()+upBorder-(struGsh.colorRGBTotal[m_type-matRGB_r][i] - minValueY)/valueYPerPixel;
        }

        QRect r(x,y,curWidth,graphHeight);

        QPainterPath path;
        path.addRect(r);
        switch (m_type) {
        case hsv_h:
            painter.fillPath(path, QColor(62, 147, 192));
            break;
        case hsv_s:
            painter.fillPath(path, QColor(180, 147, 192));
            break;
        case hsv_v:
            painter.fillPath(path, QColor(185, 185, 192));
            break;
        case rgb_r:
        case matRGB_r:
            painter.fillPath(path, QColor(255, 20, 20));
            break;
        case rgb_g:
        case matRGB_g:
            painter.fillPath(path, QColor(2, 255, 0));
            break;
        case rgb_b:
        case matRGB_b:
            painter.fillPath(path, QColor(2, 2, 255));
            break;
        case discolor_rg:
            painter.fillPath(path, QColor(150, 10, 20));
            break;
        case discolor_rb:
            painter.fillPath(path, QColor(120, 160, 0));
            break;
        case discolor_gb:
            painter.fillPath(path, QColor(18, 190, 255));
            break;
        default:
            break;
        }
    }
}


