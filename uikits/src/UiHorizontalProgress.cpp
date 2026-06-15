#include "UiHorizontalProgress.h"

UiHorizontalProgress::UiHorizontalProgress(QWidget *parent) :
    UiAbstractProgress(parent)
{
}

void UiHorizontalProgress::paintEvent(QPaintEvent *event)
{
     QStyleOption opt;
     opt.init(this);
     QPainter p(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

     float current=(currentValue-minValue)*1.0f/(maxValue-minValue);
//     float current = currentValue/100.0;

     QPainterPath path;
     path.setFillRule(Qt::WindingFill);
     path.addRect(0,1,this->width()*current-1,this->height()-2);

     QPainter m_painter(this);
     m_painter.setRenderHint(QPainter::Antialiasing, true);
     m_painter.fillPath(path, QBrush(m_fgColor));
}


void UiHorizontalProgress::onEvent(int x,int y){
    if(m_tmp_x != INT_MAX && m_tmp_y != INT_MAX){
        if(x > m_tmp_x){
            m_step = maxValue*(x-m_tmp_x)*1.0f/width();
            currentValue+=m_step;
            if(currentValue > maxValue){
                currentValue = maxValue;
            }
            update();
        }

        if(x < m_tmp_x){
             m_step = maxValue*(m_tmp_x-x)*1.0f/width();
             currentValue-=m_step;
             if(currentValue<=minValue){
                 currentValue = minValue;
             }
             update();
        }
    }
    m_tmp_x = x;
    m_tmp_y = y;
}

