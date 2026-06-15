#include "sysinfonet.h"
#include "ui_sysinfonet.h"
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QColor>

SysInfoNet::SysInfoNet(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::SysInfoNet)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(updateTextEdit()));
    ui->startIpEdit->setType(textType);
    ui->endIpEdit->setType(textType);
    QSettings setting(CFG_APPSet,QSettings::IniFormat);
    QString startIp = setting.value("startIp","162.254.129.10").toString();
    QString endIp = setting.value("endIp","162.254.129.11").toString();
    ui->startIpEdit->setText(startIp);
    ui->endIpEdit->setText(endIp);

    ui->startIpLbl->setText(g_myLan().Begin_IP);
    ui->endIpLbl->setText(g_myLan().End_IP);
    ui->checkBtn->setText(g_myLan().ids_query);
}

SysInfoNet::~SysInfoNet()
{
    delete ui;
}

void SysInfoNet::showPage(bool bshow)
{
    if(bshow){



    }else{
        if(timer->isActive())
            timer->stop();
    }
}
void SysInfoNet::receiveMsg(int msg1, int msg2, int msg3)
{

}
void SysInfoNet::retranslateUiWidget()
{

}

void SysInfoNet::on_checkBtn_clicked()
{
    if(timer->isActive()) timer->stop();

    QString startIp = ui->startIpEdit->text();
    QString endIp   = ui->endIpEdit->text();

    if(startIp.count(".") != endIp.count(".") || startIp.count(".") != 3)
    {
        qDebug()<<"ip set err";
        return;
    }
    QSettings setting(CFG_APPSet,QSettings::IniFormat);
    setting.setValue("startIp",startIp);
    setting.setValue("endIp",endIp);
    setting.sync();

    g_Runtime().mySystem("killall findActiveIp.sh; ./findActiveIp.sh "+startIp+" "+endIp+"&");
    timer->start(1000);
}

void SysInfoNet::updateTextEdit()
{
      QFile file(ACTIVE_IP_PATH);
      if(!file.exists())
      {
          timer->stop();
          return;
      }
      if(file.open(QIODevice::ReadOnly | QIODevice::Text))
      {
          QByteArray line = file.readAll();
          ui->textEdit->setText(line);
          if(ui->textEdit->toPlainText().contains(ui->endIpEdit->text()))
              timer->stop();
      }
      ui->textEdit->moveCursor(QTextCursor::End);
      file.close();
}
