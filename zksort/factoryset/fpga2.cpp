#include "fpga.h"

fpga::fpga(QWidget *parent)
    : QDialog(parent)
{
    setGeometry(0, 0, LCD_WIDTH, LCD_HEIGHT-LCD_TITLE_HEIGHT);
    
    margin = 25;
//    config = g_Config::getInstance();

    stackedWidget = new QStackedWidget(this);
    stackedWidget->setGeometry(0,0,width(),height());

    initUpdateParams();
    bTransByUSB = false;

    createMountUdiskPage();
    createFileListPage();
    createNoticePage();
    createBoardUpdatePage();
    createWholeUpdatePage();

    connect(stackedWidget, SIGNAL(currentChanged(int)), this, SLOT(onStackedWidgetIndexChangedSlt(int)));
}

fpga::~fpga()
{
}

/* 初始化下位机升级相关参数 */
void fpga::initUpdateParams()
{
    defIntMode = struCnfg.nInterfaceBoardMode;
    for(int i= 0; i < MAX_VIEW; i++)
    {
        defCameraMode[i] = struCnfg.nCameraBoardMode[0][i];
        cameraUserMode[i] = 0;
    }

    intUserMode = 0;
    ctrlUserMode = 0;
    lampUserMode = 0;

    for(int i = 0; i < MAX_VIEW; i++){
        bCameraFileIsOk[i] = false;
    }
    for(int i = 0; i < MAX_VAVLE; i++){
        bVavleFileIsOk[i] = false;
    }

    bCtrlFileIsOk = false;
    bLampFileIsOk = false;

    intAddr = 0x01;
    for(int i = 0; i < MAX_VIEW; i++){
        cameraAddr[i] = 0;
    }
    for(int i = 0; i < MAX_VAVLE; i++){
        vavleAddr[i] = 0;
    }

    ctrlAddr = 0x00;
    lampAddr = 0x00;

    bIntIsFactory = false;
    for(int i = 0; i < MAX_VIEW; i++){
        bCameraIsFactory[i] = false;
    }
    for(int i = 0; i < MAX_VAVLE; i++){
        bVavleIsFactory[i] = false;
    }
    bCtrlIsFactory = false;
    bLampIsFactory = false;

    curUpdateStep = 0;
    curLevel = 0;
}

/* 创建挂载U盘界面 */
void fpga::createMountUdiskPage()
{
    mountUdiskWidget = new QWidget;
    mountUdiskWidget->setGeometry(0,0,width(),height());

    QHBoxLayout *upLay = new QHBoxLayout;
    QHBoxLayout *midLay = new QHBoxLayout;
    QHBoxLayout *downLay = new QHBoxLayout;
    QVBoxLayout *mainLay = new QVBoxLayout(mountUdiskWidget);

//    mountUdiskLabel = new QLabel(myLan.msg_insert_udisk);
    mountUdiskLabel = new QLabel("请插入U盘/Please insert u disk");
//    mountUdiskLabel->setFont(config->getFont(DEFAULT_FONT_SIZE));
    mountUdiskLabel->setFixedHeight(LCD_HEIGHT/3);
    mountUdiskLabel->setAlignment(Qt::AlignCenter);
    warningLabel = new QLabel;
    pe.setColor(QPalette::WindowText, Qt::red);
    warningLabel->setPalette(pe);
//    warningLabel->setFont(config->getFont(DEFAULT_FONT_SIZE));
    warningLabel->setFixedHeight(LCD_HEIGHT/3);
    warningLabel->setAlignment(Qt::AlignCenter);

    QSize btnSize(BTN_WIDTH, BTN_HEIGHT);
//    mountUdiskBackBtn = new myPushButton(myLan.upgrade_return, ":/white/icon_back.png",QRect());
    mountUdiskBackBtn = new myPushButton("返回/Back", ":/white/icon_back.png",QRect());
    mountUdiskBackBtn->setFixedSize(btnSize);
//    mountUdiskGoBtn = new myPushButton(myLan.next,":/white/icon_right.png",QRect());
    mountUdiskGoBtn = new myPushButton("下一步/Next","",QRect());
    mountUdiskGoBtn->setFixedSize(btnSize);

    upLay->addStretch();
    upLay->addWidget(mountUdiskLabel);
    upLay->addStretch();
    upLay->setContentsMargins(0, 0, 0, 0);
    midLay->addStretch();
    midLay->addWidget(warningLabel);
    midLay->addStretch();
    midLay->setContentsMargins(0, 0, 0, 0);
    downLay->addWidget(mountUdiskBackBtn);
    downLay->addStretch();
    downLay->addWidget(mountUdiskGoBtn);
    downLay->setContentsMargins(0, 0, 0, 0);
    mainLay->addLayout(upLay);
    mainLay->addLayout(midLay);
    mainLay->addStretch();
    mainLay->addLayout(downLay);

    connect(mountUdiskGoBtn,SIGNAL(pressed()),this,SLOT(onMountUdiskGoBtnClicked()));
    connect(mountUdiskBackBtn,SIGNAL(pressed()),this,SLOT(onMountUdiskBackBtnClicked()));

    stackedWidget->addWidget(mountUdiskWidget);
}

/* 创建文件列表界面 */
void fpga::createFileListPage()
{
    fileListWidget = new QWidget;
    fileListWidget->setGeometry(0, 0, width(), height());

    QHBoxLayout *upLay = new QHBoxLayout;
    QHBoxLayout *downLay = new QHBoxLayout;
    QVBoxLayout *mainLay = new QVBoxLayout(fileListWidget);

    fileList = new QListWidget;
    fileList->setSelectionMode(QAbstractItemView::NoSelection);
    fileList->setMinimumHeight(LCD_HEIGHT/2+50);
    for(int i=0; i<MAX_FILE_NUM; i++){
        fileItem[i] = new QListWidgetItem;
//        fileItem[i]->setFont(config->getFont(DEFAULT_FONT_SIZE));
        fileItem[i]->setSizeHint(QSize(200, 50));
        fileList->addItem(fileItem[i]);
    }

    QSize btnSize(BTN_WIDTH, BTN_HEIGHT);
    lastBtn = new myPushButton("上一步/Last", "", QRect());
    lastBtn->setFixedSize(btnSize);
    nextBtn = new myPushButton("下一步/Next", "", QRect());
    nextBtn->setFixedSize(btnSize);
    transModeBtn = new myPushButton("", "", QRect());
    transModeBtn->setFixedSize(btnSize);

    upLay->addWidget(fileList);
    upLay->setContentsMargins(LCD_WIDTH/5, LCD_HEIGHT/10, LCD_WIDTH/5, 20);
    downLay->addWidget(lastBtn);
    downLay->addStretch();
    downLay->addWidget(transModeBtn);
    downLay->addStretch();
    downLay->addWidget(nextBtn);
    mainLay->addLayout(upLay);
    mainLay->addLayout(downLay);

    connect(fileList, SIGNAL(currentRowChanged(int)), this, SLOT(onFileListChangedSlt(int)));
    connect(lastBtn, SIGNAL(pressed()), this, SLOT(onFileListLastBtnClicked()));
    connect(nextBtn, SIGNAL(pressed()), this, SLOT(onFileListNextBtnClicked()));
    connect(transModeBtn, SIGNAL(pressed()), this, SLOT(onTransModeBtnClicked()));

    stackedWidget->addWidget(fileListWidget);
}

/* 创建提示界面 */
void fpga::createNoticePage()
{
    noticeWidget = new QWidget;
    noticeWidget->setGeometry(0, 0, width(), height());

    QHBoxLayout *downLay = new QHBoxLayout;
    QVBoxLayout *mainLay = new QVBoxLayout(noticeWidget);

    noticeLabel = new myLabel(myLan.msg_insert_data_wire, QRect());
    noticeLabel->setAlignment(Qt::AlignCenter);
    noticeLabel->setFixedHeight(100);

    QSize btnSize(BTN_WIDTH, BTN_HEIGHT);
    noticeLastBtn = new myPushButton(myLan.last, ":/white/icon_left.png", QRect());
    noticeLastBtn->setFixedSize(btnSize);
    noticeNextBtn = new myPushButton(myLan.next, ":/white/icon_right.png", QRect());
    noticeNextBtn->setFixedSize(btnSize);

    downLay->addWidget(noticeLastBtn);
    downLay->addStretch();
    downLay->addWidget(noticeNextBtn);
    mainLay->addStretch();
    mainLay->addWidget(noticeLabel);
    mainLay->addStretch();
    mainLay->addLayout(downLay);

    connect(noticeLastBtn, SIGNAL(pressed()), this, SLOT(onNoticeLastBtnClicked()));
    connect(noticeNextBtn, SIGNAL(pressed()), this, SLOT(onNoticeNextBtnClicked()));

    stackedWidget->addWidget(noticeWidget);
}

/* 创建板级升级界面 */
void fpga::createBoardUpdatePage()
{
    boardUpdateWidget = new QWidget;
    boardUpdateWidget->setGeometry(0,0,width(),height());

    QVBoxLayout *upLeftLay = new QVBoxLayout;
    QVBoxLayout *upRightLay = new QVBoxLayout;
    QHBoxLayout *upLay = new QHBoxLayout;
    QHBoxLayout *downLay = new QHBoxLayout;
    QVBoxLayout *mainLay = new QVBoxLayout(boardUpdateWidget);

    layerGroup = new myMutex(Qt::Vertical, 0);
    layerGroup->setMaximumWidth(150);
    layerMaizeAddGroup = new myMutex(Qt::Vertical, 0);
    layerMaizeAddGroup->setMaximumWidth(150);
/*
    if (struCnfe.nProduct == OPT_PRODUCT_ANCOO_MAIZE) {
        layerMaizeAddGroup->setNum(2);
        layerMaizeAddGroup->setLabelText(myLan.lower_view,myLan.upper_view);
        layerMaizeAddGroup->setCurrentIndex(0);
        layerMaizeAddGroup->show();
    } else if (struCnfe.nMachine == MACHINE_LD4) {
        layerMaizeAddGroup->setNum(2);
        layerMaizeAddGroup->setLabelText(myLan.master_view,myLan.slave_view);
        layerMaizeAddGroup->setCurrentIndex(0);
        layerMaizeAddGroup->show();
    } else if (struCnfe.nMachine == MACHINE_LDS2) {
        layerMaizeAddGroup->setNum(2);
        layerMaizeAddGroup->setLabelText(myLan.front_view,myLan.rear_view);
        layerMaizeAddGroup->setCurrentIndex(0);
        layerMaizeAddGroup->show();
    } else {
        layerMaizeAddGroup->hide();
    }
    */
    layerMaizeAddGroup->hide();
    userModeGroup = new myMutex(Qt::Horizontal);
    userModeGroup->setTitle("user_mode");
    userModeGroup->setLabelText("mode_1", "mode_2", "mode_3");
//    userModeGroup->setFixedHeight(2*config->getBtnSize(SMALL_BTN_SIZE).height());
    userModeGroup->setFixedHeight(2*BTN_HEIGHT);

//    QSize btnSize = config->getBtnSize(SMALL_BTN_SIZE);
    QSize btnSize = QSize(BTN_WIDTH, BTN_HEIGHT);
    boardNumGroup = new QGroupBox("board_num");
//    boardNumGroup->setFont(config->getFont());
    boardNumGroup->setMaximumWidth(150);
    QVBoxLayout *layout = new QVBoxLayout(boardNumGroup);

    boardNumPlusBtn = new myPushButton(":/white/icon_plus.png", btnSize);
    boardUpdateLcdNum = new QLCDNumber(1);
    boardUpdateLcdNum->display(1);
    boardUpdateLcdNum->setFixedSize(btnSize);
    boardNumMinusBtn = new myPushButton(":/white/icon_minus.png", btnSize);

    layout->addWidget(boardNumPlusBtn,0,Qt::AlignHCenter);
    layout->addWidget(boardUpdateLcdNum,0,Qt::AlignHCenter);
    layout->addWidget(boardNumMinusBtn,0,Qt::AlignHCenter);

    messagesGroup = new QGroupBox;
    messagesGroup->setFixedHeight(LCD_HEIGHT-LCD_TITLE_HEIGHT-LCD_STATUS_HEIGHT-180);
    QGridLayout *messagesLayout = new QGridLayout(messagesGroup);
    for(int i=0; i<MES_NUM; i++){
        updateMesLabels[i] = new QLabel;
//        updateMesLabels[i]->setFont(config->getFont(DEFAULT_FONT_SIZE));
        updateMesLabels[i]->setAlignment(Qt::AlignLeft);
        updateMesLabels[i]->setMinimumHeight(30);
        messagesLayout->addWidget(updateMesLabels[i], i, 0, 1, 2, Qt::AlignCenter);
        updateStatusLabels[i] = new QLabel;
//        updateStatusLabels[i]->setFont(config->getFont(DEFAULT_FONT_SIZE));
        updateStatusLabels[i]->setAlignment(Qt::AlignCenter);
        messagesLayout->addWidget(updateStatusLabels[i], i, 2, Qt::AlignCenter);
    }

//    btnSize = config->getBtnSize(DEFAULT_BTN_SIZE);
    boardUpdateBackBtn = new myPushButton("back", ":/white/icon_back.png", QRect());
    boardUpdateBackBtn->setFixedSize(btnSize);
    boardUpdateGoBtn = new myPushButton("upgrade",":/white/icon_up.png", QRect());
    boardUpdateGoBtn->setFixedSize(btnSize);
    boardUpdateRetryBtn = new myPushButton("upgrade_retry", ":/white/icon_adjust.png", QRect());
    boardUpdateRetryBtn->setFixedSize(btnSize);
    boardUpdateWholeBtn = new myPushButton("upgrade_whole", "", QRect());
    boardUpdateWholeBtn->setFixedSize(btnSize);

    upLeftLay->addWidget(layerGroup);
    upLeftLay->addSpacing(10);
    upLeftLay->addWidget(layerMaizeAddGroup);
    upLeftLay->addSpacing(10);
    upLeftLay->addWidget(boardNumGroup);
    upLeftLay->setContentsMargins(0, 0, 0, 0);
    upRightLay->addWidget(userModeGroup);
    upRightLay->addSpacing(30);
    upRightLay->addWidget(messagesGroup);
    upRightLay->setContentsMargins(0, 0, 10, 0);
    upLay->addLayout(upLeftLay);
    upLay->addSpacing(30);
    upLay->addLayout(upRightLay);
    upLay->setContentsMargins(0 ,0, 0, 0);
    downLay->addWidget(boardUpdateWholeBtn);
    downLay->addWidget(boardUpdateGoBtn);
    downLay->addStretch();
    downLay->addWidget(boardUpdateRetryBtn);
    downLay->addStretch();
    downLay->addWidget(boardUpdateBackBtn);
    downLay->setContentsMargins(0, 0, 0, 0);
    mainLay->addLayout(upLay);
    mainLay->addLayout(downLay);

    connect(boardUpdateBackBtn,SIGNAL(pressed()),this,SLOT(onBoardUpdateBackBtnClicked()));
    connect(boardUpdateGoBtn,SIGNAL(pressed()),this,SLOT(onBoardUpdateGoBtnClicked()));
    connect(boardUpdateRetryBtn,SIGNAL(pressed()),this,SLOT(onBoardUpdateRetryBtnClicked()));
    connect(boardUpdateWholeBtn,SIGNAL(pressed()),this,SLOT(onBoardUpdateWholeBtnClicked()));
    connect(boardNumPlusBtn,SIGNAL(pressed()),this,SLOT(onBoardNumPlusBtnClicked()));
    connect(boardNumMinusBtn,SIGNAL(pressed()),this,SLOT(onBoardNumMinusBtnClicked()));
    connect(userModeGroup, SIGNAL(indexChanged(int)), this, SLOT(onUserModeChangedSlt(int)));
    connect(layerGroup, SIGNAL(indexChanged(int)), this, SLOT(onLayerChangedSlt(int)));
    connect(layerMaizeAddGroup, SIGNAL(indexChanged(int)), this, SLOT(onlayerAddChangedSlt(int)));

    stackedWidget->addWidget(boardUpdateWidget);
}

/* 创建整机升级界面 */
void fpga::createWholeUpdatePage()
{
    updateWholeWidget = new QWidget;
    updateWholeWidget->setGeometry(0,0,width(),height());

    QHBoxLayout *upLay = new QHBoxLayout;
    QHBoxLayout *downLay = new QHBoxLayout;
    QVBoxLayout *mainLay = new QVBoxLayout(updateWholeWidget);

    QGroupBox *group = new QGroupBox;
    group->setFixedHeight(LCD_HEIGHT-LCD_TITLE_HEIGHT-LCD_STATUS_HEIGHT-80);
    QGridLayout *layout = new QGridLayout(group);

    for(int i=0; i<MAX_VIEW; i++){
        for(int j=0; j<MAX_VIEW_UNIT; j++){
            boardNumLabels[i][j] = new QLabel;
//            boardNumLabels[i][j]->setFont(config->getFont(DEFAULT_FONT_SIZE));
            boardNumLabels[i][j]->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
            layout->addWidget(boardNumLabels[i][j], j, 2*i, 1, 1);
            statusLabels[i][j] = new QLabel;
//            statusLabels[i][j]->setFont(config->getFont(DEFAULT_FONT_SIZE));
            statusLabels[i][j]->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
            layout->addWidget(statusLabels[i][j], j, 2*i+1, 1, 1);
        }
    }

//    QSize btnSize = config->getBtnSize(DEFAULT_BTN_SIZE);
    QSize btnSize = QSize(BTN_WIDTH, BTN_HEIGHT);
    updateWholeReturnBtn = new myPushButton("upgrade_return", "", QRect());
    updateWholeReturnBtn->setFixedSize(btnSize);
    updateWholeBackBtn = new myPushButton("back", ":/white/icon_back.png", QRect());
    updateWholeBackBtn->setFixedSize(btnSize);

    upLay->addWidget(group);
    upLay->setContentsMargins(0, 0, 0, 0);
    downLay->addWidget(updateWholeReturnBtn);
    downLay->addStretch();
    downLay->addWidget(updateWholeBackBtn);
    downLay->setContentsMargins(0, 0, 0, 0);
    mainLay->addLayout(upLay);
    mainLay->addLayout(downLay);
    mainLay->setContentsMargins(10, 10, 20, 10);

    connect(updateWholeBackBtn,SIGNAL(pressed()),this,SLOT(onWholeUpdateBackBtnClicked()));
    connect(updateWholeReturnBtn,SIGNAL(pressed()),this,SLOT(onWholeUpdateReturnBtnClicked()));

    stackedWidget->addWidget(updateWholeWidget);
}

/* 根据当前升级板的不同设置显示内容 */
void fpga::setUpdateDisplay()
{
    int num;
    pe.setColor(QPalette::WindowText, Qt::blue);
    for(int i=0; i<MES_NUM; i++){
        updateMesLabels[i]->show();
        updateStatusLabels[i]->show();
        updateStatusLabels[i]->setPalette(pe);
    }
    userModeGroup->setNum(3);
//    if(struCnfe.nMachine==MACHINE_TD && struCnfg.nLevelTotal==3)
//        layerGroup->setNum(3);
//    else
        layerGroup->setNum(2);

    switch(curBoardMode){
//    case INT_BOARD:
////    case INT_CTRL_BOARD:
//        layerMaizeAddGroup->hide();
//        boardNumGroup->hide();
//        boardUpdateWholeBtn->hide();
//        if(struCnfe.nMachine==MACHINE_CF
//            || struCnfe.nMachine==MACHINE_LD2
//            || struCnfe.nMachine==MACHINE_T2)
//            layerGroup->hide();
//        else if(struCnfe.nMachine==MACHINE_LDS
//                || (struCnfe.nMachine==MACHINE_TD && struCnfg.nLevelTotal==2)){
//            layerGroup->show();
//            layerGroup->setNum(2);
//            layerGroup->setLabelText(myLan.upper_layer, myLan.lower_layer);
//            layerGroup->setTitle(myLan.layer);
//        } else if(struCnfe.nMachine==MACHINE_TD && struCnfg.nLevelTotal==3){
//            layerGroup->show();
//            layerGroup->setNum(3);
//            layerGroup->setLabelText(myLan.upper_layer, myLan.mid_layer, myLan.lower_layer);
//            layerGroup->setTitle(myLan.layer);
//        } else if (struCnfe.nMachine==MACHINE_LD4) {
//            layerGroup->hide();
//        } else if (struCnfe.nMachine==MACHINE_LDS2) {
//            layerGroup->show();
//            layerGroup->setNum(2);
//            layerGroup->setLabelText(myLan.upper_layer, myLan.lower_layer);
//            layerGroup->setTitle(myLan.layer);
//        }
//        layerGroup->hide();
//        num = 4;
//        updateMesLabels[0]->setText(QString("1.%1").arg(myLan.msg_int_to_factory));
//        updateMesLabels[1]->setText(QString("2.%1").arg(myLan.msg_int_download_file));
//        updateMesLabels[2]->setText(QString("3.%1").arg(myLan.msg_int_burn_file));
//        updateMesLabels[3]->setText(QString("4.%1").arg(myLan.msg_int_to_user));

//        for(int i=0; i<num; i++){
//            updateStatusLabels[i]->setText(myLan.upgrade_untreated);
//        }
//        for(int i=num; i<MES_NUM; i++){
//            updateStatusLabels[i]->hide();
//            updateMesLabels[i]->hide();
//        }

//        switch(intUserMode){
//        case 0:
//            userModeGroup->clearBtnsIcon();
//            break;
//        case 1:
//            userModeGroup->setCurrentIndex(0);
//            break;
//        case 2:
//            userModeGroup->setCurrentIndex(1);
//            break;
//        case 4:
//            userModeGroup->setCurrentIndex(2);
//        default:
//            break;
//        }
//        break;

    case COLOR_BOARD:      
        layerGroup->show();
        layerMaizeAddGroup->hide();

        layerGroup->setNum(2);
        layerGroup->setLabelText("front_view", "rear_view");
        layerGroup->setCurrentIndex(0);

        boardNumGroup->show();
        boardUpdateWholeBtn->show();

        num = 5;
        updateMesLabels[0]->setText(QString("1.%1").arg("msg_int_to_factory"));
        updateMesLabels[1]->setText(QString("2.%1").arg("msg_color_download_file"));
        updateMesLabels[2]->setText(QString("3.%1").arg("msg_color_to_factory"));
        updateMesLabels[3]->setText(QString("4.%1").arg("msg_color_burn_file"));
        updateMesLabels[4]->setText(QString("5.%1").arg("msg_color_to_user"));
        for(int i=0; i<num; i++){
            updateStatusLabels[i]->setText("upgrade_untreated");
        }
        for(int i=num; i<MES_NUM; i++){
            updateStatusLabels[i]->hide();
            updateMesLabels[i]->hide();
        }
        switch(cameraUserMode[curView]){
        case 0:
            userModeGroup->clearBtnsIcon();
            break;
        case 1:
            userModeGroup->setCurrentIndex(0);
            break;
        case 2:
            userModeGroup->setCurrentIndex(1);
            break;
        case 4:
            userModeGroup->setCurrentIndex(2);
        default:
            break;
        }
        break;
    case CTRL_BOARD:
        userModeGroup->setIndexEnabled(1, false);
        userModeGroup->setIndexEnabled(2, false);
        layerGroup->hide();
        boardNumGroup->hide();
        boardUpdateWholeBtn->hide();
        layerMaizeAddGroup->hide();

        num = 4;
        updateMesLabels[0]->setText("msg_ctrl_to_factory");
        updateMesLabels[1]->setText("msg_ctrl_erase");
        updateMesLabels[2]->setText("msg_ctrl_burn_file");
        updateMesLabels[3]->setText("msg_ctrl_to_user");

        for(int i=0; i<num; i++){
            updateStatusLabels[i]->setText("upgrade_untreated");
        }
        for(int i=num; i<MES_NUM; i++){
            updateMesLabels[i]->hide();
            updateStatusLabels[i]->hide();
        }
        if(ctrlUserMode == 1)
            userModeGroup->setCurrentIndex(0);
        else
            userModeGroup->clearBtnsIcon();
        break;

    default:
        break;
    }

    boardUpdateRetryBtn->setEnabled(false);
}

/* 根据当前状态设置整机升级界面的显示 */
void fpga::setUpdateWholeDisplay()
{
    int chutePerViewLocation = 0;//通道数

    chutePerViewLocation = struCnfg.nChuteTotal;

    for(int i=0; i<MAX_VIEW; i++){
        if(curBoardMode == COLOR_BOARD){
            boardNumLabels[0][i]->setText(QString("%1 %2 %3").arg("front_view").arg(i+1).arg("upgrade"));
            boardNumLabels[1][i]->setText(QString("%1 %2 %3").arg("rear_view").arg(i+1).arg("upgrade"));
            for (int j = 2; j < MAX_VIEW_UNIT; j++) {
                boardNumLabels[j][i]->show();
                statusLabels[j][i]->show();
                setWholeStatusShow(j, i, UPDATE_NO_START);
            }
            for (int j = 2; j < MAX_VIEW_UNIT; j++) {
                boardNumLabels[j][i]->hide();
                statusLabels[j][i]->hide();
            }
        }
    }
    for(int i=0; i<MAX_VIEW; i++){
        for(int j=chutePerViewLocation; j<MAX_VIEW_UNIT; j++){
            boardNumLabels[i][j]->hide();
            statusLabels[i][j]->hide();
        }
    }
}

/* 切换下位机至工厂模式 */
bool fpga::stepSwitchToFactory(boardMode module)
{
    int ret;
    int loopTimes = 10;

    switch(module){
    case INT_BOARD:
//        MySerial.fpgaUpdateWrite(COM_INT_UPDATE_FACTORY, intAddr, 0, 0, 0, 0, 0, 0, 3);
        break;
    case COLOR_BOARD:
 //       MySerial.fpgaUpdateWrite(COM_UNIT_UPDATE_FACTORY, 2, colorAddr, 0, 0, 0, 1, 0, 3);
        break;
    case CTRL_BOARD:
        break;
    default:
        break;
    }

    myFlow.sleep(2);

    for(int i=0; i<loopTimes; i++){
        switch(module){
        case INT_BOARD:
        case COLOR_BOARD:
//            MySerial.fpgaUpdateWrite(COM_UNIT_UPDATE_STATUS, 2, colorAddr, 0, 0, 0, 1, 0, 1);
            break;
        case CTRL_BOARD:
            break;
        default:
            break;
        }
	
        myFlow.msleep(3);
//        ret = MySerial.fpgaUpdateRead(packetBuf, PACKET_SIZE);

        if(packetBuf[0]==0xAA && packetBuf[1]==0xAA && packetBuf[2]==0xAA
                && packetBuf[3]==0xAA && (packetBuf[15]&0x04)==0x04){
            return true;
        }


        myFlow.sleep(1);
    }

    return false;
}

/* 向下位机发送升级文件 */
bool fpga::stepSendFile()
{
    int ret;
    int writeLen;
    int curIsOK = 0;
    int loopTimes = 3;
    int per;

//    if(curBoardMode==INF_BOARD || curBoardMode==INT_CTRL_BOARD) {
//        writeLen = PACKET_COUNT_SMALL;
//    } else {
//        writeLen = PACKET_COUNT;
//    }
    writeLen = PACKET_COUNT;
    /* 切换接口板至串口模式 */
//    MySerial.com1Write(CMD_INT_IMAGE_CAPTURE_MODE, intAddr, 0, 0, 0, 0, 0, 0, 0, 3);

    qDebug("Step send file");
    for(int i=0; i<writeLen; i++){
        if(stackedWidget->currentIndex() == UPDATE_PAGE){
            per = (100*i)/writeLen;
            QString str = QString("%1 %2%").arg("upgrade_doing").arg(per);
            updateStatusLabels[1]->setText(str);
        }

        for(int j=0; j<loopTimes; j++){
//            MySerial.fpgaUpdateWrite(COM_SEND_FILE, intAddr, 0, 0, 0, 0, i/256, i%256, 1, fileBuf);
            myFlow.msleep(30);

//            MySerial.fpgaUpdateWrite(COM_INT_UPDATE_STATUS, intAddr, 0, 0, 0, 0, 0, 0, 1);
            myFlow.msleep(3);
 //           ret = MySerial.fpgaUpdateRead(packetBuf, PACKET_SIZE);
            if(ret < PACKET_SIZE)
                continue;

            if(packetBuf[0]==0xAA && packetBuf[1]==0xAA && packetBuf[2]==0xAA
                    && packetBuf[3]==0xAA && (packetBuf[15]&0x10)==0x10){
                curIsOK = 1;
                break;
            }
        }//end for

        if(curIsOK)
            curIsOK = 0;
        else
            return false;
    }

    return true;
}

/* 通过高速串口发送升级文件 */
bool fpga::stepSendFileByHighUART()
{
//    int ret;
//    int writeLen;
//    int curIsOK = 0;
//    int loopTimes = 3;
//    int per;

//    /* 根据当前升级文件的类型确定升级包的个数 */
//    if(curBoardMode==INF_BOARD || curBoardMode==INT_CTRL_BOARD) {
//        writeLen = PACKET_COUNT_SMALL_MAIZE;
//    } else {
//        writeLen = PACKET_COUNT_MAIZE;
//    }

//    /* 将升级文件拷贝至文件缓冲区中 */
//    uchar *sFileBuf = new uchar[PACKET_COUNT_MAIZE*FILE_PACKET_SIZE_MAIZE];
//    for(int i=0; i<PACKET_COUNT; i++) {
//        for(int j=0; j<FILE_PACKET_SIZE; j++) {
//            sFileBuf[i*FILE_PACKET_SIZE+j] = fileBuf[i][j];
//        }
//    }

//    /* 切换接口板至串口模式 */
////    MySerial.com1Write(CMD_INT_IMAGE_CAPTURE_MODE, intAddr, 0, 0, 0, 0, 0, 0, 0, 3);

//    for(int i=0; i<writeLen; i++){
//        if(stackedWidget->currentIndex() == UPDATE_PAGE){
//            per = (100*i)/writeLen;
//            QString str = QString("%1 %2%").arg(myLan.upgrade_doing).arg(per);
//            updateStatusLabels[1]->setText(str);
//        }

//        for(int j=0; j<loopTimes; j++){
////            MySerial.fpgaUpdateWriteMaize(COM_SEND_FILE, intAddr, 0, (i*32)/256, (i*32)%256, sFileBuf);
//            myFlow.msleep(100);
////            MySerial.fpgaUpdateWrite(COM_INT_UPDATE_STATUS, intAddr, 0, 0, 0, 0, 0, 0, 1);
//            myFlow.msleep(3);
////            ret = MySerial.fpgaUpdateRead(packetBuf, PACKET_SIZE);
//            if(ret < PACKET_SIZE)
//                continue;

//            if(packetBuf[0]==0xAA && packetBuf[1]==0xAA && packetBuf[2]==0xAA
//                    && packetBuf[3]==0xAA && (packetBuf[15]&0x10)==0x10){
//                curIsOK = 1;
//                break;
//            }
//        }//end for

//        if(curIsOK) {
//            curIsOK = 0;
//        } else {
//            delete []sFileBuf;
//            return false;
//        }
//    }

//    delete []sFileBuf;
    return true;
}

/* 通过USB向下位机发送升级文件 */
bool fpga::stepSendFileByUSB()
{
//#ifdef Q_OS_UNIX
#if 0
    int ret;
    int writeLen;
    int loopTimes = 10;

    /* 红外相机板及接口控制板升级文件大小为512K,其余板子为2M */
    if(curBoardMode==INF_BOARD || curBoardMode==INT_CTRL_BOARD) {
        writeLen = PACKET_COUNT_SMALL;
    } else {
        writeLen = PACKET_COUNT;
    }

    if(myUsbDev != NULL){
        usb.myusbFree();
    }
    usb.myusbInit();
    if(myUsbDev == NULL){
        qDebug("usb init error");
        return false;
    }

    /* 考虑到USB初始化向下位机发送2M数据时，会漏发前64个字节的数据，因此USB初始化结束后向下位机写128个字节的废数据 */
    char buf[128] = {0};
    ret = usb_bulk_write(myUsbDev, USB_EP_OUT_AI, buf, 128, 1000);
    qDebug("MSG : write_ret1=%d", ret);

    /* 切换接口板至USB模式 */
//    MySerial.com1Write(CMD_INT_IMAGE_CAPTURE_MODE, intAddr, 0, 0, 0, 0, 0, 0, 1, 3);
    USBSendFile(COM_SEND_FILE, intAddr, writeLen);

    for(int j=0; j<loopTimes; j++){
 //       MySerial.fpgaUpdateWrite(COM_INT_UPDATE_STATUS, intAddr, 0, 0, 0, 0, 0, 0, 1);
        myFlow.msleep(3);
 //       ret = MySerial.fpgaUpdateRead(packetBuf, PACKET_SIZE);

        if(ret < PACKET_SIZE){
            myFlow.sleep(1);
            continue;
        }

        if(packetBuf[0]==0xAA && packetBuf[1]==0xAA && packetBuf[2]==0xAA
                && packetBuf[3]==0xAA && (packetBuf[15]&0x40)==0x40){
            return true;
        }
        myFlow.sleep(1);
    }//end for

#endif

    /* 发送文件失败后复位下位机固件 */
//    MySerial.com1Write(CMD_INT_IMAGE_ERROR_REVISE, intAddr, 0, 0, 0, 0, 0, 0, 1, 3);
    myFlow.sleep(1);

    return false;
}

/* 下位机烧写升级文件 */
bool fpga::stepBurnFile()
{
    int ret;
    int writeLen;
    int loopTimes = 10;

    /* 红外相机板及接口控制板升级文件大小为512K,其余板子为2M */
        writeLen = PACKET_COUNT-1;


//    MySerial.fpgaUpdateWrite(COM_INT_FILE_LEN, intAddr, 0, 0, 0, 0, writeLen/256, writeLen%256, 3);

    myFlow.sleep(1);

    switch(curBoardMode){
    case INT_BOARD:
//    case INT_CTRL_BOARD:
//        MySerial.fpgaUpdateWrite(COM_INT_UPDATE_BURN, intAddr, 0, 0, 0, 0, 0, intUserMode, 3);
        break;
    case COLOR_BOARD:
//        MySerial.fpgaUpdateWrite(COM_UNIT_UPDATE_BURN, 2, colorAddr, 0, 0, 0, 1, cameraUserMode, 3);
        break;
//    case INF_BOARD:
//        MySerial.fpgaUpdateWrite(COM_UNIT_UPDATE_BURN, 2, infAddr, 0, 0, 0, 2, infUserMode, 3);
        break;
    case CTRL_BOARD:
        break;
    default:
        break;
    }

    myFlow.sleep(15);

    for(int i=0; i<loopTimes; i++){
        switch(curBoardMode){
        case INT_BOARD:
//            MySerial.fpgaUpdateWrite(COM_INT_UPDATE_STATUS, intAddr, 0, 0, 0, 0, 0, 0, 3);
            break;
        case COLOR_BOARD:
 //           MySerial.fpgaUpdateWrite(COM_UNIT_UPDATE_STATUS, 2, colorAddr, 0, 0, 0, 1, 0, 3);
            break;
        case CTRL_BOARD:
            break;
        default:
            break;
        }

        myFlow.msleep(3);
//        ret = MySerial.fpgaUpdateRead(packetBuf, PACKET_SIZE);

        if(packetBuf[0]==0xAA && packetBuf[1]==0xAA && packetBuf[2]==0xAA
                && packetBuf[3]==0xAA && (packetBuf[15]&0x01)==0x01){
            return true;
        }

        myFlow.sleep(1);
    }

    return false;
}

/* 切换下位机至用户模式 */
bool fpga::stepSwitchToUser(boardMode module)
{
    int ret;
    int loopTimes = 10;

    switch(module){
    case INT_BOARD:
        if(intUserMode == 0) {
 //           MySerial.fpgaUpdateWrite(COM_INT_UPDATE_USER, intAddr, 0, 0, 0, 0, 0, defIntMode, 3);
        } else{
 //           MySerial.fpgaUpdateWrite(COM_INT_UPDATE_USER, intAddr, 0, 0, 0, 0, 0, intUserMode, 3);
        }
        break;
    case COLOR_BOARD:
        if(cameraUserMode == 0) {
 //           MySerial.fpgaUpdateWrite(COM_UNIT_UPDATE_USER, 2, colorAddr, 0, 0, 0, 1, defColorMode, 3);
        } else{
 //           MySerial.fpgaUpdateWrite(COM_UNIT_UPDATE_USER, 2, colorAddr, 0, 0, 0, 1, cameraUserMode, 3);
        }
        break;
    case CTRL_BOARD:
        break;
    default:
        break;
    }

    myFlow.sleep(2);

    for(int i=0; i<loopTimes; i++){
        switch(module){
        case INT_BOARD:
 //           MySerial.fpgaUpdateWrite(COM_INT_UPDATE_STATUS, intAddr, 0, 0, 0, 0, 0, 0, 3);
            break;
        case COLOR_BOARD:
//            MySerial.fpgaUpdateWrite(COM_UNIT_UPDATE_STATUS, 2, colorAddr, 0, 0, 0, 1, 0, 3);
            break;
        case CTRL_BOARD:
            break;
        default:
            break;
        }

        myFlow.msleep(3);
//        ret = MySerial.fpgaUpdateRead(packetBuf, PACKET_SIZE);

#if 0
        qDebug("ret = %d", ret);
        printf("com << ");
        for(int i=0; i<PACKET_SIZE; i++){
            printf("%02x ", packetBuf[i]);
        }
	printf("\n");
#endif

        if (packetBuf[0]==0xAA && packetBuf[1]==0xAA && packetBuf[2]==0xAA
                && packetBuf[3]==0xAA  && (packetBuf[15]&0x02)== 0x02){
            return true;
        }

        myFlow.sleep(1);
    }

    return false;
}

/* 切换控制板至工厂模式 */
bool fpga::ctrlSwitchToFactory()
{
    int ret;
    int loopTimes = 10;

//    MySerial.fpgaUpdateWrite(COM_CTRL_UPDATE_FACTORY, ctrlAddr, 0, 0, 0, 0, 0, 0, 1);
    myFlow.sleep(2);

    for(int i=0; i<loopTimes; i++){
 //       MySerial.fpgaUpdateWrite(COM_CTRL_UPDATE_STATUS, ctrlAddr, 0, 0, 0, 0, 0, 0, 1);
        myFlow.msleep(3);

//        ret = MySerial.fpgaUpdateRead(packetBuf, PACKET_SIZE, true);
        if(ret < PACKET_SIZE){
            myFlow.sleep(1);
            continue;
        }
	
        /*
        qDebug("ret = %d", ret);
        for(int i=0; i<PACKET_SIZE; i++){
            printf("%02x ", packetBuf[i]);
        }
        printf("\n");
        */

        if (packetBuf[0]==0xAA && packetBuf[1]==0xAA && packetBuf[2]==0xAA
                && packetBuf[3]==0xAA  && (packetBuf[15]&0x04)== 0x04){
                return true;
        }

        myFlow.sleep(1);
    }

    return false;
}

/* 擦除控制板的用户空间 */
bool fpga::ctrlEraseFlash()
{
    int ret;
    int loopTimes = 10;

//    MySerial.fpgaUpdateWrite(COM_CTRL_UPDATE_ERASE, ctrlAddr, 0, 0, 0, 0, 0, ctrlUserMode, 1);
    myFlow.sleep(2);

    for(int i=0; i<loopTimes; i++){
//        MySerial.fpgaUpdateWrite(COM_CTRL_UPDATE_STATUS, ctrlAddr, 0, 0, 0, 0, 0, 0, 1);
        myFlow.msleep(3);
//        ret = MySerial.fpgaUpdateRead(packetBuf, PACKET_SIZE, true);
        if(ret < PACKET_SIZE){
            myFlow.sleep(1);
            continue;
        }

        /*
        qDebug("ret = %d", ret);
        for(int i=0; i<PACKET_SIZE; i++){
            printf("%02x ", packetBuf[i]);
        }
        printf("\n");
        */

        if (packetBuf[0]==0xAA && packetBuf[1]==0xAA && packetBuf[2]==0xAA
                && packetBuf[3]==0xAA  && (packetBuf[15]&0x20)== 0x20){
            return true;
        }

        myFlow.sleep(1);
    }

    return false;
}

/* 向控制板发送文件 */
bool fpga::ctrlSendFile()
{
    int ret;
    int per;
    int loopTimes = 3;
    int writeLen = PACKET_COUNT;
    bool curPacketIsOk = false;

    for(int i=0; i<writeLen; i++){
        if(stackedWidget->currentIndex() == UPDATE_PAGE){
            pe.setColor(QPalette::WindowText, Qt::darkBlue);
            updateStatusLabels[2]->setPalette(pe);
            per = (100*i)/writeLen;
            QString str = QString("%1 %2%").arg(myLan.upgrade_doing).arg(per);
            updateStatusLabels[2]->setText(str);
        }

//        MySerial.fpgaUpdateWrite(COM_CTRL_UPDATE_SEND_FILE, ctrlAddr, 0, 0, 0, 0, i/256, i%256, 1, fileBuf);
        myFlow.msleep(30);

        for(int j=0; j<loopTimes; j++){
//            MySerial.fpgaUpdateWrite(COM_CTRL_UPDATE_STATUS, ctrlAddr, 0, 0, 0, 0, 0, 0, 1);
            myFlow.msleep(3);

//            ret = MySerial.fpgaUpdateRead(packetBuf, PACKET_SIZE, true);

#if DEBUG_OUTPUT == 1
            printf("ret = %d\n", ret);
            for(int k=0; k<PACKET_SIZE; k++){
                printf("%02x ", packetBuf[k]);
            }
            printf("\n");
#endif

            if(ret < PACKET_SIZE)
                continue;

            if (packetBuf[0]==0xAA && packetBuf[1]==0xAA && packetBuf[2]==0xAA
                    && packetBuf[3]==0xAA  && (packetBuf[15]&0x10)== 0x10){
                curPacketIsOk = true;
                break;
            }
        }

        if(curPacketIsOk)
            curPacketIsOk = false;
        else
            return false;
    }

    return true;
}

/* 向控制板发送文件（改进版） */
bool fpga::ctrlSendFileImprove()
{
    int ret;
    int per;
    int loopTimes = 20;
    int writeLen = PACKET_COUNT;
    int bulkSize = 128;     //每发送128页的数据查询一次状态
    int bulkLen = 0;        //当前页数据在块数据中的位置
    bool curPacketIsOk = false;

    for(int i=0; i<writeLen; i++){
        if(stackedWidget->currentIndex() == UPDATE_PAGE){
            pe.setColor(QPalette::WindowText, Qt::darkBlue);
            updateStatusLabels[2]->setPalette(pe);
            per = (100*i)/writeLen;
            QString str = QString("%1 %2%").arg(myLan.upgrade_doing).arg(per);
            updateStatusLabels[2]->setText(str);
        }

//        MySerial.fpgaUpdateWrite(COM_CTRL_UPDATE_SEND_FILE, ctrlAddr, 0, 0, 0, 0, i/256, i%256, 1, fileBuf);
        ++bulkLen;
        if(bulkLen != bulkSize){
            myFlow.msleep(1);
            continue;
        }

        bulkLen = 0;
        myFlow.msleep(30);

        for(int j=0; j<loopTimes; j++){
//            MySerial.fpgaUpdateWrite(COM_CTRL_UPDATE_STATUS, ctrlAddr, 0, 0, 0, 0, 0, 0, 1);
            myFlow.msleep(3);

//           ret = MySerial.fpgaUpdateRead(packetBuf, PACKET_SIZE, true);

#if DEBUG_OUTPUT == 1
            printf("ret = %d\n", ret);
            for(int k=0; k<PACKET_SIZE; k++){
                printf("%02x ", packetBuf[k]);
            }
            printf("\n");
#endif

            if(ret < PACKET_SIZE){
                myFlow.msleep(50);
                continue;
            }

            if (packetBuf[0]==0xAA && packetBuf[1]==0xAA && packetBuf[2]==0xAA
                    && packetBuf[3]==0xAA  && (packetBuf[15]&0x10)== 0x10){
                curPacketIsOk = true;
                break;
            }

            myFlow.msleep(50);
        }

        if(curPacketIsOk)
            curPacketIsOk = false;
        else
            return false;
    }

    return true;
}

/* 切换控制板至用户模式 */
bool fpga::ctrlSwitchToUser()
{
    int ret;
    int loopTimes = 10;
    bool bIsOk = false;

    myFlow.sleep(2);
//    MySerial.fpgaUpdateWrite(COM_CTRL_UPDATE_USER, ctrlAddr, 0, 0, 0, 0, 0, ctrlUserMode, 1);
    myFlow.sleep(5);

    for(int i=0; i<loopTimes; i++){
//        MySerial.fpgaUpdateWrite(COM_CTRL_UPDATE_STATUS, ctrlAddr, 0, 0, 0, 0, 0, 0, 1);
        myFlow.msleep(3);

//        ret = MySerial.fpgaUpdateRead(packetBuf, PACKET_SIZE, true);
        if(ret < PACKET_SIZE){
            myFlow.sleep(1);
            continue;
        }

        if (packetBuf[0]==0xAA && packetBuf[1]==0xAA && packetBuf[2]==0xAA
                && packetBuf[3]==0xAA  && (packetBuf[15]&0x02)== 0x02){
            bIsOk = true;
        }

        myFlow.sleep(1);
    }

    return bIsOk;

    if(bIsOk){
//        MySerial.fpgaUpdateWrite(COM_CTRL_UPDATE_IS_OK, ctrlAddr, 0, 0, 0, 0, 0x5A, 0x5A, 1);
        return true;
    }
    else{
//        MySerial.fpgaUpdateWrite(COM_CTRL_UPDATE_IS_OK, ctrlAddr, 0, 0, 0, 0, 0xA5, 0xA5, 1);
        return false;
    }
}

/* 接口板升级流程 */
int fpga::intBoardUpdate(int step)
{
    qDebug("intAddr = %d", intAddr);
    int status = 0;
    bool tmpStatus;

    switch(step){
        case 1:
            goto step1;
            break;
        case 2:
            goto step2;
            break;
        case 3:
            goto step3;
            break;
        case 4:
            goto step4;
            break;
        default:
            break;
    }

step1:
    setStatusLabelShow(0, UPDATE_DOING);
    tmpStatus = stepSwitchToFactory(INT_BOARD);

    if(!tmpStatus){
        status = 1;
        setStatusLabelShow(0, UPDATE_FAIL);
        return status;
    }
    else
        setStatusLabelShow(0, UPDATE_OK);

step2:
    if(bTransByUSB){
        setStatusLabelShow(1, UPDATE_DOING);
        myFlow.sleep(2);
        tmpStatus = stepSendFileByUSB();
    } else {
        tmpStatus = stepSendFile();
    }

    if (!tmpStatus){
        status = 2;
        setStatusLabelShow(1, UPDATE_FAIL);
        return status;
    }
    else
        setStatusLabelShow(1, UPDATE_OK);

step3:
    setStatusLabelShow(2, UPDATE_DOING);
    tmpStatus = stepBurnFile();

    if (!tmpStatus){
        status = 3;
        setStatusLabelShow(2, UPDATE_FAIL);
        return status;
    }
    else
        setStatusLabelShow(2, UPDATE_OK);

step4:
    setStatusLabelShow(3, UPDATE_DOING);
    tmpStatus = stepSwitchToUser(INT_BOARD);

    if (!tmpStatus){
        status = 4;
        setStatusLabelShow(3, UPDATE_FAIL);
        return status;
    }
    else
        setStatusLabelShow(3, UPDATE_OK);

    return status;
}

/* 彩色相机板升级流程 */
int fpga::colorBoardUpdate(int step)
{
    qDebug("colorAddr = 0x%02x", cameraAddr[curView]);
    int status = 0;
    bool tmpStatus;
#if 0
    switch(step){
    case 1:
        switch(struCnfg.nLevelTotal){
        case 1:
            if(bCameraFileIsOk[0])
                goto step3;
            else
                goto step1;
            break;
        case 2:
            if((bColorFileIsOk[0] && intAddr==0x01) || (bColorFileIsOk[1] && intAddr==0x11))
                goto step3;
            else
                goto step1;
            break;
        case 3:
            if((bColorFileIsOk[0] && intAddr==0x01) || (bColorFileIsOk[1] && intAddr==0x11)
                    || (bColorFileIsOk[2] && intAddr==0x21))
                goto step3;
            else
                goto step1;
            break;
        default:
            break;
        }
        break;
    case 2:
        goto step2;
        break;
    case 3:
        goto step3;
        break;
    case 4:
        goto step4;
        break;
    case 5:
        goto step5;
        break;
    default:
        break;
    }

step1:
    setStatusLabelShow(0, UPDATE_DOING);
    tmpStatus = stepSwitchToFactory(INT_BOARD);

    if(!tmpStatus){
        status = 1;
        setStatusLabelShow(0, UPDATE_FAIL);
        return status;
    }
    else{
        bIntIsFactory = true;
        setStatusLabelShow(0, UPDATE_OK);
    }

step2:
    if(bTransByUSB){
        setStatusLabelShow(1, UPDATE_DOING);
        myFlow.sleep(2);
        tmpStatus = stepSendFileByUSB();
    } else {
        if(struGsh.nIntBaud[0] == MY_UARTBAUD_921600) {
            tmpStatus = stepSendFileByHighUART();
        } else {
            tmpStatus = stepSendFile();
        }
    }

    if (!tmpStatus){
        status = 2;
        setStatusLabelShow(1, UPDATE_FAIL);
        return status;
    }
    else{
        if(intAddr == 0x01)
            bColorFileIsOk[0] = true;
        else if(intAddr == 0x11)
            bColorFileIsOk[1] = true;
        else if(intAddr == 0x21)
            bColorFileIsOk[2] = true;

        setStatusLabelShow(1, UPDATE_OK);
    }

step3:
    if (struCnfe.nMachine == MACHINE_LD4 && colorAddr >= FOUR_VIEW_NEW_START_NO) {
        /* 先切换主相机 */
        colorAddr = colorAddr - FOUR_VIEW_NEW_START_NO;

        setStatusLabelShow(2, UPDATE_DOING);
        tmpStatus = stepSwitchToFactory(COLOR_BOARD);
        if(!tmpStatus){
            status = 3;
            setStatusLabelShow(2, UPDATE_FAIL);
            colorAddr = colorAddr + FOUR_VIEW_NEW_START_NO;
            return status;
        }
        else
            setStatusLabelShow(2, UPDATE_OK);

        colorAddr = colorAddr + FOUR_VIEW_NEW_START_NO;

        /* 再切换从相机 */
        setStatusLabelShow(2, UPDATE_DOING);
        tmpStatus = stepSwitchToFactory(COLOR_BOARD);
        if(!tmpStatus){
            status = 3;
            setStatusLabelShow(2, UPDATE_FAIL);
            return status;
        }
        else
            setStatusLabelShow(2, UPDATE_OK);

    } else {
        setStatusLabelShow(2, UPDATE_DOING);
        tmpStatus = stepSwitchToFactory(COLOR_BOARD);
        if(!tmpStatus){
            status = 3;
            setStatusLabelShow(2, UPDATE_FAIL);
            return status;
        }
        else
            setStatusLabelShow(2, UPDATE_OK);
    }

step4:
    setStatusLabelShow(3, UPDATE_DOING);
    tmpStatus = stepBurnFile();
    if (!tmpStatus){
        status = 4;
        setStatusLabelShow(3, UPDATE_FAIL);
        return status;
    }
    else
        setStatusLabelShow(3, UPDATE_OK);

step5:
    if (struCnfe.nMachine == MACHINE_LD4 && colorAddr >= FOUR_VIEW_NEW_START_NO) {
        /* 先切换主相机 */
        colorAddr = colorAddr - FOUR_VIEW_NEW_START_NO;

        setStatusLabelShow(4, UPDATE_DOING);
        tmpStatus = stepSwitchToUser(COLOR_BOARD);
        if (!tmpStatus){
            status = 5;
            setStatusLabelShow(4, UPDATE_FAIL);
            colorAddr = colorAddr + FOUR_VIEW_NEW_START_NO;
            return status;
        }
        else
            setStatusLabelShow(4, UPDATE_OK);

        colorAddr = colorAddr + FOUR_VIEW_NEW_START_NO;

        /* 再切换从相机 */
        setStatusLabelShow(4, UPDATE_DOING);
        tmpStatus = stepSwitchToUser(COLOR_BOARD);
        if (!tmpStatus){
            status = 5;
            setStatusLabelShow(4, UPDATE_FAIL);
            return status;
        }
        else
            setStatusLabelShow(4, UPDATE_OK);

    } else {
        setStatusLabelShow(4, UPDATE_DOING);
        tmpStatus = stepSwitchToUser(COLOR_BOARD);
        if (!tmpStatus){
            status = 5;
            setStatusLabelShow(4, UPDATE_FAIL);
            return status;
        }
        else
            setStatusLabelShow(4, UPDATE_OK);
    }

#endif
    return status;
}

/* 红外相机板升级流程 */
int fpga::infBoardUpdate(int step)
{

//    qDebug("infAddr = 0x%02x", infAddr);
    int status = 0;
    int tmpStatus;
 #if 0
    switch(step){
    case 1:
        switch(struCnfg.nLevelTotal){
        case 1:
            if(bInfFileIsOk[0])
                goto step3;
            else
                goto step1;
            break;
        case 2:
            if((bInfFileIsOk[0] && intAddr==0x01) || (bInfFileIsOk[1] && intAddr==0x11))
                goto step3;
            else
                goto step1;
            break;
        case 3:
            if((bInfFileIsOk[0] && intAddr==0x01) || (bInfFileIsOk[1] && intAddr==0x11)
                    || (bInfFileIsOk[2] && intAddr==0x21))
                goto step3;
            else
                goto step1;
            break;
        default:
            break;
        }
        break;
    case 2:
        goto step2;
        break;
    case 3:
        goto step3;
        break;
    case 4:
        goto step4;
        break;
    case 5:
        goto step5;
        break;
    case 6:
        goto step6;
        break;
    case 7:
        goto step7;
        break;
    default:
        break;
    }

step1:
    setStatusLabelShow(0, UPDATE_DOING);
    tmpStatus = stepSwitchToFactory(INT_BOARD);

    if(!tmpStatus){
        status = 1;
        setStatusLabelShow(0, UPDATE_FAIL);
        return status;
    }
    else{
        bIntIsFactory = true;
        setStatusLabelShow(0, UPDATE_OK);
    }

step2:
    if(bTransByUSB){
        setStatusLabelShow(1, UPDATE_DOING);
        myFlow.sleep(2);
        tmpStatus = stepSendFileByUSB();
    } else {
        if(struGsh.nIntBaud[0] == MY_UARTBAUD_921600) {
            tmpStatus = stepSendFileByHighUART();
        } else {
            tmpStatus = stepSendFile();
        }
    }

    if(!tmpStatus){
        status = 2;
        setStatusLabelShow(1, UPDATE_FAIL);
        return status;
    }
    else{
        if(intAddr == 0x01)
            bInfFileIsOk[0] = true;
        else if(intAddr == 0x11)
            bInfFileIsOk[1] = true;
        else if(intAddr == 0x21)
            bInfFileIsOk[2] = true;

        setStatusLabelShow(1, UPDATE_OK);
    }

step3:
    setStatusLabelShow(2, UPDATE_DOING);
    tmpStatus = stepSwitchToFactory(COLOR_BOARD);

    if(!tmpStatus){
        status = 3;
        setStatusLabelShow(2, UPDATE_FAIL);
        return status;
    }
    else{
        bColorIsFactory = true;
        setStatusLabelShow(2, UPDATE_OK);
    }

step4:
    setStatusLabelShow(3, UPDATE_DOING);
    tmpStatus = stepSwitchToFactory(INF_BOARD);

    if(!tmpStatus){
        status = 4;
        setStatusLabelShow(3, UPDATE_FAIL);
        return status;
    }
    else
        setStatusLabelShow(3, UPDATE_OK);

step5:
    setStatusLabelShow(4, UPDATE_DOING);
    tmpStatus = stepBurnFile();

    if(!tmpStatus){
        status = 5;
        setStatusLabelShow(4, UPDATE_FAIL);
        return status;
    }
    else
        setStatusLabelShow(4, UPDATE_OK);

step6:
    setStatusLabelShow(5, UPDATE_DOING);
    tmpStatus = stepSwitchToUser(INF_BOARD);

    if(!tmpStatus){
        status = 6;
        setStatusLabelShow(5, UPDATE_FAIL);
        return status;
    }
    else
        setStatusLabelShow(5, UPDATE_OK);

step7:
    setStatusLabelShow(6, UPDATE_DOING);
    tmpStatus = stepSwitchToUser(COLOR_BOARD);

    if (!tmpStatus){
        status = 7;
        setStatusLabelShow(6, UPDATE_FAIL);
        return status;
    }
    else{
        bColorIsFactory = false;
        setStatusLabelShow(6, UPDATE_OK);
    }
#endif
    return status;
}

/* 控制板升级流程 */
int fpga::ctrlBoardUpdate(int step)
{
    qDebug("ctrlAddr = %d", ctrlAddr);
    int status = 0;
    bool tmpStatus;

    switch(step){
        case 1:
            goto step1;
            break;
        case 2:
            goto step2;
            break;
        case 3:
            goto step3;
            break;
        case 4:
            goto step4;
            break;
        default:
            break;
    }

step1:
    setStatusLabelShow(0, UPDATE_DOING);
    tmpStatus = ctrlSwitchToFactory();

    if(!tmpStatus){
        status = 1;
        setStatusLabelShow(0, UPDATE_FAIL);
        return status;
    }
    else
        setStatusLabelShow(0, UPDATE_OK);

step2:
    setStatusLabelShow(1, UPDATE_DOING);
    tmpStatus = ctrlEraseFlash();

    if (!tmpStatus){
        status = 2;
        setStatusLabelShow(1, UPDATE_FAIL);
        return status;
    }
    else
        setStatusLabelShow(1, UPDATE_OK);

step3:
    tmpStatus = ctrlSendFileImprove();

    if (!tmpStatus){
        status = 3;
        setStatusLabelShow(2, UPDATE_FAIL);
        return status;
    }
    else
        setStatusLabelShow(2, UPDATE_OK);

step4:
    setStatusLabelShow(3, UPDATE_DOING);
    tmpStatus = ctrlSwitchToUser();

    if (!tmpStatus){
        status = 4;
        setStatusLabelShow(3, UPDATE_FAIL);
        return status;
    }
    else
        setStatusLabelShow(3, UPDATE_OK);

    return status;
}

void fpga::wholeUpdatIntferMaize(void)
{
#if 0
    bool bChangeBoardMode = false;
    int tmpIntAddr = intAddr;
    int tmpColorAddr = colorAddr;
    int tmpInfAddr = infAddr;
    int viewTmp = 0;
    int chuteTmp = 0;

    intAddr = 0x01;
    colorAddr = 0x00;
    infAddr = 0x00;

    for(int i=0; i<struCnfg.struLevelInfo[0].nUnitLevelTotal; i++){
        colorAddr =  struCnfg.struLevelInfo[0].nUnitId[i];
        qDebug("colorAddr = %d", colorAddr);
        if(colorAddr%2 == 0){
            if (colorAddr < MAIZE_ID_START_ADDED) {
                viewTmp  = 0;
                chuteTmp = colorAddr / 2;
            } else {
                viewTmp  = 1;
                chuteTmp = (colorAddr - MAIZE_ID_START_ADDED) / 2;
            }
        } else {
            if (colorAddr < MAIZE_ID_START_ADDED) {
                viewTmp  = 2;
                chuteTmp = colorAddr / 2;
            } else {
                viewTmp  = 3;
                chuteTmp = (colorAddr - MAIZE_ID_START_ADDED) / 2;
            }
        }

        setWholeStatusShow(viewTmp, chuteTmp, UPDATE_DOING);
        if(colorBoardUpdate(1) == 0) {
            bChangeBoardMode = true;
            setWholeStatusShow(viewTmp, chuteTmp, UPDATE_OK);
        } else {
            setWholeStatusShow(viewTmp, chuteTmp, UPDATE_FAIL);
        }
    }

    intAddr = tmpIntAddr;
    colorAddr = tmpColorAddr;
    infAddr = tmpInfAddr;

    if(bChangeBoardMode){
        struCnfg.nCameraBoardMode[curLevel][curView] = cameraUserMode[curView];
    }
#endif
    // 彩色相机板升级完成后，将接口板切至用户模式
    stepSwitchToUser(INT_BOARD);
}

/* 整机升级 */
void fpga::wholeUpdate()
{
    int tmpStatus;
    bool bChangeBoardMode = false;
    int tmpIntAddr = intAddr;
    int tmpColorAddr = cameraAddr[curView];

    int viewTmp = 0;
    int chuteTmp = 0;
//    if (struCnfe.nProduct == OPT_PRODUCT_ANCOO_MAIZE) {
//        wholeUpdatIntferMaize();
//        return;
//    }

    intAddr = 0x01;
    cameraAddr[curView] = 0x00;

    if(curBoardMode == COLOR_BOARD){
        for(int i=0; i<struCnfg.struLayerInfo[0].nViewUnitTotal[curView]; i++){
            qDebug("i = %d", i);
            if(i > 0)
                cameraAddr[curView]++;

            setWholeStatusShow(curView, cameraAddr[curView], UPDATE_DOING);
            tmpStatus = colorBoardUpdate(1);
            if(tmpStatus == 0){
                bChangeBoardMode = true;
                setWholeStatusShow(curView, cameraAddr[curView], UPDATE_OK);
            }
            else
                setWholeStatusShow(curView, cameraAddr[curView], UPDATE_FAIL);

        }
    }


    intAddr = tmpIntAddr;
    cameraAddr[curView] = tmpColorAddr;

    if(bChangeBoardMode){
        struCnfg.nCameraBoardMode[curLevel][curView] = cameraUserMode[curView];
    }
    // 红外与彩色相机板升级完成后，将接口板切至用户模式
    stepSwitchToUser(INT_BOARD);
}

/* 根据某一步升级状态显示不同内容 */
void fpga::setStatusLabelShow(int mesIndex, fpga::updateStatus status)
{
    if(stackedWidget->currentIndex() != UPDATE_PAGE)
        return;

    QString suffix;

    suffix = QString();


    switch(status){
    case UPDATE_NO_START:
        pe.setColor(QPalette::WindowText, Qt::blue);
        updateStatusLabels[mesIndex]->setPalette(pe);
        updateStatusLabels[mesIndex]->setText("upgrade_untreated");
        break;
    case UPDATE_DOING:
        pe.setColor(QPalette::WindowText, Qt::darkBlue);
        updateStatusLabels[mesIndex]->setPalette(pe);
        updateStatusLabels[mesIndex]->setText("upgrade_doing" + suffix);
        break;
    case UPDATE_OK:
        pe.setColor(QPalette::WindowText, Qt::green);
        updateStatusLabels[mesIndex]->setPalette(pe);
        updateStatusLabels[mesIndex]->setText("upgrade_ok" + suffix);
        break;
    case UPDATE_FAIL:
        pe.setColor(QPalette::WindowText, Qt::red);
        updateStatusLabels[mesIndex]->setPalette(pe);
        updateStatusLabels[mesIndex]->setText("upgrade_fail" + suffix);
        break;
    default:
        break;
    }
}

/* 根据单板的升级状态显示不同内容 */
void fpga::setWholeStatusShow(int level, int mesIndex, fpga::updateStatus status)
{
    switch(status){
    case UPDATE_NO_START:
        pe.setColor(QPalette::WindowText, Qt::blue);
        statusLabels[level][mesIndex]->setPalette(pe);
        statusLabels[level][mesIndex]->setText("upgrade_untreated");
        break;
    case UPDATE_DOING:
        pe.setColor(QPalette::WindowText, Qt::darkBlue);
        statusLabels[level][mesIndex]->setPalette(pe);
        statusLabels[level][mesIndex]->setText("upgrade_doing");
        break;
    case UPDATE_OK:
        pe.setColor(QPalette::WindowText, Qt::green);
        statusLabels[level][mesIndex]->setPalette(pe);
        statusLabels[level][mesIndex]->setText("upgrade_ok");
        break;
    case UPDATE_FAIL:
        pe.setColor(QPalette::WindowText, Qt::red);
        statusLabels[level][mesIndex]->setPalette(pe);
        statusLabels[level][mesIndex]->setText("upgrade_fail");
        break;
    case UPDATE_NO_EXIST:
        pe.setColor(QPalette::WindowText, Qt::blue);
        statusLabels[level][mesIndex]->setPalette(pe);
        statusLabels[level][mesIndex]->setText(" - -  - -");
        break;
    default:
        break;
    }
}

/* 响应挂载U盘界面的返回按钮 */
void fpga::onMountUdiskBackBtnClicked()
{
#ifdef Q_OS_WIN
    emit sBackToFactorySetPage();
#else
    myFlow.saveProfile();
    myFlow.sleep(1);
    myFlow.setTsBackLight(0);
    system("reboot");
#endif
}

/* 响应挂载U盘界面的下一步按钮 */
void fpga::onMountUdiskGoBtnClicked()
{   
#ifdef Q_OS_UNIX
    QDir mountDir("/proc/scsi/usb-storage");
#else
    QDir mountDir("/udisk");
#endif

    if(! mountDir.exists()){
        g_infoWidget().setLabelText("cfm_insert_udisk");
        g_infoWidget().showSecs();
        return;
    }

    QFileInfoList fileInfo;
    QString fileName;
    QDir dir(UPDATE_FILE_PATH);

    fileInfo = dir.entryInfoList();
    updateFileList.clear();

    for(int i=0; i<fileInfo.size(); i++){
        fileName = fileInfo.at(i).fileName();
        if(fileName.contains(".rpd"))
            updateFileList.push_back(fileName);
    }

    int fileNum = (updateFileList.size()>MAX_FILE_NUM) ? MAX_FILE_NUM:updateFileList.size();
    for(int i=0; i<fileNum; i++){
        fileItem[i]->setText(QString("%1. %2").arg(i+1).arg(updateFileList.at(i)));
        fileItem[i]->setHidden(false);
    }
    for(int i=fileNum; i<MAX_FILE_NUM;i++){
        fileItem[i]->setHidden(true);
    }

//    if(bTransByUSB)
//        transModeBtn->setText(myLan.ai_high_speed);
//    else
//        transModeBtn->setText(myLan.ai_low_speed);

    transModeBtn->setText("高速/High Speed");
    stackedWidget->setCurrentIndex(FILE_LIST_PAGE);
}

/* 响应文件列表界面文件索引发生改变的事件 */
void fpga::onFileListChangedSlt(int index)
{
    int fileNum = (updateFileList.size()>MAX_FILE_NUM) ? MAX_FILE_NUM:updateFileList.size();
    for(int i=0; i<fileNum; i++){
        fileList->item(i)->setBackground(QBrush(QColor(190,190,190)));
    }
    fileList->item(index)->setBackground(QBrush(QColor(172,144,75)));
}

/* 响应文件列表界面下一步按钮 */
void fpga::onFileListNextBtnClicked()
{
    if(updateFileList.size() == 0)
        return;

    QString fileName = QString("%1%2").arg(UPDATE_FILE_PATH).arg(updateFileList.at(fileList->currentRow()));
    qDebug("fileName = %s", fileName.toLatin1().data());

    FILE *fp;
    if((fp=fopen(fileName.toLatin1().data(), "r")) == NULL){
        g_infoWidget().setLabelText("msg_file_open_error");
        g_infoWidget().show();
        myFlow.sleep(2);
        g_infoWidget().hide();
        return;
    }

    /* 1.根据文件名判断升级板子的类型 */
    if(fileName.contains("JK")) {
        curBoardMode = INT_BOARD;
    } else if(fileName.contains("D2QZ") || fileName.contains("T1QZ")
            || fileName.contains("T3QZ")) {
        curBoardMode = COLOR_BOARD; 
    } else if(fileName.contains("KZ")){
        curBoardMode = CTRL_BOARD;
    } else if(fileName != "") {
        g_infoWidget().setLabelText("未知文件/Unknown File");
        g_infoWidget().show();
        myFlow.sleep(2);
        g_infoWidget().hide();
        return;
    }

    /* 2.设置升级页面的显示 */
    setUpdateDisplay();

    /* 3.将升级文件拷贝至本地缓冲区 */
    memset(fileBuf, 0, PACKET_COUNT*FILE_PACKET_SIZE);
//    if(curBoardMode==INF_BOARD || curBoardMode==INT_CTRL_BOARD) {
//        fread(fileBuf, 1, PACKET_COUNT_SMALL*FILE_PACKET_SIZE, fp);
//    } else {
//        fread(fileBuf, 1, PACKET_COUNT*FILE_PACKET_SIZE, fp);
//    }
    fread(fileBuf, 1, PACKET_COUNT*FILE_PACKET_SIZE, fp);
    fclose(fp);

    if(bTransByUSB)
        stackedWidget->setCurrentIndex(NOTICE_PAGE);
    else
        stackedWidget->setCurrentIndex(UPDATE_PAGE);

    /* 4.重置远程升级相关参数,并更新界面显示 */
    initUpdateParams();
    userModeGroup->clearBtnsIcon();
    layerGroup->setCurrentIndex(0);

    layerMaizeAddGroup->setCurrentIndex(0);

    boardUpdateLcdNum->display(1);  
}

/* 响应文件列表界面上一步按钮 */
void fpga::onFileListLastBtnClicked()
{
    stackedWidget->setCurrentIndex(MOUNT_PAGE);
}

/* 响应文件列表界面中传输模式按钮 */
void fpga::onTransModeBtnClicked()
{
    bTransByUSB = !bTransByUSB;
    if(bTransByUSB)
        transModeBtn->setText("ai_high_speed");
    else
        transModeBtn->setText("ai_low_speed");
}

/* 响应提示界面中的"上一步"按钮 */
void fpga::onNoticeLastBtnClicked()
{
    stackedWidget->setCurrentIndex(FILE_LIST_PAGE);
}

/* 响应提示界面中的"下一步"按钮 */
void fpga::onNoticeNextBtnClicked()
{
//#ifdef Q_OS_UNIX
#if 0
    if(myUsbDev != NULL){
        usb.myusbFree();
    }
    usb.myusbInit();
    if(myUsbDev == NULL){
        qDebug("usb init error");
        g_infoWidget().setLabelText("cfm_insert_datawire");
        g_infoWidget().show();
        myFlow.sleep(2);
        g_infoWidget().hide();
        return;
    }
#endif

    stackedWidget->setCurrentIndex(UPDATE_PAGE);
}

/* 响应板级升级界面的返回按钮 */
void fpga::onBoardUpdateBackBtnClicked()
{
    if(bTransByUSB)
        stackedWidget->setCurrentIndex(MOUNT_PAGE);
    else
        stackedWidget->setCurrentIndex(FILE_LIST_PAGE);
}

/* 响应板级升级界面的升级按钮 */
void fpga::onBoardUpdateGoBtnClicked()
{
    if((curBoardMode==INT_BOARD && intUserMode==0)
            || (curBoardMode==COLOR_BOARD && cameraUserMode==0)
            || (curBoardMode==CTRL_BOARD && ctrlUserMode==0)){
        g_infoWidget().setLabelText("msg_select_user_mode");
        g_infoWidget().show();
        myFlow.sleep(2);
        g_infoWidget().hide();
        return;
    }

    g_infoWidget().hideCentralWidget();
    switch(curBoardMode){
    case INT_BOARD:
        for(int i=0; i<4; i++){
            setStatusLabelShow(i, UPDATE_NO_START);
        }
        g_infoWidget().delayShow();
        curUpdateStep = intBoardUpdate(1);
        g_infoWidget().hide();
        break;

    case COLOR_BOARD:
        if((intAddr==0x01 && bCameraFileIsOk[0]) || (intAddr==0x11 && bCameraFileIsOk[1])
                || (intAddr==0x21 && bCameraFileIsOk[2])){
            setStatusLabelShow(0, UPDATE_OK);
            setStatusLabelShow(1, UPDATE_OK);
            for(int i=2; i<5; i++){
                setStatusLabelShow(i, UPDATE_NO_START);
            }
        }
        else{
            for(int i=0; i<5; i++){
                setStatusLabelShow(i, UPDATE_NO_START);
            }
        }
        g_infoWidget().delayShow();
        curUpdateStep = colorBoardUpdate(1);
        g_infoWidget().hide();
        break;
    case CTRL_BOARD:
        for(int i=0; i<4; i++){
            setStatusLabelShow(i, UPDATE_NO_START);
        }
        g_infoWidget().delayShow();
        curUpdateStep = ctrlBoardUpdate(1);
        g_infoWidget().hide();
        break;

    default:
        break;
    }

    if(curUpdateStep != 0)
        boardUpdateRetryBtn->setEnabled(true);
    else
    {
        // 升级成功后设置对应的全局参数
        switch(curBoardMode){
        case INT_BOARD:
            struCnfg.nInterfaceBoardMode = intUserMode;
            break;
        case COLOR_BOARD:
            struCnfg.nCameraBoardMode[curLevel][curView] = cameraUserMode[curView];
            break;
        default:
            break;
        }
    }
}

/* 响应板级升级界面的整机升级按钮 */
void fpga::onBoardUpdateWholeBtnClicked()
{
    if((curBoardMode==COLOR_BOARD && cameraUserMode==0)){
        g_infoWidget().setLabelText("msg_select_user_mode");
        g_infoWidget().show();
        myFlow.sleep(2);
        g_infoWidget().hide();
        return;
    }

    myMessageBox *mesBox = new myMessageBox(MSG_QUES, "cfm_upgrade_whole");
    int ret = mesBox->exec();

    if(ret == QDialog::Accepted){
        setUpdateWholeDisplay();
        stackedWidget->setCurrentIndex(WHOLE_UPDATE_PAGE);
        g_infoWidget().hideCentralWidget();
        g_infoWidget().delayShow();
        wholeUpdate();
        g_infoWidget().hide();
    }
}

/* 响应板级升级界面的重试按钮 */
void fpga::onBoardUpdateRetryBtnClicked()
{
    switch(curBoardMode){
    case INT_BOARD:
        curUpdateStep = intBoardUpdate(curUpdateStep);
        break;
    case COLOR_BOARD:
        curUpdateStep = colorBoardUpdate(curUpdateStep);
        break;
    case CTRL_BOARD:
        curUpdateStep = ctrlBoardUpdate(curUpdateStep);
        break;
    default:
        break;
    }

    if(curUpdateStep == 0){
        boardUpdateRetryBtn->setEnabled(false);

        // 升级成功后设置对应的全局参数
        switch(curBoardMode){
        case INT_BOARD:
            struCnfg.nInterfaceBoardMode = intUserMode;
            break;
        case COLOR_BOARD:
            struCnfg.nCameraBoardMode[0][curView] = cameraUserMode[curView];
            break;
        default:
            break;
        }
    }
}

/* 响应用户模式改变的事件 */
void fpga::onUserModeChangedSlt(int index)
{
    int tmpMode;
    if(index == 0)
        tmpMode = 1;
    else if(index == 1)
        tmpMode = 2;
    else if(index == 2)
        tmpMode = 4;

    switch(curBoardMode){
    case INT_BOARD:
        intUserMode = tmpMode;
        break;
    case COLOR_BOARD:
        cameraUserMode[curView] = tmpMode;
        break;
    case CTRL_BOARD:
        ctrlUserMode = tmpMode;
        break;
    default:
        break;
    }
}

/* 响应主从发生改变的事件 */
void fpga::onlayerAddChangedSlt(int index)
{
//     if (struCnfe.nProduct == OPT_PRODUCT_ANCOO_MAIZE) {
//        if (index == 1) {
//            if (colorAddr < MAIZE_ID_START_ADDED) {
//                if (colorAddr >= struCnfg.struLevelInfo[0].nUnitLevelTotal / 2 - 1) {
//                    colorAddr += MAIZE_ID_START_ADDED - 2;
//                    boardUpdateLcdNum->display((struCnfg.struLevelInfo[0].nUnitLevelTotal + 2) / 4 - 1);
//                } else {
//                    colorAddr += MAIZE_ID_START_ADDED;
//                }
//            }
//        } else {
//            if (colorAddr >= MAIZE_ID_START_ADDED) {
//                colorAddr -= MAIZE_ID_START_ADDED;
//            }
//        }
//    }

    qDebug("colorAddr: %d",cameraAddr[curView]);
}



/* 响应层（前后视）发生改变的事件 */
void fpga::onLayerChangedSlt(int index)
{
//    int preBoardNum = 2*(boardUpdateLcdNum->value()-1);

//    if(index == 0){
//        if (colorAddr%2==1 && infAddr%2==1) {
//            colorAddr -= 1;
//            infAddr -= 1;
//        }
//    } else{
//        if (cameraAddr[curView]%2==0 && infAddr%2==0) {
//            colorAddr += 1;
//            infAddr += 1;
//        }
//    }

    qDebug("colorAddr: %d, intAddr: %d",cameraAddr[curView],intAddr);
}

/* 响应板级升级界面的增加板号按钮 */
void fpga::onBoardNumPlusBtnClicked()
{
    int boardNum = cameraAddr[curView]+1-intAddr;

//    int plusSpecialNum[4] = {0};//4排相机的结尾序号
//    int isSpecialNum = 0;
//    int tmp = 0;

//    if(struCnfg.struLevelInfo[curLevel].nViewTotal == 2){   //双视
//        if (struCnfe.nProduct == OPT_PRODUCT_ANCOO_MAIZE) {
//            tmp = (struCnfg.struLevelInfo[ONE_LEVEL].nUnitLevelTotal + 2) / 4;//后视单排相机数
//            plusSpecialNum[0] = (tmp - 1) * 2;                            //前下
//            plusSpecialNum[1] = (tmp - 1) * 2 + 1;                         //后下
//            plusSpecialNum[2] = MAIZE_ID_START_ADDED +  (tmp - 2) * 2;     //前上
//            plusSpecialNum[3] = MAIZE_ID_START_ADDED +  (tmp - 2) * 2 + 1; //后上

//            for (int i = 0; i < 4; i++) {
//                if (colorAddr == plusSpecialNum[i]) {
//                    isSpecialNum = 1;
//                    break;
//                }
//            }

//            if (isSpecialNum == 0) {
//                colorAddr += 2;
//                if (colorAddr < MAIZE_ID_START_ADDED) {
//                    boardUpdateLcdNum->display(colorAddr / 2 + 1);
//                } else {
//                    boardUpdateLcdNum->display((colorAddr - MAIZE_ID_START_ADDED) / 2  + 1);
//                }
//            }
//        } else if (struCnfe.nMachine == MACHINE_LD4) {
//            tmp = (struCnfg.struLevelInfo[ONE_LEVEL].nUnitLevelTotal + 2) / 4;//单排相机数多
//            plusSpecialNum[0] = (tmp - 1) * 2;                                //上多
//            plusSpecialNum[1] = (tmp - 1) * 2 + 1;                            //下多
//            plusSpecialNum[2] = FOUR_VIEW_NEW_START_NO +  (tmp - 2) * 2;      //上少
//            plusSpecialNum[3] = FOUR_VIEW_NEW_START_NO +  (tmp - 2) * 2 + 1;  //下少

//            for (int i = 0; i < 4; i++) {
//                if (colorAddr == plusSpecialNum[i]) {
//                    isSpecialNum = 1;
//                    break;
//                }
//            }

//            if (isSpecialNum == 0) {
//                colorAddr += 2;
//                if (colorAddr < FOUR_VIEW_NEW_START_NO) {
//                    boardUpdateLcdNum->display(colorAddr / 2 + 1);
//                } else {
//                    boardUpdateLcdNum->display((colorAddr - FOUR_VIEW_NEW_START_NO) / 2  + 1);
//                }
//            }
//        } else {
//            if(boardNum < struCnfg.struLevelInfo[curLevel].nUnitLevelTotal-2){
//                colorAddr += 2;
//                infAddr += 2;
//                boardNum += 2;
//                boardUpdateLcdNum->display(boardNum/2+1);
//            }
//        }

//    } else if(struCnfg.struLevelInfo[curLevel].nViewTotal == 1){  //单视
//        if(boardNum < struCnfg.struLevelInfo[curLevel].nUnitLevelTotal*2-2){
//            colorAddr += 2;
//            infAddr += 2;
//            boardNum += 2;
//            boardUpdateLcdNum->display(boardNum/2+1);
//        }
//    }

    qDebug("colorAddr = %d", cameraAddr[curView]);
}

/* 响应板级升级界面的减小板号按钮 */
void fpga::onBoardNumMinusBtnClicked()
{
    int boardNum = cameraAddr[curView]+1-intAddr;
    int isSpecialNum = 0;
    int minuSpecialNum[4] = {0};//4排相机的起始序号

//    if(struCnfg.struLevelInfo[curLevel].nViewTotal == 2){   //双视
//        if (struCnfe.nProduct == OPT_PRODUCT_ANCOO_MAIZE) {
//            minuSpecialNum[0] = 0; //前下
//            minuSpecialNum[1] = 1; //后下
//            minuSpecialNum[2] = 14;//前上
//            minuSpecialNum[3] = 15;//后上

//            for (int i = 0; i < 4; i++) {
//                if (colorAddr == minuSpecialNum[i]) {
//                    isSpecialNum = 1;
//                    break;
//                }
//            }

//            if (isSpecialNum == 0) {
//                colorAddr -= 2;
//                if (colorAddr < MAIZE_ID_START_ADDED) {
//                    boardUpdateLcdNum->display(colorAddr / 2 + 1);
//                } else {
//                    boardUpdateLcdNum->display((colorAddr - MAIZE_ID_START_ADDED) / 2  + 1);
//                }
//            }
//        } else if (struCnfe.nMachine == MACHINE_LD4) {
//            minuSpecialNum[0] = 0;                          //上多
//            minuSpecialNum[1] = 1;                          //下多
//            minuSpecialNum[2] = FOUR_VIEW_NEW_START_NO;     //上少
//            minuSpecialNum[3] = FOUR_VIEW_NEW_START_NO + 1; //下少

//            for (int i = 0; i < 4; i++) {
//                if (colorAddr == minuSpecialNum[i]) {
//                    isSpecialNum = 1;
//                    break;
//                }
//            }

//            if (isSpecialNum == 0) {
//                colorAddr -= 2;
//                if (colorAddr < FOUR_VIEW_NEW_START_NO) {
//                    boardUpdateLcdNum->display(colorAddr / 2 + 1);
//                } else {
//                    boardUpdateLcdNum->display((colorAddr - FOUR_VIEW_NEW_START_NO) / 2  + 1);
//                }
//            }
//        } else {
//            if(boardNum >= 2){
//                colorAddr -= 2;
//                infAddr -= 2;
//                boardNum -= 2;
//                boardUpdateLcdNum->display(boardNum/2+1);
//            }
//        }
//    } else if(struCnfg.struLevelInfo[curLevel].nViewTotal == 1){  //单视
//        if(boardNum >= 2){
//            colorAddr -= 2;
//            infAddr -= 2;
//            boardNum -= 2;
//            boardUpdateLcdNum->display(boardNum/2+1);
//        }
//    }
    qDebug("colorAddr = %d", cameraAddr[curView]);
}

/* 响应整机升级界面的返回按钮 */
void fpga::onWholeUpdateBackBtnClicked()
{
    stackedWidget->setCurrentIndex(UPDATE_PAGE);
}

/* 响应整机升级界面的退出按钮 */
void fpga::onWholeUpdateReturnBtnClicked()
{
#ifdef Q_OS_WIN
    emit sBackToFactorySetPage();
    stackedWidget->setCurrentIndex(MOUNT_PAGE);
#else
    myFlow.saveProfile();
    myFlow.sleep(1);
    myFlow.setTsBackLight(0);
    system("reboot");
#endif
}

/* USB发送文件 */
void fpga::USBSendFile(int cmd, unsigned char module, int num)
{
//#ifdef Q_OS_UNIX
#if 0
    char packet[PACKET_COUNT*FRAME_LEN+16];
    char buf[PACKET_COUNT*FRAME_LEN+16];
    char crcdata[PACKET_COUNT*FRAME_LEN+10];
    unsigned int crcitt;
    int i, j;
    int ret = 0;

    /* pack & add checksum */
    bzero(packet, PACKET_COUNT*FRAME_LEN+16);

    packet[0]  = 0xF0;
    packet[1]  = 0xA5;
    packet[2]  = module;
    packet[3]  = 0;
    packet[4]   = cmd/256;
    packet[5]   = cmd%256;
    packet[6]   = struGsh.nSerialCount1++;
    packet[7]   = 0;
    packet[8]   = 0;
    packet[9]   = 0;
    packet[10]  = (num-1)/256;
    packet[11]  = (num-1)%256;

    for(i=0; i<num; i++){
        for(j=0; j<FRAME_LEN; j++){
            packet[12+i*FRAME_LEN+j] = fileBuf[i][j];
        }
    }

    for (i = 0; i < num*FRAME_LEN+10; i++) {
        crcdata[i] = packet[i+2];
    }

//    crcitt = MySerial.crcFpga(crcdata, num*FRAME_LEN+10);
    packet[num*FRAME_LEN+12] = crcitt/256;
    packet[num*FRAME_LEN+13] = crcitt%256;
    packet[num*FRAME_LEN+14] = 0xff;
    packet[num*FRAME_LEN+15] = 0xff;

#if 0
    printf("\n");
    printf("usb >> ");
    for(i=0; i<500; i++){
        if (i%20 == 0) {
            printf("\n");
        }
        printf("%02x ", packet[i]);
    }
    printf("\n");
#endif

    if (struGsh.nSerialCount1 > 250) {
        struGsh.nSerialCount1 = 0;
    }

    ret = usb_bulk_read(myUsbDev, USB_EP_IN_AI, buf, num*FRAME_LEN+16, 1000);
    qDebug("MSG : read_ret=%d", ret);
    ret = usb_bulk_write(myUsbDev, USB_EP_OUT_AI, packet, num*FRAME_LEN+16, 1000);
    qDebug("MSG : write_ret = %d", ret);
#endif
}

/* 返回第一页 */
void fpga::goToFirstPageSlt()
{
    stackedWidget->setCurrentIndex(MOUNT_PAGE);
}

/* 响应当前页索引发生变化的情况 */
void fpga::onStackedWidgetIndexChangedSlt(int index)
{

//    switch(index){
//    case MOUNT_PAGE:
//        titleLabel->setText(QString("%1 > %2").arg(myLan.factory_set).arg(myLan.upgrade_fpga));
//        break;
//    case FILE_LIST_PAGE:
//        titleLabel->setText(QString("%1 > %2 > %3").arg(myLan.factory_set).arg(myLan.upgrade_fpga)
//                            .arg(myLan.upgrade_file_list));
//        break;
//    case NOTICE_PAGE:
//        titleLabel->setText(QString("%1 > %2 > %3").arg(myLan.factory_set).arg(myLan.upgrade_fpga)
//                            .arg(myLan.upgrade_usb_communication));
//        break;
//    case UPDATE_PAGE:
//        titleLabel->setText(QString("%1 > %2 > %3").arg(myLan.factory_set).arg(myLan.upgrade_fpga)
//                            .arg(myLan.upgrade_board));
//        break;
//    case WHOLE_UPDATE_PAGE:
//        titleLabel->setText(QString("%1 > %2 > %3").arg(myLan.factory_set).arg(myLan.upgrade_fpga)
//                            .arg(myLan.upgrade_whole));
//        break;
//    default:
//        break;
//    }
}
