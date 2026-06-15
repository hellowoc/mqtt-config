#include "languagesetwidget.h"
#include "ui_languagesetwidget.h"
#include "global.h"
LanguageSetWidget::LanguageSetWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::LanguageSetWidget)
{
    ui->setupUi(this);

    m_pLanBtnGroup = new QButtonGroup(this);
    m_pLanBtnGroup->addButton(ui->chinese_zhBtn,0);
    m_pLanBtnGroup->addButton(ui->EnglishBtn,1);
    m_pLanBtnGroup->addButton(ui->russianBtn,2);
    m_pLanBtnGroup->addButton(ui->vietBtn,3);
    m_pLanBtnGroup->addButton(ui->thaiBtn,4);
    m_pLanBtnGroup->addButton(ui->spanBtn,5);
    m_pLanBtnGroup->addButton(ui->chtBtn,6);
    m_pLanBtnGroup->addButton(ui->koreanBtn,7);
    m_pLanBtnGroup->addButton(ui->turkeyBtn,8);
    m_pLanBtnGroup->addButton(ui->cambodiaBtn,9);
    m_pLanBtnGroup->addButton(ui->polandBtn,10);
    m_pLanBtnGroup->addButton(ui->burmaBtn,11);
    m_pLanBtnGroup->addButton(ui->persiaBtn,12);
    m_pLanBtnGroup->addButton(ui->ukraineBtn,13);
    m_pLanBtnGroup->addButton(ui->italyBtn,14);
    m_pLanBtnGroup->addButton(ui->greekBtn,15);
    m_pLanBtnGroup->addButton(ui->uighurBtn,16);
    m_pLanBtnGroup->addButton(ui->arabicBtn,17);
    m_pLanBtnGroup->addButton(ui->indonesiaBtn,18);
    m_pLanBtnGroup->addButton(ui->bulgarianBtn,19);
    m_pLanBtnGroup->addButton(ui->frenchBtn,20);
    m_pLanBtnGroup->addButton(ui->swedishBtn,21);
    m_pLanBtnGroup->addButton(ui->japanBtn,22);
    m_pLanBtnGroup->addButton(ui->PortugueseBtn,23);
    m_pLanBtnGroup->addButton(ui->HindiBtn,24);
    m_pLanBtnGroup->addButton(ui->germanBtn,25);

    ui->cambodiaBtn->hide();
    ui->bulgarianBtn->hide();
    ui->greekBtn->hide();
    ui->swedishBtn->hide();
    ui->uighurBtn->hide();
    ui->ukraineBtn->hide();

    ui->cambodiaLbl->hide();
    ui->bulgarianLbl->hide();
    ui->greekLbl->hide();
    ui->swedishLbl->hide();
    ui->uighurLbl->hide();
    ui->ukraineLbl->hide();

    m_pLangBtnMapper = new QSignalMapper(this);
    for(int i = 0;i < m_pLanBtnGroup->buttons().count();i++){
        m_pLanBtnGroup->button(i)->setText("");//清空ui文件上用于方便查找语言具体按钮设置的文字，样式表会设置
        m_pLanBtnGroup->button(i)->setCheckable(true);
        m_pLangBtnMapper->setMapping(m_pLanBtnGroup->button(i),i);
        connect(m_pLanBtnGroup->button(i),SIGNAL(clicked()),m_pLangBtnMapper,SLOT(map()));
    }
    connect(m_pLangBtnMapper,SIGNAL(mapped(int)),this,SLOT(langBtn_clicked(int)));
}

LanguageSetWidget::~LanguageSetWidget()
{
    delete ui;
}

void LanguageSetWidget::showPage(bool bshow)
{
    if(bshow){
       updateLanBtnState();
    }
}

void LanguageSetWidget::retranslateUiWidget()
{
    g_MainManager().RefreshCurWidget();
}

void LanguageSetWidget::langBtn_clicked(int index)
{
    g_myLan().g_languageIndex = index+1;
    logger()->info("set language setting %1", g_myLan().g_languageIndex);
    updateLanBtnState();
    g_myLan().ChangeLanguage();
}

void LanguageSetWidget::updateLanBtnState()
{
    QPushButton * btn;
    for(int i = 0;i < m_pLanBtnGroup->buttons().count();i++){
        btn = (qobject_cast<QPushButton*>)(m_pLanBtnGroup->button(i));
        if(i == g_myLan().g_languageIndex-1){
            btn->setChecked(true);
        }else{
            btn->setChecked(false);
        }
    }
    ui->arabicLbl->setVisible(g_myLan().g_languageIndex == 1);
    ui->burmaLbl->setVisible(g_myLan().g_languageIndex == 1);
    ui->chinese_zhLbl->setVisible(g_myLan().g_languageIndex == 1);
    ui->chtLbl->setVisible(g_myLan().g_languageIndex == 1);
    ui->EnglishLbl->setVisible(g_myLan().g_languageIndex == 1);
    ui->italyLbl->setVisible(g_myLan().g_languageIndex == 1);
    ui->japanLbl->setVisible(g_myLan().g_languageIndex == 1);
    ui->koreanLbl->setVisible(g_myLan().g_languageIndex == 1);
    ui->persiaLbl->setVisible(g_myLan().g_languageIndex == 1);
    ui->PortugueseLbl->setVisible(g_myLan().g_languageIndex == 1);
    ui->russianLbl->setVisible(g_myLan().g_languageIndex == 1);
    ui->frenchLbl->setVisible(g_myLan().g_languageIndex == 1);
    ui->spanLbl->setVisible(g_myLan().g_languageIndex == 1);
    ui->thaiLbl->setVisible(g_myLan().g_languageIndex == 1);
    ui->turkeyLbl->setVisible(g_myLan().g_languageIndex == 1);
    ui->vieLbl->setVisible(g_myLan().g_languageIndex == 1);
    ui->polandLbl->setVisible(g_myLan().g_languageIndex == 1);
    ui->indonesiaLbl->setVisible(g_myLan().g_languageIndex == 1);
    ui->HindiLbl->setVisible(g_myLan().g_languageIndex == 1);
    ui->germanLbl->setVisible(g_myLan().g_languageIndex == 1);
}
