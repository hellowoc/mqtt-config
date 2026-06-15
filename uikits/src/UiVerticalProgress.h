#ifndef THUIPROGRESS_H
#define THUIPROGRESS_H

#include <QtGui>
#include "UiAbstractProgress.h"


class UiVerticalProgress : public UiAbstractProgress
{
    Q_OBJECT
public:
    explicit UiVerticalProgress(QWidget *parent = 0);
    virtual void paintEvent(QPaintEvent *event);
    virtual void onEvent(int x,int y);



signals:

public slots:

};

#endif // THUIPROGRESS_H
