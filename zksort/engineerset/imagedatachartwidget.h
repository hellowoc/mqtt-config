#ifndef IMAGEDATACHARTWIDGET_H
#define IMAGEDATACHARTWIDGET_H

#include "basewidget.h"

namespace Ui {
class ImageDataChartWidget;
}

class ImageDataChartWidget : public basewidget
{
    Q_OBJECT

public:
    explicit ImageDataChartWidget(QWidget *parent = 0);
    ~ImageDataChartWidget();

    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
    void retranslateUiWidget();
private:
    Ui::ImageDataChartWidget *ui;
};

#endif // IMAGEDATACHARTWIDGET_H
