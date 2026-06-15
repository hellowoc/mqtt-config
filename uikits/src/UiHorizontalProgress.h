#ifndef UiHorizontalProgress_H
#define UiHorizontalProgress_H
#include "UiAbstractProgress.h"
class UiHorizontalProgress : public UiAbstractProgress
{
    Q_OBJECT
public:
    explicit UiHorizontalProgress(QWidget *parent = 0);
    virtual void paintEvent(QPaintEvent *event);
    virtual void onEvent(int x,int y);
signals:

public slots:

};

#endif // UiHorizontalProgress_H
