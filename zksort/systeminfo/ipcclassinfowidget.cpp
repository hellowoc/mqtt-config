#include "ipcclassinfowidget.h"
#include "ui_ipcclassinfowidget.h"
const int columnCount = 2;
IPCClassInfoWidget::IPCClassInfoWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::IPCClassInfoWidget)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(columnCount);
    ui->tableWidget->setColumnWidth(0,300);
    ui->tableWidget->setColumnWidth(1,500);
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setFocusPolicy(Qt::NoFocus);
    ui->tableWidget->setShowGrid(false);

    QHeaderView *header = ui->tableWidget->verticalHeader();
    header->setHidden(true);// 隐藏行号

    QHeaderView *horiHeader = ui->tableWidget->horizontalHeader();
    horiHeader->setFixedHeight(50);
    horiHeader->setSectionResizeMode(QHeaderView::Stretch);
}

IPCClassInfoWidget::~IPCClassInfoWidget()
{
    delete ui;
}

void IPCClassInfoWidget::showPage(bool bshow)
{
    if(bshow){
        updateVersionInfo();
        updateTableWidget();
    }
}

void IPCClassInfoWidget::retranslateUiWidget()
{
    ui->search->setText(g_myLan().recheck);
}

void IPCClassInfoWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_VERSION){
        g_infoWidget().hide();
        updateVersionInfo();
        updateTableWidget();
    }
}

void IPCClassInfoWidget::updateVersionInfo()
{
    m_classInfoVec.clear();
    QString namestr,classNameStr;

    stu_ipc_model_class modelClassIf;
    memcpy(&modelClassIf, &struIpcShare.struIpcModelClass[struGsh.nView][struGsh.nUnit],sizeof(modelClassIf));
//    namestr = g_myLan().ids_Type;
//    IPCModelClassInfo info(namestr,"");
//    m_classInfoVec.push_back(info);

    for(int j = 0;j < modelClassIf.classCount;j++){
        namestr = g_myLan().classType + QString("%1").arg(j+1);
        classNameStr = modelClassIf.className[j];
        IPCModelClassInfo info(namestr,classNameStr);
        m_classInfoVec.push_back(info);
    }
}

void IPCClassInfoWidget::updateTableWidget()
{
    QStringList list;
    list<<g_myLan().ipc_num<<g_myLan().ipc_typeName;
    ui->tableWidget->setHorizontalHeaderLabels(list);
    ui->tableWidget->setRowCount(m_classInfoVec.size());
    for(int i = 0;i< m_classInfoVec.size();++i){
        ui->tableWidget->setItem(i,0, new QTableWidgetItem(m_classInfoVec.at(i).strName));

        QString statestr = m_classInfoVec.at(i).strClassName;

        ui->tableWidget->setItem(i,1, new QTableWidgetItem(statestr));

        for(int j= 0;j < columnCount;j++){
            ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            ui->tableWidget->item(i,j)->setFlags(Qt::ItemIsEnabled);
        }
    }
}

#if 1
void IPCClassInfoWidget::on_search_clicked()
{
    //xiewj:色选时不允许查询版本，as此时正在连续采图
    if(struGsh.bSortStart)
    {
        g_infoWidget().setLabelText(g_myLan().msg_turn_off_first);
        g_infoWidget().delayShow();
        myFlow.sleep(1);
        g_infoWidget().hide();
        return;
    }
    g_infoWidget().setLabelText(g_myLan().searching+"...");
    g_infoWidget().delayShow();

    myFlow.getIpcModelInfo(false,struGsh.nView,struGsh.nUnit);
    myFlow.sleep(1);
    g_infoWidget().hide();
    updateVersionInfo();
    updateTableWidget();
}
#endif
