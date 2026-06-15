#include "mylistwidgetitem.h"

myListWidgetItem::myListWidgetItem()
{
}

myListWidgetItem::myListWidgetItem(QString text, QIcon icon, QSize size):
    QListWidgetItem()
{
//    config = g_Config::getInstance();
    setFont(QFont("Times", 20, QFont::Bold));
    setText(text);
    setIcon(icon);
    setSizeHint(size);
}
