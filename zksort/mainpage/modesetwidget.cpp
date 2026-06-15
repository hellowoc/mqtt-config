#include "ModeSetWidget.h"
#include "ui_ModeSetWidget.h"

ModeSetWidget::ModeSetWidget(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::ModeSetWidget)
{
    ui->setupUi(this);
    m_pbtnGroup = new QButtonGroup(this);
    m_pbtnGroup->addButton(ui->identifyBtn,0);
    m_pbtnGroup->addButton(ui->ejectBtn,1);
    m_pbtnGroup->addButton(ui->lightBtn,2);
    m_pbtnGroup->addButton(ui->backgroundBtn,3);
    for(int i = 0;i< 4;i++){
        m_pbtnGroup->button(i)->setCheckable(true);
        connect(m_pbtnGroup->button(i),SIGNAL(clicked(bool)),this,SLOT(btn_clicked(bool)));
    }

}

ModeSetWidget::~ModeSetWidget()
{
    delete ui;
}

void ModeSetWidget::showPage(bool bshow)
{
    if(bshow){
        ui->stackedWidget->setCurrentIndex(0);
        m_pbtnGroup->button(0)->setChecked(true);
        BaseWidget *widget =(BaseWidget*) ui->stackedWidget->currentWidget();
        widget->showPage();
    }else{
        for(int i = 0;i< ui->stackedWidget->count();i++){
            BaseWidget *widget =(BaseWidget*) ui->stackedWidget->widget(i);
            widget->showPage(false);
        }
    }
}

void ModeSetWidget::retranslateUiWidget()
{
    ui->retranslateUi(this);
}

void ModeSetWidget::btn_clicked(bool)
{
    BaseWidget *widget =(BaseWidget*) ui->stackedWidget->currentWidget();
    widget->showPage(false);
    ui->stackedWidget->setCurrentIndex(m_pbtnGroup->id((QPushButton*)sender()));
    widget =(BaseWidget*) ui->stackedWidget->currentWidget();
    widget->showPage();
}
