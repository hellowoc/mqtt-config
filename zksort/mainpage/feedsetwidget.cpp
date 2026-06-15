#include "feedsetwidget.h"
#include "globalflow.h"
#include "ui_feedsetwidget.h"
#include <QButtonGroup>

FeedSetWidget::FeedSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::FeedSetWidget)
{
    ui->setupUi(this);
    m_pfeedTypebtnGroup = new QButtonGroup(this);
    m_pfeedTypebtnGroup->addButton(ui->radioButton,0);
    m_pfeedTypebtnGroup->addButton(ui->radioButton_2,1);
    m_pfeedTypebtnGroup->addButton(ui->radioButton_3,2);
    m_pfeedTypebtnGroup->setExclusive(true);
    for(int i = 0;i< 3;i++){
        m_pfeedTypebtnGroup->button(i)->setCheckable(true);
        connect(m_pfeedTypebtnGroup->button(i),SIGNAL(clicked(bool)),this,SLOT(feedTypebtnClicked(bool)));
    }

    m_pctrlBoardBtn[0] = ui->pushButton;
    m_pctrlBoardBtn[1] = ui->pushButton_2;
    for(int i = 0;i < 2;i++){
        if(m_pctrlBoardBtn[i]){
            m_pctrlBoardBtn[i]->setCheckable(true);
            connect(m_pctrlBoardBtn[i],SIGNAL(clicked(bool)),this,SLOT(ctrlBtnClickedSlots()));
        }
    }
    ui->pushButton->setFixedWidth(50);
    ui->pushButton_2->setFixedWidth(50);

    ui->targetfeedlineEdit->setFixedWidth(200);

    setSlotConnect();
    m_ctrlBoardIndex = 0;
}

FeedSetWidget::~FeedSetWidget()
{
    delete ui;
}

void FeedSetWidget::showPage(bool bshow)
{
    bool SetByGroupDisplay = true;
    if(bshow){
        ui->pushButton->hide();
        ui->pushButton_2->hide();
        ui->pushButton->setText(g_myLan().ids_controlBoard +QString(" 1"));
        ui->pushButton_2->setText(g_myLan().ids_controlBoard +QString(" 2"));

        m_pfeedTypebtnGroup->button(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeedSetType)->setChecked(true);

        if((struCnfg.nChuteTotal == 1)||(struCnfg.nMachineSpliceType == MACHINE_SPLICE_TYPE_BIG))
        {
            ui->radioButton->hide();
            ui->radioButton_2->hide();
            ui->radioButton_3->hide();
        }
        else
        {
            ui->radioButton->show();
            ui->radioButton_2->show();
            ui->radioButton_3->hide();
        }

        ui->widget_feed->setVisible(struCnfg.nFeedViceTotal == 1);
        ui->widget_feed->setParams(this,0,100,0,&struCnfp.struGroupCtrl[0].nFeederValueVice[0]);
        ui->widget_feed->setValue(struCnfp.struGroupCtrl[0].nFeederValueVice[0]);

        SetByGroupDisplay = (struCnfp.bEnableBalanceFeed == 1) ? false : true;

        ui->targetfeedlineEdit->setText(QString("%1").arg(struCnfp.nAutoFeed[struGsh.nLayer][0].target_feeder));
        ui->targetfeedlineEdit->setMinMax(this,0,100,0,&struCnfp.nAutoFeed[struGsh.nLayer][0].target_feeder);
        ui->targetfeedlineEdit->setVisible(true);
        ui->feedtargetlabel->setVisible(true);

        ui->clearfeedcheckBox->setChecked(struGsh.isClearMode);

        if(SetByGroupDisplay == false)  //开启自动均衡，就进展按次调整流量
        {
            ui->radioButton_2->hide();
            if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeedSetType == 1)
            {
                struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeedSetType = 0;
            }
            ui->clearfeedcheckBox->show();
        }
        else
        {
            if(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeedSetType == 2)
            {
                struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeedSetType = 0;
            }
            ui->radioButton_3->hide();
            ui->clearfeedcheckBox->hide();
        }
        updateStackwidget();


        ui->adjustBtn->setVisible(struCnfg.nAutoStaticsEnable == 1);
    }
    basewidget::showPage(bshow);
}

void FeedSetWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){
        //todo 辅振动器更改下发
        myFlow.resetFeederRG();
    }
}

void FeedSetWidget::OnLayerChange()
{
//    updateFeedValue();
}

void FeedSetWidget::ctrlBtnClickedSlots()
{
    for(int i=0; i<2; i++ )
        if(sender() == m_pctrlBoardBtn[i])
        {
            m_pctrlBoardBtn[m_ctrlBoardIndex]->setChecked(false);
            m_ctrlBoardIndex = i;
            m_pctrlBoardBtn[m_ctrlBoardIndex]->setChecked(true);
            break;
        }
}

void FeedSetWidget::feedTypebtnClicked(bool check)
{
    if(check){
        struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeedSetType = m_pfeedTypebtnGroup->id((QPushButton*)sender());
    }
    updateStackwidget();
}

void FeedSetWidget::retranslateUiWidget()
{
    ui->feedtargetlabel->setText(g_myLan().ids_SetYeild);
    ui->adjustBtn->setText(g_myLan().ids_autoFeed);
    ui->widget_feed->setTitle(g_myLan().vice_feeder);
    ui->radioButton->setText(g_myLan().setByChute);
    ui->radioButton_2->setText(g_myLan().setByGroup);
//    ui->radioButton_3->setText(g_myLan().setByCleanUp);
//    ui->clearfeedcheckBox->setText(g_myLan().setByCleanUp);
    for(int i = 0;i< ui->stackedWidget->count();i++){
        basewidget *widget =(basewidget*) ui->stackedWidget->widget(i);
        widget->retranslateUiWidget();
    }
}

void FeedSetWidget::updateStackwidget()
{
    ui->stackedWidget->setCurrentIndex(struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeedSetType);
    basewidget *widget = qobject_cast<basewidget*>(ui->stackedWidget->currentWidget());
    widget->showPage();
}

void FeedSetWidget::on_adjustBtn_clicked()
{
    g_MainManager().ShowWidget(SM_AUTO_FEEDER_WIDGET);
}

void FeedSetWidget::on_clearfeedcheckBox_clicked()
{
    struGsh.isClearMode = (struGsh.isClearMode == false) ? true : false;

    ui->clearfeedcheckBox->setChecked(struGsh.isClearMode);

    if(struGsh.isClearMode)
    {
        myFlow.resetFeederClear();
    }
    else
    {
        myFlow.resetFeederRG();
    }
}
