#include "sysipcbadpointswidget.h"
#include "ui_sysipcbadpointswidget.h"
#include "global.h"

SysIpcBadPointsWidget::SysIpcBadPointsWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::SysIpcBadPointsWidget)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setColumnWidth(0,200);
    ui->tableWidget->setColumnWidth(1,500);
    ui->tableWidget->setColumnWidth(2,300);
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setFocusPolicy(Qt::NoFocus);
    ui->tableWidget->setShowGrid(false);

    QHeaderView *header = ui->tableWidget->verticalHeader();
    header->setHidden(true);// 隐藏行号

    QHeaderView *horiHeader = ui->tableWidget->horizontalHeader();
    horiHeader->setFixedHeight(50);
    horiHeader->setSectionResizeMode(QHeaderView::Stretch);
    m_pMessage = NULL;

    getThread = new getBadPointsThread();
    connect(getThread,SIGNAL(sGetBadPointsFinished()),this,SLOT(updateTableWidget()));
    getThread->m_pParentWidget = this;

    m_dateTimeWidget[0][0] = ui->lineEdit;
    m_dateTimeWidget[0][1] = ui->lineEdit_2;
    m_dateTimeWidget[0][2] = ui->lineEdit_3;
    m_dateTimeWidget[1][0] = ui->lineEdit_4;
    m_dateTimeWidget[1][1] = ui->lineEdit_5;
    m_dateTimeWidget[1][2] = ui->lineEdit_6;

    QDateTime  dateTime = QDateTime::currentDateTime();
    for(int i =0;i < 2;i++)
    {
        m_dateTime[i][0] = dateTime.date().year();
        m_dateTime[i][1] = dateTime.date().month();
        m_dateTime[i][2] = dateTime.date().day();

        m_dateTimeWidget[i][0]->setText(QString::number(m_dateTime[i][0]));
        m_dateTimeWidget[i][1]->setText(QString::number(m_dateTime[i][1]));
        m_dateTimeWidget[i][2]->setText(QString::number(m_dateTime[i][2]));

        m_dateTimeWidget[i][0]->setMinMax(this,2000,2099,MSG_ID_YEAR,&m_dateTime[i][0]);
        m_dateTimeWidget[i][1]->setMinMax(this,1,12,MSG_ID_MONTH,&m_dateTime[i][1]);
        m_dateTimeWidget[i][2]->setMinMax(this,1,31,MSG_ID_DAY,&m_dateTime[i][2]);
    }
}

SysIpcBadPointsWidget::~SysIpcBadPointsWidget()
{
    delete ui;
    delete getThread;
    getThread = NULL;
}

void SysIpcBadPointsWidget::showPage(bool bshow)
{
    if(bshow){
        updateTableWidget();
    }
}

void SysIpcBadPointsWidget::retranslateUiWidget()
{
    ui->search->setText(g_myLan().recheck);
    ui->pushButton->setText(g_myLan().clear);
}

void SysIpcBadPointsWidget::receiveMsg(int msg1, int msg2, int msg3)
{
//    if(msg1 == MSG_VERSION){
//#ifdef Q_OS_UNIX
//        myMqttMsgParaseThread->n_uploadPartsStatusFlag = 1;
//#endif
//        g_infoWidget().hide();
//        updateVersionInfo();
//        updateTableWidget();
//    }
}

void SysIpcBadPointsWidget::updateTableWidget()
{
    g_infoWidget().hide();
    QStringList list;
    list<<g_myLan().ids_date<<g_myLan().ids_Type<<g_myLan().ids_eza;
    ui->tableWidget->setHorizontalHeaderLabels(list);
    ui->tableWidget->setRowCount(getThread->m_verinfoVec.size());
    for(int i = 0;i< getThread->m_verinfoVec.size();++i){
        ui->tableWidget->setItem(i,0, new QTableWidgetItem(getThread->m_verinfoVec.at(i).strDate));
        ui->tableWidget->setItem(i,1, new QTableWidgetItem(getThread->m_verinfoVec.at(i).modelName));
        ui->tableWidget->setItem(i,2, new QTableWidgetItem(QString::number(getThread->m_verinfoVec.at(i).ezaValue)));

        for(int j= 0;j < 3;j++){
            ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            ui->tableWidget->item(i,j)->setFlags(Qt::ItemIsEnabled);
        }
    }
    ui->pushButton->setVisible(struGsh.bRunMode >= RUNMODE_FACTORY);
}

void SysIpcBadPointsWidget::on_search_clicked()
{
    //xiewj:色选时不允许查询版本，as此时正在连续采图
    if(struGsh.bSortStart)
    {
        g_infoWidget().setLabelText(g_myLan().msg_turn_off_first);
        g_infoWidget().showSecs();
        return;
    }
    if(g_Runtime().modelFullName == "" || g_Runtime().modelFullNameList.length() > struIpcShare.struIpcModelAble[struGsh.nView][struGsh.nUnit].modelCount)
    {
        g_infoWidget().setLabelText(g_myLan().connect_ipc_fail);
        g_infoWidget().showSecs();
        return;
    }
    startDateStr = QString("%1-").arg(m_dateTime[0][0],4,10,QChar('0'))
            +QString("%1-").arg(m_dateTime[0][1],2,10,QChar('0'))
            +QString("%1").arg(m_dateTime[0][2],2,10,QChar('0'));
    endDateStr = QString("%1-").arg(m_dateTime[1][0],4,10,QChar('0'))
            +QString("%1-").arg(m_dateTime[1][1],2,10,QChar('0'))
            +QString("%1").arg(m_dateTime[1][2],2,10,QChar('0'));

    g_infoWidget().setLabelText(g_myLan().searching);
    g_infoWidget().delayShow();
    getThread->start();
}


/******************************************** 含杂查询线程 ***********************************************/
void getBadPointsThread::run()
{
    m_dateStr = m_pParentWidget->endDateStr;
    m_modelNmae = g_Runtime().modelFullName;
    m_value = 0;
    m_verinfoVec.clear();
    BadPointInfo info(m_dateStr,m_modelNmae,m_value);
    MySqlite mysql;
    if(!mysql.openDatabase())
    {
        g_infoWidget().setLabelText(g_myLan().ids_connectSqlFailed);
        g_infoWidget().showSecs();
        qDebug()<<"STATISTICDB open fail!";
        return ;
    }
    QSqlQuery query(mysql.db);
    if(struIpcShare.allIpcModelLoadOk == 2 && struIpcShare.allIpcConnecOk == 1)
    {
        for(int i =0;i<struIpcShare.struIpcModelAble[struGsh.nView][struGsh.nUnit].modelCount;i++)
        {
            sql = "select ";
            sql.append("CreatedTime, eza from ");
            sql.append(g_Runtime().modelFullNameList.at(i));
            sql.append(" where CreatedTime Between '");
            sql.append(m_pParentWidget->startDateStr);
            sql.append("' And '");
            sql.append(m_pParentWidget->endDateStr);
            sql.append("'");
            query.exec(sql);
            while(query.next())
            {
                qDebug()<<query.value(0).toString()<<query.value(1).toString();
                m_dateStr = query.value(0).toString();
                m_value = query.value(1).toInt();
                m_modelNmae = g_Runtime().modelFullNameList.at(i);
                info = BadPointInfo(m_dateStr,m_modelNmae,m_value);
                m_verinfoVec.push_back(info);
            }
        }
    }
    else
    {
        sql = "select ";
        sql.append("CreatedTime, eza from ");
        sql.append(g_Runtime().modelFullName);
        sql.append(" where CreatedTime Between '");
        sql.append(m_pParentWidget->startDateStr);
        sql.append("' And '");
        sql.append(m_pParentWidget->endDateStr);
        sql.append("'");
        query.exec(sql);
        while(query.next())
        {
            qDebug()<<query.value(0).toString()<<query.value(1).toString();
            m_dateStr = query.value(0).toString();
            m_value = query.value(1).toInt();
            m_modelNmae = g_Runtime().modelFullName;
            info = BadPointInfo(m_dateStr,m_modelNmae,m_value);
            m_verinfoVec.push_back(info);
        }
    }
    mysql.closeDatabase();
    emit sGetBadPointsFinished();
    return;
}

void SysIpcBadPointsWidget::on_pushButton_clicked()
{
    //xiewj:色选时不允许查询版本，as此时正在连续采图
    if(struGsh.bSortStart)
    {
        g_infoWidget().setLabelText(g_myLan().msg_turn_off_first);
        g_infoWidget().showSecs();
        return;
    }
    myMessageBox msgBox(MSG_QUES, g_myLan().cfm_clean_sample);
    int ret = msgBox.exec();
    if (ret == QDialog::Accepted){ //用户操作，同意进行
        MySqlite mysql;
        for(int i =0;i<struIpcShare.struIpcModelAble[struGsh.nView][struGsh.nUnit].modelCount;i++)
        {
            //mysql.deleteTable(g_Runtime().modelFullNameList.at(i));
            mysql.clearTable(g_Runtime().modelFullNameList.at(i));
        }
    }
}
