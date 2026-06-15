#include "messagedialog.h"
#include "ui_messagedialog.h"

MessageDialog::MessageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
}

MessageDialog::MessageDialog(QString msg, QString title, int type, QWidget *parent):
    QDialog(parent),
ui(new Ui::MessageDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    ui->label->setText(msg);
    if(type == 0){
        ui->pushButton->show();
        ui->pushButton_2->hide();
    }else if(type == 1){
        ui->pushButton->show();
        ui->pushButton_2->show();
    }else if(type == -1){
        ui->pushButton->hide();
        ui->pushButton_2->hide();
    }

}

MessageDialog::~MessageDialog()
{
    delete ui;
}

void MessageDialog::setText(QString msg)
{
    ui->label->setText(msg);
}

void MessageDialog::on_pushButton_clicked()
{
    accept();
}

void MessageDialog::on_pushButton_2_clicked()
{
    reject();
}
