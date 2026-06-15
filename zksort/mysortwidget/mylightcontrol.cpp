#include "mylightcontrol.h"
#include "ui_mylightcontrol.h"

MylightControl::MylightControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MylightControl)
{
    ui->setupUi(this);
    m_boardIndex = -1;
    m_portIndex = -1;
}

void MylightControl::initState(int boardIndex, int portIndex, QString text, int value, int stateValue)
{
    m_boardIndex = boardIndex;
    m_portIndex= portIndex;
    ui->label->setText(text);
    ui->lineEdit->setText(QString::number(value));
    ui->lineEdit->setMinMax(this,1,struCnfg.stuLayerLampCfg.nLampBoardLimit[m_boardIndex][m_portIndex],0,&struCnfp.nLampValue[0][m_boardIndex][m_portIndex]);

    ui->checkBox->setChecked((bool)stateValue);
    ui->checkBox->setEnabled(struGsh.bRunMode != RUNMODE_ENGINEER);

}


void MylightControl::initSeq(int boardIndex, int portIndex, int value)
{
    m_boardIndex = boardIndex;
    m_portIndex= portIndex;
    ui->lineEdit_2->setText(QString::number(value));
    ui->lineEdit_2->setMinMax(this,0,15, 0, &struCnfp.nLampSeq[0][m_boardIndex][m_portIndex]);
   // ui->lineEdit_2->setVisible(struCnfp.nTimeDivisionScheme[struGsh.nLayer][struGsh.nView] != 0);
    ui->lineEdit_2->setEnabled(struGsh.debugMode);
}

MylightControl::~MylightControl()
{
    delete ui;
}

void MylightControl::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){
       logger()->info(QString::fromUtf8("adjust light").toUtf8().data());
        myFlow.resetLampRG();
    }
}

void MylightControl::updateBtnText()
{
    if(ui->checkBox->isChecked()){
        ui->checkBox->setText(g_myLan().ids_enable);
    }else{
        ui->checkBox->setText(g_myLan().ids_disable);
    }
}

void MylightControl::on_checkBox_clicked(bool checked)
{
    if(checked){
        struCnfp.nLampEnable[struGsh.nLayer][m_boardIndex][m_portIndex] = 1;
    }else{
        struCnfp.nLampEnable[struGsh.nLayer][m_boardIndex][m_portIndex] = 0;
    }
//    updateBtnText();
    myFlow.resetLampRG();
}
