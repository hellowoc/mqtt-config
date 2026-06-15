#include "AnimationWidgetLabel.h"

AnimationWidgetLabel::AnimationWidgetLabel(QWidget *parent, QLabel* child) :
    QWidget(parent)
  , m_parentWidget(parent)
  , m_childLabel(child)
{
    m_drawArea = QRect(0, 0, 0, 0);
    hide();
    m_iAnimationState = 0;

    m_firstLabel = new QLabel(this);
    m_secondLabel = new QLabel(this);

    m_frontAnimation = new QPropertyAnimation(m_firstLabel, "pos");
    m_frontAnimation->setDuration(500);
    m_rearAnimation = new QPropertyAnimation(m_secondLabel, "pos");
    m_rearAnimation->setDuration(500);

    m_aniGroup.clear();
    m_aniGroup.addAnimation(m_frontAnimation);
    m_aniGroup.addAnimation(m_rearAnimation);
    connect(&m_aniGroup, SIGNAL(finished()), this, SLOT(OnAnimationsFinished()));
}

void AnimationWidgetLabel::setAnimationArea(const QRect &rect)
{
    m_drawArea = rect;
    resize(rect.size());
    QPoint curPos = m_childLabel->mapTo(m_parentWidget, QPoint(0,0));
    m_labelPos = QPoint(curPos.x()-rect.left(), curPos.y()-rect.top());
    m_firstLabel->resize(m_childLabel->size());
    m_secondLabel->resize(m_childLabel->size());
}


void AnimationWidgetLabel::switchSecondLabel(QString strFirst, QString strSecond)
{
    //动画区域没有设置，不能运行动画
    if(m_drawArea.height() <= 0)
        return;

    m_iAnimationState = 1;
    move(m_drawArea.topLeft());
    show();
    m_childLabel->hide();

    m_firstLabel->setText(strFirst);
    m_firstLabel->setStyleSheet(m_childLabel->styleSheet());
    m_firstLabel->setAlignment(m_childLabel->alignment());
    m_secondLabel->setText(strSecond);
    m_secondLabel->setStyleSheet(m_childLabel->styleSheet());
    m_secondLabel->setAlignment(m_childLabel->alignment());

    m_frontAnimation->setStartValue(m_labelPos);
    m_frontAnimation->setEndValue(QPoint(m_labelPos.x(), -(m_firstLabel->rect().height())));
    m_rearAnimation->setStartValue(QPoint(m_labelPos.x(), m_drawArea.height()));
    m_rearAnimation->setEndValue(m_labelPos);

    m_aniGroup.start();
}

void AnimationWidgetLabel::switchFirstLabel(QString strFirst, QString strSecond)
{
    //动画区域没有设置，不能运行动画
    if(m_drawArea.height() <= 0)
        return;

    m_iAnimationState = 2;
    move(m_drawArea.topLeft());
    show();
    m_childLabel->hide();

    m_firstLabel->setText(strFirst);
    m_firstLabel->setStyleSheet(m_childLabel->styleSheet());
    m_firstLabel->setAlignment(m_childLabel->alignment());
    m_secondLabel->setText(strSecond);
    m_secondLabel->setStyleSheet(m_childLabel->styleSheet());
    m_secondLabel->setAlignment(m_childLabel->alignment());

    m_frontAnimation->setStartValue(QPoint(m_labelPos.x(), -(m_firstLabel->rect().height())));
    m_frontAnimation->setEndValue(m_labelPos);
    m_rearAnimation->setStartValue(m_labelPos);
    m_rearAnimation->setEndValue(QPoint(m_labelPos.x(), m_drawArea.height()));

    m_aniGroup.start();
}

int AnimationWidgetLabel::getAnimationState()
{
    return m_iAnimationState;
}

void AnimationWidgetLabel::OnAnimationsFinished()
{
    m_iAnimationState = 0;
    hide();
    m_childLabel->show();

    emit animationFinished();
}

