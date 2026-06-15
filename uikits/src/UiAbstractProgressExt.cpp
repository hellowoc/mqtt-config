#include "UiAbstractProgressExt.h"
#include <QDebug>
const int DEFALUE_PROGRESS_WIDTH = 54;//defalut progress width
const int DEFAULT_BUTTON_WIDTH = 54;//default button width
const int VIEW_DIVIDE = 5;
const float F_RATIO = 1.0f;

UiAbstractProgressExt::UiAbstractProgressExt(QWidget *parent) :
    QWidget(parent)
{

    progress = NULL;
    m_bPercent = false;
    button_add = new QPushButton(this);
    button_add->setObjectName("button_add");
    button_add->setFocusPolicy(Qt::NoFocus);
    button_add->setText("");

    button_sub = new QPushButton(this);
    button_sub->setObjectName("button_sub");
    button_sub->setFocusPolicy(Qt::NoFocus);
    button_sub->setText("");

    setButtonStyle(DEFULT_BUTTON_STYLE);


    label_ValueText = new QLabel(this);
    label_ValueText->setObjectName("senseValueLabel");
    label_ValueText->setAlignment(Qt::AlignCenter);
    connect(this->button_add,SIGNAL(clicked()),this,SLOT(clickAdd()));
    connect(this->button_sub,SIGNAL(clicked()),this,SLOT(clickSub()));
}

void UiAbstractProgressExt::moveEvent(QMoveEvent *event){
//    onLayout();
}

void UiAbstractProgressExt::resizeEvent(QResizeEvent *event)
{
    //onLayout();
}

void UiAbstractProgressExt::setProgressEnabled(bool enabled)
{
    progress->setEnabled(enabled);
}

void UiAbstractProgressExt::setMaxAndMinValue(int maxValue,int minValue){
    if(progress == NULL)
        return ;
     progress->setMaxAndMinValue(maxValue,minValue);
     m_maxValue = maxValue;
     m_minValue = minValue;
}

void UiAbstractProgressExt::setFloatMaxAndMinValue(float maxValue, float minValue)
{
    if(progress == NULL)
        return ;
     progress->setMaxAndMinValue(maxValue,minValue);
     m_maxValue = maxValue;
     m_minValue = minValue;
}

void UiAbstractProgressExt::setCurrentValue(int currentValue){
    if(progress == NULL)
        return ;
//    if(value >= minValue && value <= maxValue){
//        currentValue = value;
//        update();
//    }
    //int value = (int)((int)currentValue/100.0f-m_minValue)/(m_maxValue-m_minValue)*100;
    progress->setCurrentValue(currentValue);

    updateValue(currentValue);
}

void UiAbstractProgressExt::setFloatCurrentValue(float currentValue)
{
    if(progress == NULL)
        return ;
    progress->setCurrentValue(currentValue);

}
void UiAbstractProgressExt::setBackgroundColor(const QColor &color)
{
    if(progress == NULL)
        return ;
    progress->setBackgroundColor(color);
}

void UiAbstractProgressExt::setFrontgroundColor(const QColor &color)
{
    if(progress == NULL)
        return ;
    progress->setFrontgroundColor(color);
}

void UiAbstractProgressExt::setFontColor(const QColor &color)
{
    if(progress == NULL)
        return ;
    progress->setFontColor(color);
}

void UiAbstractProgressExt::slotsConnect()
{
    connect(progress,SIGNAL(onValueChange(int)),this,SLOT(valueChange(int)));
    connect(progress,SIGNAL(onUpdateValueChange(int)),this,SLOT(updateValue(int)));
}

void UiAbstractProgressExt::setbPercent(bool bPercent)
{
    m_bPercent = bPercent;
}

void UiAbstractProgressExt::setButtonStyle(const QString &style){

    buttonStyleInit(button_add,style);
    buttonStyleInit(button_sub,style);
}

void UiAbstractProgressExt::updateValue(int changeValue)
{
    if(m_bPercent){
        float value = (float)changeValue/((float)m_maxValue-(float)m_minValue)*100.0;
        label_ValueText->setText(QString::number(value,'f',1));
    }else
        label_ValueText->setText(QString::number(changeValue));
}

void UiAbstractProgressExt::buttonStyleInit(QPushButton *btn,const QString &style){
    if(btn == NULL){
        return ;
    }
    btn->setAutoRepeat(true);
    btn->setAutoRepeatDelay(500);
    btn->setAutoRepeatInterval(100);
}

void UiAbstractProgressExt::clickAdd(){
    if(progress == NULL)
        return ;
    progress->sendChangeStep(1);
}
void UiAbstractProgressExt::clickSub(){
    if(progress == NULL)
        return ;
    progress->sendChangeStep(-1);
}
void UiAbstractProgressExt::valueChange(int changeValue){
    updateValue(changeValue);
    emit onValueChangeExt(changeValue);
}
