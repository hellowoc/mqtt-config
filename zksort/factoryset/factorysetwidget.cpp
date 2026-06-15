#include "factorysetwidget.h"
#include "ui_factorysetwidget.h"
#include <QToolButton>
#include "global.h"
#include "fpga.h"

FactorySetWidget::FactorySetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::FactorySetWidget)
{
    ui->setupUi(this);
    m_gridLayout = ui->gridLayout;
}

FactorySetWidget::~FactorySetWidget()
{
    delete ui;
}

void FactorySetWidget::showPage(bool bshow)
{
    if(bshow)
    {
        buildGridUi();
    }
}

void FactorySetWidget::buildGridUi()
{
    m_itemList.clear();
    clearLayout(m_gridLayout);

    ItemCell cellLanguage(0,g_myLan().language,"languageBtn");
    ItemCell cellMachine(1,g_myLan().ids_machineModel,"machineSetBtn");
    ItemCell cellCamera(2,g_myLan().camera_set,"cameraSetBtn");
    ItemCell cellBackGround(3,g_myLan().signal_set,"signalSetBtn");
    ItemCell cellPixel(4,g_myLan().ids_devidePixel,"pixelBtn");
//    ItemCell cellIpcSet(5,g_myLan().ids_ipc+g_myLan().ids_set,"IpcSetBtn");

    ItemCell cellIpcSet(5,"IPC","IpcSetBtn");
    ItemCell cellUpdateFpga(6,g_myLan().ids_upgrade,"upgradeFpgaBtn");
    ItemCell cellMachineFunc(7,g_myLan().system_misc_set,"systemMiscSetBtn");
    ItemCell cellLampBoard(8,g_myLan().ids_ccsBoard,"lampBoardSetBtn");

    m_itemList.append(cellLanguage);
    m_itemList.append(cellMachine);
    m_itemList.append(cellCamera);
    m_itemList.append(cellBackGround);
    m_itemList.append(cellPixel);
    m_itemList.append(cellIpcSet);
    m_itemList.append(cellUpdateFpga);
    m_itemList.append(cellMachineFunc);
    m_itemList.append(cellLampBoard);

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
         int column = i%3;
         int row = i/3;
         m_gridLayout->addWidget(toolBtn,row,column);

    }

}

void FactorySetWidget::toolBtnClick()
{
    QToolButton *clickBtn = qobject_cast<QToolButton *>(sender());

    myInputMethod inputPanel;
    int itemId = clickBtn->whatsThis().toInt();

    switch(itemId)
    {
    case 0:
#ifdef Q_OS_UNIX
        if(paramDelayCode.uiLanguage != 0xAA)
        {
            return;
        }
#endif
        logger()->info("enter language page");
        g_MainManager().ShowWidget(SM_LANGUAGE_WIDGET);
        break;
    case 1:
        logger()->info("enter setting machine page");
        g_MainManager().ShowWidget(SM_MACHINE_SET_WIDGET);
        break;
    case 2:
        logger()->info("enter setting camera page");
        g_MainManager().ShowWidget(SM_CAMERA_SET_WIDGET);
        break;
    case 3:
        logger()->info("enter camera signal page");
        g_MainManager().ShowWidget(SM_CAMERA_SIGNAL_WIDGET);
        break;
    case 4:
        logger()->info("enter dividing pixels page");
        g_MainManager().ShowWidget(SM_DIVIDE_PIXEL_WIDGET);
        break;
    case 5:
        if(struCnfg.nDPMachine){
            logger()->info("enter ipc setting page");
            g_MainManager().ShowWidget(SM_IPC_SET_WIDGET);
        }
        break;
    case 6:
//        onFpgaUpgradeClicked();
        logger()->info("enter upgrading fpga software page");
        g_MainManager().ShowWidget(SM_UPDATE_FPGA_WDIGET);
        break;
    case 7:
        if(struGsh.bRunMode == RUNMODE_ADMIN){
            logger()->info("enter misc function of machine page");
            g_MainManager().ShowWidget(SM_MACHINE_FUNC_WIDGET);
            break;
        }
#ifdef Q_OS_UNIX
        inputPanel.setType(KB_PD);
        inputPanel.setInputBtnText(g_myLan().ids_ok,g_myLan().ids_cancel,g_myLan().ids_delete,g_myLan().ids_space);
        if(inputPanel.exec() == QDialog::Accepted)
        {
            QString str = inputPanel.getText();
            if ((PRODUCT_DEFAULT_VENDOR == PRODUCT_VENDOR_ZK && (str == "ZK603011" || str.contains("XXXX")))||
                    (PRODUCT_DEFAULT_VENDOR == PRODUCT_VENDOR_LV && str == "LV.123.321.SYS"))
            {
                struGsh.bRunMode = RUNMODE_ADMIN;
                logger()->info("enter misc function of machine page");
                g_MainManager().ShowWidget(SM_MACHINE_FUNC_WIDGET);
            }
            else
            {
                g_infoWidget().setLabelText(g_myLan().ids_wrongPassword);
                g_infoWidget().delayShow();
                myFlow.sleep(2);
                g_infoWidget().hide();
            }
        }
#else
        struGsh.bRunMode = RUNMODE_ADMIN;
        struGsh.bFlagAutowipe = 0;
        logger()->info("enter misc function of machine page");
        g_MainManager().ShowWidget(SM_MACHINE_FUNC_WIDGET);
#endif
        break;
    case 8:
        logger()->info("enter setting lamp board page");
        g_MainManager().ShowWidget(SM_LAMP_BOARD_SET_WIDGET);
        break;
    }

}
void FactorySetWidget::onUpdateMainClicked()
{
    myMessageBox *mesBox = new myMessageBox(MSG_QUES, g_myLan().cfm_upgrade_screen);
    int stat = -1;
    int ret = mesBox->exec();
    if(ret == QDialog::Accepted){   
#ifdef Q_OS_UNIX
        myFlow.resetMemoryDetection();

        QFile myfile;
//        if(myfile.exists("/dev/mmcblk1")) //sd存在
//        {
//            if(myfile.exists("/sdcard/3-app.bin")) //从sd卡启动
//            {
//                stat = (struGsh.struMemoryStatus.nSDStatus[1] & struGsh.struMemoryStatus.nSDStatus[0]);
//            }
//            else    //从emmc启动
//            {
//                stat = (struGsh.struMemoryStatus.nEMMCStatus[2] & struGsh.struMemoryStatus.nEMMCStatus[1] &struGsh.struMemoryStatus.nEMMCStatus[0]);
//            }
//        }
//        else //sd卡不存在
//        {
//            stat = (struGsh.struMemoryStatus.nEMMCStatus[2] & struGsh.struMemoryStatus.nEMMCStatus[1] &struGsh.struMemoryStatus.nEMMCStatus[0]);
//        }

        if (!stat){
            g_infoWidget().setLabelText(g_myLan().pls_reboot_normaly);
            g_infoWidget().delayShow();
            myFlow.sleep(2);
            g_infoWidget().hide();
            return;
        }
#endif

        struGsh.poweroff_status = 1;
        QSettings setting(CFG_APPSet,QSettings::IniFormat);
        setting.setValue("poweroffstatus",struGsh.poweroff_status);
        setting.sync();

        delete mesBox;
        mesBox = nullptr;
        this->close();
        myFlow.msleep(100);
        QProcess::startDetached("/app/MyUpdate.sh");
        qApp->quit();
    }
}

void FactorySetWidget::onFpgaUpgradeClicked()
{
//    myMessageBox *mesBox = new myMessageBox(MSG_QUES, g_myLan().cfm_upgrade_fpga);
//    int ret = mesBox->exec();
//    if(ret == QDialog::Accepted){

//        fpga *myFpga = new fpga;
//        myFpga->exec();

//    }
}

void FactorySetWidget::clearLayout(QLayout *_layout)
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

void FactorySetWidget::retranslateUiWidget()
{

}
