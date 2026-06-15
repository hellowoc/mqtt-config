#include "sensesetwidget.h"
#include "ui_sensesetwidget.h"
#include "global.h"
SenseSetWidget::SenseSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::SenseSetWidget)
{
    ui->setupUi(this);
    m_pGroupWidget = ui->groupWidget;
    m_pViewBtn[0] = ui->pushButton_0;
    m_pViewBtn[1] = ui->pushButton_1;
    m_pViewBtn[2] = ui->pushButton_2;
    m_pViewBtn[3] = ui->pushButton_3;
    m_pViewBtn[4]= ui->pushButton_4;
    m_pViewBtn[5]= ui->pushButton_5;
    m_pViewBtn[6]= ui->pushButton_6;
    m_pViewBtn[7]= ui->pushButton_7;
    m_pLayerBtn[0] = ui->mainBtn;
    m_pLayerBtn[1] = ui->viceBtn;
    setSlotConnect();
    m_pMoreSetBtnsignalMapper = new QSignalMapper(this);

    m_pUsedBtn[0] = ui->useBtn1;
    m_pUsedBtn[1] = ui->useBtn2;
    m_pUsedBtn[2] = ui->useBtn3;
    m_pUsedBtn[3] = ui->useBtn4;
    m_pUsedBtn[4] = ui->useBtn5;
    m_pUsedBtn[5] = ui->useBtn6;
    m_pUsedBtn[6] = ui->useBtn7;
    m_pUsedBtn[7] = ui->useBtn8;
    m_pUsedBtn[8] = ui->useBtn9;
    m_pUsedBtn[9] = ui->useBtn10;
    m_pUsedBtn[10] = ui->useBtn11;
    m_pUsedBtn[11] = ui->useBtn12;
    m_pUsedBtn[12] = ui->useBtn13;
    m_pUsedBtn[13] = ui->useBtn14;
    m_pUsedBtn[14] = ui->useBtn15;
    m_pUsedBtn[15] = ui->useBtn16;
    m_pUsedBtn[16] = ui->useBtn17;
    m_pUsedBtn[17] = ui->useBtn18;
    m_pUsedBtn[18] = ui->useBtn19;
    m_pUsedBtn[19] = ui->useBtn20;
    m_pUsedBtn[20] = ui->useBtn21;

    m_pMoreSetBtn[0] = ui->moreBtn1;
    m_pMoreSetBtn[1] = ui->moreBtn2;
    m_pMoreSetBtn[2] = ui->moreBtn3;
    m_pMoreSetBtn[3] = ui->moreBtn4;
    m_pMoreSetBtn[4] = ui->moreBtn5;
    m_pMoreSetBtn[5] = ui->moreBtn6;
    m_pMoreSetBtn[6] = ui->moreBtn7;
    m_pMoreSetBtn[7] = ui->moreBtn8;
    m_pMoreSetBtn[8] = ui->moreBtn9;
    m_pMoreSetBtn[9] = ui->moreBtn10;
    m_pMoreSetBtn[10] = ui->moreBtn11;
    m_pMoreSetBtn[11] = ui->moreBtn12;
    m_pMoreSetBtn[12] = ui->moreBtn13;
    m_pMoreSetBtn[13] = ui->moreBtn14;
    m_pMoreSetBtn[14] = ui->moreBtn15;
    m_pMoreSetBtn[15] = ui->moreBtn16;
    m_pMoreSetBtn[16] = ui->moreBtn17;
    m_pMoreSetBtn[17] = ui->moreBtn18;
    m_pMoreSetBtn[18] = ui->moreBtn19;
    m_pMoreSetBtn[19] = ui->moreBtn20;
    m_pMoreSetBtn[20] = ui->moreBtn21;

    m_pViewNameLabel[0] = ui->label;
    m_pViewNameLabel[1] = ui->label_2;
    m_pViewNameLabel[2] = ui->label_3;
    m_pViewNameLabel[3] = ui->label_4;
    m_pViewNameLabel[4] = ui->label_5;
    m_pViewNameLabel[5] = ui->label_6;
    m_pViewNameLabel[6] = ui->label_7;
    m_pViewNameLabel[7] = ui->label_8;
    m_pViewNameLabel[8] = ui->label_9;
    m_pViewNameLabel[9] = ui->label_10;
    m_pViewNameLabel[10] = ui->label_11;
    m_pViewNameLabel[11] = ui->label_12;
    m_pViewNameLabel[12] = ui->label_13;
    m_pViewNameLabel[13] = ui->label_14;
    m_pViewNameLabel[14] = ui->label_15;
    m_pViewNameLabel[15] = ui->label_16;
    m_pViewNameLabel[16] = ui->label_17;
    m_pViewNameLabel[17] = ui->label_18;
    m_pViewNameLabel[18] = ui->label_19;
    m_pViewNameLabel[19] = ui->label_20;
    m_pViewNameLabel[20] = ui->label_21;

    m_pSenseNameLabel[0] = ui->senseNamelabel1;
    m_pSenseNameLabel[1] = ui->senseNamelabel2;
    m_pSenseNameLabel[2] = ui->senseNamelabel3;
    m_pSenseNameLabel[3] = ui->senseNamelabel4;
    m_pSenseNameLabel[4] = ui->senseNamelabel5;
    m_pSenseNameLabel[5] = ui->senseNamelabel6;
    m_pSenseNameLabel[6] = ui->senseNamelabel7;
    m_pSenseNameLabel[7] = ui->senseNamelabel8;
    m_pSenseNameLabel[8] = ui->senseNamelabel9;
    m_pSenseNameLabel[9] = ui->senseNamelabel10;
    m_pSenseNameLabel[10] = ui->senseNamelabel11;
    m_pSenseNameLabel[11] = ui->senseNamelabel12;
    m_pSenseNameLabel[12] = ui->senseNamelabel13;
    m_pSenseNameLabel[13] = ui->senseNamelabel14;
    m_pSenseNameLabel[14] = ui->senseNamelabel15;
    m_pSenseNameLabel[15] = ui->senseNamelabel16;
    m_pSenseNameLabel[16] = ui->senseNamelabel17;
    m_pSenseNameLabel[17] = ui->senseNamelabel18;
    m_pSenseNameLabel[18] = ui->senseNamelabel19;
    m_pSenseNameLabel[19] = ui->senseNamelabel20;
    m_pSenseNameLabel[20] = ui->senseNamelabel21;

    m_pHoriSenseProgress[0] = ui->horiProgress1;
    m_pHoriSenseProgress[1] = ui->horiProgress2;
    m_pHoriSenseProgress[2] = ui->horiProgress3;
    m_pHoriSenseProgress[3] = ui->horiProgress4;
    m_pHoriSenseProgress[4] = ui->horiProgress5;
    m_pHoriSenseProgress[5] = ui->horiProgress6;
    m_pHoriSenseProgress[6] = ui->horiProgress7;
    m_pHoriSenseProgress[7] = ui->horiProgress8;
    m_pHoriSenseProgress[8] = ui->horiProgress9;
    m_pHoriSenseProgress[9] = ui->horiProgress10;
    m_pHoriSenseProgress[10] = ui->horiProgress11;
    m_pHoriSenseProgress[11] = ui->horiProgress12;
    m_pHoriSenseProgress[12] = ui->horiProgress13;
    m_pHoriSenseProgress[13] = ui->horiProgress14;
    m_pHoriSenseProgress[14] = ui->horiProgress15;
    m_pHoriSenseProgress[15] = ui->horiProgress16;
    m_pHoriSenseProgress[16] = ui->horiProgress17;
    m_pHoriSenseProgress[17] = ui->horiProgress18;
    m_pHoriSenseProgress[18] = ui->horiProgress19;
    m_pHoriSenseProgress[19] = ui->horiProgress20;
    m_pHoriSenseProgress[20] = ui->horiProgress21;

    m_pSenseHLayout[0] = ui->horizontalLayout_2;
    m_pSenseHLayout[1] = ui->horizontalLayout_4;
    m_pSenseHLayout[2] = ui->horizontalLayout_7;
    m_pSenseHLayout[3] = ui->horizontalLayout_8;
    m_pSenseHLayout[4] = ui->horizontalLayout_11;
    m_pSenseHLayout[5] = ui->horizontalLayout_12;
    m_pSenseHLayout[6] = ui->horizontalLayout_13;
    m_pSenseHLayout[7] = ui->horizontalLayout_14;
    m_pSenseHLayout[8] = ui->horizontalLayout_20;
    m_pSenseHLayout[9] = ui->horizontalLayout_21;
    m_pSenseHLayout[10] = ui->horizontalLayout_19;
    m_pSenseHLayout[11] = ui->horizontalLayout_18;
    m_pSenseHLayout[12] = ui->horizontalLayout_22;
    m_pSenseHLayout[13] = ui->horizontalLayout_23;
    m_pSenseHLayout[14] = ui->horizontalLayout_24;
    m_pSenseHLayout[15] = ui->horizontalLayout_25;
    m_pSenseHLayout[16] = ui->horizontalLayout_26;
    m_pSenseHLayout[17] = ui->horizontalLayout_27;
    m_pSenseHLayout[18] = ui->horizontalLayout_28;
    m_pSenseHLayout[19] = ui->horizontalLayout_29;
    m_pSenseHLayout[20] = ui->horizontalLayout_30;

    for(int i = 0;i < UI_ARITH_COUNT;i++){
        connect(m_pHoriSenseProgress[i],SIGNAL(onValueChangeExt(int)),this,SLOT(onSenseValueChanged(int)));
        m_pUsedBtn[i]->setCheckable(true);
        m_pUsedBtn[i]->setObjectName("useBtn");//用于样式表调整
        m_pMoreSetBtn[i]->setObjectName("detailSetBtn");//用于样式表调整
        m_pSenseNameLabel[i]->setObjectName("senseNameLabel");
        m_pViewNameLabel[i]->setObjectName("viewNameLabel");
        m_pViewNameLabel[i]->setVisible(false);
        m_pMoreSetBtnsignalMapper->setMapping(m_pMoreSetBtn[i],i);
        m_pMoreSetBtn[i]->setText("");
        m_pUsedBtn[i]->setWhatsThis(QString::number(i));
        connect(m_pUsedBtn[i], SIGNAL(clicked(bool)), this, SLOT(onUseBtnClicked(bool)));
        connect(m_pMoreSetBtn[i],SIGNAL(clicked()),m_pMoreSetBtnsignalMapper,SLOT(map()));
        m_pSenseNameLabel[i]->setAlignment(Qt::AlignCenter);
        m_pViewNameLabel[i]->setAlignment(Qt::AlignCenter);

    }


    connect(m_pMoreSetBtnsignalMapper,SIGNAL(mapped(int)),this,SLOT(onMoreSetBtnClicked(int)));
    connect(ui->rejectRatioProgress,SIGNAL(onValueChangeExt(int)),this,SLOT(onRatioValueChanged(int)));
    shottimer = new QTimer;
    connect(shottimer,SIGNAL(timeout()),this, SLOT(getscreenshot()));


}

SenseSetWidget::~SenseSetWidget()
{
    delete ui;
}

void SenseSetWidget::showPage(bool bshow)
{
    basewidget::showPage(bshow);
    if(bshow){
        updateGroupWidget();
        updateViewButtonVisible();
        updateViewBtnState();
        updateGroupWidgetState();

        /* 色选次数为1或滑槽数为1 */
        if((struCnfg.struLayerInfo[struGsh.nLayer].stuVavleInfo[0].nTickGroupTotal == 1))
        {
            ui->groupWidget->hide();
        }
        else
        {
            ui->groupWidget->show();
        }
        initSense();

        if( struGsh.bScreenShotStatus ==1)
        {
            shottimer->start(1000);

        }
    }
}

void SenseSetWidget::OnViewChange(int view)
{
    initSense();
}

void SenseSetWidget::OnGroupChange(int groupIndex)
{
    initSense();
}

void SenseSetWidget::OnLayerChange()
{
    initSense();
}

void SenseSetWidget::getscreenshot()
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


void SenseSetWidget::onSenseValueChanged(int value)
{
    UiHorizontalProgressExt* pHor = qobject_cast<UiHorizontalProgressExt*>(sender());
    int index = -1;
    for(int i = 0; i < m_SenseDataList.size(); i++)
    {
        if(pHor == m_pHoriSenseProgress[i]){
            index = i;
            break;
        }
    }
    if(index < 0)
        return;
    int arithIndex = m_SenseDataList[index].m_senseIndex;
    int view = m_SenseDataList[index].m_view;

    int group = struGsh.nMatGroup;

    /* 带分割的大机器，默认全部使用一次灵敏度*/
    if(struCnfg.nMachineSpliceType  == MACHINE_SPLICE_TYPE_BIG)
    {
        group = 0;
    }

//    mOldHighHue = struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv.nHsvSensUpH;
//    mOldLowHue = struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv.nHsvSensLowH;


    switch (arithIndex) {
    case ARITH_GREY_A:
    case ARITH_GREY_B:
    case ARITH_GREY_C:
    case ARITH_GREY_D:
    case ARITH_DISCOLOR_A:
    case ARITH_DISCOLOR_B:
    case ARITH_CROSS:
        if(struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struGreyColor[arithIndex].nMode == 0)
            struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struGreyColor[arithIndex].nSensLow = value;
        else
            struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struGreyColor[arithIndex].nSensUp = value;

        /*  带分割的机器，灵敏度一致 */
        if(struCnfg.nMachineSpliceType  == MACHINE_SPLICE_TYPE_BIG)
        {
            if(struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struGreyColor[arithIndex].nMode == 0)
            {
                for(int i = 0; i < MAX_GROUP_IDENTIFY; i++)
                {
                    struCnfp.struGroupIdentify[struGsh.nLayer][view][i].struGreyColor[arithIndex].nSensLow = value;
                }
            }
            else
            {
                for(int i = 0; i < MAX_GROUP_IDENTIFY; i++)
                {
                    struCnfp.struGroupIdentify[struGsh.nLayer][view][i].struGreyColor[arithIndex].nSensUp = value;
                }
            }
        }
        break;
    case ARITH_SVM_A:
    case ARITH_SVM_B:
    case ARITH_SVM_C:
    case ARITH_SVM_D:
        struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struSvm[arithIndex-ARITH_SVM_A].nSens = value;
        /*  带分割的机器，灵敏度一致 */
        if(struCnfg.nMachineSpliceType  == MACHINE_SPLICE_TYPE_BIG)
        {
            for(int i = 0; i < MAX_GROUP_IDENTIFY; i++)
            {
                struCnfp.struGroupIdentify[struGsh.nLayer][view][i].struSvm[arithIndex-ARITH_SVM_A].nSens = value;
            }
        }
        break;
    case ARITH_SHAPE_SVM_A:
    case ARITH_SHAPE_SVM_B:
    case ARITH_SHAPE_SVM_C:
        struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struShapeSvm[arithIndex-ARITH_SHAPE_SVM_A].nSens = value;
        /*  带分割的机器，灵敏度一致 */
        if(struCnfg.nMachineSpliceType  == MACHINE_SPLICE_TYPE_BIG)
        {
            for(int i = 0; i < MAX_GROUP_IDENTIFY; i++)
            {
                struCnfp.struGroupIdentify[struGsh.nLayer][view][i].struShapeSvm[arithIndex-ARITH_SHAPE_SVM_A].nSens = value;
            }
        }
        break;
    case ARITH_BIG_SMALL:
    case ARITH_BIG_SMALL_B:
        if(struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struBigSmall[arithIndex-ARITH_BIG_SMALL].nMode ==0){
            struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struBigSmall[arithIndex-ARITH_BIG_SMALL].nAreaLow = value;
        }else{
            struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struBigSmall[arithIndex-ARITH_BIG_SMALL].nAreaUp = value;
        }
        /*  带分割的机器，灵敏度一致 */
        if(struCnfg.nMachineSpliceType  == MACHINE_SPLICE_TYPE_BIG)
        {
            if(struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struBigSmall[arithIndex-ARITH_BIG_SMALL].nMode ==0)
            {
                for(int i = 0; i < MAX_GROUP_IDENTIFY; i++)
                {
                    struCnfp.struGroupIdentify[struGsh.nLayer][view][i].struBigSmall[arithIndex-ARITH_BIG_SMALL].nAreaLow = value;
                }
            }
            else
            {
                for(int i = 0; i < MAX_GROUP_IDENTIFY; i++)
                {
                    struCnfp.struGroupIdentify[struGsh.nLayer][view][i].struBigSmall[arithIndex-ARITH_BIG_SMALL].nAreaUp = value;
                }
            }
        }
        break;
    case ARITH_LONG_SHORT:
    case ARITH_LONG_SHORT_B:
        if(struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struLongShort[arithIndex-ARITH_LONG_SHORT].nLengthMode == 0){
            struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struLongShort[arithIndex-ARITH_LONG_SHORT].nLengthLow = value;
        }else{
            struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struLongShort[arithIndex-ARITH_LONG_SHORT].nLengthUp = value;
        }
        break;
    case ARITH_CIRCLE_LONG:
    case ARITH_CIRCLE_LONG_B:
        if(struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struCircleLong[arithIndex-ARITH_CIRCLE_LONG].nMode == 0){
            struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struCircleLong[arithIndex-ARITH_CIRCLE_LONG].nCiloLow = value;
        }else{
            struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struCircleLong[arithIndex-ARITH_CIRCLE_LONG].nCilosUp = value;
        }
        break;
    case ARITH_HSV:
    case ARITH_HSV_B:
    case ARITH_HSV_C:
        switch(struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struHsv[arithIndex-ARITH_HSV].nHsvColor)
        {
        case 0:
            if(struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struHsv[arithIndex-ARITH_HSV].nHsvMode == 0){
                struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struHsv[arithIndex-ARITH_HSV].nHsvSensLowH = value;
            }else{
                struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struHsv[arithIndex-ARITH_HSV].nHsvSensUpH = value;
            }
            break;
        case 1:
            if(struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struHsv[arithIndex-ARITH_HSV].nHsvMode == 0){
                struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struHsv[arithIndex-ARITH_HSV].nHsvSensLowS = value;
            }else{
                struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struHsv[arithIndex-ARITH_HSV].nHsvSensUpS = value;
            }
            break;
        case 2:
            if(struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struHsv[arithIndex-ARITH_HSV].nHsvMode == 0){
                struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struHsv[arithIndex-ARITH_HSV].nHsvSensLowV = value;
            }else{
                struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struHsv[arithIndex-ARITH_HSV].nHsvSensUpV = value;
            }
            break;
        }

//        // 颜色单独添加部分
//        stru_iden_group_info groupInfo;
//        memcpy(&groupInfo, &struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nView].stuIdenGupInfo[struGsh.nMatGroup], sizeof(groupInfo));
//        for( int i = 0; i < (2 * groupInfo.nGroupUnitTotal); i++ )
//        {
//            if( struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv.nHsvSensUpH - mOldHighHue > 0 )
//            {
//                struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][i][0].hueH = struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][i][0].hueH
//                        + (struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv.nHsvSensUpH - mOldHighHue);
//            }
//            else
//            {
//                struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][i][0].hueH = struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][i][0].hueH
//                        + (struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv.nHsvSensUpH - mOldHighHue);
//            }

//            if( struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv.nHsvSensLowH - mOldLowHue > 0 )
//            {
//                struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][i][0].hueL = struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][i][0].hueL
//                        + (struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv.nHsvSensLowH - mOldLowHue);
//            }
//            else
//            {
//                struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][i][0].hueL = struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][i][0].hueL
//                        + (struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv.nHsvSensLowH - mOldLowHue);
//            }

//            if( struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][i][0].hueH > 359 )
//            {
//                struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][i][0].hueH = 359;
//            }
//            else if ( struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][i][0].hueH < 0 )
//            {
//                struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][i][0].hueH = 0;
//            }

//            if( struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][i][0].hueL > 359 )
//            {
//                struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][i][0].hueL = 359;
//            }
//            else if ( struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][i][0].hueL < 0 )
//            {
//                struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView][i][0].hueL = 0;
//            }

//        }

//        if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT)
//        {
//            for( int i = 0; i < (2 * groupInfo.nGroupUnitTotal); i++ )
//            {
//                if( struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv.nHsvSensUpH - mOldHighHue > 0 )
//                {
//                    struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][i][0].hueH = struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][i][0].hueH
//                            + (struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv.nHsvSensUpH - mOldHighHue);
//                }
//                else
//                {
//                    struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][i][0].hueH = struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][i][0].hueH
//                            + (struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv.nHsvSensUpH - mOldHighHue);
//                }

//                if( struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv.nHsvSensLowH - mOldLowHue > 0 )
//                {
//                    struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][i][0].hueL = struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][i][0].hueL
//                            + (struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv.nHsvSensLowH - mOldLowHue);
//                }
//                else
//                {
//                    struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][i][0].hueL = struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][i][0].hueL
//                            + (struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv.nHsvSensLowH - mOldLowHue);
//                }

//                if( struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][i][0].hueH > 359 )
//                {
//                    struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][i][0].hueH = 359;
//                }
//                else if ( struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][i][0].hueH < 0 )
//                {
//                    struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][i][0].hueH = 0;
//                }

//                if( struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][i][0].hueL > 359 )
//                {
//                    struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][i][0].hueL = 359;
//                }
//                else if ( struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][i][0].hueL < 0 )
//                {
//                    struCnfp.struUnitSens[struGsh.nLayer][struGsh.nView+1][i][0].hueL = 0;
//                }

//            }
//        }
        break;
    case ARITH_SAT:
        if(struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struColorSat.nMode == 0){
            struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struColorSat.nSensLow = value;
        }else{
            struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struColorSat.nSensUp = value;
        }
        if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT){//前后相同或整机相同时
            if(struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struColorSat.nMode == 0){
                struCnfp.struGroupIdentify[struGsh.nLayer][view+1][group].struColorSat.nSensLow = value;
            }else{
                struCnfp.struGroupIdentify[struGsh.nLayer][view+1][group].struColorSat.nSensUp = value;
            }
        }
        break;
    case ARITH_HUE:
        struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struColorHue.nHighHue = value;
        if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT){//前后相同或整机相同时
            struCnfp.struGroupIdentify[struGsh.nLayer][view+1][group].struColorHue.nHighHue = value;
        }
        break;
    case ARITH_SCALE:
        struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struColorScale.nSen = value;
        if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT){//前后相同或整机相同时
            struCnfp.struGroupIdentify[struGsh.nLayer][view+1][group].struColorScale.nSen = value;
        }
        break;
    case ARITH_MILDEW:
        struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struMildew.nSens = value;
        if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT){//前后相同或整机相同时
            struCnfp.struGroupIdentify[struGsh.nLayer][view+1][group].struMildew.nSens = value;
        }
        break;
    case ARITH_OUTLINE:
        struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struOutline.nSens  = value;
        if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT){//前后相同或整机相同时
            struCnfp.struGroupIdentify[struGsh.nLayer][view+1][group].struOutline.nSens = value;
        }
        break;
    case ARITH_LINE:
        struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struLine.nSens1  = value;
        if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT){//前后相同或整机相同时
            struCnfp.struGroupIdentify[struGsh.nLayer][view+1][group].struLine.nSens1 = value;
        }
        break;
    case ARITH_BUD:
    case ARITH_BUD_B:
        struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struBud[arithIndex-ARITH_BUD].nSens  = value;
        if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT){//前后相同或整机相同时
            struCnfp.struGroupIdentify[struGsh.nLayer][view+1][group].struBud[arithIndex-ARITH_BUD].nSens = value;
        }
        break;
    case ARITH_ROUND:
    case ARITH_ROUND_B:
        if(struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struRound[arithIndex-ARITH_ROUND].nSelectMode == 0){
            struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struRound[arithIndex-ARITH_ROUND].nSensUp = value;
        }else{
            struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struRound[arithIndex-ARITH_ROUND].nSensLow = value;
        }
        if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT)
        {
            if(struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struRound[arithIndex-ARITH_ROUND].nSelectMode == 0){
                struCnfp.struGroupIdentify[struGsh.nLayer][view+1][group].struRound[arithIndex-ARITH_ROUND].nSensUp = value;
            }else{
                struCnfp.struGroupIdentify[struGsh.nLayer][view+1][group].struRound[arithIndex-ARITH_ROUND].nSensLow = value;
            }
        }
        break;
    case ARITH_VALUE:
        if(struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struColorValue.nMode == 0){
            struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struColorValue.nSensLow = value;
        }else{
            struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struColorValue.nSensUp = value;
        }
        if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT){//前后相同或整机相同时
            if(struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struColorValue.nMode == 0){
                struCnfp.struGroupIdentify[struGsh.nLayer][view+1][group].struColorValue.nSensLow = value;
            }else{
                struCnfp.struGroupIdentify[struGsh.nLayer][view+1][group].struColorValue.nSensUp = value;
            }
        }
        break;
    case ARITH_SPL_MILDEW:
        struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struSplMildew.nSens = value;
        if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT){//前后相同或整机相同时
            struCnfp.struGroupIdentify[struGsh.nLayer][view+1][group].struSplMildew.nSens = value;
        }
        break;
    case ARITH_W_PEANUT:
        struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struWhitePeanut.nSensLow = value;
        if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT){//前后相同或整机相同时
            struCnfp.struGroupIdentify[struGsh.nLayer][view+1][group].struWhitePeanut.nSensLow = value;
        }
        break;
    case ARITH_PEPPER_HANDLE:
        struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struPepperHandle.nSens = value;
        if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT){//前后相同或整机相同时
            struCnfp.struGroupIdentify[struGsh.nLayer][view+1][group].struPepperHandle.nSens = value;
        }
        break;
//    case ARITH_SUPPRESS_LARGE_A:
//    case ARITH_SUPPRESS_LARGE_B:
//    case ARITH_SUPPRESS_LARGE_C:

//        struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struSuppressLarge[arithIndex-ARITH_SUPPRESS_LARGE_A].nSens = value;

//        break;
    case ARITH_WHEAT_SPROUT:
        struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struWheatSprout.nSense = value;
        if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT){//前后相同或整机相同时
            struCnfp.struGroupIdentify[struGsh.nLayer][view+1][group].struWheatSprout.nSense = value;
        }
        break;
    case TEA_ARITH_COLOR:
    case TEA_ARITH_COLOR_2:
    case TEA_ARITH_COLOR_3:
    case TEA_ARITH_COLOR_4:
    case TEA_ARITH_COLOR_5:
        struCnfp.struGroupIdentify[struGsh.nLayer][view][group].struTeaColor[arithIndex-TEA_ARITH_COLOR].sense = value/10.0;

        if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT)
        {
            struCnfp.struGroupIdentify[struGsh.nLayer][view+1][group].struTeaColor[arithIndex-TEA_ARITH_COLOR].sense = value/10.0;
        }
        break;
    default:
        break;
    }
    myFlow.materialParamsCopySendAssemble(struGsh.nLayer, view, group, false, arithIndex, 0, true);
}

void SenseSetWidget::onUseBtnClicked(bool checked)
{
    QCheckBox *btn = (QCheckBox*)(sender());
    int index = btn->whatsThis().toInt();
    int enable = (int)m_pUsedBtn[index]->isChecked();
    int arithIndex = m_SenseDataList[index].m_senseIndex;
    int layer = struGsh.nLayer;
    int view = m_SenseDataList[index].m_view;
    int group = struGsh.nMatGroup;

    struCnfp.nGroupArithEnable[layer][view][group][arithIndex] = enable;    //用于仿真使用

    switch (arithIndex) {
    case ARITH_GREY_A:
    case ARITH_GREY_B:
    case ARITH_GREY_C:
    case ARITH_GREY_D:
    case ARITH_DISCOLOR_A:
    case ARITH_DISCOLOR_B:
    case ARITH_CROSS:
        struCnfp.struGroupIdentify[layer][view][group].struGreyColor[arithIndex].nArithEnable = enable;
        break;
    case ARITH_SVM_A:
    case ARITH_SVM_B:
    case ARITH_SVM_C:
    case ARITH_SVM_D:
        struCnfp.struGroupIdentify[layer][view][group].struSvm[arithIndex-ARITH_SVM_A].nArithEnable = enable;
        break;
    case ARITH_SHAPE_SVM_A:
    case ARITH_SHAPE_SVM_B:
    case ARITH_SHAPE_SVM_C:
        struCnfp.struGroupIdentify[layer][view][group].struShapeSvm[arithIndex-ARITH_SHAPE_SVM_A].nArithEnable = enable;
        break;
    case ARITH_BIG_SMALL:
    case ARITH_BIG_SMALL_B:
        struCnfp.struGroupIdentify[layer][view][group].struBigSmall[arithIndex-ARITH_BIG_SMALL].nArithEnable = enable;
        break;
    case ARITH_LONG_SHORT:
    case ARITH_LONG_SHORT_B:
        struCnfp.struGroupIdentify[layer][view][group].struLongShort[arithIndex-ARITH_LONG_SHORT].nArithEnable = enable;
        break;
    case ARITH_CIRCLE_LONG:
    case ARITH_CIRCLE_LONG_B:
        struCnfp.struGroupIdentify[layer][view][group].struCircleLong[arithIndex-ARITH_CIRCLE_LONG].nArithEnable = enable;
        break;
    case ARITH_HSV:
    case ARITH_HSV_B:
    case ARITH_HSV_C:
        struCnfp.struGroupIdentify[layer][view][group].struHsv[arithIndex-ARITH_HSV].nArithEnable = enable;
        break;
    case ARITH_SAT:
        struCnfp.struGroupIdentify[layer][view][group].struColorSat.nArithEnable = enable;
        break;
    case ARITH_HUE:
        struCnfp.struGroupIdentify[layer][view][group].struColorHue.nArithEnable = enable;
        break;
    case ARITH_SCALE:
        struCnfp.struGroupIdentify[layer][view][group].struColorScale.nArithEnable = enable;
        break;
    case ARITH_MILDEW:
        struCnfp.struGroupIdentify[layer][view][group].struMildew.nArithEnable  = enable;
        break;
    case ARITH_OUTLINE:
        struCnfp.struGroupIdentify[layer][view][group].struOutline.nArithEnable = enable;
        break;
    case ARITH_LINE:
        struCnfp.struGroupIdentify[layer][view][group].struLine.nArithEnable = enable;
        break;
    case ARITH_BUD:
    case ARITH_BUD_B:
        struCnfp.struGroupIdentify[layer][view][group].struBud[arithIndex-ARITH_BUD].nArithEnable = enable;
        break;
    case ARITH_ROUND:
    case ARITH_ROUND_B:
        struCnfp.struGroupIdentify[layer][view][group].struRound[arithIndex-ARITH_ROUND].nArithEnable = enable;
        break;
    case ARITH_VALUE:
        struCnfp.struGroupIdentify[layer][view][group].struColorValue.nArithEnable = enable;
        break;
    case ARITH_SPL_MILDEW:
        struCnfp.struGroupIdentify[layer][view][group].struSplMildew.nArithEnable  = enable;
        break;
    case ARITH_W_PEANUT:
        struCnfp.struGroupIdentify[layer][view][group].struWhitePeanut.nArithEnable  = enable;
        break;
    case ARITH_PEPPER_HANDLE:
        struCnfp.struGroupIdentify[layer][view][group].struPepperHandle.nArithEnable  = enable;
        break;
    case ARITH_SUPPRESS_LARGE_A:
    case ARITH_SUPPRESS_LARGE_B:
    case ARITH_SUPPRESS_LARGE_C:
        struCnfp.struGroupIdentify[layer][view][group].struSuppressLarge[arithIndex-ARITH_SUPPRESS_LARGE_A].nArithEnable = enable;
        break;
    case ARITH_WHEAT_SPROUT:
        struCnfp.struGroupIdentify[layer][view][group].struWheatSprout.nArithEnable  = enable;
        break;
    case TEA_ARITH_COLOR:
    case TEA_ARITH_COLOR_2:
    case TEA_ARITH_COLOR_3:
    case TEA_ARITH_COLOR_4:
    case TEA_ARITH_COLOR_5:
        struCnfp.struGroupIdentify[layer][view][group].struTeaColor[arithIndex-TEA_ARITH_COLOR].nArithEnable  = enable;
        break;
    default:
        break;
    }

    myFlow.materialArithEnableCopy(layer, view, group, arithIndex, true); //根据参数设置方式，算法使能
    myFlow.materialParamsCopySendAssemble(layer, view, group, false, arithIndex, 0, true);

    if(struCnfg.nMachineSpliceType == MACHINE_SPLICE_TYPE_BIG)
    {
        if((group > 0)
                &&(struCnfg.struLayerInfo[layer].stuViewInfo[view].stuIdenGupInfo[group].nGroupUnitTotal == 1)       //只有一个相机板,且被分割
                &&(struCnfg.struLayerInfo[layer].stuViewInfo[view].stuIdenGupInfo[group].nUnitGupSplice[0] == 2))     //2表示右半边
        {
            myFlow.materialParamsCopySendAssemble(layer, view, group-1, false, arithIndex, 0, true);
        }
    }

//    myFlow.resetArithmeticEnable();
}

void SenseSetWidget::onMoreSetBtnClicked(int index)
{
    int arithIndex = m_SenseDataList[index].m_senseIndex;
    struGsh.nView = m_SenseDataList[index].m_view;
    struGsh.nArithIndex = arithIndex;
    switch (arithIndex) {
    case ARITH_GREY_A:
    case ARITH_GREY_B:
    case ARITH_GREY_C:
    case ARITH_GREY_D:
        g_MainManager().ShowWidget(SM_GRAYSENSE_SET_WIDGET);
        break;
    case ARITH_DISCOLOR_A:
    case ARITH_DISCOLOR_B:
    case ARITH_CROSS:
        g_MainManager().ShowWidget(SM_DISCOLOR_SENSE_SET_WIDGET);
        break;
    case ARITH_SVM_A:
    case ARITH_SVM_B:
    case ARITH_SVM_C:
    case ARITH_SVM_D:
        g_MainManager().ShowWidget(SM_SVM_SENSE_SET_WIDGET);
        break;
    case ARITH_SHAPE_SVM_A:
    case ARITH_SHAPE_SVM_B:
    case ARITH_SHAPE_SVM_C:
        g_MainManager().ShowWidget(SM_SHAPE_SVM_SENSE_SET_WIDGET);
        break;
    case ARITH_BIG_SMALL:
    case ARITH_BIG_SMALL_B:
        g_MainManager().ShowWidget(SM_BIG_SMALL_SENSE_SET_WIDGET);
        break;
    case ARITH_LONG_SHORT:
    case ARITH_LONG_SHORT_B:
        g_MainManager().ShowWidget(SM_LONG_SHORT_SENSE_SET_WIDGET);
        break;
    case ARITH_CIRCLE_LONG:
    case ARITH_CIRCLE_LONG_B:
        g_MainManager().ShowWidget(SM_CIRCLE_LONG_SENSE_SET_WIDGET);
        break;
    case ARITH_HSV:
    case ARITH_HSV_B:
    case ARITH_HSV_C:
        g_MainManager().ShowWidget(SM_HSV_SENSE_SET_WIDGET);
        break;
    case ARITH_SAT:
        g_MainManager().ShowWidget(SM_COLOR_SAT_SENSE_SET_WIDGET);
        break;
    case ARITH_HUE:
        g_MainManager().ShowWidget(SM_COLOR_HUE_SENSE_SET_WIDGET);
        break;
    case ARITH_SCALE:
        g_MainManager().ShowWidget(SM_COLOR_SCALE_SENSE_SET_WIDGET);
        break;
    case ARITH_MILDEW:
        g_MainManager().ShowWidget(SM_MILDEW_SENSE_SET_WIDGET);
        break;
    case ARITH_OUTLINE:
        g_MainManager().ShowWidget(SM_OUTLINE_SENSE_SET_WIDGET);
        break;
    case ARITH_LINE:
        g_MainManager().ShowWidget(SM_LINE_SENSE_SET_WIDGET);
        break;
    case ARITH_BUD:
    case ARITH_BUD_B:
        g_MainManager().ShowWidget(SM_BUD_SENSE_SET_WIDGET);
        break;
    case ARITH_ROUND:
    case ARITH_ROUND_B:
        g_MainManager().ShowWidget(SM_ROUND_SENSE_SET_WIDGET);
        break;
    case ARITH_VALUE:
        g_MainManager().ShowWidget(SM_COLOR_VALUE_SENSE_SET_WIDGET);
        break;
    case ARITH_SPL_MILDEW:
        g_MainManager().ShowWidget(SM_SPL_MILDEW_SENSE_SET_WIDGET);
        break;
    case ARITH_W_PEANUT:
        g_MainManager().ShowWidget(SM_WHITE_PEANUT_SENSE_SET_WIDGET);
        break;
    case ARITH_PEPPER_HANDLE:
        g_MainManager().ShowWidget(SM_PEPPER_HANDLE_SENSE_SET_WIDGET);
        break;
    case ARITH_SUPPRESS_LARGE_A:
    case ARITH_SUPPRESS_LARGE_B:
    case ARITH_SUPPRESS_LARGE_C:
        g_MainManager().ShowWidget(SM_SUPPRESS_LARGE_SENSE_SET_WIDGET);
        break;
    case ARITH_WHEAT_SPROUT :
        g_MainManager().ShowWidget(SM_WHEAT_SPROUT_SENSE_SET_WIDGET);
        break;
    case TEA_ARITH_COLOR:
    case TEA_ARITH_COLOR_2:
    case TEA_ARITH_COLOR_3:
    case TEA_ARITH_COLOR_4:
    case TEA_ARITH_COLOR_5:
        g_MainManager().ShowWidget(SM_TEA_COLOR_SENSE_SET_WIDGET);
        break;
    default:
        break;
    }
}

void SenseSetWidget::retranslateUiWidget()
{
    ui->rejectRatioLbl->setText(g_myLan().ids_arithRejectRatio);
}

void SenseSetWidget::initSense()
{
    if(struCnfg.nMachineSpliceType == MACHINE_SPLICE_TYPE_NULL)
    {
        loadSenseData();
    }
    else
    {
        loadSenseDataSplice();
    }

    for(int i = 0;i < UI_ARITH_COUNT;i++){
        if(i < m_SenseDataList.count()){
            m_pUsedBtn[i]->setChecked((bool)m_SenseDataList[i].m_bUsed);
            if(m_SenseDataList[i].m_senseIndex == ARITH_DISCOLOR_A || m_SenseDataList[i].m_senseIndex == ARITH_DISCOLOR_B
                    || m_SenseDataList[i].m_senseIndex == ARITH_CROSS ||
                    (m_SenseDataList[i].m_senseIndex >= TEA_ARITH_COLOR &&  m_SenseDataList[i].m_senseIndex <= TEA_ARITH_COLOR_5 ))
                m_pHoriSenseProgress[i]->setbPercent(true);
            else if((m_SenseDataList[i].m_senseIndex == ARITH_HSV && struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv[0].nHsvColor == 0)
                    ||(m_SenseDataList[i].m_senseIndex == ARITH_HSV_B && struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv[1].nHsvColor == 0)
                    ||(m_SenseDataList[i].m_senseIndex == ARITH_HSV_C && struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].struHsv[2].nHsvColor == 0))
                                   m_pHoriSenseProgress[i]->setbPercent(3);
            else
                m_pHoriSenseProgress[i]->setbPercent(false);
            m_pHoriSenseProgress[i]->setMaxAndMinValue(m_SenseDataList[i].m_max,m_SenseDataList[i].m_min);
            m_pHoriSenseProgress[i]->setCurrentValue(m_SenseDataList[i].m_value);
            m_pSenseNameLabel[i]->setText(m_SenseDataList[i].m_senseName);

            switch(m_SenseDataList[i].m_view){
            case 0:
                if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT){
                    m_pViewNameLabel[i]->setText(g_myLan().ids_master);
                }else{
                    m_pViewNameLabel[i]->setText(g_myLan().ids_fm);
                }
                break;
            case 1:
                m_pViewNameLabel[i]->setText(g_myLan().ids_bm);
                break;
            case 2:
                if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT){
                    m_pViewNameLabel[i]->setText(g_myLan().front_rear_vice);
                }else{
                    m_pViewNameLabel[i]->setText(g_myLan().ids_fs);
                }
                break;
            case 3:
                m_pViewNameLabel[i]->setText(g_myLan().ids_bs);
                break;
            default:
                break;
            }

            m_pUsedBtn[i]->show();
            m_pMoreSetBtn[i]->show();
            m_pSenseNameLabel[i]->show();
            m_pHoriSenseProgress[i]->show();
        }else{
            m_pUsedBtn[i]->hide();
            m_pMoreSetBtn[i]->hide();
            m_pSenseNameLabel[i]->hide();
            m_pHoriSenseProgress[i]->hide();
            ui->verticalLayout->removeItem(m_pSenseHLayout[i]);
        }
    }
    updateSenseByRunMode();
    //控碎
    ui->rejectRatioGroup->setVisible(struCnfg.nDPMachine & struCnfg.nUseIpcEnable & struCnfg.nUseBMModule);
    ui->rejectRatioProgress->setMaxAndMinValue(1000,0);
    ui->rejectRatioProgress->setCurrentValue(struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].arithRejectRatio);
}

void SenseSetWidget::onRatioValueChanged(int value)
{
    struCnfp.struGroupIdentify[struGsh.nLayer][struGsh.nView][struGsh.nMatGroup].arithRejectRatio = value;
    myFlow.resetArithRejectRatio();
}

void SenseSetWidget::loadSenseData()
{
    m_SenseDataList.clear();

    int group = struGsh.nMatGroup;
    int view = struGsh.nView;
    QString strSenseName;
    int used = 0;
    int value =50;
    int  max = 100;
    int min = 0;
    int index = 0;
    stu_group_identify senseIdentify;

//    for(int view = 0;view < struCnfg.struLayerInfo[struGsh.nLayer].nViewChainTotal;view++){
//        if(struCnfp.nMatCopySetMode == MAT_COPY_ALL_SAME || struCnfp.nMatCopySetMode == MAT_COPY_REAR_ALIKE_FRONT){//前后视相同，只显示一个视
//            if(view == 1 || view == 3){
//                continue;
//            }
//        }
        memcpy(&senseIdentify,&struCnfp.struGroupIdentify[struGsh.nLayer][view][group],sizeof(senseIdentify));

        for(int i = 0;i < MAX_GREY_COLOR_ARITH_TOTAL;i++){
            if(senseIdentify.struGreyColor[i].nDispEnable){
                index = i;
                min = 0;

                switch (i) {
                case ARITH_GREY_A:
                case ARITH_GREY_B:
                case ARITH_GREY_C:
                case ARITH_GREY_D:
                    max = ARITH_GREY_MAX_SENS;
                    break;
                case ARITH_DISCOLOR_A:
                case ARITH_DISCOLOR_B:
                case ARITH_CROSS:
                    max = ARITH_COLOR_MAX_SENS;
                    break;
                default:
                    break;
                }
                if(senseIdentify.struGreyColor[i].nMode){
                    value = senseIdentify.struGreyColor[i].nSensUp;
                }else{
                    value = senseIdentify.struGreyColor[i].nSensLow;
                }
                used = senseIdentify.struGreyColor[i].nArithEnable;

                QByteArray senseNameArr(senseIdentify.struGreyColor[i].sName);
                strSenseName = QString::fromUtf8(senseNameArr);
                SingleSenseData sense(index,value,strSenseName,max,min,view,used);
                m_SenseDataList.append(sense);
            }
        }

        for(int i = 0;i < MAX_SVM_ARITH_TOTAL;i++){
            if(senseIdentify.struSvm[i].nDispEnable){
                index = ARITH_SVM_A+i;
                min = 100-senseIdentify.struSvm[i].maxSenseValue;
                max = 100+senseIdentify.struSvm[i].maxSenseValue;
                if(senseIdentify.struSvm[i].nSens<min || senseIdentify.struSvm[i].nSens>max)
                    value = 100;
                else
                    value = senseIdentify.struSvm[i].nSens;
                used = senseIdentify.struSvm[i].nArithEnable;
                QByteArray senseNameArr(senseIdentify.struSvm[i].sName);
                strSenseName = QString::fromUtf8(senseNameArr);
                SingleSenseData sense(index,value,strSenseName,max,min,view,used);
                m_SenseDataList.append(sense);
            }
        }
        for(int i = 0;i < MAX_SHAPE_SVM_ARITH_TOTAL;i++){
            if(senseIdentify.struShapeSvm[i].nDispEnable){
                index = ARITH_SHAPE_SVM_A+i;
                min = 1;
                max = max_svm;
                value = senseIdentify.struShapeSvm[i].nSens;
                used = senseIdentify.struShapeSvm[i].nArithEnable;
                QByteArray senseNameArr(senseIdentify.struShapeSvm[i].sName);
                strSenseName = QString::fromUtf8(senseNameArr);
                SingleSenseData sense(index,value,strSenseName,max,min,view,used);
                m_SenseDataList.append(sense);
            }
        }

        for(int i = 0;i < MAX_BIG_SMALL_TOTAL;i++){
            if(senseIdentify.struBigSmall[i].nDispEnable){
                index = ARITH_BIG_SMALL+i;
                min = 0;
                max = struCnfg.nBigSmallArithMaxValue;
                if(senseIdentify.struBigSmall[i].nMode)
                    value = senseIdentify.struBigSmall[i].nAreaUp;
                else
                    value = senseIdentify.struBigSmall[i].nAreaLow;
                used = senseIdentify.struBigSmall[i].nArithEnable;
                QByteArray senseNameArr(senseIdentify.struBigSmall[i].sName);
                strSenseName = QString::fromUtf8(senseNameArr);
                SingleSenseData sense(index,value,strSenseName,max,min,view,used);
                m_SenseDataList.append(sense);
            }
        }

        for(int i = 0;i < MAX_CIRCLE_LONG_TOTAL;i++){
            if(senseIdentify.struCircleLong[i].nDispEnable){
                index = ARITH_CIRCLE_LONG+i;
                min = 0;
                max = ARITH_LONG_CIRCLE_MAX_VALUE;
                if(senseIdentify.struCircleLong[i].nMode)
                    value = senseIdentify.struCircleLong[i].nCilosUp;
                else
                    value = senseIdentify.struCircleLong[i].nCiloLow;
                used = senseIdentify.struCircleLong[i].nArithEnable;
                QByteArray senseNameArr(senseIdentify.struCircleLong[i].sName);
                strSenseName = QString::fromUtf8(senseNameArr);
                SingleSenseData sense(index,value,strSenseName,max,min,view,used);
                m_SenseDataList.append(sense);
            }
        }

        for(int i = 0;i < MAX_LONG_SHORT_TOTAL;i++){
            if(senseIdentify.struLongShort[i].nDispEnable){
                index = ARITH_LONG_SHORT+i;
                min = 0;
                max = ARITH_LENGTH_MAX_VALUE;
                if(senseIdentify.struLongShort[i].nLengthMode)
                    value = senseIdentify.struLongShort[i].nLengthUp;
                else
                    value = senseIdentify.struLongShort[i].nLengthLow;

                used = senseIdentify.struLongShort[i].nArithEnable;
                QByteArray senseNameArr(senseIdentify.struLongShort[i].sName);
                strSenseName = QString::fromUtf8(senseNameArr);
                SingleSenseData sense(index,value,strSenseName,max,min,view,used);
                m_SenseDataList.append(sense);
            }
        }

        // 饱和
        if(senseIdentify.struColorSat.nDispEnable){
            index = ARITH_SAT;
            min = 0;
            max = ARITH_SAT_MAX_SENSE;
            if(senseIdentify.struColorSat.nMode)
                value = senseIdentify.struColorSat.nSensUp;
            else
                value = senseIdentify.struColorSat.nSensLow;

            used = senseIdentify.struColorSat.nArithEnable;
            QByteArray senseNameArr(senseIdentify.struColorSat.sName);
            strSenseName = QString::fromUtf8(senseNameArr);
            SingleSenseData sense(index,value,strSenseName,max,min,view,used);
            m_SenseDataList.append(sense);
        }

        // 颜色
        if(senseIdentify.struColorHue.nDispEnable){
            index = ARITH_HUE;
            min = 0;
            max = ARITH_HUE_MAX_SENSE_NEW;
            value = senseIdentify.struColorHue.nHighHue;

            used = senseIdentify.struColorHue.nArithEnable;
            QByteArray senseNameArr(senseIdentify.struColorHue.sName);
            strSenseName = QString::fromUtf8(senseNameArr);
            SingleSenseData sense(index,value,strSenseName,max,min,view,used);
            m_SenseDataList.append(sense);
        }

//        // 颜色低
//        if(senseIdentify.struColorHue.nDispEnable){
//            index = 15;
//            min = 0;
//            max = ARITH_HUE_MAX_SENSE;
//            value = senseIdentify.struColorHue.nLowHue;

//            used = struCnfp.nGroupArithEnable[struGsh.nLayer][view][group][ARITH_HUE];
//            QByteArray senseNameArr(senseIdentify.struColorHue.sName);
//            strSenseName = QString::fromUtf8(senseNameArr);
//            SingleSenseData sense(index,value,strSenseName,max,min,view,used);
//            m_SenseDataList.append(sense);
//        }

        // 比例
        if(senseIdentify.struColorScale.nDispEnable){
            index = ARITH_SCALE;
            min = 0;
            max = ARITH_SCALE_MAX_SENSE;
            value = senseIdentify.struColorScale.nSen;

            used = senseIdentify.struColorScale.nArithEnable;
            QByteArray senseNameArr(senseIdentify.struColorScale.sName);
            strSenseName = QString::fromUtf8(senseNameArr);
            SingleSenseData sense(index,value,strSenseName,max,min,view,used);
            m_SenseDataList.append(sense);
        }

        // 霉点
        if(senseIdentify.struMildew.nDispEnable){
            index = ARITH_MILDEW;
            min = 0;
            max = ARITH_MILDEW_MAX_SENSE;
            value = senseIdentify.struMildew.nSens;

            used = senseIdentify.struMildew.nArithEnable;
            QByteArray senseNameArr(senseIdentify.struMildew.sName);
            strSenseName = QString::fromUtf8(senseNameArr);
            SingleSenseData sense(index,value,strSenseName,max,min,view,used);
            m_SenseDataList.append(sense);
        }

        // 轮廓
        if(senseIdentify.struOutline.nDispEnable){
            index = ARITH_OUTLINE;
            min = 0;
            max = ARITH_OUTLINE_MAX_SENSE;
            value = senseIdentify.struOutline.nSens;

            used = senseIdentify.struOutline.nArithEnable;
            QByteArray senseNameArr(senseIdentify.struOutline.sName);
            strSenseName = QString::fromUtf8(senseNameArr);
            SingleSenseData sense(index,value,strSenseName,max,min,view,used);
            m_SenseDataList.append(sense);
        }

        // 细纹
        if(senseIdentify.struLine.nDispEnable){
            index = ARITH_LINE;
            min = 0;
            max = ARITH_LINE_MAX_SENSE;
            value = senseIdentify.struLine.nSens1;

            used = senseIdentify.struLine.nArithEnable;
            QByteArray senseNameArr(senseIdentify.struLine.sName);
            strSenseName = QString::fromUtf8(senseNameArr);
            SingleSenseData sense(index,value,strSenseName,max,min,view,used);
            m_SenseDataList.append(sense);
        }

        // 选芽
        for(int i = 0;i < MAX_BUD_TOTAL;i++){
            if(senseIdentify.struBud[i].nDispEnable){
                index = ARITH_BUD+i;
                min = 0;
                max = ARITH_BUD_MAX_SENSE;
                value = senseIdentify.struBud[i].nSens;

                used = senseIdentify.struBud[0].nArithEnable;
                QByteArray senseNameArr(senseIdentify.struBud[i].sName);
                strSenseName = QString::fromUtf8(senseNameArr);
                SingleSenseData sense(index,value,strSenseName,max,min,view,used);
                m_SenseDataList.append(sense);
            }
        }

        // 选圆、非圆
        for(int i = 0;i < MAX_ROUND_TOTAL;i++){
            if(senseIdentify.struRound[i].nDispEnable){
                index = ARITH_ROUND+i;
                min = 0;
                max = ARITH_ROUND_MAX_SENSE;
                if(senseIdentify.struRound[i].nSelectMode)
                    value = senseIdentify.struRound[i].nSensLow;
                else
                    value = senseIdentify.struRound[i].nSensUp;
                used = senseIdentify.struRound[i].nArithEnable;
                QByteArray SenseNameArr(senseIdentify.struRound[i].sName);
                strSenseName = QString::fromUtf8(SenseNameArr);
                SingleSenseData sense(index,value,strSenseName,max,min,view,used);
                m_SenseDataList.append(sense);
            }
        }

        // 葫芦籽霉点
        if(senseIdentify.struSplMildew.nDispEnable){
            index = ARITH_SPL_MILDEW;
            min = 0;
            max = ARITH_SPL_MILDEW_MAX_SENSE;
            value = senseIdentify.struSplMildew.nSens;

            used = senseIdentify.struSplMildew.nArithEnable;
            QByteArray senseNameArr(senseIdentify.struSplMildew.sName);
            strSenseName = QString::fromUtf8(senseNameArr);
            SingleSenseData sense(index,value,strSenseName,max,min,view,used);
            m_SenseDataList.append(sense);
        }

        // 白沙果
        if(senseIdentify.struWhitePeanut.nDispEnable){
            index = ARITH_W_PEANUT;
            min = 0;
            max = ARITH_WHITE_PEANUT_MAX_SENSE;
            value = senseIdentify.struWhitePeanut.nSensLow;

            used = senseIdentify.struWhitePeanut.nArithEnable;
            QByteArray senseNameArr(senseIdentify.struWhitePeanut.sName);
            strSenseName = QString::fromUtf8(senseNameArr);
            SingleSenseData sense(index,value,strSenseName,max,min,view,used);
            m_SenseDataList.append(sense);
        }
        // 辣椒选把
        if(senseIdentify.struPepperHandle.nDispEnable){
            index = ARITH_PEPPER_HANDLE;
            min = 0;
            max = ARITH_PEPPER_HANDLE_MAX_LIMIT;
            value = senseIdentify.struPepperHandle.nSens;

            used = senseIdentify.struPepperHandle.nArithEnable;
            QByteArray senseNameArr(senseIdentify.struPepperHandle.sName);
            strSenseName = QString::fromUtf8(senseNameArr);
            SingleSenseData sense(index,value,strSenseName,max,min,view,used);
            m_SenseDataList.append(sense);
        }

        // 压制大
        for(int i = 0;i < MAX_SUPPRESS_LARGE_TOTAL;i++){
            if(senseIdentify.struSuppressLarge[i].nDispEnable){
                index = ARITH_SUPPRESS_LARGE_A+i;
                min = 0;
                max = ARITH_ROUND_MAX_SENSE;
                value = 0;
                used = senseIdentify.struSuppressLarge[i].nArithEnable;
                QByteArray SenseNameArr(senseIdentify.struSuppressLarge[i].sName);
                strSenseName = QString::fromUtf8(SenseNameArr);
                SingleSenseData sense(index,value,strSenseName,max,min,view,used);
                m_SenseDataList.append(sense);
            }
        }
        // 小麦芽头
        if(senseIdentify.struWheatSprout.nDispEnable){
            index = ARITH_WHEAT_SPROUT;
            min = 0;
            max = 255;
            value = senseIdentify.struWheatSprout.nSense;

            used = senseIdentify.struWheatSprout.nArithEnable;
            QByteArray senseNameArr(senseIdentify.struWheatSprout.sName);
            strSenseName = QString::fromUtf8(senseNameArr);
            SingleSenseData sense(index,value,strSenseName,max,min,view,used);
            m_SenseDataList.append(sense);
        }
        // 灰度
        if(senseIdentify.struColorValue.nDispEnable){
            index = ARITH_VALUE;
            min = 0;
            max = ARITH_VALUE_MAX_SENSE;
            if(senseIdentify.struColorValue.nMode)
                value = senseIdentify.struColorValue.nSensUp;
            else
                value = senseIdentify.struColorValue.nSensLow;

            used = struCnfp.nGroupArithEnable[struGsh.nLayer][view][group][ARITH_VALUE];
            QByteArray senseNameArr(senseIdentify.struColorValue.sName);
            strSenseName = QString::fromUtf8(senseNameArr);
            SingleSenseData sense(index,value,strSenseName,max,min,view,used);
            m_SenseDataList.append(sense);
        }
    // TEA
    for(int i = 0;i < MAX_TEA_ARITH_TOTAL;i++){
        if(senseIdentify.struTeaColor[i].nDispEnable){
            index = TEA_ARITH_COLOR+i;
            min = 0;
            max = ARITH_TEA_SENSE_MAX_LIMIT*10;
            value = senseIdentify.struTeaColor[i].sense*10;

            used = senseIdentify.struTeaColor[i].nArithEnable;
            QByteArray SenseNameArr(senseIdentify.struTeaColor[i].sName);
            strSenseName = QString::fromUtf8(SenseNameArr);
            SingleSenseData sense(index,value,strSenseName,max,min,view,used);
            m_SenseDataList.append(sense);
        }
    }

    //HSV
    for(int i = 0;i < MAX_HSV_TOTAL;i++){
        if(senseIdentify.struHsv[i].nDispEnable)
        {
            index = ARITH_HSV+i;
            min = 0;
            if(senseIdentify.struHsv[i].nHsvColor == 0)
                max = 3599;
            else
                max = 255;
            switch(senseIdentify.struHsv[i].nHsvColor)
            {
            case 0:
                value = (senseIdentify.struHsv[i].nHsvMode == 0) ? senseIdentify.struHsv[i].nHsvSensLowH : senseIdentify.struHsv[i].nHsvSensUpH;
                break;
            case 1:
                value = (senseIdentify.struHsv[i].nHsvMode == 0) ? senseIdentify.struHsv[i].nHsvSensLowS : senseIdentify.struHsv[i].nHsvSensUpS;
                break;
            case 2:
                value = (senseIdentify.struHsv[i].nHsvMode == 0) ? senseIdentify.struHsv[i].nHsvSensLowV : senseIdentify.struHsv[i].nHsvSensUpV;
                break;
            }

            used = senseIdentify.struHsv[i].nArithEnable;
            //            used = struCnfp.nGroupArithEnable[struGsh.nLayer][view][group][ARITH_HSV];
            QByteArray senseNameArr(senseIdentify.struHsv[i].sName);
            strSenseName = QString::fromUtf8(senseNameArr);
            SingleSenseData sense(index,value,strSenseName,max,min,view,used);
            m_SenseDataList.append(sense);
        }
    }
    if(ui->verticalLayout->count() == m_SenseDataList.size())
        return;
    while(ui->verticalLayout->count() > 0)
        ui->verticalLayout->removeItem(ui->verticalLayout->takeAt(0));
    for(int i = 0;i < UI_ARITH_COUNT;i++){
        ui->verticalLayout->addItem(m_pSenseHLayout[i]);
    }
}

void SenseSetWidget::loadSenseDataSplice()
{
    m_SenseDataList.clear();

    int group = 0;  //struGsh.nMatGroup;
    int view = struGsh.nView;
    QString strSenseName;
    int used = 0;
    int value =50;
    int  max = 100;
    int min = 0;
    int index = 0;
    stu_group_identify senseIdentify;

    memcpy(&senseIdentify,&struCnfp.struGroupIdentify[struGsh.nLayer][view][group],sizeof(senseIdentify));

    for(int i = 0;i < MAX_GREY_COLOR_ARITH_TOTAL;i++)
    {
        if(struCnfp.struGroupIdentify[struGsh.nLayer][view][struGsh.nMatGroup].struGreyColor[i].nDispEnable)
        {
            index = i;
            min = 0;

            switch (i) {
            case ARITH_GREY_A:
            case ARITH_GREY_B:
            case ARITH_GREY_C:
            case ARITH_GREY_D:
                max = ARITH_GREY_MAX_SENS;
                break;
            case ARITH_DISCOLOR_A:
            case ARITH_DISCOLOR_B:
            case ARITH_CROSS:
                max = ARITH_COLOR_MAX_SENS;
                break;
            default:
                break;
            }
            if(senseIdentify.struGreyColor[i].nMode){
                value = senseIdentify.struGreyColor[i].nSensUp;
            }else{
                value = senseIdentify.struGreyColor[i].nSensLow;
            }
            used = struCnfp.nGroupArithEnable[struGsh.nLayer][view][struGsh.nMatGroup][ARITH_GREY_A+i];

            QByteArray senseNameArr(senseIdentify.struGreyColor[i].sName);
            strSenseName = QString::fromUtf8(senseNameArr);
            SingleSenseData sense(index,value,strSenseName,max,min,view,used);
            m_SenseDataList.append(sense);
        }
    }

    for(int i = 0;i < MAX_SVM_ARITH_TOTAL;i++)
    {
        if(struCnfp.struGroupIdentify[struGsh.nLayer][view][struGsh.nMatGroup].struSvm[i].nDispEnable){
            index = ARITH_SVM_A+i;
            min = 1;
            max = max_svm;
            value = senseIdentify.struSvm[i].nSens;
            used = struCnfp.nGroupArithEnable[struGsh.nLayer][view][struGsh.nMatGroup][ARITH_SVM_A+i];
            QByteArray senseNameArr(senseIdentify.struSvm[i].sName);
            strSenseName = QString::fromUtf8(senseNameArr);
            SingleSenseData sense(index,value,strSenseName,max,min,view,used);
            m_SenseDataList.append(sense);
        }
    }

    for(int i = 0;i < MAX_SHAPE_SVM_ARITH_TOTAL;i++)
    {
        if(struCnfp.struGroupIdentify[struGsh.nLayer][view][struGsh.nMatGroup].struShapeSvm[i].nDispEnable){
            index = ARITH_SHAPE_SVM_A+i;
            min = 1;
            max = max_svm;
            value = senseIdentify.struShapeSvm[i].nSens;
            used = struCnfp.nGroupArithEnable[struGsh.nLayer][view][struGsh.nMatGroup][ARITH_SHAPE_SVM_A+i];
            QByteArray senseNameArr(senseIdentify.struShapeSvm[i].sName);
            strSenseName = QString::fromUtf8(senseNameArr);
            SingleSenseData sense(index,value,strSenseName,max,min,view,used);
            m_SenseDataList.append(sense);
        }
    }

    for(int i = 0;i < MAX_BIG_SMALL_TOTAL;i++)
    {
        if(struCnfp.struGroupIdentify[struGsh.nLayer][view][struGsh.nMatGroup].struBigSmall[i].nDispEnable)
        {
            index = ARITH_BIG_SMALL+i;
            min = 0;
            max = struCnfg.nBigSmallArithMaxValue;
            if(senseIdentify.struBigSmall[i].nMode)
                value = senseIdentify.struBigSmall[i].nAreaUp;
            else
                value = senseIdentify.struBigSmall[i].nAreaLow;
            used = struCnfp.nGroupArithEnable[struGsh.nLayer][view][struGsh.nMatGroup][ARITH_BIG_SMALL+i];
            QByteArray senseNameArr(senseIdentify.struBigSmall[i].sName);
            strSenseName = QString::fromUtf8(senseNameArr);
            SingleSenseData sense(index,value,strSenseName,max,min,view,used);
            m_SenseDataList.append(sense);
        }
    }


    for(int i = 0;i < MAX_CIRCLE_LONG_TOTAL;i++)
    {
        if(struCnfp.struGroupIdentify[struGsh.nLayer][view][struGsh.nMatGroup].struCircleLong[i].nDispEnable)
        {
            index = ARITH_CIRCLE_LONG+i;
            min = 0;
            max = ARITH_LONG_CIRCLE_MAX_VALUE;
            if(senseIdentify.struCircleLong[i].nMode)
                value = senseIdentify.struCircleLong[i].nCilosUp;
            else
                value = senseIdentify.struCircleLong[i].nCiloLow;
            used = struCnfp.nGroupArithEnable[struGsh.nLayer][view][struGsh.nMatGroup][ARITH_CIRCLE_LONG+i];
            QByteArray senseNameArr(senseIdentify.struCircleLong[i].sName);
            strSenseName = QString::fromUtf8(senseNameArr);
            SingleSenseData sense(index,value,strSenseName,max,min,view,used);
            m_SenseDataList.append(sense);
        }
    }

    for (int i = 0; i < MAX_LONG_SHORT_TOTAL; i++)
    {
        if(struCnfp.struGroupIdentify[struGsh.nLayer][view][struGsh.nMatGroup].struLongShort[i].nDispEnable)
        {
            index = ARITH_LONG_SHORT+i;
            min = 0;
            max = ARITH_LENGTH_MAX_VALUE;
            if(senseIdentify.struLongShort[i].nLengthMode)
                value = senseIdentify.struLongShort[i].nLengthUp;
            else
                value = senseIdentify.struLongShort[i].nLengthLow;

            used = struCnfp.nGroupArithEnable[struGsh.nLayer][view][struGsh.nMatGroup][ARITH_LONG_SHORT+i];
            QByteArray senseNameArr(senseIdentify.struLongShort[i].sName);
            strSenseName = QString::fromUtf8(senseNameArr);
            SingleSenseData sense(index,value,strSenseName,max,min,view,used);
            m_SenseDataList.append(sense);
        }
    }


    for(int i = 0;i < MAX_HSV_TOTAL;i++){
        if(senseIdentify.struHsv[i].nDispEnable){
            index = ARITH_HSV+i;
            min = 0;
            if(senseIdentify.struHsv[i].nHsvColor == 0)
                max = 3599;
            else
                max = 255;

            switch(senseIdentify.struHsv[i].nHsvColor)
            {
            case 0:
                value = (senseIdentify.struHsv[i].nHsvMode == 0) ? senseIdentify.struHsv[i].nHsvSensLowH : senseIdentify.struHsv[i].nHsvSensUpH;
                break;
            case 1:
                value = (senseIdentify.struHsv[i].nHsvMode == 0) ? senseIdentify.struHsv[i].nHsvSensLowS : senseIdentify.struHsv[i].nHsvSensUpS;
                break;
            case 2:
                value = (senseIdentify.struHsv[i].nHsvMode == 0) ? senseIdentify.struHsv[i].nHsvSensLowV : senseIdentify.struHsv[i].nHsvSensUpV;
                break;
            }
            //            if(senseIdentify.struHsv.nHsvMode == 0){
            //                if(senseIdentify.struHsv.nHsvColor == 0)
            //                    value = senseIdentify.struHsv.nHsvSensLowH;
            //                else
            //                    value = senseIdentify.struHsv.nHsvSensLowS;
            //            }
            //            else{
            //                if(senseIdentify.struHsv.nHsvColor == 0)
            //                    value = senseIdentify.struHsv.nHsvSensUpH;
            //                else
            //                    value = senseIdentify.struHsv.nHsvSensUpS;
            //            }

            used = senseIdentify.struHsv[i].nArithEnable;
            //            used = struCnfp.nGroupArithEnable[struGsh.nLayer][view][struGsh.nMatGroup][ARITH_HSV];
            QByteArray senseNameArr(senseIdentify.struHsv[i].sName);
            strSenseName = QString::fromUtf8(senseNameArr);
            SingleSenseData sense(index,value,strSenseName,max,min,view,used);
            m_SenseDataList.append(sense);
        }
    }
    // 饱和
    if(senseIdentify.struColorSat.nDispEnable){
        index = ARITH_SAT;
        min = 0;
        max = ARITH_SAT_MAX_SENSE;
        if(senseIdentify.struColorSat.nMode)
            value = senseIdentify.struColorSat.nSensUp;
        else
            value = senseIdentify.struColorSat.nSensLow;

        used = struCnfp.nGroupArithEnable[struGsh.nLayer][view][struGsh.nMatGroup][ARITH_SAT];
        QByteArray senseNameArr(senseIdentify.struColorSat.sName);
        strSenseName = QString::fromUtf8(senseNameArr);
        SingleSenseData sense(index,value,strSenseName,max,min,view,used);
        m_SenseDataList.append(sense);
    }

    // 颜色
    if(senseIdentify.struColorHue.nDispEnable){
        index = ARITH_HUE;
        min = 0;
        max = ARITH_HUE_MAX_SENSE;
        value = senseIdentify.struColorHue.nHighHue;

        used = struCnfp.nGroupArithEnable[struGsh.nLayer][view][struGsh.nMatGroup][ARITH_HUE];
        QByteArray senseNameArr(senseIdentify.struColorHue.sName);
        strSenseName = QString::fromUtf8(senseNameArr);
        SingleSenseData sense(index,value,strSenseName,max,min,view,used);
        m_SenseDataList.append(sense);
    }

    // 比例
    if(senseIdentify.struColorScale.nDispEnable){
        index = ARITH_SCALE;
        min = 0;
        max = ARITH_SCALE_MAX_SENSE;
        value = senseIdentify.struColorScale.nSen;

        used = struCnfp.nGroupArithEnable[struGsh.nLayer][view][struGsh.nMatGroup][ARITH_SCALE];
        QByteArray senseNameArr(senseIdentify.struColorScale.sName);
        strSenseName = QString::fromUtf8(senseNameArr);
        SingleSenseData sense(index,value,strSenseName,max,min,view,used);
        m_SenseDataList.append(sense);
    }

    // 霉点
    if(senseIdentify.struMildew.nDispEnable)
    {
        index = ARITH_MILDEW;
        min = 0;
        max = ARITH_MILDEW_MAX_SENSE;
        value = senseIdentify.struMildew.nSens;

        used = struCnfp.nGroupArithEnable[struGsh.nLayer][view][struGsh.nMatGroup][ARITH_MILDEW];
        QByteArray senseNameArr(senseIdentify.struMildew.sName);
        strSenseName = QString::fromUtf8(senseNameArr);
        SingleSenseData sense(index, value, strSenseName, max, min, view, used);
        m_SenseDataList.append(sense);
    }

    // 轮廓
    if(senseIdentify.struOutline.nDispEnable)
    {
        index = ARITH_OUTLINE;
        min = 0;
        max = ARITH_OUTLINE_MAX_SENSE;
        value = senseIdentify.struOutline.nSens;

        used = struCnfp.nGroupArithEnable[struGsh.nLayer][view][struGsh.nMatGroup][ARITH_OUTLINE];
        QByteArray senseNameArr(senseIdentify.struOutline.sName);
        strSenseName = QString::fromUtf8(senseNameArr);
        SingleSenseData sense(index, value, strSenseName, max, min, view, used);
        m_SenseDataList.append(sense);
    }

    // 纹理
    if(senseIdentify.struLine.nDispEnable)
    {
        index = ARITH_LINE;
        min = 0;
        max = ARITH_LINE_MAX_SENSE;
        value = senseIdentify.struLine.nSens1;

        used = struCnfp.nGroupArithEnable[struGsh.nLayer][view][struGsh.nMatGroup][ARITH_LINE];
        QByteArray senseNameArr(senseIdentify.struLine.sName);
        strSenseName = QString::fromUtf8(senseNameArr);
        SingleSenseData sense(index, value, strSenseName, max, min, view, used);
        m_SenseDataList.append(sense);
    }

    // 选芽
    for (int i = 0; i < MAX_BUD_TOTAL; i++)
    {
        if(senseIdentify.struBud[i].nDispEnable)
        {
            index = ARITH_BUD+i;
            min = 0;
            max = ARITH_BUD_MAX_SENSE;
            value = senseIdentify.struBud[i].nSens;

            used = struCnfp.nGroupArithEnable[struGsh.nLayer][view][struGsh.nMatGroup][ARITH_BUD+i];
            QByteArray senseNameArr(senseIdentify.struBud[i].sName);
            strSenseName = QString::fromUtf8(senseNameArr);
            SingleSenseData sense(index, value, strSenseName, max, min, view, used);
            m_SenseDataList.append(sense);
        }
    }


    //选圆
    for (int i = 0; i < MAX_ROUND_TOTAL; i++)
    {
        if(struCnfp.struGroupIdentify[struGsh.nLayer][view][struGsh.nMatGroup].struRound[i].nDispEnable)
        {
            index = ARITH_ROUND+i;
            min = 0;
            max = ARITH_ROUND_MAX_SENSE;
            if(senseIdentify.struRound[i].nSelectMode)
                value = senseIdentify.struRound[i].nSensLow;
            else
                value = senseIdentify.struRound[i].nSensUp;
            used = struCnfp.nGroupArithEnable[struGsh.nLayer][view][struGsh.nMatGroup][ARITH_ROUND+i];
            QByteArray senseNameArr(senseIdentify.struRound[i].sName);
            strSenseName = QString::fromUtf8(senseNameArr);
            SingleSenseData sense(index,value,strSenseName,max,min,view,used);
            m_SenseDataList.append(sense);
        }
    }

    // 葫芦籽霉点
    if(senseIdentify.struSplMildew.nDispEnable)
    {
        index = ARITH_SPL_MILDEW;
        min = 0;
        max = ARITH_SPL_MILDEW_MAX_SENSE;
        value = senseIdentify.struSplMildew.nSens;

        used = struCnfp.nGroupArithEnable[struGsh.nLayer][view][struGsh.nMatGroup][ARITH_SPL_MILDEW];
        QByteArray senseNameArr(senseIdentify.struSplMildew.sName);
        strSenseName = QString::fromUtf8(senseNameArr);
        SingleSenseData sense(index, value, strSenseName, max, min, view, used);
        m_SenseDataList.append(sense);
    }
    // 压制大
    for(int i = 0;i < MAX_SUPPRESS_LARGE_TOTAL;i++){
        if(senseIdentify.struSuppressLarge[i].nDispEnable){
            index = ARITH_SUPPRESS_LARGE_A+i;
            min = 0;
            max = ARITH_ROUND_MAX_SENSE;
            value = 0;
            used = senseIdentify.struSuppressLarge[i].nArithEnable;
            QByteArray SenseNameArr(senseIdentify.struSuppressLarge[i].sName);
            strSenseName = QString::fromUtf8(SenseNameArr);
            SingleSenseData sense(index,value,strSenseName,max,min,view,used);
            m_SenseDataList.append(sense);
        }
    }

    // 白沙果算法
    if(senseIdentify.struWhitePeanut.nDispEnable)
    {
        index = ARITH_W_PEANUT;
        min = 0;
        max = ARITH_WHITE_PEANUT_MAX_SENSE;
        value = senseIdentify.struWhitePeanut.nSensLow;

        used = struCnfp.nGroupArithEnable[struGsh.nLayer][view][struGsh.nMatGroup][ARITH_W_PEANUT];
        QByteArray senseNameArr(senseIdentify.struWhitePeanut.sName);
        strSenseName = QString::fromUtf8(senseNameArr);
        SingleSenseData sense(index, value, strSenseName, max, min, view, used);
        m_SenseDataList.append(sense);
    }

    // 辣椒选把算法
    if(senseIdentify.struPepperHandle.nDispEnable)
    {
        index = ARITH_PEPPER_HANDLE;
        min = 0;
        max = ARITH_PEPPER_HANDLE_MAX_SENSE;
        value = senseIdentify.struPepperHandle.nSens;

        used = struCnfp.nGroupArithEnable[struGsh.nLayer][view][struGsh.nMatGroup][ARITH_PEPPER_HANDLE];
        QByteArray senseNameArr(senseIdentify.struPepperHandle.sName);
        strSenseName = QString::fromUtf8(senseNameArr);
        SingleSenseData sense(index, value, strSenseName, max, min, view, used);
        m_SenseDataList.append(sense);
    }

    // 小麦芽头
    if(senseIdentify.struWheatSprout.nDispEnable){
        index = ARITH_WHEAT_SPROUT;
        min = 0;
        max = 255;
        value = senseIdentify.struWheatSprout.nSense;

        used = senseIdentify.struWheatSprout.nArithEnable;
        QByteArray senseNameArr(senseIdentify.struWheatSprout.sName);
        strSenseName = QString::fromUtf8(senseNameArr);
        SingleSenseData sense(index,value,strSenseName,max,min,view,used);
        m_SenseDataList.append(sense);
    }

    // TEA
    for(int i = 0;i < MAX_TEA_ARITH_TOTAL;i++){
        if(senseIdentify.struTeaColor[i].nDispEnable){
            index = TEA_ARITH_COLOR+i;
            min = 0;
            max = ARITH_TEA_SENSE_MAX_LIMIT*10;
            value = senseIdentify.struTeaColor[i].sense;

            used = senseIdentify.struTeaColor[i].nArithEnable;
            QByteArray SenseNameArr(senseIdentify.struTeaColor[i].sName);
            strSenseName = QString::fromUtf8(SenseNameArr);
            SingleSenseData sense(index,value,strSenseName,max,min,view,used);
            m_SenseDataList.append(sense);
        }
    }

    // 灰度
    if(senseIdentify.struColorValue.nDispEnable){
        index = ARITH_VALUE;
        min = 0;
        max = ARITH_VALUE_MAX_SENSE;
        if(senseIdentify.struColorValue.nMode)
            value = senseIdentify.struColorValue.nSensUp;
        else
            value = senseIdentify.struColorValue.nSensLow;

        used = struCnfp.nGroupArithEnable[struGsh.nLayer][view][struGsh.nMatGroup][ARITH_VALUE];
        QByteArray senseNameArr(senseIdentify.struColorSat.sName);
        strSenseName = QString::fromUtf8(senseNameArr);
        SingleSenseData sense(index,value,strSenseName,max,min,view,used);
        m_SenseDataList.append(sense);
    }

    if(ui->verticalLayout->count() == m_SenseDataList.size())
        return;
    while(ui->verticalLayout->count() > 0)
        ui->verticalLayout->removeItem(ui->verticalLayout->takeAt(0));
    for(int i = 0;i < UI_ARITH_COUNT;i++){
        ui->verticalLayout->addItem(m_pSenseHLayout[i]);
    }
}

void SenseSetWidget::updateUseBtnText(int index)
{
    if(m_pUsedBtn[index]->isChecked())
        m_pUsedBtn[index]->setText(g_myLan().ids_enable);
    else
        m_pUsedBtn[index]->setText(g_myLan().ids_disable);
}

void SenseSetWidget::updateSenseByRunMode()
{
    if(struGsh.bRunMode == RUNMODE_USER)
    {
        for(int i = 0;i < m_SenseDataList.count();i++){
            m_pMoreSetBtn[i]->hide();
            m_pUsedBtn[i]->hide();
            if (!m_SenseDataList.at(i).m_bUsed){ //工程师模式下使能的算法才给用户使用
                m_pSenseNameLabel[i]->hide();
                m_pHoriSenseProgress[i]->hide();
                ui->verticalLayout->removeItem(m_pSenseHLayout[i]);
            }
        }
    }
}
