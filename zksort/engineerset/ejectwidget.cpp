#include "ejectwidget.h"
#include "ui_ejectwidget.h"
#include <qmath.h>
#include <QDebug>
#include "globalflow.h"
enum{
    EdgeCut,
    Impured,
    CloseCal,
    ReverseSort,
//    Delay,
    BadPoint,
    CenterSize,
    DelayRow
};
EjectWidget::EjectWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::EjectWidget)
{
    ui->setupUi(this);

    curView  = 0;
    curVavle = 0;
    curGroup = 0;

    m_pLayerBtn[0] = ui->mainBtn;
    m_pLayerBtn[1] = ui->viceBtn;
    m_pGroupWidget = ui->groupWidget;
    m_pViewBtn[0]= ui->pushButton_0;
    m_pViewBtn[1]= ui->pushButton_1;
    m_pViewBtn[2]= ui->pushButton_4;
    m_pViewBtn[3]= ui->pushButton_5;
    m_pViewBtn[4]= ui->pushButton_6;
    m_pViewBtn[5]= ui->pushButton_7;
    m_pViewBtn[6]= ui->pushButton_8;
    m_pViewBtn[7]= ui->pushButton_9;
    setSlotConnect();
//    ui->pushButton_2->hide();
 #if 0
    m_pBelowModeGroup = new QButtonGroup(this);
    m_pBelowModeGroup->addButton(ui->radioButton,0);
    m_pBelowModeGroup->addButton(ui->radioButton_3,1);
    m_pBelowModeGroup->setExclusive(true);
    for(int i = 0;i< 2;i++){
        m_pBelowModeGroup->button(i)->setCheckable(true);
        connect(m_pBelowModeGroup->button(i),SIGNAL(clicked(bool)),this,SLOT(BelowModeBtnClicked(bool)));
    }
#endif

    connect(ui->viewMode,SIGNAL(clicked(bool)),this,SLOT(ViewModeBtnClicked(bool)));

    ui->ReverseSortBtn1->setCheckable(true);
    ui->ReverseSortBtn2->setCheckable(true);
    ui->pushButton_3->setCheckable(true);
    ui->viewMode->setCheckable(true);

    ui->EdgeCutLbl1->setVisible(false);
    ui->ReverseLbl1->setVisible(false);
    ui->EdgeCutLbl2->setVisible(false);
    ui->EdgeCutEdit2->setVisible(false);
    ui->ReverseLbl2->setVisible(false);
    ui->ReverseSortBtn2->setVisible(false);
    ui->groupBox_8->setVisible(false);
//    ui->groupBox_9->setVisible(false);

    abnormalDisplay = false;
}

EjectWidget::~EjectWidget()
{
    delete ui;
}

void EjectWidget::retranslateUiWidget()
{
    ui->groupBox->setTitle(g_myLan().edge_cut);
    ui->groupBox_6->setTitle(g_myLan().infect_arithmetic);
    ui->groupBox_5->setTitle(g_myLan().sort_mode);
    ui->ReverseSortBtn1->setText(g_myLan().reverse_sort);
    ui->ReverseSortBtn2->setText(g_myLan().reverse_sort);
    ui->groupBox_4->setTitle(g_myLan().logical_relation);
    ui->groupBox_7->setTitle(g_myLan().bad_point);
    ui->groupBox_8->setTitle(g_myLan().close_calculate);
    ui->colorArithLbl->setText(g_myLan().ids_color);
    ui->shapeArithLbl->setText(g_myLan().shape);
    ui->viewMode->setText(g_myLan().logic_and);
    ui->groupBox_9->setTitle(g_myLan().ids_centerSize);
//    ui->newInfrctArithLbl->setText(g_myLan().infect_arithmetic+"2");
    ui->newInfrctArithLbl->setText(g_myLan().ipc_lightBlow);
    ui->newInfrctArithLbl2->setText(g_myLan().ipc_heavyBlow);
    ui->newInfrctArithLbl3->setText(g_myLan().ipc_flag);

    ui->groupBox_delayRow->setTitle(g_myLan().Buffer_lines);
}

void EjectWidget::showPage(bool bshow)
{
    basewidget::showPage(bshow);
    if(bshow){
        curView = struGsh.nView;
        curGroup = struGsh.nMatGroup;
        curVavle = struGsh.nVavle;

        struGsh.nView = 0;
        struGsh.nMatGroup = 0;
        struGsh.nVavle = 0;

        updateGroupWidgetStatus();

        updateView();
        updateViewValue();
    }
    else
    {
        struGsh.nView = 0;
        struGsh.nMatGroup = 0;
        struGsh.nVavle = 0;

        abnormalDisplay = false;
    }
}

void EjectWidget::OnGroupChange(int groupIndex)
{
    curVavle = (struCnfg.nMachineType == MACHINE_LD2_ZL_GP) ? struGsh.nView/4 : 0;
    updateView();
    updateViewValue();
}

void EjectWidget::OnViewChange(int view)
{
    curVavle = (struCnfg.nMachineType == MACHINE_LD2_ZL_GP) ? struGsh.nView/4 : 0;
    updateView();
    updateViewValue();
}

void EjectWidget::OnLayerChange()
{
    curVavle = (struCnfg.nMachineType == MACHINE_LD2_ZL_GP) ? struGsh.nView/4 : 0;
    updateView();
    updateViewValue();
}

void EjectWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){
        if(msg2 == EdgeCut)
        {
            if((struCnfg.struLayerInfo[struGsh.nLayer].nSplice == 1)&&(struCnfg.nChuteTotal == 1)) //1通道，且带带分割的机器
            {
                for(int i = 0; i < struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nIdentifyGroupTotal; i++)
                {
                    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][i].nEdgeCut[0] = struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].nEdgeCut[0];
                }
            }
            logger()->info("adjust edgecut view-%1, group-%2,edge-%3", struGsh.nView, struGsh.nMatGroup, struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].nEdgeCut[0]);
            myFlow.resetCameraEdgeCutRG();
        }
        if(msg2 == Impured)
        {
            if((struCnfg.struLayerInfo[struGsh.nLayer].nSplice == 1)&&(struCnfg.nChuteTotal == 1)) //1通道，且带带分割的机器
            {
                for(int i = 0; i < struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nIdentifyGroupTotal; i++)
                {
                    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][i].nImpured[0] = struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].nImpured[0];
                }
            }
            logger()->info("adjust impure view-%1, group-%2,impure-%3", struGsh.nView, struGsh.nMatGroup, struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].nImpured[0]);
            myFlow.resetCameraImpureRG();
        }
        else if(msg2 == BadPoint)
        {
            if((struCnfg.struLayerInfo[struGsh.nLayer].nSplice == 1)&&(struCnfg.nChuteTotal == 1)) //1通道，且带带分割的机器
            {
                for(int i = 0; i < struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nIdentifyGroupTotal; i++)
                {
                    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][i].nBadNum = struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].nBadNum;
                }
            }

            logger()->info("adjust badpoint view-%1, group-%2,badpoint-%3", struGsh.nView, struGsh.nMatGroup, struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].nBadNum);
            myFlow.resetCameraBadNumRG();
        }
        if(msg2 == CenterSize)
        {
            if((struCnfg.struLayerInfo[struGsh.nLayer].nSplice == 1)&&(struCnfg.nChuteTotal == 1)) //1通道，且带带分割的机器
            {
                for(int i = 0; i < struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nIdentifyGroupTotal; i++)
                {
                    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][i].nCenterSize[0] = struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].nCenterSize[0];
                    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][i].nCenterSize[1] = struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].nCenterSize[1];
                }
            }
            logger()->info("adjust center view-%1,group-%2,center-%3", struGsh.nView, struGsh.nMatGroup, struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].nCenterSize[0]);
            myFlow.resetCameraCenterSize();
        }
        if(msg2 == DelayRow)
        {
            if(struCnfg.nDPMachine)
                myFlow.setIpcDelayBufRowCount();
        }
    }
}
#if 0
void EjectWidget::BelowModeBtnClicked(bool check)
{
    curVavle = (struCnfg.nMachineType == MACHINE_LD2_ZL_GP) ? struGsh.nView/2 : 0;

    if(check){
        if(m_pBelowModeGroup->id((QPushButton*)sender()) == 0)
            struCnfp.struGroupTick[struGsh.nLayer][curVavle][struGsh.nMatGroup].nEjwidth = 0;
        else
            struCnfp.struGroupTick[struGsh.nLayer][curVavle][struGsh.nMatGroup].nEjwidth = 1;
        myFlow.resetEjectDurationTimeRG();
    }
}
#endif
void EjectWidget::ViewModeBtnClicked(bool check)
{
    curVavle = (struCnfg.nMachineType == MACHINE_LD2_ZL_GP) ? struGsh.nView/4 : 0;
//    if(struGsh.nView < 2){
    if(struGsh.nView == 0 ||struGsh.nView == 1 ||struGsh.nView == 4 ||struGsh.nView == 5){
        if(check == true)
        {
            struCnfp.struGroupTick[struGsh.nLayer][curVavle][struGsh.nMatGroup].nFrontRearMode = struCnfp.struGroupTick[struGsh.nLayer][curVavle][struGsh.nMatGroup].nFrontRearMode|0x03;
        }
        else
        {
            struCnfp.struGroupTick[struGsh.nLayer][curVavle][struGsh.nMatGroup].nFrontRearMode = struCnfp.struGroupTick[struGsh.nLayer][curVavle][struGsh.nMatGroup].nFrontRearMode&0x0C;
        }
    }
    else{
        if(check == true)
            struCnfp.struGroupTick[struGsh.nLayer][curVavle][struGsh.nMatGroup].nFrontRearMode = struCnfp.struGroupTick[struGsh.nLayer][curVavle][struGsh.nMatGroup].nFrontRearMode|0x0C;
        else
            struCnfp.struGroupTick[struGsh.nLayer][curVavle][struGsh.nMatGroup].nFrontRearMode = struCnfp.struGroupTick[struGsh.nLayer][curVavle][struGsh.nMatGroup].nFrontRearMode&0x03;
    }

    if((struCnfg.struLayerInfo[struGsh.nLayer].nSplice == 1)&&(struCnfg.nChuteTotal == 1)) //1通道，且带带分割的机器
    {
        for(int i = 0; i < struCnfg.struLayerInfo[struGsh.nLayer].stuVavleInfo[curVavle].nTickGroupTotal; i++) // 相机 组单元
        {
            struCnfp.struGroupTick[struGsh.nLayer][curVavle][i].nFrontRearMode = struCnfp.struGroupTick[struGsh.nLayer][curVavle][struGsh.nMatGroup].nFrontRearMode;
        }
    }
    myFlow.resetEjectFrontRearLogicRG();

}

void EjectWidget::updateGroupWidgetStatus()
{
    if((struCnfg.struLayerInfo[struGsh.nLayer].nSplice == 1)&&(struCnfg.nChuteTotal == 1)) //1通道，且带带分割的机器，不显示一二次
    {
        m_pGroupWidget->hide();
    }
    else
    {
        m_pGroupWidget->show();
    }

    for(int i = 0; i < struCnfg.struLayerInfo[struGsh.nLayer].stuVavleInfo[0].nTickGroupTotal; i++)
    {
        m_pGroupWidget->setPosEnable(i, true);
    }
    if(struCnfg.struLayerInfo[struGsh.nLayer].nSplice == 1)
    {
        for(int i = 0; i < struCnfg.struLayerInfo[struGsh.nLayer].stuVavleInfo[0].nTickGroupTotal; i++)
        {
            if((struCnfg.struLayerInfo[struGsh.nLayer].stuVavleInfo[0].stuTickGupInfo[i].nEjBoardGupTotal == 1)
                &&(struCnfg.struLayerInfo[struGsh.nLayer].stuVavleInfo[0].stuTickGupInfo[i].nEjBoardGupSplice[0] == 2)) //2表示右半边
            {
                m_pGroupWidget->setPosEnable(i, false);
            }
        }
    }

}

void EjectWidget::updateView()
{
    stu_group_identify &identify = struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup];
    ui->EdgeCutEdit1->setMinMax(this,0,struCnfg.nEdgeCutMaxValue,EdgeCut,&identify.nEdgeCut[0]);
    ui->EdgeCutEdit2->setMinMax(this,0,struCnfg.nEdgeCutMaxValue,EdgeCut,&identify.nEdgeCut[1]);

    ui->EdgeCutEditDelayRow->setMinMax(this,0,1023,DelayRow,&struCnfp.nIpcDelayBufRowCount[struGsh.nLayer][struGsh.nView]);
    //yy:30改成16,又改成32
    ui->ImpuredEdit1->setMinMax(this,0,struCnfg.nImpuredMaxValue,Impured,&identify.nImpured[0]);
    ui->ImpuredEdit2->setMinMax(this,0,struCnfg.nImpuredMaxValue,Impured,&identify.nImpured[1]);
    ui->ImpuredEdit3->setMinMax(this,0,struCnfg.nImpuredMaxValue,Impured,&identify.nImpuredIpc[0]);
    ui->ImpuredEdit4->setMinMax(this,0,struCnfg.nImpuredMaxValue,Impured,&identify.nImpuredIpc[1]);
    ui->ImpuredEdit5->setMinMax(this,0,255,Impured,&identify.nImpuredIpc[2]);

    ui->ColseCalEdit1->setMinMax(this,0,1,CloseCal,&identify.nColseCal[0]);
    ui->ColseCalEdit2->setMinMax(this,0,1,CloseCal,&identify.nColseCal[1]);

    ui->centerSizeEdit_1->setMinMax(this,0,8,CenterSize,&identify.nCenterSize[0]);
    ui->centerSizeEdit_2->setMinMax(this,0,8,CenterSize,&identify.nCenterSize[1]);

    ui->badPointEdit->setMinMax(this,0,50,BadPoint,&identify.nBadNum);

    ui->pushButton_3->setChecked(abnormalDisplay);
    ui->widget_abnormal->setVisible(abnormalDisplay);

    ui->ImpuredEdit3->setVisible(struCnfg.nDPMachine);
    ui->ImpuredEdit4->setVisible(struCnfg.nDPMachine);
    ui->ImpuredEdit5->setVisible(struCnfg.nDPMachine);
    ui->newInfrctArithLbl->setVisible(struCnfg.nDPMachine);
    ui->newInfrctArithLbl2->setVisible(struCnfg.nDPMachine);
    ui->newInfrctArithLbl3->setVisible(struCnfg.nDPMachine);

    ui->EdgeCutEditDelayRow->hide();
    ui->groupBox_delayRow->hide();
}

void EjectWidget::updateViewValue()
{
    stu_group_identify &identify = struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup];
    ui->EdgeCutEdit1->setText(QString::number(identify.nEdgeCut[0]));
    ui->EdgeCutEdit2->setText(QString::number(identify.nEdgeCut[1]));

    ui->EdgeCutEditDelayRow->setText(QString::number(struCnfp.nIpcDelayBufRowCount[struGsh.nLayer][struGsh.nView]));

    ui->ImpuredEdit1->setText(QString::number(identify.nImpured[0]));
    ui->ImpuredEdit2->setText(QString::number(identify.nImpured[1]));

    ui->ImpuredEdit3->setText(QString::number(identify.nImpuredIpc[0]));
    ui->ImpuredEdit4->setText(QString::number(identify.nImpuredIpc[1]));
    ui->ImpuredEdit5->setText(QString::number(identify.nImpuredIpc[2]));

    ui->ColseCalEdit1->setText(QString::number(identify.nColseCal[0]));
    ui->ColseCalEdit2->setText(QString::number(identify.nColseCal[1]));

    ui->centerSizeEdit_1->setText(QString::number(identify.nCenterSize[0]));
    ui->centerSizeEdit_2->setText(QString::number(identify.nCenterSize[1]));

    ui->ReverseSortBtn1->setChecked(identify.nReverseSort[0] == 1);
    ui->ReverseSortBtn2->setChecked(identify.nReverseSort[1] == 1);


    ui->badPointEdit->setText(QString::number(identify.nBadNum));

    updateRelationBtnState();
}

void EjectWidget::updateReverseSortBtnState()
{
    if(ui->ReverseSortBtn1->isChecked()){
        ui->ReverseSortBtn1->setText(g_myLan().normal_sort);
    }else{
        ui->ReverseSortBtn1->setText(g_myLan().reverse_sort);
    }
    if(ui->ReverseSortBtn2->isChecked()){
        ui->ReverseSortBtn2->setText(g_myLan().normal_sort);
    }else{
        ui->ReverseSortBtn2->setText(g_myLan().reverse_sort);
    }
}

void EjectWidget::updateRelationBtnState()
{
    curVavle = (struCnfg.nMachineType == MACHINE_LD2_ZL_GP) ? struGsh.nView/4 : 0;

    stu_group_tick &tick = struCnfp.struGroupTick[struGsh.nLayer][curVavle][struGsh.nMatGroup];

    if(struGsh.nView == 0 ||struGsh.nView == 1 ||struGsh.nView == 4 ||struGsh.nView == 5){
        ui->label->setText(g_myLan().main_view);
        if(tick.nFrontRearMode & 0x03 == 0x03){
            ui->viewMode->setChecked(true);
        }else{
            ui->viewMode->setChecked(false);
        }
    }
    else{
        ui->label->setText(g_myLan().vice_view);
        int mode = tick.nFrontRearMode & 0x0C;
        if( mode== 0x0C){
            ui->viewMode->setChecked(true);
        }else{
            ui->viewMode->setChecked(false);
        }
    }
}

void EjectWidget::on_ReverseSortBtn1_clicked(bool checked)
{
    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].nReverseSort[0] = (checked == true) ? 1 : 0;
    logger()->info("adjust reverse view-%1,group-%2,reverse-%3", struGsh.nView, struGsh.nMatGroup, struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].nReverseSort[0]);
    if((struCnfg.struLayerInfo[struGsh.nLayer].nSplice == 1)&&(struCnfg.nChuteTotal == 1)) //1通道，且带带分割的机器
    {
        for(int i = 0; i < struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nIdentifyGroupTotal; i++)
        {
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][i].nReverseSort[0] = struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].nReverseSort[0];
        }
    }
    myFlow.resetCameraReverseSortRG();
}

void EjectWidget::on_ReverseSortBtn2_clicked(bool checked)
{
    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].nReverseSort[1] = (int)!checked;
    logger()->info("adjust reverse2 view-%1,group-%2,reverse-%3", struGsh.nView, struGsh.nMatGroup, struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].nReverseSort[1]);
    if((struCnfg.struLayerInfo[struGsh.nLayer].nSplice == 1)&&(struCnfg.nChuteTotal == 1)) //1通道，且带带分割的机器
    {
        for(int i = 0; i < struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nIdentifyGroupTotal; i++)
        {
            struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][i].nReverseSort[1] = struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].nReverseSort[1];
        }
    }
    updateReverseSortBtnState();
    myFlow.resetCameraReverseSortRG();
}

void EjectWidget::on_pushButton_3_clicked(bool checked)
{     
      abnormalDisplay = checked;
      updateView();
//      ui->widget_abnormal->setVisible(abnormalDisplay);
}
