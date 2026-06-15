#include "AnimationFrame.h"

AnimationFrame::AnimationFrame(QWidget *parent) :
    QFrame(parent)
{
    m_isAnimation = false;
    m_bIsLeftRight = true;
    connect(&m_aniGroup, SIGNAL(finished()), this, SLOT(OnAnimationsFinished()));
    m_bIsPress = false;
}

void AnimationFrame::addEnableMoveChildWidget(QWidget * child)
{
    if(child)
    {
        SlideChildWidget scw;
        scw.child = child;
        scw.pos = child->mapTo(this, QPoint(0,0));
        scw.animation = new QPropertyAnimation(child, "pos");   //每个子控件都创建一个动画
        scw.animation->setDuration(500);

        m_childWidgets.push_back(scw);
        m_aniGroup.addAnimation(scw.animation); //将动画放入并行处理的动画组中
    }
}

void AnimationFrame::cleanChildWidgets()
{
    m_childWidgets.clear();
    m_aniGroup.clear();
}

void AnimationFrame::initParam(QRect animationArea, bool bLeftRight)
{
    m_animationArea = animationArea;
    m_bIsLeftRight = bLeftRight;
}

void AnimationFrame::slideToNext()
{
    if(m_isAnimation || 0 == m_childWidgets.size())
    {
        emit animationFinished();
        return;
    }

    m_isAnimation = true;
    if(0 == m_animationArea.width())
        m_animationArea = rect();

    int movePixel;  //向左/向上移动的像素值
    SlideChildWidget scw;
    for(int i = 0; i < m_childWidgets.size(); i++)
    {
        scw = m_childWidgets[i];
        //获取第一个控件向左/向上移动的像素值，后续控件以该参考值作为移动的相对位置
        if(0 == i)
        {
            if(m_bIsLeftRight)
                movePixel = scw.pos.x() - m_animationArea.left();
            else
                movePixel = scw.pos.y() - m_animationArea.top();
        }

        scw.animation->setStartValue(scw.pos);
        if(m_bIsLeftRight)
            scw.animation->setEndValue(QPoint(scw.pos.x()-movePixel, scw.pos.y()));
        else
            scw.animation->setEndValue(QPoint(scw.pos.x(), scw.pos.y()-movePixel));
    }
    m_aniGroup.start(); //启动动画，直到动画结束
}

void AnimationFrame::slideToPrevious()
{
    if(m_isAnimation || 0 == m_childWidgets.size())
    {
        emit animationFinished();
        return;
    }

    m_isAnimation = true;
    if(0 == m_animationArea.width())
        m_animationArea = rect();

    int movePixel;  //向右/向下移动的像素值
    SlideChildWidget scw;
    for(int i = 0; i < m_childWidgets.size(); i++)
    {
        scw = m_childWidgets[i];
        //获取第一个控件向右/向下移动的像素值，后续控件以该参考值作为移动的相对位置
        if(0 == i)
        {
            if(m_bIsLeftRight)
                movePixel = m_animationArea.right() - scw.pos.x();
            else
                movePixel = m_animationArea.bottom() - scw.pos.y();
        }

        scw.animation->setStartValue(scw.pos);
        if(m_bIsLeftRight)
            scw.animation->setEndValue(QPoint(scw.pos.x()+movePixel, scw.pos.y()));
        else
            scw.animation->setEndValue(QPoint(scw.pos.x(), scw.pos.y()+movePixel));
    }
    m_aniGroup.start(); //启动动画，直到动画结束
}

void AnimationFrame::addEventControl(QObject * pObj)
{
    pObj->installEventFilter(this);
}

bool AnimationFrame::eventFilter(QObject *obj, QEvent *ev)
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
                    if(m_time.elapsed() < 5000 && qAbs(px) >= 50 && qAbs(px) > qAbs(py))
                    {
                        if(px < 0)
                            emit nextPage();
                        else
                            emit previousPage();
                    }
                }
            }
        }
    }

    return QFrame::eventFilter(obj, ev);
}

void AnimationFrame::OnAnimationsFinished()
{
    if(m_isAnimation)
    {
        m_isAnimation = false;
        //动画结束后，将移动的各个子控件再移回原位置
        for(int i = 0; i < m_childWidgets.size(); i++)
            m_childWidgets[i].child->move(m_childWidgets[i].pos);

        emit animationFinished();
    }
}
