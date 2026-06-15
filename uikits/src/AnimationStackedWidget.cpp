#include "AnimationStackedWidget.h"
#include <QPainter>
#include <QDebug>

AnimationStackedWidget::AnimationStackedWidget(QWidget *parent) :
    QStackedWidget(parent)
{
    m_isAnimation = false;
    m_bIsSwitchNext = true;
    //默认的动画间隔时间
    m_duration = 300;
    //初始化animation框架、并连接信号和槽
    m_animation = new QPropertyAnimation(this, QByteArray());
    connect(m_animation, SIGNAL(valueChanged(QVariant)), this, SLOT(valueChanged_slot(QVariant)));
    connect(m_animation, SIGNAL(finished()), this, SLOT(animationFinished()));

    m_bIsPress = false;
}

AnimationStackedWidget::~AnimationStackedWidget()
{

}

void AnimationStackedWidget::addEventControl(QObject * pObj)
{
    pObj->installEventFilter(this);
}

void AnimationStackedWidget::paintEvent(QPaintEvent *e)
{
    if( m_isAnimation )
    {
        QPainter paint(this);
        if(m_bIsSwitchNext)
        {
            //绘制当前Widget
            paintPrevious(paint, currentIndex());
            //绘制下一个widget
            paintNext(paint, m_anotherIndex);
        }
        else
        {
            //绘制上一个Widget
            paintPrevious(paint, m_anotherIndex);
            //绘制当前widget
            paintNext(paint, currentIndex());
        }
    }
}

void AnimationStackedWidget::setDuration(int dura)
{
    m_duration = dura;
}

void AnimationStackedWidget::setCurrentIndex(int index)
{
    if(index >= count() || index < 0)
        return;

    int curIndex = currentIndex();
    if(-1 == curIndex || index == curIndex)
    {
        QStackedWidget::setCurrentIndex(index);
        emit finished();
        return;
    }

    //切换页面显示动画效果
    if(index > curIndex)
        next(index - curIndex);
    else
        forward(curIndex - index);
}

void AnimationStackedWidget::valueChanged_slot(QVariant value)
{
    m_currentValue = value;
    update();
}

void AnimationStackedWidget::animationFinished()
{
    m_isAnimation = false;
    widget(currentIndex())->show();
    QStackedWidget::setCurrentIndex(m_anotherIndex);

    emit finished();
}

bool AnimationStackedWidget::eventFilter(QObject *obj, QEvent *ev)
{
    if(ev->type() == QEvent::MouseButtonPress
        || ev->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent* e = (QMouseEvent*)ev;
        if(e && e->button() == Qt::LeftButton)
        {
            //按下事件
            if(ev->type() == QEvent::MouseButtonPress)
            {
                m_bIsPress = true;
                m_pressPoint = e->pos();
                m_time.restart();
            }
            //弹起事件
            else
            {
                if(m_bIsPress)
                {
                    m_bIsPress = false;
                    int px = e->pos().x() - m_pressPoint.x();
                    int py = e->pos().y() - m_pressPoint.y();

                    //滑动时间不超过5秒，且水平滑动一定距离、比垂直滑动的距离长时，表示该次滑动有效
                    //if(m_time.elapsed() < 5000 && qAbs(px) >= width()/5 && qAbs(px) > qAbs(py))
                    if(m_time.elapsed() < 5000 && qAbs(px) >= 50 && qAbs(px) > qAbs(py))
                    {
                        if(px < 0)
                            next();
                        else
                            forward();
                    }
                }
            }
        }
    }

    return QStackedWidget::eventFilter(obj, ev);
}

/*
void AnimationStackedWidget::mousePressEvent(QMouseEvent *e)
{
    m_bIsPress = true;
    m_pressPoint = e->pos();
    m_time.restart();
}

void AnimationStackedWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if(m_bIsPress)
    {
        m_bIsPress = false;
        int px = e->pos().x() - m_pressPoint.x();
        int py = e->pos().y() - m_pressPoint.y();

        //滑动时间不超过5秒，且水平滑动一定距离、比垂直滑动的距离长时，表示该次滑动有效
        if(m_time.elapsed() < 5000 && qAbs(px) >= width()/4 && qAbs(px) > qAbs(py))
        {
            if(px < 0)
                next();
            else
                forward();
        }
    }
}
*/

void AnimationStackedWidget::next(int intervalPages)
{
    if(intervalPages <= 0)
        return;

    int c = currentIndex();
    if(c + intervalPages >= count())
        return;     //已经是尾页，不能显示下个page

    //如果正在动画，直接返回
    if( m_isAnimation )
        return;

    m_isAnimation = true;
    m_bIsSwitchNext = true;
    m_anotherIndex = c + intervalPages;
    widget(c)->hide();

    //开始动画并设置间隔和开始、结束值
    QRect g = geometry();
    int width = g.width();
    m_animation->setStartValue(width);
    m_animation->setEndValue(0);
    m_animation->setDuration(m_duration);
    m_animation->start();
}

void AnimationStackedWidget::forward(int intervalPages)
{
    if(intervalPages <= 0)
        return;

    int c = currentIndex();
    if(c - intervalPages < 0)
        return;     //已经是首页，不能显示上个page

    //如果正在动画，直接返回
    if( m_isAnimation )
        return;

    m_isAnimation = true;
    m_bIsSwitchNext = false;
    m_anotherIndex = c - intervalPages;
    widget(c)->hide();

    //开始动画并设置间隔和开始、结束值
    QRect g = geometry();
    int width = g.width();
    m_animation->setStartValue(0);
    m_animation->setEndValue(width);
    m_animation->setDuration(m_duration);
    m_animation->start();
}

void AnimationStackedWidget::paintPrevious(QPainter & paint, int preIndex)
{
    //获得当前页面的Widget
    QWidget *w = widget(preIndex);
    QPixmap pixmap(w->size());
    //将Widget的内容渲染到QPixmap对象中，即将Widget变成一张图片
    w->render(&pixmap);
    QRect r = w->geometry();
    //绘制当前的Widget
    double value = m_currentValue.toDouble();
    QRectF r1(0.0, 0.0, value, r.height());
    QRectF r2(r.width() - value, 0, value, r.height());
    paint.drawPixmap(r1, pixmap, r2);
}

void AnimationStackedWidget::paintNext(QPainter & paint, int nextIndex)
{
    QWidget *nextWidget = widget(nextIndex);
    QRect r = geometry();
    //这行代码不加会有bug，第一次切换的时候，QStackedWidget并没有为child分配大小
    nextWidget->resize(r.width(), r.height());
    QPixmap nextPixmap(nextWidget->size());
    nextWidget->render(&nextPixmap);
    double value = m_currentValue.toDouble();
    QRectF r1(value, 0.0, r.width() - value, r.height());
    QRectF r2(0.0, 0.0, r.width() - value, r.height());
    paint.drawPixmap(r1, nextPixmap, r2);
}
