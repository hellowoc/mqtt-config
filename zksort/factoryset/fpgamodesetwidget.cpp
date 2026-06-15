#include "fpgamodesetwidget.h"
#include "ui_fpgamodesetwidget.h"
#include <QRegExp>


FpgaModeSetWidget::FpgaModeSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::FpgaModeSetWidget)
{
    ui->setupUi(this);
    m_count = 0;
    m_nBoardTypeIndex = 1;
    m_nChainIndex = 0;
    m_nChuteIndex = 0;
    m_nMirrorIndex= 0;
    m_view = 0;

    viewList<<QString(g_myLan().lower_layer+g_myLan().ids_fm) <<QString(g_myLan().lower_layer+g_myLan().ids_bm)<<QString(g_myLan().lower_layer+g_myLan().ids_fs)<<QString(g_myLan().lower_layer+g_myLan().ids_bs)
           <<QString(g_myLan().upper_layer+g_myLan().ids_fm) <<QString(g_myLan().upper_layer+g_myLan().ids_bm)<<QString(g_myLan().upper_layer+g_myLan().ids_fs)<<QString(g_myLan().upper_layer+g_myLan().ids_bs);
    viewMap[board_camera] = "";
    viewMap[board_ipc_camera]= "IPC";
    initUi();

}

FpgaModeSetWidget::~FpgaModeSetWidget()
{
    delete ui;
}

void FpgaModeSetWidget::showPage(bool bshow)
{
    if(bshow){
        memcpy(&m_temCnf,&struCnfp,sizeof(struCnfProfile));

        viewList.clear();
        if (struCnfg.nMachineType == MACHINE_LD2_ZL_GP){
            viewList<<QString(g_myLan().lower_layer+g_myLan().ids_fm) <<QString(g_myLan().lower_layer+g_myLan().ids_bm)<<QString(g_myLan().lower_layer+g_myLan().ids_fs)<<QString(g_myLan().lower_layer+g_myLan().ids_bs)
                   <<QString(g_myLan().upper_layer+g_myLan().ids_fm) <<QString(g_myLan().upper_layer+g_myLan().ids_bm)<<QString(g_myLan().upper_layer+g_myLan().ids_fs)<<QString(g_myLan().upper_layer+g_myLan().ids_bs);
        }
        else{
            viewList<<g_myLan().ids_fm <<g_myLan().ids_bm<<g_myLan().ids_fs<<g_myLan().ids_bs
                   <<g_myLan().ids_fm <<g_myLan().ids_bm<<g_myLan().ids_fs<<g_myLan().ids_bs;

        }
        struGsh.bUpdateState = 1;
        updateUi();

    }
    else
    {
        struGsh.bUpdateState = 0;
        memcpy(&struCnfp,&m_temCnf,sizeof(struCnfProfile));
    }

}

void FpgaModeSetWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){
        updateUi();
    }

}



void FpgaModeSetWidget::retranslateUiWidget()
{
    updateUi();
    ui->label_board->setText(g_myLan().ids_board);
    ui->label_chain->setText(g_myLan().ids_chain);
    ui->label_chute->setText(g_myLan().ids_channel);
    ui->label_mode->setText(g_myLan().ids_model);
    ui->startUpgradeBtn->setText(g_myLan().change_mode);
}


void FpgaModeSetWidget::on_startUpgradeBtn_clicked()
{
    qDebug()<<m_nBoardTypeIndex<<m_nChainIndex<<m_nChuteIndex<<m_nMirrorIndex;
    myMessageBox *mesBox = new myMessageBox(MSG_QUES, g_myLan().ChangeUserMode+"?");
    int ret = mesBox->exec();
    if(ret == QDialog::Accepted)
    {
        // 切换到工厂
        if(m_nMirrorIndex == 2)
        {
            myFlow.switchUserMirror(board_camera<<4|FACTORY_MODE,myFlow.getBoardAddr(board_camera,m_nChainIndex),m_nChuteIndex,0);
            myFlow.sleep(6);
            if(m_nChainIndex  == 0) // 整机
            {
                int k;
                for(int i = 0; i < MAX_VIEW ; i++)
                {
                    if(struCnfg.struLayerInfo[0].nViewIsUsed[i] == 0)
                        continue;

                    for (int m =0;m<struCnfg.struLayerInfo[0].nViewUnitTotal[i];m++)
                    {
                        myFlow.readVersion(myFlow.getBoardAddr(board_camera,i+1), m+1);
                        for(k = 0; k < 20; k++)
                        {
                            myFlow.msleep(500);
                            if(myFlow.boardSwitchFactory(board_camera,i+1,m))
                            {
                                break;
                            }
                        }
                        if(k == 20)
                        {
                            g_infoWidget().setLabelText(g_myLan().ids_setFactoryFailed);
                            g_infoWidget().showSecs();
                            return;
                        }
                    }
                }
                g_infoWidget().setLabelText(g_myLan().ids_setFactorySucceeded);
                g_infoWidget().showSecs();
                return;

            }
            else
            {
                if(m_nChuteIndex == 0) // 全通道
                {
                    int k;
                    for (int m =0;m<struCnfg.struLayerInfo[0].nViewUnitTotal[m_nChainIndex-1];m++)
                    {
                        myFlow.readVersion(myFlow.getBoardAddr(board_camera,m_nChainIndex), m+1);
                        for(k = 0; k < 20; k++)
                        {
                            myFlow.msleep(500);
                            if(myFlow.boardSwitchFactory(board_camera,m_nChainIndex,m))
                            {
                                break;
                            }
                        }
                        if(k == 10)
                        {
                            g_infoWidget().setLabelText(g_myLan().ids_setFactoryFailed);
                            g_infoWidget().showSecs();
                            return;
                        }
                    }
                    g_infoWidget().setLabelText(g_myLan().ids_setFactorySucceeded);
                    g_infoWidget().showSecs();
                    return;
                }
                else
                {
                    myFlow.readVersion(myFlow.getBoardAddr(board_camera,m_nChainIndex), m_nChuteIndex);
                    int k;
                    for(k = 0; k < 10; k++)
                    {
                        myFlow.msleep(500);
                        if(myFlow.boardSwitchFactory(board_camera,m_nChainIndex,m_nChuteIndex-1))
                        {
                            break;
                        }
                    }
                    if(k == 10)
                    {
                        g_infoWidget().setLabelText(g_myLan().ids_setFactoryFailed);
                        g_infoWidget().showSecs();
                        return;
                    }
                    g_infoWidget().setLabelText(g_myLan().ids_setFactorySucceeded);
                    g_infoWidget().showSecs();
                    return;
                }
            }
            return;
        }

        if(m_nChainIndex == 0)
        {
            for(int i = 0 ; i < MAX_VIEW ; i++)
            {
                for(int j = 0 ; j < struCnfg.struLayerInfo[0].nViewUnitTotal[i]; j++)
                {
                    struCnfp.nCameraBoardUserMode[0][i][j] = m_nMirrorIndex+1;
                }
            }

        }
        else
        {
            if(m_nChuteIndex == 0)
            {
                for(int j = 0 ; j < struCnfg.struLayerInfo[0].nViewUnitTotal[m_nChainIndex-1]; j++)
                {
                    struCnfp.nCameraBoardUserMode[0][m_nChainIndex-1][j] = m_nMirrorIndex+1;
                }
            }
            else
            {
                struCnfp.nCameraBoardUserMode[0][m_nChainIndex-1][m_nChuteIndex-1] = m_nMirrorIndex+1;
            }
        }

        g_infoWidget().setLabelText(g_myLan().msg_mode_changing);
        g_infoWidget().delayShow();
        myFlow.SwitchUserMode(m_nChainIndex , m_nChuteIndex); // 切换用户模式
        myFlow.sleep(6);
        // 整机
        if(m_nChainIndex == 0)
        {
            int k;
            for(int  view = 0; view < MAX_VIEW ; view++)
            {
                if(struCnfg.struLayerInfo[0].nViewIsUsed[view] == 0)
                    continue;

                for (int m =0;m<struCnfg.struLayerInfo[0].nViewUnitTotal[view];m++)
                {
                    myFlow.readVersion(myFlow.getBoardAddr(board_camera,view+1), m+1);
                    for(k = 0; k < 20; k++)
                    {
                        myFlow.msleep(500);
                        if(myFlow.boardUpdateSuccess(board_camera,view+1,m,0))
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
            }
        }
        else  // 单链
        {
            int k;
            if(m_nChuteIndex == 0) // 全通道
            {
                for (int m =0;m<struCnfg.struLayerInfo[0].nViewUnitTotal[m_nChainIndex -1];m++)
                {
                    myFlow.readVersion(myFlow.getBoardAddr(board_camera,m_nChainIndex), m+1);
                    for(k = 0; k < 20; k++)
                    {
                        myFlow.msleep(500);
                        if(myFlow.boardUpdateSuccess(board_camera,m_nChainIndex,m,0))
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
            }
            else    // 单通道
            {

                myFlow.readVersion(myFlow.getBoardAddr(board_camera,m_nChainIndex), m_nChuteIndex);

                for(k = 0; k < 20; k++)
                {
                    myFlow.msleep(500);
                    if(myFlow.boardUpdateSuccess(board_camera,m_nChainIndex,m_nChuteIndex-1,0))
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
        }
    }

    g_infoWidget().setLabelText(g_myLan().ids_setUserModeSucceeded);
    g_infoWidget().showSecs();

}



void FpgaModeSetWidget::updateChuteCombobox()
{
    QStringList list;
    list<<g_myLan().ids_allChannel;

    for(int i = 1;i< struCnfg.nChuteTotal+1;i++){
        list<<QString::number(i);
    }
    ui->comboBox_chute->clear();
    ui->comboBox_chute->addItems(list);
    ui->comboBox_chute->setCurrentIndex(m_nChuteIndex);
}

void FpgaModeSetWidget::updateModeCombobox()
{
    QStringList list;
    list<<g_myLan().mode_1<<g_myLan().mode_2<<g_myLan().factory_set;
    ui->comboBox_mode->clear();
    ui->comboBox_mode->addItems(list);
    ui->comboBox_mode->setCurrentIndex(m_nMirrorIndex);
}
void FpgaModeSetWidget::updateElecBoardCombobox()
{
    QStringList list;
    list<<g_myLan().ids_interfaceBoard<<g_myLan().ids_camera<<"IPC"+g_myLan().ids_camera<<g_myLan().ids_ipc
       <<g_myLan().ids_valveBoard<<g_myLan().ids_controlBoard<<g_myLan().ids_ccsBoard<<g_myLan().ids_accelerationBoard<<g_myLan().ids_halogenBoard;
    ui->comboBox_board->clear();
    ui->comboBox_board->addItems(list);
    ui->comboBox_board->setCurrentIndex(m_nBoardTypeIndex);
    ui->comboBox_board->setEnabled(false);
}

void FpgaModeSetWidget::updateUi()
{
    updateElecBoardCombobox();
    updateChainCombobox();
    updateChuteCombobox();
    updateModeCombobox();
    updateParamSet();
}

void FpgaModeSetWidget::updateParamSet()
{
    for(int i=0; i<16; i++){

        if(i< struCnfg.fpgaParamsNum)
        {
            qlabel[i]->show();
            m_pParamEdit[i]->show();
            qlabel[i]->setText(QString::number(i+1));

            m_pParamEdit[i]->setText(QString::number(struGsh.fpgaParmsforDebug[i]));
            m_pParamEdit[i]->setMinMax(this, 0, 255, 2,&struGsh.fpgaParmsforDebug[i]);

        }
        else
        {
            qlabel[i]->hide();
            m_pParamEdit[i]->hide();
        }
    }
}

void FpgaModeSetWidget::initUi()
{

    qlabel[0] = ui->label;
    qlabel[1] = ui->label_2;
    qlabel[2] = ui->label_3;
    qlabel[3] = ui->label_4;
    qlabel[4] = ui->label_5;
    qlabel[5] = ui->label_6;
    qlabel[6] = ui->label_7;
    qlabel[7] = ui->label_8;
    qlabel[8] = ui->label_9;
    qlabel[9] = ui->label_10;
    qlabel[10] = ui->label_11;
    qlabel[11] = ui->label_12;
    qlabel[12] = ui->label_13;
    qlabel[13] = ui->label_14;
    qlabel[14] = ui->label_15;
    qlabel[15] = ui->label_16;

    m_pParamEdit[0] = ui->lineEdit;
    m_pParamEdit[1] = ui->lineEdit_2;
    m_pParamEdit[2] = ui->lineEdit_3;
    m_pParamEdit[3] = ui->lineEdit_4;
    m_pParamEdit[4] = ui->lineEdit_5;
    m_pParamEdit[5] = ui->lineEdit_6;
    m_pParamEdit[6] = ui->lineEdit_7;
    m_pParamEdit[7] = ui->lineEdit_8;
    m_pParamEdit[8] = ui->lineEdit_9;
    m_pParamEdit[9] = ui->lineEdit_10;
    m_pParamEdit[10] = ui->lineEdit_11;
    m_pParamEdit[11] = ui->lineEdit_12;
    m_pParamEdit[12] = ui->lineEdit_13;
    m_pParamEdit[13] = ui->lineEdit_14;
    m_pParamEdit[14] = ui->lineEdit_15;
    m_pParamEdit[15] = ui->lineEdit_16;

    ui->paramsnum->setTitle("");
    ui->paramsnum->setValue(struCnfg.fpgaParamsNum);
    ui->paramsnum->setParams(this,1,64,0,&struCnfg.fpgaParamsNum);
    //    const int totalGroups = 64;       //
    //    const int groupsPerRow = 4;       // 每行4组
    //    int currentRow = 0;               // 当前行
    //    int currentCol = 0;               // 当前列
    //    QGridLayout* mainGridLayout = new QGridLayout();
    //    mainGridLayout->setSpacing(15);    // 组与组之间的间距

    //    for(int i=0; i<totalGroups; i++){
    //        QHBoxLayout* layout = new QHBoxLayout();
    //        layout->setSpacing(10);

    //        QLabel* Label = new QLabel();
    //        Label->setText(QString::number(i));
    //        layout->addWidget(Label);
    //        qlabel[i] = Label;

    //        myLineEdit* classNameEdit = new myLineEdit();
    //        layout->addWidget(classNameEdit);
    //        m_pParamEdit[i] = classNameEdit;

    //        ui->scrollAreaWidgetContents->addLayout(layout,currentRow,currentCol);

    //        currentCol++;
    //        if(currentCol >= groupsPerRow)
    //        {
    //            currentCol = 0;
    //            currentRow++;

    //        }

    //    }
    //ui->scrollAreaWidgetContents->setLayout(mainGridLayout);

}
void FpgaModeSetWidget::updateChainCombobox()
{
    chainList.clear();
    QStringList list;
    list<<g_myLan().front_rear_view;
    for(int i = 0;i< struCnfg.struLayerInfo[0].nViewChainTotal;i++){
        if(struCnfg.struLayerInfo[0].nViewIsUsed[i])
        {
            chainList<<QString::number(i);
            if (struCnfg.nMachineType == MACHINE_LD2_ZL_GP){
                switch (i) {
                case 0:
                    list<<g_myLan().lower_layer+g_myLan().ids_fm;
                    break;
                case 1:
                    list<<g_myLan().lower_layer+g_myLan().ids_bm;
                    break;
                case 2:
                    list<<g_myLan().lower_layer+g_myLan().ids_fs;
                    break;
                case 3:
                    list<<g_myLan().lower_layer+g_myLan().ids_bs;
                    break;
                case 4:
                    list<<g_myLan().upper_layer+g_myLan().ids_fm;
                    break;
                case 5:
                    list<<g_myLan().upper_layer+g_myLan().ids_bm;
                    break;
                case 6:
                    list<<g_myLan().upper_layer+g_myLan().ids_fs;
                    break;
                case 7:
                    list<<g_myLan().upper_layer+g_myLan().ids_bs;
                    break;
                default:
                    break;
                }
            }
            else{
                switch (i) {
                case 0:
                    list<<g_myLan().ids_fm;
                    break;
                case 1:
                    list<<g_myLan().ids_bm;
                    break;
                case 2:
                    list<<g_myLan().ids_fs;
                    break;
                case 3:
                    list<<g_myLan().ids_bs;
                    break;
                default:
                    break;
                }
            }
        }
    }
    ui->comboBox_chain->clear();
    ui->comboBox_chain->addItems(list);
    ui->comboBox_chain->setCurrentIndex(m_nChainIndex);

    if (m_nChainIndex == 0)
    {
        ui->comboBox_chute->setCurrentIndex(0);
        ui->comboBox_chute->setEnabled(false);
    }
}



//bool FpgaModeSetWidget::boardSwitchFactory(int boardIndex,int chainIndex, int chuteIndex)
//{
//    switch (boardIndex) {
//    case board_interface:
//        if(struGsh.struVer.nInterfaceRes == 1 && struGsh.struVer.sInterface[2] == FACTORY_MODE)
//            return true;
//        break;
//    case board_camera:
//        if(struGsh.struVer.nUnitRes[0][chainIndex-1][chuteIndex] == 1 && struGsh.struVer.sUnit[0][chainIndex-1][chuteIndex][0] != 0 && struGsh.struVer.sUnit[0][chainIndex-1][chuteIndex][2] == FACTORY_MODE)
//            return true;
//        break;
//    case board_ipc_camera:
//        if(struGsh.struVer.nUnitRes[0][chainIndex-1][chuteIndex] == 1 && struGsh.struVer.sUnitIpc[0][chainIndex-1][chuteIndex][0] != 0 && struGsh.struVer.sUnitIpc[0][chainIndex-1][chuteIndex][2] == FACTORY_MODE)
//            return true;
//        break;
//    case board_valve:
//        if(struGsh.struVer.nVavleRes[0][chainIndex-1][chuteIndex] == 1 && struGsh.struVer.sVavle[0][chainIndex-1][chuteIndex][0] != 0 && struGsh.struVer.sVavle[0][chainIndex-1][chuteIndex][2] == FACTORY_MODE)
//            return true;
//        break;
//    case board_control:
//        if(struGsh.struVer.nCtrlRes[chuteIndex] == 1 && struGsh.struVer.sCtrlBoard[chuteIndex][0] != 0 && struGsh.struVer.sCtrlBoard[chuteIndex][2] == FACTORY_MODE)
//            return true;
//        break;
//    case board_lamp:
//        if(struGsh.struVer.nLampRes[chuteIndex] == 1 && struGsh.struVer.sLampBoard[chuteIndex][0] != 0 && struGsh.struVer.sLampBoard[chuteIndex][2] == FACTORY_MODE)
//            return true;
//        break;
//    case board_control_zd:
//        if(struGsh.struVer.nAccelerationRes[chuteIndex] == 1 && struGsh.struVer.sAccelerationBoard[chuteIndex][0] != 0 && struGsh.struVer.sAccelerationBoard[chuteIndex][2] == FACTORY_MODE)
//            return true;
//        break;
//    case board_control_dl:
//        if(struGsh.struVer.nHalogenRes[chuteIndex] == 1 && struGsh.struVer.sAccelerationBoard[chuteIndex][0] != 0 && struGsh.struVer.sAccelerationBoard[chuteIndex][2] == FACTORY_MODE)
//            return true;
//        break;
//    default:
//        return false;
//        break;
//    }
//    return false;
//}

void FpgaModeSetWidget::on_comboBox_board_activated(int index)
{

    m_nBoardTypeIndex = index;
    ui->comboBox_board->setCurrentIndex(m_nBoardTypeIndex);
}

void FpgaModeSetWidget::on_comboBox_chute_activated(int index)
{
    m_nChuteIndex = index;
}

void FpgaModeSetWidget::on_comboBox_chain_activated(int index)
{
    m_nChainIndex = index;
    if (m_nChainIndex == 0)
    {
        m_nChuteIndex = 0;
        ui->comboBox_chute->setCurrentIndex(0);
        ui->comboBox_chute->setEnabled(false);


    }
    else
    {
        ui->comboBox_chute->setEnabled(true);
    }
    on_comboBox_chute_activated(0);
    updateChuteCombobox();
}

void FpgaModeSetWidget::on_comboBox_mode_activated(int index)
{
    m_nMirrorIndex = index;
}





void FpgaModeSetWidget::on_pushButton_clicked()
{
    g_infoWidget().setLabelText(g_myLan().msg_applying);
    g_infoWidget().delayShow();
    myFlow.setFpgaRegisterStatus(m_nBoardTypeIndex,m_nChainIndex,m_nChuteIndex);
    myFlow.sleep(1);
    g_infoWidget().hide();
}

void FpgaModeSetWidget::on_pushButton_2_clicked()
{
    // memset(fpgaDebugData , 0 ,sizeof(fpgaDebugData))


    g_infoWidget().setLabelText(g_myLan().msg_applying);
    g_infoWidget().delayShow();
    myFlow.getFpgaRegisterStatus(m_nBoardTypeIndex,m_nChainIndex,m_nChuteIndex);
    myFlow.sleep(2);
    updateDebugdata();
    g_infoWidget().hide();



}

void FpgaModeSetWidget::on_pushButton_3_clicked()
{
    fpgadebugdata.clear();
    updateDebugdata();
}

void FpgaModeSetWidget::on_pushButton_4_clicked()
{

}


void FpgaModeSetWidget::updateDebugdata()
{

    ui->textEdit->clear(); // 清空控件

    for (int i = 0; i < fpgadebugdata.size(); ++i)
    {
        const QVector<char> &line = fpgadebugdata.at(i);
        // 转为16进制大写字符串（带空格分隔，易读）
        QByteArray ba = QByteArray(line.constData(), line.size());
        QString hexStr = ba.toHex().toUpper();

        // 插入空格格式化（可选）
        for (int j = 2; j < hexStr.length(); j += 3)
        {
            hexStr.insert(j, " ");
        }

        QString showStr = QString("%1：%2").arg(i+1).arg(hexStr);
        ui->textEdit->append(showStr);
    }
}

void FpgaModeSetWidget::on_pushButton_5_clicked()
{
    g_infoWidget().setLabelText(g_myLan().msg_applying);
    g_infoWidget().delayShow();

    myFlow.getFpgaCameraArith(m_nBoardTypeIndex, m_nChainIndex, m_nChuteIndex);
    myFlow.sleep(2);
    updateDebugdata();
    g_infoWidget().hide();
}
