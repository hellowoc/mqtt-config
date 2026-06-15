#include "UiAbstractProgress.h"
#include <QDebug>
UiAbstractProgress::UiAbstractProgress(QWidget *parent) :
    QWidget(parent)
{

//    m_bgColor = QColor::fromRgb(255,255,255);
    m_bgColor = QColor("#ffffff");
    m_fgColor = QColor("#7bbcff");
    m_fontColor = QColor("#007ffc");
//    m_fgColor = QColor::fromRgb(17,100,224,180);
//    m_fgColor = QColor::fromRgb(51,153,255,180);
//    m_fgColor = QColor::fromRgb(240,240,30);
//    m_fgColor = QColor::fromRgb(30,232,216);
    m_fontColor = QColor(Qt::white);
    m_tmp_x = INT_MAX;
    m_tmp_y = INT_MAX;

    minValue = 1;
    maxValue = 100;
    currentValue = 0;
    m_fontSize = 25;

    m_step = 1;

    setBackgroundColor(m_bgColor);
}
void UiAbstractProgress::mouseMoveEvent(QMouseEvent *e){
    onEvent(e->x(),e->y());
    emit onUpdateValueChange(currentValue);
}

void UiAbstractProgress::mouseReleaseEvent(QMouseEvent *event){
    emit onValueChange(currentValue);
    m_tmp_x = INT_MAX;
    m_tmp_y = INT_MAX;
}

void UiAbstractProgress::dragMoveEvent(QDragMoveEvent *event){
    onEvent(event->pos().x(),event->pos().y());
    emit onUpdateValueChange(currentValue);
}

void UiAbstractProgress::dragLeaveEvent(QDragLeaveEvent *event){
    emit onValueChange(currentValue);
    m_tmp_x = INT_MAX;
    m_tmp_y = INT_MAX;
}


void UiAbstractProgress::setBackgroundColor(const QColor &color){

    QPalette pal=this->palette();
    pal.setColor(QPalette::Background,color);
    setAutoFillBackground(true);
    this->setPalette(pal);

    m_bgColor = color;
}
void UiAbstractProgress::setFrontgroundColor(const QColor &color){
    m_fgColor = color;
    update();
}
void UiAbstractProgress::setFontColor(const QColor &color){
    m_fontColor = color;
    update();
}
void UiAbstractProgress::sendChangeStep(int step){
    currentValue +=step;
    if(currentValue<=minValue){
        currentValue = minValue;
    }else if(currentValue >= maxValue){
        currentValue = maxValue;
    }
    emit onValueChange(currentValue);
    update();
}

int UiAbstractProgress::getCurrentValue() const
{
    return currentValue;
}

void UiAbstractProgress::setCurrentValue(int value)
{
    if(value >= minValue && value <= maxValue){
        currentValue = value;
        update();
    }
}
int UiAbstractProgress::getMaxValue() const
{
    return maxValue;
}

void UiAbstractProgress::setMaxValue(int value)
{
    maxValue = value;
}
float UiAbstractProgress::getMinValue() const
{
    return minValue;
}

void UiAbstractProgress::setMinValue(float value)
{
    minValue = value;
}

void UiAbstractProgress::setMaxAndMinValue(int max,int min){
    int bIsUpdate = false;
    if(max != maxValue || min != minValue)
        bIsUpdate = true;
    maxValue = max;
    minValue = min;
    if(bIsUpdate)
        update();
    if(max == min)
    {
        //qDebug()<<"Max can not equal Min";
    }
}

void UiAbstractProgress::setFloatMaxAndMinValue(float max, float min)
{
    int bIsUpdate = false;
    if(max != maxValue || min != minValue)
        bIsUpdate = true;
    maxValue = max;
    minValue = min;
    if(bIsUpdate)
        update();
    if(max == min)
    {
        //qDebug()<<"Max can not equal Min";
    }
}

void UiAbstractProgress::setFontSize(int fontSize)
{
    m_fontSize = fontSize;
}
