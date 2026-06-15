#include "imageinforgbwidget.h"
#include "ui_imageinforgbwidget.h"

ImageInfoRgbWidget::ImageInfoRgbWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::ImageInfoRgbWidget)
{
    ui->setupUi(this);
    ui->widget->setDrawType(matRGB_r,3);
    ui->widget_2->setDrawType(matRGB_g,3);
    ui->widget_3->setDrawType(matRGB_b,3);
}

ImageInfoRgbWidget::~ImageInfoRgbWidget()
{
    delete ui;
}

void ImageInfoRgbWidget::showPage(bool bshow)
{
    if(bshow){
        ui->widget->update();
        ui->widget_2->update();
        ui->widget_3->update();
    }
}

void ImageInfoRgbWidget::receiveMsg(int msg1, int msg2, int msg3)
{

}

void ImageInfoRgbWidget::retranslateUiWidget()
{

}
