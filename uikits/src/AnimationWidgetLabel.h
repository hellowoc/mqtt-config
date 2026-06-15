#ifndef ANIMATIONWIDGETLABEL_H
#define ANIMATIONWIDGETLABEL_H
#include "uikits_shared.h"
#include <QWidget>
#include <QLabel>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

//前后视Label切换的动画控件
class UIKITS_SHARED AnimationWidgetLabel : public QWidget
{
    Q_OBJECT
public:
    explicit AnimationWidgetLabel(QWidget *parent, QLabel* child);

public:
    void setAnimationArea(const QRect& rect);
    void switchSecondLabel(QString strFirst, QString strSecond);
    void switchFirstLabel(QString strFirst, QString strSecond);
    int getAnimationState();

signals:
    void animationFinished();

private slots:
    void OnAnimationsFinished();


private:
    QWidget*    m_parentWidget;
    QLabel*     m_childLabel;
    QRect       m_drawArea;     //动画运行的区域（相对于父窗体的坐标）

    QPoint  m_labelPos;         //相对该widget的m_childLabel坐标位置
    QLabel*  m_firstLabel, *m_secondLabel;
    QPropertyAnimation* m_frontAnimation, *m_rearAnimation;
    QParallelAnimationGroup m_aniGroup;
    int    m_iAnimationState;       //前后视切换的动画状态：0/1/2——未进行动画/正在进行前视切换后视的动画/正在进行后视切换前视的动画
};

#endif // ANIMATIONWIDGETLABEL_H
