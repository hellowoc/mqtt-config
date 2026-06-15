#include "feedbanlanceaimsingle.h"
#include "ui_feedbanlanceaimsingle.h"

FeedBanlanceAimSingle::FeedBanlanceAimSingle(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::FeedBanlanceAimSingle)
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
    m_pFeedEdit[12]= ui->widget_13;

    setSlotConnect();
}

FeedBanlanceAimSingle::~FeedBanlanceAimSingle()
{
    delete ui;
}

void FeedBanlanceAimSingle::showPage(bool bshow)
{
    if(bshow){
        updateFeedValue();
    }
    basewidget::showPage(bshow);
}

void FeedBanlanceAimSingle::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){
        struCnfg.stuFeedAutoCtrl.nFeedValueBias[msg2] = m_FeedValue[msg2];
    }
}

void FeedBanlanceAimSingle::OnLayerChange()
{
    updateFeedValue();
}


void FeedBanlanceAimSingle::retranslateUiWidget()
{

}

void FeedBanlanceAimSingle::updateFeedValue()
{
    int totalFeed = 0;

    if((struCnfg.nMachineSpliceType == MACHINE_SPLICE_TYPE_BIG)&&(struCnfg.nChuteTotal > 2 || (struCnfg.nChuteTotal <= 2 && !struCnfg.nAllInOne)))
    {
        totalFeed = struCnfg.nChuteTotal+1;
    }
    else
    {
        totalFeed = struCnfg.nChuteTotal;
    }

    for(int i = 0;i < MAX_VIEW_UNIT_USE;i++){
        if(i < totalFeed){
            m_FeedValue[i] = struCnfg.stuFeedAutoCtrl.nFeedValueBias[i];
            m_pFeedEdit[i]->setParams(this,0,200,i,&m_FeedValue[i]);
            m_pFeedEdit[i]->setTitle(QString::number(i+1));
            m_pFeedEdit[i]->setValue(m_FeedValue[i]);
            m_pFeedEdit[i]->setVisible(true);
        }
        else
        {
            m_pFeedEdit[i]->setVisible(false);
        }
    }
}
