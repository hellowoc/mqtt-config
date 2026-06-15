#include "simulatewidget.h"
#include "ui_simulatewidget.h"
SimulateWidget::SimulateWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::SimulateWidget)
{
    ui->setupUi(this);
    m_imageIndex = 0;
    ui->listWidget->setMinimumHeight(90);
    ui->listWidget->setIconSize(QSize(188,90));
    ui->listWidget->setViewMode(QListView::IconMode);
    ui->listWidget->setMovement(QListView::Static);
    ui->listWidget->setFlow(QListView::LeftToRight);
    ui->widget->setWidgetType(simulateType);
}

SimulateWidget::~SimulateWidget()
{
    delete ui;
}

void SimulateWidget::showPage(bool bshow)
{
    if(bshow){
        g_Runtime().updateTmpImgList();
        updateImageList();
        if(m_imageIndex < g_Runtime().nimageNameVec.count()){
            updateCurrentImage(m_imageIndex,true);
        }
        connect(ui->listWidget,SIGNAL(currentRowChanged(int)),this,SLOT(imageListItemSelect(int)));
    }else{
        disconnect(ui->listWidget,SIGNAL(currentRowChanged(int)),this,SLOT(imageListItemSelect(int)));//清空list前需要断开信号槽连接
        while (ui->listWidget->count()>0) {
           QListWidgetItem *item = ui->listWidget->takeItem(0);
           if(item){
               delete item;
           }
        }
    }
}

void SimulateWidget::receiveMsg(int msg1, int msg2, int msg3)
{

}

void SimulateWidget::retranslateUiWidget()
{
    ui->widget->retranslateUiWidget();
}


void SimulateWidget::updateCurrentImage(int row,bool show)
{
    ui->listWidget->setCurrentRow(row);
    QImage  image;
    int layer =0,view= 0,unit = 0;
    g_Runtime().getCurImageLayerAndView(layer,view,unit,row);
    QString imagePath = g_Runtime().nimageNameVec[row];
    if (imagePath.contains("png")){
        if(image.load(imagePath)){
            ui->widget->setImage(image,layer,view,unit);
            if(show)
                ui->widget->showPage();
            else
                ui->widget->updateImage();
        }
    }
    else{
        ui->widget->setImage(imagePath,layer,view,unit);
        if(show)
            ui->widget->showPage();
        else
            ui->widget->updateImage();
    }
}

void SimulateWidget::imageListItemSelect(int row)
{
    m_imageIndex = row;
    updateCurrentImage(row);
}


void SimulateWidget::updateImageList()
{
    int i =0;
    if(!g_Runtime().nimageNameVec.isEmpty()){
        while (i < g_Runtime().nimageNameVec.count()) {
            bool bIsImg = true;
            QImage  image;
#ifdef Q_OS_UNIX
            QString imagePath = g_Runtime().nimageNameVec[i]+"_copy";
#else
            QString imagePath = g_Runtime().nimageNameVec[i];
#endif
            bIsImg = image.load(imagePath);
            if(!bIsImg){
                bIsImg = image.load(g_Runtime().nimageNameVec[i],"bmp");
            }
            QString name = myFlow.getImageShowName(imagePath);
            if (bIsImg) {
                image = image.scaled(146,57);
                QListWidgetItem *item = new QListWidgetItem(QPixmap::fromImage(image),name,ui->listWidget,i);
                item->setTextAlignment(Qt::AlignHCenter);
            }
            i++;
        }
    }

}
