#include "straightsensesetwidget.h"
#include "ui_straightsensesetwidget.h"

StraightSenseSetWidget::StraightSenseSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::StraightSenseSetWidget)
{
    ui->setupUi(this);
}

StraightSenseSetWidget::~StraightSenseSetWidget()
{
    delete ui;
}

void StraightSenseSetWidget::showPage(bool bshow)
{

}

void StraightSenseSetWidget::receiveMsg(int msg1, int msg2, int msg3)
{

}

void StraightSenseSetWidget::retranslateUiWidget()
{

}
