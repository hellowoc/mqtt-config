#include "sysstatewidget.h"
#include "ui_sysstatewidget.h"
#include <QButtonGroup>

SysStateWidget::SysStateWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::SysStateWidget)
{
    ui->setupUi(this);

    m_pbtnGroup = new QButtonGroup(this);
    m_pbtnGroup->addButton(ui->btn_system_info,btn_system_info);
    m_pbtnGroup->addButton(ui->btn_alarm_set,btn_alarm_set);
    m_pbtnGroup->addButton(ui->btn_sw_version,btn_sw_version);
    m_pbtnGroup->addButton(ui->btn_device_info,btn_device_info);
    m_pbtnGroup->addButton(ui->btn_remote_help,btn_remote_help);
    m_pbtnGroup->addButton(ui->btn_iPCInfo,btn_iPCInfo);
    m_pbtnGroup->addButton(ui->btn_iPCClassInfo,btn_iPCClassInfo);
    m_pbtnGroup->addButton(ui->btn_viBoardInfo,btn_viBoardInfo);
    m_pbtnGroup->addButton(ui->btn_wifi_set,btn_wifi_set);
    m_pbtnGroup->addButton(ui->btn_camcali_info,btn_camcali_info);

    for(int i = 0;i< Sys_State_Index;i++){
        m_pbtnGroup->button(i)->setCheckable(true);
        connect(m_pbtnGroup->button(i),SIGNAL(clicked(bool)),this,SLOT(btn_clicked(bool)));
    }
}

SysStateWidget::~SysStateWidget()
{
    delete ui;
}

void SysStateWidget::showPage(bool bshow)
{
    if(bshow){
        m_pbtnGroup->button(btn_iPCInfo)->setVisible(struCnfg.nDPMachine);
        m_pbtnGroup->button(btn_iPCClassInfo)->setVisible(struCnfg.nDPMachine);

        ui->stackedWidget->setCurrentIndex(0);
        m_pbtnGroup->button(0)->setChecked(true);

        if( struGsh.bScreenShotStatus ==1)
        {

            basewidget *widget =(basewidget*) ui->stackedWidget->currentWidget();
            widget->showPage(false);
            ui->stackedWidget->setCurrentIndex(2);
            m_pbtnGroup->button(2)->setChecked(true);
        }

        basewidget *widget =(basewidget*) ui->stackedWidget->currentWidget();
        widget->showPage();

    }else{
        for(int i = 0;i< ui->stackedWidget->count();i++){
            basewidget *widget =(basewidget*) ui->stackedWidget->widget(i);
            widget->showPage(false);
        }
        g_MainManager().SetBottomEnable(true);
    }
}

void SysStateWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_VERSION){
        ui->page_2->receiveMsg(msg1,msg2,msg3);
    }
}

void SysStateWidget::retranslateUiWidget()
{
    for(int i = 0;i< ui->stackedWidget->count();i++){
        basewidget *widget =(basewidget*) ui->stackedWidget->widget(i);
        widget->retranslateUiWidget();
    }
    ui->btn_system_info->setText(g_myLan().system_stat);
    ui->btn_iPCInfo->setText(g_myLan().ipc_info);
    ui->btn_alarm_set->setText(g_myLan().ids_alarm);
    ui->btn_sw_version->setText(g_myLan().version_info);
    ui->btn_device_info->setText(g_myLan().machine_info);
    ui->btn_remote_help->setText(g_myLan().remote_control);
    ui->btn_iPCClassInfo->setText(g_myLan().ipc_model+g_myLan().classType);
    ui->btn_viBoardInfo->setText("V-I"+g_myLan().ids_board);
    ui->btn_camcali_info->setText("CamCali");
}

void SysStateWidget::btn_clicked(bool)
{
    basewidget *widget =(basewidget*) ui->stackedWidget->currentWidget();
    widget->showPage(false);

    ui->stackedWidget->setCurrentIndex(m_pbtnGroup->id((QPushButton*)sender()));
    widget =(basewidget*) ui->stackedWidget->currentWidget();
    widget->showPage();
}

