#include "remoteControl.h"
#include "ui_remoteControl.h"

#ifdef Q_OS_UNIX
RemoteControlWidget::RemoteControlWidget(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::RemoteControlWidget)
{
    ui->setupUi(this);

    timer_start_1.setSingleShot(true);
    timer_start_2.setSingleShot(true);

    QObject::connect(&timer_start_1, SIGNAL(timeout()), this, SLOT(on_pushButton_clicked())); //
    QObject::connect(&timer_start_2, SIGNAL(timeout()), this, SLOT(autolockscreem())); //


#ifdef Q_OS_UNIX
//    QObject::connect(myMqttMsgParaseThread, SIGNAL(EndRemoteControl()), this, SLOT(dealendremotecontrol())); //
#endif
}



RemoteControlWidget::~RemoteControlWidget()
{
    delete ui;
}

void RemoteControlWidget::showPage(bool bshow)
{
    if(bshow)
    {
        QString str = g_myLan().Remote_request_lockControl;

        if( struGsh.remoControlmode == 2 )
        {
    #ifdef Q_OS_UNIX
//            str += "\n" + myMqttMsgParaseThread->userphonenum;
    #endif
            ui->label->setText(str);
            timer_start_2.start( 10 * 1000 );// 开始计时
            ui->pushButton->hide();
            ui->pushButton_2->show();
            ui->pushButton_3->show();
        }

    }
    else
    {
        if(timer_start_2.isActive())
            timer_start_2.stop();
    }

}

void RemoteControlWidget::receiveMsg(int msg1, int msg2, int msg3)
{



}



void RemoteControlWidget::retranslateUiWidget()
{

}


void RemoteControlWidget::dealendremotecontrol()// 远程指令断开连接
{
    if(struGsh.remoControlmode == 1) // 当前已经锁屏
    {
        on_pushButton_clicked();

    }
    else if(struGsh.remoControlmode == 2) //　当前在等待确认状态
    {
        if(timer_start_2.isActive())
            timer_start_2.stop();


        struGsh.remoControlmode = 0;
        struGsh.remoteControlstatus = REMOTE_CANCEL; //结束控制

#ifdef Q_OS_UNIX
//        myMqttMsgParaseThread->userphonenum = "";
#endif
        g_infoWidget().setLabelText(g_myLan().ids_cancel);
        g_infoWidget().showSecs(1);
        g_infoWidget().hide();


        g_MainManager().returnBackPage();
    }

#ifdef Q_OS_UNIX

//        myMqttMsgParaseThread->n_uploadRemoteControlFlag = 1;
#endif

}

void RemoteControlWidget::on_pushButton_clicked()// 断开连接状态
{
    if(timer_start_1.isActive())
        timer_start_1.stop();

    struGsh.remoteControlstatus = 0;
    struGsh.remoControlmode = 0;//结束控制

#ifdef Q_OS_UNIX
//    myMqttMsgParaseThread->userphonenum = "";
   // myMqttThread->n_uploadRemoteControlFlag = 1;
#endif


    g_infoWidget().setLabelText(g_myLan().disconnect);
    g_infoWidget().showSecs(1);
    g_infoWidget().hide();

    g_MainManager().returnBackPage();
}

void RemoteControlWidget::autolockscreem()// 默认锁屏
{
    if(timer_start_2.isActive())
        timer_start_2.stop();

    struGsh.remoControlmode = 1;
    qDebug()<<"autolockscreem";
    struGsh.remoteControlstatus = 0; //控制结果

    QString str = g_myLan().Remote_lockControl;
#ifdef Q_OS_UNIX

//    myMqttMsgParaseThread->n_uploadRemoteControlFlag = 1;
//    str += "\n" + myMqttMsgParaseThread->userphonenum;

#endif
    ui->label->setText(str);
    timer_start_1.start( struGsh.remoControltime * 60 * 1000 );// 开始计时

    ui->pushButton->show();
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();
}

void RemoteControlWidget::on_pushButton_3_clicked()// 手动取消锁屏
{
    if(timer_start_2.isActive())
        timer_start_2.stop();

    struGsh.remoteControlstatus = USERREJECT; //用户拒绝
    struGsh.remoControlmode = 0;
#ifdef Q_OS_UNIX
//    myMqttMsgParaseThread->userphonenum = "";
//    myMqttMsgParaseThread->n_uploadRemoteControlFlag = 1;
#endif

    g_infoWidget().setLabelText(g_myLan().disconnect);
    g_infoWidget().showSecs(1);
    g_infoWidget().hide();
    g_MainManager().returnBackPage();
}


void RemoteControlWidget::on_pushButton_2_clicked()// 手动确认锁屏
{
    if(timer_start_2.isActive())
        timer_start_2.stop();

    QString str = g_myLan().Remote_lockControl;
    struGsh.remoControlmode = 1;
    struGsh.remoteControlstatus = 0; //

#ifdef Q_OS_UNIX
//    str += "\n" +  myMqttMsgParaseThread->userphonenum;
//    myMqttMsgParaseThread->n_uploadRemoteControlFlag = 1;
#endif

    ui->label->setText(str);
    timer_start_1.start( struGsh.remoControltime * 60 * 1000 );// 开始计时

    ui->pushButton->show();
    ui->pushButton_2->hide();
    ui->pushButton_3->hide();
}

#endif
