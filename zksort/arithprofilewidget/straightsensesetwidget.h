#ifndef STRAIGHTSENSESETWIDGET_H
#define STRAIGHTSENSESETWIDGET_H

#include "basewidget.h"

namespace Ui {
class StraightSenseSetWidget;
}

class StraightSenseSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit StraightSenseSetWidget(QWidget *parent = 0);
    ~StraightSenseSetWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);


protected:
    void retranslateUiWidget();

private:
    Ui::StraightSenseSetWidget *ui;
};

#endif // STRAIGHTSENSESETWIDGET_H
