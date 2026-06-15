#include "mytoolbutton.h"
#include <QPainter>
#include <QBitmap>
#include <QDebug>
#include <QStyleOption>
#include <QTime>
#include "global.h"
myToolButton::myToolButton(QWidget *parent):QPushButton(parent)
{
    m_color = QColor(Qt::white);
    setAttribute(Qt::WA_TranslucentBackground);
    m_btnIndex = 0;
}

void myToolButton::setPixFileStr(QString file1, QString file2,int btnindex)
{
    m_fileStrNM = file1;
    m_fileStrPress = file2;
    pix.load(file1);
    setMask(pix.mask());
    m_btnIndex = btnindex;
}

void myToolButton::mousePressEvent(QMouseEvent *event)
{
    pix.load(m_fileStrPress);
    QPushButton::mousePressEvent(event);
}

void myToolButton::mouseReleaseEvent(QMouseEvent *event)
{
    pix.load(m_fileStrNM);
    QPushButton::mouseReleaseEvent(event);
}

void myToolButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    //    painter.drawPixmap(0,0,pix.scaled(this->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    painter.drawPixmap(0,0,pix);
    painter.setPen(QPen(m_color));
    if(g_myLan().g_languageIndex != LANG_CHS)
    {
          double textWidth = fontMetrics().width(m_text);//得到文字的宽度
          if(m_btnIndex > 4)
          {
               painter.drawText(width()-textWidth-19,65,m_text);
          }
          else
          {
              painter.drawText(19,65,m_text);
          }
    }
    else
    {
        if(m_btnIndex > 4){
            double textWidth = fontMetrics().width(m_text);
            painter.drawText(width()-textWidth-40,65,m_text);
//            if(m_btnIndex == 8 || m_btnIndex == 5){
//                painter.drawText(150,65,m_text);
//            }else
//                painter.drawText(190,65,m_text);
        }
        else{
            painter.drawText(40,65,m_text);
//            if(m_btnIndex == 1 || m_btnIndex == 4){
//                painter.drawText(70,65,m_text);
//            }else
//                painter.drawText(40,65,m_text);
        }
    }
}

void myToolButton::updateText(QString text)
{
    m_text = text;
    update();
}

void myToolButton::updateRun(bool bRun)
{
    if(bRun)
    {
        pix.load(m_fileStrPress);
    }
    else
    {
        pix.load(m_fileStrNM);
    }
    update();
}
