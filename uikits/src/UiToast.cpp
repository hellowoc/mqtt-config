#include "UiToast.h"
#include <QTimer>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QDebug>
const int DEFAULT_H_SAPCE = 10;
const int DEFAULT_V_SAPCE = 5;

UiToast::UiToast(QWidget *parent,QString message,const QString &style) :
    QLabel(parent)
{
    mode = 0;
    m_parent = parent;
    alpha = 255;
    fontSize = 18;
    m_message = message;
    this->setText(m_message);
    this->setStyleSheet(style);
    this->setAlignment(Qt::AlignCenter);

    setAutoSize();


    startx = 0;
    starty = 0;
    endy   = 0;

}
UiToast::UiToast(QWidget *parent ,QString message) :
    QLabel(parent),
    m_parent(parent),
    bgColor(17,205,124),
    fontColor(255,255,255),
    fontSize(18),
    m_message(message){

    mode = 1;
    this->setText(m_message);
    this->setAlignment(Qt::AlignCenter);

    setAutoSize();

    setAlpha(255);



}

void UiToast::setAutoSize(){
    QFont font;
    font.setPointSize(fontSize);
    this->setFont(font);

    QFontMetrics fm(font);
    QRect rec = fm.boundingRect(m_message);

    m_width = rec.width()+2*DEFAULT_H_SAPCE;
    m_height = rec.height()+2*DEFAULT_V_SAPCE;

    this->setFixedSize(m_width,m_height);
}
void UiToast::caculatePosition(TOAST_POSITION_FLAG posFlag){
    if(m_parent == NULL){
        qDebug()<<"m_parent is NULL";
        return;
    }
    switch(posFlag){
       case TOAST_POSITION_BOTTOM:
          {
           startx = (m_parent->width()-m_width)/2;
           starty =  m_parent->height();
           endy   =  m_parent->height()-m_height;

          }
          break;
       case TOAST_POSITION_TOP:
          {
            startx = (m_parent->width()-m_width)/2;
            starty =  -m_height;
            endy   =  0;
          }
          break;
       default:{
            startx = (m_parent->width()-m_width)/2;
            starty =  m_parent->height()-m_height;
            endy   = (m_parent->height()-m_height)*3/4;
          }
          break;
    }
}

void UiToast::showToast(TOAST_FLAG flag,TOAST_POSITION_FLAG posFlag){
    caculatePosition(posFlag);

    m_flag = flag;
    QPropertyAnimation *animal= new QPropertyAnimation(this,"pos");
    animal->setDuration(500);
    animal->setStartValue(QPoint(startx,starty));
    animal->setEndValue(QPoint(startx,endy));
    animal->setEasingCurve(QEasingCurve::OutCubic);
    connect(animal, SIGNAL(finished()), this, SLOT(animationFinished()));

    this->show();

    animal->start();

}
void UiToast::showToast(QWidget *anchor,int offsetX,int offsetY,TOAST_FLAG flag,TOAST_POSITION_FLAG posFlag){

    QPoint pos=anchor->mapTo(m_parent, QPoint(anchor->width()/2-width()/2, 0));

    int  sx =pos.x();
    int  sy =pos.y();

    int  ex = sx + offsetX;
    int  ey = sy + offsetY - height();

    m_flag = flag;
    QPropertyAnimation *posAnimal= new QPropertyAnimation(this,"pos");
    posAnimal->setDuration(500);
    posAnimal->setStartValue(QPoint(sx,sy));
    posAnimal->setEndValue(QPoint(ex,ey));
    posAnimal->setEasingCurve(QEasingCurve::OutCubic);

    QPropertyAnimation *opacityAnimal= new QPropertyAnimation(this,"alpha");
    opacityAnimal->setDuration(500);
    opacityAnimal->setStartValue(120);
    opacityAnimal->setEndValue(255);

    QParallelAnimationGroup *animationGroup = new QParallelAnimationGroup(this);
    animationGroup->addAnimation(posAnimal);
    animationGroup->addAnimation(opacityAnimal);
    connect(animationGroup,SIGNAL(finished()), this, SLOT(animationFinished()));


    this->show();




    animationGroup->start();

}


void UiToast::dismiss(){
    delete this;
}
void UiToast::animationFinished(){
    if(m_flag == TOAST_LENGTH){
       QTimer::singleShot(2000,this,SLOT(dismiss()));
    }else{
       QTimer::singleShot(800,this,SLOT(dismiss()));
    }

}
int UiToast::getAlpha() const
{
    return alpha;
}

void UiToast::setAlpha(int value)
{
    alpha = value;
    QPalette pa=this->palette();

    pa.setColor(QPalette::Background,QColor::fromRgb(bgColor.red(),bgColor.green(),bgColor.blue(),alpha));
    pa.setColor(QPalette::Foreground,QColor::fromRgb(fontColor.red(),fontColor.green(),fontColor.blue(),alpha));
    this->setAutoFillBackground(true);

    this->setPalette(pa);
}

QColor UiToast::getBgColor() const
{
    return bgColor;
}

void UiToast::setBgColor(const QColor &value)
{
    bgColor = value;
}

int UiToast::getFontSize() const
{
    return fontSize;
}

void UiToast::setFontSize(int value)
{
    fontSize = value;
}

QColor UiToast::getFontColor() const
{
    return fontColor;
}

void UiToast::setFontColor(const QColor &value)
{
    fontColor = value;
}

