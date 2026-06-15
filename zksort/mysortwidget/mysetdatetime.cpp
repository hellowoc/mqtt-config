#include "mysetdatetime.h"
#include "ui_mysetdatetime.h"
#include <QDateTime>
#include <QDebug>
MySetDateTime::MySetDateTime(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MySetDateTime)
{

    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    QDateTime  dateTime = QDateTime::currentDateTime();

    data[0]          = dateTime.date().year();
    data[1]          = dateTime.date().month();
    data[2]          = dateTime.date().day();
    data[3]          = dateTime.time().hour();
    data[4]          = dateTime.time().minute();
    data[5]          = dateTime.time().second();
    ui->lineEditYear->setText(QString::number(data[0]));
    ui->lineEditYear->setMinMax(this,0,2100,0,&data[0]);
    ui->lineEditMonth->setText(QString::number(data[1]));
    ui->lineEditMonth->setMinMax(this,0,12,1,&data[1]);
    ui->lineEditDay->setText(QString::number(data[2]));
    ui->lineEditDay->setMinMax(this,0,31,2,&data[2]);

    ui->lineEditHour->setText(QString::number(data[3]));
    ui->lineEditHour->setMinMax(this,0,23,3,&data[3]);
    ui->lineEditMinute->setText(QString::number(data[4]));
    ui->lineEditMinute->setMinMax(this,0,59,4,&data[4]);
    ui->lineEditSecond->setText(QString::number(data[5]));
    ui->lineEditSecond->setMinMax(this,0,59,5,&data[5]);
    ui->okBtn->setText(g_myLan().ids_ok);
    ui->cancelBtn->setText(g_myLan().ids_cancel);
    retranslateUiWidget();
}

MySetDateTime::~MySetDateTime()
{
    delete ui;
}

void MySetDateTime::retranslateUiWidget()
{
    ui->label->setText(g_myLan().year);
    ui->label_2->setText(g_myLan().month);
    ui->label_3->setText(g_myLan().day);
    ui->label_4->setText(g_myLan().hour);
    ui->label_5->setText(g_myLan().minute);
    ui->label_6->setText(g_myLan().sec);
    ui->okBtn->setText(g_myLan().ids_ok);
    ui->cancelBtn->setText(g_myLan().ids_cancel);
}

void MySetDateTime::receiveMsg(int msg1, int msg2, int msg3)
{
#ifdef Q_OS_UNIX
    char time[64];
    sprintf(time, "date -s \"%04d-%02d-%02d %02d:%02d:%02d\"", data[0], data[1], data[2],
            data[3], data[4], data[5]);
    system(time);
    popen("hwclock -w","r");
#endif
}

void MySetDateTime::on_cancelBtn_clicked()
{
    reject();
}

void MySetDateTime::on_okBtn_clicked()
{
    accept();
}
