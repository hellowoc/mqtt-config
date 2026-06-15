#include "imagedatachartwidget.h"
#include "ui_imagedatachartwidget.h"

ImageDataChartWidget::ImageDataChartWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::ImageDataChartWidget)
{
    ui->setupUi(this);
    ui->widget->setDrawType(hsv_h,0);
    ui->widget_2->setDrawType(hsv_s,0);
    ui->widget_3->setDrawType(hsv_v,0);
    ui->widget_4->setDrawType(hsv_h,1);
    ui->widget_5->setDrawType(hsv_s,1);
    ui->widget_6->setDrawType(hsv_v,1);
}

ImageDataChartWidget::~ImageDataChartWidget()
{
    delete ui;
}

void ImageDataChartWidget::showPage(bool bshow)
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

void ImageDataChartWidget::receiveMsg(int msg1, int msg2, int msg3)
{

}

void ImageDataChartWidget::retranslateUiWidget()
{
}

