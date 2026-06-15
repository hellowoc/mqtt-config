#include "hsvcolorcircle.h"
#include <QPainter>
#include <QMouseEvent>
#include <QLinearGradient>
#include <QVector2D>
#include <qmath.h>
#include <QStyleOption>
#define Text_WIDTH 24
#define Text_HEIGHT 20
HsvColorCircle::HsvColorCircle(QWidget* parent)
    :QWidget(parent)
{
    m_colorVec << QColor(255, 0, 0) << QColor(255, 255, 0) \
               << QColor(0, 255, 0) << QColor(0, 255, 255) \
               << QColor(0, 0, 255) << QColor(255, 0, 255);
    m_nHValueLow = 0;
    m_nHValueUpp = 60;
    m_nSValueLow = 60;
    m_nSValueUpp = 180;
    m_nVValueLow = 0;
    m_nVValueUpp = 255;
    m_bMove = false;
    //this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_bupper = false;
}

HsvColorCircle::~HsvColorCircle()
{

}
void HsvColorCircle::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    customPaint(&p);
    return QWidget::paintEvent(event);
}
void HsvColorCircle::customPaint(QPainter* painter)
{
    painter->setRenderHint(QPainter::Antialiasing);

    QConicalGradient conicalGradient(this->rect().center(), 0);
    qreal interval = 1.0 / m_colorVec.size();
    // 设置渐变颜色
    for (int i=0; i<m_colorVec.size(); ++i)
    {
        conicalGradient.setColorAt(interval * i, m_colorVec[i]);
    }
    conicalGradient.setColorAt(1.0, m_colorVec[0]);

    // 绘制圆环
    painter->setBrush(conicalGradient);
    painter->setPen(QPen(Qt::NoPen));
    int width = qMin(this->width(), this->height()) - 2*Text_WIDTH ;
    QRect centerRect(this->rect().center().x() - width / 2+5, \
                     this->rect().center().y() - width / 2, \
                     width, width);
    painter->drawEllipse(centerRect);

    //绘制扇形h区域
    int startAngle = m_nHValueLow * 16/10.0;//起始角度，角度可以为负值，如-30*16
    int spanAngle;//覆盖的角度，绘制方向为逆时针方向
    if(m_nHValueLow > m_nHValueUpp){
        startAngle = (m_nHValueLow/10.0-360)*16;
        spanAngle = (360-m_nHValueLow/10.0+m_nHValueUpp/10.0)*16;
    }else{
        spanAngle = (m_nHValueUpp-m_nHValueLow)*16/10.0;
    }
    QPen nPen;
    nPen.setColor(QColor(0, 0, 0));
    nPen.setWidth(2);
    painter->setPen(nPen);
    painter->setBrush(Qt::NoBrush);

    QFont font;
    font.setPixelSize(15);
    painter->setFont(font);
    QRect textRect(0,this->rect().center().y() - Text_HEIGHT / 2, \
                     Text_WIDTH, Text_HEIGHT);
    painter->drawText(textRect,QString("180"));
    textRect = QRect(this->rect().center().x()+width/2+Text_WIDTH/2,this->rect().center().y() - Text_HEIGHT / 2,Text_WIDTH, Text_HEIGHT);
    painter->drawText(textRect,QString("0"));
    textRect= QRect(this->rect().center().x()-5,5,Text_WIDTH, Text_HEIGHT);
    painter->drawText(textRect,QString("90"));
    textRect= QRect(this->rect().center().x()-Text_WIDTH/2,this->rect().center().y()+width/2+5,Text_WIDTH, Text_HEIGHT);
    painter->drawText(textRect,QString("270"));
    painter->drawPie(centerRect, startAngle, spanAngle);


    // 绘制中间部分
    centerRect.adjust(m_nCircleWidth, m_nCircleWidth, -m_nCircleWidth, -m_nCircleWidth);
    painter->setBrush(QColor((80, 80, 80)));
    painter->setPen(QPen(Qt::NoPen));
    painter->drawEllipse(centerRect);



    // 绘制H选择圆圈
    nPen.setColor(QColor(0, 0, 0));
    nPen.setWidth(2);
    painter->setPen(nPen);
    painter->setBrush(Qt::NoBrush);
    QVector2D vec(cos(m_nHValue/10.0 / (180 / 3.1415926)), sin(m_nHValue/10.0 / (180 / 3.1415926)));
    vec.normalize();
    int length = centerRect.width() / 2 + m_nCircleWidth / 2;
    vec = vec * length;
    vec = QVector2D(this->rect().center().x() + vec.x(), this->rect().center().y() - vec.y());
//    painter->drawEllipse(QPoint(vec.x(), vec.y()), m_nCircleWidth / 5, m_nCircleWidth / 5);

    // 绘制中间矩形
    int d = static_cast<int>(sqrt(2) * centerRect.width() / 2);
    QRect rect(centerRect.center().x() - d / 2+1, centerRect.center().y() - d / 2+1, d, d);
    rect.adjust(2, 2, -2, -2);
    drawCenterRect(rect, painter);
}

void HsvColorCircle::drawCenterRect(const QRect& rect, QPainter* painter)
{
    painter->save();
    QLinearGradient linearGradientH(rect.topLeft(), rect.topRight());
    linearGradientH.setColorAt(0, QColor(255, 255, 255));
    QColor color;
    int hValue;
    if(m_nHValueLow < m_nHValueUpp)
        hValue = m_nHValueLow/10.0+(m_nHValueUpp-m_nHValueLow)/10.0/2;
    else
        hValue = m_nHValueLow/10.0+(360+m_nHValueUpp-m_nHValueLow)/10.0/2;

    color.setHsv(hValue, S_V_MAX, S_V_MAX);
    linearGradientH.setColorAt(1, color);
    painter->fillRect(rect, linearGradientH);

    QLinearGradient linearGradientV(rect.topLeft(), rect.bottomLeft());
    linearGradientV.setColorAt(0, QColor(0, 0, 0, 0));
    linearGradientV.setColorAt(1, QColor(0, 0, 0, 255));
    painter->fillRect(rect, linearGradientV);

    // 绘制选中的圈
    int h, s, v;
    m_currentColor.getHsv(&h, &s, &v);
    qreal interValX = rect.width() * m_nSValueLow * 1.0 / S_V_MAX;
    qreal interValY = rect.height() * m_nVValueLow * 1.0 / S_V_MAX;

    qreal interValX2 = rect.width() * m_nSValueUpp * 1.0 / S_V_MAX;
    qreal interValY2 = rect.height() * m_nVValueUpp * 1.0 / S_V_MAX;
    QPoint centerPos(rect.bottomLeft().x() + interValX, rect.bottomLeft().y() - interValY);
    QPen nPen;
    nPen.setColor(QColor(0, 0, 0));
    nPen.setWidth(2);
    painter->setPen(nPen);
    painter->setBrush(Qt::NoBrush);
//    painter->drawEllipse(centerPos, m_nCircleWidth / 5, m_nCircleWidth / 5);

    QRect svrect;
    svrect.setBottomLeft(rect.bottomLeft()+QPoint(interValX,-interValY));
    svrect.setTopRight(rect.bottomLeft()+QPoint(interValX2,-interValY2));
//    painter->fillRect(svrect,Qt::black);

    painter->restore();
}

qreal HsvColorCircle::distance(QPoint p1, QPoint p2)
{
    return sqrt((p1.x() - p2.x()) * (p1.x() - p2.x()) + (p1.y() - p2.y()) * (p1.y() - p2.y()));
}

int HsvColorCircle::getHValue(QPoint pos)
{
    QPoint centerPos = this->rect().center();
    QVector2D vec = QVector2D(pos) - QVector2D(centerPos);
    vec.normalize();

    float angle = acos(vec.x());
    int value = static_cast<int>(angle * (180 / 3.1415926)*10);
    if (pos.y() > centerPos.y())
        value = 3600 - value;

    return value;
}

void HsvColorCircle::updateCurrentColor(QPoint pos)
{
    int width = qMin(this->width(), this->height()) / 2 * 2;
    QRect centerRect(this->rect().center().x() - width / 2, \
                     this->rect().center().y() - width / 2, \
                     width, width);

    centerRect.adjust(m_nCircleWidth, m_nCircleWidth, -m_nCircleWidth, -m_nCircleWidth);
    int d = static_cast<int>(sqrt(2) * centerRect.width() / 2);
    QRect rect(centerRect.center().x() - d / 2, centerRect.center().y() - d / 2, d, d);
    rect.adjust(2, 2, -2, -2);

    int sValue = qMin(static_cast<int>((pos.x() - rect.x()) * 1.0 / rect.width() * S_V_MAX), S_V_MAX);
    int vValue = qMin(static_cast<int>((rect.bottom() - pos.y()) * 1.0 / rect.height() * S_V_MAX), S_V_MAX);
    sValue = qMax(sValue, 0);
    vValue = qMax(vValue, 0);
    m_currentColor.setHsv(m_nHValue/10.0, sValue, vValue);
}

QColor HsvColorCircle::getCurrentColor(void)
{
    int h, s, v;
    m_currentColor.getHsv(&h, &s, &v);
    m_currentColor.setHsv(m_nHValue/10.0, s, v);

    return m_currentColor;
}

void HsvColorCircle::setCurrentColor(const QColor& color)
{
    m_currentColor = color;

    int h, s, v;
    m_currentColor.getHsv(&h, &s, &v);
    m_nHValue = h*10.0;
    this->update();
}

void HsvColorCircle::setHsv(int h1, int s1, int v1, int h2, int s2, int v2)
{
    m_nHValueLow = h1;
    m_nSValueLow = s1;
    m_nVValueLow = v1;
    m_nHValueUpp = h2;
    m_nSValueUpp = s2;
    m_nVValueUpp = v2;
    update();
}

void HsvColorCircle::getHsv(int &h1, int &s1, int &v1, int &h2, int &s2, int &v2)
{
    h1 = m_nHValueLow;
    h2 = m_nHValueUpp;
    s1 = m_nSValueLow;
    s2 = m_nSValueUpp;
    v1 = m_nVValueLow;
    v2 = m_nVValueUpp;
}

void HsvColorCircle::resizeEvent(QResizeEvent* event)
{
    int width = qMin(this->size().width(), this->size().height());
    m_nCircleWidth = static_cast<int>(width / 4.0);

    return QWidget::resizeEvent(event);
}

void HsvColorCircle::mousePressEvent(QMouseEvent* event)
{
    m_selectMode = Normal;
    QPoint pos = event->pos();

    int width = qMin(this->width(), this->height()) / 2 * 2;
    QRect centerRect(this->rect().center().x() - width / 2, \
                     this->rect().center().y() - width / 2, \
                     width, width);

    qreal distance1 = distance(this->rect().center(), pos);
    qreal distance2 = abs(centerRect.topLeft().x() - this->rect().center().x());

    // 判断是否选中圆环
    centerRect.adjust(m_nCircleWidth, m_nCircleWidth, -m_nCircleWidth, -m_nCircleWidth);
    qreal distance3 = abs(centerRect.topLeft().x() - this->rect().center().x());
    if (distance1 > distance3 && distance1 < distance2)
        m_selectMode = Circle;

    // 判断是否选中中间矩形
    int d = static_cast<int>(sqrt(2) * centerRect.width() / 2);
    QRect rect(centerRect.center().x() - d / 2, centerRect.center().y() - d / 2, d, d);
    rect.adjust(2, 2, -2, -2);
    if (rect.contains(pos))
        m_selectMode = Rect;

    if (m_selectMode == Circle)
    {
        m_nHValue = getHValue(pos);
        int Udis = qAbs(m_nHValue-m_nHValueLow);
        int Ldis = qAbs(m_nHValue-m_nHValueUpp);
        if(Udis < 200 || Ldis < 200){
            m_bMove = true;
            if(Udis > Ldis)
                m_bupper = true;
            else
                m_bupper = false;
//            emit colorChanged();
            this->update();
        }else{
            m_bMove = false;
        }

    }
    else if (m_selectMode == Rect)
    {
        updateCurrentColor(pos);
//        emit colorChanged();
        this->update();
    }

return QWidget::mousePressEvent(event);
}

void HsvColorCircle::mouseReleaseEvent(QMouseEvent* event)
{
    QPoint pos = event->pos();
    if (m_selectMode == Circle)
    {
        if(m_bMove){
            m_nHValue = getHValue(pos);
            if(m_bupper)
                m_nHValueUpp = m_nHValue;
            else
                m_nHValueLow = m_nHValue;
            emit colorChanged();
            this->update();
        }
    }
    else if (m_selectMode == Rect)
    {
        updateCurrentColor(pos);
        emit colorChanged();
        this->update();
    }

    m_selectMode = Normal;
return QWidget::mouseReleaseEvent(event);
}

void HsvColorCircle::mouseMoveEvent(QMouseEvent* event)
{
    QPoint pos = event->pos();
    if (m_selectMode == Circle)
    {
        if(m_bMove){
            m_nHValue = getHValue(pos);
            if(m_bupper)
                m_nHValueUpp = m_nHValue;
            else
                m_nHValueLow = m_nHValue;
//            emit colorChanged();
            this->update();
        }
    }
    else if (m_selectMode == Rect)
    {
        updateCurrentColor(pos);
//        emit colorChanged();
        this->update();
    }
    return QWidget::mouseMoveEvent(event);
}

QSize HsvColorCircle::sizeHint() const
{
    QSize size(250, 250);
    return size;
}
