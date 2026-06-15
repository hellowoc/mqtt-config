#ifndef IMAGEANALYSISWIDGET_H
#define IMAGEANALYSISWIDGET_H

#include "basewidget.h"

namespace Ui {
class ImageAnalysisWidget;
}

class ImageAnalysisWidget : public basewidget
{
    Q_OBJECT

public:
    explicit ImageAnalysisWidget(QWidget *parent = 0);
    ~ImageAnalysisWidget();
    void showPage(bool bshow);
    void receiveMsg(int msg1, int msg2, int msg3);
protected:
    void retranslateUiWidget();
private:
    Ui::ImageAnalysisWidget *ui;
};

#endif // IMAGEANALYSISWIDGET_H
