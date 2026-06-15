#include "basearithwidget.h"

BaseArithWidget::BaseArithWidget(QWidget *parent) :BaseUi(parent)
{
    m_pModeBtnGroup = new QButtonGroup(this);
    m_arithType = 0;
}

BaseArithWidget::~BaseArithWidget()
{

}

void BaseArithWidget::setArithType(int type)
{
    m_arithType = type;
}

void BaseArithWidget::connectSlots()
{
    for(int i = 0;i< 2;i++){
        QPushButton *btn = (QPushButton*)m_pModeBtnGroup->button(i);
        btn->setCheckable(true);
        connect(btn,SIGNAL(clicked(bool)),this,SLOT(modeBtnClicked(bool)));
        connect(btn,SIGNAL(pressed()),this,SLOT(modePressed()));
    }
}

void BaseArithWidget::modeBtnClicked(bool checked)
{
    if(checked)
        onModeChange(m_pModeBtnGroup->id((QPushButton*)sender()));
}

void BaseArithWidget::modePressed()
{
    int a = 0;
}

