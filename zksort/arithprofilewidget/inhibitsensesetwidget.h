#ifndef INHIBITSENSESETWIDGET_H
#define INHIBITSENSESETWIDGET_H

#include "basewidget.h"

namespace Ui {
class InhibitSensesetWidget;
}

class InhibitSensesetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit InhibitSensesetWidget(QWidget *parent = 0);
    ~InhibitSensesetWidget();

    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);


protected:
    void retranslateUiWidget();
private:
    Ui::InhibitSensesetWidget *ui;
};

#endif // INHIBITSENSESETWIDGET_H
