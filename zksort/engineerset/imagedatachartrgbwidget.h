#ifndef IMAGEDATACHARTRGBWIDGET_H
#define IMAGEDATACHARTRGBWIDGET_H

#include "basewidget.h"

namespace Ui {
class ImageDataChartRgbWidget;
}

class ImageDataChartRgbWidget : public basewidget
{
    Q_OBJECT

public:
    explicit ImageDataChartRgbWidget(QWidget *parent = 0);
    ~ImageDataChartRgbWidget();

    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
    void retranslateUiWidget();
private:
    Ui::ImageDataChartRgbWidget *ui;
};

#endif // IMAGEDATACHARTRGBWIDGET_H
