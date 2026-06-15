#include "lightsetwidget.h"
#include "ui_lightsetwidget.h"
LightSetWidget::LightSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::LightSetWidget)
{
    ui->setupUi(this);
    m_frontLamp[0] = ui->widget_1;
    m_frontLamp[1] = ui->widget_2;
    m_frontLamp[2] = ui->widget_3;
    m_frontLamp[3] = ui->widget_4;
    m_frontLamp[4] = ui->widget_5;
    m_frontLamp[5] = ui->widget_6;
    m_frontBackLamp[0] = ui->widget_7;
    m_frontBackLamp[1] = ui->widget_8;
    m_frontBackLamp[2] = ui->widget_9;
    m_frontBackLamp[3] = ui->widget_10;


    m_rearLamp[0] = ui->widget_1_1;
    m_rearLamp[1] = ui->widget_1_2;
    m_rearLamp[2] = ui->widget_1_3;
    m_rearLamp[3] = ui->widget_1_4;
    m_rearLamp[4] = ui->widget_1_5;
    m_rearLamp[5] = ui->widget_1_6;
    m_rearBackLamp[0] = ui->widget_1_7;
    m_rearBackLamp[1] = ui->widget_1_8;
    m_rearBackLamp[2] = ui->widget_1_9;
    m_rearBackLamp[3] = ui->widget_1_10;

    m_pLayerBtn[0] = ui->mainBtn;
    m_pLayerBtn[1] = ui->viceBtn;
    setSlotConnect();
}

LightSetWidget::~LightSetWidget()
{
    delete ui;
}

void LightSetWidget::retranslateUiWidget()
{
    ui->frontlabel->setText(g_myLan().front);
    ui->rearlabel->setText(g_myLan().rear);
    ui->groupBox->setTitle(g_myLan().ids_materialLight);
    ui->groupBox_2->setTitle(g_myLan().ids_materialLight);
    ui->groupBox_3->setTitle(g_myLan().ids_bkgLight);
    ui->groupBox_4->setTitle(g_myLan().ids_bkgLight);
}

void LightSetWidget::showPage(bool bshow)
{
    if(bshow){
        updateLayerBtnState();
        updateFrontRearState();
    }
}

void LightSetWidget::OnLayerChange()
{
   updateFrontRearState();
}

void LightSetWidget::updateFrontRearState()
{
     m_frontMaterialLampCount =0;
     m_frontBackLampCount = 0;

     QString name;
     QStringList list;
     list<<g_myLan().ids_red<<g_myLan().ids_green<<g_myLan().ids_blue<<g_myLan().ids_n;
     getLampCount();
     qDebug()<<n_fBackLampCnt<<n_rBackLampCnt<<n_fMaterialLampCnt<<n_rMaterialLampCnt;

     m_rearMaterialLampCount = 0;
     m_rearBackLampCount = 0;
     bool bHasFront = false,bHasRear = false;
     for(int i = 0;i< struCnfg.stuLayerLampCfg.nLampBoardTotal;i++){
         for(int j = 0;j < MAX_LAMP_PER_BOARD;j++){
             if(struCnfg.stuLayerLampCfg.nLampLayerAddr[i][j] == 0){//只处理设置为0层的
                 if(struCnfg.stuLayerLampCfg.nLampPortEnable[i][j]){//used
                     name = struCnfg.stuLayerLampCfg.sLampName[i][j];
                     if(struCnfg.stuLayerLampCfg.nLampViewAddr[i][j] == 0){//front
                         if(struCnfg.stuLayerLampCfg.nLampPortType[i][j] == 0){//material
                             if(m_frontMaterialLampCount >= MAX_LAMP_PER_BOARD){
                                 logger()->error("front lamp material set out of index");
                                 return;
                             }
                             if(name == "")
                             {
                                 name = QString::number(m_frontMaterialLampCount+1);
                             }
                             m_frontLamp[m_frontMaterialLampCount]->initState(i,j,name,struCnfp.nLampValue[0][i][j],struCnfp.nLampEnable[0][i][j]);
                             m_frontLamp[m_frontMaterialLampCount]->show();
                             m_frontMaterialLampCount++;
                         }else{//background
                             if(m_frontBackLampCount >= 4){
                                 logger()->error("front lamp background set out of index");
                                 return;
                             }
                             if(name == "")
                             {
                                 name = QString::number(m_frontBackLampCount+1);
                             }
                             m_frontBackLamp[m_frontBackLampCount]->initState(i,j,name,struCnfp.nLampValue[0][i][j],struCnfp.nLampEnable[0][i][j]);
                             m_frontBackLamp[m_frontBackLampCount]->show();
                             m_frontBackLampCount++;
                         }

                         bHasFront = true;
                     }else if(struCnfg.stuLayerLampCfg.nLampViewAddr[i][j] == 1){//rear
                         if(struCnfg.stuLayerLampCfg.nLampPortType[i][j] == 0){//material
                             if(m_rearMaterialLampCount >= MAX_LAMP_PER_BOARD){
                                 logger()->error("rear lamp material set out of index");
                                 return;
                             }
                             if(name == "")
                             {
                                 name = QString::number(m_rearMaterialLampCount+1);
                             }
                             m_rearLamp[m_rearMaterialLampCount]->initState(i,j,name,struCnfp.nLampValue[0][i][j],struCnfp.nLampEnable[0][i][j]);
                             m_rearLamp[m_rearMaterialLampCount]->show();
                             m_rearMaterialLampCount++;
                         }else{//background
                             if(m_rearBackLampCount >= 4){
                                 logger()->error("rear lamp background set out of index");
                                 return;
                             }
                             if(name == "")
                             {
                                 name = QString::number(m_rearBackLampCount+1);
                             }
                             m_rearBackLamp[m_rearBackLampCount]->initState(i,j,name,struCnfp.nLampValue[0][i][j],struCnfp.nLampEnable[0][i][j]);
                             m_rearBackLamp[m_rearBackLampCount]->show();
                             m_rearBackLampCount++;
                         }
                         bHasRear = true;
                     }
                 }
             }
         }
     }
     ui->frontlabel->setVisible(bHasFront);
     ui->rearlabel->setVisible(bHasRear);

     if(struCnfg.stuLayerLampCfg.nLampBoardTotal == 1 && struCnfg.nAllInOne)
     {
        ui->frontlabel->hide();
        ui->rearlabel->hide();
        ui->groupBox_2->hide();
        ui->groupBox_4->hide();
     }
     else
     {
         ui->groupBox_2->show();
         ui->groupBox_4->show();
     }

     updateFrontLampState();

     updateRearLampState();

}

void LightSetWidget::updateFrontLampState()
{
    for(int i = 0;i < MAX_LAMP_PER_BOARD;i++){
        if(i >= m_frontMaterialLampCount){
            m_frontLamp[i]->hide();
        }
    }

    for(int i = 0;i < 4;i++){
        if(i >= m_frontBackLampCount){
            m_frontBackLamp[i]->hide();
        }
    }
}

void LightSetWidget::updateRearLampState()
{
    for(int i = 0;i < MAX_LAMP_PER_BOARD;i++){
        if( i >= m_rearMaterialLampCount){
            m_rearLamp[i]->hide();
        }
    }

    for(int i = 0;i < 4;i++){
        if(i >= m_rearBackLampCount){
            m_rearBackLamp[i]->hide();
        }
    }
}
void LightSetWidget::getLampCount()
{
    n_fMaterialLampCnt = 0;
    n_fBackLampCnt = 0;
    n_rMaterialLampCnt = 0;
    n_rBackLampCnt = 0;
    for(int i = 0;i< struCnfg.stuLayerLampCfg.nLampBoardTotal;i++){
        for(int j = 0;j < MAX_LAMP_PER_BOARD;j++){
            if(struCnfg.stuLayerLampCfg.nLampLayerAddr[i][j] == 0){//只处理设置为0层的
                if(struCnfg.stuLayerLampCfg.nLampPortEnable[i][j]){//used
                    if(struCnfg.stuLayerLampCfg.nLampViewAddr[i][j] == 0){//front
                        if(struCnfg.stuLayerLampCfg.nLampPortType[i][j] == 0){//material
                            n_fMaterialLampCnt++;
                        }else{//background
                            n_fBackLampCnt++;
                        }
                    }else if(struCnfg.stuLayerLampCfg.nLampViewAddr[i][j] == 1){//rear
                        if(struCnfg.stuLayerLampCfg.nLampPortType[i][j] == 0){//material
                            n_rMaterialLampCnt++;
                        }else{//background
                            n_rBackLampCnt++;
                        }
                    }
                }
            }
        }
    }
}
