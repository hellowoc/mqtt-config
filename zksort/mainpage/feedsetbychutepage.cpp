#include "feedsetbychutepage.h"
#include "ui_feedsetbychutepage.h"

FeedSetByChutePage::FeedSetByChutePage(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::FeedSetByChutePage)
{
    ui->setupUi(this);
    m_pFeedEdit[0]= ui->widget;
    m_pFeedEdit[1]= ui->widget_2;
    m_pFeedEdit[2]= ui->widget_3;
    m_pFeedEdit[3]= ui->widget_4;
    m_pFeedEdit[4]= ui->widget_5;
    m_pFeedEdit[5]= ui->widget_6;
    m_pFeedEdit[6]= ui->widget_7;
    m_pFeedEdit[7]= ui->widget_8;
    m_pFeedEdit[8]= ui->widget_9;
    m_pFeedEdit[9]= ui->widget_10;
    m_pFeedEdit[10]= ui->widget_11;
    m_pFeedEdit[11]= ui->widget_12;
    setSlotConnect();
    connect(myUpdateStatusThread,SIGNAL(sRefreshBalFeedSig()),this,SLOT(updateFeedValue()));
}

FeedSetByChutePage::~FeedSetByChutePage()
{
    delete ui;
}

void FeedSetByChutePage::showPage(bool bshow)
{
    if(bshow){
        updateFeedValue();
        updateConveyor();
    }
    basewidget::showPage(bshow);
}

void FeedSetByChutePage::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){
        if((struCnfp.bEnableBalanceFeed == 1)
                &&(struCnfg.stuFeedAutoCtrl.autoFeedMode == AUTO_FEED_MODE_0)
                &&(struCnfg.stuFeedAutoCtrl.isHasRadar == 1))
        {
            return;
        }
        else
        {
            struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[msg2] = m_FeedValue[msg2];
            logger()->info("adjust output chute %1",msg2);
            myFlow.resetFeederRG();
            for (int i = 0; i < MAX_FEEDER; i++) { //xiewj:21/12/22 人为改变产量直接影响柔性供料参数
                struGsh.chuteFeederBeforeStop[i] = struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[i];

                struGsh.chuteFeedMainValue[i] = struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[i];
                struGsh.chuteFeedAutoLastValue[i] = struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[i];
            }

            if(struCnfg.nMachineType == MACHINE_LD_ZL_GP || struCnfg.nMachineType == MACHINE_LD2_ZL_GP)
                myFlow.resetConveyorSpeed();
        }
    }
}

void FeedSetByChutePage::OnLayerChange()
{
    updateFeedValue();
}


void FeedSetByChutePage::retranslateUiWidget()
{

}

void FeedSetByChutePage::updateFeedValue()
{
    int totalFeed = 0;

    if((struCnfg.nMachineSpliceType == MACHINE_SPLICE_TYPE_BIG)&&(struCnfg.nChuteTotal > 2))
    {
        totalFeed = struCnfg.nChuteTotal+1;
    }
    else
    {
        totalFeed = struCnfg.nChuteTotal;
    }
    ui->widget_13->setVisible(struCnfg.bigVibIsUse);
    ui->widget_13->setParams(this,0,100,-1,&struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederTea);
    ui->widget_13->setTitle(g_myLan().teaFeeder);
    ui->widget_13->setValue(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederTea);


    if((struCnfp.bEnableBalanceFeed == 1)&&(struCnfg.stuFeedAutoCtrl.autoFeedMode == AUTO_FEED_MODE_0))
    {
        for(int i = 0;i < MAX_VIEW_UNIT_USE;i++){
            if(i < totalFeed){
                m_FeedValue[i] = struGsh.feederRealSend[i];
                m_pFeedEdit[i]->setVisible(true);
                m_pFeedEdit[i]->setTitle(QString::number(i+1));
                m_pFeedEdit[i]->setValue(m_FeedValue[i]);
                m_pFeedEdit[i]->setEnabled(false);
            }else{
                m_pFeedEdit[i]->setVisible(false);
            }
        }
    }
    else
    {
        for(int i = 0;i < MAX_VIEW_UNIT_USE;i++){
            if(i < totalFeed){
                m_FeedValue[i] = struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[i];
                m_pFeedEdit[i]->setVisible(true);
                m_pFeedEdit[i]->setParams(this,0,100,i,&m_FeedValue[i]);
                m_pFeedEdit[i]->setTitle(QString::number(i+1));
                m_pFeedEdit[i]->setValue(m_FeedValue[i]);
                m_pFeedEdit[i]->setEnabled(true);
            }else{
                m_pFeedEdit[i]->setVisible(false);
            }
        }
    }
}

void FeedSetByChutePage::updateConveyor()
{
    ui->conveyorBtn->setVisible(false);
    ui->conveyorSpeed->setVisible((struCnfg.nMachineType == MACHINE_LD_ZL_GP||struCnfg.nMachineType == MACHINE_LD2_ZL_GP)
                                  && (struGsh.bRunMode != RUNMODE_USER && struGsh.bRunMode != RUNMODE_ENGINEER));
    ui->conveyorSpeed->setTitle(g_myLan().ids_Conveyor);
    ui->conveyorSpeed->setParams(this,0,30000,-1,&struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nConveyorSpeed);
    ui->conveyorSpeed->setValue(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nConveyorSpeed);
}

void FeedSetByChutePage::on_conveyorBtn_clicked()
{
    myFlow.resetConveyorSpeed();
}
