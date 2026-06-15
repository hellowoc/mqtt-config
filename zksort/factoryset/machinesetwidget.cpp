#include "machinesetwidget.h"
#include "ui_machinesetwidget.h"
#include "global.h"
MachineSetWidget::MachineSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::MachineSetWidget)
{
    ui->setupUi(this);
    retranslateUiWidget();

    m_viewCheckBox[0] = ui->front;
    m_viewCheckBox[1] = ui->rear;
    m_viewCheckBox[2] = ui->frontAuxiliary;
    m_viewCheckBox[3] = ui->rearAuxiliary;
    m_viewCheckBox[4] = ui->front_3;
    m_viewCheckBox[5] = ui->rear_3;
    m_viewCheckBox[6] = ui->frontAuxiliary_3;
    m_viewCheckBox[7] = ui->rearAuxiliary_3;

    m_viewGroupset[0] = ui->groupsetBtn_1;
    m_viewGroupset[1] = ui->groupsetBtn_2;
    m_viewGroupset[2] = ui->groupsetBtn_3;
    m_viewGroupset[3] = ui->groupsetBtn_4;
    m_viewGroupset[4] = ui->groupsetBtn_5;
    m_viewGroupset[5] = ui->groupsetBtn_6;
    m_viewGroupset[6] = ui->groupsetBtn_7;
    m_viewGroupset[7] = ui->groupsetBtn_8;


    m_viewXsysCheckBox[0] = ui->frontX;
    m_viewXsysCheckBox[1] = ui->rearX;
    m_viewXsysCheckBox[2] = ui->frontAuxiliaryX;
    m_viewXsysCheckBox[3] = ui->rearAuxiliaryX;
    m_viewXsysCheckBox[4] = ui->frontX_3;
    m_viewXsysCheckBox[5] = ui->rearX_3;
    m_viewXsysCheckBox[6] = ui->frontAuxiliaryX_3;
    m_viewXsysCheckBox[7] = ui->rearAuxiliaryX_3;

    m_pViewUintComboBox[0]  = ui->frontUintComboBox;
    m_pViewUintComboBox[1] = ui->rearUintComboBox;
    m_pViewUintComboBox[2] = ui->frontAuxiliUintComboBox;
    m_pViewUintComboBox[3]  = ui->rearAuxiUintComboBox;
    m_pViewUintComboBox[4]  = ui->frontUintComboBox_3;
    m_pViewUintComboBox[5] = ui->rearUintComboBox_3;
    m_pViewUintComboBox[6] = ui->frontAuxiliUintComboBox_3;
    m_pViewUintComboBox[7]  = ui->rearAuxiUintComboBox_3;


    m_pViewValveComboBox[0]  = ui->frontValveComboBox;
    m_pViewValveComboBox[1] = ui->rearValveComboBox;
    m_pViewValveComboBox[2] = ui->frontAuxiliValveComboBox;
    m_pViewValveComboBox[3]  = ui->rearAuxiValveComboBox;
    m_pViewValveComboBox[4]  = ui->frontValveComboBox_3;
    m_pViewValveComboBox[5] = ui->rearValveComboBox_3;
    m_pViewValveComboBox[6] = ui->frontAuxiliValveComboBox_3;
    m_pViewValveComboBox[7]  = ui->rearAuxiValveComboBox_3;

    m_pVavleCountComboBox[0]  = ui->lowLayerComboBox;
    m_pVavleCountComboBox[1] = ui->upLayerComboBox;

    m_pGroupTitleLbl[0] = ui->label_2;
    m_pGroupTitleLbl[1] = ui->label_3;
    m_pGroupTitleLbl[2] = ui->label_6;
    m_pGroupTitleLbl[3] = ui->label_7;
    m_pGroupTitleLbl[4] = ui->label_4;

    m_groupVavleLabel[0] = ui->label_9;
    m_groupVavleLabel[1] = ui->label_10;
    m_groupVavleLabel[2] = ui->label_11;
    m_groupVavleLabel[3] = ui->label_12;
    m_groupVavleLabel[4] = ui->label_13;

    m_groupVavleBtnDel[0] = ui->delBtn_1;
    m_groupVavleBtnDel[1] = ui->delBtn_2;
    m_groupVavleBtnDel[2] = ui->delBtn_3;
    m_groupVavleBtnDel[3] = ui->delBtn_4;
    m_groupVavleBtnDel[4] = ui->delBtn_5;


    m_groupVavleBtnAdd[0] = ui->addBtn_1;
    m_groupVavleBtnAdd[1] = ui->addBtn_2;
    m_groupVavleBtnAdd[2] = ui->addBtn_3;
    m_groupVavleBtnAdd[3] = ui->addBtn_4;
    m_groupVavleBtnAdd[4] = ui->addBtn_5;
    for(int i = 0;i < MAX_GROUP_IDENTIFY;i++){
        m_pGroupTitleLbl[i]->setObjectName("grouptitlelbl");
        m_groupVavleLabel[i]->setObjectName("groupValuelbl");
        m_groupVavleLabel[i]->setEnabled(false);
        m_groupVavleBtnDel[i]->setWhatsThis(QString::number(i));
        m_groupVavleBtnAdd[i]->setWhatsThis(QString::number(i));
        connect(m_groupVavleBtnDel[i],SIGNAL(clicked(bool)),this,SLOT(groupDelBtnClicked(bool)));
        connect(m_groupVavleBtnAdd[i],SIGNAL(clicked(bool)),this,SLOT(groupAddBtnClicked(bool)));
    }
    m_machineTypeStrList<<"ZL_LS"<<"ZL_LD"<<"ZL_LDS";//机型显示列表
    vavleList<<"4"<<"8"<<"24"<<"48"<<"60"<<"64"<<"68"<<"80"<<"96"<<"128"<<"240"<<"256";

    QStringList verChuteList,ldChutelist;//料槽数量，立式和履带
    verChuteList<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"10"<<"11"<<"12";
    ldChutelist<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"10"<<"11"<<"12";

    QStringList valveRatioList;
    valveRatioList<<"1:1"<<"1:2";
    ui->comboBoxVavleRatio->addItems(valveRatioList);

    crossEjectorList<<"0"<<"8"<<"16"<<"32";
    ui->comboBoxCrossEjector->addItems(crossEjectorList);
    QStringList controlBoardList;
    controlBoardList<<"0"<<"1"<<"2";
    ui->comboBoxControlBoard->addItems(controlBoardList);
    m_pGroupsetBtnMapper = new QSignalMapper(this);

    for(int i = 0;i < MAX_VIEW ;i++){
        m_viewCheckBox[i]->setWhatsThis(QString::number(i));
        connect(m_viewCheckBox[i],SIGNAL(clicked(bool)),this,SLOT(viewCheckboxClicked(bool)));

        m_viewXsysCheckBox[i]->setWhatsThis(QString::number(i));
        connect(m_viewXsysCheckBox[i],SIGNAL(clicked(bool)),this,SLOT(viewXsysCheckboxClicked(bool)));

        m_pGroupsetBtnMapper->setMapping(m_viewGroupset[i],i);
        connect(m_viewGroupset[i],SIGNAL(clicked()),m_pGroupsetBtnMapper,SLOT(map()));
        m_viewGroupset[i]->setText(g_myLan().times);

        m_pViewUintComboBox[i]->addItems(verChuteList);
        m_pViewUintComboBox[i]->setVisible(true);
        m_pViewValveComboBox[i]->addItems(vavleList);
    }
    connect(m_pGroupsetBtnMapper,SIGNAL(mapped(int)),this,SLOT( GroupSetBtnClicked(int) ) ) ;

    for(int i = 0;i < 2 ;i++){
        m_pVavleCountComboBox[i]->addItems(ldChutelist);
        m_pVavleCountComboBox[i]->setVisible(true);
    }

    m_machineTypeChuteMap.clear();
    m_machineTypeChuteMap.insert("ZL_LS",verChuteList);
    m_machineTypeChuteMap.insert("ZL_LD",ldChutelist);
    m_machineTypeChuteMap.insert("ZL_LDS",ldChutelist);

    m_machineTypeMap.clear();
    m_machineTypeMap.insert("ZL_LS",MACHINE_VERTICAL_ZL_CV);
    m_machineTypeMap.insert("ZL_LD",MACHINE_LD_ZL_GP);
    m_machineTypeMap.insert("ZL_LDS",MACHINE_LD2_ZL_GP);
    ui->segmentCheckBox->hide();

    m_groupShow = false;

    shottimer = new QTimer;
    connect(shottimer,SIGNAL(timeout()),this, SLOT(getscreenshot()));


}

MachineSetWidget::~MachineSetWidget()
{
    delete ui;
}

void MachineSetWidget::showPage(bool bshow)
{
    if(bshow){
        memcpy(&m_temCnf,&struCnfg,sizeof(m_temCnf));
        updateLayer();
        ui->comboBoxMachineType->clear();
        ui->comboBoxMachineType->addItems(m_machineTypeStrList);
        updateMachineSetting();
        ui->comboBoxVavle->clear();
        ui->comboBoxVavleRatio->setCurrentIndex(m_temCnf.nValveRatio);
        ui->comboBoxControlBoard->setCurrentIndex(m_temCnf.nCtrlBoardTotal);
        ui->comboBoxCrossEjector->setCurrentIndex(crossEjectorList.indexOf(QString::number(m_temCnf.nCorssEjector)));
        ui->comboBoxVavle->addItems(vavleList);
        QString vavlestr = QString::number(m_temCnf.struLayerInfo[struGsh.nLayer].nEjectorsPerChute);
        int index = vavleList.indexOf(vavlestr);
        ui->comboBoxVavle->setCurrentIndex(index);
        m_chuteNum = m_temCnf.nChuteTotal;
        m_vavleNum = m_temCnf.struLayerInfo[struGsh.nLayer].nEjectorsPerChute;
        m_machineAttr = struCnfg.nMachineAttr;
        updateSpliceState();
        updateVavleStep();
        updateAll();
        initVavleGroupNum();
        updateViewCheckboxAndViewBtnState();
        updateViewXsysCheckboxAndViewBtnState();
        ui->groupBox->setVisible(false);
        if(struCnfg.nMachineType == MACHINE_LD2_ZL_GP)
        {
            ui->upLayerComboBox->show();
            ui->labelLayerUp->show();
            ui->groupBox_4->show();
        }
        else
        {
            ui->upLayerComboBox->hide();
            ui->labelLayerUp->hide();
            ui->groupBox_4->hide();
        }

        if(struGsh.bScreenShotStatus == 1)
        {
            shottimer->start(1000);
        }
    }else{

        if(struGsh.bScreenShotStatus == 1)
            return;

//        if(memcmp(&m_temCnf,&struCnfg,sizeof(m_temCnf)) != 0)
//        {
//            myMessageBox msgBox(MSG_QUES, g_myLan().ids_toSaveParameters);
//            int ret = msgBox.exec();
//            if (ret == QDialog::Accepted)
//            {
//                if(setVavleGroup())
//                {
//                    memcpy(&struCnfg,&m_temCnf,sizeof(m_temCnf));

//                    myFlow.saveGlobal();
//                    myFlow.sendAll();
//                }else{
//                    myMessageBox msgBox(MSG_INFO, g_myLan().msg_times_set_err);
//                    msgBox.exec();
//                }
//            }
//        }
    }
}

void MachineSetWidget::retranslateUiWidget()
{
    ui->mainGroup->setTitle("");
    ui->groupBox_2->setTitle(g_myLan().product);
    ui->groupBox_5->setTitle(g_myLan().camera_set+"/"+g_myLan().ejector_num);
    ui->groupBox_6->setTitle(g_myLan().ids_set+g_myLan().ids_valveBoard);
    ui->label->setText(g_myLan().ids_machineModel);
    ui->label_5->setText(g_myLan().ids_channel);
    ui->label_8->setText(g_myLan().ejector_num);
    ui->groupBox->setTitle(g_myLan().times);

    ui->label_2->setText(g_myLan().ids_one);
    ui->label_3->setText(g_myLan().ids_two);
    ui->label_6->setText(g_myLan().ids_three);
    ui->label_7->setText(g_myLan().ids_four);
    ui->label_4->setText(g_myLan().ids_five);
    ui->segmentCheckBox->setText(g_myLan().splice);
    ui->applyBtn->setText(g_myLan().ids_apply);
    ui->front->setText(g_myLan().ids_fm);
    ui->frontAuxiliary->setText(g_myLan().ids_fs);
    ui->rear->setText(g_myLan().ids_bm);
    ui->rearAuxiliary->setText(g_myLan().ids_bs);
    ui->front_3->setText(g_myLan().ids_fm);
    ui->frontAuxiliary_3->setText(g_myLan().ids_fs);
    ui->rear_3->setText(g_myLan().ids_bm);
    ui->rearAuxiliary_3->setText(g_myLan().ids_bs);
    ui->label_15->setText("X"+g_myLan().ejector_num);
    ui->InputSetBtn->setText(g_myLan().ids_import);
    ui->OutputSetBtn->setText(g_myLan().ids_export);

    if(struCnfg.nMachineType == MACHINE_LD2_ZL_GP)
    {
        ui->labelLayerUp->setText(g_myLan().upper_layer+g_myLan().main_vavle);
        ui->labelLayerLow->setText(g_myLan().lower_layer+g_myLan().main_vavle);
        ui->groupBox_3->setTitle(g_myLan().lower_layer);
    }
    else
    {
        ui->labelLayerLow->setText(g_myLan().main_vavle);
        ui->groupBox_3->setTitle("");
    }
    ui->groupBox_4->setTitle(g_myLan().upper_layer);
    ui->groupBox_6->setTitle(g_myLan().ids_valveBoard);
    ui->label_19->setText(g_myLan().ids_controlBoard);
}

void MachineSetWidget::getscreenshot()
{
    shottimer->stop();
    {

        myFlow.sleep(1);
        myFlow.saveScreenshot();


        struGsh.bScreenShotStatus = 0;
    }
}

void MachineSetWidget::viewCheckboxClicked(bool checked)
{
    QCheckBox* btn = (QCheckBox*)sender();
    int index = btn->whatsThis().toInt();
    int curLayer = struGsh.nLayer;
    m_temCnf.struLayerInfo[curLayer].nViewIsUsed[index] = (int)checked;
    //    for(int i = 0;i < MAX_VIEW;i++){
    //        if(m_viewCheckBox[i]->isChecked()){
    //            m_temCnf.struLayerInfo[curLayer].nViewChainTotal++;
    //        }
    //    }
}

void MachineSetWidget::viewXsysCheckboxClicked(bool checked)
{
    QCheckBox* btn = (QCheckBox*)sender();
    int index = btn->whatsThis().toInt();

    m_temCnf.nXsystemBoard[index] = (int)checked;
}

void MachineSetWidget::groupDelBtnClicked(bool checked)
{
    QPushButton *btn = (QPushButton*)sender();
    int index = btn->whatsThis().toInt();
    if(m_groupVavleNum[index] >= m_vavleStep ){
        if(index == 0 && m_groupVavleNum[index] > m_vavleStep){
            m_groupVavleNum[index] -= m_vavleStep;
            m_groupVavleNum[index+1] += m_vavleStep;
        }
        if(index > 0){
            if(index < 4){
                if(m_groupVavleNum[index+1] > 0){
                    if(m_groupVavleNum[index] > m_vavleStep){
                        m_groupVavleNum[index] -= m_vavleStep;
                        m_groupVavleNum[index-1] += m_vavleStep;
                    }
                }else{
                    m_groupVavleNum[index] -= m_vavleStep;
                    m_groupVavleNum[index-1] += m_vavleStep;
                }
            }else{
                m_groupVavleNum[index] -= m_vavleStep;
                m_groupVavleNum[index-1] += m_vavleStep;
            }
        }
    }
    setVavleGroup();
    updateGroup();
}

void MachineSetWidget::groupAddBtnClicked(bool checked)
{
    QPushButton *btn = (QPushButton*)sender();
    int index = btn->whatsThis().toInt();
    bool bAdd = false;
    if(m_groupVavleNum[index] < m_chuteNum*m_vavleNum){
        if(index == 0 && m_groupVavleNum[1] >= m_vavleStep){
            if(m_groupVavleNum[2] == 0){
                m_groupVavleNum[index] += m_vavleStep;
                m_groupVavleNum[index+1] -= m_vavleStep;
            }else{
                if(m_groupVavleNum[1] > m_vavleStep){
                    m_groupVavleNum[index] += m_vavleStep;
                    m_groupVavleNum[index+1] -= m_vavleStep;
                }
            }
        }
        if(index > 0 && m_groupVavleNum[index-1] >= 2*m_vavleStep){
            m_groupVavleNum[index] += m_vavleStep;
            m_groupVavleNum[index-1] -= m_vavleStep;
        }
    }
    setVavleGroup();
    updateGroup();
}


void MachineSetWidget::updateViewCheckboxAndViewBtnState()
{
    for(int i = 0;i < MAX_VIEW;i++){
        m_viewCheckBox[i]->setChecked((bool)m_temCnf.struLayerInfo[struGsh.nLayer].nViewIsUsed[i]);
    }
}

void MachineSetWidget::updateViewXsysCheckboxAndViewBtnState()
{
    for(int i = 0;i < MAX_VIEW;i++){
        m_viewXsysCheckBox[i]->setChecked((bool)m_temCnf.nXsystemBoard[i]);
    }
}

void MachineSetWidget::updateMachineSetting()
{
    ui->comboBoxMachineType->setCurrentIndex(m_machineTypeStrList.indexOf(m_machineTypeMap.key(m_temCnf.nMachineType)));
    ui->comboBoxMachineChildType->clear();
    QStringList childTypeList = m_machineTypeChuteMap.value(m_machineTypeMap.key(m_temCnf.nMachineType));
    ui->comboBoxMachineChildType->addItems(childTypeList);

    ui->comboBoxMachineChildType->setCurrentIndex(m_temCnf.nChuteTotal-1);
}

void MachineSetWidget::updateLayer()
{
    //    if(m_pCurLayerBtn[0]){
    //        for(int i = 0;i < 2;i++){
    //            m_pCurLayerBtn[i]->setChecked(i == struGsh.nLayer);
    //            if(m_temCnf.nLayerTotal >= 2){
    //                m_pCurLayerBtn[i]->show();
    //            }else{
    //                m_pCurLayerBtn[i]->hide();
    //            }
    //        }
    //        m_pCurLayerBtn[0]->setText(g_myLan().upper_layer);
    //        m_pCurLayerBtn[1]->setText(g_myLan().lower_layer);
    //        m_pCurLayerBtn[0]->setObjectName("mainLayerBtn");
    //        m_pCurLayerBtn[1]->setObjectName("viceLayerBtn");
    //    }
}

void MachineSetWidget::initVavleGroupNum()
{
    memset(m_groupVavleNum,0,sizeof(m_groupVavleNum));
    for(int i = 0;i < m_temCnf.struLayerInfo[struGsh.nLayer].stuVavleInfo[0].nTickGroupTotal;i++)
        m_groupVavleNum[i] = myFlow.getTickGroupVavleNum(0,0,i);
    updateGroup();
}

void MachineSetWidget::updateGroup()
{
    for(int i = 0;i < MAX_GROUP_IDENTIFY;i++)
    {
        m_groupVavleLabel[i]->setText(QString::number(m_groupVavleNum[i]));
    }
}

bool MachineSetWidget::setVavleGroup()
{
    QSettings setting(CFG_MachineSet,QSettings::IniFormat);
    setting.beginGroup("groupset");
    for(int i = 0; i < MAX_VIEW; i++){

        if(!m_temCnf.struLayerInfo[0].nViewIsUsed[i])
            continue;
        QString list ;//= setting.value("view%1groupnum","1").toInt();
        int grouptotal  = setting.value(QString("view%1groupnum").arg(i),"1").toInt();
        m_temCnf.struLayerInfo[0].stuViewInfo[i].nIdentifyGroupTotal = grouptotal;
        for(int j = 0; j < grouptotal; j++)
        {
            list += setting.value(QString("view%1group%2").arg(i).arg(j), QString("%1-%2").arg(1).arg(m_temCnf.struLayerInfo[0].nViewUnitTotal[i])).toString();
            list += "/";
        }
        qDebug()<<"list"<<list<<"grouptotal"<<grouptotal;
        int start[MAX_GROUP_IDENTIFY],end[MAX_GROUP_IDENTIFY];
        memset(start,0,sizeof(start));
        memset(end,0,sizeof(end));
        for(int groupIdx = 0; groupIdx < MAX_GROUP_IDENTIFY; groupIdx++)
        {
            for(int unit = 0; unit < MAX_VIEW_UNIT; unit++)
            {
                m_temCnf.struLayerInfo[0].stuViewInfo[i].stuIdenGupInfo[groupIdx].nUnitGupSplice[unit] = 0;
            }
        }

        for(int groupIdx = 0; groupIdx < m_temCnf.struLayerInfo[0].stuViewInfo[i].nIdentifyGroupTotal;groupIdx++)
        {

            QString tmp = list.split("/").at(groupIdx);
            QStringList list2 = tmp.split("-");
            if(list2.length() < 2){
                qDebug()<<"set err!";
                continue;
            }

            start[groupIdx] = list2.at(0).toInt()-1;
            end[groupIdx] = list2.at(1).toInt()-1;
            m_temCnf.struLayerInfo[0].nViewUnitTotal[i]  = list2.at(1).toInt();
            m_temCnf.struLayerInfo[0].stuViewInfo[i].stuIdenGupInfo[groupIdx].nGroupUnitTotal = end[groupIdx] - start[groupIdx] + 1;
            for(int k = 0; k < m_temCnf.struLayerInfo[0].stuViewInfo[i].stuIdenGupInfo[groupIdx].nGroupUnitTotal;k++ )
            {
                m_temCnf.struLayerInfo[0].stuViewInfo[i].stuIdenGupInfo[groupIdx].nUnitGupId[k] = start[groupIdx]+k;
                qDebug()<<"view"<<i<<"group"<<groupIdx<<"id"<<m_temCnf.struLayerInfo[0].stuViewInfo[i].stuIdenGupInfo[groupIdx].nUnitGupId[k];
            }

            if(groupIdx){
                m_temCnf.struLayerInfo[0].nSplice = 1;
                m_temCnf.struLayerInfo[0].nSpliceType = 2;
                for(int mis = start[groupIdx];mis <= end[groupIdx-1];mis++ ){
                    m_temCnf.struLayerInfo[0].stuViewInfo[i].stuIdenGupInfo[groupIdx-1].nUnitGupSplice[end[groupIdx-1] - start[groupIdx-1] - mis + start[groupIdx]]=1;
                    m_temCnf.struLayerInfo[0].stuViewInfo[i].stuIdenGupInfo[groupIdx].nUnitGupSplice[mis-start[groupIdx]]=2;
                }
            }
        }
        //剔除链其实不需要分割分次
        if(i == 0 )
        {
            int curValue = i/4;
            //剔除
            m_temCnf.struLayerInfo[0].stuVavleInfo[curValue].nTickGroupTotal = grouptotal;;
            memset(start,0,sizeof(start));
            memset(end,0,sizeof(end));
            for(int groupIdx = 0; groupIdx < m_temCnf.struLayerInfo[0].stuVavleInfo[curValue].nTickGroupTotal;groupIdx++)
            {
                memset(m_temCnf.struLayerInfo[0].stuVavleInfo[curValue].stuTickGupInfo[groupIdx].nEjBoardGupSplice, 0 ,sizeof(MAX_VAVLE_BOARD));
                if(groupIdx == list.size()) break;
                QStringList list2 = list.split("/").at(groupIdx).split("-");
                if(list2.length() < 2){
                    qDebug()<<"set err!";
                    continue;
                }
                start[groupIdx] = list2.at(0).toInt()-1;
                end[groupIdx] = list2.at(1).toInt()-1;
                m_temCnf.struLayerInfo[0].stuVavleInfo[curValue].stuTickGupInfo[groupIdx].nEjBoardGupTotal = end[groupIdx] - start[groupIdx] + 1;

                for(int k = 0; k < m_temCnf.struLayerInfo[0].stuVavleInfo[curValue].stuTickGupInfo[groupIdx].nEjBoardGupTotal;k++ ){
                    m_temCnf.struLayerInfo[0].stuVavleInfo[curValue].stuTickGupInfo[groupIdx].nEjBoardGupId[k] = start[groupIdx] +k;
                    qDebug()<<"value"<<curValue<<"group"<<groupIdx<<"id"<<m_temCnf.struLayerInfo[0].stuVavleInfo[curValue].stuTickGupInfo[groupIdx].nEjBoardGupId[k];
                }
                if(groupIdx){
                    m_temCnf.struLayerInfo[0].nSpliceType = 2;
                    for(int mis = start[groupIdx];mis <= end[groupIdx-1];mis++ ){
                        m_temCnf.struLayerInfo[0].stuVavleInfo[curValue].stuTickGupInfo[groupIdx-1].nEjBoardGupSplice[end[groupIdx-1] - start[groupIdx-1] - mis + start[groupIdx]]=1;
                        m_temCnf.struLayerInfo[0].stuVavleInfo[curValue].stuTickGupInfo[groupIdx].nEjBoardGupSplice[mis-start[groupIdx]]=2;
                    }

                }

            }
        }

    }

    for(int i = 0; i < MAX_VIEW; i++){
        if(!m_temCnf.struLayerInfo[0].nViewIsUsed[i])
            continue;
        for(int k = 0; k <m_temCnf.struLayerInfo[0].stuViewInfo[i].nIdentifyGroupTotal; k++)
        {
            for(int t = 0; t < m_temCnf.struLayerInfo[0].stuViewInfo[i].stuIdenGupInfo[k].nGroupUnitTotal; t++) // 认为视的次数和吹的序号一致
            {
                qDebug()<<"view"<<i<<"group"<<k<<"unit"<<t<<m_temCnf.struLayerInfo[0].stuViewInfo[i].stuIdenGupInfo[k].nUnitGupId[t]
                          <<"splice"<<m_temCnf.struLayerInfo[0].stuViewInfo[i].stuIdenGupInfo[k].nUnitGupSplice[t];
                if(m_temCnf.struLayerInfo[0].stuViewInfo[i].stuIdenGupInfo[k].nUnitGupSplice[t])
                    m_temCnf.struLayerInfo[0].stuViewInfo[i].bViewSplice[m_temCnf.struLayerInfo[0].stuViewInfo[i].stuIdenGupInfo[k].nUnitGupId[t]] = 1;
                else
                    m_temCnf.struLayerInfo[0].stuViewInfo[i].bViewSplice[m_temCnf.struLayerInfo[0].stuViewInfo[i].stuIdenGupInfo[k].nUnitGupId[t]] = 0;
            }
        }
    }

    for(int i = 0; i < MAX_VIEW; i++){
        if(!m_temCnf.struLayerInfo[0].nViewIsUsed[i])
            continue;
        for(int k = 0; k <m_temCnf.struLayerInfo[0].nViewUnitTotal[i]; k++)
        {
            qDebug()<<"unit"<<k<<m_temCnf.struLayerInfo[0].stuViewInfo[i].bViewSplice[k];
        }
    }

setting.endGroup();
    return true;
}
//{
//    //只支持 前三个分组有分割
//    if(m_groupVavleNum[0]%m_vavleNum && m_groupVavleNum[1]%m_vavleNum == 0)
//        return false;
//    if(m_groupVavleNum[0]%m_vavleNum == 0 && m_groupVavleNum[1]%m_vavleNum && m_groupVavleNum[2]%m_vavleNum == 0)
//        return false;
//    if(m_groupVavleNum[0]%m_vavleNum == 0 && m_groupVavleNum[1]%m_vavleNum == 0\
//            && m_groupVavleNum[2]%m_vavleNum && m_groupVavleNum[3]%m_vavleNum == 0)
//        return false;
//    if(m_groupVavleNum[0]%m_vavleNum && m_groupVavleNum[2]% m_vavleNum) //只支持一个通道分割
//        return false;
//    if(m_groupVavleNum[3]%m_vavleNum || m_groupVavleNum[4]%m_vavleNum)
//        return false;
//    int groupNum = 0;
//    for(int i =0;i < MAX_GROUP_IDENTIFY;i++){
//        if(m_groupVavleNum[i] > 0){
//            groupNum++;
//        }
//    }

//    for(int i = 0;i < m_temCnf.struLayerInfo[struGsh.nLayer].nViewChainTotal;i++){//相机板分组信息处理
//        if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[i]) continue;
//        m_temCnf.struLayerInfo[struGsh.nLayer].stuViewInfo[i].nIdentifyGroupTotal = groupNum;
//        bool bsplice = false;
//        bool bgroup2splice = false;
//        int unitIndex = 0;//阀板索引或相机板索引7
//        int vavleCountPerGroup;//每组所占有的相机板数量
//        for(int groupIdx = 0; groupIdx < groupNum;groupIdx++){
//            memset(m_temCnf.struLayerInfo[struGsh.nLayer].stuViewInfo[i].stuIdenGupInfo[groupIdx].nUnitGupId,0,MAX_VIEW_UNIT*sizeof(int));
//            vavleCountPerGroup = m_groupVavleNum[groupIdx]/m_vavleNum;
//            if(groupIdx == 0){
//                if(m_groupVavleNum[groupIdx]%m_vavleNum != 0)//分组一有某一料槽分割
//                {
//                    vavleCountPerGroup+=1;
//                    bsplice = true;
//                }
//            }else if(groupIdx == 1){
//                if(bsplice){ //要保证后面的分组不分割，分组一分割则分组二也必须分割
//                    vavleCountPerGroup+=1;
//                    unitIndex -= 1;
//                }else{
//                    if(m_groupVavleNum[groupIdx]%m_vavleNum != 0)//分组二有某一料槽分割
//                    {
//                        vavleCountPerGroup+=1;
//                        bgroup2splice = true;
//                    }
//                }
//            }else if(groupIdx == 2){
//                if(bgroup2splice){
//                    vavleCountPerGroup+=1;
//                    unitIndex -= 1;
//                }
//            }
//            if(2 == i || 3 == i|| 6 == i || 7 == i){
//                if (struCnfg.bCrossMode2)
//                {
//                    vavleCountPerGroup += 1;
//                }
//                else if (struCnfg.bCrossMode1 )
//                {
//                    vavleCountPerGroup -= 1;
//                }
//            }
//            m_temCnf.struLayerInfo[struGsh.nLayer].stuViewInfo[i].stuIdenGupInfo[groupIdx].nGroupUnitTotal = vavleCountPerGroup;
//            for(int k = 0;k < vavleCountPerGroup;k++){
//                if(i < 4)
//                    m_temCnf.struLayerInfo[struGsh.nLayer].stuViewInfo[i].stuIdenGupInfo[groupIdx].nUnitGupId[k]=k+unitIndex;
//                else
//                    m_temCnf.struLayerInfo[struGsh.nLayer].stuViewInfo[i].stuIdenGupInfo[groupIdx].nUnitGupId[k]=m_temCnf.nChuteTotal-unitIndex-vavleCountPerGroup+k;
//                if(groupIdx == 0){
//                    if(i < 4){
//                        if(k == vavleCountPerGroup-1 && bsplice)//分组一单独处理分割 分组一的最后一个相机板单独处理
//                        {
//                            m_temCnf.struLayerInfo[struGsh.nLayer].stuViewInfo[i].stuIdenGupInfo[0].nUnitGupSplice[k]=1;
//                        }else{
//                            m_temCnf.struLayerInfo[struGsh.nLayer].stuViewInfo[i].stuIdenGupInfo[groupIdx].nUnitGupSplice[k]=0;
//                        }
//                    }
//                    else{
//                        if(k == 0 && bsplice){//分组一单独处理分割 分组一的第一个相机板单独处理
//                            m_temCnf.struLayerInfo[struGsh.nLayer].stuViewInfo[i].stuIdenGupInfo[0].nUnitGupSplice[k]=2;
//                        }else{
//                            m_temCnf.struLayerInfo[struGsh.nLayer].stuViewInfo[i].stuIdenGupInfo[groupIdx].nUnitGupSplice[k]=0;
//                        }
//                    }
//                }else if(groupIdx == 1){
//                    if(i < 4){
//                        if(k == 0 && bsplice){//分组二单独处理分割 分组二的第一个相机板单独处理
//                            m_temCnf.struLayerInfo[struGsh.nLayer].stuViewInfo[i].stuIdenGupInfo[1].nUnitGupSplice[k]=2;
//                        }else{
//                            if(k == vavleCountPerGroup-1 && bgroup2splice)//分组一单独处理分割 分组一的最后一个相机板单独处理
//                            {
//                                m_temCnf.struLayerInfo[struGsh.nLayer].stuViewInfo[i].stuIdenGupInfo[groupIdx].nUnitGupSplice[k]=1;
//                            }else{
//                                m_temCnf.struLayerInfo[struGsh.nLayer].stuViewInfo[i].stuIdenGupInfo[groupIdx].nUnitGupSplice[k]=0;
//                            }
//                        }
//                    }
//                    else{
//                        if(k == vavleCountPerGroup-1 && bsplice){//分组二单独处理分割 分组二的最后一个相机板单独处理
//                            m_temCnf.struLayerInfo[struGsh.nLayer].stuViewInfo[i].stuIdenGupInfo[1].nUnitGupSplice[k]=1;
//                        }else{
//                            if(k == 0 && bgroup2splice){//分组一单独处理分割 分组一的第一个相机板单独处理
//                                m_temCnf.struLayerInfo[struGsh.nLayer].stuViewInfo[i].stuIdenGupInfo[groupIdx].nUnitGupSplice[k]=2;
//                            }else{
//                                m_temCnf.struLayerInfo[struGsh.nLayer].stuViewInfo[i].stuIdenGupInfo[groupIdx].nUnitGupSplice[k]=0;
//                            }
//                        }
//                    }
//                }else if(groupIdx == 2){
//                    if(i < 4){
//                        if(k == 0 && bgroup2splice){//分组三单独处理分割 分组三的第一个相机板单独处理
//                            m_temCnf.struLayerInfo[struGsh.nLayer].stuViewInfo[i].stuIdenGupInfo[groupIdx].nUnitGupSplice[k]=2;
//                        }else{
//                            m_temCnf.struLayerInfo[struGsh.nLayer].stuViewInfo[i].stuIdenGupInfo[groupIdx].nUnitGupSplice[k]=0;
//                        }
//                    }else{
//                        if(k == vavleCountPerGroup-1 && bgroup2splice){//分组三单独处理分割 分组三的第一个相机板单独处理
//                            m_temCnf.struLayerInfo[struGsh.nLayer].stuViewInfo[i].stuIdenGupInfo[groupIdx].nUnitGupSplice[k]=1;
//                        }else{
//                            m_temCnf.struLayerInfo[struGsh.nLayer].stuViewInfo[i].stuIdenGupInfo[groupIdx].nUnitGupSplice[k]=0;
//                        }
//                    }

//                }else{
//                    m_temCnf.struLayerInfo[struGsh.nLayer].stuViewInfo[i].stuIdenGupInfo[groupIdx].nUnitGupSplice[k]=0;
//                }
//            }
//            unitIndex+= vavleCountPerGroup;
//        }
//    }


//    for(int i = 0;i < m_temCnf.struLayerInfo[struGsh.nLayer].nVavleChainTotal;i++){//喷阀板分组信息处理
//        m_temCnf.struLayerInfo[struGsh.nLayer].stuVavleInfo[i].nTickGroupTotal = groupNum;
//        bool bsplice = false;
//        bool bgroup2splice = false;
//        int unitIndex = 0;//阀板索引或相机板索引
//        int vavleCountPerGroup;//每组所占有的喷阀板数量
//        for(int groupIdx = 0; groupIdx < groupNum;groupIdx++){
//            vavleCountPerGroup = m_groupVavleNum[groupIdx]/m_vavleNum;
//            if(groupIdx == 0){
//                if(m_groupVavleNum[groupIdx]%m_vavleNum != 0)//分组一有某一料槽分割
//                {
//                    updateSpliceType(m_groupVavleNum[groupIdx]);
//                    vavleCountPerGroup+=1;
////                    m_temCnf.struLayerInfo[struGsh.nLayer].nSpliceChuteAddr = vavleCountPerGroup;
//                    bsplice = true;
//                }
//            }else if(groupIdx == 1){
//                if(bsplice){ //要保证后面的分组不分割，分组一分割则分组二也必须分割
//                    vavleCountPerGroup+=1;
//                    unitIndex -= 1;
//                }else{
//                    if(m_groupVavleNum[groupIdx]%m_vavleNum != 0)//分组二有某一料槽分割
//                    {
//                        updateSpliceType(m_groupVavleNum[groupIdx]);
//                        vavleCountPerGroup+=1;
////                        m_temCnf.struLayerInfo[struGsh.nLayer].nSpliceChuteAddr = vavleCountPerGroup;
//                        bgroup2splice = true;
//                    }
//                }
//            }else if(groupIdx == 2){
//                if(bgroup2splice){
//                    vavleCountPerGroup+=1;
//                    unitIndex -= 1;
//                }
//            }
//            m_temCnf.struLayerInfo[struGsh.nLayer].stuVavleInfo[i].stuTickGupInfo[groupIdx].nEjBoardGupTotal = vavleCountPerGroup;
//            m_temCnf.struLayerInfo[struGsh.nLayer].stuVavleInfo[i].stuTickGupInfo[groupIdx].nUnitGupTotal = vavleCountPerGroup;
//            for(int k = 0;k < vavleCountPerGroup;k++){
//                m_temCnf.struLayerInfo[struGsh.nLayer].stuVavleInfo[i].stuTickGupInfo[groupIdx].nUnitGupId[k]=k+unitIndex;
//                m_temCnf.struLayerInfo[struGsh.nLayer].stuVavleInfo[i].stuTickGupInfo[groupIdx].nEjBoardGupId[k]=k+unitIndex;
//                if(groupIdx == 0){
//                    if(k == vavleCountPerGroup-1 && bsplice)//分组一单独处理分割 分组一的最后一个相机板单独处理
//                    {
//                        m_temCnf.struLayerInfo[struGsh.nLayer].stuVavleInfo[i].stuTickGupInfo[0].nEjBoardGupSplice[k]=1;
//                    }else{
//                        m_temCnf.struLayerInfo[struGsh.nLayer].stuVavleInfo[i].stuTickGupInfo[groupIdx].nEjBoardGupSplice[k]=0;
//                    }
//                }else if(groupIdx == 1){
//                    if(k == 0 && bsplice){//分组二单独处理分割 分组二的第一个相机板单独处理
//                        m_temCnf.struLayerInfo[struGsh.nLayer].stuVavleInfo[i].stuTickGupInfo[1].nEjBoardGupSplice[k]=2;
//                    }else{
//                        if(k == vavleCountPerGroup-1 && bgroup2splice)//分组一单独处理分割 分组一的最后一个相机板单独处理
//                        {
//                            m_temCnf.struLayerInfo[struGsh.nLayer].stuVavleInfo[i].stuTickGupInfo[groupIdx].nEjBoardGupSplice[k]=1;
//                        }else{
//                            m_temCnf.struLayerInfo[struGsh.nLayer].stuVavleInfo[i].stuTickGupInfo[groupIdx].nEjBoardGupSplice[k]=0;
//                        }
//                    }
//                }else if(groupIdx == 2){
//                    if(k == 0 && bgroup2splice){//分组三单独处理分割 分组二的第一个相机板单独处理
//                        m_temCnf.struLayerInfo[struGsh.nLayer].stuVavleInfo[i].stuTickGupInfo[2].nEjBoardGupSplice[k]=2;
//                    }else{
//                        m_temCnf.struLayerInfo[struGsh.nLayer].stuVavleInfo[i].stuTickGupInfo[groupIdx].nEjBoardGupSplice[k]=0;
//                    }
//                }else{
//                    m_temCnf.struLayerInfo[struGsh.nLayer].stuVavleInfo[i].stuTickGupInfo[groupIdx].nEjBoardGupSplice[k]=0;
//                }
//            }
//            unitIndex+= vavleCountPerGroup;
//        }
//    }
//    return true;
//}

void MachineSetWidget::updateSpliceState()
{
    if(m_temCnf.struLayerInfo[struGsh.nLayer].nSplice == 1){
        ui->segmentCheckBox->setChecked(true);
    }else{
        ui->segmentCheckBox->setChecked(false);
    }
}

void MachineSetWidget::updateVavleStep()
{
    if(m_temCnf.struLayerInfo[struGsh.nLayer].nSplice){
        m_vavleStep = m_vavleNum/4;
    }else{
        m_vavleStep = m_vavleNum;
    }
}

void MachineSetWidget::updateSpliceType(int valveNum)
{
    int tempNum = valveNum % m_vavleNum;

    m_temCnf.struLayerInfo[struGsh.nLayer].nSpliceType = tempNum /m_vavleStep;
}

void MachineSetWidget::setChuteNumInfo()
{
    m_temCnf.nChuteTotal = m_chuteNum;
    m_temCnf.nFeedMainTotal = m_chuteNum;

    /* 更新相机板通道信息 */
    for(int j = 0; j < m_temCnf.struLayerInfo[struGsh.nLayer].nViewChainTotal; j++)
    {
        if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
        if(2 == j || 3 == j|| 6 == j || 7 == j){
            if (struCnfg.bCrossMode2)
            {
                m_temCnf.struLayerInfo[struGsh.nLayer].nViewUnitTotal[j] = m_chuteNum+1;
            }
            else if (struCnfg.bCrossMode1 )
            {
                m_temCnf.struLayerInfo[struGsh.nLayer].nViewUnitTotal[j] = m_chuteNum - 1;
            }
        }
        //        else
        //        {
        //            m_temCnf.struLayerInfo[struGsh.nLayer].nViewUnitTotal[j] = m_chuteNum;
        //        }
    }

    /* 更新喷阀板通道信息 */
    //    m_temCnf.struLayerInfo[struGsh.nLayer].nVavleBoardType[0]     = BOARD_VAVLE_LAYER_1;

    //    for(int j = 0; j < m_temCnf.struLayerInfo[struGsh.nLayer].nVavleChainTotal; j++)
    //    {
    //        m_temCnf.struLayerInfo[struGsh.nLayer].nVavleBoardTotal[j] = m_chuteNum;
    //    }
    //    int maxView0 = m_temCnf.struLayerInfo[struGsh.nLayer].nViewUnitTotal[0];
    //    int maxView1 = m_temCnf.struLayerInfo[struGsh.nLayer].nViewUnitTotal[1];
    //    m_temCnf.struLayerInfo[struGsh.nLayer].nVavleBoardTotal[0] = maxView0 > maxView1?maxView0:maxView1;

    //    maxView0 = m_temCnf.struLayerInfo[struGsh.nLayer].nViewUnitTotal[2];
    //    maxView1 = m_temCnf.struLayerInfo[struGsh.nLayer].nViewUnitTotal[3];
    //    m_temCnf.struLayerInfo[struGsh.nLayer].nVavleBoardTotal[1] = maxView0 > maxView1?maxView0:maxView1;

}

void MachineSetWidget::updateAll()
{
    updateMachineSetting();

    m_chuteNum = m_temCnf.nChuteTotal;
    setChuteNumInfo();
    memset(m_groupVavleNum,0,sizeof(m_groupVavleNum));
    m_groupVavleNum[0] = m_chuteNum*m_vavleNum;
    updateGroup();
    updateViewCheckboxAndViewBtnState();
    updateViewXsysCheckboxAndViewBtnState();
    updateViewUintComboBox();
    updateViewValveComboBox();
    updateVavleCountComboBox();
}

void MachineSetWidget::updateViewUintComboBox()
{
    for(int i = 0;i < MAX_VIEW;i++)
    {
        m_pViewUintComboBox[i]->setCurrentIndex(m_temCnf.struLayerInfo[struGsh.nLayer].nViewUnitTotal[i]-1);
    }
}
void MachineSetWidget::updateViewValveComboBox()
{
    for(int i = 0;i < MAX_VIEW;i++)
    {
        int index = vavleList.indexOf(QString::number(m_temCnf.struLayerInfo[struGsh.nLayer].stuViewInfo[i].nEjectorsPerCamera));
        m_pViewValveComboBox[i]->setCurrentIndex(index);
    }
}

void MachineSetWidget::updateVavleCountComboBox()
{
    int val= 0;
    for(int i = 0;i < 2;i++)
    {
        val = m_temCnf.struLayerInfo[struGsh.nLayer].nVavleBoardTotal[i]/(m_temCnf.nValveRatio+1)-1;
        if(val < 0) val = 0;
        m_pVavleCountComboBox[i]->setCurrentIndex(val);
    }
}

#if 0
void MachineSetWidget::on_comboBoxMachineType_activated(const QString &arg1)
{
    m_temCnf.nLayerTotal = 1;
    m_temCnf.nChuteTotal = 1;
    struGsh.nLayer = 0;
    m_temCnf.nCtrlBoardTotal = 1;
    m_temCnf.nMachineType = m_machineTypeMap[arg1];
    if(m_temCnf.nMachineType == MACHINE_LD2_ZL_GP)
    {
        m_temCnf.struLayerInfo[ struGsh.nLayer].nViewBoardType[0]     = BOARD_CAMERA_1_FRONT_MAIN;
        m_temCnf.struLayerInfo[ struGsh.nLayer].nViewBoardType[1]     = BOARD_CAMERA_1_REAR_MAIN;
        m_temCnf.struLayerInfo[ struGsh.nLayer].nViewBoardType[2]     = BOARD_CAMERA_2_FRONT_MAIN;
        m_temCnf.struLayerInfo[ struGsh.nLayer].nViewBoardType[3]     = BOARD_CAMERA_2_REAR_MAIN;
    }
    else
    {
        m_temCnf.struLayerInfo[ struGsh.nLayer].nViewBoardType[0]     = BOARD_CAMERA_1_FRONT_MAIN;
        m_temCnf.struLayerInfo[ struGsh.nLayer].nViewBoardType[1]     = BOARD_CAMERA_1_REAR_MAIN;
        m_temCnf.struLayerInfo[ struGsh.nLayer].nViewBoardType[2]     = BOARD_CAMERA_1_FRONT_VICE;
        m_temCnf.struLayerInfo[ struGsh.nLayer].nViewBoardType[3]     = BOARD_CAMERA_1_REAR_VICE;
    }
    updateLayer();
    updateAll();
}
#endif

void MachineSetWidget::on_comboBoxMachineType_activated(const QString &arg1)
{
    m_temCnf.nMachineType = m_machineTypeMap[arg1];
    if (m_temCnf.nMachineType == struCnfg.nMachineType) return;

    struCnfg.nMachineType = m_machineTypeMap[arg1];

    for(int i =0; i<MAX_VIEW;i++){
        struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[i] = 0;
    }

    if(struCnfg.nMachineType == MACHINE_LD2_ZL_GP)
    {
        myFlow.getBeltLd2GeneralLayerInfo();
        ui->upLayerComboBox->show();
        ui->labelLayerUp->show();
        ui->groupBox_4->show();
        ui->labelLayerUp->setText(g_myLan().upper_layer+g_myLan().main_vavle);
        ui->labelLayerLow->setText(g_myLan().lower_layer+g_myLan().main_vavle);
        ui->groupBox_3->setTitle(g_myLan().lower_layer);
    }
    else
    {
        myFlow.getVertRiceGeneralLayerInfo();
        ui->groupBox_4->hide();
        ui->upLayerComboBox->hide();
        ui->labelLayerUp->hide();
        ui->labelLayerLow->setText(g_myLan().main_vavle);
        ui->groupBox_3->setTitle("");
    }

    myFlow.getGlobalIpcConfig();

    on_segmentCheckBox_clicked(false);

    memcpy(&m_temCnf,&struCnfg,sizeof(m_temCnf));

    updateLayer();
    updateAll();
}

void MachineSetWidget::on_comboBoxMachineChildType_activated(int index)
{
    m_chuteNum = index+1;

    setChuteNumInfo();


    memset(m_groupVavleNum,0,sizeof(m_groupVavleNum));
    m_groupVavleNum[0] = m_chuteNum*m_vavleNum;
    updateGroup();
}

void MachineSetWidget::on_comboBoxVavle_activated(const QString &arg1)
{
    m_vavleNum = arg1.toInt();
    memset(m_groupVavleNum,0,sizeof(m_groupVavleNum));
    m_groupVavleNum[0] = m_chuteNum*m_vavleNum;
    updateGroup();
    m_temCnf.struLayerInfo[struGsh.nLayer].nEjectorsPerChute = m_vavleNum;
    updateVavleStep();
}

void MachineSetWidget::on_segmentCheckBox_clicked(bool checked)
{
    ui->segmentCheckBox->setChecked(checked);
    m_temCnf.struLayerInfo[struGsh.nLayer].nSplice = checked?1:0;

    if(m_temCnf.struLayerInfo[struGsh.nLayer].nSplice == 1)
    {
        m_temCnf.nMachineSpliceType = MACHINE_SPLICE_TYPE_BIG;
    }
    else
    {
        m_temCnf.nMachineSpliceType = MACHINE_SPLICE_TYPE_NULL;
    }

    updateVavleStep();
    memset(m_groupVavleNum,0,sizeof(m_groupVavleNum));
    m_groupVavleNum[0] = m_chuteNum*m_vavleNum;
    updateGroup();
}

void MachineSetWidget::on_applyBtn_clicked()
{
    int tmpChuteTotal = struCnfg.nChuteTotal;
    m_temCnf.struLayerInfo[struGsh.nLayer].nVavleBoardTotal[0] = (m_pVavleCountComboBox[0]->currentIndex()+1)*(m_temCnf.nValveRatio+1);
    m_temCnf.struLayerInfo[struGsh.nLayer].nVavleBoardTotal[1] = (m_pVavleCountComboBox[1]->currentIndex()+1)*(m_temCnf.nValveRatio+1);

    if(setVavleGroup())
    {
        logger()->info("apply new machine setting");
        memcpy(&struCnfg,&m_temCnf,sizeof(m_temCnf));

        /* 添加通道分割地址分配  */
        //if(struCnfg.struLayerInfo[struGsh.nLayer].nSplice == 1 )
        //{
        // if(struCnfg.nChuteTotal == 1)
        // {
        //   struCnfg.struLayerInfo[struGsh.nLayer].nSpliceChuteAddr = 1;
        // }
        // else
        //{
        //for(int i = 0; i < struCnfg.nLayerTotal; i++)
        //  {
        // for(int j = 0; j < struCnfg.struLayerInfo[i].nVavleChainTotal; j++)
        //  {
        // for(int m = 0; m < struCnfg.struLayerInfo[i].stuVavleInfo[j].nTickGroupTotal; m++)
        // {
        //      for(int k = 0; k < struCnfg.struLayerInfo[i].stuVavleInfo[j].stuTickGupInfo[m].nEjBoardGupTotal; k++)
        //      {
        //           if(struCnfg.struLayerInfo[i].stuVavleInfo[j].stuTickGupInfo[m].nEjBoardGupSplice[k] == 1)
        //          {
        //              struCnfg.struLayerInfo[i].nSpliceChuteAddr = struCnfg.struLayerInfo[i].stuVavleInfo[j].stuTickGupInfo[m].nEjBoardGupId[k]+1;
        //             break;
        //        }
        //     }
        //    }
        //    }
        //  }
        //  }
        //}
        //        if((tmpChuteTotal != struCnfg.nChuteTotal)||( m_machineAttr != struCnfg.nMachineAttr))
        //        {
        //            myFlow.getGlobalLampConfig();
        //        }

        //        if( m_temCnf.struLayerInfo[0].nViewIsUsed[2] ||m_temCnf.struLayerInfo[0].nViewIsUsed[3]
        //            ||m_temCnf.struLayerInfo[0].nViewIsUsed[6]||m_temCnf.struLayerInfo[0].nViewIsUsed[7]
        //            ||m_temCnf.struLayerInfo[0].nViewIsUsed[4]||m_temCnf.struLayerInfo[0].nViewIsUsed[5]) //22/6/27 带辅视时or双层机时，延迟时间默认不按次设置
        //        {
        //            struCnfg.nVavleTimeSetMode = 0;
        //            m_temCnf.nVavleTimeSetMode = 0;
        //        }
        //        else
        //        {
        //            struCnfg.nVavleTimeSetMode = 1;
        //            m_temCnf.nVavleTimeSetMode = 1;
        //        }
        if((struCnfg.struLayerInfo[struGsh.nLayer].stuVavleInfo[0].nTickGroupTotal == 1) ||(struCnfg.nChuteTotal == 1))
        {
            struCnfg.struLayerInfo[struGsh.nLayer].nOnlyDispFirst = 1;
            if(struCnfg.struLayerInfo[struGsh.nLayer].nSplice == 1)
            {
                struCnfp.nMatCopySetMode = MAT_COPY_SECOND_ALIKE_FIRST;
            }
            if(struCnfg.nChuteTotal == 1 || struCnfg.nChuteTotal == 2)    //1通道机器使用合板
            {
                struCnfp.struGroupCtrl[struGsh.ctrlboardIndex].nFeedSetType = 0;
            }
        }

        for(int i = 0; i < struCnfg.nLayerTotal; i++)
        {
            for(int j = 0; j < struCnfg.struLayerInfo[i].nVavleChainTotal; j++)
            {
                for(int k = 0; k < MAX_VAVLE_BOARD; k++)
                {
                    struCnfg.struLayerInfo[i].stuVavleInfo[j].nVavleBoardId[k] = k;
                }
            }
            if(CUSTOM != CUSTOM_YANSI)
            {
                for(int j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
                {
                    for(int k = 0; k < MAX_VIEW_UNIT; k++)
                    {
                      //  struCnfg.struLayerInfo[i].stuCameraInfo[j].nEjectEnd[k] = m_temCnf.struLayerInfo[i].nEjectorsPerChute;
                    }
                }
            }
        }
        for(int j = 0; j < struCnfg.struLayerInfo[struGsh.nLayer].nViewChainTotal; j++)
        {
            for(int k = 0; k < MAX_VIEW_UNIT; k++)
            {
                struCnfg.struLayerInfo[struGsh.nLayer].stuCameraInfo[j].nEjectEnd[k] = struCnfg.struLayerInfo[struGsh.nLayer].stuViewInfo[j].nEjectorsPerCamera;
            }
        }

        //应用时直接根据各视相机数默认分配IP,减少手动输入
        for(int i = 0; i < struCnfg.nLayerTotal; i++)
        {
            int id = 0;
            for(int j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
            {
                if(!struCnfg.struLayerInfo[struGsh.nLayer].nViewIsUsed[j]) continue;
                for(int k = 0; k < struCnfg.struLayerInfo[struGsh.nLayer].nViewUnitTotal[j]; k++)
                {
                    struCnfg.struIpcCameraParams[i][j][k].cameraId      = id++;
                    struCnfg.struIpcCameraParams[i][j][k].nIpAddr[0]    = 192;
                    struCnfg.struIpcCameraParams[i][j][k].nIpAddr[1]    = 168;
                    struCnfg.struIpcCameraParams[i][j][k].nIpAddr[2]    = 1;
                    struCnfg.struIpcCameraParams[i][j][k].nIpAddr[3]    = id;
                }
            }
        }

        myFlow.saveGlobal();
        memcpy(&m_temCnf,&struCnfg,sizeof(m_temCnf));
        myFlow.saveProfile();
        //        myFlow.initGsh();
        myFlow.sendAll();

        QFileInfo file("/sdcard/cnf/remotecache.txt");
        if (file.exists() && file.size() > 0) {
            g_Runtime().mySystem("mv /sdcard/cnf/remotecache.txt /sdcard/cnf/cache");
            g_Runtime().mySystem("sync");
        }

        returnBack();
    }else{
        myMessageBox msgBox(MSG_INFO, g_myLan().msg_times_set_err);
        msgBox.exec();
    }

    myFlow.getFpgaCameraArith(0,0,0);
}

void MachineSetWidget::curlayerBtnClickedSlots()
{
    //    for(int i=0; i<2; i++ )
    //    {
    //        if(sender() == m_pCurLayerBtn[i])
    //        {
    //            m_pCurLayerBtn[struGsh.nLayer]->setChecked(false);
    //            struGsh.nLayer = i;
    //            m_pCurLayerBtn[i]->setChecked(true);
    //            updateAll();
    //            break;
    //        }
    //    }
}

void MachineSetWidget::on_frontUintComboBox_activated(int index)
{
    m_temCnf.struLayerInfo[struGsh.nLayer].nViewUnitTotal[0] = index+1;


    QSettings setting(CFG_MachineSet,QSettings::IniFormat);
    setting.beginGroup("groupset");
    setting.setValue(QString("view%1groupnum").arg(0),1);
    setting.setValue(QString("view%1group%2").arg(0).arg(0), QString("%1-%2").arg(1).arg(m_temCnf.struLayerInfo[0].nViewUnitTotal[0]));
    setting.endGroup();
    setting.sync();
//    int grouptotal  = setting.value(QString("view%1groupnum").arg(i),"1").toInt();
//    m_temCnf.struLayerInfo[0].stuViewInfo[i].nIdentifyGroupTotal = grouptotal;
//    for(int j = 0; j < grouptotal; j++)
//    {
//        list += setting.value(QString("view%1group%2").arg(i).arg(j), QString("%1-%2").arg(1).arg(m_temCnf.struLayerInfo[0].nViewUnitTotal[i])).toString();



//    QString tmp = QString("1-%1").arg(QString::number(index+1));
//    QSettings setting(CFG_MachineSet,QSettings::IniFormat);
//    setting.beginGroup("groupset");
//    setting.setValue(QString("view%1group").arg(0),tmp);
//    setting.endGroup();
//    setting.sync();

    setChuteNumInfo();
}

void MachineSetWidget::on_frontAuxiliUintComboBox_activated(int index)
{
    m_temCnf.struLayerInfo[struGsh.nLayer].nViewUnitTotal[2] = index+1;

    QSettings setting(CFG_MachineSet,QSettings::IniFormat);
    setting.beginGroup("groupset");
    setting.setValue(QString("view%1groupnum").arg(2),1);
    setting.setValue(QString("view%1group%2").arg(2).arg(0), QString("%1-%2").arg(1).arg(m_temCnf.struLayerInfo[0].nViewUnitTotal[2]));
    setting.endGroup();
    setting.sync();

//    QString tmp = QString("1-%1").arg(QString::number(index+1));
//    QSettings setting(CFG_MachineSet,QSettings::IniFormat);
//    setting.beginGroup("groupset");
//    setting.setValue(QString("view%1group").arg(2),tmp);
//    setting.endGroup();
//    setting.sync();

    setChuteNumInfo();
}

void MachineSetWidget::on_rearUintComboBox_activated(int index)
{
    m_temCnf.struLayerInfo[struGsh.nLayer].nViewUnitTotal[1] = index+1;

    QSettings setting(CFG_MachineSet,QSettings::IniFormat);
    setting.beginGroup("groupset");
    setting.setValue(QString("view%1groupnum").arg(1),1);
    setting.setValue(QString("view%1group%2").arg(1).arg(0), QString("%1-%2").arg(1).arg(m_temCnf.struLayerInfo[0].nViewUnitTotal[1]));
    setting.endGroup();
    setting.sync();

//    QString tmp = QString("1-%1").arg(QString::number(index+1));
//    QSettings setting(CFG_MachineSet,QSettings::IniFormat);
//    setting.beginGroup("groupset");
//    setting.setValue(QString("view%1group").arg(1),tmp);
//    setting.endGroup();
//    setting.sync();
    setChuteNumInfo();
}

void MachineSetWidget::on_rearAuxiUintComboBox_activated(int index)
{
    m_temCnf.struLayerInfo[struGsh.nLayer].nViewUnitTotal[3] = index+1;

//    QString tmp = QString("1-%1").arg(QString::number(index+1));
//    QSettings setting(CFG_MachineSet,QSettings::IniFormat);
//    setting.beginGroup("groupset");
//    setting.setValue(QString("view%1group").arg(3),tmp);
//    setting.endGroup();
//    setting.sync();

    QSettings setting(CFG_MachineSet,QSettings::IniFormat);
    setting.beginGroup("groupset");
    setting.setValue(QString("view%1groupnum").arg(3),1);
    setting.setValue(QString("view%1group%2").arg(3).arg(0), QString("%1-%2").arg(1).arg(m_temCnf.struLayerInfo[0].nViewUnitTotal[3]));
    setting.endGroup();
    setting.sync();

    setChuteNumInfo();
}

void MachineSetWidget::on_lowLayerComboBox_activated(int index)
{
    m_temCnf.struLayerInfo[struGsh.nLayer].nVavleBoardTotal[0] = (index+1)*(m_temCnf.nValveRatio+1);
    setChuteNumInfo();
}

void MachineSetWidget::on_upLayerComboBox_activated(int index)
{
    m_temCnf.struLayerInfo[struGsh.nLayer].nVavleBoardTotal[1] = (index+1)*(m_temCnf.nValveRatio+1);
    setChuteNumInfo();
}

void MachineSetWidget::on_frontUintComboBox_3_activated(int index)
{
    m_temCnf.struLayerInfo[struGsh.nLayer].nViewUnitTotal[4] = index+1;

    QSettings setting(CFG_MachineSet,QSettings::IniFormat);
    setting.beginGroup("groupset");
    setting.setValue(QString("view%1groupnum").arg(4),1);
    setting.setValue(QString("view%1group%2").arg(4).arg(0), QString("%1-%2").arg(1).arg(m_temCnf.struLayerInfo[0].nViewUnitTotal[4]));
    setting.endGroup();
    setting.sync();
//    QString tmp = QString("1-%1").arg(QString::number(index+1));
//    QSettings setting(CFG_MachineSet,QSettings::IniFormat);
//    setting.beginGroup("groupset");
//    setting.setValue(QString("view%1group").arg(4),tmp);
//    setting.endGroup();
//    setting.sync();
    setChuteNumInfo();
}

void MachineSetWidget::on_frontAuxiliUintComboBox_3_activated(int index)
{
    m_temCnf.struLayerInfo[struGsh.nLayer].nViewUnitTotal[6] = index+1;
//    QString tmp = QString("1-%1").arg(QString::number(index+1));
//    QSettings setting(CFG_MachineSet,QSettings::IniFormat);
//    setting.beginGroup("groupset");
//    setting.setValue(QString("view%1group").arg(6),tmp);
//    setting.endGroup();
//    setting.sync();

    QSettings setting(CFG_MachineSet,QSettings::IniFormat);
    setting.beginGroup("groupset");
    setting.setValue(QString("view%1groupnum").arg(6),1);
    setting.setValue(QString("view%1group%2").arg(6).arg(0), QString("%1-%2").arg(1).arg(m_temCnf.struLayerInfo[0].nViewUnitTotal[6]));
    setting.endGroup();
    setting.sync();

    setChuteNumInfo();
}

void MachineSetWidget::on_rearUintComboBox_3_activated(int index)
{
    m_temCnf.struLayerInfo[struGsh.nLayer].nViewUnitTotal[5] = index+1;
//    QString tmp = QString("1-%1").arg(QString::number(index+1));
//    QSettings setting(CFG_MachineSet,QSettings::IniFormat);
//    setting.beginGroup("groupset");
//    setting.setValue(QString("view%1group").arg(5),tmp);
//    setting.endGroup();
//    setting.sync();

    QSettings setting(CFG_MachineSet,QSettings::IniFormat);
    setting.beginGroup("groupset");
    setting.setValue(QString("view%1groupnum").arg(5),1);
    setting.setValue(QString("view%1group%2").arg(5).arg(0), QString("%1-%2").arg(1).arg(m_temCnf.struLayerInfo[0].nViewUnitTotal[5]));
    setting.endGroup();
    setting.sync();

    setChuteNumInfo();
}

void MachineSetWidget::on_rearAuxiUintComboBox_3_activated(int index)
{
    m_temCnf.struLayerInfo[struGsh.nLayer].nViewUnitTotal[7] = index+1;
//    QString tmp = QString("1-%1").arg(QString::number(index+1));
//    QSettings setting(CFG_MachineSet,QSettings::IniFormat);
//    setting.beginGroup("groupset");
//    setting.setValue(QString("view%1group").arg(7),tmp);
//    setting.endGroup();
//    setting.sync();

    QSettings setting(CFG_MachineSet,QSettings::IniFormat);
    setting.beginGroup("groupset");
    setting.setValue(QString("view%1groupnum").arg(7),1);
    setting.setValue(QString("view%1group%2").arg(7).arg(0), QString("%1-%2").arg(1).arg(m_temCnf.struLayerInfo[0].nViewUnitTotal[7]));
    setting.endGroup();
    setting.sync();
    setChuteNumInfo();
}

void MachineSetWidget::on_comboBoxVavleRatio_activated(int index)
{
    m_temCnf.nValveRatio = index;
    setChuteNumInfo();
}

void MachineSetWidget::on_frontValveComboBox_activated(int index)
{
    m_temCnf.struLayerInfo[struGsh.nLayer].stuViewInfo[0].nEjectorsPerCamera = vavleList.at(index).toInt();
}

void MachineSetWidget::on_rearValveComboBox_activated(int index)
{
    m_temCnf.struLayerInfo[struGsh.nLayer].stuViewInfo[1].nEjectorsPerCamera = vavleList.at(index).toInt();
}

void MachineSetWidget::on_frontAuxiliValveComboBox_activated(int index)
{
    m_temCnf.struLayerInfo[struGsh.nLayer].stuViewInfo[2].nEjectorsPerCamera = vavleList.at(index).toInt();
}

void MachineSetWidget::on_rearAuxiValveComboBox_activated(int index)
{
    m_temCnf.struLayerInfo[struGsh.nLayer].stuViewInfo[3].nEjectorsPerCamera = vavleList.at(index).toInt();
}

void MachineSetWidget::on_frontValveComboBox_3_activated(int index)
{
    m_temCnf.struLayerInfo[struGsh.nLayer].stuViewInfo[4].nEjectorsPerCamera = vavleList.at(index).toInt();
}

void MachineSetWidget::on_rearValveComboBox_3_activated(int index)
{
    m_temCnf.struLayerInfo[struGsh.nLayer].stuViewInfo[5].nEjectorsPerCamera = vavleList.at(index).toInt();
}

void MachineSetWidget::on_frontAuxiliValveComboBox_3_activated(int index)
{
    m_temCnf.struLayerInfo[struGsh.nLayer].stuViewInfo[6].nEjectorsPerCamera = vavleList.at(index).toInt();
}

void MachineSetWidget::on_rearAuxiValveComboBox_3_activated(int index)
{
    m_temCnf.struLayerInfo[struGsh.nLayer].stuViewInfo[7].nEjectorsPerCamera = vavleList.at(index).toInt();
}

void MachineSetWidget::on_comboBoxCrossEjector_activated(int index)
{
    m_temCnf.nCorssEjector = crossEjectorList.at(index).toInt();
}


void MachineSetWidget::on_comboBoxControlBoard_activated(int index)
{
    m_temCnf.nCtrlBoardTotal = index;
}

void MachineSetWidget::on_InputSetBtn_clicked()
{
    if(!g_Runtime().checkUsbExist()){
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

    myMessageBox msgBox(MSG_QUES, g_myLan().cfm_import_parameter);
    int ret = msgBox.exec();
    if (ret == QDialog::Accepted)
    {
        logger()->info("import machine set");
        QFileInfo file1(path + "machineset.ini");

        if(file1.exists())
        {
            g_infoWidget().setLabelText(g_myLan().msg_importing_params);
            g_infoWidget().delayShow();

#ifdef Q_OS_UNIX
            QString str = QString("%1 %2 %3 -r").arg("cp").arg(path+"machineset.ini").arg(CFG_MachineSet);
            const char *cmd = QString(str).toLocal8Bit().constData();
            system(cmd);
            g_Runtime().mySystem("");
            system("sync");
#endif
            g_infoWidget().setLabelText(g_myLan().msg_usb_logofile_replace_success);
            g_infoWidget().showSecs();

            g_infoWidget().setLabelText(g_myLan().Reapply_machine_model);
            g_infoWidget().showSecs();
        }
    }
}

void MachineSetWidget::on_OutputSetBtn_clicked()
{
    if(!g_Runtime().checkUsbExist()){
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

    myMessageBox msgBox(MSG_QUES, g_myLan().cfm_export_parameter);

    int ret = msgBox.exec();

    if (ret == QDialog::Accepted)
    {
        logger()->info("output machine set");
        QFileInfo file1(CFG_MachineSet);

        if(file1.exists())
        {
            g_infoWidget().setLabelText(g_myLan().msg_exporting_params);
            g_infoWidget().delayShow();

#ifdef Q_OS_UNIX
            QString str = QString("%1 %2 %3 -r").arg("cp").arg(CFG_MachineSet).arg(path+"machineset.ini");
            const char *cmd = QString(str).toLocal8Bit().constData();
            system(cmd);
            g_Runtime().mySystem("");
            system("sync");
#endif
            g_infoWidget().setLabelText(g_myLan().msg_usb_logofile_replace_success);
            g_infoWidget().showSecs();
        }
    }
}

void MachineSetWidget::GroupSetBtnClicked(int view)
{
    GroupSetDialog set;
    set.nMachineType = m_temCnf.nMachineType;
    set.nView = view;
    memset(&set.mLayerInfo, 0, sizeof(struct stru_layer_info));
    memcpy(&set.mLayerInfo, &m_temCnf.struLayerInfo[struGsh.nLayer],sizeof(struct stru_layer_info));
    set.updateshow();
    int ret = set.exec();
}
