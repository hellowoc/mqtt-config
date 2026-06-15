#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include <mymutex.h>

class myLabel : public QLabel
{
public:
    explicit myLabel();
    explicit myLabel(QString text, QRect rect=QRect(), QWidget *parent = 0);

private:
    g_Config *config;
};

#endif // MYLABEL_H
