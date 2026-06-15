#include "remotewidget.h"
#include "ui_remotewidget.h"

RemoteWidget::RemoteWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::RemoteWidget)
{
    ui->setupUi(this); 
}

RemoteWidget::~RemoteWidget()
{
    delete ui;
}

void RemoteWidget::showPage(bool bshow)
{
    if(bshow){
        basewidget *widget =(basewidget*) ui->tabWidget->currentWidget();
        widget->showPage();

        ui->tabWidget->setTabEnabled(1, struCnfg.bautoplcctrlEnable==1);
    }
}

void RemoteWidget::retranslateUiWidget()
{
    ui->tabWidget->setTabText(0,g_myLan().network_config);
    ui->tabWidget->setTabText(1,g_myLan().plc_ctrl);
    
}
