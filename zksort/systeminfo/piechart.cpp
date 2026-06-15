#include "piechart.h"
#include <QPainter>
#include <QMouseEvent>
#include <QLinearGradient>
#include <QVector2D>
#include <qmath.h>
#include <QStyleOption>
#include <QDebug>
#include "mylanguage.h"
#include "globalflow.h"
#include "globalparams.h"
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

Piechart::Piechart(QWidget* parent)
    :QWidget(parent)
{


    //定义编码格式
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    //设置和对本地文件系统读写时候的编码格式
    QTextCodec::setCodecForLocale(codec);
    //设置传给tr函数时的默认字符串编码
    // [Qt5 Migration] Removed: QTextCodec::setCodecForTr(codec);
    //用在字符常量或者QByteArray构造QString对象时使用的一种编码方式
    // [Qt5 Migration] Removed: QTextCodec::setCodecForCStrings(codec);
}

Piechart::~Piechart()
{

}

void Piechart::updatewidget()
{

    QColor color ;
    colorList.clear();
    resultList.clear();
    resultsum = 0.0;

    for(int i = 0; i <  namelist.size(); i++)
    {
       resultsum  +=  m_numList.at(i); // 计算总数
       resultList.append(0.0);
    }

    {
        for(int i = 0; i <  namelist.size(); i++)
        {

           if(resultsum <= 0.0)
           {
               resultList[i] = 0.0;
               resultsum = 0.0;
           }
           else
               resultList[i] = m_numList.at(i)/resultsum;
           colorList.append( pageHits[i%13].color) ;
        }

    }

    update();

}

/*  旋转的是文字所在的位置 文字本身并不旋转
 *  point: 文字所在的点
 *  from_angle : 文字所在的度数
 *  rotate : 需要旋转的角度,值为-360~360(为负数表示逆时针旋转,为正数表示顺时针旋转)
 */
QPoint CustomRotate(QPointF point,qreal from_angle,qreal rotate)
{
    qreal PI=3.141592653589;
    QPointF Tmp;
    qreal arc = (rotate-from_angle)/180*PI;
    qreal Length = sqrt(point.x()*point.x() +point.y()*point.y());
    Tmp.setX(Length*cos(arc));
    Tmp.setY(Length*sin(arc));

    return Tmp.toPoint();
}

void Piechart::paintEvent(QPaintEvent* e)
{
#if 1
    m_nPeopleCount = 0;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::NoBrush);

    QRectF rect(-170, -170,(170 << 1), (170 << 1));

    QStringList percentStrlist ;
    percentStrlist.clear();
    /*圆的中心位置*/

    painter.translate(350, 230);
    qreal startAngle = 0;


    for(int i =0 ;i <resultList.size();i++ )
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(colorList.at(i)));

        //该答案选项所占比例
        float percentNum;
        percentNum = (float)resultList.at(i); // 情况三 点击开始并且有下料

        //所占比例对应的角度
        qreal arcLength = 360 * percentNum;

        //画扇形
        QPainterPath path;
        path.arcTo(rect, startAngle, arcLength); // startAngle扇形的起始角度     arcLength扇形的角度值大小

//        //扇形二等分线的角度
        qreal middleAngle = startAngle + arcLength / 2;   //middleAngle  扇形的中间角度
        qreal radianNum = 0;
        if (middleAngle >= 0 && middleAngle < 90)
        {
            radianNum = middleAngle * m_pi / 180;
        }
        else if (middleAngle >= 90 && middleAngle < 180)
        {
            radianNum = (180 - middleAngle)*m_pi / 180;
        }
        else if (middleAngle >= 180 && middleAngle < 270)
        {
            radianNum = (middleAngle - 180)*m_pi / 180;
        }
        else if (middleAngle >= 270 && middleAngle < 360)
        {
            radianNum = (360 - middleAngle)*m_pi / 180;
        }
        //二等分线的中点距原点的距离
        qreal textPosY = 180 * sin(radianNum);
        qreal textPosX = 180 * cos(radianNum);




        //根据二等分线所在的象限确定二等分线的中点坐标
        if (middleAngle >= 0 && middleAngle < 90)
        {

            textPosY = -textPosY;
        }
        else if (middleAngle >= 90 && middleAngle < 180)
        {
            textPosX = -textPosX;
            textPosY = -textPosY;
        }
        else if (middleAngle >= 180 && middleAngle < 270)
        {
            textPosX = -textPosX;
            textPosY = textPosY ;
        }
        else
        {
            textPosX = textPosX;
            textPosY = textPosY;
        }
        startAngle += arcLength;
        QString percentStr = QString::number((float)percentNum*100 ,'f',2);
        percentStr = (percentStr) + "%";
        painter.drawPath(path);

        painter.setPen(QColor(Qt::darkBlue));
		percentStrlist.append(percentStr);
        if(arcLength >= 25) // 角度大于25才显示数值
        {
//           QPoint pointmp =  CustomRotate(QPoint(textPosX , textPosY),startAngle/2, 0);
//           painter.drawText(pointmp,
//                            percentStr );
           // painter.drawText(textPosX , textPosY , 60, 25, Qt::AlignCenter, percentStr);
        }

    }
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QColor(0, 0, 0));
    painter.translate(0, -150);

    //绘制右侧的标注栏
    QFont ft;
    ft.setPixelSize(20);
    painter.setFont(ft);

//    if(struCnfg.bGetMaterialType == 0)
//        painter.drawText(330, -70, 500, 40, Qt::AlignLeft, QString("%1_%2").arg(resultsum).arg(QString::number(struGsh.totalArea/1000000,'f',2) )
//            + g_myLan().unitton ) ;
//    else if(struCnfg.bGetMaterialType == 1)
//    {
//        painter.drawText(330, -70, 500, 40, Qt::AlignLeft, QString("%1_%2").arg(QString::number(struGsh.totalArea/1000000,'f',2) )
//            + g_myLan().unitton ) ;
//    }


//    painter.drawText(330, -70, 500, 40, Qt::AlignLeft, QString("%1_%2").arg(QString::number(struGsh.totalArea/1000000,'f',2) )
//                + g_myLan().unitton ) ;

    int textStart = -30;
    for(int i = 0; i < namelist.size(); i++)
    {
        painter.setBrush(QBrush(colorList.at(i) ) );
        painter.drawRect( 330 -25, textStart, 20,20 );
        painter.drawText(330, textStart, 100, 40, Qt::AlignLeft, namelist.at(i));
        painter.drawText(330 + 65, textStart, 200, 40, Qt::AlignLeft, percentStrlist.at(i)+":"+QString::number(struGsh.totalArea[i],'f',2));

        textStart += 40;

        if( i == 10)
            break;
    }

    QWidget::paintEvent(e);

#endif
}


