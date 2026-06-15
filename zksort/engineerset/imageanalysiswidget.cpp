#include "imageanalysiswidget.h"
#include "ui_imageanalysiswidget.h"
#include "global.h"
ImageAnalysisWidget::ImageAnalysisWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::ImageAnalysisWidget)
{
    ui->setupUi(this);
}

ImageAnalysisWidget::~ImageAnalysisWidget()
{
    delete ui;
}

void ImageAnalysisWidget::showPage(bool bshow)
{
    if(bshow){
        ui->widget->setWidgetType(analyzeType);

        int layer,view,nUnit;
        g_Runtime().getCurImageLayerAndView(layer,view,nUnit,struGsh.nImageIndex);

        if(g_Runtime().nimageNameVec[struGsh.nImageIndex].contains("png")){ //兼容png图片
            QImage image;
            image.load(g_Runtime().nimageNameVec[struGsh.nImageIndex],"png");
            ui->widget->setImage(image,layer,view,nUnit);
        }
        else //适用bmp图片
            ui->widget->setImage(g_Runtime().nimageNameVec[struGsh.nImageIndex],layer,view,nUnit);

        ui->widget->showPage(true);

    }else{
        ui->widget->showPage(false);
    }
}

void ImageAnalysisWidget::receiveMsg(int msg1, int msg2, int msg3)
{

}

void ImageAnalysisWidget::retranslateUiWidget()
{
    ui->widget->retranslateUiWidget();
}
