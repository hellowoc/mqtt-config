#include "renameprofiledialog.h"
#include "ui_renameprofiledialog.h"

RenameProfileDialog::RenameProfileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RenameProfileDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
}

RenameProfileDialog::~RenameProfileDialog()
{
    delete ui;
}

void RenameProfileDialog::initName(QString name)
{
    ui->lineEdit->setText(name);
}

QString RenameProfileDialog::getName()
{
    return ui->lineEdit->text();
}

void RenameProfileDialog::on_buttonBox_rejected()
{
    reject();
}

void RenameProfileDialog::on_buttonBox_accepted()
{
    accept();
}
