#include "videtectboardinfowidget.h"
#include "ui_videtectboardinfowidget.h"
#include "globalflow.h"

VIDetectBoardInfoWidget::VIDetectBoardInfoWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::VIDetectBoardInfoWidget)
{
    ui->setupUi(this);

    m_viboardInfoLabel[0][0] = ui->label_0;
    m_viboardInfoLabel[1][0] = ui->label_1;
    m_viboardInfoLabel[2][0] = ui->label_2;
    m_viboardInfoLabel[3][0] = ui->label_3;
    m_viboardInfoLabel[4][0] = ui->label_4;
    m_viboardInfoLabel[5][0] = ui->label_5;
    m_viboardInfoLabel[6][0] = ui->label_6;
    m_viboardInfoLabel[7][0] = ui->label_7;
    m_viboardInfoLabel[8][0] = ui->label_8;
    m_viboardInfoLabel[9][0] = ui->label_9;

    m_viboardInfoLabel[0][1] = ui->label_v_1;
    m_viboardInfoLabel[1][1] = ui->label_v_2;
    m_viboardInfoLabel[2][1] = ui->label_v_3;
    m_viboardInfoLabel[3][1] = ui->label_v_4;
    m_viboardInfoLabel[4][1] = ui->label_v_5;
    m_viboardInfoLabel[5][1] = ui->label_v_6;
    m_viboardInfoLabel[6][1] = ui->label_v_7;
    m_viboardInfoLabel[7][1] = ui->label_v_8;
    m_viboardInfoLabel[8][1] = ui->label_v_9;
    m_viboardInfoLabel[9][1] = ui->label_v_10;

    m_viboardInfoLabel[0][2] = ui->label_i_1;
    m_viboardInfoLabel[1][2] = ui->label_i_2;
    m_viboardInfoLabel[2][2] = ui->label_i_3;
    m_viboardInfoLabel[3][2] = ui->label_i_4;
    m_viboardInfoLabel[4][2] = ui->label_i_5;
    m_viboardInfoLabel[5][2] = ui->label_i_6;
    m_viboardInfoLabel[6][2] = ui->label_i_7;
    m_viboardInfoLabel[7][2] = ui->label_i_8;
    m_viboardInfoLabel[8][2] = ui->label_i_9;
    m_viboardInfoLabel[9][2] = ui->label_i_10;

    m_viboardInfoCombox[0][0] = ui->comboBox_addr_1;
    m_viboardInfoCombox[1][0] = ui->comboBox_addr_2;
    m_viboardInfoCombox[2][0] = ui->comboBox_addr_3;
    m_viboardInfoCombox[3][0] = ui->comboBox_addr_4;
    m_viboardInfoCombox[4][0] = ui->comboBox_addr_5;
    m_viboardInfoCombox[5][0] = ui->comboBox_addr_6;
    m_viboardInfoCombox[6][0] = ui->comboBox_addr_7;
    m_viboardInfoCombox[7][0] = ui->comboBox_addr_8;
    m_viboardInfoCombox[8][0] = ui->comboBox_addr_9;
    m_viboardInfoCombox[9][0] = ui->comboBox_addr_10;

    m_viboardInfoCombox[0][1] = ui->comboBox_boardtype_1;
    m_viboardInfoCombox[1][1] = ui->comboBox_boardtype_2;
    m_viboardInfoCombox[2][1] = ui->comboBox_boardtype_3;
    m_viboardInfoCombox[3][1] = ui->comboBox_boardtype_4;
    m_viboardInfoCombox[4][1] = ui->comboBox_boardtype_5;
    m_viboardInfoCombox[5][1] = ui->comboBox_boardtype_6;
    m_viboardInfoCombox[6][1] = ui->comboBox_boardtype_7;
    m_viboardInfoCombox[7][1] = ui->comboBox_boardtype_8;
    m_viboardInfoCombox[8][1] = ui->comboBox_boardtype_9;
    m_viboardInfoCombox[9][1] = ui->comboBox_boardtype_10;

    ui->label_ip->hide();
    ui->lineEdit_cam1_ip->hide();
    ui->lineEdit_cam2_ip->hide();
    ui->lineEdit_cam3_ip->hide();
    ui->lineEdit_cam4_ip->hide();
    ui->lineEdit_cam5_ip->hide();
    ui->lineEdit_cam6_ip->hide();
    ui->lineEdit_cam7_ip->hide();
    ui->lineEdit_cam8_ip->hide();
    ui->lineEdit_cam9_ip->hide();
    ui->lineEdit_cam10_ip->hide();

    m_pVIBoardAddrSigMapper = new QSignalMapper(this);
    for(int i = 0; i < CONST_MAX_VIBOARDINFO_PER_PAGE; i++){
        m_pVIBoardAddrSigMapper->setMapping(m_viboardInfoCombox[i][0], i);
        connect(m_viboardInfoCombox[i][0],SIGNAL(activated(int)),m_pVIBoardAddrSigMapper,SLOT(map()));
    }
    connect(m_pVIBoardAddrSigMapper,SIGNAL(mapped(int)),this,SLOT(viBoardAddrActivated(int)));

    m_pVIBoardTypeSigMapper = new QSignalMapper(this);
    for(int i = 0; i < CONST_MAX_VIBOARDINFO_PER_PAGE; i++){
        m_pVIBoardTypeSigMapper->setMapping(m_viboardInfoCombox[i][1], i);
        connect(m_viboardInfoCombox[i][1],SIGNAL(activated(int)),m_pVIBoardTypeSigMapper,SLOT(map()));
    }
    connect(m_pVIBoardTypeSigMapper,SIGNAL(mapped(int)),this,SLOT(viBoardTypeActivated(int)));

    ui->lineEdit_totalVIboard->setMinMax(this,0,MAX_VI_BOARD,100,&struCnfg.viBoardTotalCount);
    ui->checkBox->setChecked(struCnfg.viBoardAlarmEnable);
    m_curPage = 0;
    connect(mySerialRecv,SIGNAL(signalupdateVIdata()),this,SLOT(updateVIData()));
}

VIDetectBoardInfoWidget::~VIDetectBoardInfoWidget()
{
    delete ui;
}

void VIDetectBoardInfoWidget::showPage(bool bshow)
{
    if(bshow){
        updateAll();
    }
}

void VIDetectBoardInfoWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){
        if (msg2 == 100){
            m_curPage = 0;
            updateAll();
        }
    }
}

void VIDetectBoardInfoWidget::updateVIInfo()
{
    m_toalPage = struCnfg.viBoardTotalCount /CONST_MAX_VIBOARDINFO_PER_PAGE;
    QStringList list;
    QStringList listType;
    listType<<"Sys"<<"Valve"<<"Lamp";

    for(int i = 1; i < struCnfg.viBoardTotalCount+1;i++)
    {
        list.append(QString::number(i));
    }
    for(int i = 0; i < CONST_MAX_VIBOARDINFO_PER_PAGE;i++)
    {
        m_viboardInfoCombox[i][0]->clear();
        m_viboardInfoCombox[i][1]->clear();
        m_viboardInfoCombox[i][0]->addItems(list);
        m_viboardInfoCombox[i][1]->addItems(listType);
    }

    if(m_curPage < 0 || m_curPage >m_toalPage)
        m_curPage = 0;

    for(int i = 0; i < CONST_MAX_VIBOARDINFO_PER_PAGE;i++)
    {
        int index = m_curPage * CONST_MAX_VIBOARDINFO_PER_PAGE + i;
        m_viboardInfoLabel[i][0]->setVisible(index < struCnfg.viBoardTotalCount);
        m_viboardInfoLabel[i][1]->setVisible(index < struCnfg.viBoardTotalCount);
        m_viboardInfoLabel[i][2]->setVisible(index < struCnfg.viBoardTotalCount);
        m_viboardInfoCombox[i][0]->setVisible(index < struCnfg.viBoardTotalCount);
        m_viboardInfoCombox[i][1]->setVisible(index < struCnfg.viBoardTotalCount);
    }

    for(int i = 0; i < CONST_MAX_VIBOARDINFO_PER_PAGE;i++)
    {
        int curBoard = m_curPage * CONST_MAX_VIBOARDINFO_PER_PAGE + i;
        QString str = "V-I board"+QString::number(curBoard +1);
        m_viboardInfoLabel[i][0]->setText(str);
        m_viboardInfoCombox[i][0]->setCurrentIndex(struCnfg.viBoardInfo[curBoard][0]);
        m_viboardInfoCombox[i][1]->setCurrentIndex(struCnfg.viBoardInfo[curBoard][1]);
    }

    ui->lineEdit_totalVIboard->setText(QString::number(struCnfg.viBoardTotalCount));
    ui->pushButton->setVisible( struCnfg.viBoardTotalCount  > CONST_MAX_VIBOARDINFO_PER_PAGE);
    ui->pushButton_2->setVisible( struCnfg.viBoardTotalCount  > CONST_MAX_VIBOARDINFO_PER_PAGE);

}

void VIDetectBoardInfoWidget::updateVIData()
{
    for(int i = 0; i < CONST_MAX_VIBOARDINFO_PER_PAGE;i++)
    {
        int curBoard = m_curPage * CONST_MAX_VIBOARDINFO_PER_PAGE + i;
        QString voltage  =  QString::number(struCnfg.viBoardInfo[curBoard][2]/1000.0,'f',3);
        QString current  =  QString::number(struCnfg.viBoardInfo[curBoard][3]/1000.0,'f',3);

        if(struCnfg.viBoardAlarmEnable)
        {
            if(struCnfg.viBoardInfo[curBoard][1] == 0 || struCnfg.viBoardInfo[curBoard][1] == 1)//sys、valve
            {
                if(struCnfg.viBoardInfo[curBoard][2] < 22800 || struCnfg.viBoardInfo[curBoard][2] > 25200)

                    m_viboardInfoLabel[i][1]->setStyleSheet("color:red");
                else
                    m_viboardInfoLabel[i][1]->setStyleSheet("color:blue");

                if(struCnfg.viBoardInfo[curBoard][3] > 25000)
                    m_viboardInfoLabel[i][2]->setStyleSheet("color:red");
                else
                    m_viboardInfoLabel[i][2]->setStyleSheet("color:blue");
            }
            if(struCnfg.viBoardInfo[curBoard][1] == 2) //lamp
            {
                if(struCnfg.viBoardInfo[curBoard][2] < 45600 || struCnfg.viBoardInfo[curBoard][2] > 50400)
                    m_viboardInfoLabel[i][1]->setStyleSheet("color:red");
                else
                    m_viboardInfoLabel[i][1]->setStyleSheet("color:blue");

                if(struCnfg.viBoardInfo[curBoard][3] > 12500)
                    m_viboardInfoLabel[i][2]->setStyleSheet("color:red");
                else
                    m_viboardInfoLabel[i][2]->setStyleSheet("color:blue");
            }
        }
        else{
            m_viboardInfoLabel[i][1]->setStyleSheet("color:blue");
            m_viboardInfoLabel[i][2]->setStyleSheet("color:blue");
        }
        m_viboardInfoLabel[i][1]->setText(voltage);
        m_viboardInfoLabel[i][2]->setText(current);
    }
}
void VIDetectBoardInfoWidget::updateAll()
{
    updateVIInfo();
    updateVIData();
}

void VIDetectBoardInfoWidget::retranslateUiWidget()
{
    ui->checkBox->setText(g_myLan().alarm);
    ui->label_name->setText("Name");
    ui->label_addr->setText("Addr");
    ui->label_type->setText("Type");
    ui->label_v->setText("V(V)");
    ui->label_i->setText("I(A)");
}

void VIDetectBoardInfoWidget::viBoardAddrActivated(int index)
{
    int curBoard = m_curPage * CONST_MAX_VIBOARDINFO_PER_PAGE + index;
    struCnfg.viBoardInfo[curBoard][0] = m_viboardInfoCombox[index][0]->currentIndex();
}

void VIDetectBoardInfoWidget::viBoardTypeActivated(int index)
{
    int curBoard = m_curPage * CONST_MAX_VIBOARDINFO_PER_PAGE + index;
    struCnfg.viBoardInfo[curBoard][1] = m_viboardInfoCombox[index][1]->currentIndex();
}

void VIDetectBoardInfoWidget::on_pushButton_clicked()
{
    if(m_curPage > 0){
        m_curPage--;
        updateAll();
    }
}

void VIDetectBoardInfoWidget::on_pushButton_2_clicked()
{
    if(m_curPage < m_toalPage){
        m_curPage++;
        updateAll();
    }
}

void VIDetectBoardInfoWidget::on_checkBox_clicked(bool checked)
{
    ui->checkBox->setChecked(checked);
    struCnfg.viBoardAlarmEnable = checked;
}
