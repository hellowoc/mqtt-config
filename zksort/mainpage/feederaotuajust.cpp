#include "feederaotuajust.h"
#include "ui_feederaotuajust.h"

FeederAotuAjust::FeederAotuAjust(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::FeederAotuAjust)
{
    ui->setupUi(this);
    m_pGroupWidget = ui->groupWidget;

    feedPixelCaliDuration = 60;
    totalRicePixels = 0;
    m_fixedFlag = 0;
    setSlotConnect();

    ui->throughoutcaliwiget->hide();
    ui->throughoutwiget1->hide();
    ui->chuteBalSetBtn->hide();
}

FeederAotuAjust::~FeederAotuAjust()
{
    delete ui;
}


void FeederAotuAjust::showPage(bool bshow)
{
    basewidget::showPage(bshow);
    if(bshow){
        updateWidget();
    }
}

void FeederAotuAjust::OnGroupChange(int groupIndex)
{
    updateGroupWidget();
    updateWidget();
}

void FeederAotuAjust::OnLayerChange()
{
    updateWidget();
}

void FeederAotuAjust::retranslateUiWidget()
{
    ui->checkBox_enable->setText(g_myLan().ids_autoFeed);
//    ui->checkBox_quick_balance->setText(g_myLan().ids_quickFeedAdjust);
    ui->label_target_1->setText(g_myLan().ids_SetYeild +"-1 " +"(kg/h)");
    ui->label_target_upper->setText(g_myLan().ids_uplimit);
    ui->label_target_lower->setText(g_myLan().ids_lowlimit);

    ui->label_quality->setText(g_myLan().ids_CalibrateInKG);
    ui->label_target_2->setText(g_myLan().ids_SetYeild +"-2 " +"(kg/h)");
    ui->label_target_3->setText(g_myLan().ids_SetYeild +"-3 " +"(kg/h)");
    ui->label_target_4->setText(g_myLan().ids_SetYeild +"-4 " +"(kg/h)");
    ui->pushButton->setText(g_myLan().ids_CalibrateMass);
//    ui->ajust_range->setText(g_myLan().ids_FeedSetRange);
    ui->startBtn->setText(g_myLan().ids_run);
    ui->pushButton_2->setText(g_myLan().ids_advance+g_myLan().ids_CalibrateMass);
    ui->label_quality_2->setText(g_myLan().ids_CalibrateInKG);
    ui->chuteBalSetBtn->setText(g_myLan().ids_more);

    ui->singleaimcheckBox->setText(g_myLan().Adjust_ratio);
}

void FeederAotuAjust::updateWidget()
{
    if(struCnfg.stuFeedAutoCtrl.autoFeedMode == AUTO_FEED_MODE_0)
    {
       ui->checkBox_enable->setChecked(struCnfp.bEnableBalanceFeed == 1);
    }
    else
    {
        ui->checkBox_enable->setChecked(struCnfp.nAutoFeed[struGsh.nLayer][struGsh.nMatGroup].bEnable == 1);
    }


    ui->checkBox_quick_balance->setChecked(struCnfp.nAutoFeed[struGsh.nLayer][struGsh.nMatGroup].bQuickBalance == 1);
    ui->singleaimcheckBox->setChecked(struCnfp.nAutoFeed[struGsh.nLayer][struGsh.nMatGroup].bSingleAimBalance == 1);

    ui->target_flow_1_edit->setText(QString::number(struCnfp.nAutoFeed[struGsh.nLayer][struGsh.nMatGroup].mode1FeedTargetPart[0]));
    ui->target_flow_2_edit->setText(QString::number(struCnfp.nAutoFeed[struGsh.nLayer][struGsh.nMatGroup].mode1FeedTargetPart[1]));
    ui->target_flow_3_edit->setText(QString::number(struCnfp.nAutoFeed[struGsh.nLayer][struGsh.nMatGroup].mode1FeedTargetPart[2]));
    ui->target_flow_4_edit->setText(QString::number(struCnfp.nAutoFeed[struGsh.nLayer][struGsh.nMatGroup].mode1FeedTargetPart[3]));
    ui->target_upper_edit->setText(QString::number(struCnfp.nAutoFeed[struGsh.nLayer][struGsh.nMatGroup].mode2_target_upper));
    ui->target_lower_edit->setText(QString::number(struCnfp.nAutoFeed[struGsh.nLayer][struGsh.nMatGroup].mode2_target_lower));

    ui->target_flow_1_edit->setMinMax(this,200,5000,-1,&struCnfp.nAutoFeed[struGsh.nLayer][struGsh.nMatGroup].mode1FeedTargetPart[0]);
    ui->target_flow_2_edit->setMinMax(this,1,5000,-1,&struCnfp.nAutoFeed[struGsh.nLayer][struGsh.nMatGroup].mode1FeedTargetPart[1]);
    ui->target_flow_3_edit->setMinMax(this,1,5000,-1,&struCnfp.nAutoFeed[struGsh.nLayer][struGsh.nMatGroup].mode1FeedTargetPart[2]);
    ui->target_flow_4_edit->setMinMax(this,1,5000,-1,&struCnfp.nAutoFeed[struGsh.nLayer][struGsh.nMatGroup].mode1FeedTargetPart[3]);
    ui->target_upper_edit->setMinMax(this,1,5000,-1,&struCnfp.nAutoFeed[struGsh.nLayer][struGsh.nMatGroup].mode2_target_upper);
    ui->target_lower_edit->setMinMax(this,1,5000,-1,&struCnfp.nAutoFeed[struGsh.nLayer][struGsh.nMatGroup].mode2_target_lower);

    ui->ajust_range_edit->setText(QString::number(struCnfp.nAutoFeed[struGsh.nLayer][struGsh.nMatGroup].ajust_feeder_range));
    ui->ajust_range_edit->setMinMax(this,0,60,-1,&struCnfp.nAutoFeed[struGsh.nLayer][struGsh.nMatGroup].ajust_feeder_range);

    ui->quality_edit->setFloatMinMax(this,0.0,50.0,-1,&m_quality,false);
    ui->flow_value->setMinMax(this,0,10000000000,-1,&totalRicePixels);
    ui->label_coff->setText(QString::number(struCnfp.n_coff));

    ui->quality_edit_2->setFloatMinMax(this,0.0,500000.0,-1,&m_quality_2,false);

    ui->singleaimcheckBox->hide();

    ui->checkBox_quick_balance->hide();
    ui->target_flow_1_edit->hide();
    ui->target_flow_2_edit->hide();
    ui->target_flow_3_edit->hide();
    ui->target_flow_4_edit->hide();

    ui->label_target_1->hide();
    ui->label_target_2->hide();
    ui->label_target_3->hide();
    ui->label_target_4->hide();

    ui->target_upper_edit->hide();
    ui->label_target_upper->hide();
    ui->target_lower_edit->hide();
    ui->label_target_lower->hide();

    switch(struCnfg.stuFeedAutoCtrl.autoFeedMode)
    {
    case AUTO_FEED_MODE_0:
        break;
    case AUTO_FEED_MODE_1:
        ui->checkBox_quick_balance->show();
        switch(struCnfg.stuFeedAutoCtrl.mode1FeedTargetPartCount)
        {
        case 1:
            ui->target_flow_1_edit->show();
            ui->label_target_1->show();
            break;
        case 2:
            ui->target_flow_1_edit->show();
            ui->label_target_1->show();
            ui->target_flow_2_edit->show();
            ui->label_target_2->show();
            break;
        case 3:
            ui->target_flow_1_edit->show();
            ui->label_target_1->show();
            ui->target_flow_2_edit->show();
            ui->label_target_2->show();
            ui->target_flow_3_edit->show();
            ui->label_target_3->show();
            break;
        case 4:
            ui->target_flow_1_edit->show();
            ui->label_target_1->show();
            ui->target_flow_2_edit->show();
            ui->label_target_2->show();
            ui->target_flow_3_edit->show();
            ui->label_target_3->show();
            ui->target_flow_4_edit->show();
            ui->label_target_4->show();
            break;
        }
        break;
    case AUTO_FEED_MODE_2:
        ui->checkBox_quick_balance->show();
        ui->target_upper_edit->show();
        ui->label_target_upper->show();
        ui->target_lower_edit->show();
        ui->label_target_lower->show();
        break;
    }
}

void FeederAotuAjust::on_checkBox_enable_clicked(bool checked)
{
    if(checked){
        struCnfp.nAutoFeed[struGsh.nLayer][struGsh.nMatGroup].bEnable = 1;

    }else{
        struCnfp.nAutoFeed[struGsh.nLayer][struGsh.nMatGroup].bEnable = 0;
    }
    struCnfp.bEnableBalanceFeed = 0;
    for(int group = 0; group < struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[struGsh.nViewforstatic].nIdentifyGroupTotal; group++)
    {
        if(struCnfp.nAutoFeed[struGsh.nLayer][group].bEnable == 1)
        {
            struCnfp.bEnableBalanceFeed = 1;
        }
    }
}

void FeederAotuAjust::on_pushButton_clicked()
{
    float value = ui->quality_edit->text().toFloat();
    struCnfp.n_coff = totalRicePixels/value;
    ui->label_coff->setText(QString::number(struCnfp.n_coff));
}

void FeederAotuAjust::on_startBtn_clicked()
{
    /* 禁止清灰 */
    struGsh.bFlagAutowipe = 0;

    int nInt=0, board, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];

    g_infoWidget().setLabelText(g_myLan().searching);
    g_infoWidget().delayShow();

    struCnfg.nAutoStaticsEnable = 0;
    /* step 1 清零上次统计 */
    for(int i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(int j = 0; j < 1; j++)  //只有前主
        {
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            for(int k = 0; k < 1; k++)  //只有一次
            {
                for(int m = 0; m < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++) //
                {
                    nUnitAddr = (myFlow.getIdentifyGroupAddr(i, j, k, m));   //防止分割
                    if(nUnitAddr > 128) //克服通道分割
                    {
                        continue;
                    }

                    mySerial.pushCom1CmdData(CMD_CAMERA_STATICS_CAPTURE, nInt, board, nUnitAddr, data, 1);
                    myFlow.msleep(10);
                }
            }
        }
    }

    //等待3秒，保证每个通道被清零
    myFlow.sleep(3);

    struCnfg.nAutoStaticsEnable = 1;
    /* step 2 打开振动器*/
    myFlow.onOff();

    /* step 3 等待标定时间*/
    myFlow.sleep(feedPixelCaliDuration);

    /* step 4 关闭振动器*/
    myFlow.onOff();

    //等待5秒，保证每个通道大米下完
    myFlow.sleep(5);

    /* step 5 请求大米标定信息 */
    for(int i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(int j = 0; j < 1; j++)  //只有前主
        {
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            for(int k = 0; k < 1; k++)  //只有一次
            {
                for(int m = 0; m < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++) //
                {
                    nUnitAddr = (myFlow.getIdentifyGroupAddr(i, j, k, m));   //防止分割
                    if(nUnitAddr > 128) //克服通道分割
                    {
                        continue;
                    }

                    mySerial.pushCom1CmdData(CMD_CAMERA_STATICS_CAPTURE, nInt, board, nUnitAddr, data, 1);
                    myFlow.msleep(10);
                }
            }
        }
    }

    /* step 6 获取大米标定信息，只获取一次前主*/
    myFlow.sleep(3);
    totalRicePixels = 0;
    for(int i = 0; i < struCnfg.nLayerTotal; i++)
    {
        for(int j = 0; j < 1; j++)  //只有前主
        {
            board = struCnfg.struLayerInfo[i].nViewBoardType[j];
            memset(data, 0, SEND_PACKET_DATA_SIZE);
            for(int k = 0; k < 1; k++)  //只有一次
            {
                for(int m = 0; m < struCnfg.struLayerInfo[i].stuViewInfo[j].stuIdenGupInfo[k].nGroupUnitTotal; m++) //
                {
                    nUnitAddr = (myFlow.getIdentifyGroupAddr(i, j, k, m));   //防止分割
                    if(nUnitAddr > 128) //克服通道分割
                    {
                        continue;
                    }

                    totalRicePixels += struGsh.struStatics[i][j][nUnitAddr].nThroughout;
                }
            }
        }
    }

    g_infoWidget().hide();

    /* 使能清灰 */
    struGsh.bFlagAutowipe = 1;
    ui->flow_value->setText(QString::number(totalRicePixels));
}

/* 开启色选前开始标定，关闭色选后停止标定*/
void FeederAotuAjust::on_pushButton_2_clicked()
{
    if (m_fixedFlag == 0)
    {
        struGsh.fixedThroughout = 0;
        m_fixedFlag = 1;
        ui->pushButton_2->setText(g_myLan().ids_advance+g_myLan().ids_CalibrateMass);
        ui->quality_edit_2->setEnabled(false);
    }
    else
    {
        if(struGsh.bSortStart == 1)
        {
            g_infoWidget().setLabelText(g_myLan().msg_turn_off_first);
            g_infoWidget().showSecs();
            return;
        }

        myMessageBox msgBox(MSG_QUES, g_myLan().ids_ToCalibrateMass);

        int ret = msgBox.exec();

        if (ret == QDialog::Accepted)
        {
            ui->quality_edit_2->setEnabled(true);
            if (m_quality_2 == 0)
            {
                g_infoWidget().setLabelText(g_myLan().ids_InputNonzeroValue);
                g_infoWidget().showSecs();
                return;
            }
            struCnfg.n_coff_user = struGsh.fixedThroughout / m_quality_2;
            struCnfp.n_coff =(int)(struCnfp.n_coff*struCnfg.n_coff_user);
            qDebug()<< struGsh.fixedThroughout<<m_quality_2<<struCnfg.n_coff_user<<struCnfp.n_coff;
            g_infoWidget().setLabelText(g_myLan().ids_CalibrateMass+g_myLan().ids_succeeded);
            g_infoWidget().showSecs();
            ui->label_coff->setText(QString::number(struCnfp.n_coff));
            ui->pushButton_2->setText(g_myLan().ids_advance+g_myLan().ids_CalibrateMass);
            m_fixedFlag = 0;
            return;
        }
        else{
            m_fixedFlag = 0;
            ui->quality_edit_2->setEnabled(true);
        }
    }
}

void FeederAotuAjust::on_checkBox_quick_balance_clicked()
{
    struCnfp.nAutoFeed[struGsh.nLayer][struGsh.nMatGroup].bQuickBalance
            = (struCnfp.nAutoFeed[struGsh.nLayer][struGsh.nMatGroup].bQuickBalance == 0) ? 1 : 0;

    updateWidget();
}

void FeederAotuAjust::on_chuteBalSetBtn_clicked()
{
//    g_MainManager().ShowWidget(SM_BALANCE_SINGLE_SET_WIDGET);
}

void FeederAotuAjust::on_singleaimcheckBox_clicked()
{
    struCnfp.nAutoFeed[struGsh.nLayer][struGsh.nMatGroup].bSingleAimBalance
            = (struCnfp.nAutoFeed[struGsh.nLayer][struGsh.nMatGroup].bSingleAimBalance == 0) ? 1 : 0;

    updateWidget();
}
