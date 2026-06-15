#include <QPainter>
#include <QMouseEvent>
#include "switchbtn.h"
#include <QDebug>
#include <QPainterPath>
SwitchBtn::SwitchBtn(QWidget *parent)
    : QWidget(parent),
      m_nHeight(16),
      m_bChecked(false),
      m_radius(20.0),
      m_nMargin(3),
//      m_checkedColor(26,250,41),
      m_checkedColor(251,153,18),
      m_thumbColor(Qt::white),
      //m_thumbColor(200,200,34),
      m_disabledColor(160, 160, 160),
      m_background(209,212,214)
      //m_background(Qt::gray)
{
    // 鼠标滑过光标形状 - 手型
    setCursor(Qt::PointingHandCursor);

    // 连接信号槽
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

// 绘制开关
void SwitchBtn::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath path;
    QColor background;
    QColor thumbColor;
    qreal dOpacity, dCrlcOpacity;
    if (isEnabled()) { // 可用状态
        if (m_bChecked) { // 打开状态
            background = m_checkedColor;
            thumbColor = m_thumbColor;
            dOpacity = 1.0;
            dCrlcOpacity = 1.0;
        } else { //关闭状态
            background = m_background;
            thumbColor = m_disabledColor;
            dOpacity = 1.0;
            dCrlcOpacity = 1.0;
        }
    } else {  // 不可用状态
        background = m_background;
        thumbColor = m_disabledColor;
        dOpacity = 0.5;
        dCrlcOpacity = 0.6;
    }
    // 绘制大椭圆
    painter.setBrush(background);
    painter.setOpacity(dOpacity);
    path.addRoundedRect(QRectF(m_nMargin, m_nMargin, width() - 2 * m_nMargin, height() - 2 * m_nMargin), m_radius, m_radius);
    painter.drawPath(path.simplified());

    // 绘制小圆
    painter.setBrush(thumbColor);
    painter.setOpacity(dCrlcOpacity);
    painter.drawEllipse(QPointF(m_nX,(height()/2)), height()/2-m_nMargin, height()/2-m_nMargin);
//    if(m_bChecked){
//        QFont font;
//        font.setPointSize(25);
//        painter.setFont(font);
//        painter.setPen(QPen(Qt::white,5));
//        painter.drawText(QRectF(m_nMargin,height()/2-20,50,60),g_myLan()."),QTextOption(Qt::AlignHCenter));
//    }
}

// 鼠标按下事件
void SwitchBtn::mousePressEvent(QMouseEvent *event)
{
    if (isEnabled()) {
        if (event->buttons() & Qt::LeftButton) {
            event->accept();
        } else {
            event->ignore();
        }
    }
}

// 鼠标释放事件 - 切换开关状态、发射toggled()信号
void SwitchBtn::mouseReleaseEvent(QMouseEvent *event)
{
    if (isEnabled()) {
        if ((event->type() == QMouseEvent::MouseButtonRelease) && (event->button() == Qt::LeftButton)) {
            event->accept();
            m_bChecked = !m_bChecked;
            emit toggled(m_bChecked);
            m_timer.start(5);
        } else {
            event->ignore();
        }
    }
}

// 大小改变事件
void SwitchBtn::resizeEvent(QResizeEvent *event)
{
    m_nX = height()/2;
    m_nY = m_nHeight / 2;
    QWidget::resizeEvent(event);
}

// 默认大小
QSize SwitchBtn::sizeHint() const
{
    return minimumSizeHint();
}

// 最小大小
QSize SwitchBtn::minimumSizeHint() const
{
    return QSize(2 * (m_nHeight + m_nMargin), m_nHeight + 2 * m_nMargin);
}

// 切换状态 - 滑动
void SwitchBtn::onTimeout()
{
    if (m_bChecked) {
        m_nX += 2;
        if (m_nX >= width() -height()/2)
            m_timer.stop();
    } else {
        m_nX -= 2;
        if (m_nX <= height()/2)
            m_timer.stop();
    }
    update();
}

// 返回开关状态 - 打开：true 关闭：false
bool SwitchBtn::isToggled() const
{
    return m_bChecked;
}

// 设置开关状态
void SwitchBtn::setToggle(bool checked)
{
    m_bChecked = checked;
    if(checked)
    {
        m_nX =  width() -height()/2;
    }
    else
    {
        m_nX =  height()/2;
    }
    m_nY = m_nHeight / 2;
    update();
}

// 设置背景颜色
void SwitchBtn::setBackgroundColor(QColor color)
{
    m_background = color;
}

// 设置选中颜色
void SwitchBtn::setCheckedColor(QColor color)
{
    m_checkedColor = color;
}

// 设置不可用颜色
void SwitchBtn::setDisbaledColor(QColor color)
{
    m_disabledColor = color;
}
