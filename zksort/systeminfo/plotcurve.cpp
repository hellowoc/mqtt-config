#include "plotcurve.h"
#include <QDateTime>
#include <QPen>
#include <QTime>
#include <QPainter>
#include "globalparams.h"
#include <QDebug>
#include "globalflow.h"

PlotCurve::PlotCurve(QWidget *parent ) : QWidget(parent)
{
    m_bDrawBoarder = false;

    m_nDrawQueryType = RealTime;
    setMinValueX(0);
    setMinValueY(0);
    setMaxValueX(100);
    setMaxValueY(500);
    setTickAttr();

    font.setPixelSize(16);
    setBorder(50,20,20,40); // 设置坐标轴的左右上下边界
}
PlotCurve::~PlotCurve()
{
}

/* 根据分辨率来设置X坐标 */
void PlotCurve::setXByData()
{
    int maxValue = 0;
      if(!myQueryStatisticThread->m_dataValueList.isEmpty())
           maxValue = myQueryStatisticThread->m_dataCount-1;

    setMaxValueX(maxValue);     // 设置x轴上的最大值
    setTickAttr();
}

/* 根据数据设置Y坐标 */
void PlotCurve::setYByData()
{
    int maxValue = 0,minValue = 0;
    if(!myQueryStatisticThread->m_dataValueList.isEmpty())
        minValue = myQueryStatisticThread->m_dataValueList.at(0);

    for (int i = 0;i < myQueryStatisticThread->m_dataCount;i++)
    {
        if (maxValue < myQueryStatisticThread->m_dataValueList.at(i))
            maxValue = myQueryStatisticThread->m_dataValueList.at(i);
        if (minValue > myQueryStatisticThread->m_dataValueList.at(i))
            minValue = myQueryStatisticThread->m_dataValueList.at(i);
    }
    if (maxValue == 0)
    {
        if (m_nDrawStatisticType == ArithEjectTimes)
        {
            setMaxValueY(500);
        }
        else
        {
            setMaxValueY(4000);
        }
    }
    else
    {
        if (m_nDrawStatisticType == Throughout && m_nDrawQueryType == RealTime)
        {
            setMaxValueY(4000);
        }
        else
        {
            if (maxValue >100000)
                setMaxValueY((maxValue/10000+1)*10000);
            else if (maxValue >10000)
                setMaxValueY((maxValue/1000+1)*1000);
            else if(maxValue >1000)
                setMaxValueY((maxValue/100+1)*100);
            else if(maxValue >100)
                setMaxValueY((maxValue/100+1)*100);
            else if(maxValue >50)
               setMaxValueY((maxValue/50+1)*50);
            else if(maxValue >10)
                setMaxValueY(50);
            else
                setMaxValueY(10);
        }
    }

    setMinValueY(0);
    setTickAttr();
}

/* 设置x轴上的最小值 */
void PlotCurve::setMinValueX(int value)
{
    minValueX = value;
}

/* 设置x轴上的最大值 */
void PlotCurve::setMaxValueX(int value)
{
    maxValueX = value;
}

/* 设置y轴上的最小值 */
void PlotCurve::setMinValueY(int value)
{
    minValueY = value;
}

/* 设置y轴上的最大值 */
void PlotCurve::setMaxValueY(int value)
{
    maxValueY = value;
}

/* 设置坐标轴的边界 */
void PlotCurve::setBorder(int left, int right, int up, int down)
{
    leftBorder = left;
    rightBorder = right;
    upBorder = up;
    downBorder = down;
}

/* 获取左边界 */
int PlotCurve::getLeftBorder()
{
    return leftBorder;
}

/* 获取右边界 */
int PlotCurve::getRightBorder()
{
    return rightBorder;
}

/* 获取上边界 */
int PlotCurve::getUpBorder()
{
    return upBorder;
}

/* 获取下边界 */
int PlotCurve::getDownBorder()
{
    return downBorder;
}

/* 设置标记属性 */
void PlotCurve::setTickAttr()
{
    setTickValue();
    setTickNum();
}

/* 设置标记的数目 */
void PlotCurve::setTickNum()
{
    majorXNum = (maxValueX-minValueX)/majorTickValueX;  // 长刻度数量
    minorXNum = (maxValueX-minValueX)/minorTickValueX;  // 短刻度数量

    majorYNum = (maxValueY-minValueY)/majorTickValueY;
    minorYNum = (maxValueY-minValueY)/minorTickValueY;

}

/* 计算记号上的值 */
void PlotCurve::setTickValue()
{
    majorTickValueX = 1;   // 长刻度单位
    minorTickValueX = 1;   // 短刻度单位

    switch ( maxValueY - minValueY) {
    case 0:
        majorTickValueY = 100;
        minorTickValueY = 50;
        break;
    case 500:
        majorTickValueY = 100;
        minorTickValueY = 50;
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
void PlotCurve::getNewdata(StatisticType statisticType)
{
//    g_infoWidget().setLabelText("repainting...");
//    g_infoWidget().delayShow();
    qDebug()<<"PlotCurve::getnewdata"<<statisticType;
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
    myQueryStatisticThread->setQueryInfo(m_nDrawStatisticType,m_nDrawQueryType);
    myQueryStatisticThread->clearDataMap();

    myQueryStatisticThread->isQuerying = true;
    myQueryStatisticThread->start();

    qDebug()<<"running...";
}

/* 设置绘制的类型 */
void PlotCurve::setDrawType(StatisticType type,QueryType type2)
{
    m_nDrawStatisticType = type;
    m_nDrawQueryType = type2;
}

/* 设置边界是否显示 */
void PlotCurve::setBorderShow(bool bFlag)
{
    m_bDrawBoarder = bFlag;
}

/* 获取边界是否显示 */
bool PlotCurve::getBorderShow()
{
    return m_bDrawBoarder;
}


/* 覆写绘制事件　*/
void PlotCurve::paintEvent(QPaintEvent *)
{
    if (m_nDrawStatisticType != myQueryStatisticThread->m_nStatisticType)
    {
        qDebug()<<"type wrong";
        return;
    }
    if (myQueryStatisticThread->m_dataCount == 0)
    {
        return;
    }
    if (myQueryStatisticThread->isQuerying)
    {
        qDebug()<<"isQuering";
        return;
    }
    QPainter painter(this);
    painter.setFont(font);
qDebug()<<m_nDrawStatisticType<<" real painting.........";
    /* 若绘制重叠波形则无需重绘坐标轴 */
    paintAxis(painter);
    drawWanted(painter);
}

/* 绘制整体的坐标轴 */
void PlotCurve::paintAxis(QPainter &painter)
{
    if(width()<leftBorder+rightBorder || height()<upBorder+downBorder)
        return;

    setXByData();
    setYByData();
    paintAxisX(painter);
    paintAxisY(painter);
}

/* 绘制坐标轴x */
void PlotCurve::paintAxisX(QPainter &painter)
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
        if (m_nDrawQueryType == LastDay)
        {
            painter.drawText(textRect, QString("%1")
                             .arg(minValueX+i*majorTickValueX*2),QTextOption(Qt::AlignHCenter));
        }
        else
        {
            painter.drawText(textRect, QString("%1")
                         .arg(minValueX+i*majorTickValueX),QTextOption(Qt::AlignHCenter));
        }
        painter.setPen(pen_gridline);
        /* 绘制坐标轴中的网格 */
        painter.drawLine(tmp,QPointF(tmp.x(),upBorder));
    }

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
void PlotCurve::paintAxisY(QPainter &painter)
{
    qreal majorTickY, minorTickY;
    QPen pen_axis(Qt::black, 2);
    QPen pen_gridline(Qt::black, 1, Qt::DotLine);
    QPen pen_majortick(Qt::black, 2);
    QPen pen_minortick(Qt::black, 1);


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

        textRect.setTopLeft(QPoint(tmp.x()-50,tmp.y()-halfTextHeight));
        textRect.setSize(QSize(50,2*halfTextHeight));

        if (m_nDrawStatisticType == ArithEjectTimes)
            painter.drawText(textRect,QString("%1")
                             .arg(minValueY+i*majorTickValueY),QTextOption(Qt::AlignCenter));
        else
            painter.drawText(textRect,QString("%1t")
                             .arg((float)(minValueY+i*majorTickValueY)/1000),QTextOption(Qt::AlignCenter));

        painter.setPen(pen_gridline);
        painter.drawLine(tmp, QPointF(width()-rightBorder,tmp.y()));
    }

    textRect.setTopLeft(QPoint(leftBorder-45,height()-downBorder-halfTextHeight));
    textRect.setSize(QSize(45,2*halfTextHeight));

    painter.drawText(textRect,QString("0"),QTextOption(Qt::AlignCenter));
    /* 绘制辅标记 */
    painter.setPen(pen_minortick);
    for(int i=1; i<=minorYNum; i++)
    {
        minorTickY = (qreal)(i*minorTickValueY/valueYPerPixel);
        tmp.setY(height()-downBorder-minorTickY);
        painter.drawLine(tmp,tmp+QPointF(10,0));
    }
}


/* 绘制想要的图形 */
void PlotCurve::drawWanted(QPainter &painter)
{
    QPen pen,pen_dot;
    pen.setWidth(2);
    pen.setColor(Qt::red);
    pen_dot.setWidth(4);
    pen_dot.setColor(Qt::black);

    QRect rect = this->rect();
    rect.adjust(leftBorder,upBorder,-rightBorder,-downBorder);

    painter.setBrush(Qt::black);
    painter.setPen(pen_dot);

    QPolygonF polyLine(myQueryStatisticThread->m_dataCount);
    double x;
    double y;

    for(int i=0; i<myQueryStatisticThread->m_dataCount; i++)
    {
        x=leftBorder+i/valueXPerPixel;
        y=rect.height()+upBorder-(myQueryStatisticThread->m_dataValueList.at(i)-minValueY)/valueYPerPixel;
        polyLine[i]=QPointF(x,y);
//qDebug()<<i<<"===="<<myQueryStatisticThread->m_dataValueList.at(i);
        painter.drawEllipse(QPointF(x,y),2,2);

        if (m_nDrawStatisticType == Throughout)
        {
            float tmp =(float) myQueryStatisticThread->m_dataValueList.at(i)/1000;
            painter.drawText(x+5,y-5,QString::number(tmp,'f',1));
        }
        else
        {
            painter.drawText(x+5,y-5,QString::number(myQueryStatisticThread->m_dataValueList.at(i)));
        }
    }
    painter.setClipRect(rect);
    painter.setPen(pen);

    painter.drawPolyline(polyLine);

}

void PlotCurve::drawBackgroundValue(QPainter &painter)
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

    painter.setPen(QColor(255,255,0));
    painter.drawLine(x1,y2,x2,y2);
}

