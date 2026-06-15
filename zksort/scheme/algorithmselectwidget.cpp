#include "algorithmselectwidget.h"
#include "ui_algorithmselectwidget.h"

AlgorithmSelectWidget::AlgorithmSelectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AlgorithmSelectWidget)
{
    ui->setupUi(this);
}

AlgorithmSelectWidget::~AlgorithmSelectWidget()
{
    delete ui;
}
