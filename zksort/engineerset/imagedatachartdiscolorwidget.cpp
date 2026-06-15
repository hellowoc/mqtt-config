#include "imagedatachartdiscolorwidget.h"
#include "ui_imagedatachartdiscolorwidget.h"

ImageDataChartDiscolorWidget::ImageDataChartDiscolorWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::ImageDataChartDiscolorWidget)
{
    ui->setupUi(this);
    ui->widget->setDrawType(discolor_rg,0);
    ui->widget_2->setDrawType(discolor_rb,0);
    ui->widget_3->setDrawType(discolor_gb,0);
    ui->widget_4->setDrawType(discolor_rg,1);
    ui->widget_5->setDrawType(discolor_rb,1);
    ui->widget_6->setDrawType(discolor_gb,1);

}

ImageDataChartDiscolorWidget::~ImageDataChartDiscolorWidget()
{
    delete ui;
}

void ImageDataChartDiscolorWidget::showPage(bool bshow)
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

void ImageDataChartDiscolorWidget::receiveMsg(int msg1, int msg2, int msg3)
{

}

void ImageDataChartDiscolorWidget::retranslateUiWidget()
{
}
