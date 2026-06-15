#ifndef PLCCTRLWIDGET_H
#define PLCCTRLWIDGET_H

#include "basewidget.h"

namespace Ui {
class PlcCtrlWidget;
}

class PlcCtrlWidget : public basewidget
{
    Q_OBJECT

public:
    explicit PlcCtrlWidget(QWidget *parent = 0);
    ~PlcCtrlWidget();

    void showPage(bool bshow);
    void retranslateUiWidget();
    void receiveMsg(int msg1, int msg2, int msg3);

private slots:
    void aliveStatusUpdate();

private:
    Ui::PlcCtrlWidget *ui;
    QTimer *timer;

};

#endif // PLCCTRLWIDGET_H

