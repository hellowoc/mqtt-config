#ifndef REMOTEWIDGET_H
#define REMOTEWIDGET_H

#include "basewidget.h"

namespace Ui {
class RemoteWidget;
}

class RemoteWidget : public basewidget
{
    Q_OBJECT

public:
    explicit RemoteWidget(QWidget *parent = 0);
    ~RemoteWidget();
    void showPage(bool bshow = true);
protected:
    virtual void retranslateUiWidget();
private:
    Ui::RemoteWidget *ui;
};

#endif // REMOTEWIDGET_H
