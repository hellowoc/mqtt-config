#include "iroffsetwidget.h"
#include "ui_iroffsetwidget.h"

IrOffsetWidget::IrOffsetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::IrOffsetWidget)
{
    ui->setupUi(this);
}

IrOffsetWidget::~IrOffsetWidget()
{
    delete ui;
}

void IrOffsetWidget::showPage(bool bshow)
{
    if(bshow){
        ui->widget->setWidgetType(analyzeType);

        int layer,view,nUnit;
        QString m_curFilePath = "";

        g_Runtime().updateTmpImgList();
        if(!g_Runtime().nimageNameVec.isEmpty())
            m_curFilePath = g_Runtime().nimageNameVec[0];
        if(!m_curFilePath.isEmpty()){
            if (m_curFilePath.contains("png")){ //compatible with old png
                QImage img(myFlow.getSensorPixels(struGsh.nLayer,struGsh.nView),myFlow.imgHeight,QImage::Format_ARGB32);
                img.load(m_curFilePath);
                ui->widget->setImage(img,struGsh.nLayer,struGsh.nView,struGsh.nUnit);
            }
            else //bmp all here
                ui->widget->setImage(m_curFilePath,struGsh.nLayer,struGsh.nView,struGsh.nUnit);
        }
        else{
            QImage img(myFlow.getSensorPixels(struGsh.nLayer,struGsh.nView),myFlow.imgHeight,QImage::Format_ARGB32);
            img.fill(Qt::white);
            ui->widget->setImage(img,struGsh.nLayer,struGsh.nView,struGsh.nUnit);
        }

        struGsh.mIrOffsetEnable = true;

//        g_Runtime().getCurImageLayerAndView(layer,view,nUnit,struGsh.nImageIndex);

//        if(g_Runtime().nimageNameVec[struGsh.nImageIndex].contains("png")){ //兼容png图片
//            QImage image;
//            image.load(g_Runtime().nimageNameVec[struGsh.nImageIndex],"png");
//            ui->widget->setImage(image,layer,view,nUnit);
//        }
//        else //适用bmp图片
//            ui->widget->setImage(g_Runtime().nimageNameVec[struGsh.nImageIndex],layer,view,nUnit);

        ui->widget->showPage(true);

    }else{
        struGsh.mIrOffsetEnable = false;
        ui->widget->showPage(false);

    }
}

void IrOffsetWidget::receiveMsg(int msg1, int msg2, int msg3)
{

}

void IrOffsetWidget::retranslateUiWidget()
{
    ui->widget->retranslateUiWidget();
}
