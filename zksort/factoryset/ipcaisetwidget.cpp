#include "ipcaisetwidget.h"
#include "ui_ipcaisetwidget.h"
#include "globalflow.h"

IpcAISetWidget::IpcAISetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::IpcAISetWidget)
{
    ui->setupUi(this);

    m_pIpcAITitle[0] = ui->label_0;
    m_pIpcAITitle[1] = ui->label_1;
    m_pIpcAITitle[2] = ui->label_2;
    m_pIpcAITitle[3] = ui->label_3;
    m_pIpcAITitle[4] = ui->label_4;
    m_pIpcAITitle[5] = ui->label_5;
    m_pIpcAITitle[6] = ui->label_6;
    m_pIpcAITitle[7] = ui->label_7;
    m_pIpcAITitle[8] = ui->label_8;
    m_pIpcAITitle[9] = ui->label_9;
    m_pIpcAITitle[10] = ui->label_10;
    m_pIpcAITitle[11] = ui->label_11;

    m_pIpcAIPara[0][0] = ui->lineEdit_0_0;
    m_pIpcAIPara[1][0] = ui->lineEdit_1_0;
    m_pIpcAIPara[2][0] = ui->lineEdit_2_0;
    m_pIpcAIPara[3][0] = ui->lineEdit_3_0;
    m_pIpcAIPara[4][0] = ui->lineEdit_4_0;
    m_pIpcAIPara[5][0] = ui->lineEdit_5_0;
    m_pIpcAIPara[6][0] = ui->lineEdit_6_0;
    m_pIpcAIPara[7][0] = ui->lineEdit_7_0;
    m_pIpcAIPara[8][0] = ui->lineEdit_8_0;
    m_pIpcAIPara[9][0] = ui->lineEdit_9_0;
    m_pIpcAIPara[10][0] = ui->lineEdit_10_0;
    m_pIpcAIPara[11][0] = ui->lineEdit_11_0;

    m_pIpcAIPara[0][1] = ui->lineEdit_0_1;
    m_pIpcAIPara[1][1] = ui->lineEdit_1_1;
    m_pIpcAIPara[2][1] = ui->lineEdit_2_1;
    m_pIpcAIPara[3][1] = ui->lineEdit_3_1;
    m_pIpcAIPara[4][1] = ui->lineEdit_4_1;
    m_pIpcAIPara[5][1] = ui->lineEdit_5_1;
    m_pIpcAIPara[6][1] = ui->lineEdit_6_1;
    m_pIpcAIPara[7][1] = ui->lineEdit_7_1;
    m_pIpcAIPara[8][1] = ui->lineEdit_8_1;
    m_pIpcAIPara[9][1] = ui->lineEdit_9_1;
    m_pIpcAIPara[10][1] = ui->lineEdit_10_1;
    m_pIpcAIPara[11][1] = ui->lineEdit_11_1;

    m_pIpcAIPara[0][2] = ui->lineEdit_0_2;
    m_pIpcAIPara[1][2] = ui->lineEdit_1_2;
    m_pIpcAIPara[2][2] = ui->lineEdit_2_2;
    m_pIpcAIPara[3][2] = ui->lineEdit_3_2;
    m_pIpcAIPara[4][2] = ui->lineEdit_4_2;
    m_pIpcAIPara[5][2] = ui->lineEdit_5_2;
    m_pIpcAIPara[6][2] = ui->lineEdit_6_2;
    m_pIpcAIPara[7][2] = ui->lineEdit_7_2;
    m_pIpcAIPara[8][2] = ui->lineEdit_8_2;
    m_pIpcAIPara[9][2] = ui->lineEdit_9_2;
    m_pIpcAIPara[10][2] = ui->lineEdit_10_2;
    m_pIpcAIPara[11][2] = ui->lineEdit_11_2;

    m_pIpcAIPara[0][3] = ui->lineEdit_0_3;
    m_pIpcAIPara[1][3] = ui->lineEdit_1_3;
    m_pIpcAIPara[2][3] = ui->lineEdit_2_3;
    m_pIpcAIPara[3][3] = ui->lineEdit_3_3;
    m_pIpcAIPara[4][3] = ui->lineEdit_4_3;
    m_pIpcAIPara[5][3] = ui->lineEdit_5_3;
    m_pIpcAIPara[6][3] = ui->lineEdit_6_3;
    m_pIpcAIPara[7][3] = ui->lineEdit_7_3;
    m_pIpcAIPara[8][3] = ui->lineEdit_8_3;
    m_pIpcAIPara[9][3] = ui->lineEdit_9_3;
    m_pIpcAIPara[10][3] = ui->lineEdit_10_3;
    m_pIpcAIPara[11][3] = ui->lineEdit_11_3;

}

IpcAISetWidget::~IpcAISetWidget()
{
    delete ui;
}

void IpcAISetWidget::showPage(bool bshow)
{
    basewidget::showPage(bshow);
    if(bshow){
        updateView();
    }
    else{
//            myFlow.configIpcAIPara(struGsh.nView,struGsh.nUnit);
    }
}
void IpcAISetWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){
        if (msg2 == 61)
        {
            updateView();
        }
    }
}

void IpcAISetWidget::retranslateUiWidget()
{
    ui->pushButton->setText(g_myLan().ids_apply);
}

void IpcAISetWidget::updateView()
{
    int i, j;

    for(i = 0; i < MAX_IPC_NUM; i++)
    {
        m_pIpcAITitle[i]->setVisible(i<struCnfg.nIpcCount);
        for(j = 0; j< 4;j++)
        {
            m_pIpcAIPara[i][j]->setVisible(i<struCnfg.nIpcCount);
            m_pIpcAIPara[i][j]->setEnabled(i<struCnfg.nIpcCount);
        }
//        if(i < struCnfg.nIpcCount)
//        {
//            m_pIpcAIPara[i][0]->setText(QString::number(struCnfg.stuIpcConfigFileInfo[i].cameraCount));
//            m_pIpcAIPara[i][1]->setText(QString::number(struCnfg.stuIpcConfigFileInfo[i].batchSize));
//            m_pIpcAIPara[i][2]->setText(QString::number(struCnfg.stuIpcConfigFileInfo[i].roiHeight));
//            m_pIpcAIPara[i][3]->setText(QString::number(struCnfg.stuIpcConfigFileInfo[i].roiRepeat));

//            m_pIpcAIPara[i][0]->setMinMax(this,0,16,51,&struCnfg.stuIpcConfigFileInfo[i].cameraCount);
//            m_pIpcAIPara[i][1]->setMinMax(this,0,100,51,&struCnfg.stuIpcConfigFileInfo[i].batchSize);
//            m_pIpcAIPara[i][2]->setMinMax(this,0,4096,51,&struCnfg.stuIpcConfigFileInfo[i].roiHeight);
//            m_pIpcAIPara[i][3]->setMinMax(this,0,100,51,&struCnfg.stuIpcConfigFileInfo[i].roiRepeat);
//        }
    }
}

void IpcAISetWidget::on_pushButton_clicked()
{

}
