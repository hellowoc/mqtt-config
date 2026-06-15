#include "camerasetwidget.h"
#include "ui_camerasetwidget.h"
#include <qmath.h>
#include "globalflow.h"
#include "constant.h"
CameraSetWidget::CameraSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::CameraSetWidget)
{
    ui->setupUi(this);
    m_pTableWidget = ui->tableWidget;
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setColumnWidth(0,250);
    ui->tableWidget->setColumnWidth(1,80);

    m_pTableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    m_pTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pTableWidget->setFocusPolicy(Qt::NoFocus);
    ui->tableWidget->setShowGrid(false);

    QHeaderView *header = m_pTableWidget->verticalHeader();
    header->setHidden(true);// 隐藏行号

    QHeaderView *horiHeader = m_pTableWidget->horizontalHeader();
    horiHeader->setFixedHeight(45);
    horiHeader->setSectionResizeMode(QHeaderView::Stretch);

    ui->sensorType->setEditable(false);
    QStringList list;
    list<<"T1"<<"T1_850"<<"C1"<<"D1"<<"D2"<<"H1"<<"G2"<<"C1-2"<<"G3";
    ui->sensorType->addItems(list);

    m_list1<<"1024"<<"2048"<<"256"<<"512"<<"x1152"<<"4096"<<"x1664"<<"x3328"<<"1024-TD"<<"x2304"<<"2048-TD"<<"2048-XTD"<<"2048-GTD";
    ui->SensorPixelcomboBox->addItems(m_list1);

    QStringList list2,list6;
    list2<<QString("1")<<QString("2")<<QString("3")<<QString("4");
    ui->SensorDimComboBox->addItems(list2);
    ui->SensorDimComboBox_2->addItems(list2);

    list2.clear();
    list2<<QString("0")<<QString("1")<<QString("2")<<QString("3");

    list2.clear();
    list2<<"SS"<<"SS1"<<"SS2"<<"SS3";
    ui->TDscheme->addItems(list2);

    m_list3<<"8"<<"9"<<"10";
    ui->SensBitWidthComboBox->addItems(m_list3);

    m_list4<<"0.5"<<"1"<<"2"<<"2.5"<<"2.8"<<"3"<<"3.5"<<"3.6";
    ui->SensorRowFreComboBox->addItems(m_list4);

    list6<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"10"<<"11";
    ui->chainAddrCombox->addItems(list6);

    m_list5<<"0"<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7";
    ui->BrightSelectComboBox->addItems(m_list5);
    m_list5.clear();
    m_list5<<"0"<<"1";
    ui->MirrorComboBox->addItems(m_list5);
    m_list5<<"2"<<"3";
    ui->SensorImageComboBox->addItems(m_list5);

    m_list6<<"LS"<<"LD";
    ui->MachineTypeComboBox->addItems(m_list6);

    m_list_7<<"10M"<<"20M";
    ui->SensorCmdSpeedComboBox->addItems(m_list_7);

    m_list_7.clear();
    m_list_7<<"0"<<"1";
    ui->ResizeModeComboBox->addItems(m_list_7);
    m_list_7.clear();
    m_list_7<<"model_1"<<"model_2";
    ui->UserMode->addItems(m_list_7);


    m_pViewBtn[0] = ui->frontMainBtn;
    m_pViewBtn[1] = ui->rearMainBtn;
    m_pViewBtn[2] = ui->frontViceBtn;
    m_pViewBtn[3] = ui->rearViceBtn;
    m_pViewBtn[4] = ui->rearViceBtn_2;
    m_pViewBtn[5] = ui->rearViceBtn_3;
    m_pViewBtn[6] = ui->rearViceBtn_4;
    m_pViewBtn[7] = ui->rearViceBtn_5;
    m_pLayerBtn[0] = ui->mainBtn;
    m_pLayerBtn[1] = ui->viceBtn;

    QStringList listColorMode;
    listColorMode<<g_myLan().mode+"0"<<struCnfp.sD2ImgModeName[0]<<struCnfp.sD2ImgModeName[1]<<struCnfp.sD2ImgModeName[2]<<struCnfp.sD2ImgModeName[3];
    ui->colorModeComboBox->addItems(listColorMode);

    //位宽隐藏
    ui->label_3->hide();
    ui->SensBitWidthComboBox->hide();
    ui->label_6->hide();
    ui->chainAddrCombox->hide();
    ui->label_5->hide();
    ui->label_16->hide();
    ui->ResizeModeComboBox->hide();
    ui->label_12->hide();
    ui->SensorDropLineEdit->hide();
    setSlotConnect();

    for(int i = 0; i< ARITHMETIC_TOTAL;i++)
        m_arithCheckBox[i] = NULL;

    shottimer = new QTimer;
    connect(shottimer,SIGNAL(timeout()),this, SLOT(getscreenshot()));


}

CameraSetWidget::~CameraSetWidget()
{
    delete ui;
}

void CameraSetWidget::retranslateUiWidget()
{
    ui->sensorTypelbl->setText(g_myLan().sensor_type);
    ui->label->setText(g_myLan().sensor_dip);
    ui->label_2->setText(g_myLan().sensor_dim);
    ui->label_13->setText(g_myLan().sensor_dim+"2");
    ui->label_3->setText(g_myLan().sensor_bit_width);
    ui->label_4->setText(g_myLan().sensor_fre);
    ui->label_9->setText(g_myLan().sensor_fre);
    ui->label_5->setText(g_myLan().ids_model);
    ui->label_6->setText(g_myLan().camera_chain_addr);
    ui->label_7->setText(g_myLan().ids_imgProcess);

    ui->viewsetgroup->setTitle(g_myLan().camera_arith_config);
    ui->applyBtn->setText(g_myLan().ids_apply);
    ui->restoreBtn->setText(g_myLan().ids_restore);
    ui->label_8->setText(g_myLan().ids_machineModel);
    ui->label_10->setText(g_myLan().ids_nMirror);
    ui->label_11->setText(g_myLan().brightSelect);
    ui->imgCompresscheckBox->setText(g_myLan().compression);
    ui->label_12->setText(g_myLan().dropLine);
    ui->label_14->setText(g_myLan().colorSelect);
    ui->label_15->setText(g_myLan().speed);
    ui->label_16->setText(g_myLan().resizeMode);
    ui->label_17->setText(g_myLan().realtimeCorrect);
    ui->label_18->setText(g_myLan().mode);
    ui->label_20->setText(g_myLan().image_enhance);
}

void CameraSetWidget::showPage(bool bshow)
{
    basewidget::showPage(bshow);
    if(bshow){
        memcpy(&m_temCnfg,&struCnfg,sizeof(struCnfGlobal));
        memcpy(&m_temCnfp,&struCnfp,sizeof(struCnfProfile));

        for(int i = 0;i < MAX_VIEW;i++){
            splice[i] = m_temCnfp.nImageSplice[struGsh.nLayer][i];
        }
        QStringList list,list5;
        list<<g_myLan().camera_arith_list<<g_myLan().state;
        list5<<g_myLan().ids_model +QString(" 1")<<g_myLan().ids_model +QString(" 2")<<g_myLan().ids_model +QString(" 3")<<g_myLan().ids_model +QString(" 4");
        ui->CameraBoardModeComboBox->clear();
        ui->CameraBoardModeComboBox->addItems(list5);
        m_pTableWidget->setHorizontalHeaderLabels(list);

        updateViewButtonVisible();
        updateCameraSet();
        updateArithState();

        if(struGsh.bScreenShotStatus == 1)
        {
            shottimer->start(1000);

        }
    }else{

    }
}

void CameraSetWidget::OnViewChange(int view)
{
    updateCameraSet();
    updateArithState();
}

void CameraSetWidget::OnLayerChange()
{
    updateCameraSet();
    updateArithState();
}

void CameraSetWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){
        switch (msg2) {
        case 39:
            if(freq > 0){
                m_temCnfp.nSensorRowFre[struGsh.nLayer][struGsh.nView] = freq*100+0.5;
            }
            break;
        }
    }
}

void CameraSetWidget::getscreenshot()
{
    shottimer->stop();
    {
        for(int i=0;i<MAX_VIEW;i++){
            if(m_temCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[i])
            {
                QMetaObject::invokeMethod(m_pViewBtn[i],"clicked", Qt::QueuedConnection);

                myFlow.sleep(1);
                myFlow.saveScreenshot();
            }
        }
        myFlow.sleep(1);
        struGsh.bScreenShotStatus = 0;
    }
}

void CameraSetWidget::viewBtnClicked(bool checked)
{
    QPushButton* btn = (QPushButton*)sender();
    int index = btn->whatsThis().toInt();

    for(int i = 0;i < MAX_VIEW;i++){
        if(checked){
            if(index == i){
                struGsh.nView = index;
                m_pViewBtn[i]->setEnabled(false);
            }else{
                m_pViewBtn[i]->setChecked(false);
                m_pViewBtn[i]->setEnabled(true);
            }
        }
    }
    updateArithState();
}

void CameraSetWidget::updateArithState()
{
    for(int i = 0;i < m_arithIndexMap.count();i++){
        disconnect(m_arithCheckBox[i],SIGNAL(clicked(bool)),this,SLOT(arithCheckboxClicked(bool)));
    }
    while (m_pTableWidget->rowCount() > 0) {
        QTableWidgetItem *item = m_pTableWidget->takeItem(0,0);
        if(item){
            delete item;
        }
        QWidget* widget = m_pTableWidget->cellWidget(0,1);
        if(widget)
            delete widget;
        m_pTableWidget->removeRow(0);
    }
    m_pTableWidget->clear();
    m_arithIndexMap.clear();
    int view = struGsh.nView;
    //switch(m_temCnf.struLayerInfo[0].nViewSensorType[view])
    switch(m_temCnfp.nViewSensorType[0][struGsh.nView])
    {
    case SENSOR_T1:
    case SENSOR_D2:
    case SENSOR_G2:
    case SENSOR_G3:
        m_arithEnable[ARITH_GREY_A] = 1;
        m_arithEnable[ARITH_GREY_B] = 1;
        m_arithEnable[ARITH_GREY_C] = 1;
        m_arithEnable[ARITH_GREY_D] = 0;
        m_arithEnable[ARITH_DISCOLOR_A] = 1;
        m_arithEnable[ARITH_DISCOLOR_B] = 1;
        m_arithEnable[ARITH_CROSS] = 1;
        m_arithEnable[ARITH_SVM_A] = 1;
        m_arithEnable[ARITH_SVM_B] = 1;
        m_arithEnable[ARITH_SVM_C] = 1;
        m_arithEnable[ARITH_SVM_D] = 1;
        m_arithEnable[ARITH_SHAPE_SVM_A] = 0;
        m_arithEnable[ARITH_SHAPE_SVM_B] = 0;
        m_arithEnable[ARITH_SHAPE_SVM_C] = 0;
        m_arithEnable[ARITH_BIG_SMALL] = 1;
        m_arithEnable[ARITH_BIG_SMALL_B] = 0;
        m_arithEnable[ARITH_LONG_SHORT] = 1;
        m_arithEnable[ARITH_LONG_SHORT_B] = 0;
        m_arithEnable[ARITH_CIRCLE_LONG] = 1;
        m_arithEnable[ARITH_CIRCLE_LONG_B] = 0;
        m_arithEnable[ARITH_HSV] = 1;
        m_arithEnable[ARITH_HSV_B] = 1;
        m_arithEnable[ARITH_HSV_C] = 0;
        m_arithEnable[ARITH_SAT] = 0;
        m_arithEnable[ARITH_HUE] = 0;
        m_arithEnable[ARITH_SCALE] = 0;
        m_arithEnable[ARITH_MILDEW] = 0;
        m_arithEnable[ARITH_OUTLINE] = 0;
        m_arithEnable[ARITH_LINE] = 0;
        m_arithEnable[ARITH_BUD] = 0;
        m_arithEnable[ARITH_ROUND] = 1;
        m_arithEnable[ARITH_ROUND_B] = 0;
        m_arithEnable[ARITH_VALUE] = 0;
        m_arithEnable[ARITH_SPL_MILDEW] = 0;
        m_arithEnable[ARITH_BUD_B]  = 0;
        m_arithEnable[ARITH_W_PEANUT] = 0;
        m_arithEnable[ARITH_PEPPER_HANDLE] = 0;
        m_arithEnable[ARITH_SUPPRESS_LARGE_A] = 0;
        m_arithEnable[ARITH_SUPPRESS_LARGE_B] = 0;
        m_arithEnable[ARITH_SUPPRESS_LARGE_C] = 0;
        m_arithEnable[TEA_ARITH_COLOR] = 1;
        m_arithEnable[TEA_ARITH_COLOR_2] = 1;
        m_arithEnable[ARITH_WHEAT_SPROUT] = 0;
        break;
    case SENSOR_T1_850:
        m_arithEnable[ARITH_GREY_A] = 1;
        m_arithEnable[ARITH_GREY_B] = 1;
        m_arithEnable[ARITH_GREY_C] = 0;
        m_arithEnable[ARITH_GREY_D] = 0;
        m_arithEnable[ARITH_DISCOLOR_A] = 0;
        m_arithEnable[ARITH_DISCOLOR_B] = 0;
        m_arithEnable[ARITH_CROSS] = 0;
        m_arithEnable[ARITH_SVM_A] = 0;
        m_arithEnable[ARITH_SVM_B] = 0;
        m_arithEnable[ARITH_SVM_C] = 0;
        m_arithEnable[ARITH_SVM_D] = 0;
        m_arithEnable[ARITH_SHAPE_SVM_A] = 0;
        m_arithEnable[ARITH_SHAPE_SVM_B] = 0;
        m_arithEnable[ARITH_SHAPE_SVM_C] = 0;
        m_arithEnable[ARITH_BIG_SMALL] = 0;
        m_arithEnable[ARITH_BIG_SMALL_B] = 0;
        m_arithEnable[ARITH_LONG_SHORT] = 0;
        m_arithEnable[ARITH_LONG_SHORT_B] = 0;
        m_arithEnable[ARITH_CIRCLE_LONG] = 0;
        m_arithEnable[ARITH_CIRCLE_LONG_B] = 0;
        m_arithEnable[ARITH_HSV] =0;
        m_arithEnable[ARITH_HSV_B] =0;
        m_arithEnable[ARITH_HSV_C] =0;
        m_arithEnable[ARITH_SAT] = 0;
        m_arithEnable[ARITH_HUE] = 0;
        m_arithEnable[ARITH_SCALE] = 0;
        m_arithEnable[ARITH_MILDEW] = 0;
        m_arithEnable[ARITH_OUTLINE] = 0;
        m_arithEnable[ARITH_LINE] = 0;
        m_arithEnable[ARITH_BUD] = 0;
        m_arithEnable[ARITH_ROUND] = 0;
        m_arithEnable[ARITH_ROUND_B] = 0;
        m_arithEnable[ARITH_VALUE] = 0;
        m_arithEnable[ARITH_SPL_MILDEW] = 0;
        m_arithEnable[ARITH_BUD_B]  = 0;
        m_arithEnable[ARITH_W_PEANUT] = 0;
        m_arithEnable[ARITH_PEPPER_HANDLE] = 0;
        m_arithEnable[ARITH_SUPPRESS_LARGE_A] = 0;
        m_arithEnable[ARITH_SUPPRESS_LARGE_B] = 0;
        m_arithEnable[ARITH_SUPPRESS_LARGE_C] = 0;
        m_arithEnable[ARITH_WHEAT_SPROUT] = 0;
        m_arithEnable[TEA_ARITH_COLOR] = 1;
        m_arithEnable[TEA_ARITH_COLOR_2] = 1;
        m_arithEnable[TEA_ARITH_COLOR_3] = 0;
        m_arithEnable[TEA_ARITH_COLOR_4] = 0;
        m_arithEnable[TEA_ARITH_COLOR_5] = 0;
        break;
    case SENSOR_C1:
    case SENSOR_H1:
        m_arithEnable[ARITH_GREY_A] = 1;
        m_arithEnable[ARITH_GREY_B] = 1;
        m_arithEnable[ARITH_GREY_C] = 0;
        m_arithEnable[ARITH_GREY_D] = 0;
        m_arithEnable[ARITH_DISCOLOR_A] = 1;
        m_arithEnable[ARITH_DISCOLOR_B] = 1;
        m_arithEnable[ARITH_CROSS] = 0;
        m_arithEnable[ARITH_SVM_A] = 0;
        m_arithEnable[ARITH_SVM_B] = 0;
        m_arithEnable[ARITH_SVM_C] = 0;
        m_arithEnable[ARITH_SVM_D] = 0;
        m_arithEnable[ARITH_SHAPE_SVM_A] = 0;
        m_arithEnable[ARITH_SHAPE_SVM_B] = 0;
        m_arithEnable[ARITH_SHAPE_SVM_C] = 0;
        m_arithEnable[ARITH_BIG_SMALL] = 0;
        m_arithEnable[ARITH_BIG_SMALL_B] = 0;
        m_arithEnable[ARITH_LONG_SHORT] = 0;
        m_arithEnable[ARITH_LONG_SHORT_B] = 0;
        m_arithEnable[ARITH_CIRCLE_LONG] = 0;
        m_arithEnable[ARITH_CIRCLE_LONG_B] = 0;
        m_arithEnable[ARITH_HSV] =0;
        m_arithEnable[ARITH_HSV_B] =0;
        m_arithEnable[ARITH_HSV_C] =0;
        m_arithEnable[ARITH_SAT] = 0;
        m_arithEnable[ARITH_HUE] = 0;
        m_arithEnable[ARITH_SCALE] = 0;
        m_arithEnable[ARITH_MILDEW] = 0;
        m_arithEnable[ARITH_OUTLINE] = 0;
        m_arithEnable[ARITH_LINE] = 0;
        m_arithEnable[ARITH_BUD] = 0;
        m_arithEnable[ARITH_ROUND] = 0;
        m_arithEnable[ARITH_ROUND_B] = 0;
        m_arithEnable[ARITH_VALUE] = 0;
        m_arithEnable[ARITH_SPL_MILDEW] = 0;
        m_arithEnable[ARITH_BUD_B]  = 0;
        m_arithEnable[ARITH_W_PEANUT] = 0;
        m_arithEnable[ARITH_PEPPER_HANDLE] = 0;
        m_arithEnable[ARITH_SUPPRESS_LARGE_A] = 0;
        m_arithEnable[ARITH_SUPPRESS_LARGE_B] = 0;
        m_arithEnable[ARITH_SUPPRESS_LARGE_C] = 0;
        m_arithEnable[ARITH_WHEAT_SPROUT] = 0;
        m_arithEnable[TEA_ARITH_COLOR] = 1;
        m_arithEnable[TEA_ARITH_COLOR_2] = 1;
        m_arithEnable[TEA_ARITH_COLOR_3] = 0;
        m_arithEnable[TEA_ARITH_COLOR_4] = 0;
        m_arithEnable[TEA_ARITH_COLOR_5] = 0;
        break;
    case SENSOR_C1_2:
        m_arithEnable[ARITH_GREY_A] = 1;
        m_arithEnable[ARITH_GREY_B] = 1;
        m_arithEnable[ARITH_GREY_C] = 1;
        m_arithEnable[ARITH_GREY_D] = 0;
        m_arithEnable[ARITH_DISCOLOR_A] = 1;
        m_arithEnable[ARITH_DISCOLOR_B] = 1;
        m_arithEnable[ARITH_CROSS] = 1;
        m_arithEnable[ARITH_SVM_A] = 0;
        m_arithEnable[ARITH_SVM_B] = 0;
        m_arithEnable[ARITH_SVM_C] = 0;
        m_arithEnable[ARITH_BIG_SMALL] = 1;
        m_arithEnable[ARITH_BIG_SMALL_B] = 0;
        m_arithEnable[ARITH_LONG_SHORT] = 1;
        m_arithEnable[ARITH_LONG_SHORT_B] = 0;
        m_arithEnable[ARITH_CIRCLE_LONG] = 0;
        m_arithEnable[ARITH_CIRCLE_LONG_B] = 0;
        m_arithEnable[ARITH_SAT] = 0;
        m_arithEnable[ARITH_HUE] = 0;
        m_arithEnable[ARITH_SCALE] = 0;
        m_arithEnable[ARITH_MILDEW] = 0;
        m_arithEnable[ARITH_OUTLINE] = 0;
        m_arithEnable[ARITH_LINE] = 0;
        m_arithEnable[ARITH_BUD] = 0;
        m_arithEnable[ARITH_ROUND] = 1;
        m_arithEnable[ARITH_BUD_B] = 0;
        m_arithEnable[ARITH_ROUND_B] = 0;
        m_arithEnable[ARITH_W_PEANUT] = 0;
        m_arithEnable[ARITH_SPL_MILDEW] = 0;
        m_arithEnable[ARITH_VALUE] = 0;
        m_arithEnable[ARITH_PEPPER_HANDLE] = 0;
        m_arithEnable[ARITH_SHAPE_SVM_A] = 0;
        m_arithEnable[ARITH_SHAPE_SVM_B] = 0;
        m_arithEnable[ARITH_SHAPE_SVM_C] = 0;
        m_arithEnable[ARITH_HSV] = 0;
        m_arithEnable[ARITH_HSV_B] = 0;
        break;
    default:
        break;
    }
    int index = 0;
    for(int i = 0;i < ARITHMETIC_TOTAL;i++){
        if(m_arithEnable[i]){
            m_arithIndexMap.insert(index,i);
            index++;
        }
    }
    QStringList list;
    list<<g_myLan().camera_arith_list<<g_myLan().state;
    m_pTableWidget->setHorizontalHeaderLabels(list);
    ui->tableWidget->setRowCount(index);
    qDebug()<<"index:"<<index<<m_arithIndexMap.size();
    for(int i = 0;i< index;i++){
        qDebug()<<i;
        QTableWidgetItem *item0 = new QTableWidgetItem(myFlow.getArithNameByIndex(m_arithIndexMap[i]));
        //        item0->setTextAlignment(Qt::AlignCenter);
        item0->setFlags(Qt::ItemIsEnabled);
        m_pTableWidget->setItem(i,0, item0);
        QCheckBox *checkbox = new QCheckBox;
        checkbox->setWhatsThis(QString::number(m_arithIndexMap[i]));
        m_arithCheckBox[i] = checkbox;
        if(m_temCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].nViewArithVisiable[m_arithIndexMap[i]]){
            checkbox->setChecked(true);
        }
        QWidget *widget = new QWidget;
        QHBoxLayout *layout = new QHBoxLayout;
        layout->setSpacing(0);
        layout->setMargin(0);
        layout->addWidget(m_arithCheckBox[i],0,Qt::AlignHCenter);
        widget->setLayout(layout);
        m_pTableWidget->setCellWidget(i,1,widget);
        m_pTableWidget->setRowHeight(i,50);
        connect(m_arithCheckBox[i],SIGNAL(clicked(bool)),this,SLOT(arithCheckboxClicked(bool)));
    }

    retranslateUiWidget();
}

void CameraSetWidget::updateCameraSet()
{
    int view = struGsh.nView;
    stru_view_info viewInfo;
    memcpy(&viewInfo,&m_temCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view],sizeof(viewInfo));
    // ui->sensorType->setCurrentIndex(m_temCnf.struLayerInfo[struGsh.nLayer].nViewSensorType[view]-1);
    ui->sensorType->setCurrentIndex(m_temCnfp.nViewSensorType[0][view] -1);
    ui->SensorPixelcomboBox->setCurrentIndex(m_temCnfp.nSensorPixel[0][view]);
    ui->SensorDimComboBox->setCurrentIndex(m_temCnfp.nSensorDim[0][view]-1);
    if(m_temCnfp.nSensorDimD2[0][view] < 1) m_temCnfp.nSensorDimD2[0][view]= m_temCnfp.nSensorDim[0][view];
    ui->SensorDimComboBox_2->setCurrentIndex(m_temCnfp.nSensorDimD2[0][view]-1);
    ui->SensorDimComboBox_2->setVisible(ui->sensorType->currentText()=="D2");
    ui->label_13->setVisible(ui->sensorType->currentText()=="D2");
    ui->SensBitWidthComboBox->setCurrentIndex(m_list3.indexOf(QString::number(m_temCnfp.nSensBitWidth[0][view])));
    //    updateSensorFreCombox();
    //    int index = getCurrentFreqIndex();
    //    ui->SensorRowFreComboBox->setCurrentIndex(index);
    //    ui->CameraBoardModeComboBox->setCurrentIndex(m_temCnf.nCameraBoardMode[struGsh.nLayer][view]-1);
    ui->CameraBoardModeComboBox->setCurrentIndex(m_temCnfp.nCameraCompressMode[struGsh.nLayer][view]);
    ui->chainAddrCombox->setCurrentIndex(m_temCnfp.nViewChainAddr[struGsh.nLayer][view]);
    ui->SensorImageComboBox->setCurrentIndex(m_temCnfp.nImageSplice[struGsh.nLayer][view]);
    ui->MachineTypeComboBox->setCurrentIndex(m_temCnfp.nSensorDataType[struGsh.nLayer][view]);
    ui->MirrorComboBox->setCurrentIndex(m_temCnfp.nMirror[struGsh.nLayer][view]);
    ui->BrightSelectComboBox->setCurrentIndex(m_temCnfp.nSenseBrightSelection[struGsh.nLayer][view]);
    freq = m_temCnfp.nSensorRowFre[struGsh.nLayer][view]/100.0;
    ui->SensorRowFreLineEdit->setFloatMinMax2(this,0.5,50,39,&freq,percentFloatType2);
    ui->SensorRowFreLineEdit->setText(QString::number(freq,'f',2));
    ui->SensorDropLineEdit->setMinMax(this,0,100,40,&m_temCnfp.nSensDropLine[struGsh.nLayer][view]);
    ui->SensorDropLineEdit->setText(QString::number(m_temCnfp.nSensDropLine[struGsh.nLayer][view]));

    ui->label_4->setVisible(false);
    ui->SensorRowFreComboBox->setVisible(false);
    ui->label_9->setVisible(m_temCnfp.nSensorDataType[struGsh.nLayer][view] >=0 );
    ui->SensorRowFreLineEdit->setVisible(m_temCnfp.nSensorDataType[struGsh.nLayer][view] >=0 );

    ui->imgCompresscheckBox->setChecked(m_temCnfp.nIsCameraConpress == 1);
    ui->imgCompresscheckBox->setVisible(m_temCnfg.nDPMachine & m_temCnfg.nUseIpcEnable);

    ui->CameraBoardModeComboBox->setVisible(m_temCnfp.nIsCameraConpress);
    ui->checkBox_B->setChecked(!m_temCnfp.nSensorColorChoose[struGsh.nLayer][view][0]);
    ui->checkBox_G->setChecked(!m_temCnfp.nSensorColorChoose[struGsh.nLayer][view][1]);
    ui->checkBox_R->setChecked(!m_temCnfp.nSensorColorChoose[struGsh.nLayer][view][2]);
    ui->checkBox_N->setChecked(!m_temCnfp.nSensorColorChoose[struGsh.nLayer][view][3]);

    ui->checkBox_B->setVisible(ui->sensorType->currentText()=="D2"&false);
    ui->checkBox_G->setVisible(ui->sensorType->currentText()=="D2"&false);
    ui->checkBox_R->setVisible(ui->sensorType->currentText()=="D2"&false);
    ui->checkBox_N->setVisible(ui->sensorType->currentText()=="D2"&false);
    ui->label_14->setVisible(ui->sensorType->currentText()=="D2");

    ui->colorModeComboBox->setCurrentIndex(m_temCnfp.nSensorD2Mode[struGsh.nLayer][view]);
    ui->colorModeComboBox->setVisible(ui->sensorType->currentText()=="D2");
    ui->SensorCmdSpeedComboBox->setCurrentIndex(m_temCnfp.nSensCmdSpeed[struGsh.nLayer][view]);
    ui->ResizeModeComboBox->setCurrentIndex(m_temCnfp.nSensResizeMode[struGsh.nLayer][view]);
    ui->correctionCheckBox->setChecked(m_temCnfp.nRealTimeCorrection[struGsh.nLayer][view]);
    ui->TDscheme->setCurrentIndex(m_temCnfp.nTimeDivisionScheme[struGsh.nLayer][view]);
    ui->TDscheme->setVisible(true);
    //ui->UserMode->setCurrentIndex(m_temCnfp.nCameraBoardMode[0][struGsh.nView]-1);
    ui->UserMode->setCurrentIndex(m_temCnfp.nCameraBoardUserMode[0][struGsh.nView][0]-1);
    ui->inhanceCheckBox->setVisible(struCnfg.nMachineType == MACHINE_LD_ZL_GP || struCnfg.nMachineType == MACHINE_LD2_ZL_GP);
    ui->label_20->setVisible(struCnfg.nMachineType == MACHINE_LD_ZL_GP || struCnfg.nMachineType == MACHINE_LD2_ZL_GP);
    ui->inhanceCheckBox->setChecked(m_temCnfp.imageEnahnce[view]);
}

void CameraSetWidget::updateSensorFreCombox()
{
    m_list4.clear();
    int view = struGsh.nView;
    //switch(m_temCnf.struLayerInfo[struGsh.nLayer].nViewSensorType[view])
    switch(m_temCnfp.nViewSensorType[0][view])
    {
    case SENSOR_T1:
        m_list4<<"0.5"<<"1"<<"2"<<"2.5"<<"2.8"<<"3";
        break;
    case SENSOR_D2:
        m_list4<<"0.5"<<"1"<<"2"<<"2.5"<<"2.8"<<"3"<<"3.5"<<"3.6";
        break;
    default:
        m_list4<<"0.5"<<"1"<<"2"<<"2.5"<<"2.8"<<"3";
        break;
    }
    ui->SensorRowFreComboBox->clear();
    ui->SensorRowFreComboBox->addItems(m_list4);
    //    ui->SensorRowFreComboBox->setCurrentIndex(index);
}
void CameraSetWidget::arithCheckboxClicked(bool checked)
{
    QCheckBox *checkbox = (QCheckBox*)(sender());
    int arithIndex = checkbox->whatsThis().toInt();
    int view = struGsh.nView;
    m_temCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].nViewArithVisiable[arithIndex] = (int)checked;
}

void CameraSetWidget::on_SensorPixelcomboBox_activated(int index)
{
    int view = struGsh.nView;
    m_temCnfp.nSensorPixel[0][view] = index;
    switch (index ) {
    case SENSOR_PIXELS_1024:
        myFlow.imgHeight = 416;
        break;
    case SENSOR_PIXELS_2048:
    case SENSOR_PIXELS_2048_XTD:
    case SENSOR_PIXELS_2048_GTD:
        myFlow.imgHeight = 832;
        break;
    default:
        myFlow.imgHeight = 416;
        break;
    }

    updateCameraSet();
}

void CameraSetWidget::on_chainAddrCombox_activated(int index)
{
    int view = struGsh.nView;
    m_temCnfp.nViewChainAddr[struGsh.nLayer][view] = index;
}

void CameraSetWidget::on_CameraBoardModeComboBox_activated(int index)
{
    int view = struGsh.nView;
    m_temCnfp.nCameraCompressMode[struGsh.nLayer][view] = index;
}

void CameraSetWidget::on_SensorDimComboBox_activated(int index)
{
    int view = struGsh.nView;
    m_temCnfp.nSensorDim[0][view] = index+1;
}

void CameraSetWidget::on_SensorDimComboBox_2_activated(int index)
{
    int view = struGsh.nView;
    m_temCnfp.nSensorDimD2[0][view] = index+1;
}

void CameraSetWidget::on_SensBitWidthComboBox_activated(const QString &arg1)
{
    int view = struGsh.nView;
    m_temCnfp.nSensBitWidth[0][view] = arg1.toInt();
}


void CameraSetWidget::on_sensorType_activated(int index)
{
    int view = struGsh.nView;

    m_temCnfp.nViewSensorType[0][view] = index+1;
    updateArithState();
    updateCameraSet();
    on_SensorRowFreComboBox_activated(index);
}

void CameraSetWidget::on_applyBtn_clicked()
{
    m_temCnfg.nRealTimeCorrection = 0;
    for(int i = 0; i < m_temCnfg.nLayerTotal; i++)
    {
        for(int j = 0; j < m_temCnfg.struLayerInfo[i].nViewChainTotal; j++)
        {
            for(int m = 0; m < MAX_GROUP_IDENTIFY; m++)
            {
                for(int k = 0;k < MAX_GREY_COLOR_ARITH_TOTAL;k++)
                {
                    if(m_temCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_GREY_A+k] == 0)
                    {
                        m_temCnfp.struGroupIdentify[i][j][m].struGreyColor[k].nArithEnable = 0;
                        m_temCnfp.struGroupIdentify[i][j][m].struGreyColor[k].nDispEnable = 0;
                    }
                }
                for(int k = 0;k < MAX_SVM_ARITH_TOTAL;k++)
                {
                    if(m_temCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SVM_A+k] == 0)
                    {
                        m_temCnfp.struGroupIdentify[i][j][m].struSvm[k].nArithEnable = 0;
                        m_temCnfp.struGroupIdentify[i][j][m].struSvm[k].nDispEnable = 0;
                    }
                }
                for(int k = 0;k < MAX_SHAPE_SVM_ARITH_TOTAL;k++)
                {
                    if(m_temCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SHAPE_SVM_A+k] == 0)
                    {
                        m_temCnfp.struGroupIdentify[i][j][m].struShapeSvm[k].nArithEnable = 0;
                        m_temCnfp.struGroupIdentify[i][j][m].struShapeSvm[k].nDispEnable = 0;
                    }
                }
                for(int k = 0;k < MAX_LONG_SHORT_TOTAL;k++)
                {
                    if(m_temCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_LONG_SHORT+k] == 0)
                    {
                        m_temCnfp.struGroupIdentify[i][j][m].struLongShort[k].nArithEnable = 0;
                        m_temCnfp.struGroupIdentify[i][j][m].struLongShort[k].nDispEnable = 0;
                    }
                }
                for(int k = 0;k < MAX_BUD_TOTAL;k++)
                {
                    if(m_temCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_BUD+k] == 0)
                    {
                        m_temCnfp.struGroupIdentify[i][j][m].struBud[k].nArithEnable = 0;
                        m_temCnfp.struGroupIdentify[i][j][m].struBud[k].nDispEnable = 0;
                    }
                }

                for(int k = 0;k < MAX_BIG_SMALL_TOTAL;k++)
                {
                    if(m_temCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_BIG_SMALL+k] == 0)
                    {
                        m_temCnfp.struGroupIdentify[i][j][m].struBigSmall[k].nArithEnable = 0;
                        m_temCnfp.struGroupIdentify[i][j][m].struBigSmall[k].nDispEnable = 0;
                    }
                }

                for(int k = 0;k < MAX_CIRCLE_LONG_TOTAL;k++)
                {
                    if(m_temCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_CIRCLE_LONG+k] == 0)
                    {
                        m_temCnfp.struGroupIdentify[i][j][m].struCircleLong[k].nArithEnable = 0;
                        m_temCnfp.struGroupIdentify[i][j][m].struCircleLong[k].nDispEnable = 0;
                    }
                }

                if(m_temCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SAT] == 0)
                {
                    m_temCnfp.struGroupIdentify[i][j][m].struColorSat.nArithEnable = 0;
                    m_temCnfp.struGroupIdentify[i][j][m].struColorSat.nDispEnable = 0;
                }

                if(m_temCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_HUE] == 0)
                {
                    m_temCnfp.struGroupIdentify[i][j][m].struColorHue.nArithEnable = 0;
                    m_temCnfp.struGroupIdentify[i][j][m].struColorHue.nDispEnable = 0;
                }

                if(m_temCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SCALE] == 0)
                {
                    m_temCnfp.struGroupIdentify[i][j][m].struColorScale.nArithEnable = 0;
                    m_temCnfp.struGroupIdentify[i][j][m].struColorScale.nDispEnable = 0;
                }

                if(m_temCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_MILDEW] == 0)
                {
                    m_temCnfp.struGroupIdentify[i][j][m].struMildew.nArithEnable = 0;
                    m_temCnfp.struGroupIdentify[i][j][m].struMildew.nDispEnable = 0;
                }

                if(m_temCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_OUTLINE] == 0)
                {
                    m_temCnfp.struGroupIdentify[i][j][m].struOutline.nArithEnable = 0;
                    m_temCnfp.struGroupIdentify[i][j][m].struOutline.nDispEnable = 0;
                }

                if(m_temCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_LINE] == 0)
                {
                    m_temCnfp.struGroupIdentify[i][j][m].struLine.nArithEnable = 0;
                    m_temCnfp.struGroupIdentify[i][j][m].struLine.nDispEnable = 0;
                }

                if(m_temCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_VALUE] == 0)
                {
                    m_temCnfp.struGroupIdentify[i][j][m].struColorValue.nArithEnable = 0;
                    m_temCnfp.struGroupIdentify[i][j][m].struColorValue.nDispEnable = 0;
                }

                for(int k = 0;k < MAX_HSV_TOTAL;k++)
                {
                    if(m_temCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_HSV+k] == 0)
                    {
                        m_temCnfp.struGroupIdentify[i][j][m].struHsv[k].nArithEnable = 0;
                        m_temCnfp.struGroupIdentify[i][j][m].struHsv[k].nDispEnable = 0;
                    }
                }

                for(int k = 0;k < MAX_ROUND_TOTAL;k++)
                {
                    if(m_temCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_ROUND+k] == 0)
                    {
                        m_temCnfp.struGroupIdentify[i][j][m].struRound[k].nArithEnable = 0;
                        m_temCnfp.struGroupIdentify[i][j][m].struRound[k].nDispEnable = 0;
                    }
                }
                if(m_temCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SPL_MILDEW] == 0)
                {
                    m_temCnfp.struGroupIdentify[i][j][m].struSplMildew.nArithEnable = 0;
                    m_temCnfp.struGroupIdentify[i][j][m].struSplMildew.nDispEnable = 0;
                }

                if(m_temCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_W_PEANUT] == 0)
                {
                    m_temCnfp.struGroupIdentify[i][j][m].struWhitePeanut.nArithEnable = 0;
                    m_temCnfp.struGroupIdentify[i][j][m].struWhitePeanut.nDispEnable = 0;
                }
                if(m_temCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_PEPPER_HANDLE] == 0)
                {
                    m_temCnfp.struGroupIdentify[i][j][m].struPepperHandle.nArithEnable = 0;
                    m_temCnfp.struGroupIdentify[i][j][m].struPepperHandle.nDispEnable = 0;
                }
                for(int k = 0;k < MAX_SUPPRESS_LARGE_TOTAL;k++)
                {
                    if(m_temCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_SUPPRESS_LARGE_A+k] == 0)
                    {
                        m_temCnfp.struGroupIdentify[i][j][m].struSuppressLarge[k].nArithEnable = 0;
                        m_temCnfp.struGroupIdentify[i][j][m].struSuppressLarge[k].nDispEnable = 0;
                    }
                }
                if(m_temCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[ARITH_WHEAT_SPROUT] == 0)
                {
                    m_temCnfp.struGroupIdentify[i][j][m].struWheatSprout.nArithEnable = 0;
                    m_temCnfp.struGroupIdentify[i][j][m].struWheatSprout.nDispEnable = 0;
                }
                for(int k = 0;k < MAX_TEA_ARITH_TOTAL;k++)
                {
                    if(m_temCnfg.struLayerInfo[i].stuViewInfo[j].nViewArithVisiable[TEA_ARITH_COLOR+k] == 0)
                    {
                        m_temCnfp.struGroupIdentify[i][j][m].struTeaColor[k].nArithEnable = 0;
                        m_temCnfp.struGroupIdentify[i][j][m].struTeaColor[k].nDispEnable = 0;
                    }
                }
            }
            if(m_temCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[i] &&
                    m_temCnfp.nRealTimeCorrection[struGsh.nLayer][i])
                m_temCnfg.nRealTimeCorrection = 1;
        }
    }




    memcpy(&struCnfg,&m_temCnfg,sizeof(struCnfGlobal));
    myFlow.saveGlobal();
    memcpy(&struCnfp,&m_temCnfp,sizeof(struCnfProfile));
    myFlow.saveProfile();
    //memcpy(&m_temCnf,&struCnfg,sizeof(struCnfGlobal));//saveglobal时保存了系统运行的秒数
    myFlow.sendAll();
    returnBack();
}


void CameraSetWidget::on_restoreBtn_clicked()
{
    myFlow.getCameraViewDefaultArithEnable(struGsh.nLayer);

    memcpy(&m_temCnfg,&struCnfg,sizeof(struCnfGlobal));

    updateArithState();
}

int CameraSetWidget::getCurrentFreqIndex()
{

}

void CameraSetWidget::on_SensorRowFreComboBox_activated(int index)
{

}

void CameraSetWidget::on_SensorImageComboBox_activated(int index)
{
    int view = struGsh.nView;
    m_temCnfp.nImageSplice[struGsh.nLayer][view] = index;
}

void CameraSetWidget::on_MachineTypeComboBox_activated(int index)
{
    int view = struGsh.nView;
    m_temCnfp.nSensorDataType[struGsh.nLayer][view] = index;

    //    ui->label_4->setVisible(index == 1);
    //    ui->SensorRowFreComboBox->setVisible(index == 1);
    //    ui->label_9->setVisible(index == 0);
    //    ui->SensorRowFreLineEdit->setVisible(index == 0);
}

void CameraSetWidget::on_MirrorComboBox_activated(int index)
{
    int view = struGsh.nView;
    m_temCnfp.nMirror[struGsh.nLayer][view] = index;
}

void CameraSetWidget::on_BrightSelectComboBox_activated(int index)
{
    int view = struGsh.nView;
    m_temCnfp.nSenseBrightSelection[struGsh.nLayer][view] = index;
}

void CameraSetWidget::on_imgCompresscheckBox_clicked()
{
    m_temCnfp.nIsCameraConpress = (m_temCnfp.nIsCameraConpress == 0) ? 1 : 0;
    updateCameraSet();
}

void CameraSetWidget::on_checkBox_B_clicked(bool checked)
{
    int view = struGsh.nView;
    m_temCnfp.nSensorColorChoose[struGsh.nLayer][view][0] = !checked;
}

void CameraSetWidget::on_checkBox_G_clicked(bool checked)
{
    int view = struGsh.nView;
    m_temCnfp.nSensorColorChoose[struGsh.nLayer][view][1] = !checked;
}

void CameraSetWidget::on_checkBox_R_clicked(bool checked)
{
    int view = struGsh.nView;
    m_temCnfp.nSensorColorChoose[struGsh.nLayer][view][2] = !checked;
}

void CameraSetWidget::on_checkBox_N_clicked(bool checked)
{
    int view = struGsh.nView;
    m_temCnfp.nSensorColorChoose[struGsh.nLayer][view][3] = !checked;
}

void CameraSetWidget::on_colorModeComboBox_activated(int index)
{
    int view = struGsh.nView;
    m_temCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[view].nSensorD2Mode = index;
    m_temCnfp.nSensorD2Mode[struGsh.nLayer][view] = index;

    //    stru_view_info viewInfo;
    //    memcpy(&viewInfo,&m_temCnf.struLayerInfo[struGsh.nLayer].stuViewInfo[view],sizeof(viewInfo));
    //    memset(viewInfo.nSensorColorChoose, 1, sizeof(viewInfo.nSensorColorChoose));
    //    switch (index){
    //    case 1://rgb
    //        viewInfo.nSensorColorChoose[3] = 0;
    //        break;
    //    case 2://rgn
    //        viewInfo.nSensorColorChoose[0] = 0;
    //        break;
    //    case 3://rbn
    //        viewInfo.nSensorColorChoose[1] = 0;
    //        break;
    //    case 4://gbn
    //        viewInfo.nSensorColorChoose[2] = 0;
    //        break;
    //    default:
    //        break;
    //    }
}

void CameraSetWidget::on_SensorCmdSpeedComboBox_activated(int index)
{
    int view = struGsh.nView;
    m_temCnfp.nSensCmdSpeed[struGsh.nLayer][view] = index;

}

void CameraSetWidget::on_ResizeModeComboBox_activated(int index)
{
    int view = struGsh.nView;
    m_temCnfp.nSensResizeMode[struGsh.nLayer][view] = index;
}

void CameraSetWidget::on_correctionCheckBox_clicked(bool checked)
{
    int view = struGsh.nView;
    m_temCnfp.nRealTimeCorrection[struGsh.nLayer][view] = (int)checked;
}

void CameraSetWidget::on_TDscheme_activated(int index)
{
    int view = struGsh.nView;
    qDebug()<<index;
    if(index == m_temCnfp.nTimeDivisionScheme[struGsh.nLayer][view])
        return;
    else
    {
        m_temCnfp.nTimeDivisionScheme[struGsh.nLayer][view] = index;
       // myFlow.setLightOrder();
        int tmp = 0;
        for(int i = 0; i < MAX_LAMP_BOARD_PER_INTERFACE;i++)
        {
            tmp = 0;
            for(int j = 0; j < MAX_LAMP_PER_BOARD;j++){

                if(m_temCnfp.nTimeDivisionScheme[0][struGsh.nView] == 0)  //不分时
                {
                    m_temCnfp.nLampSeq[0][i][j] = 0;
                }
                else //  1 2 1 2
                {
                    m_temCnfp.nLampSeq[0][i][j] = tmp%2 +1;
                }
                tmp++;
            }
        }

    }


}

void CameraSetWidget::on_UserMode_activated(int index)
{
    myMessageBox *mesBox = new myMessageBox(MSG_QUES, g_myLan().ChangeUserMode+"?");
    int ret = mesBox->exec();
    if(ret == QDialog::Accepted)
    {
        for(int i = 0; i < struCnfg.struLayerInfo[0].nViewUnitTotal[struGsh.nView]; i++)
        {
            struCnfp.nCameraBoardUserMode[0][struGsh.nView][i]  = index +1;
            m_temCnfp.nCameraBoardUserMode[0][struGsh.nView][i]  = index +1;
        }

        {
            g_infoWidget().setLabelText(g_myLan().msg_mode_changing);
            g_infoWidget().delayShow();
            myFlow.SwitchUserMode(struGsh.nView+1, 0);
            myFlow.msleep(3000);
            for (int m =0;m<struCnfg.struLayerInfo[0].nViewUnitTotal[struGsh.nView];m++)
            {
                int k;
                for( k = 0; k < 20; k++)
                {
                    myFlow.readVersion(myFlow.getBoardAddr(board_camera,struGsh.nView+1), m+1);
                    myFlow.msleep(500);
                    if(myFlow.boardUpdateSuccess(board_camera,struGsh.nView+1,m,0))
                    {
                        break;
                    }
                }
                if(k == 20)
                {
                    g_infoWidget().setLabelText(g_myLan().ids_setUserModeFailed);
                    g_infoWidget().showSecs();
                    return;
                }
            }
            g_infoWidget().setLabelText(g_myLan().ids_setUserModeSucceeded);
            g_infoWidget().showSecs();
            return;
        }
    }
    else
        return;
}
void CameraSetWidget::on_inhanceCheckBox_clicked(bool checked)
{
    m_temCnfp.imageEnahnce[struGsh.nView] = (int)checked;
}
