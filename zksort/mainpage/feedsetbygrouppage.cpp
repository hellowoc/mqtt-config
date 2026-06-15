#include "feedsetbygrouppage.h"
#include "ui_feedsetbygrouppage.h"

FeedSetByGroupPage::FeedSetByGroupPage(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::FeedSetByGroupPage)
{
    ui->setupUi(this);
    m_pFeedProgress[0] = ui->horiProgress;
    m_pFeedProgress[1] = ui->horiProgress2;
    m_pFeedProgress[2] = ui->horiProgress3;
    m_pFeedProgress[3] = ui->horiProgress4;
    m_pFeedProgress[4] = ui->horiProgress5;

    m_pFeedNameLbl[0] = ui->feedNameLbl;
    m_pFeedNameLbl[1] = ui->feedNameLbl2;
    m_pFeedNameLbl[2] = ui->feedNameLbl3;
    m_pFeedNameLbl[3] = ui->feedNameLbl4;
    m_pFeedNameLbl[4] = ui->feedNameLbl5;

    for(int i = 0;i < MAX_GROUP_IDENTIFY;i++){
        connect(m_pFeedProgress[i],SIGNAL(onValueChangeExt(int)),this,SLOT(onFeedValueChanged(int)));

        m_pFeedProgress[i]->setObjectName("FeedProgress");
        m_pFeedNameLbl[i]->setObjectName("FeedNameLabel");
        m_pFeedNameLbl[i]->setAlignment(Qt::AlignCenter);

    }

}

FeedSetByGroupPage::~FeedSetByGroupPage()
{
    delete ui;
}

void FeedSetByGroupPage::showPage(bool bshow)
{
    if(bshow){
        for(int i = 0;i < MAX_GROUP_IDENTIFY;i++){
            if(i < struCnfg.struLayerInfo[struGsh.nLayer].stuVavleInfo[struGsh.nVavle].nTickGroupTotal){
                stru_vavle_group_info vavle_group = struCnfg.struLayerInfo[struGsh.nLayer].stuVavleInfo[struGsh.nVavle].stuTickGupInfo[i];

                int feedid = vavle_group.nEjBoardGupId[0];
                int feedValue = struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedid];

                m_pFeedProgress[i]->setMaxAndMinValue(100,0);
                m_pFeedProgress[i]->setCurrentValue(feedValue);

                m_pFeedNameLbl[i]->show();
                m_pFeedProgress[i]->show();
            }else{
                m_pFeedNameLbl[i]->hide();
                m_pFeedProgress[i]->hide();
            }
        }
    }
}

void FeedSetByGroupPage::receiveMsg(int msg1, int msg2, int msg3)
{

}

void FeedSetByGroupPage::OnLayerChange()
{

}

void FeedSetByGroupPage::onFeedValueChanged(int value)
{
    UiHorizontalProgressExt* pHor = qobject_cast<UiHorizontalProgressExt*>(sender());
    int index = -1;
    for(int i = 0; i < MAX_GROUP_IDENTIFY; i++)
    {
        if(pHor == m_pFeedProgress[i]){
            index = i;
            break;
        }
    }
    if(index < 0)
        return;

    stru_vavle_group_info vavle_group = struCnfg.struLayerInfo[struGsh.nLayer].stuVavleInfo[struGsh.nVavle].stuTickGupInfo[index];
    int tempFeedTotal = vavle_group.nEjBoardGupTotal;
    int feedid = vavle_group.nEjBoardGupId[0];
//    int offset = value - struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedid];
    for(int curIdx = 0 ;curIdx < tempFeedTotal;curIdx++){
        feedid = vavle_group.nEjBoardGupId[curIdx];
        struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedid] = value;

        if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedid] > 100)
            struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedid] = 100;
        if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedid] < 0)
            struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedid] = 0;
    }
    logger()->info("adjust output of group %1:%2",index,struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[feedid]);
    for (int i = 0; i < MAX_FEEDER; i++) { //xiewj:21/12/22 人为改变产量直接影响柔性供料参数
        struGsh.chuteFeederBeforeStop[i] = struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[i];
        struGsh.chuteFeedAutoLastValue[i] = struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[i];
        struGsh.chuteFeedMainValue[i] = struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeederValueMain[i];
    }
    myFlow.resetFeederRG();
}

void FeedSetByGroupPage::retranslateUiWidget()
{
    m_pFeedNameLbl[0]->setText(g_myLan().ids_one);
    m_pFeedNameLbl[1]->setText(g_myLan().ids_two);
    m_pFeedNameLbl[2]->setText(g_myLan().ids_three);
    m_pFeedNameLbl[3]->setText(g_myLan().ids_four);
    m_pFeedNameLbl[4]->setText(g_myLan().ids_five);
}
