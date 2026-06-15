#include "feedsetbyclearuppage.h"
#include "ui_feedsetbyclearuppage.h"

FeedSetByClearUpPage::FeedSetByClearUpPage(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::FeedSetByClearUpPage)
{
    ui->setupUi(this);

//    setSlotConnect();
//    connect(myUpdateStatusThread,SIGNAL(setSignal()),this,SLOT(updateFeedValue()));
}

FeedSetByClearUpPage::~FeedSetByClearUpPage()
{
    delete ui;
}

void FeedSetByClearUpPage::showPage(bool bshow)
{
    if(bshow){
        ui->lineEdit->setText(QString("%1").arg(struCnfg.stuFeedAutoCtrl.feedBalanceClearUpValue));

        ui->lineEdit->setMinMax(this,0,100,0,&struCnfg.stuFeedAutoCtrl.feedBalanceClearUpValue);

    }
    basewidget::showPage(bshow);
}

void FeedSetByClearUpPage::retranslateUiWidget()
{
//    ui->label->setText(g_myLan().setByCleanUp);
}

void FeedSetByClearUpPage::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){

    }
}
