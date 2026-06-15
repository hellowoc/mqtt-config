#ifndef IMAGEINFORGBWIDGET_H
#define IMAGEINFORGBWIDGET_H

#include "basewidget.h"

namespace Ui {
class ImageInfoRgbWidget;
}

class ImageInfoRgbWidget : public basewidget
{
    Q_OBJECT

public:
    explicit ImageInfoRgbWidget(QWidget *parent = 0);
    ~ImageInfoRgbWidget();

    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
    void retranslateUiWidget();
private:
    Ui::ImageInfoRgbWidget *ui;
};

#endif // IMAGEINFORGBWIDGET_H
