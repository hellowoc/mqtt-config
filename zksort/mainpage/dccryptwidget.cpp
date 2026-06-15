#include "dccryptwidget.h"
#include "ui_dccryptwidget.h"
#include "global.h"
DccryptWidget::DccryptWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::DccryptWidget)
{
    ui->setupUi(this);
}

DccryptWidget::~DccryptWidget()
{
    delete ui;
}

void DccryptWidget::showPage(bool bshow)
{

}

void DccryptWidget::retranslateUiWidget()
{
    ui->label_2->setText(g_myLan().dc_input);
    ui->label->setText(g_myLan().dc_productsn);
    ui->DccryptBtn->setText(g_myLan().ids_ok);
}

void DccryptWidget::on_DccryptBtn_clicked()
{
    g_MainManager().ShowWidget(SM_SELF_CHECK_WIDGET);
}
