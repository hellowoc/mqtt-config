#include "myswitchwidget.h"
#include "ui_myswitchwidget.h"

MySwitchWidget::MySwitchWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MySwitchWidget)
{
    ui->setupUi(this);
    connect(ui->switchbtn,SIGNAL(toggled(bool)),this,SLOT(switchToggledSlots(bool)));
}

MySwitchWidget::~MySwitchWidget()
{
    delete ui;
}

QString MySwitchWidget::getText()
{
    return text;
}

void MySwitchWidget::setText(const QString titletext)
{
    text = titletext;
    ui->switchText->setText(text);
}

void MySwitchWidget::setToggle(bool checked)
{
    ui->switchbtn->setToggle(checked);
}

void MySwitchWidget::switchToggledSlots(bool checked)
{
    emit switchToggledSignal(checked);
}
