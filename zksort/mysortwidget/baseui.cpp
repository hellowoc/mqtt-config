#include "baseui.h"
#include <QEvent>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>
BaseUi::BaseUi(QWidget *parent) : QWidget(parent)
{

}

void BaseUi::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    switch(event->type()){
    case QEvent::LanguageChange:
        retranslateUiWidget();
        break;
    default:
        break;
    }
}

void BaseUi::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
