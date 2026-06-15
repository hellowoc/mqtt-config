#ifndef IROFFSETWIDGET_H
#define IROFFSETWIDGET_H

#include "basewidget.h"

namespace Ui {
class IrOffsetWidget;
}

class IrOffsetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit IrOffsetWidget(QWidget *parent = 0);
    ~IrOffsetWidget();
    void showPage(bool bshow);
    void receiveMsg(int msg1, int msg2, int msg3);
protected:
    void retranslateUiWidget();


private:
    Ui::IrOffsetWidget *ui;
};

#endif // IROFFSETWIDGET_H
