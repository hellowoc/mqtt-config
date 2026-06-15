#ifndef ANIMATIONFRAME_H
#define ANIMATIONFRAME_H
#include "uikits_shared.h"
#include <QFrame>
#include <QVector>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QTime>
#include <QPaintEvent>

struct SlideChildWidget
{
    QWidget*            child;
    QPoint              pos;
    QPropertyAnimation* animation;
};

class UIKITS_SHARED AnimationFrame : public QFrame
{
    Q_OBJECT
public:
    explicit AnimationFrame(QWidget *parent = 0);

    void addEnableMoveChildWidget(QWidget*);        //增加可以平移的子控件（将第一个子控件左在动画区域内平移的相对位置作为参考值）
    void cleanChildWidgets();                       //清除所有平移的子控件
    void initParam(QRect animationArea, bool bLeftRight);

    void slideToNext();         //显示下一分页的动画
    void slideToPrevious();     //显示上一分页的动画

    void addEventControl(QObject *);    //允许支持滑动的子控件

protected:
    bool eventFilter(QObject *obj, QEvent *ev);

signals:
    void animationFinished();       //动画完成的信号
    void nextPage();                //向左滑屏，显示下一分页信息的信号
    void previousPage();            //向右滑屏，显示上一分页信息的信号

private slots:
    void OnAnimationsFinished();

private:
    QVector<SlideChildWidget> m_childWidgets;
    QParallelAnimationGroup m_aniGroup;
    bool        m_isAnimation;      //是否正在动画
    bool        m_bIsLeftRight;     //动画是左右平移还是上下平移（默认是左右）
    QRect       m_animationArea;    //动画显示的区域

    bool        m_bIsPress;
    QPoint      m_pressPoint;
    QTime       m_time;
};

#endif // ANIMATIONFRAME_H
