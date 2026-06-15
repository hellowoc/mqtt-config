#include "createnewmodewidget.h"
#include "ui_createnewmodewidget.h"

CreateNewModeWidget::CreateNewModeWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::CreateNewModeWidget)
{
    ui->setupUi(this);
    m_riceModeList<<"common"<<"yellow_white"<<"ids_yise"<<"yellow_alpha"<<"belly_white";
    m_pBtnGroup = new QButtonGroup(this);
    m_pBtnGroup->addButton(ui->radioButton1,0);
    m_pBtnGroup->addButton(ui->radioButton_2,1);
    m_pBtnGroup->addButton(ui->radioButton_3,2);
    m_pBtnGroup->addButton(ui->radioButton_4,3);
    m_pBtnGroup->addButton(ui->radioButton_5,4);
    m_pBtnGroup->addButton(ui->radioButton_6,5);
    m_pBtnGroup->addButton(ui->radioButton_7,6);
    m_pBtnGroup->addButton(ui->radioButton_8,7);
    m_pBtnGroup->addButton(ui->radioButton_9,8);

    m_pSignalMapper = new QSignalMapper(this);
    for(int i = 0;i < m_pBtnGroup->buttons().count();i++){
        m_pSignalMapper->setMapping(m_pBtnGroup->button(i),i);
        connect(m_pBtnGroup->button(i),SIGNAL(clicked()),m_pSignalMapper,SLOT(map()));
    }
    connect(m_pSignalMapper,SIGNAL(mapped(int)),this,SLOT(mapBtnClicked(int)));
}

CreateNewModeWidget::~CreateNewModeWidget()
{
    delete ui;
}

void CreateNewModeWidget::showPage(bool bshow)
{
    if(bshow){
        retranslateUiWidget();
        m_modeIndex = 0;
        m_pBtnGroup->button(m_modeIndex)->setChecked(true);
    }
}

void CreateNewModeWidget::receiveMsg(int msg1, int msg2, int msg3)
{

}

void CreateNewModeWidget::retranslateUiWidget()
{
    ui->cancelBtn->setText(g_myLan().ids_cancel);
    ui->okBtn->setText(g_myLan().ids_ok);
    switch(struCnfg.nMachineType){
    case MACHINE_VERTICAL_DM_RG:
    case MACHINE_VERTICAL_DM_ZK:

        m_modeNameLanList.clear();
        m_modeNameLanList<<g_myLan().common<<g_myLan().yellow_white<<g_myLan().ids_yise<<g_myLan().yellow_alpha<<g_myLan().belly_white;
        for(int i = 0;i < m_pBtnGroup->buttons().count();i++){
            if(i < m_riceModeList.count()){
                m_pBtnGroup->button(i)->setVisible(true);
                m_pBtnGroup->button(i)->setText(m_modeNameLanList[i]);
            }
            else
                m_pBtnGroup->button(i)->setVisible(false);

        }
        break;
    case MACHINE_VERTICAL_ZL_CS:
    case MACHINE_VERTICAL_ZL_CV:
        for(int i = 0;i < m_pBtnGroup->buttons().count();i++){
            if(i < m_foodModeList.count())
                m_pBtnGroup->button(i)->setVisible(true);
            else
                m_pBtnGroup->button(i)->setVisible(false);
        }
        break;
    default:
        break;

    }
}
void CreateNewModeWidget::on_cancelBtn_clicked()
{
    returnBack();
}

void CreateNewModeWidget::on_okBtn_clicked()
{
    QString smaterialName = m_modeNameLanList[m_modeIndex];
    QByteArray sMaterialArr = smaterialName.toUtf8();

    memset(struCnfg.stuProfileIndex[struCnfg.nProfileTotal].sMaterialName, 0,
            sizeof(struCnfg.stuProfileIndex[struCnfg.nProfileTotal].sMaterialName));
    memcpy(struCnfg.stuProfileIndex[struCnfg.nProfileTotal].sMaterialName,
            sMaterialArr.data(), sMaterialArr.length());
    QDateTime time = QDateTime::currentDateTime();
    QString tmpTime = time.toString("yyyyMMddhhmmsszzz");
    QByteArray array = tmpTime.toUtf8();
    const char *strName = array.data();

    memset(struCnfg.stuProfileIndex[struCnfg.nProfileTotal].sProfileName, 0, sizeof(struCnfg.stuProfileIndex[struCnfg.nProfileTotal].sProfileName));
    memcpy(struCnfg.stuProfileIndex[struCnfg.nProfileTotal].sProfileName, strName, array.length());

    /* 改变当前方案索引号 */
    struCnfg.nProfile = struCnfg.nProfileTotal;
    struCnfg.nProfileTotal++;
    myFlow.saveGlobal();

    QSettings setting(CFG_APPSet,QSettings::IniFormat);

    switch(struCnfg.nMachineType){
    case MACHINE_VERTICAL_DM_RG:
    case MACHINE_VERTICAL_DM_ZK:
    {
        setting.beginGroup("rice");
        g_Runtime().m_expModeName = setting.value(m_riceModeList[m_modeIndex],"yellow").toString();
        setting.endGroup();
    }
        break;
    case MACHINE_VERTICAL_ZL_CS:
    case MACHINE_VERTICAL_ZL_CV:

        break;
    default:
        break;
    }

    g_infoWidget().setLabelText(g_myLan().ids_apply);
    g_infoWidget().delayShow();
    myFlow.createProfile();
    myFlow.saveProfile();
    /* 重新初始化方案 */
    myFlow.resetSortParams();
    g_infoWidget().hide();
    returnBack();
}

void CreateNewModeWidget::mapBtnClicked(int index)
{
    m_modeIndex = index;
}
