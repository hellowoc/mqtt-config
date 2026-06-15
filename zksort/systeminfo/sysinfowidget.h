#ifndef SYSINFOWIDGET_H
#define SYSINFOWIDGET_H

#include "basewidget.h"

namespace Ui {
class SysInfoWidget;
}
class SysInfoWidget : public basewidget
{
    Q_OBJECT

public:
    explicit SysInfoWidget(QWidget *parent = 0);
    ~SysInfoWidget();
    void showPage(bool bshow);
    void retranslateUiWidget();

protected slots:

private:
    Ui::SysInfoWidget *ui;

};

#endif // SYSINFOWIDGET_H
