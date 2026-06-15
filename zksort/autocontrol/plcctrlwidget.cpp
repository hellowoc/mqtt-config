#include "plcctrlwidget.h"
#include "ui_plcctrlwidget.h"

PlcCtrlWidget::PlcCtrlWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::PlcCtrlWidget)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(aliveStatusUpdate()));
}

PlcCtrlWidget::~PlcCtrlWidget()
{
    delete ui;
}

void PlcCtrlWidget::showPage(bool bshow)
{
    if(bshow)
    {
        timer->start(3000);
        struCnfg.nPlcCtrl = 1;
        if(struCnfg.autoCtrlBoardAlive <= 2)
        {
            ui->label->setText(g_myLan().plc_ctrling);
            ui->label->setStyleSheet("color:rgb(0,0,0)");
        }
        else
        {
            ui->label->setText(g_myLan().auto_ctrl_board_offline);
            ui->label->setStyleSheet("color:rgb(255,0,0)");
        }
    }
    else
    {
        timer->stop();
        struCnfg.nPlcCtrl = 0;
    }
}

void PlcCtrlWidget::retranslateUiWidget()
{

}

void PlcCtrlWidget::receiveMsg(int msg1, int msg2, int msg3)
{

}

void PlcCtrlWidget::aliveStatusUpdate()
{
//    qDebug()<<struCnfg.autoCtrlBoardAlive;
    if(struCnfg.autoCtrlBoardAlive <= 2)
    {
        ui->label->setText(g_myLan().plc_ctrling);
        ui->label->setStyleSheet("color:rgb(0,0,0)");
    }
    else{
        ui->label->setText(g_myLan().auto_ctrl_board_offline);
        ui->label->setStyleSheet("color:rgb(255,0,0)");
    }
    struCnfg.autoCtrlBoardAlive++;
}


