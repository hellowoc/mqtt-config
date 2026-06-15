#ifndef DCCRYPTWIDGET_H
#define DCCRYPTWIDGET_H

#include "basewidget.h"
#include "time_encrypt.h"

namespace Ui {
class DccryptWidget;
}

class DccryptWidget : public basewidget
{
    Q_OBJECT

public:
    explicit DccryptWidget(QWidget *parent = 0);
    ~DccryptWidget();
    void showPage(bool bshow = true);
protected:
    void retranslateUiWidget();
private slots:
    void on_DccryptBtn_clicked();

private:
    Ui::DccryptWidget *ui;
};

#endif // DCCRYPTWIDGET_H
