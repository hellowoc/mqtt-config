#include "UiVerticalProgress.h"
#include <QDebug>
#include <qmath.h>
#include <climits>
#include <QPushButton>

UiVerticalProgress::UiVerticalProgress(QWidget *parent) :
    UiAbstractProgress(parent)
{

}

void UiVerticalProgress::paintEvent(QPaintEvent *event)
{
     QStyleOption opt;
     opt.init(this);
     QPainter p(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

     float current=(currentValue-minValue)*1.0f/(maxValue-minValue);
     QPainterPath path;
     path.setFillRule(Qt::WindingFill);
     path.addRect(0,this->height()*(1-current), this->width(),this->height()*current);

     QPainter m_painter(this);
     m_painter.setRenderHint(QPainter::Antialiasing, true);

     m_painter.fillPath(path, QBrush(m_fgColor));
}


void UiVerticalProgress::onEvent(int x,int y){
    if(m_tmp_x != INT_MAX && m_tmp_y != INT_MAX){
        if(y > m_tmp_y){

            m_step = 100.0f*(y-m_tmp_y)*1.0f/(float)height();
            currentValue-=m_step;
            if(currentValue<=0){
                currentValue = 0;
            }
            update();
        }

        if(y < m_tmp_y){
             m_step = 100.0f*(m_tmp_y-y)*1.0f/(float)height();
             currentValue+=m_step;
             if(currentValue > 100){
                 currentValue = 100;
             }
             update();
        }
    }
    m_tmp_x = x;
    m_tmp_y = y;
}



