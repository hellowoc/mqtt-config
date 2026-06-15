#include "groupsetdialog.h"
#include "ui_groupsetdialog.h"

GroupSetDialog::GroupSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GroupSetDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);

    groupbox[0] = ui->groupBox;
    groupbox[1] = ui->groupBox_2;
    groupbox[2] = ui->groupBox_3;
    groupbox[3] = ui->groupBox_4;
    groupbox[4] = ui->groupBox_5;

    m_combox[0][0] = ui->comboBox;
    m_combox[0][1] = ui->comboBox_2;
    m_combox[1][0] = ui->comboBox_3;
    m_combox[1][1] = ui->comboBox_4;
    m_combox[2][0] = ui->comboBox_5;
    m_combox[2][1] = ui->comboBox_6;
    m_combox[3][0] = ui->comboBox_7;
    m_combox[3][1] = ui->comboBox_8;
    m_combox[4][0] = ui->comboBox_9;
    m_combox[4][1] = ui->comboBox_10;
    ui->cancelBtn->setText(g_myLan().ids_cancel);
    ui->okBtn->setText(g_myLan().ids_ok);

    groupbox[0]->setTitle(g_myLan().ids_one);
    groupbox[1]->setTitle(g_myLan().ids_two);
    groupbox[2]->setTitle(g_myLan().ids_three);
    groupbox[3]->setTitle(g_myLan().ids_four);
    groupbox[4]->setTitle(g_myLan().ids_five);



    //updateshow();

}

GroupSetDialog::~GroupSetDialog()
{
    delete ui;
}

void GroupSetDialog::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){


        qDebug()<<"receiveMsg"<<struGsh.strumachineset.groupnum[nView];
        QSettings setting(CFG_MachineSet,QSettings::IniFormat);
        setting.beginGroup("groupset");
        setting.setValue(QString("view%1groupnum").arg(nView),QString::number(struGsh.strumachineset.groupnum[nView]));
        setting.endGroup();
        setting.sync();
        updateshow();
    }

}

void GroupSetDialog::on_cancelBtn_clicked()
{
    reject();
}

void GroupSetDialog::on_okBtn_clicked()
{


    if(!checkgroupset())
    {

        g_infoWidget().setLabelText(g_myLan().msg_times_set_err);
        g_infoWidget().showSecs();
        return;
    }
    else
    {
        updateGroupInfo();
        accept();
    }

}

void GroupSetDialog::updateGroupInfo()
{

    QSettings setting(CFG_MachineSet,QSettings::IniFormat);
    setting.beginGroup("groupset");

    for(int i = 0 ; i < struGsh.strumachineset.groupnum[nView]; i++)
    {
       // tmp = setting.value(QString("view%1group%2").arg(nView).arg(i),"").toString();
        QString tmp = QString("%1-%2").arg(m_combox[i][0]->currentIndex()+1).arg(m_combox[i][1]->currentIndex()+1);
        setting.setValue(QString("view%1group%2").arg(nView).arg(i), tmp);
    }
    setting.endGroup();

}

void GroupSetDialog::updateshow()
{

    qDebug()<<"updateshow";
    QSettings setting(CFG_MachineSet,QSettings::IniFormat);
    QString tmp;
    groupset.clear();
    QStringList chutelist;
    chutelist.clear();

    setting.beginGroup("groupset");
    struGsh.strumachineset.groupnum[nView] = setting.value(QString("view%1groupnum").arg(nView),"1").toInt();
    ui->groupnum->setTitle(g_myLan().ids_byTimes);
    ui->groupnum->setValue(struGsh.strumachineset.groupnum[nView]);
    ui->groupnum->setParams(this,1,5, 0, &struGsh.strumachineset.groupnum[nView]);

    for(int i = 0; i < mLayerInfo.nViewUnitTotal[nView]; i++)
    {
        chutelist<<QString::number(i+1);
    }

    for(int i =0 ;i < MAX_GROUP_IDENTIFY; i++)
    {
        for(int j = 0; j<2; j++)
        {
            m_combox[i][j]->clear();
            m_combox[i][j]->addItems(chutelist);
        }
    }

    for(int i = 0 ; i < 5; i++)
    {
        groupbox[i]->setVisible( i < struGsh.strumachineset.groupnum[nView] );
        tmp = setting.value(QString("view%1group%2").arg(nView).arg(i),"").toString();

        if(tmp.split("-").size() == 2)
        {
            m_combox[i][0]->setCurrentIndex(tmp.split("-").at(0).toInt()-1);
            m_combox[i][1]->setCurrentIndex(tmp.split("-").at(1).toInt()-1);
        }
        else
        {
            m_combox[i][0]->setCurrentIndex(0);
            m_combox[i][1]->setCurrentIndex(mLayerInfo.nViewUnitTotal[nView]-1);
        }
    }
    setting.endGroup();

}


bool  GroupSetDialog::checkgroupset()  // 增加检查机制
{
    QSettings setting(CFG_MachineSet,QSettings::IniFormat);
    QString tmp;
    QList<QPair<int, int>> groups;
    setting.beginGroup("groupset");
    //nGroupnum = setting.value(QString("view%1groupnum").arg(nView),"").toInt();
    for(int i = 0 ; i < struGsh.strumachineset.groupnum[nView]; i++)
    {
        QPair<int,int> tmppair = qMakePair(m_combox[i][0]->currentIndex()+1, m_combox[i][1]->currentIndex()+1);

        groups.push_back(tmppair);

//        tmp = setting.value(QString("view%1group%2").arg(nView).arg(i),"").toString();
//        if(tmp == "")
//        {
//            return 1;
//        }
    }
    setting.endGroup();
    return checkChannelGroupValid(groups, mLayerInfo.nViewUnitTotal[nView]);
}

bool GroupSetDialog::checkChannelGroupValid(const QList<QPair<int, int>>& groups, int MAX_CH)
{
    const int MIN_CH = 1;
   // const int MAX_CH = 12;
    const int MIN_GROUP = 1;
    const int MAX_GROUP = 5;

    // --------------------------
    // 1. 检查分组数量 1~5
    // --------------------------
    int cnt = groups.size();
    if (cnt < MIN_GROUP || cnt > MAX_GROUP) {
        qDebug() << "错误：分组数量必须在1~5之间";
        return false;
    }


    // --------------------------
    // 2. 检查每组：范围1~12、起始<=结束
    // --------------------------
    for (int i = 0; i < cnt; ++i) {
        int s = groups[i].first;
        int e = groups[i].second;

        if (s < MIN_CH || s > MAX_CH || e < MIN_CH || e > MAX_CH) {
            qDebug() << "错误：第" << i + 1 << "组通道超出1~12范围";
            return false;
        }
        if (s > e) {
            qDebug() << "错误：第" << i + 1 << "组起始 > 结束";
            return false;
        }
    }

    // --------------------------
    // 3. 单组特殊判断
    // --------------------------
    if (cnt == 1) {
        if (groups[0].first == 1 && groups[0].second == MAX_CH)
            return true;
        else {
            qDebug() << "错误：单组必须是 1~12";
            return false;
        }
    }

    // --------------------------
    // 4. 多组规则校验
    // --------------------------

    // 4.1 第一组必须从 1 开始
    if (groups.first().first != 1) {
        qDebug() << "错误：第一组必须从通道1开始";
        return false;
    }

    // 4.2 最后一组必须到 12 结束
    if (groups.last().second != MAX_CH) {
        qDebug() << "错误：最后一组必须结束于通道12";
        return false;
    }

    // 4.3 关键：从第2组开始，本组起始必须 == 前一组结束 或 +1
    for (int i = 1; i < cnt; ++i) {
        int prev_end = groups[i - 1].second;
        int curr_start = groups[i].first;

        if (curr_start != prev_end && curr_start != prev_end + 1) {
            qDebug() << "错误：第" << i + 1 << "组起始通道不合法";
            qDebug() << "前一组结束：" << prev_end;
            qDebug() << "本组起始：" << curr_start;
            qDebug() << "必须 = 前一组结束 或 前一组结束+1";
            return false;
        }
    }

    // 所有规则通过
    qDebug() << "✅ 通道分组完全合法！";
    return true;
}

//void GroupSetDialog::on_lineEdit_textChanged(const QString &arg1)
//{
//    if(arg1 == "")
//        return;
//    QSettings setting(CFG_MachineSet,QSettings::IniFormat);
//    setting.beginGroup("groupset");
//    setting.setValue(QString("view%1group%2").arg(nView).arg(0), arg1);
//    setting.endGroup();
//    setting.sync();
//    updateshow();
//}

//void GroupSetDialog::on_lineEdit_2_textChanged(const QString &arg1)
//{
//    if(arg1 == "")
//        return;
//    QSettings setting(CFG_MachineSet,QSettings::IniFormat);
//    setting.beginGroup("groupset");
//    setting.setValue(QString("view%1group%2").arg(nView).arg(1), arg1);
//    setting.endGroup();
//    setting.sync();
//    updateshow();
//}

//void GroupSetDialog::on_lineEdit_3_textChanged(const QString &arg1)
//{
//    if(arg1 == "")
//        return;
//    QSettings setting(CFG_MachineSet,QSettings::IniFormat);
//    setting.beginGroup("groupset");
//    setting.setValue(QString("view%1group%2").arg(nView).arg(2), arg1);
//    setting.endGroup();
//    setting.sync();
//    updateshow();
//}

//void GroupSetDialog::on_lineEdit_4_textChanged(const QString &arg1)
//{
//    if(arg1 == "")
//        return;
//    QSettings setting(CFG_MachineSet,QSettings::IniFormat);
//    setting.setValue(QString("view%1group%2").arg(nView).arg(3), arg1);
//    setting.setValue("view3group",arg1);
//    setting.endGroup();
//    setting.sync();
//    updateshow();
//}

//void GroupSetDialog::on_lineEdit_5_textChanged(const QString &arg1)
//{
//    if(arg1 == "")
//        return;
//    QSettings setting(CFG_MachineSet,QSettings::IniFormat);
//    setting.setValue(QString("view%1group%2").arg(nView).arg(4), arg1);
//    setting.setValue("view4group",arg1);
//    setting.endGroup();
//    setting.sync();
//    updateshow();
//}

////void GroupSetDialog::on_lineEdit_6_textChanged(const QString &arg1)
////{
////    if(arg1 == "")
////        return;
////    QSettings setting(CFG_MachineSet,QSettings::IniFormat);
////    setting.beginGroup("groupset");
////    setting.setValue("view5group",arg1);
////    setting.endGroup();
////    setting.sync();
////    updateshow();
////}

////void GroupSetDialog::on_lineEdit_7_textChanged(const QString &arg1)
////{
////    if(arg1 == "")
////        return;
////    QSettings setting(CFG_MachineSet,QSettings::IniFormat);
////    setting.beginGroup("groupset");
////    setting.setValue("view6group",arg1);
////    setting.endGroup();
////    setting.sync();
////    updateshow();
////}

////void GroupSetDialog::on_lineEdit_8_textChanged(const QString &arg1)
////{
////    if(arg1 == "")
////        return;
////    QSettings setting(CFG_MachineSet,QSettings::IniFormat);
////    setting.beginGroup("groupset");
////    setting.setValue("view7group",arg1);
////    setting.endGroup();
////    setting.sync();
////    updateshow();
////}
