#include "lampboardcfg.h"
#include "ui_lampboardcfg.h"
#include "global.h"
#include <QButtonGroup>
LampBoardCfg::LampBoardCfg(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::LampBoardCfg)
{
    ui->setupUi(this);
    m_CurBoardIndex = 0;
    m_pBoardBtnGroup = new QButtonGroup(this);
    m_pBoardBtnGroup->addButton(ui->pushButton,0);
    m_pBoardBtnGroup->addButton(ui->pushButton_2,1);
    m_pBoardBtnGroup->addButton(ui->pushButton_3,2);
    m_pBoardBtnGroup->addButton(ui->pushButton_4,3);
    m_pBoardBtnGroup->addButton(ui->pushButton_5,4);
    m_pBoardBtnGroup->addButton(ui->pushButton_6,5);
    m_pBoardBtnGroup->addButton(ui->pushButton_allInOne,6);

    m_pBoardBtnGroupSigMapper = new QSignalMapper(this);
    for(int i = 0;i < MAX_LAMP_BOARD_PER_INTERFACE-2;i++){
        m_pBoardBtnGroup->button(i)->setCheckable(true);
        m_pBoardBtnGroupSigMapper->setMapping(m_pBoardBtnGroup->button(i),i);
        connect(m_pBoardBtnGroup->button(i),SIGNAL(clicked()),m_pBoardBtnGroupSigMapper,SLOT(map()));
    }
    connect(m_pBoardBtnGroupSigMapper,SIGNAL(mapped(int)),this,SLOT(lampBoardBtnClicked(int)));

    m_plampPortEnable[0] = ui->checkBox;
    m_plampPortEnable[1] = ui->checkBox_2;
    m_plampPortEnable[2] = ui->checkBox_3;
    m_plampPortEnable[3] = ui->checkBox_4;
    m_plampPortEnable[4] = ui->checkBox_5;
    m_plampPortEnable[5] = ui->checkBox_6;
    m_plampPortEnable[6] = ui->checkBox_7;
    m_plampPortEnable[7] = ui->checkBox_8;
    //    m_plampPortEnable[8] = ui->checkBox_9;
    //    m_plampPortEnable[9] = ui->checkBox_10;

    m_pboardLayerEdit[0] = ui->layerEdit;
    m_pboardLayerEdit[1] = ui->layerEdit_2;
    m_pboardLayerEdit[2] = ui->layerEdit_3;
    m_pboardLayerEdit[3] = ui->layerEdit_4;
    m_pboardLayerEdit[4] = ui->layerEdit_5;
    m_pboardLayerEdit[5] = ui->layerEdit_6;
    m_pboardLayerEdit[6] = ui->layerEdit_7;
    m_pboardLayerEdit[7] = ui->layerEdit_8;
    //    m_pboardLayerEdit[8] = ui->layerEdit_9;
    //    m_pboardLayerEdit[9] = ui->layerEdit_10;

    m_plampViewBtn[0] = ui->viewBtn;
    m_plampViewBtn[1] = ui->viewBtn_2;
    m_plampViewBtn[2] = ui->viewBtn_3;
    m_plampViewBtn[3] = ui->viewBtn_4;
    m_plampViewBtn[4] = ui->viewBtn_5;
    m_plampViewBtn[5] = ui->viewBtn_6;
    m_plampViewBtn[6] = ui->viewBtn_7;
    m_plampViewBtn[7] = ui->viewBtn_8;
    //    m_plampViewBtn[8] = ui->viewBtn_9;
    //    m_plampViewBtn[9] = ui->viewBtn_10;

    m_pLampPortTypeBtn[0] = ui->typeBtn;
    m_pLampPortTypeBtn[1] = ui->typeBtn_2;
    m_pLampPortTypeBtn[2] = ui->typeBtn_3;
    m_pLampPortTypeBtn[3] = ui->typeBtn_4;
    m_pLampPortTypeBtn[4] = ui->typeBtn_5;
    m_pLampPortTypeBtn[5] = ui->typeBtn_6;
    m_pLampPortTypeBtn[6] = ui->typeBtn_7;
    m_pLampPortTypeBtn[7] = ui->typeBtn_8;
    //    m_pLampPortTypeBtn[8] = ui->typeBtn_9;
    //    m_pLampPortTypeBtn[9] = ui->typeBtn_10;

    m_pLimitEdit[0] = ui->lineEdit;
    m_pLimitEdit[1] = ui->lineEdit_2;
    m_pLimitEdit[2] = ui->lineEdit_3;
    m_pLimitEdit[3] = ui->lineEdit_4;
    m_pLimitEdit[4] = ui->lineEdit_5;
    m_pLimitEdit[5] = ui->lineEdit_6;
    m_pLimitEdit[6] = ui->lineEdit_7;
    m_pLimitEdit[7] = ui->lineEdit_8;
    //    m_pLimitEdit[8] = ui->lineEdit_9;
    //    m_pLimitEdit[9] = ui->lineEdit_10;

    m_pLampCoef[0] = ui->coefEdit1;
    m_pLampCoef[1] = ui->coefEdit2;
    m_pLampCoef[2] = ui->coefEdit3;
    m_pLampCoef[3] = ui->coefEdit4;
    m_pLampCoef[4] = ui->coefEdit5;
    m_pLampCoef[5] = ui->coefEdit6;
    m_pLampCoef[6] = ui->coefEdit7;
    m_pLampCoef[7] = ui->coefEdit8;
    //    m_pLampCoef[8] = ui->coefEdit9;
    //    m_pLampCoef[9] = ui->coefEdit10;

    m_pRenameEdit[0] = ui->lineEdit_n1;
    m_pRenameEdit[1] = ui->lineEdit_n2;
    m_pRenameEdit[2] = ui->lineEdit_n3;
    m_pRenameEdit[3] = ui->lineEdit_n4;
    m_pRenameEdit[4] = ui->lineEdit_n5;
    m_pRenameEdit[5] = ui->lineEdit_n6;
    m_pRenameEdit[6] = ui->lineEdit_n7;
    m_pRenameEdit[7] = ui->lineEdit_n8;
    //    m_pRenameEdit[8] = ui->lineEdit_n9;
    //    m_pRenameEdit[9] = ui->lineEdit_n10;




    m_pLampPortTypeBtnSigMapper = new QSignalMapper(this);
    m_pLampPortViewBtnSigMapper = new QSignalMapper(this);
    m_plampPortEnableSigMapper = new QSignalMapper(this);
    for(int i = 0;i < MAX_LAMP_PER_BOARD;i++){
        m_pLampPortTypeBtn[i]->setCheckable(true);
        m_plampViewBtn[i]->setCheckable(true);
        m_pLampPortTypeBtnSigMapper->setMapping(m_pLampPortTypeBtn[i],i);
        connect(m_pLampPortTypeBtn[i],SIGNAL(clicked()),m_pLampPortTypeBtnSigMapper,SLOT(map()));

        m_pLampPortViewBtnSigMapper->setMapping(m_plampViewBtn[i],i);
        connect(m_plampViewBtn[i],SIGNAL(clicked()),m_pLampPortViewBtnSigMapper,SLOT(map()));

        m_plampPortEnableSigMapper->setMapping(m_plampPortEnable[i],i);
        connect(m_plampPortEnable[i],SIGNAL(pressed()),m_plampPortEnableSigMapper,SLOT(map()));

        connect(m_pRenameEdit[i],SIGNAL(valueSetSignal()),this,SLOT(sLampNameChanged()));
    }

    connect(m_pLampPortTypeBtnSigMapper,SIGNAL(mapped(int)),this,SLOT(lampTypeBtnClicked(int)));
    connect(m_plampPortEnableSigMapper,SIGNAL(mapped(int)),this,SLOT(lampPortEnableClicked(int)));
    connect(m_pLampPortViewBtnSigMapper,SIGNAL(mapped(int)),this,SLOT(lampViewBtnClicked(int)));
    //    ui->setBoardNum->hide();
    ui->label_9->hide();
    ui->label_10->hide();
    ui->checkBox_9->hide();
    ui->checkBox_10->hide();
    ui->layerEdit_9->hide();
    ui->layerEdit_10->hide();
    ui->viewBtn_9->hide();
    ui->viewBtn_10->hide();
    ui->typeBtn_9->hide();
    ui->typeBtn_10->hide();
    ui->lineEdit_9->hide();
    ui->lineEdit_10->hide();
    ui->coefEdit9->hide();
    ui->coefEdit10->hide();
    ui->lineEdit_n9->hide();
    ui->lineEdit_n10->hide();


    shottimer = new QTimer;
    connect(shottimer,SIGNAL(timeout()),this, SLOT(getscreenshot()));



}

LampBoardCfg::~LampBoardCfg()
{
    delete ui;
}
void LampBoardCfg::showPage(bool bshow)
{
    if(bshow){
        ui->setBoardNum->setParams(this,0,MAX_LAMP_BOARD_PER_INTERFACE-2,3,&struCnfg.stuLayerLampCfg.nLampBoardTotal);
        ui->setBoardNum->setTitle(g_myLan().ids_ccsBoard);
        ui->setBoardNum->setValue(struCnfg.stuLayerLampCfg.nLampBoardTotal);
        updateLayerSetVisiable();
        if(struCnfg.nAllInOne){
            ui->pushButton_allInOne->setVisible(true);
            ui->pushButton_allInOne->setCheckable(true);
            if(struCnfg.stuLayerLampCfg.nLampBoardTotal==0){
                m_CurBoardIndex = MAX_LAMP_BOARD_PER_INTERFACE-2;
            }
        }else{
            ui->pushButton_allInOne->setVisible(false);
        }
        updateBoardBtn();
        updateCurBoard();

        if(struGsh.bScreenShotStatus == 1)
        {
            shottimer->start(1000);

        }

    }else{
        if(memcmp(&_t_struCnfg.stuLayerLampCfg,&struCnfg.stuLayerLampCfg,sizeof(stru_layer_lamp_cfg))){
            myMessageBox msgBox(MSG_QUES, g_myLan().save_parameters);
            int ret = msgBox.exec();

            if (ret == QDialog::Accepted)
            {
                memcpy(&_t_struCnfg.stuLayerLampCfg,&struCnfg.stuLayerLampCfg,sizeof(stru_layer_lamp_cfg));
                myFlow.saveGlobal();
            }
        }
    }
}

void LampBoardCfg::retranslateUiWidget()
{
    ui->pushButton->setText(g_myLan().ids_ccsBoard+QString("1"));
    ui->pushButton_2->setText(g_myLan().ids_ccsBoard+QString("2"));
    ui->pushButton_3->setText(g_myLan().ids_ccsBoard+QString("3"));
    ui->pushButton_4->setText(g_myLan().ids_ccsBoard+QString("4"));
    ui->pushButton_5->setText(g_myLan().ids_ccsBoard+QString("5"));
    ui->pushButton_6->setText(g_myLan().ids_ccsBoard+QString("6"));
    ui->label_layer->setText(g_myLan().layer);
    ui->label_limit->setText(g_myLan().ids_uplimit);
    ui->restoreBtn->setText(g_myLan().ids_restore);
    ui->okbtn->setText(g_myLan().ids_ok);
    ui->label_coef->setText(g_myLan().lamp_coef);
    ui->label_rename->setText(g_myLan().ids_rename);
    ui->pushButton_allInOne->setText(g_myLan().ids_allInOne);
}

void LampBoardCfg::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){
        if(msg2 == 3){
            updateBoardBtn();
        }
    }
}


void LampBoardCfg::getscreenshot()
{
    shottimer->stop();
    {

        for(int i = 0;i < MAX_LAMP_BOARD_PER_INTERFACE-2;i++)
        {
            if(i < struCnfg.stuLayerLampCfg.nLampBoardTotal){

                QMetaObject::invokeMethod(m_pBoardBtnGroup->button(i),"clicked", Qt::QueuedConnection);

                myFlow.sleep(1);
                myFlow.saveScreenshot();
            }
        }

        myFlow.sleep(1);
        struGsh.bScreenShotStatus = 0;
    }
}


void LampBoardCfg::updateBoardBtn()
{
    if(struCnfg.nAllInOne && m_CurBoardIndex == MAX_LAMP_BOARD_PER_INTERFACE-2){
        //ui->pushButton_allInOne->setChecked(true);
    }else{
        if(m_CurBoardIndex >= struCnfg.stuLayerLampCfg.nLampBoardTotal)
            m_CurBoardIndex = 0;
    }
    m_pBoardBtnGroup->button(m_CurBoardIndex)->setChecked(true);
    for(int i = 0;i < MAX_LAMP_BOARD_PER_INTERFACE-2;i++){
        if(i < struCnfg.stuLayerLampCfg.nLampBoardTotal){
            m_pBoardBtnGroup->button(i)->setVisible(true);
        }else{
            m_pBoardBtnGroup->button(i)->setVisible(false);
        }
    }
}

void LampBoardCfg::updateCurBoard()
{
    for(int i = 0;i< MAX_LAMP_PER_BOARD;i++){
        updateLampPortEnableState(i);
        updateLampLayerAddr(i);
        updateLampPortViewBtnState(i);
        updateLampBoardTypeBtnState(i);
        updateLampBoardLimit(i);
        updateLampCoef(i);
        updateLampName(i);
    }
}

void LampBoardCfg::updateLampBoardTypeBtnState(int portIndex)
{
    bool checked = (struCnfg.stuLayerLampCfg.nLampPortType[m_CurBoardIndex][portIndex] == 1);
    m_pLampPortTypeBtn[portIndex]->setChecked(checked);
    if(checked)
        m_pLampPortTypeBtn[portIndex]->setText(g_myLan().ids_bkgLight);
    else
        m_pLampPortTypeBtn[portIndex]->setText(g_myLan().ids_materialLight);
}

void LampBoardCfg::updateLampPortEnableState(int portIndex)
{
    m_plampPortEnable[portIndex]->setChecked(struCnfg.stuLayerLampCfg.nLampPortEnable[m_CurBoardIndex][portIndex]);
}

void LampBoardCfg::updateLampPortViewBtnState(int portIndex)
{
    bool checked = (struCnfg.stuLayerLampCfg.nLampViewAddr[m_CurBoardIndex][portIndex] == 1);
    m_plampViewBtn[portIndex]->setChecked(checked);
    if(struCnfg.nMachineAttr == MACHINE_TYPE_VERTICAL) {//立式机  使用前后
        if(checked)
            m_plampViewBtn[portIndex]->setText(g_myLan().rear);
        else
            m_plampViewBtn[portIndex]->setText(g_myLan().front);
    }else{
        if(checked)
            m_plampViewBtn[portIndex]->setText(g_myLan().upper_layer);
        else
            m_plampViewBtn[portIndex]->setText(g_myLan().mid_layer);
    }
}

void LampBoardCfg::updateLampLayerAddr(int portIndex)
{
    m_pboardLayerEdit[portIndex]->setMinMax(this,0,MAX_LAYER,0,&struCnfg.stuLayerLampCfg.nLampLayerAddr[m_CurBoardIndex][portIndex]);
    m_pboardLayerEdit[portIndex]->setText(QString::number(struCnfg.stuLayerLampCfg.nLampLayerAddr[m_CurBoardIndex][portIndex]));
}

void LampBoardCfg::updateLampBoardLimit(int portIndex)
{
    m_pLimitEdit[portIndex]->setMinMax(this,0,600,0,&struCnfg.stuLayerLampCfg.nLampBoardLimit[m_CurBoardIndex][portIndex]);
    m_pLimitEdit[portIndex]->setText(QString::number(struCnfg.stuLayerLampCfg.nLampBoardLimit[m_CurBoardIndex][portIndex]));
}

void LampBoardCfg::updateLampCoef(int portIndex)
{
    m_pLampCoef[portIndex]->setFloatMinMax(this,0,5.000,0,&struCnfg.stuLayerLampCfg.fLampBoardQuo[m_CurBoardIndex][portIndex]);
    m_pLampCoef[portIndex]->setType(FloatType3);
    m_pLampCoef[portIndex]->setText(QString::number(struCnfg.stuLayerLampCfg.fLampBoardQuo[m_CurBoardIndex][portIndex],'f',3));
}

void LampBoardCfg::updateLampName(int portIndex)
{
    m_pRenameEdit[portIndex]->setType(textType);
    QByteArray lampName(struCnfg.stuLayerLampCfg.sLampName[m_CurBoardIndex][portIndex]);
    m_pRenameEdit[portIndex]->setText(QString::fromUtf8(lampName.data()));
}

void LampBoardCfg::sLampNameChanged()
{
    int index = 0;
    myLineEdit *m_pEdit;
    for(int i = 0; i < MAX_LAMP_PER_BOARD; i++){
        m_pEdit = qobject_cast<myLineEdit*>(sender());
        if(m_pEdit == m_pRenameEdit[i]){
            index = i;
            break;
        }
    }
    memset(struCnfg.stuLayerLampCfg.sLampName[m_CurBoardIndex][index],0,sizeof(struCnfg.stuLayerLampCfg.sLampName[m_CurBoardIndex][index]));
    QString strText = m_pRenameEdit[index]->text();
    QByteArray arrName = strText.toUtf8();
    const char *strName = arrName.data();
    memcpy(struCnfg.stuLayerLampCfg.sLampName[m_CurBoardIndex][index],strName,strlen(strName));
}

void LampBoardCfg::updateLayerSetVisiable()
{
    bool bvisiable = false;
    if(struCnfg.nLayerTotal > 1){
        bvisiable = true;
    }
    ui->label_layer->setVisible(bvisiable);
    for(int i = 0; i < MAX_LAMP_PER_BOARD;i++){
        m_pboardLayerEdit[i]->setVisible(bvisiable);
    }
}

void LampBoardCfg::lampBoardBtnClicked(int boardIndex)
{
    m_CurBoardIndex = boardIndex;
    updateBoardBtn();
    updateCurBoard();
}

void LampBoardCfg::lampTypeBtnClicked(int portIndex)
{
    if(struCnfg.stuLayerLampCfg.nLampPortType[m_CurBoardIndex][portIndex] == 0)
        struCnfg.stuLayerLampCfg.nLampPortType[m_CurBoardIndex][portIndex] = 1;
    else
        struCnfg.stuLayerLampCfg.nLampPortType[m_CurBoardIndex][portIndex] = 0;
    updateLampBoardTypeBtnState(portIndex);

}

void LampBoardCfg::lampViewBtnClicked(int portIndex)
{
    if(struCnfg.stuLayerLampCfg.nLampViewAddr[m_CurBoardIndex][portIndex] == 0)
        struCnfg.stuLayerLampCfg.nLampViewAddr[m_CurBoardIndex][portIndex] = 1;
    else
        struCnfg.stuLayerLampCfg.nLampViewAddr[m_CurBoardIndex][portIndex] = 0;
    updateLampPortViewBtnState(portIndex);
}

void LampBoardCfg::lampPortEnableClicked(int portIndex)
{
    if(struCnfg.stuLayerLampCfg.nLampPortEnable[m_CurBoardIndex][portIndex])
        struCnfg.stuLayerLampCfg.nLampPortEnable[m_CurBoardIndex][portIndex] =0;
    else
        struCnfg.stuLayerLampCfg.nLampPortEnable[m_CurBoardIndex][portIndex] = 1;
}

void LampBoardCfg::on_okbtn_clicked()
{
    myMessageBox msgBox(MSG_QUES, g_myLan().ids_toSaveParameters);
    int ret = msgBox.exec();
    if (ret == QDialog::Accepted)
    {
        logger()->info("applying lamp board setting");
        myFlow.saveGlobal();
    }
    returnBack();
}

void LampBoardCfg::on_restoreBtn_clicked()
{
    myMessageBox msgBox(MSG_QUES, g_myLan().cfm_restore_default);
    int ret = msgBox.exec();
    if (ret == QDialog::Accepted)
    {
        logger()->info("restoring lamp board setting");
        //myFlow.getGlobalLampConfig();

        myFlow.getDefaultLampConfig();
        memcpy(&_t_struCnfg.stuLayerLampCfg,&struCnfg.stuLayerLampCfg,sizeof(stru_layer_lamp_cfg));
        showPage(true);
    }
}

void LampBoardCfg::on_pushButton_allInOne_clicked(bool checked)
{
    if(checked){
        m_CurBoardIndex = MAX_LAMP_BOARD_PER_INTERFACE-2;
        updateBoardBtn();
        updateCurBoard();
    }
}


