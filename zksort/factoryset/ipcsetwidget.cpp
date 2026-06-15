#include "ipcsetwidget.h"
#include "ui_ipcsetwidget.h"
#include <QToolButton>

IpcSetWidget::IpcSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::IpcSetWidget)
{
    ui->setupUi(this);
    m_gridLayout = ui->gridLayout;
}

IpcSetWidget::~IpcSetWidget()
{
    delete ui;
}
void IpcSetWidget::showPage(bool bshow)
{
    if(bshow)
    {
        buildGridUi();
    }
}

void IpcSetWidget::buildGridUi()
{
    m_itemList.clear();
    clearLayout(m_gridLayout);

    ItemCell cellIpcCamera(0,g_myLan().camera_set,"ipcCameraSetBtn");
    ItemCell cellCameraColorRestore(1, g_myLan().ipc_colorRestore,"cameraColorRestoreBtn");
    ItemCell cellIpcAlarmControl(2,g_myLan().alarm_method ,"ipcAlarmControlBtn");
    ItemCell cellFastImgCap(3,g_myLan().HSCapture,"fastImgCapBtn");
//    ItemCell cellPixel(4,g_myLan().ids_devidePixel,"pixelBtn");
//    ItemCell cellUpdateSelf(5,g_myLan().upgrade_screen,"upgradeScreenBtn");
//    ItemCell cellUpdateFpga(6,g_myLan().upgrade_fpga,"upgradeFpgaBtn");
//    ItemCell cellMachineFunc(7,g_myLan().system_misc_set,"systemMiscSetBtn");
//    ItemCell cellLampBoard(8,g_myLan().ids_ccsBoard,"lampBoardSetBtn");

    m_itemList.append(cellIpcCamera);
    m_itemList.append(cellCameraColorRestore);
    m_itemList.append(cellIpcAlarmControl);
    m_itemList.append(cellFastImgCap);
//    m_itemList.append(cellCamera);
//    m_itemList.append(cellBackGround);
//    m_itemList.append(cellPixel);
//    m_itemList.append(cellUpdateSelf);
//    m_itemList.append(cellUpdateFpga);
//    m_itemList.append(cellMachineFunc);
//    m_itemList.append(cellLampBoard);

    for(int i=0;i<m_itemList.count();i++)
    {
         QToolButton *toolBtn = new QToolButton;

         toolBtn->setText(m_itemList.at(i).itemText);
         toolBtn->setObjectName(m_itemList.at(i).itemObjName);
         toolBtn->setWhatsThis(QString::number(m_itemList.at(i).itemId));
//         toolBtn->setToolTip(m_itemList.at(i).itemObjName);
         toolBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
         toolBtn->setFocusPolicy(Qt::NoFocus);


         connect(toolBtn,SIGNAL(clicked()),this,SLOT(toolBtnClick()));
         int column = i%4;
         int row = i/4;
         m_gridLayout->addWidget(toolBtn,row,column);

    }

}

void IpcSetWidget::toolBtnClick()
{
    QToolButton *clickBtn = qobject_cast<QToolButton *>(sender());

    int itemId = clickBtn->whatsThis().toInt();

    switch(itemId)
    {
    case 0:
        logger()->info("enter ipc camera setting page");
        if(struCnfg.nChuteTotal <= 12)
        g_MainManager().ShowWidget(SM_IPC_CAMERA_SET_WIDGET);
        break;
    case 1:
        logger()->info("enter color restore page");
        g_MainManager().ShowWidget(SM_CAMERA_COLOR_RESTORE_WIDGET);
        break;
    case 2:
        logger()->info("enter ipc alarm control page");
        g_MainManager().ShowWidget(SM_IPC_ALARM_CONTROL_WIDGET);
        break;
    case 3:
        logger()->info("enter fast capture page");
        g_MainManager().ShowWidget(SM_IMAGE_FAST_CAPTURE_WIDGET);
        break;
//    case 4:
//        logger()->info("enter dividing pixels page");
//        g_MainManager().ShowWidget(SM_DIVIDE_PIXEL_WIDGET);
//        break;
//    case 5:
//        logger()->info("enter upgrading lcd software page");
//        onUpdateMainClicked();
//        break;
//    case 6:
//        onFpgaUpgradeClicked();
//        logger()->info("enter upgrading fpga software page");
//        g_MainManager().ShowWidget(SM_UPDATE_FPGA_WDIGET);
//        break;
//    case 7:
//        if(struGsh.bRunMode == RUNMODE_ADMIN){
//            logger()->info("enter misc function of machine page");
//            g_MainManager().ShowWidget(SM_MACHINE_FUNC_WIDGET);
//            break;
//        }
//#ifdef Q_OS_UNIX
//        inputPanel.setType(KB_PD);
//        inputPanel.setInputBtnText(g_myLan().ids_ok,g_myLan().ids_cancel,g_myLan().ids_delete,g_myLan().ids_space);
//        if(inputPanel.exec() == QDialog::Accepted)
//        {
//            QString str = inputPanel.getText();
//            if (str == "XXXXXX")
//            {
//                struGsh.bRunMode = RUNMODE_ADMIN;
//                struGsh.bFlagAutowipe = 0;
//                logger()->info("enter misc function of machine page");
//                g_MainManager().ShowWidget(SM_MACHINE_FUNC_WIDGET);
//            }
//            else
//            {
//                g_infoWidget().setLabelText(g_myLan().ids_wrongPassword);
//                g_infoWidget().delayShow();
//                myFlow.sleep(2);
//                g_infoWidget().hide();
//            }
//        }
//#else
//        struGsh.bRunMode = RUNMODE_ADMIN;
//        struGsh.bFlagAutowipe = 0;
//        logger()->info("enter misc function of machine page");
//        g_MainManager().ShowWidget(SM_MACHINE_FUNC_WIDGET);
//#endif
//        break;
//    case 8:
//        logger()->info("enter setting lamp board page");
//        g_MainManager().ShowWidget(SM_LAMP_BOARD_SET_WIDGET);
//        break;
    }

}
//void IpcSetWidget::onUpdateMainClicked()
//{
//    myMessageBox *mesBox = new myMessageBox(MSG_QUES, g_myLan().cfm_upgrade_screen);
//    int ret = mesBox->exec();
//    if(ret == QDialog::Accepted){
//        QProcess *process = new QProcess;
//        process->startDetached("./MyUpdate.sh");
//        system("/app/kmy.sh");
//    }
//}


void IpcSetWidget::clearLayout(QLayout *_layout)
{
    QLayoutItem *item;
    while((item = _layout->takeAt(0)))
    {
        if(item->widget())
        {
            delete item->widget();
        }
        QLayout *childLayout = item->layout();
        if(childLayout)
        {
            clearLayout(childLayout);
        }
        delete item;
    }
}

void IpcSetWidget::retranslateUiWidget()
{

}
