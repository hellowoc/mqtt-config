#include "pixeldivisionshowwidget.h"
#include "ui_pixeldivisionshowwidget.h"
#include "globalflow.h"
PixelDivisionShowWidget::PixelDivisionShowWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::PixelDivisionShowWidget)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(10);
    ui->tableWidget->setColumnWidth(0,180);//起始像元
    ui->tableWidget->setColumnWidth(1,180);//结束像元
    ui->tableWidget->setColumnWidth(2,180);//中间像元
    ui->tableWidget->setColumnWidth(3,180);//起始吹嘴
    ui->tableWidget->setColumnWidth(4,180);//结束吹嘴
    ui->tableWidget->setColumnWidth(5,180);//起始像元2
    ui->tableWidget->setColumnWidth(6,180);//结束像元2
    ui->tableWidget->setColumnWidth(7,180);//起始吹嘴2
    ui->tableWidget->setColumnWidth(8,180);//结束吹嘴2
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setFocusPolicy(Qt::NoFocus);
    ui->tableWidget->setShowGrid(true);

    QHeaderView *header = ui->tableWidget->verticalHeader();
    header->setHidden(true);// 隐藏行号

    QHeaderView *horiHeader = ui->tableWidget->horizontalHeader();
    horiHeader->setFixedHeight(50);

}

PixelDivisionShowWidget::~PixelDivisionShowWidget()
{
    delete ui;
}

void PixelDivisionShowWidget::showPage(bool bshow)
{
    if(bshow){
        updatePixVec();
        updateTable();
    }
}

void PixelDivisionShowWidget::retranslateUiWidget()
{
    ui->resetAllBtn->setText(g_myLan().pixel_restore);
}

void PixelDivisionShowWidget::updateTable()
{
    for(int i = 0;i< ui->tableWidget->rowCount();i++)
        for(int j = 0;j < 10;j++){
            QTableWidgetItem *item = ui->tableWidget->takeItem(i,j);
            delete item;
        }
    QStringList list;
    list<<g_myLan().ids_pixelList<<g_myLan().ids_startPixel<<g_myLan().ids_endPixel<<g_myLan().ids_midPixel
       <<g_myLan().ejectstart<<g_myLan().ejectend<<g_myLan().ids_startPixel+"2"<<g_myLan().ids_endPixel+"2"
       <<g_myLan().ejectstart+"2"<<g_myLan().ejectend+"2";
    ui->tableWidget->setHorizontalHeaderLabels(list);
    ui->tableWidget->setRowCount(m_pixVec.size());
    ui->tableWidget->hideColumn(3);
//    ui->tableWidget->hideColumn(6);
//    ui->tableWidget->hideColumn(7);
//    ui->tableWidget->hideColumn(8);
//    ui->tableWidget->hideColumn(9);
    for(int i = 0;i< m_pixVec.size();++i){
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(m_pixVec.at(i).cameraNamestr));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(m_pixVec.at(i).startstr));
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(m_pixVec.at(i).endstr));
        ui->tableWidget->setItem(i,3,new QTableWidgetItem(m_pixVec.at(i).midstr));
        ui->tableWidget->setItem(i,4,new QTableWidgetItem(m_pixVec.at(i).starteject));
        ui->tableWidget->setItem(i,5,new QTableWidgetItem(m_pixVec.at(i).endeject));
        ui->tableWidget->setItem(i,6,new QTableWidgetItem(m_pixVec.at(i).startstr2));
        ui->tableWidget->setItem(i,7,new QTableWidgetItem(m_pixVec.at(i).endstr2));
        ui->tableWidget->setItem(i,8,new QTableWidgetItem(m_pixVec.at(i).starteject2));
        ui->tableWidget->setItem(i,9,new QTableWidgetItem(m_pixVec.at(i).endeject2));
        for(int j= 0;j < 10;j++){
            ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            ui->tableWidget->item(i,j)->setFlags(Qt::ItemIsEnabled);
        }
    }
}

void PixelDivisionShowWidget::updatePixVec()
{
    m_pixVec.clear();
    PixelDivision pixel;
    for(int i = 0;i < struCnfg.nLayerTotal;i++){
        for(int j = 0;j < struCnfg.struLayerInfo[i].nViewChainTotal;j++){
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            for(int k = 0;k < struCnfg.struLayerInfo[i].nViewUnitTotal[j];k++){
                pixel.cameraNamestr = myFlow.getUnitShowName(j,k);
                if(struCnfg.nLayerTotal > 1){
                    pixel.cameraNamestr = QString("%1 %2 ").arg(g_myLan().layer).arg(i) +  pixel.cameraNamestr;
                }
                pixel.startstr = QString::number(struCnfg.struLayerInfo[i].stuCameraInfo[j].nChannelBegin[k]);
                pixel.endstr = QString::number(struCnfg.struLayerInfo[i].stuCameraInfo[j].nChannelEnd[k]);
                pixel.startstr2 = QString::number(struCnfg.struLayerInfo[i].stuCameraInfo[j].nChannelBegin2[k]);
                pixel.endstr2 = QString::number(struCnfg.struLayerInfo[i].stuCameraInfo[j].nChannelEnd2[k]);
                pixel.midstr = "0";
                pixel.starteject = QString::number(struCnfg.struLayerInfo[i].stuCameraInfo[j].nEjectBegin[k]);
                pixel.endeject = QString::number(struCnfg.struLayerInfo[i].stuCameraInfo[j].nEjectEnd[k]);
                pixel.starteject2 = QString::number(struCnfg.struLayerInfo[i].stuCameraInfo[j].nEjectBegin2[k]);
                pixel.endeject2 = QString::number(struCnfg.struLayerInfo[i].stuCameraInfo[j].nEjectEnd2[k]);
                m_pixVec.append(pixel);
            }
        }
    }
}

void PixelDivisionShowWidget::on_resetAllBtn_clicked()
{
    g_infoWidget().setLabelText(g_myLan().ids_apply);
    g_infoWidget().delayShow();
    myFlow.resetCameraPixelRG(true,struGsh.nLayer,struGsh.nView,struGsh.nUnit);

    myFlow.sleep(1);
    if ((struCnfg.nDPMachine == 1)&&(struCnfp.nIsCameraConpress == 0))
    {
        myFlow.configIpcCamera();
    }
    g_infoWidget().hide();
}
