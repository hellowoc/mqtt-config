#include "syscamcaliinfowidget.h"
#include "ui_syscamcaliinfowidget.h"
#include "global.h"

SysCamCaliInfoWidget::SysCamCaliInfoWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::SysCamCaliInfoWidget)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->setColumnWidth(0,120);
    ui->tableWidget->setColumnWidth(1,130);
    ui->tableWidget->setColumnWidth(2,130);
    ui->tableWidget->setColumnWidth(3,130);
    ui->tableWidget->setColumnWidth(4,130);
    ui->tableWidget->setColumnWidth(5,130);
    ui->tableWidget->setColumnWidth(6,130);
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setFocusPolicy(Qt::NoFocus);
  //  ui->tableWidget->verticalHeader()->setDefaultSectionSize(35);
    ui->tableWidget->setShowGrid(false);
    QHeaderView *header = ui->tableWidget->verticalHeader();
    header->setHidden(true);// 隐藏行号
    QHeaderView *horiHeader = ui->tableWidget->horizontalHeader();
    horiHeader->setFixedHeight(50);
    m_pMessage = NULL;
    shottimer = new QTimer;
    connect(shottimer,SIGNAL(timeout()),this, SLOT(getscreenshot()));
    tdModeList<<"SS"<<"SS1"<<"SS2"<<"SS3";
}

SysCamCaliInfoWidget::~SysCamCaliInfoWidget()
{
    delete ui;
}

void SysCamCaliInfoWidget::showPage(bool bshow)
{
    if(bshow){
        updateVersionInfo();
        updateTableWidget();

        if( struGsh.bScreenShotStatus ==1)
        {
            shottimer->start(1000);

        }
    }
}

void SysCamCaliInfoWidget::retranslateUiWidget()
{
    ui->search->setText(g_myLan().recheck);
}

void SysCamCaliInfoWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_VERSION){
#ifdef Q_OS_UNIX
//        myMqttMsgParaseThread->n_uploadPartsStatusFlag = 1;
#endif
        g_infoWidget().hide();
        updateVersionInfo();
        updateTableWidget();
    }
}

void SysCamCaliInfoWidget::getscreenshot()
{
    shottimer->stop();
    {


        int count = ui->tableWidget->rowCount()-1;
        int countid = 0;
        while(countid <= count)
        {
            ui->tableWidget->scrollTo(ui->tableWidget->model()->index(countid,0)); // 滑动滚轮下拉
            myFlow.sleep(1);
            myFlow.saveScreenshot();

            countid += 16;
        }

        ui->tableWidget->scrollTo(ui->tableWidget->model()->index(count,0));
        myFlow.sleep(1);
        myFlow.saveScreenshot();

        struGsh.bScreenShotStatus = 0;
    }
}

void SysCamCaliInfoWidget::updateVersionInfo()
{
    m_verinfoVec.clear();
    int state;
    QString namestr,valuestr1,valuestr2,valuestr3,valuestr4,valuestr5,valuestr6;
    QStringList list;
    stru_version version;
    CamCaliInfo info(namestr,valuestr1,valuestr2,valuestr3,valuestr4,valuestr5,valuestr6);
    memcpy(&version,&struGsh.struVer,sizeof(version));

    for(int n = 0;n < struCnfg.nLayerTotal;n++)
    {
        for(int i = 0;i < MAX_VIEW;i++){
            if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[i]) continue;
            for(int j = 0;j < struCnfg.struLayerInfo[n].nViewUnitTotal[i];j++){
                namestr = myFlow.getUnitShowName(i,j);
                for(int k = 0; k < 6; k++)
                {
                    if(version.sUnitCali[n][i][j][k][0] != 255 && version.sUnitCali[n][i][j][k][3] != 255
                        && version.sUnitCali[n][i][j][k][0] != 0 && version.sUnitCali[n][i][j][k][3] != 0)
                    {
                        valuestr1 = QString("%1%2%3.%4%5%6").arg(version.sUnitCali[n][i][j][k][0]).
                                arg(version.sUnitCali[n][i][j][k][1],2,10,QChar('0')).
                                arg(version.sUnitCali[n][i][j][k][2],2,10,QChar('0')).
                                arg(version.sUnitCali[n][i][j][k][3],2,10,QChar('0')).
                                arg(version.sUnitCali[n][i][j][k][4],2,10,QChar('0')).
                                arg(version.sUnitCali[n][i][j][k][5],2,10,QChar('0'));

                        if(version.sUnitCaliFSType[n][i][j][k] < tdModeList.length())
                            valuestr2 = tdModeList.at(version.sUnitCaliFSType[n][i][j][k]);
                        else
                            valuestr2 = "-";
                        valuestr3 = QString::number(version.sUnitCaliRowFre[n][i][j][k]/100.0,'f',2);
                        valuestr4 = QString::number(version.sUnitCaliTarget[n][i][j][k]);//
                        list.append(valuestr1+"\n"+valuestr2+"/"+valuestr3+"/"+valuestr4);
                    }
                    else
                    {
                        list.append("-");
                    }

                }
                info = CamCaliInfo(namestr,list.at(0),list.at(1),list.at(2),list.at(3),list.at(4),list.at(5));
                m_verinfoVec.push_back(info);
                list.clear();
            }
        }
    }
}

void SysCamCaliInfoWidget::updateTableWidget()
{
    QStringList list;
    list<<g_myLan().ids_board<<"0"<<"1"<<"2"<<"3"<<"4"<<"5";
    ui->tableWidget->setHorizontalHeaderLabels(list);
    ui->tableWidget->setRowCount(m_verinfoVec.size());
    for(int i = 0;i< m_verinfoVec.size();++i){
        ui->tableWidget->setItem(i,0, new QTableWidgetItem(m_verinfoVec.at(i).strName));

        ui->tableWidget->setItem(i,1, new QTableWidgetItem(m_verinfoVec.at(i).cali0));
        ui->tableWidget->setItem(i,2, new QTableWidgetItem(m_verinfoVec.at(i).cali1));
        ui->tableWidget->setItem(i,3, new QTableWidgetItem(m_verinfoVec.at(i).cali2));
        ui->tableWidget->setItem(i,4, new QTableWidgetItem(m_verinfoVec.at(i).cali3));
        ui->tableWidget->setItem(i,5, new QTableWidgetItem(m_verinfoVec.at(i).cali4));
        ui->tableWidget->setItem(i,6, new QTableWidgetItem(m_verinfoVec.at(i).cali5));

        ui->tableWidget->setRowHeight(i,55);

        for(int j= 0;j < 7;j++){
            if(j != 0)
                ui->tableWidget->item(i,j)->setFont(QFont("微软雅黑",14));

            ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            ui->tableWidget->item(i,j)->setFlags(Qt::ItemIsEnabled);
        }
    }
}

void SysCamCaliInfoWidget::on_search_clicked()
{
    if(struGsh.bSortStart)
    {
        g_infoWidget().setLabelText(g_myLan().msg_turn_off_first);
        g_infoWidget().delayShow();
        myFlow.sleep(2);
        g_infoWidget().hide();
        return;
    }
    g_infoWidget().setLabelText(g_myLan().msg_require_version);
    g_infoWidget().delayShow();

    struGsh.nVersionCaptureIsRun = 1;

    for(int i = 0; i <MAX_LAYER; i++) {
        for(int j = 0; j < MAX_VIEW; j++) {
            for(int m = 0; m < struCnfg.struLayerInfo[i].nViewUnitTotal[j]; m++) {

                for(int n = 0; n <MAX_CALIBRATE_SETS; n++)
                {
                    struGsh.struVer.sUnitCaliFSType[i][j][m][n] = 0;
                    struGsh.struVer.sUnitCaliRowFre[i][j][m][n] = 0;
                    struGsh.struVer.sUnitCaliTarget[i][j][m][n] = 0;
                    for(int p =0; p< 6;p++)
                        struGsh.struVer.sUnitCali[i][j][m][n][p] = 0;
                }
            }
        }
    }

    int i, j, k, m, n, board, nUnitAddr;
    char data[SEND_PACKET_DATA_SIZE];
    memset(data,0,sizeof(data));
    for(i = 0; i < struCnfg.nLayerTotal ; i++)
    {
        for(j = 0; j < MAX_VIEW; j++)
        {
            if(struCnfg.struLayerInfo[i].nViewIsUsed[j]){
                board = struCnfg.struLayerInfo[i].nViewBoardType[j];
                for(m = 0; m < struCnfg.struLayerInfo[i].nViewUnitTotal[j]; m++)
                {
                    nUnitAddr = struCnfg.struLayerInfo[i].stuViewInfo[j].nViewUnitId[m];
                    mySerial.pushCom1CmdData(CMD_CAMERA_GET_CALIINFO, 0, board, nUnitAddr, data, 1);
                    myFlow.msleep(10);
                }
            }
        }
    }
    myFlow.sleep(2);
    g_infoWidget().hide();

    updateVersionInfo();
    updateTableWidget();
}

