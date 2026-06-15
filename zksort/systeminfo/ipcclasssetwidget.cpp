#include "ipcclasssetwidget.h"
#include "ui_ipcclasssetwidget.h"

IpcClassSetWidget::IpcClassSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::IpcClassSetWidget)
{
    ui->setupUi(this);

    //m_nIndexOfPage = 0;
    m_nameClassEdit[0]= ui->class1NameEdit;
    m_nameClassEdit[1]= ui->class1NameEdit_2;
    m_nameClassEdit[2]= ui->class1NameEdit_3;
    m_nameClassEdit[3]= ui->class1NameEdit_4;
    m_nameClassEdit[4]= ui->class1NameEdit_5;
    m_nameClassEdit[5]= ui->class1NameEdit_6;
    m_nameClassEdit[6]= ui->class1NameEdit_7;
    m_nameClassEdit[7]= ui->class1NameEdit_8;
    m_nameClassEdit[8]= ui->class1NameEdit_9;
    m_nameClassEdit[9]= ui->class1NameEdit_10;
    m_nameClassEdit[10]= ui->class1NameEdit_11;
    m_nameClassEdit[11]= ui->class1NameEdit_12;
    m_nameClassEdit[12]= ui->class1NameEdit_13;
    m_nameClassEdit[13]= ui->class1NameEdit_14;
    m_nameClassEdit[14]= ui->class1NameEdit_15;
    m_nameClassEdit[15]= ui->class1NameEdit_16;
    m_nameClassEdit[16]= ui->class1NameEdit_17;
    m_nameClassEdit[17]= ui->class1NameEdit_18;
    m_nameClassEdit[18]= ui->class1NameEdit_19;
    m_nameClassEdit[19]= ui->class1NameEdit_20;
    m_nameClassEdit[20]= ui->class1NameEdit_21;
    m_nameClassEdit[21]= ui->class1NameEdit_22;
    m_nameClassEdit[22]= ui->class1NameEdit_23;
    m_nameClassEdit[23]= ui->class1NameEdit_24;
    m_nameClassEdit[24]= ui->class1NameEdit_25;
    m_nameClassEdit[25]= ui->class1NameEdit_26;
    m_nameClassEdit[26]= ui->class1NameEdit_27;
    m_nameClassEdit[27]= ui->class1NameEdit_28;
    m_nameClassEdit[28]= ui->class1NameEdit_29;
    m_nameClassEdit[29]= ui->class1NameEdit_30;
    m_nameClassEdit[30]= ui->class1NameEdit_31;
    m_nameClassEdit[31]= ui->class1NameEdit_32;
    m_nameClassEdit[32]= ui->class1NameEdit_33;
    m_nameClassEdit[33]= ui->class1NameEdit_34;
    m_nameClassEdit[34]= ui->class1NameEdit_35;
    m_nameClassEdit[35]= ui->class1NameEdit_36;
    m_nameClassEdit[36]= ui->class1NameEdit_37;
    m_nameClassEdit[37]= ui->class1NameEdit_38;
    m_nameClassEdit[38]= ui->class1NameEdit_39;
    m_nameClassEdit[39]= ui->class1NameEdit_40;
    m_nameClassEdit[40]= ui->class1NameEdit_41;
    m_nameClassEdit[41]= ui->class1NameEdit_42;
    m_nameClassEdit[42]= ui->class1NameEdit_43;
    m_nameClassEdit[43]= ui->class1NameEdit_44;
    m_nameClassEdit[44]= ui->class1NameEdit_45;
    m_nameClassEdit[45]= ui->class1NameEdit_46;
    m_nameClassEdit[46]= ui->class1NameEdit_47;
    m_nameClassEdit[47]= ui->class1NameEdit_48;
    m_nameClassEdit[48]= ui->class1NameEdit_49;
    m_nameClassEdit[49]= ui->class1NameEdit_50;
    m_nameClassEdit[50]= ui->class1NameEdit_51;
    m_nameClassEdit[51]= ui->class1NameEdit_52;



    m_pClassStatisticsLineEdit[0]= ui->lineEditStatistic1_1;
    m_pClassStatisticsLineEdit[1]= ui->lineEditStatistic1_2;
    m_pClassStatisticsLineEdit[2]= ui->lineEditStatistic1_3;
    m_pClassStatisticsLineEdit[3]= ui->lineEditStatistic1_4;
    m_pClassStatisticsLineEdit[4]= ui->lineEditStatistic1_5;
    m_pClassStatisticsLineEdit[5]= ui->lineEditStatistic1_6;
    m_pClassStatisticsLineEdit[6]= ui->lineEditStatistic1_7;
    m_pClassStatisticsLineEdit[7]= ui->lineEditStatistic1_8;
    m_pClassStatisticsLineEdit[8]= ui->lineEditStatistic1_9;
    m_pClassStatisticsLineEdit[9]= ui->lineEditStatistic1_10;
    m_pClassStatisticsLineEdit[10]= ui->lineEditStatistic1_11;
    m_pClassStatisticsLineEdit[11]= ui->lineEditStatistic1_12;
    m_pClassStatisticsLineEdit[12]= ui->lineEditStatistic1_13;
    m_pClassStatisticsLineEdit[13]= ui->lineEditStatistic1_14;
    m_pClassStatisticsLineEdit[14]= ui->lineEditStatistic1_15;
    m_pClassStatisticsLineEdit[15]= ui->lineEditStatistic1_16;
    m_pClassStatisticsLineEdit[16]= ui->lineEditStatistic1_17;
    m_pClassStatisticsLineEdit[17]= ui->lineEditStatistic1_18;
    m_pClassStatisticsLineEdit[18]= ui->lineEditStatistic1_19;
    m_pClassStatisticsLineEdit[19]= ui->lineEditStatistic1_20;
    m_pClassStatisticsLineEdit[20]= ui->lineEditStatistic1_21;
    m_pClassStatisticsLineEdit[21]= ui->lineEditStatistic1_22;
    m_pClassStatisticsLineEdit[22]= ui->lineEditStatistic1_23;
    m_pClassStatisticsLineEdit[23]= ui->lineEditStatistic1_24;
    m_pClassStatisticsLineEdit[24]= ui->lineEditStatistic1_25;
    m_pClassStatisticsLineEdit[25]= ui->lineEditStatistic1_26;
    m_pClassStatisticsLineEdit[26]= ui->lineEditStatistic1_27;
    m_pClassStatisticsLineEdit[27]= ui->lineEditStatistic1_28;
    m_pClassStatisticsLineEdit[28]= ui->lineEditStatistic1_29;
    m_pClassStatisticsLineEdit[29]= ui->lineEditStatistic1_30;
    m_pClassStatisticsLineEdit[30]= ui->lineEditStatistic1_31;
    m_pClassStatisticsLineEdit[31]= ui->lineEditStatistic1_32;
    m_pClassStatisticsLineEdit[32]= ui->lineEditStatistic1_33;
    m_pClassStatisticsLineEdit[33]= ui->lineEditStatistic1_34;
    m_pClassStatisticsLineEdit[34]= ui->lineEditStatistic1_35;
    m_pClassStatisticsLineEdit[35]= ui->lineEditStatistic1_36;
    m_pClassStatisticsLineEdit[36]= ui->lineEditStatistic1_37;
    m_pClassStatisticsLineEdit[37]= ui->lineEditStatistic1_38;
    m_pClassStatisticsLineEdit[38]= ui->lineEditStatistic1_39;
    m_pClassStatisticsLineEdit[39]= ui->lineEditStatistic1_40;
    m_pClassStatisticsLineEdit[40]= ui->lineEditStatistic1_41;
    m_pClassStatisticsLineEdit[41]= ui->lineEditStatistic1_42;
    m_pClassStatisticsLineEdit[42]= ui->lineEditStatistic1_43;
    m_pClassStatisticsLineEdit[43]= ui->lineEditStatistic1_44;
    m_pClassStatisticsLineEdit[44]= ui->lineEditStatistic1_45;
    m_pClassStatisticsLineEdit[45]= ui->lineEditStatistic1_46;
    m_pClassStatisticsLineEdit[46]= ui->lineEditStatistic1_47;
    m_pClassStatisticsLineEdit[47]= ui->lineEditStatistic1_48;
    m_pClassStatisticsLineEdit[48]= ui->lineEditStatistic1_49;
    m_pClassStatisticsLineEdit[49]= ui->lineEditStatistic1_50;
    m_pClassStatisticsLineEdit[50]= ui->lineEditStatistic1_51;
    m_pClassStatisticsLineEdit[51]= ui->lineEditStatistic1_52;


    m_pClassStatisticsQbox[0]= ui->checkBox;
    m_pClassStatisticsQbox[1]= ui->checkBox_2;
    m_pClassStatisticsQbox[2]= ui->checkBox_3;
    m_pClassStatisticsQbox[3]= ui->checkBox_4;
    m_pClassStatisticsQbox[4]= ui->checkBox_5;
    m_pClassStatisticsQbox[5]= ui->checkBox_6;
    m_pClassStatisticsQbox[6]= ui->checkBox_7;
    m_pClassStatisticsQbox[7]= ui->checkBox_8;
    m_pClassStatisticsQbox[8]= ui->checkBox_9;
    m_pClassStatisticsQbox[9]= ui->checkBox_10;
    m_pClassStatisticsQbox[10]= ui->checkBox_11;
    m_pClassStatisticsQbox[11]= ui->checkBox_12;
    m_pClassStatisticsQbox[12]= ui->checkBox_13;
    m_pClassStatisticsQbox[13]= ui->checkBox_14;
    m_pClassStatisticsQbox[14]= ui->checkBox_15;
    m_pClassStatisticsQbox[15]= ui->checkBox_16;
    m_pClassStatisticsQbox[16]= ui->checkBox_17;
    m_pClassStatisticsQbox[17]= ui->checkBox_18;
    m_pClassStatisticsQbox[18]= ui->checkBox_19;
    m_pClassStatisticsQbox[19]= ui->checkBox_20;
    m_pClassStatisticsQbox[20]= ui->checkBox_21;
    m_pClassStatisticsQbox[21]= ui->checkBox_22;
    m_pClassStatisticsQbox[22]= ui->checkBox_23;
    m_pClassStatisticsQbox[23]= ui->checkBox_24;
    m_pClassStatisticsQbox[24]= ui->checkBox_25;
    m_pClassStatisticsQbox[25]= ui->checkBox_26;
    m_pClassStatisticsQbox[26]= ui->checkBox_27;
    m_pClassStatisticsQbox[27]= ui->checkBox_28;
    m_pClassStatisticsQbox[28]= ui->checkBox_29;
    m_pClassStatisticsQbox[29]= ui->checkBox_30;
    m_pClassStatisticsQbox[30]= ui->checkBox_31;
    m_pClassStatisticsQbox[31]= ui->checkBox_32;
    m_pClassStatisticsQbox[32]= ui->checkBox_33;
    m_pClassStatisticsQbox[33]= ui->checkBox_34;
    m_pClassStatisticsQbox[34]= ui->checkBox_35;
    m_pClassStatisticsQbox[35]= ui->checkBox_36;
    m_pClassStatisticsQbox[36]= ui->checkBox_37;
    m_pClassStatisticsQbox[37]= ui->checkBox_38;
    m_pClassStatisticsQbox[38]= ui->checkBox_39;
    m_pClassStatisticsQbox[39]= ui->checkBox_40;
    m_pClassStatisticsQbox[40]= ui->checkBox_41;
    m_pClassStatisticsQbox[41]= ui->checkBox_42;
    m_pClassStatisticsQbox[42]= ui->checkBox_43;
    m_pClassStatisticsQbox[43]= ui->checkBox_44;
    m_pClassStatisticsQbox[44]= ui->checkBox_45;
    m_pClassStatisticsQbox[45]= ui->checkBox_46;
    m_pClassStatisticsQbox[46]= ui->checkBox_47;
    m_pClassStatisticsQbox[47]= ui->checkBox_48;
    m_pClassStatisticsQbox[48]= ui->checkBox_49;
    m_pClassStatisticsQbox[49]= ui->checkBox_50;
    m_pClassStatisticsQbox[50]= ui->checkBox_51;
    m_pClassStatisticsQbox[51]= ui->checkBox_52;

    for( int i = 0; i < MAX_IPC_MODEL_CLASS; i++ )
    {
        m_nameClassEdit[i]->setType(textType);
        m_nameClassEdit[i]->setEnabled(true);
        m_pClassStatisticsLineEdit[i]->setType(intType);
        m_pClassStatisticsQbox[i]->setCheckable(true);
        m_pClassStatisticsQbox[i]->setText("");
    }

    ////////////////
    m_nameBigClassEdit[0]= ui->bigclass1NameEdit_1;
    m_nameBigClassEdit[1]= ui->bigclass1NameEdit_2;
    m_nameBigClassEdit[2]= ui->bigclass1NameEdit_3;
    m_nameBigClassEdit[3]= ui->bigclass1NameEdit_4;
    m_nameBigClassEdit[4]= ui->bigclass1NameEdit_5;
    m_nameBigClassEdit[5]= ui->bigclass1NameEdit_6;
    m_nameBigClassEdit[6]= ui->bigclass1NameEdit_7;
    m_nameBigClassEdit[7]= ui->bigclass1NameEdit_8;
    m_nameBigClassEdit[8]= ui->bigclass1NameEdit_9;
    m_nameBigClassEdit[9]= ui->bigclass1NameEdit_10;
    m_nameBigClassEdit[10]= ui->bigclass1NameEdit_11;
    m_nameBigClassEdit[11]= ui->bigclass1NameEdit_12;
    m_nameBigClassEdit[12]= ui->bigclass1NameEdit_13;
    m_nameBigClassEdit[13]= ui->bigclass1NameEdit_14;
    m_nameBigClassEdit[14]= ui->bigclass1NameEdit_15;
    m_nameBigClassEdit[15]= ui->bigclass1NameEdit_16;
    m_nameBigClassEdit[16]= ui->bigclass1NameEdit_17;
    m_nameBigClassEdit[17]= ui->bigclass1NameEdit_18;
    m_nameBigClassEdit[18]= ui->bigclass1NameEdit_19;
    m_nameBigClassEdit[19]= ui->bigclass1NameEdit_20;

    m_pBigClassStatisticsCoeff[0]= ui->lineEditStatisticId_1;
    m_pBigClassStatisticsCoeff[1]= ui->lineEditStatisticId_2;
    m_pBigClassStatisticsCoeff[2]= ui->lineEditStatisticId_3;
    m_pBigClassStatisticsCoeff[3]= ui->lineEditStatisticId_4;
    m_pBigClassStatisticsCoeff[4]= ui->lineEditStatisticId_5;
    m_pBigClassStatisticsCoeff[5]= ui->lineEditStatisticId_6;
    m_pBigClassStatisticsCoeff[6]= ui->lineEditStatisticId_7;
    m_pBigClassStatisticsCoeff[7]= ui->lineEditStatisticId_8;
    m_pBigClassStatisticsCoeff[8]= ui->lineEditStatisticId_9;
    m_pBigClassStatisticsCoeff[9]= ui->lineEditStatisticId_10;
    m_pBigClassStatisticsCoeff[10]= ui->lineEditStatisticId_11;
    m_pBigClassStatisticsCoeff[11]= ui->lineEditStatisticId_12;
    m_pBigClassStatisticsCoeff[12]= ui->lineEditStatisticId_13;
    m_pBigClassStatisticsCoeff[13]= ui->lineEditStatisticId_14;
    m_pBigClassStatisticsCoeff[14]= ui->lineEditStatisticId_15;
    m_pBigClassStatisticsCoeff[15]= ui->lineEditStatisticId_16;
    m_pBigClassStatisticsCoeff[16]= ui->lineEditStatisticId_17;
    m_pBigClassStatisticsCoeff[17]= ui->lineEditStatisticId_18;
    m_pBigClassStatisticsCoeff[18]= ui->lineEditStatisticId_19;
    m_pBigClassStatisticsCoeff[19]= ui->lineEditStatisticId_20;

    m_pBigClassStatisticsId[0]= ui->label_2;
    m_pBigClassStatisticsId[1]= ui->label_3;
    m_pBigClassStatisticsId[2]= ui->label_5;
    m_pBigClassStatisticsId[3]= ui->label_6;
    m_pBigClassStatisticsId[4]= ui->label_7;
    m_pBigClassStatisticsId[5]= ui->label_8;
    m_pBigClassStatisticsId[6]= ui->label_9;
    m_pBigClassStatisticsId[7]= ui->label_10;
    m_pBigClassStatisticsId[8]= ui->label_11;
    m_pBigClassStatisticsId[9]= ui->label_12;
    m_pBigClassStatisticsId[10]= ui->label_13;
    m_pBigClassStatisticsId[11]= ui->label_14;
    m_pBigClassStatisticsId[12]= ui->label_15;
    m_pBigClassStatisticsId[13]= ui->label_16;
    m_pBigClassStatisticsId[14]= ui->label_17;
    m_pBigClassStatisticsId[15]= ui->label_18;
    m_pBigClassStatisticsId[16]= ui->label_19;
    m_pBigClassStatisticsId[17]= ui->label_20;
    m_pBigClassStatisticsId[18]= ui->label_21;
    m_pBigClassStatisticsId[19]= ui->label_22;


    m_classNameEditSigMapper = new QSignalMapper(this);
    for( int i = 0; i < BIGCLASSNUM; i++ )
    {
        m_nameBigClassEdit[i]->setType(textType);
        m_classNameEditSigMapper->setMapping(m_nameBigClassEdit[i], i);
        connect(m_nameBigClassEdit[i],SIGNAL(valueSetSignal()),m_classNameEditSigMapper,SLOT(map()));
        m_pBigClassStatisticsCoeff[i]->setType(intType);
        m_pBigClassStatisticsCoeff[i]->setEnabled(true);
    }
    connect(m_classNameEditSigMapper,SIGNAL(mapped(int)),this,SLOT(sClass1NameChanged(int)));

    m_alarmEnableSigMapper = new QSignalMapper(this);
    for(int i = 0; i < MAX_IPC_MODEL_CLASS; i++){
        m_alarmEnableSigMapper->setMapping(m_pClassStatisticsQbox[i], i);
        m_pClassStatisticsQbox[i]->setObjectName("ipcClassEnale");
        connect(m_pClassStatisticsQbox[i],SIGNAL(clicked()),m_alarmEnableSigMapper,SLOT(map()));


    }
    connect(m_alarmEnableSigMapper,SIGNAL(mapped(int)),this,SLOT(sAlarmEnableSlots(int)));


    bigclassNameStrList.clear();
    currentpiciname =  QDateTime::currentDateTime().toString("yyyyMMdd")+ "-000";
    ui->currentpiciname->setType(textType);

}

IpcClassSetWidget::~IpcClassSetWidget()
{
    delete ui;
}

void IpcClassSetWidget::showPage(bool bshow)
{
    if(bshow)
    {
        //m_nIndexOfPage = 0;
        bigclassNameStrList.clear();
        memcpy(&m_temCnfg,&struCnfg,sizeof(m_temCnfg));
        memcpy(&m_temCnfp,&struCnfp,sizeof(m_temCnfp));

        ui->setBigClassNum->setParams(this, 0, BIGCLASSNUM, 1, &m_temCnfp.nIpcStatisticsBigclass );
        ui->setBigClassNum->setTitle(g_myLan().classGroupNum);
        ui->setBigClassNum->setValue( m_temCnfp.nIpcStatisticsBigclass );
        updateClassInfo();
        updateClassShow();
        for(int i = 1; i <= BIGCLASSNUM; i++)
        {
            QByteArray sMaterialArr(struCnfp.sCurBigstatisname[i-1]);
            QString tmp = QString::fromLocal8Bit(sMaterialArr);
            bigclassNameStrList.append(tmp); //
        }
        updateBigClassShow();

        QSettings setting(CFG_APPSet,QSettings::IniFormat);
        QString tempname =  setting.value("piciname" ,"").toString();
        ui->currentpiciname->setText(tempname);
    }
    else
    {
    }

}

void IpcClassSetWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){
        if(msg2 == 1)
        {
            updateBigClassShow();
        }
        if(msg2 == 4)
        {
            updateClassInfo();
            updateClassShow();
        }
    }
}

void IpcClassSetWidget::updateClassInfo()
{
    classNameStrList.clear();
    for(int i = 0; i < MAX_IPC_MODEL_CLASS; i++)
    {
        classNameStrList.append(QString("%1").arg( struCnfp.struCameraIpcClassGroupParams[struGsh.nView][0].className[i] ));
    }
}

void IpcClassSetWidget::updateClassShow()
{
    for(int j = 0; j < MAX_IPC_MODEL_CLASS; j++)
    {

        m_nameClassEdit[j]->setText("");
        if(j >=  m_temCnfp.ipcModelClassCount[struGsh.nView][0])
        {
            m_nameClassEdit[j]->setVisible(false);
            m_pClassStatisticsLineEdit[j]->setVisible(false);
            m_pClassStatisticsQbox[j]->setVisible(false);
        }
        else
        {
            m_nameClassEdit[j]->setVisible(true);
            m_nameClassEdit[j]->setEnabled(false);
            m_pClassStatisticsLineEdit[j]->setVisible(true);
            m_pClassStatisticsQbox[j]->setVisible(true);
            m_nameClassEdit[j]->setText( classNameStrList.at(j) );
            m_pClassStatisticsLineEdit[j]->setMinMax( this,0, MAX_IPC_MODEL_CLASS, 2, &m_temCnfp.nIpcStatisticsclassId[j] );
            m_pClassStatisticsLineEdit[j]->setText( QString::number( m_temCnfp.nIpcStatisticsclassId[j] ));
            m_pClassStatisticsQbox[j]->setChecked(m_temCnfp.bstaticClassAlarm[j]== 1);
        }
    }
    //updatePagelabel();
}

void IpcClassSetWidget::updateBigClassShow()
{
    for( int i = 0; i < BIGCLASSNUM; i++ )
    {
        m_nameBigClassEdit[i]->setText("");
        m_pBigClassStatisticsId[i]->setText(QString::number(i+1));
        m_pBigClassStatisticsCoeff[i]->setVisible(true);
        m_nameBigClassEdit[i]->setVisible(true);
        m_pBigClassStatisticsId[i]->setVisible(true);

        if(i >= m_temCnfp.nIpcStatisticsBigclass )
        {
            m_nameBigClassEdit[i]->setVisible(false);
            m_pBigClassStatisticsId[i]->setVisible(false);
            m_pBigClassStatisticsCoeff[i]->setVisible(false);
        }
        else
        {

            QByteArray sMaterialArr(m_temCnfp.sCurBigstatisname[i]);
            QString tmp = QString::fromLocal8Bit(sMaterialArr);
            m_nameBigClassEdit[i]->setText(tmp);

            m_pBigClassStatisticsCoeff[i]->setMinMax( this,0, 1000, 3, &m_temCnfp.nIpcStatisticsbigclassCoeff[i] );
            m_pBigClassStatisticsCoeff[i]->setText(QString::number(m_temCnfp.nIpcStatisticsbigclassCoeff[i]));
        }
    }
}


void IpcClassSetWidget::on_pushButton_clicked()
{

    g_MainManager().returnBackPage();
}

void IpcClassSetWidget::on_pushButton_2_clicked()
{
    bigclassNameStrList.clear();
    for(int i = 0; i < m_temCnfp.nIpcStatisticsBigclass  ; i++)
    {
        if(m_nameBigClassEdit[i]->text() == "")
        {
            g_infoWidget().setLabelText(g_myLan().data_not_entered);
            g_infoWidget().delayShow();
            myFlow.sleep(1);
            g_infoWidget().hide();
            return;
        }
        QByteArray sMaterialArr = m_nameBigClassEdit[i]->text().toUtf8();

        memset(m_temCnfp.sCurBigstatisname[i], 0,
               sizeof(m_temCnfp.sCurBigstatisname[i]));
        memcpy(m_temCnfp.sCurBigstatisname[i],
               sMaterialArr.data(), sMaterialArr.length());
        bigclassNameStrList.append(m_nameBigClassEdit[i]->text()); // 更新大类型名称
    }


    memcpy(&struCnfg,&m_temCnfg,sizeof(struCnfGlobal));
    memcpy(&struCnfp,&m_temCnfp,sizeof(struCnfProfile));
    g_Runtime().save();
    g_MainManager().returnBackPage();
}

void IpcClassSetWidget::retranslateUiWidget()
{
    ui->label->setText(g_myLan().batchName);

    ui->setBigClassNum->setTitle(g_myLan().classGroupNum);
    ui->pushButton->setText(g_myLan().ids_cancel);
    ui->pushButton_2->setText(g_myLan().ids_ok);
    ui->label_24->setText(g_myLan().ipc_typeName);
    ui->label_23->setText(g_myLan().ids_index);
    ui->label_25->setText(g_myLan().staticClassname);
    ui->label_26->setText(g_myLan().ids_capactiy+g_myLan().lamp_coef);
    ui->labelstastics1_1->setText(g_myLan().classType);
    ui->label_4->setText(g_myLan().ids_alarm);
}



void IpcClassSetWidget::on_currentpiciname_textChanged(const QString &arg1)
{
    if (arg1 != "")
    {
        currentpiciname = arg1;
        QSettings setting(CFG_APPSet,QSettings::IniFormat);
        QString tempname = "piciname";
        setting.setValue(tempname ,currentpiciname);
        setting.sync();
    }
}

//void IpcClassSetWidget::updatePagelabel()
//{
//    /* 更新页码显示 */
//    int tmp = struCnfp.ipcModelClassCount[struGsh.nView][0]% BIGCLASSNUM;
//    if (tmp == 0)
//        m_nTotalPage = struCnfp.ipcModelClassCount[struGsh.nView][0]/BIGCLASSNUM;
//    else
//        m_nTotalPage = struCnfp.ipcModelClassCount[struGsh.nView][0]/BIGCLASSNUM + 1;
//    QString str = QString("%1/%2").arg(m_nIndexOfPage+1).arg(m_nTotalPage);
//    ui->curPagelabel->setText(str);
//}

//void IpcClassSetWidget::on_lastPageBtn_clicked()
//{
//    if (m_nIndexOfPage >= 1) {
//        m_nIndexOfPage--;

//    }
//    updateClassShow();
//}

//void IpcClassSetWidget::on_nextPageBtn_clicked()
//{
//    if (m_nIndexOfPage < (m_nTotalPage-1))
//    {
//        m_nIndexOfPage++;
//    }
//    updateClassShow();
//}

void IpcClassSetWidget::sClass1NameChanged(int index)
{

    QString name = m_nameBigClassEdit[index]->text();
    QByteArray namearr = name.toUtf8();
    const char *strName = namearr.data();
    memcpy(m_temCnfp.sCurBigstatisname[index],strName,strlen(strName));
}

void IpcClassSetWidget::sAlarmEnableSlots(int index)
{
    if(m_temCnfp.bstaticClassAlarm[index] ==0)
        m_temCnfp.bstaticClassAlarm[index] = 1;
    else
        m_temCnfp.bstaticClassAlarm[index] = 0;

}

