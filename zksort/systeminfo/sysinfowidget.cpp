#include "sysinfowidget.h"
#include "ui_sysinfowidget.h"

SysInfoWidget::SysInfoWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::SysInfoWidget)
{
    ui->setupUi(this);
}

SysInfoWidget::~SysInfoWidget()
{
    delete ui;
}

void SysInfoWidget::showPage(bool bshow)
{
}

void SysInfoWidget::retranslateUiWidget()
{
    ui->tabWidget->setTabText(0,g_myLan().ids_operate_log);
    ui->tabWidget->setTabText(1,"Net");
    ui->tabWidget->setTabText(2,g_myLan().ids_more+g_myLan().information);
}

