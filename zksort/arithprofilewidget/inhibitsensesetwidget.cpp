#include "inhibitsensesetwidget.h"
#include "ui_inhibitsensesetwidget.h"

InhibitSensesetWidget::InhibitSensesetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::InhibitSensesetWidget)
{
    ui->setupUi(this);
}

InhibitSensesetWidget::~InhibitSensesetWidget()
{
    delete ui;
}

void InhibitSensesetWidget::showPage(bool bshow)
{

}

void InhibitSensesetWidget::receiveMsg(int msg1, int msg2, int msg3)
{

}

void InhibitSensesetWidget::retranslateUiWidget()
{

}
