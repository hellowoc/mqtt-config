#include "sysoutputwidget.h"
#include "ui_sysoutputwidget.h"
#include "global.h"
#include "ipcclasssetwidget.h"
#include "qualitychart.h"
SysOutputWidget::SysOutputWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::SysOutputWidget)
{
    ui->setupUi(this);


    m_dateTimeWidget[0] = ui->lineEdit_y;
    m_dateTimeWidget[1] = ui->lineEdit_m;
    m_dateTimeWidget[2] = ui->lineEdit_d;
    m_dateTimeWidget[0]->setType(intType);
    m_dateTimeWidget[1]->setType(intType);
    m_dateTimeWidget[2]->setType(intType);
    m_dateTimeWidget[0]->setMinMax(this, 2023, 2050, 1, &year);
    m_dateTimeWidget[1]->setMinMax(this, 1, 12, 1, &month);
    m_dateTimeWidget[2]->setMinMax(this, 1, 31, 1, &day);
    currenshowtype = 1; // 默认直方图显示
    ui->comboBoxtype->setCurrentIndex(currenshowtype);
    ui->stackedWidget->setCurrentIndex(currenshowtype); //

    picinameList.clear();
    picinameList.append("ALL");// 默认所有批次
    ui->comboBox->addItems(picinameList);
    ui->comboBox->setCurrentIndex(0);
    ui->comboBox->hide();

    viewnameList.clear();
    viewMap.clear();

    for(int k=0;k<MAX_VIEW;k++)
    {
        if (struCnfg.struLayerInfo[0].nViewIsUsed[k])
        {

            switch (k) {
            case 0:
                viewnameList.append(QString(g_myLan().ids_fm));
                viewMap.insert(QString(g_myLan().ids_fm),k);
                break;
            case 1:
                viewnameList.append(QString(g_myLan().ids_bm));
                viewMap.insert(QString(g_myLan().ids_bm),k);
                break;
            case 2:
                viewnameList.append(QString(g_myLan().ids_fs));
                viewMap.insert(QString(g_myLan().ids_fs),k);
                break;
            case 3:
                viewnameList.append(QString(g_myLan().ids_bs));
                viewMap.insert(QString(g_myLan().ids_bs),k);
                break;
            case 4:
                viewnameList.append("II"+QString(g_myLan().ids_fm));
                viewMap.insert("II"+QString(g_myLan().ids_fm),k);
                break;
            case 5:
                viewnameList.append("II"+QString(g_myLan().ids_bm));
                viewMap.insert("II"+QString(g_myLan().ids_bm),k);
                break;
            case 6:
                viewnameList.append("II"+QString(g_myLan().ids_fs));
                viewMap.insert("II"+QString(g_myLan().ids_fs),k);
                break;
            case 7:
                viewnameList.append("II"+QString(g_myLan().ids_bs));
                viewMap.insert("II"+QString(g_myLan().ids_bs),k);
                break;
            default:
                break;
            }


        }//end used view

    }
    ui->comboBox_view->addItems(viewnameList);
    currentview= viewMap[viewnameList.at(0)];

    QStringList comlist;
    comlist.clear();
    comlist<<g_myLan().Material_count<<g_myLan().Material_area<<g_myLan().Frame_area;
    ui->comboBox_2->addItems(comlist);
    ui->comboBox_2->hide();

}


/*更新数据后上传*/
void SysOutputWidget:: uploadMulScreendata()
{
    // 先不考虑上传

    //    g_MultSceUdp.materialcountsupload(StaticClassNameList.size(),name, result);
}

SysOutputWidget::~SysOutputWidget()
{
    delete ui;

}

void SysOutputWidget::showPage(bool bshow)
{
    if(bshow)
    {

        /*更新界面时间显示 显示当前的时间信息*/
        QString currentdaytime = QDateTime::currentDateTime().toString("yyyy_MM_dd");
        year  = currentdaytime.split("_").at(0).toInt();
        month = currentdaytime.split("_").at(1).toInt();
        day   = currentdaytime.split("_").at(2).toInt();

        m_dateTimeWidget[0]->setText(QString::number(year));
        m_dateTimeWidget[1]->setText(QString::number(month));
        m_dateTimeWidget[2]->setText(QString::number(day));
        ui->comboBox_2->setCurrentIndex(struCnfg.bGetMaterialType);
        //   ui->comboBox_view->setCurrentIndex(currentview);
        ui->comboBox_view->setCurrentIndex((viewnameList.indexOf(viewMap.key(currentview))));
        getThread->currentview = currentview;
        getseektime();
        getcurrentpicinameandinfo();
        updatepicinfolist();
        connect(getThread,SIGNAL(sGetClassPointsFinished()),this,SLOT(updateshowdata()) );

        for(int j = 0; j < MAX_IPC_MODEL_CLASS; j++)
        {


            qDebug()<<j<<struCnfp.bstaticClassAlarm[j];
        }

    }
    else
    {
        disconnect(getThread,SIGNAL(sGetClassPointsFinished()),this,SLOT(updateshowdata()) );
    }

}

void SysOutputWidget::updateshowpage()
{
    if(struGsh.bRunMode == RUNMODE_USER) // 用户模式隐藏
    {
        ui->exportdataBtn->hide();
    }
    else
    {
        ui->exportdataBtn->show();
    }
}

void SysOutputWidget::retranslateUiWidget()
{
    ui->label_2->setText(g_myLan().year);
    ui->label_3->setText(g_myLan().month);
    ui->label_5->setText(g_myLan().day);
    ui->search->setText(g_myLan().ids_query);
    ui->classSet->setText(g_myLan().staticSet);
    ui->exportdataBtn->setText(g_myLan().ids_exportReport);
    ui->comboBoxtype->clear();
    QStringList showtype ;
    showtype<<g_myLan().allDayData<<g_myLan().staticClassdata;//<<g_myLan().PieChart;
    ui->comboBoxtype->addItems(showtype);
    ui->comboBoxtype->setCurrentIndex(currenshowtype);
    ui->deletedataBtn->setText(g_myLan().ids_delete);
}

void SysOutputWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    getseektime();
}

void SysOutputWidget::on_comboBox_activated(int index)
{
    ui->comboBox->setCurrentIndex(index);
    getThread->currentpiciname=picinameList.at(index);
    struGsh.updateIpcStatisdata = 1;
}

void SysOutputWidget::on_search_clicked()
{
    if(struGsh.bSortStart)
    {
        g_infoWidget().setLabelText(g_myLan().msg_turn_off_first);
        g_infoWidget().showSecs();
        return;
    }
    struGsh.updateIpcStatisdata = 1;

    g_infoWidget().setLabelText(g_myLan().ids_query);
    g_infoWidget().delayShow();


    for(int i = 0; i < 30; i++)
    {
        myFlow.sleep(1);
        if(struGsh.updateIpcStatisdata == 1)
            continue;
    }
    g_infoWidget().hide();

}

void SysOutputWidget::on_classSet_clicked()
{
    if(struGsh.bSortStart)
    {
        g_infoWidget().setLabelText(g_myLan().msg_turn_off_first);
        g_infoWidget().showSecs();
        return;
    }
    g_MainManager().ShowWidget(SM_IPC_CLASSSET_WIDGET);
}



void SysOutputWidget::on_comboBoxtype_activated(int index)
{
    currenshowtype = index;
    ui->stackedWidget->setCurrentIndex(currenshowtype); //
}


void  SysOutputWidget::getseektime()
{
    year  =  m_dateTimeWidget[0]->text().toInt();
    month =  m_dateTimeWidget[1]->text().toInt();
    day   =  m_dateTimeWidget[2]->text().toInt();
    seektime = QString::number(year);
    if(month<10)
        seektime += QString("0%1").arg(month);
    else
        seektime += QString("%1").arg(month);
    if(day<10)
        seektime += QString("0%1").arg(day);
    else
        seektime += QString("%1").arg(day);

    getThread->seektime = seektime;
}

void  SysOutputWidget::getcurrentpicinameandinfo()
{
    /* 获取当前的批次名称和批次信息*/
    QSettings setting(CFG_APPSet,QSettings::IniFormat);
    QString tempname;
    tempname = setting.value("piciname","000").toString();
    g_Runtime().piciname =tempname;

}


void SysOutputWidget::on_exportdataBtn_clicked()
{

    if(struGsh.bSortStart)
    {
        g_infoWidget().setLabelText(g_myLan().msg_turn_off_first);
        g_infoWidget().showSecs();
        return;
    }

#ifdef Q_OS_UNIX

    if(!g_Runtime().checkUsbExist())
    {
        g_infoWidget().setLabelText(g_myLan().msg_usb_insert);
        g_infoWidget().showSecs();
        return;
    }
    QString path = g_Runtime().getUsbPath();
    if (path == ""){
        g_infoWidget().setLabelText(g_myLan().msg_usb_not_recognized);
        g_infoWidget().showSecs();
        return;
    }

    g_infoWidget().setLabelText(g_myLan().ids_backup);
    g_infoWidget().delayShow();
    if(g_Runtime().mySystem("cp /sdcard/ts/statisticDataLog.db -rf "+path))
    {
        g_infoWidget().setLabelText(g_myLan().ids_backup+g_myLan().ids_succeeded);
        g_infoWidget().delayShow();
        myFlow.sleep(2);
        g_infoWidget().hide();
    }
    else
    {
        g_infoWidget().setLabelText(g_myLan().ids_backup+g_myLan().dc_save_erro);
        g_infoWidget().delayShow();
        myFlow.sleep(2);
        g_infoWidget().hide();
    }

#endif

}

void SysOutputWidget::on_comboBox_2_activated(int index)
{
    struCnfg.bGetMaterialType = index;
    struGsh.updateIpcStatisdata = 1;

}

void SysOutputWidget::updatepicinfolist()
{
    int currentpici = ui->comboBox->currentIndex();
    ui->comboBox->clear();
    ui->comboBox->addItems(picinameList);
    ui->comboBox->setCurrentIndex(currentpici);
    getThread->currentpiciname=picinameList.at(currentpici);;
}

void SysOutputWidget::updateshowdata()
{
    qDebug()<<"updateshowdata";
    QList<QString> name;
    if(!ui->widget->namelist.isEmpty())
    {
        ui->widget->namelist.clear();
    }
    if(!ui->widget->m_numList.isEmpty())
    {
        ui->widget->m_numList.clear();
    }
    if(!ui->widget->impuritylist.isEmpty())
    {
        ui->widget->impuritylist.clear();
    }
    qDebug()<<"updateshowdata1";
    if(!ui->page->namelist.isEmpty())
    {
        ui->page->namelist.clear();
    }
    if(!ui->page->m_numList.isEmpty())
    {
        ui->page->m_numList.clear();
    }
    if(!ui->page->impuritylist.isEmpty())
    {
        ui->widget->impuritylist.clear();
    }
    qDebug()<<"updateshowdata2";
    if(!ui->page_2->namelist.isEmpty())
    {
        ui->page_2->namelist.clear();
    }
    if(!ui->page->m_numList.isEmpty())
    {
        ui->page_2->m_numList.clear();
    }

    qDebug()<<"updateshowdata3";
    for(int i  = 0; i < 24; i++)
    {
        ui->widget->namelist.append( QString("%1").arg(i) );
        ui->widget->m_numList.append( getThread->m_alltimeValueVec.at(i) );
    }
    ui->page->namelist = getThread->StaticClassNameList;
    ui->page_2->namelist = getThread->StaticClassNameList;
    for(int i = 0; i < getThread->StaticClassNameList.size(); i++)
    {
        name.append(getThread->StaticClassNameList.at(i));
        ui->page->m_numList.append( getThread->m_statictotalValueVec.at(i) );
        ui->page_2->m_numList.append( getThread->m_statictotalValueVec.at(i) );
    }

    ui->widget->updateData();
    ui->page->updateData();
    ui->page_2->updatewidget();


}


void SysOutputWidget::on_comboBox_view_activated(int index)
{
    currentview = index;
    ui->comboBox_view->setCurrentIndex(currentview);
    getThread->currentview = currentview;
    //struGsh.updateIpcStatisdata = 1;
}


void SysOutputWidget::on_deletedataBtn_clicked()
{
    if(struGsh.bSortStart)
    {
        g_infoWidget().setLabelText(g_myLan().msg_turn_off_first);
        g_infoWidget().showSecs();
        return;
    }
    //getseektime();
    year  =  m_dateTimeWidget[0]->text().toInt();
    month =  m_dateTimeWidget[1]->text().toInt();
    day   =  m_dateTimeWidget[2]->text().toInt();
    seektime = QString::number(year);
    if(month<10)
        seektime += QString("0%1").arg(month);
    else
        seektime += QString("%1").arg(month);
    if(day<10)
        seektime += QString("0%1").arg(day);
    else
        seektime += QString("%1").arg(day);

    getThread->seektime = seektime;
    myMessageBox msgBox(MSG_QUES, g_myLan().cfm_clean_sample);
    int ret = msgBox.exec();
    if (ret == QDialog::Accepted)
    { //用户操作，同意进行
        g_infoWidget().setLabelText(g_myLan().msg_applying);
        g_infoWidget().delayShow();
        struGsh.deleteIpcStatisdata = 1;
    }

    for(int i = 0; i < 90; i++)
    {
        myFlow.sleep(1);
        if(struGsh.deleteIpcStatisdata == 1)
            continue;
    }
    myFlow.sleep(1);
    struGsh.updateIpcStatisdata = 1;
    g_infoWidget().hide();
}



void SysOutputWidget::on_comboBox_view_activated(const QString &arg1)
{
    currentview= viewMap[arg1];
    getThread->currentview = currentview;
    ui->comboBox_view->setCurrentIndex((viewnameList.indexOf(viewMap.key(getThread->currentview))));
}
