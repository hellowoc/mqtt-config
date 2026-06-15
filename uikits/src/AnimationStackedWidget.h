#ifndef ANIMATIONSTACKEDWIDGET_H
#define ANIMATIONSTACKEDWIDGET_H
#include "uikits_shared.h"
#include <QPaintEvent>
#include <QStackedWidget>
#include <QPropertyAnimation>
#include <QTime>

class UIKITS_SHARED AnimationStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit AnimationStackedWidget(QWidget *parent = 0);
    ~AnimationStackedWidget();

     //设置动画持续的间隔
    void addEventControl(QObject *);
    void setDuration(int dura);

public slots:
    void setCurrentIndex(int index);   //改写基类的方法

signals:
    void finished();

private slots:
    //属性动画值改变的槽
    void valueChanged_slot(QVariant );
    //动画切换完成
    void animationFinished();

protected:
    bool eventFilter(QObject *obj, QEvent *ev);
    //void mousePressEvent(QMouseEvent *e);
    //void mouseReleaseEvent(QMouseEvent *e);

private:
    void next(int intervalPages = 1);      //显示下几页
    void forward(int intervalPages = 1);   //显示上几页

    void paintEvent(QPaintEvent *);
    void paintPrevious(QPainter &, int);
    void paintNext(QPainter &, int);

private:
    QPropertyAnimation* m_animation; //动画框架
    int         m_duration;         //动画的持续时间
    bool        m_isAnimation;      //是否正在动画
    QVariant    m_currentValue;     //被Animation动画改变的值
    int         m_anotherIndex;    //与当前页面切换的另一个页面的索引
    bool        m_bIsSwitchNext;    //是否往后切换页面的标记

    bool        m_bIsPress;
    QPoint      m_pressPoint;
    QTime       m_time;
};

#endif // ANIMATIONSTACKEDWIDGET_H
