#include "nirdotcorrectivelistwidget.h"
#include "ui_nirdotcorrectivelistwidget.h"

NirDotCorrectiveListWidget::NirDotCorrectiveListWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::NirDotCorrectiveListWidget)
{
    ui->setupUi(this);
    m_pViewBtn[0] = ui->pushButton;
    m_pViewBtn[1] = ui->pushButton_2;
    m_pViewBtn[2] = ui->pushButton_3;
    m_pViewBtn[3] = ui->pushButton_4;
    m_pViewBtn[4] = ui->pushButton_5;
    m_pViewBtn[5] = ui->pushButton_6;
    m_pViewBtn[6] = ui->pushButton_7;
    m_pViewBtn[7] = ui->pushButton_8;
    m_pChuteWidget = ui->chuteWidget;
    m_pLayerBtn[0] = ui->mainBtn;
    m_pLayerBtn[1] = ui->viceBtn;
    setSlotConnect();
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setFocusPolicy(Qt::NoFocus);
    ui->tableWidget->setShowGrid(true);

    QHeaderView *header = ui->tableWidget->verticalHeader();
    header->setHidden(true);// 隐藏行号

    QHeaderView *horiHeader = ui->tableWidget->horizontalHeader();
    horiHeader->setFixedHeight(50);
    horiHeader->setSectionResizeMode(QHeaderView::Stretch);
}

NirDotCorrectiveListWidget::~NirDotCorrectiveListWidget()
{
    delete ui;
}

void NirDotCorrectiveListWidget::showPage(bool bshow)
{
    if(bshow){
        basewidget::showPage(bshow);
        //if(SENSOR_C1_2 == struCnfg.struLayerInfo[struGsh.nLayer].nViewSensorType[struGsh.nView])
        if(struCnfp.nViewSensorType[0][struGsh.nView] == SENSOR_C1_2)
        {
            ui->dotDataCapture->hide();
        }
        else
        {
            ui->dotDataCapture->show();
        }
        updateValue();
    }
}

void NirDotCorrectiveListWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){
        if(msg2 == chute_change){
            basewidget::receiveMsg(msg1,msg2,msg3);
        }
            updateValue();
    }
}

void NirDotCorrectiveListWidget::OnViewChange(int view)
{
    updateValue();
}

void NirDotCorrectiveListWidget::OnChuteChange()
{
    updateValue();
}

void NirDotCorrectiveListWidget::OnLayerChange()
{
    updateValue();
}
void NirDotCorrectiveListWidget::retranslateUiWidget()
{
    ui->dotDataCapture->setText(g_myLan().get_all_bad);
    ui->sendAll->setText(g_myLan().send_all);
    ui->pushButton_9->setText(g_myLan().clear);
}

void NirDotCorrectiveListWidget::updateTable()
{
    for(int i = 0;i< ui->tableWidget->rowCount();i++)
        for(int j = 0;j < 2;j++){
            QTableWidgetItem *item = ui->tableWidget->takeItem(i,j);
            delete item;
        }
    QStringList list;
    list<<g_myLan().ids_color<<g_myLan().nir_bad_point;
    ui->tableWidget->setHorizontalHeaderLabels(list);
    ui->tableWidget->setRowCount(m_dotVec.size());
    for(int i = 0;i< m_dotVec.size();++i){
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(m_dotVec.at(i).redOrgreen));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(m_dotVec.at(i).dotValue));
        for(int j= 0;j < 2;j++){
            ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            ui->tableWidget->item(i,j)->setFlags(Qt::ItemIsEnabled);
        }
    }

}

void NirDotCorrectiveListWidget::updateValue()
{
    updateDotVec();
    updateTable();
}

void NirDotCorrectiveListWidget::updateDotVec()
{
    m_dotVec.clear();
    DotStruct dot;

    for(int ud = 0;ud < 2;ud++)//传感器上下的坏点数据都要显示出来
    {
        for(int i = 0;i < 1024;i++)
        {
            if(struGsh.nNirDotCorrectiveRed[struGsh.nView][struGsh.nUnit][ud][i])
            {
                if(0 == ud)
                {
                    dot.redOrgreen = g_myLan().ids_red + "(" + g_myLan().ids_up + ")";
                }
                else
                {
                    dot.redOrgreen = g_myLan().ids_red + "(" + g_myLan().ids_down + ")";
                }
                dot.dotValue = QString::number(i+1);
                m_dotVec.append(dot);
            }
        }
        for(int i = 0;i < 1024;i++)
        {
            if(struGsh.nNirDotCorrectiveGreen[struGsh.nView][struGsh.nUnit][ud][i])
            {
                if(0 == ud)
                {
                    dot.redOrgreen = g_myLan().ids_green + "(" + g_myLan().ids_up + ")";
                }
                else
                {
                    dot.redOrgreen = g_myLan().ids_green + "(" + g_myLan().ids_down + ")";
                }
                dot.dotValue = QString::number(i+1);
                m_dotVec.append(dot);
            }
        }
        for(int i = 0;i < 1024;i++)
        {
            if(struGsh.nNirDotCorrectiveBlue[struGsh.nView][struGsh.nUnit][ud][i])
            {
                if(0 == ud)
                {
                    dot.redOrgreen = g_myLan().ids_blue + "(" + g_myLan().ids_up + ")";
                }
                else
                {
                    dot.redOrgreen = g_myLan().ids_blue + "(" + g_myLan().ids_down + ")";
                }
                dot.dotValue = QString::number(i+1);
                m_dotVec.append(dot);
            }
        }
        for(int i = 0;i < 1024;i++)
        {
            if(struGsh.nNirDotCorrectiveN[struGsh.nView][struGsh.nUnit][ud][i])
            {
                if(0 == ud)
                {
                    dot.redOrgreen = g_myLan().ids_n + "(" + g_myLan().ids_up + ")";
                }
                else
                {
                    dot.redOrgreen = g_myLan().ids_n + "(" + g_myLan().ids_down + ")";
                }
                dot.dotValue = QString::number(i+1);
                m_dotVec.append(dot);
            }
        }
    }
}

void NirDotCorrectiveListWidget::on_sendAll_clicked()
{
    myFlow.resetCameraNirDotCorrectiveRG(struGsh.nLayer,struGsh.nView,struGsh.nUnit);
}

void NirDotCorrectiveListWidget::on_dotDataCapture_clicked()
{
    g_infoWidget().setLabelText(g_myLan().msg_applying);
    g_infoWidget().delayShow();

    myFlow.myNetWork->setUdpRecvType(CAPTURE_SINGLE_WAVE);
    myFlow.myNetWork->udpStart();

    struGsh.nSignalDispType = 3;
    myFlow.resetCameraWaveType(struGsh.nLayer,struGsh.nView,struGsh.nUnit);

    myFlow.sleep(3);

    myFlow.myNetWork->udpStop();

    struGsh.nSignalDispType = 0;
    myFlow.resetCameraWaveType(struGsh.nLayer,struGsh.nView,struGsh.nUnit);

    myFlow.sleep(2);
    g_infoWidget().hide();

    updateValue();
}

void NirDotCorrectiveListWidget::on_pushButton_9_clicked()
{
    for(int ud = 0;ud < 2;ud++)//传感器上下的坏点数据都要显示出来
    {
        for(int i = 0;i < 1024;i++)
        {
            struGsh.nNirDotCorrectiveRed[struGsh.nView][struGsh.nUnit][ud][i] = 0;

        }
        for(int i = 0;i < 1024;i++)
        {
            struGsh.nNirDotCorrectiveGreen[struGsh.nView][struGsh.nUnit][ud][i] = 0;

        }
        for(int i = 0;i < 1024;i++)
        {
            struGsh.nNirDotCorrectiveBlue[struGsh.nView][struGsh.nUnit][ud][i] = 0;

        }
        for(int i = 0;i < 1024;i++)
        {
            struGsh.nNirDotCorrectiveN[struGsh.nView][struGsh.nUnit][ud][i] = 0;

        }
    }
    updateValue();
}
