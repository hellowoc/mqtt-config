#include "userwidget.h"
#include "ui_userwidget.h"
#include "globalflow.h"
UserWidget::UserWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::UserWidget)
{
    ui->setupUi(this);
    m_pSensePage[0] = new UserSenseSetPage(0, ui->stackedWidget, this);
    m_pSensePage[0]->hide();
    m_pSensePage[1] = new UserSenseSetPage(1, ui->stackedWidget, this);
    m_pSensePage[1]->hide();
    m_pSensePage[2] = new UserSenseSetPage(2, ui->stackedWidget, this);
    m_pSensePage[2]->hide();
    logger()->info("create user page");
    m_pGroupWidget = ui->groupWidget;
    setSlotConnect();

    connect(ui->stackedWidget,SIGNAL(finished()),this,SLOT(updatePreNextBtnState()));
}

UserWidget::~UserWidget()
{
    delete ui;
}

void UserWidget::showPage(bool bshow)
{
    if(bshow){
        initSenseSetStackedWidget();
        struGsh.nMatGroup = 0;
        updateGroupWidget();
        ui->vibProgress->setRange(0,100);
        if(bshow){
            ui->comboBox->clear();
            QString strScheme;
            for(int i = 0; i < struCnfg.nProfileTotal; i++){
                QByteArray array(struCnfg.stuProfileIndex[i].sMaterialName);
                strScheme =QString::fromUtf8(array);
                ui->comboBox->addItem(strScheme);
                ui->comboBox->setCurrentIndex(struCnfg.nProfile);
            }
        }
    }
    basewidget::showPage(bshow);
}

void UserWidget::OnGroupChange(int groupIndex)
{
    initSenseSetStackedWidget();
}

void UserWidget::retranslateUiWidget()
{
    ui->label->setText(g_myLan().ids_model);
}

void UserWidget::initSenseSetStackedWidget()
{
    loadSenseData();
    m_pageCount = 0;
    m_pageCount = m_singleSenseDataVec.size() / MAX_NUM_HISTOGRAM;
    if(0 != m_singleSenseDataVec.size() % MAX_NUM_HISTOGRAM)
        m_pageCount += 1;

    //显示灵敏度的子界面
    if(m_pageCount > 0)
    {
        //灵敏度子界面数量改变后
        if(ui->stackedWidget->count() != m_pageCount)
        {
            m_pSensePage[0]->hide();
            m_pSensePage[1]->hide();
            m_pSensePage[2]->hide();
            while(ui->stackedWidget->count() > 0)
                ui->stackedWidget->removeWidget(ui->stackedWidget->widget(0));

            m_pSensePage[0]->show();
            ui->stackedWidget->addWidget(m_pSensePage[0]);
            if(m_pageCount > 1)
            {
                m_pSensePage[1]->show();
                ui->stackedWidget->addWidget(m_pSensePage[1]);
            }
            if(m_pageCount > 2)
            {
                m_pSensePage[2]->show();
                ui->stackedWidget->addWidget(m_pSensePage[2]);
            }

            ui->stackedWidget->setCurrentIndex(0);

        }


        //初始化各分页数据
        m_pSensePage[0]->InitUiPage();
        if(m_pageCount > 1)
            m_pSensePage[1]->InitUiPage();
        if(m_pageCount > 2)
            m_pSensePage[2]->InitUiPage();


    }
    else
    {
        while(ui->stackedWidget->count() > 0)
            ui->stackedWidget->removeWidget(ui->stackedWidget->widget(0));
    }
    updatePreNextBtnState();
}

void UserWidget::loadSenseData()
{
    m_singleSenseDataVec.clear();

    int group = struGsh.nMatGroup;

    QString strSenseName;
    int value =50;
    int  max = 100;
    int min = 0;
    int index = 0;
    stu_group_identify senseIdentify;
    int tmpEnable[ARITHMETIC_TOTAL];

    for(int view = 0;view < struCnfg.struLayerInfo[struGsh.nLayer].nViewChainTotal;view++){
        if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[view]) continue;
        for(int i  = 0; i < ARITHMETIC_TOTAL; i++)
        {
            tmpEnable[i] = struCnfp.nGroupArithEnable[struGsh.nLayer][view][group][i];
        }

        memcpy(&senseIdentify,&struCnfp.struGroupIdentify[struGsh.nLayer][view][group],sizeof(senseIdentify));

        for(int i = 0;i < MAX_GREY_COLOR_ARITH_TOTAL;i++){
            if(tmpEnable[ARITH_GREY_A+i] == 0)
                continue;
            if(senseIdentify.struGreyColor[i].nDispEnable){
                index = i;
                min = 0;
                if((i==ARITH_GREY_A)||(i==ARITH_GREY_B)||(i==ARITH_GREY_C)||(i==ARITH_GREY_D)){
                    max = ARITH_GREY_MAX_SENS;
                }else{
                    max = ARITH_COLOR_MAX_SENS;
                }
                if(senseIdentify.struGreyColor[i].nMode){
                    value = senseIdentify.struGreyColor[i].nSensUp;
                }else{
                    value = senseIdentify.struGreyColor[i].nSensLow;
                }
                QByteArray senseNameArr(senseIdentify.struGreyColor[i].sName);
                strSenseName = QString::fromUtf8(senseNameArr);
                SingleSenseData sense(index,value,strSenseName,max,min,view);
                m_singleSenseDataVec.append(sense);
            }
        }

        for(int i = 0;i < MAX_SVM_ARITH_TOTAL;i++){
            if(tmpEnable[ARITH_SVM_A+i] == 0)
                continue;
            if(senseIdentify.struSvm[i].nDispEnable){
                index = ARITH_SVM_A+i;
                min = 0;
                max = 100;
                value = senseIdentify.struSvm[i].nSens;
                QByteArray senseNameArr(senseIdentify.struSvm[i].sName);
                strSenseName = QString::fromUtf8(senseNameArr);
                SingleSenseData sense(index,value,strSenseName,max,min,view);
                m_singleSenseDataVec.append(sense);
            }
        }


        for(int i = 0;i < MAX_BIG_SMALL_TOTAL;i++){
            if(tmpEnable[ARITH_BIG_SMALL+i]){
                if(senseIdentify.struBigSmall[i].nDispEnable){
                    index = ARITH_BIG_SMALL+i;
                    min = 0;
                    max = struCnfg.nBigSmallArithMaxValue;
                    if(senseIdentify.struBigSmall[i].nMode)
                        value = senseIdentify.struBigSmall[i].nAreaUp;
                    else
                        value = senseIdentify.struBigSmall[i].nAreaLow;
                    QByteArray senseNameArr(senseIdentify.struBigSmall[i].sName);
                    strSenseName = QString::fromUtf8(senseNameArr);
                    SingleSenseData sense(index,value,strSenseName,max,min,view);
                    m_singleSenseDataVec.append(sense);
                }
            }
        }


        for(int i = 0;i < MAX_CIRCLE_LONG_TOTAL;i++){
            if(tmpEnable[ARITH_CIRCLE_LONG+i]){
                if(senseIdentify.struCircleLong[i].nDispEnable){
                    index = ARITH_CIRCLE_LONG;
                    min = 0;
                    max = ARITH_LONG_CIRCLE_MAX_VALUE;
                    if(senseIdentify.struCircleLong[i].nMode)
                        value = senseIdentify.struCircleLong[i].nCilosUp;
                    else
                        value = senseIdentify.struCircleLong[i].nCiloLow;
                    QByteArray senseNameArr(senseIdentify.struCircleLong[i].sName);
                    strSenseName = QString::fromUtf8(senseNameArr);
                    SingleSenseData sense(index,value,strSenseName,max,min,view);
                    m_singleSenseDataVec.append(sense);
                }
            }
        }

        for (int i = 0; i < MAX_LONG_SHORT_TOTAL; i++ )
        {
            if(tmpEnable[ARITH_LONG_SHORT]+i == 0)
            {
                continue;
            }
            if(senseIdentify.struLongShort[i].nDispEnable){
                index = ARITH_LONG_SHORT+i;
                min = 0;
                max = ARITH_LENGTH_MAX_VALUE;
                if(senseIdentify.struLongShort[i].nLengthMode)
                    value = senseIdentify.struLongShort[i].nLengthUp;
                else
                    value = senseIdentify.struLongShort[i].nLengthLow;
                QByteArray senseNameArr(senseIdentify.struLongShort[i].sName);
                strSenseName = QString::fromUtf8(senseNameArr);
                SingleSenseData sense(index,value,strSenseName,max,min,view);
                m_singleSenseDataVec.append(sense);
            }
        }


        // 饱和
        if(tmpEnable[ARITH_SAT]){
            if(senseIdentify.struColorSat.nDispEnable){
                index = ARITH_SAT;
                min = 0;
                max = ARITH_SAT_MAX_SENSE;
                if(senseIdentify.struColorSat.nMode)
                    value = senseIdentify.struColorSat.nSensUp;
                else
                    value = senseIdentify.struColorSat.nSensLow;
                QByteArray senseNameArr(senseIdentify.struColorSat.sName);
                strSenseName = QString::fromUtf8(senseNameArr);
                SingleSenseData sense(index,value,strSenseName,max,min,view);
                m_singleSenseDataVec.append(sense);
            }
        }

        // 颜色
        if(tmpEnable[ARITH_HUE]){
            if(senseIdentify.struColorHue.nDispEnable){
                index = ARITH_HUE;
                min = 0;
                max = ARITH_HUE_MAX_SENSE_NEW;
                value = senseIdentify.struColorHue.nHighHue;
                QByteArray senseNameArr(senseIdentify.struColorHue.sName);
                strSenseName = QString::fromUtf8(senseNameArr);
                SingleSenseData sense(index,value,strSenseName,max,min,view);
                m_singleSenseDataVec.append(sense);
            }
        }

        // 比例
        if(tmpEnable[ARITH_SCALE]){
            if(senseIdentify.struColorScale.nDispEnable){
                index = ARITH_SCALE;
                min = 0;
                max = ARITH_SCALE_MAX_SENSE;
                value = senseIdentify.struColorScale.nSen;
                QByteArray senseNameArr(senseIdentify.struColorScale.sName);
                strSenseName = QString::fromUtf8(senseNameArr);
                SingleSenseData sense(index,value,strSenseName,max,min,view);
                m_singleSenseDataVec.append(sense);
            }
        }

        // 霉点
        if(tmpEnable[ARITH_MILDEW]){
            if(senseIdentify.struMildew.nDispEnable){
                index = ARITH_MILDEW;
                min = 0;
                max = ARITH_MILDEW_MAX_SENSE;
                value = senseIdentify.struMildew.nSens;
                QByteArray senseNameArr(senseIdentify.struMildew.sName);
                strSenseName = QString::fromUtf8(senseNameArr);
                SingleSenseData sense(index,value,strSenseName,max,min,view);
                m_singleSenseDataVec.append(sense);
            }
        }

        // 轮廓
        if(tmpEnable[ARITH_OUTLINE]){
            if(senseIdentify.struOutline.nDispEnable){
                index = ARITH_OUTLINE;
                min = 0;
                max = ARITH_OUTLINE_MAX_SENSE;
                value = senseIdentify.struOutline.nSens;
                QByteArray senseNameArr(senseIdentify.struOutline.sName);
                strSenseName = QString::fromUtf8(senseNameArr);
                SingleSenseData sense(index,value,strSenseName,max,min,view);
                m_singleSenseDataVec.append(sense);
            }
        }

        // 纹理
        if(tmpEnable[ARITH_LINE]){
            if(senseIdentify.struLine.nDispEnable){
                index = ARITH_LINE;
                min = 0;
                max = ARITH_LINE_MAX_SENSE;
                value = senseIdentify.struLine.nSens1;
                QByteArray senseNameArr(senseIdentify.struLine.sName);
                strSenseName = QString::fromUtf8(senseNameArr);
                SingleSenseData sense(index,value,strSenseName,max,min,view);
                m_singleSenseDataVec.append(sense);
            }
        }

        // 选芽
        for(int i = 0; i < MAX_BUD_TOTAL; i++)
        {
            if(tmpEnable[ARITH_BUD+i] == 0 ){
                continue;

            }
            if(senseIdentify.struBud[i].nDispEnable){
                index = ARITH_BUD+i;
                min = 0;
                max = ARITH_BUD_MAX_SENSE;
                value = senseIdentify.struBud[i].nSens;
                QByteArray senseNameArr(senseIdentify.struBud[i].sName);
                strSenseName = QString::fromUtf8(senseNameArr);
                SingleSenseData sense(index,value,strSenseName,max,min,view);
                m_singleSenseDataVec.append(sense);
            }
        }


        // 选圆
        for(int i = 0; i < MAX_BUD_TOTAL; i++)
        {
            if(tmpEnable[ARITH_ROUND]){
                if(senseIdentify.struRound[i].nDispEnable){
                    index = ARITH_ROUND;
                    min = 0;
                    max = ARITH_ROUND_MAX_SENSE;
                    if(senseIdentify.struRound[i].nSelectMode)
                        value = senseIdentify.struRound[i].nSensLow;
                    else
                        value = senseIdentify.struRound[i].nSensUp;
                    QByteArray senseNameArr(senseIdentify.struRound[i].sName);
                    strSenseName = QString::fromUtf8(senseNameArr);
                    SingleSenseData sense(index,value,strSenseName,max,min,view);
                    m_singleSenseDataVec.append(sense);
                }
            }
        }

        // 霉点
        if(tmpEnable[ARITH_SPL_MILDEW]){
            if(senseIdentify.struSplMildew.nDispEnable){
                index = ARITH_SPL_MILDEW;
                min = 0;
                max = ARITH_SPL_MILDEW_MAX_SENSE;
                value = senseIdentify.struSplMildew.nSens;
                QByteArray senseNameArr(senseIdentify.struSplMildew.sName);
                strSenseName = QString::fromUtf8(senseNameArr);
                SingleSenseData sense(index,value,strSenseName,max,min,view);
                m_singleSenseDataVec.append(sense);
            }
        }

        // 白沙果
        if(tmpEnable[ARITH_W_PEANUT]){
            if(senseIdentify.struWhitePeanut.nDispEnable){
                index = ARITH_W_PEANUT;
                min = 0;
                max = ARITH_WHITE_PEANUT_MAX_SENSE;
                value = senseIdentify.struWhitePeanut.nSensLow;
                QByteArray senseNameArr(senseIdentify.struWhitePeanut.sName);
                strSenseName = QString::fromUtf8(senseNameArr);
                SingleSenseData sense(index,value,strSenseName,max,min,view);
                m_singleSenseDataVec.append(sense);
            }
        }
        // 辣椒选把
        if(tmpEnable[ARITH_PEPPER_HANDLE]){
            if(senseIdentify.struPepperHandle.nDispEnable){
                index = ARITH_PEPPER_HANDLE;
                min = 0;
                max = ARITH_PEPPER_HANDLE_MAX_SENSE;
                value = senseIdentify.struPepperHandle.nSens;
                QByteArray senseNameArr(senseIdentify.struPepperHandle.sName);
                strSenseName = QString::fromUtf8(senseNameArr);
                SingleSenseData sense(index,value,strSenseName,max,min,view);
                m_singleSenseDataVec.append(sense);
            }
        }
        // 压制大
        for(int i = 0; i < MAX_SUPPRESS_LARGE_TOTAL; i++)
        {
            if(tmpEnable[ARITH_SUPPRESS_LARGE_A+i]){
                if(senseIdentify.struSuppressLarge[i].nDispEnable){
                    index = ARITH_SUPPRESS_LARGE_A;
                    min = 0;
                    max = ARITH_ROUND_MAX_SENSE;
                    value = 0;
                    QByteArray senseNameArr(senseIdentify.struSuppressLarge[i].sName);
                    strSenseName = QString::fromUtf8(senseNameArr);
                    SingleSenseData sense(index,value,strSenseName,max,min,view);
                    m_singleSenseDataVec.append(sense);
                }
            }
        }
        // 小麦芽头
        if(tmpEnable[ARITH_WHEAT_SPROUT]){
            if(senseIdentify.struWheatSprout.nDispEnable){
                index = ARITH_WHEAT_SPROUT;
                min = 0;
                max = 255;
                value = senseIdentify.struWheatSprout.nSense;
                QByteArray senseNameArr(senseIdentify.struWheatSprout.sName);
                strSenseName = QString::fromUtf8(senseNameArr);
                SingleSenseData sense(index,value,strSenseName,max,min,view);
                m_singleSenseDataVec.append(sense);
            }
        }
        // TEA
        for(int i = 0;i < MAX_TEA_ARITH_TOTAL;i++){
            if(tmpEnable[TEA_ARITH_COLOR+i]){
                if(senseIdentify.struTeaColor[i].nDispEnable){
                    index = TEA_ARITH_COLOR;
                    min = 0;
                    max = ARITH_TEA_SENSE_MAX_LIMIT;
                    value = senseIdentify.struTeaColor[i].sense;
                    QByteArray senseNameArr(senseIdentify.struTeaColor[i].sName);
                    strSenseName = QString::fromUtf8(senseNameArr);
                    SingleSenseData sense(index,value,strSenseName,max,min,view);
                    m_singleSenseDataVec.append(sense);
                }
            }
        }

        // 灰度
        if(tmpEnable[ARITH_VALUE]){
            if(senseIdentify.struColorValue.nDispEnable){
                index = ARITH_VALUE;
                min = 0;
                max = ARITH_VALUE_MAX_SENSE;
                if(senseIdentify.struColorValue.nMode)
                    value = senseIdentify.struColorValue.nSensUp;
                else
                    value = senseIdentify.struColorValue.nSensLow;
                QByteArray senseNameArr(senseIdentify.struColorValue.sName);
                strSenseName = QString::fromUtf8(senseNameArr);
                SingleSenseData sense(index,value,strSenseName,max,min,view);
                m_singleSenseDataVec.append(sense);
            }
        }
    }
}

void UserWidget::updatePreNextBtnState()
{
    int index = ui->stackedWidget->currentIndex();
    if(index > 0)
        ui->preBtn->show();
    else
        ui->preBtn->hide();
    if(index < m_pageCount-1)
        ui->nextBtn->show();
    else
        ui->nextBtn->hide();
}

void UserWidget::on_preBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()-1);
}

void UserWidget::on_nextBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()+1);
}


void UserWidget::on_comboBox_activated(int index)
{
    g_infoWidget().setLabelText(g_myLan().msg_applying);
    g_infoWidget().delayShow();

    myFlow.saveProfile();
    struCnfg.nProfile = index;
    myFlow.getProfile();
    myFlow.resetSortParams();
    usleep(2000);
    g_infoWidget().hide();
}
