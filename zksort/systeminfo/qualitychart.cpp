#include <QPainter>
#include <cstdlib>
#include <math.h>
#include <QDebug>
#include "qualitychart.h"
#include <QTextCodec>

const struct
{
    const int hits;
    QColor color;
}

pageHits[] =
{
{  17, QColor("#ff4500") },
{  10, QColor("#ffa500" ) },
{  20, QColor("#ffff00") },
{  30, QColor("#008000") },
{  25, QColor("#00ffff" ) },
{  14, QColor("#0000ff" ) },
{  9,  QColor("#ff00ff" ) },
{  20, QColor("#00000" ) },
{  11, QColor("#00ffff") },
{  29, QColor("#d98719") },
{  35, QColor("#42426f") },
{  12, QColor("#dbdb70") },
{  5,  QColor("#9370db") },
};

QualityChart::QualityChart(QWidget* parent)
    :QWidget(parent)
{
    QStringList namelist;
    //定义编码格式
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    //设置和对本地文件系统读写时候的编码格式
    QTextCodec::setCodecForLocale(codec);
    //设置传给tr函数时的默认字符串编码
    // [Qt5 Migration] Removed: QTextCodec::setCodecForTr(codec);
    //用在字符常量或者QByteArray构造QString对象时使用的一种编码方式
    // [Qt5 Migration] Removed: QTextCodec::setCodecForCStrings(codec);

}

QualityChart::~QualityChart()
{

}



void QualityChart::updateData()
{
    maxvalue = 0.0;
    for(int i =0; i < m_numList.size(); i++)
    {
        if(maxvalue < m_numList.at(i))
            maxvalue = m_numList.at(i);
    }

    if(maxvalue<100)
        maxvalue = 100;

    update();

}



void QualityChart::paintEvent(QPaintEvent* event)
{

#if 1

    if(namelist.size() <= 0)
        return;


    qDebug()<<this->size();

    if(this->width() < 1000 && this->height() < 400)
    {
        //原点位置
         x_start = 20;
         y_start = 260;

        //坐标轴长度
         y_length = 250;
         x_length= 720;
    }
    else
    {
        //原点位置
         x_start = 20;
         y_start = 385;

        //坐标轴长度
         y_length = 360;
         x_length= 950;
    }


    // 坐标轴宽度
     axis_width= 3;
    // 坐标箭头长度
     arrow_length=10;

    // 坐标轴刻度长度
     scale_length= 5;


    QPainter painter;

    int xaxis_num = namelist.size(); // 直方图个数
  //  qDebug()<<"xaxis_num"<<xaxis_num;

    painter.begin(this);
    QPen pen;
    pen.setWidth(axis_width);
    pen.setColor(Qt::lightGray);
    painter.setPen(pen);

    painter.drawLine(x_start,y_start,x_start + x_length,y_start); // 横坐标
    painter.drawLine(x_start,y_start,x_start,y_start-y_length);   // 纵坐标
    painter.drawLine(x_start,y_start-y_length,x_start+arrow_length -5,y_start-y_length +arrow_length);   //上箭头
    painter.drawLine(x_start,y_start-y_length,x_start-arrow_length +5,y_start-y_length +arrow_length);   //上箭头
    //    painter.drawLine(x_start + x_length,y_start,x_start + x_length - arrow_length,y_start+arrow_length);   //右箭头
    //    painter.drawLine(x_start + x_length,y_start,x_start + x_length -arrow_length,y_start-arrow_length);   //右箭头

    pen.setWidth(1);
    pen.setColor(Qt::black);
    painter.setPen(pen);

    //绘制向上坐标刻度
//    double y_kedujiange = (y_length-25)/10;
//    double jiange = ( maxvalue )/10;
//    for(int i=0;i<=10;i++){
//      //  painter.drawLine(x_start ,y_start - i*y_kedujiange,x_start -5 ,y_start - i*y_kedujiange);
//        //  painter.drawText(QPoint(x_start -33,y_start - i*y_kedujiange),QString::number(i*jiange));
//    }
    //
    //*******************绘制直方图***********//

    int  scale_width  = x_length/(2*xaxis_num +1); // 计算刻度值大小

    int  zhifang_width = scale_width;        // 计算直方图的宽度

//    if(xaxis_num >= 5)
//        zhifang_width = scale_width - 15;

    QFont ft;
    QFont ft_1;


    ft_1.setPixelSize(10);
    ft.setPixelSize(15);

    float rate ;
    int zhifang_heigh;
    QPoint point_tmp;

    for(int i = 1 ; i < xaxis_num+1; i++)
    {

        painter.setFont(ft);

        //  计算直方图高度
        rate = m_numList.at(i-1)/maxvalue;
        zhifang_heigh = rate*(y_length-60); // 直方图的最高度

         // 绘制向下坐标轴刻度
        painter.drawLine(x_start + 2*scale_width*(i-1)+scale_width +zhifang_width/2, y_start, x_start+ 2*scale_width*(i-1)+scale_width +zhifang_width/2, y_start+5);

        // 绘制x轴含杂名称
        point_tmp = QPoint(x_start + 2*scale_width*(i-1)+scale_width + zhifang_width/2 , y_start+25);
        painter.translate(point_tmp);
        painter.rotate(-20);  //
        painter.drawText( QPoint(8-zhifang_width/2,0), namelist.at(i-1) );
        painter.rotate(20);  //
        painter.translate( -point_tmp.x(), -point_tmp.y());

        /***绘制直方图**/
        painter.setBrush(QBrush(pageHits[i%13-1].color));    // 设置画刷颜色
        painter.drawRect(x_start + 2*scale_width*(i-1)+scale_width  , y_start ,zhifang_width,-zhifang_heigh);   // 绘制直方图

        /*直方图上方显示百分比 */
        painter.setFont(ft_1);
        point_tmp = QPoint(x_start + 2*scale_width*(i-1)+scale_width, y_start - zhifang_heigh -8);
        painter.translate(point_tmp);
        painter.rotate(-20);  //

        painter.drawText(QPoint(0,0),
                         QString::number( m_numList.at( i-1 ),'f',0 ) );
        painter.rotate(20);  //
        painter.translate(-point_tmp.x(), -point_tmp.y());

        /************************/
    }

    painter.end();


#endif
}
