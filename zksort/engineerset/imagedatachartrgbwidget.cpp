#include "imagedatachartrgbwidget.h"
#include "ui_imagedatachartrgbwidget.h"

ImageDataChartRgbWidget::ImageDataChartRgbWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::ImageDataChartRgbWidget)
{
    ui->setupUi(this);
    ui->widget->setDrawType(rgb_r,0);
    ui->widget_2->setDrawType(rgb_g,0);
    ui->widget_3->setDrawType(rgb_b,0);
    ui->widget_4->setDrawType(rgb_r,1);
    ui->widget_5->setDrawType(rgb_g,1);
    ui->widget_6->setDrawType(rgb_b,1);
}

ImageDataChartRgbWidget::~ImageDataChartRgbWidget()
{
    delete ui;
}

void ImageDataChartRgbWidget::showPage(bool bshow)
{
    if(bshow){
        ui->widget->update();
        ui->widget_2->update();
        ui->widget_3->update();
        ui->widget_4->update();
        ui->widget_5->update();
        ui->widget_6->update();
    }else{
    }
}

void ImageDataChartRgbWidget::receiveMsg(int msg1, int msg2, int msg3)
{

}

void ImageDataChartRgbWidget::retranslateUiWidget()
{
}
