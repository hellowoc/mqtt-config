#ifndef MYLISTWIDGETITEM_H
#define MYLISTWIDGETITEM_H

#include <QListWidgetItem>

class myListWidgetItem : public QListWidgetItem
{
public:
    explicit myListWidgetItem();
    explicit myListWidgetItem(QString text,QIcon icon,QSize size);

private:
//    g_Config *config;
};

#endif // MYLISTWIDGETITEM_H
