#include "doublelightsetwidget.h"
#include "ui_doublelightsetwidget.h"

DoubleLightSetWidget::DoubleLightSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::DoubleLightSetWidget)
{
    ui->setupUi(this);

    m_pLightBoardBtn[0] = ui->pushButton_0;
    m_pLightBoardBtn[1] = ui->pushButton_1;
    m_pLightBoardBtn[2] = ui->pushButton_2;
    m_pLightBoardBtn[3] = ui->pushButton_3;
    m_pLightBoardBtn[4] = ui->pushButton_4;
    m_pLightBoardBtn[5] = ui->pushButton_5;
//    m_pLightBoardBtn[6] = ui->pushButton_6;
//    m_pLightBoardBtn[7] = ui->pushButton_7;
    ui->pushButton_6->hide();
    ui->pushButton_7->hide();
    m_pLightBoardBtn[6] = ui->pushButton_allInOne;
    for(int i = 0;i < MAX_LAMP_BOARD_PER_INTERFACE-2;i++){
        m_pLightBoardBtn[i]->setCheckable(true);
        connect(m_pLightBoardBtn[i],SIGNAL(clicked()),this,SLOT(lightBoardBtnClicked()));
    }
    m_pLightBoardBtn[MAX_LAMP_BOARD_PER_INTERFACE-2]->setCheckable(true);
    m_lightBoardIndex = 0;
    m_materialLamp[0] = ui->widget_1_1;
    m_materialLamp[1] = ui->widget_1_2;
    m_materialLamp[2] = ui->widget_1_3;
    m_materialLamp[3] = ui->widget_1_4;
    m_materialLamp[4] = ui->widget_1_5;
    m_materialLamp[5] = ui->widget_1_6;
    m_materialLamp[6] = ui->widget_1_7;
    m_materialLamp[7] = ui->widget_1_8;
//    m_materialLamp[8] = ui->widget_1_9;
//    m_materialLamp[9] = ui->widget_1_10;

    m_BackLamp[0] = ui->widget_1_11;
    m_BackLamp[1] = ui->widget_1_12;
    m_BackLamp[2] = ui->widget_1_13;
    m_BackLamp[3] = ui->widget_1_14;
    m_BackLamp[4] = ui->widget_1_15;
    m_BackLamp[5] = ui->widget_1_16;
    m_BackLamp[6] = ui->widget_1_17;
    m_BackLamp[7] = ui->widget_1_18;
//    m_BackLamp[8] = ui->widget_1_19;
//    m_BackLamp[9] = ui->widget_1_20;
    ui->widget_1_9->hide();
    ui->widget_1_10->hide();
    ui->widget_1_19->hide();
    ui->widget_1_20->hide();

    setSlotConnect();

    shottimer = new QTimer;
    connect(shottimer,SIGNAL(timeout()),this, SLOT(getscreenshot()));


}

DoubleLightSetWidget::~DoubleLightSetWidget()
{
    delete ui;
}

void DoubleLightSetWidget::retranslateUiWidget()
{
    ui->groupBox_2->setTitle(g_myLan().ids_materialLight);
    ui->groupBox_4->setTitle(g_myLan().ids_bkgLight);
    ui->pushButton_allInOne->setText(g_myLan().ids_allInOne);
}

void DoubleLightSetWidget::showPage(bool bshow)
{
    basewidget::showPage(bshow);
    if(bshow){
        updateLightBoardBtn();
        updateState();

        if(struGsh.bScreenShotStatus == 1)
           {
               shottimer->start(1000);

           }
    }
}


void DoubleLightSetWidget::getscreenshot()
{
    shottimer->stop();
    {

        for(int i = 0;i < MAX_LAMP_BOARD_PER_INTERFACE-2;i++)
        {
            if(i < struCnfg.stuLayerLampCfg.nLampBoardTotal){

                QMetaObject::invokeMethod(m_pLightBoardBtn[i],"clicked", Qt::QueuedConnection);

                myFlow.sleep(1);
                myFlow.saveScreenshot();
            }


        }

        myFlow.sleep(1);

        struGsh.bScreenShotStatus = 0;
    }
}

void DoubleLightSetWidget::lightBoardBtnClicked()
{
    for(int i=0; i<MAX_LAMP_BOARD_PER_INTERFACE; i++ )
        if(sender() == m_pLightBoardBtn[i])
        {
            m_pLightBoardBtn[m_lightBoardIndex]->setChecked(false);
            m_lightBoardIndex = i;
            m_pLightBoardBtn[m_lightBoardIndex]->setChecked(true);
            break;
        }
    updateState();
}
void DoubleLightSetWidget::updateState()
{
     m_MaterialLampCount =0;
     m_BackLampCount = 0;

     QString lampPortName;
     QStringList list;
     list<<g_myLan().ids_red<<g_myLan().ids_green<<g_myLan().ids_blue<<g_myLan().ids_n;
     getLampCount();

     for(int j = 0;j < MAX_LAMP_PER_BOARD;j++){
         if(struCnfg.stuLayerLampCfg.nLampPortEnable[m_lightBoardIndex][j]){//used
             lampPortName = struCnfg.stuLayerLampCfg.sLampName[m_lightBoardIndex][j];
             if(struCnfg.stuLayerLampCfg.nLampPortType[m_lightBoardIndex][j] == 0){//material
                 if(m_MaterialLampCount >= MAX_LAMP_PER_BOARD){
                     logger()->error("lamp material set out of index");
                     return;
                 }
                 if(lampPortName == "")
                 {
                     lampPortName = QString::number(m_MaterialLampCount+1);
                 }
                 m_materialLamp[m_MaterialLampCount]->initState(m_lightBoardIndex,j,lampPortName\
                                                                    ,struCnfp.nLampValue[0][m_lightBoardIndex][j],struCnfp.nLampEnable[0][m_lightBoardIndex][j]);
                 m_materialLamp[m_MaterialLampCount]->initSeq(m_lightBoardIndex,j\
                                                                    ,struCnfp.nLampSeq[0][m_lightBoardIndex][j]);

                 m_materialLamp[m_MaterialLampCount]->show();
                 m_MaterialLampCount++;
             }else{//background
                 if(m_BackLampCount >= MAX_LAMP_PER_BOARD){
                     logger()->error("lamp background set out of index");
                     return;
                 }
                 if(lampPortName == "")
                 {
                     lampPortName = QString::number(m_BackLampCount+1);
                 }
                 m_BackLamp[m_BackLampCount]->initState(m_lightBoardIndex,j,lampPortName\
                                                        ,struCnfp.nLampValue[0][m_lightBoardIndex][j],struCnfp.nLampEnable[0][m_lightBoardIndex][j]);

                 m_BackLamp[m_BackLampCount]->initSeq(m_lightBoardIndex,j\
                                                                    ,struCnfp.nLampSeq[0][m_lightBoardIndex][j]);

                 m_BackLamp[m_BackLampCount]->show();
                 m_BackLampCount++;
             }
         }
     }
     updateLampState();

}

void DoubleLightSetWidget::updateLampState()
{
    for(int i = 0;i < MAX_LAMP_PER_BOARD;i++){
        if(i >= m_MaterialLampCount){
            m_materialLamp[i]->hide();
        }
    }

    for(int i = 0;i < MAX_LAMP_PER_BOARD;i++){
        if(i >= m_BackLampCount){
            m_BackLamp[i]->hide();
        }
    }
}

void DoubleLightSetWidget::updateLightBoardBtn()
{
    for(int i = 0;i < MAX_LAMP_BOARD_PER_INTERFACE-2;i++){
        m_pLightBoardBtn[i]->setChecked(false);
        m_pLightBoardBtn[i]->setVisible(i < struCnfg.stuLayerLampCfg.nLampBoardTotal);
        if(i < struCnfg.stuLayerLampCfg.nLampBoardTotal){
            m_pLightBoardBtn[i]->setText(g_myLan().ids_ccsBoard+QString::number(i+1));
        }
    }
    if(struCnfg.nAllInOne){
        ui->pushButton_allInOne->setVisible(true);
        ui->pushButton_allInOne->setCheckable(true);
        if(struCnfg.stuLayerLampCfg.nLampBoardTotal==0)
        {
            m_lightBoardIndex = MAX_LAMP_BOARD_PER_INTERFACE-2;
        }
    }else{
        ui->pushButton_allInOne->setVisible(false);
    }
    m_pLightBoardBtn[m_lightBoardIndex]->setChecked(true);

}
void DoubleLightSetWidget::getLampCount()
{
    n_MaterialLampCnt = 0;
    n_BackLampCnt = 0;
    for(int j = 0;j < MAX_LAMP_PER_BOARD;j++){
        if(struCnfg.stuLayerLampCfg.nLampPortEnable[m_lightBoardIndex][j]){//used
            if(struCnfg.stuLayerLampCfg.nLampPortType[m_lightBoardIndex][j] == 0){//material
                n_MaterialLampCnt++;
            }else{//background
                n_BackLampCnt++;
            }
        }
    }
}

void DoubleLightSetWidget::on_pushButton_allInOne_clicked(bool checked)
{
    if(checked){
        m_pLightBoardBtn[m_lightBoardIndex]->setChecked(false);
        m_lightBoardIndex = MAX_LAMP_BOARD_PER_INTERFACE-2;
        m_pLightBoardBtn[m_lightBoardIndex]->setChecked(true);
        updateState();
    }
}
