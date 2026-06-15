#include "identifywidget.h"
#include "ui_identifywidget.h"
#include "constant.h"
#include "globalparams.h"

IdentifyWidget::IdentifyWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::IdentifyWidget)
{
    ui->setupUi(this);
    m_gridLayout = new QGridLayout(this);
    m_gridLayout->setHorizontalSpacing(10);
    m_gridLayout->setVerticalSpacing(5);
    ui->widget->setLayout(m_gridLayout);

    m_pViewBtn[0] = ui->pushButton_0;
    m_pViewBtn[1] = ui->pushButton_1;
    m_pViewBtn[2] = ui->pushButton_2;
    m_pViewBtn[3] = ui->pushButton_3;
    m_pViewBtn[4] = ui->pushButton_4;
    m_pViewBtn[5] = ui->pushButton_5;
    m_pViewBtn[6] = ui->pushButton_6;
    m_pViewBtn[7] = ui->pushButton_7;

    m_pLayerBtn[0] = ui->mainBtn;
    m_pLayerBtn[1] = ui->viceBtn;
    m_pGroupWidget = ui->groupWidget;
    setSlotConnect();

    shottimer = new QTimer;
    connect(shottimer,SIGNAL(timeout()),this, SLOT(getscreenshot()));
}

IdentifyWidget::~IdentifyWidget()
{
    delete ui;
}

void IdentifyWidget::OnViewChange(int view)
{
    updateArithState();
    updateViewButtonVisible();

   updateGroupWidgetStatus();
}

void IdentifyWidget::showPage(bool bshow)
{
    basewidget::showPage(bshow);
    if(bshow){
        m_curArithUiIndex = -1;

        QStringList items;
        items<<g_myLan().all_seperate<<g_myLan().all_alike<<g_myLan().ids_masterSame<<g_myLan().second_alike_first;
        ui->comboBox->clear();
        ui->comboBox->addItems(items);
        ui->comboBox->setCurrentIndex(struCnfp.nMatCopySetMode);

        updateBtnVisiable();

        updateArithState();

        updateGroupWidgetStatus();


        if(struCnfg.nEnableDispArithSetByTimes == 0)
        {
            ui->comboBox->setEnabled(false);
            ui->comboBox->hide();
        }
        else
        {
            ui->comboBox->setEnabled(true);
            ui->comboBox->show();
        }


        if( struGsh.bScreenShotStatus == 1)
        {
            shottimer->start(1000);

        }
    }
}

void IdentifyWidget::OnGroupChange(int groupIndex)
{
    updateGroupArithState();
    updateGroupWidget();

    updateGroupWidgetStatus();
}

void IdentifyWidget::OnLayerChange()
{
    updateGroupArithState();

    updateGroupWidgetStatus();
}


void IdentifyWidget::getscreenshot()
{
    shottimer->stop();
    {
        for(int i=0;i<MAX_VIEW;i++){
            if(struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[i])
            {
                QMetaObject::invokeMethod(m_pViewBtn[i],"clicked", Qt::QueuedConnection);
                struGsh.nMatGroup = 0;
                for(int j=0; j<struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[i].nIdentifyGroupTotal;j++)
                {
                    groupBtnClick(j);
                    updateGroupWidgetState();
                    myFlow.sleep(1);
                    myFlow.saveScreenshot();
                }
            }
        }
        myFlow.sleep(1);
        struGsh.bScreenShotStatus = 0;
    }
}

void IdentifyWidget::updateGroupWidgetStatus()
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

void IdentifyWidget::updateArithState()
{
    for(int i = 0; i < m_ArithUiVector.size(); i++)
    {
        delete m_ArithUiVector[i];
    }
    m_ArithUiVector.clear();

    while(m_gridLayout->count() > 0)
    m_gridLayout->removeItem(m_gridLayout->takeAt(0));

    for(int i = 0;i< ARITHMETIC_TOTAL;i++){
        if(struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].nViewArithVisiable[i])
        {
            MyArithUi *arith = new MyArithUi(i);
            m_ArithUiVector.push_back(arith);
        }
    }
    for(int index = 0; index < m_ArithUiVector.size(); index++)
    {
       connect(m_ArithUiVector[index],SIGNAL(arithBtnCheckedSignal(bool,int)),this,SLOT(arithWidgetSlots(bool,int)));
       m_gridLayout->addWidget(m_ArithUiVector[index],index/3,index%3);
       m_ArithUiVector[index]->showWidget();
    }
}

void IdentifyWidget::updateGroupArithState()
{
    for(int index = 0; index < m_ArithUiVector.size(); index++)
    {
       m_ArithUiVector[index]->updateWidget();
    }
}

void IdentifyWidget::updateBtnVisiable()
{
    int index = struCnfp.nMatCopySetMode;

    struGsh.nView = 0;
    struGsh.nMatGroup = 0;
    OnViewChange(0);
    OnGroupChange(0);

    updateViewButtonVisible();

    updateGroupWidgetState();
    updateViewBtnState();
}

void IdentifyWidget::retranslateUiWidget()
{

}

void IdentifyWidget::arithWidgetSlots(bool checked, int arithType)
{
    MyArithUi *tmparith = (qobject_cast<MyArithUi*>)(sender());
    int index = m_ArithUiVector.indexOf(tmparith);


    m_curArithUiIndex = index;

}

void IdentifyWidget::copySpliceDisplay()
{
    int i, j, k, m;

    for(i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
            for(k = 1; k < struCnfg.struLayerInfo[i].stuViewInfo[j].nIdentifyGroupTotal; k++)
            {
                if((struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal == 1)       //只有一个相机板,且被分割
                    &&(struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[j].stuIdenGupInfo[k].nUnitGupSplice[0] == 2)) //2表示右半边
                {
                    for(m = 0; m < MAX_GREY_COLOR_ARITH_TOTAL; m++)
                    {
                        struCnfp.struGroupIdentify[i][j][k].struGreyColor[m].nDispEnable = struCnfp.struGroupIdentify[i][j][k-1].struGreyColor[m].nDispEnable;
                    }
                    for(m = 0; m < MAX_SVM_ARITH_TOTAL; m++)
                    {
                        struCnfp.struGroupIdentify[i][j][k].struSvm[m].nDispEnable = struCnfp.struGroupIdentify[i][j][k-1].struSvm[m].nDispEnable;
                    }
                    for(m = 0; m < MAX_LONG_SHORT_TOTAL; m++)
                    {
                        struCnfp.struGroupIdentify[i][j][k].struLongShort[m].nDispEnable  = struCnfp.struGroupIdentify[i][j][k-1].struLongShort[m].nDispEnable;
                    }
                    for(m = 0; m < MAX_BUD_TOTAL; m++)
                    {
                        struCnfp.struGroupIdentify[i][j][k].struBud[m].nDispEnable             = struCnfp.struGroupIdentify[i][j][k-1].struBud[m].nDispEnable;
                    }
                    for(m = 0; m < MAX_BIG_SMALL_TOTAL; m++)
                    {
                        struCnfp.struGroupIdentify[i][j][k].struBigSmall[m].nDispEnable        = struCnfp.struGroupIdentify[i][j][k-1].struBigSmall[m].nDispEnable;
                    }
                    for(m = 0; m < MAX_CIRCLE_LONG_TOTAL; m++)
                    {
                        struCnfp.struGroupIdentify[i][j][k].struCircleLong[m].nDispEnable      = struCnfp.struGroupIdentify[i][j][k-1].struCircleLong[m].nDispEnable;
                    }
                    struCnfp.struGroupIdentify[i][j][k].struColorSat.nDispEnable        = struCnfp.struGroupIdentify[i][j][k-1].struColorSat.nDispEnable;
                    struCnfp.struGroupIdentify[i][j][k].struColorHue.nDispEnable        = struCnfp.struGroupIdentify[i][j][k-1].struColorHue.nDispEnable;
                    struCnfp.struGroupIdentify[i][j][k].struColorScale.nDispEnable      = struCnfp.struGroupIdentify[i][j][k-1].struColorScale.nDispEnable;
                    struCnfp.struGroupIdentify[i][j][k].struMildew.nDispEnable          = struCnfp.struGroupIdentify[i][j][k-1].struMildew.nDispEnable;
                    struCnfp.struGroupIdentify[i][j][k].struOutline.nDispEnable         = struCnfp.struGroupIdentify[i][j][k-1].struOutline.nDispEnable;
                    struCnfp.struGroupIdentify[i][j][k].struLine.nDispEnable            = struCnfp.struGroupIdentify[i][j][k-1].struLine.nDispEnable;
                    for(m = 0; m < MAX_ROUND_TOTAL; m++)
                    {
                        struCnfp.struGroupIdentify[i][j][k].struRound[m].nDispEnable           = struCnfp.struGroupIdentify[i][j][k-1].struRound[m].nDispEnable;
                    }
                    struCnfp.struGroupIdentify[i][j][k].struColorValue.nDispEnable      = struCnfp.struGroupIdentify[i][j][k-1].struColorValue.nDispEnable;
                    struCnfp.struGroupIdentify[i][j][k].struSplMildew.nDispEnable       = struCnfp.struGroupIdentify[i][j][k-1].struSplMildew.nDispEnable;
                    struCnfp.struGroupIdentify[i][j][k].struWhitePeanut.nDispEnable       = struCnfp.struGroupIdentify[i][j][k-1].struWhitePeanut.nDispEnable;
                    struCnfp.struGroupIdentify[i][j][k].struPepperHandle.nDispEnable       = struCnfp.struGroupIdentify[i][j][k-1].struPepperHandle.nDispEnable;
                    for(m = 0; m < MAX_SUPPRESS_LARGE_TOTAL; m++)
                    {
                        struCnfp.struGroupIdentify[i][j][k].struSuppressLarge[m].nDispEnable = struCnfp.struGroupIdentify[i][j][k-1].struSuppressLarge[m].nDispEnable;
                    }
                    struCnfp.struGroupIdentify[i][j][k].struWheatSprout.nDispEnable       = struCnfp.struGroupIdentify[i][j][k-1].struWheatSprout.nDispEnable;
                    for(m = 0; m < MAX_TEA_ARITH_TOTAL; m++)
                    {
                        struCnfp.struGroupIdentify[i][j][k].struTeaColor[m].nDispEnable = struCnfp.struGroupIdentify[i][j][k-1].struTeaColor[m].nDispEnable;
                    }
                }
            }
        }
    }
}

void IdentifyWidget::on_comboBox_activated(int index)
{
    struGsh.nView = 0;
    struCnfp.nMatCopySetMode = index;
#if 0
    for(int i = 0;i < MAX_VIEW;i++){
        for(int j = 0;j < MAX_GROUP_IDENTIFY;j++){
            for(int k = 0;k < MAX_GREY_COLOR_ARITH_TOTAL;k++)
            {
                struCnfp.struGroupIdentify[struGsh.nLayer][i][j].struGreyColor[k].nDispEnable = 0;
            }
            for(int k = 0; k < MAX_SVM_ARITH_TOTAL; k++)
            {
                struCnfp.struGroupIdentify[struGsh.nLayer][i][j].struSvm[k].nDispEnable = 0;
            }
            struCnfp.struGroupIdentify[struGsh.nLayer][i][j].struBigSmall.nDispEnable = 0;
            struCnfp.struGroupIdentify[struGsh.nLayer][i][j].struCircleLong.nDispEnable = 0;
            struCnfp.struGroupIdentify[struGsh.nLayer][i][j].struLongShort.nDispEnable = 0;
        }
    }
    memset(struCnfp.nGroupArithEnable,0,sizeof(struCnfp.nGroupArithEnable));

    myFlow.resetAllArithEnableClear();
    myFlow.resetAllArithParams();
#endif

    /* 参数更新 */
    for(int k = 0;k < MAX_GREY_COLOR_ARITH_TOTAL;k++)
    {
        myFlow.materialArithEnableCopy(struGsh.nLayer,LCD_VERI_MAIN_VIEW_FRONT, 0, k, true);
    }
    for(int k = 0; k < MAX_SVM_ARITH_TOTAL; k++)
    {
        myFlow.materialArithEnableCopy(struGsh.nLayer,LCD_VERI_MAIN_VIEW_FRONT, 0, ARITH_SVM_A+k, true);
    }
    for (int k = 0; k < MAX_LONG_SHORT_TOTAL; k++)
    {
        myFlow.materialArithEnableCopy(struGsh.nLayer,LCD_VERI_MAIN_VIEW_FRONT, 0, ARITH_LONG_SHORT+k, true);
    }
    for (int k = 0; k < MAX_BUD_TOTAL; k++)
    {
        myFlow.materialArithEnableCopy(struGsh.nLayer,LCD_VERI_MAIN_VIEW_FRONT, 0, ARITH_BUD+k, true);
    }
    for(int k = 0; k < MAX_BIG_SMALL_TOTAL; k++)
    {
        myFlow.materialArithEnableCopy(struGsh.nLayer, LCD_VERI_MAIN_VIEW_FRONT, 0, ARITH_BIG_SMALL+k, true);
    }
    for(int k = 0; k < MAX_CIRCLE_LONG_TOTAL; k++)
    {
        myFlow.materialArithEnableCopy(struGsh.nLayer, LCD_VERI_MAIN_VIEW_FRONT, 0, ARITH_CIRCLE_LONG+k, true);
    }
    for(int k = 0; k < MAX_HSV_TOTAL; k++)
    {
        myFlow.materialArithEnableCopy(struGsh.nLayer, LCD_VERI_MAIN_VIEW_FRONT, 0, ARITH_HSV+k, true);
    }

    myFlow.materialArithEnableCopy(struGsh.nLayer, LCD_VERI_MAIN_VIEW_FRONT, 0, ARITH_SAT, true);
    myFlow.materialArithEnableCopy(struGsh.nLayer, LCD_VERI_MAIN_VIEW_FRONT, 0, ARITH_HUE, true);
    myFlow.materialArithEnableCopy(struGsh.nLayer, LCD_VERI_MAIN_VIEW_FRONT, 0, ARITH_SCALE, true);
    myFlow.materialArithEnableCopy(struGsh.nLayer, LCD_VERI_MAIN_VIEW_FRONT, 0, ARITH_MILDEW, true);
    myFlow.materialArithEnableCopy(struGsh.nLayer, LCD_VERI_MAIN_VIEW_FRONT, 0, ARITH_OUTLINE, true);
    myFlow.materialArithEnableCopy(struGsh.nLayer, LCD_VERI_MAIN_VIEW_FRONT, 0, ARITH_LINE, true);
    myFlow.materialArithEnableCopy(struGsh.nLayer, LCD_VERI_MAIN_VIEW_FRONT, 0, ARITH_VALUE, true);
    for(int k = 0; k < MAX_ROUND_TOTAL; k++)
    {
        myFlow.materialArithEnableCopy(struGsh.nLayer, LCD_VERI_MAIN_VIEW_FRONT, 0, ARITH_ROUND+k, true);
    }
    myFlow.materialArithEnableCopy(struGsh.nLayer, LCD_VERI_MAIN_VIEW_FRONT, 0, ARITH_SPL_MILDEW, true);
    myFlow.materialArithEnableCopy(struGsh.nLayer, LCD_VERI_MAIN_VIEW_FRONT, 0, ARITH_W_PEANUT, true);
    for(int k = 0; k < MAX_SUPPRESS_LARGE_TOTAL; k++)
    {
        myFlow.materialArithEnableCopy(struGsh.nLayer, LCD_VERI_MAIN_VIEW_FRONT, 0, ARITH_SUPPRESS_LARGE_A+k, true);
    }

    if((struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[2] == 1)&&(struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[3] == 1))//有辅视相机
    {
        for(int k = 0;k < MAX_GREY_COLOR_ARITH_TOTAL;k++)
        {
            myFlow.materialArithEnableCopy(struGsh.nLayer,LCD_VERI_VICE_VIEW_FRONT, 0, k, true);
        }
        for(int k = 0; k < MAX_SVM_ARITH_TOTAL; k++)
        {
            myFlow.materialArithEnableCopy(struGsh.nLayer,LCD_VERI_VICE_VIEW_FRONT, 0, ARITH_SVM_A+k, true);
        }
        for (int k = 0; k < MAX_LONG_SHORT_TOTAL; k++)
        {
            myFlow.materialArithEnableCopy(struGsh.nLayer,LCD_VERI_VICE_VIEW_FRONT, 0, ARITH_LONG_SHORT+k, true);
        }
        for (int k = 0; k < MAX_BUD_TOTAL; k++)
        {
            myFlow.materialArithEnableCopy(struGsh.nLayer,LCD_VERI_VICE_VIEW_FRONT, 0, ARITH_BUD+k, true);
        }
        for(int k = 0; k < MAX_BIG_SMALL_TOTAL; k++)
        {
            myFlow.materialArithEnableCopy(struGsh.nLayer, LCD_VERI_VICE_VIEW_FRONT, 0, ARITH_BIG_SMALL+k, true);
        }
        for(int k = 0; k < MAX_CIRCLE_LONG_TOTAL; k++)
        {
            myFlow.materialArithEnableCopy(struGsh.nLayer, LCD_VERI_VICE_VIEW_FRONT, 0, ARITH_CIRCLE_LONG+k, true);
        }
        for(int k = 0; k < MAX_HSV_TOTAL; k++)
        {
            myFlow.materialArithEnableCopy(struGsh.nLayer, LCD_VERI_VICE_VIEW_FRONT, 0, ARITH_HSV+k, true);
        }
        myFlow.materialArithEnableCopy(struGsh.nLayer, LCD_VERI_VICE_VIEW_FRONT, 0, ARITH_SAT, true);
        myFlow.materialArithEnableCopy(struGsh.nLayer, LCD_VERI_VICE_VIEW_FRONT, 0, ARITH_HUE, true);
        myFlow.materialArithEnableCopy(struGsh.nLayer, LCD_VERI_VICE_VIEW_FRONT, 0, ARITH_SCALE, true);
        myFlow.materialArithEnableCopy(struGsh.nLayer, LCD_VERI_VICE_VIEW_FRONT, 0, ARITH_MILDEW, true);
        myFlow.materialArithEnableCopy(struGsh.nLayer, LCD_VERI_VICE_VIEW_FRONT, 0, ARITH_OUTLINE, true);
        myFlow.materialArithEnableCopy(struGsh.nLayer, LCD_VERI_VICE_VIEW_FRONT, 0, ARITH_LINE, true);
        myFlow.materialArithEnableCopy(struGsh.nLayer, LCD_VERI_VICE_VIEW_FRONT, 0, ARITH_VALUE, true);
        for(int k = 0; k < MAX_ROUND_TOTAL; k++)
        {
            myFlow.materialArithEnableCopy(struGsh.nLayer, LCD_VERI_VICE_VIEW_FRONT, 0, ARITH_ROUND+k, true);
        }
        myFlow.materialArithEnableCopy(struGsh.nLayer, LCD_VERI_VICE_VIEW_FRONT, 0, ARITH_SPL_MILDEW, true);
        myFlow.materialArithEnableCopy(struGsh.nLayer, LCD_VERI_VICE_VIEW_FRONT, 0, ARITH_W_PEANUT, true);
        for(int k = 0; k < MAX_SUPPRESS_LARGE_TOTAL; k++)
        {
            myFlow.materialArithEnableCopy(struGsh.nLayer, LCD_VERI_VICE_VIEW_FRONT, 0, ARITH_SUPPRESS_LARGE_A+k, true);
        }
    }

    /* 参数发送 */
    for(int k = 0;k < MAX_GREY_COLOR_ARITH_TOTAL;k++)
    {
        myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_MAIN_VIEW_FRONT, 0, false, k, 0, true);
    }
    for(int k = 0; k < MAX_SVM_ARITH_TOTAL; k++)
    {
        myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_MAIN_VIEW_FRONT, 0, false, ARITH_SVM_A+k, 0, true);
    }
    for (int k = 0; k < MAX_LONG_SHORT_TOTAL; k++)
    {
        myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_MAIN_VIEW_FRONT, 0, false, ARITH_LONG_SHORT+k, 0, true);
    }
    for (int k = 0; k < MAX_BUD_TOTAL; k++)
    {
        myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_MAIN_VIEW_FRONT, 0, false, ARITH_BUD+k, 0, true);
    }
    for(int k = 0; k < MAX_BIG_SMALL_TOTAL; k++)
    {
        myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_MAIN_VIEW_FRONT, 0, false, ARITH_BIG_SMALL+k, 0, true);
    }
    for(int k = 0; k < MAX_CIRCLE_LONG_TOTAL; k++)
    {
        myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_MAIN_VIEW_FRONT, 0, false, ARITH_CIRCLE_LONG+k, 0, true);
    }
    myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_MAIN_VIEW_FRONT, 0, false, ARITH_SAT, 0, true);
    myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_MAIN_VIEW_FRONT, 0, false, ARITH_HUE, 0, true);
    myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_MAIN_VIEW_FRONT, 0, false, ARITH_SCALE, 0, true);

    myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_MAIN_VIEW_FRONT, 0, false, ARITH_MILDEW, 0, true);
    myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_MAIN_VIEW_FRONT, 0, false, ARITH_OUTLINE, 0, true);
    myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_MAIN_VIEW_FRONT, 0, false, ARITH_LINE, 0, true);
    myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_MAIN_VIEW_FRONT, 0, false, ARITH_VALUE, 0, true);
    for(int k = 0; k < MAX_HSV_TOTAL; k++)
    {
        myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_MAIN_VIEW_FRONT, 0, false, ARITH_HSV+k, 0,true);
    }
    for(int k = 0; k < MAX_ROUND_TOTAL; k++)
    {
        myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_MAIN_VIEW_FRONT, 0, false, ARITH_ROUND+k, 0, true);
    }
    myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_MAIN_VIEW_FRONT, 0, false, ARITH_SPL_MILDEW, 0, true);
    myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_MAIN_VIEW_FRONT, 0, false, ARITH_W_PEANUT, 0, true);
    for(int k = 0; k < MAX_SUPPRESS_LARGE_TOTAL; k++)
    {
        myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_MAIN_VIEW_FRONT, 0, false, ARITH_SUPPRESS_LARGE_A+k, 0, true);
    }

    if((struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[2] == 1)&&(struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[3] == 1))//有辅视相机
    {
        for(int k = 0;k < MAX_GREY_COLOR_ARITH_TOTAL;k++)
        {
            myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_VICE_VIEW_FRONT, 0, false, k, 0, true);
        }
        for(int k = 0; k < MAX_SVM_ARITH_TOTAL; k++)
        {
            myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_VICE_VIEW_FRONT, 0, false, ARITH_SVM_A+k, 0, true);
        }
        for (int k = 0; k < MAX_LONG_SHORT_TOTAL; k++)
        {
            myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_VICE_VIEW_FRONT, 0, false, ARITH_LONG_SHORT+k, 0, true);
        }
        for (int k = 0; k < MAX_BUD_TOTAL; k++)
        {
            myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_VICE_VIEW_FRONT, 0, false, ARITH_BUD+k, 0, true);
        }
        for(int k = 0; k < MAX_BIG_SMALL_TOTAL; k++)
        {
            myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_VICE_VIEW_FRONT, 0, false, ARITH_BIG_SMALL+k, 0, true);
        }
        for(int k = 0; k < MAX_CIRCLE_LONG_TOTAL; k++)
        {
            myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_VICE_VIEW_FRONT, 0, false, ARITH_CIRCLE_LONG+k, 0, true);
        }
        myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_VICE_VIEW_FRONT, 0, false, ARITH_SAT, 0, true);
        myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_VICE_VIEW_FRONT, 0, false, ARITH_HUE, 0, true);
        myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_VICE_VIEW_FRONT, 0, false, ARITH_SCALE, 0, true);

        myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_VICE_VIEW_FRONT, 0, false, ARITH_MILDEW, 0, true);
        myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_VICE_VIEW_FRONT, 0, false, ARITH_OUTLINE, 0, true);
        myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_VICE_VIEW_FRONT, 0, false, ARITH_LINE, 0, true);
        myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_VICE_VIEW_FRONT, 0, false, ARITH_VALUE, 0, true);
        for(int k = 0; k < MAX_HSV_TOTAL; k++)
        {
            myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_VICE_VIEW_FRONT, 0, false, ARITH_HSV+k, 0, true);
        }
        for(int k = 0; k < MAX_ROUND_TOTAL; k++)
        {
            myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_VICE_VIEW_FRONT, 0, false, ARITH_ROUND+k, 0, true);
        }
        myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_VICE_VIEW_FRONT, 0, false, ARITH_SPL_MILDEW, 0, true);
        myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_VICE_VIEW_FRONT, 0, false, ARITH_W_PEANUT, 0, true);
        for(int k = 0; k < MAX_SUPPRESS_LARGE_TOTAL; k++)
        {
            myFlow.materialParamsCopySendAssemble(struGsh.nLayer, LCD_VERI_VICE_VIEW_FRONT, 0, false, ARITH_SUPPRESS_LARGE_A+k, 0, true);
        }
    }
    updateBtnVisiable();
}

#if 0
void IdentifyWidget::on_comboBox_activated(int index)
{
    struCnfp.nMatCopySetMode = index;
    struGsh.nView = 0;
    for(int i = 0;i < MAX_VIEW;i++){
        for(int j = 0;j < MAX_GROUP_IDENTIFY;j++){
            for(int k = 0;k < MAX_GREY_COLOR_ARITH_TOTAL;k++){
                struCnfp.struGroupIdentify[struGsh.nLayer][i][j].struGreyColor[k].nDispEnable = 0;
                if(k < 3)
                    struCnfp.struGroupIdentify[struGsh.nLayer][i][j].struSvm[k].nDispEnable = 0;
            }
            for (int m = 0; m < MAX_LONG_SHORT_TOTAL; m++)
            {
                struCnfp.struGroupIdentify[struGsh.nLayer][i][j].struLongShort[m].nDispEnable = 0;
            }
            for (int m = 0; m < MAX_BUD_TOTAL; m++)
            {
                struCnfp.struGroupIdentify[struGsh.nLayer][i][j].struBud[m].nDispEnable = 0;
            }
            struCnfp.struGroupIdentify[struGsh.nLayer][i][j].struBigSmall.nDispEnable = 0;
            struCnfp.struGroupIdentify[struGsh.nLayer][i][j].struCircleLong.nDispEnable = 0;
            struCnfp.struGroupIdentify[struGsh.nLayer][i][j].struColorSat.nDispEnable = 0;
            struCnfp.struGroupIdentify[struGsh.nLayer][i][j].struColorHue.nDispEnable = 0;
            struCnfp.struGroupIdentify[struGsh.nLayer][i][j].struColorScale.nDispEnable = 0;
            struCnfp.struGroupIdentify[struGsh.nLayer][i][j].struMildew.nDispEnable = 0;
            struCnfp.struGroupIdentify[struGsh.nLayer][i][j].struOutline.nDispEnable = 0;
            struCnfp.struGroupIdentify[struGsh.nLayer][i][j].struLine.nDispEnable = 0;
            struCnfp.struGroupIdentify[struGsh.nLayer][i][j].struColorValue.nDispEnable = 0;
            struCnfp.struGroupIdentify[struGsh.nLayer][i][j].struHsv.nDispEnable = 0;
            struCnfp.struGroupIdentify[struGsh.nLayer][i][j].struRound.nDispEnable = 0;
            struCnfp.struGroupIdentify[struGsh.nLayer][i][j].struSplMildew.nDispEnable = 0;
            struCnfp.struGroupIdentify[struGsh.nLayer][i][j].struWhitePeanut.nDispEnable = 0;
        }
    }
    memset(struCnfp.nGroupArithEnable,0,sizeof(struCnfp.nGroupArithEnable));

    myFlow.resetAllArithEnableClear();
    myFlow.resetAllArithParams();

    updateBtnVisiable();
}
#endif
