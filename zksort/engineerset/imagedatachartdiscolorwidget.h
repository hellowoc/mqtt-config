#ifndef IMAGEDATACHARTDISCOLORWIDGET_H
#define IMAGEDATACHARTDISCOLORWIDGET_H

#include "basewidget.h"

namespace Ui {
class ImageDataChartDiscolorWidget;
}

class ImageDataChartDiscolorWidget : public basewidget
{
    Q_OBJECT

public:
    explicit ImageDataChartDiscolorWidget(QWidget *parent = 0);
    ~ImageDataChartDiscolorWidget();

    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
    void retranslateUiWidget();
private:
    Ui::ImageDataChartDiscolorWidget *ui;
};

#endif // IMAGEDATACHARTDISCOLORWIDGET_H
