#include "mylabel.h"

myLabel::myLabel()
{
    config = g_Config::getInstance();
    setFont(config->getFont());
}

myLabel::myLabel(QString text, QRect rect, QWidget *parent):
    QLabel(parent)
{
    config = g_Config::getInstance();
    setFont(config->getFont());
    setText(text);
    setGeometry(rect);
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setSizePolicy(sizePolicy);
}
