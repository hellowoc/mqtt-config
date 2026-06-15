#include "sysinfodebug.h"
#include "ui_sysinfodebug.h"
#include <QDebug>

SysInfoDebug::SysInfoDebug(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::SysInfoDebug)
{
    ui->setupUi(this);
    ui->lineEdit->setType(textType);
//    setStyleSheet("QTextEdit{font:18px;} QLineEdit{min-width:85px;max-width:1000px;min-height:50px;max-height:50px;qproperty-alignment:AlignLeft;}");
    setStyleSheet("QTextEdit{font:16px;} QLineEdit{font:16px;min-width:85px;max-width:1000px;min-height:50px;max-height:50px;qproperty-alignment:AlignLeft;}");
    m_pThread = new SysInfoDebugThread(this);
//    ui->pushButton_2->hide();
    connect(m_pThread,SIGNAL(finished()),this,SLOT(debugFinished()));
    cmdType = ARM;
}

SysInfoDebug::~SysInfoDebug()
{
    delete ui;
    delete m_pThread;
    m_pThread = NULL;
}

void SysInfoDebug::retranslateUiWidget()
{
    ui->pushButton->setText(g_myLan().update);
    ui->pushButton_2->setText(g_myLan().clear);
}

void SysInfoDebug::on_pushButton_clicked()
{
    cmdType = ARM;
    ui->textEdit->clear();
    cmd = ui->lineEdit->text();
    if(cmd == "zktime"){
        QString str = getZkTime(0);
        ui->textEdit->setText(str);
        return;
    }
    if(cmd == "fptime"){
        QString str = getZkTime(1);
        ui->textEdit->setText(str);
        return;
    }
    if(cmd == "emtime"){
        QString str = getZkTime(2);
        ui->textEdit->setText(str);
        return;
    }
    if(cmd == "zkdebug on"){
        struGsh.debugMode = 1;
        QSettings setting(DEBUG_MODE_Set,QSettings::IniFormat);
        setting.setValue("debugmode",struGsh.debugMode);
        setting.sync();
        QString str = "zkdebug start!";
        ui->textEdit->setText(str);
        return;
    }
    if(cmd == "zkdebug off"){
        struGsh.debugMode = 0;
        QSettings setting(DEBUG_MODE_Set,QSettings::IniFormat);
        setting.setValue("debugmode",struGsh.debugMode);
        setting.sync();
        QString str = "zkdebug finish!";
        ui->textEdit->setText(str);
        return;
    }

    g_infoWidget().setLabelText(g_myLan().msg_connecting);
    g_infoWidget().delayShow();
    m_pThread->start();

}

void SysInfoDebug::on_pushButton_2_clicked()
{
    cmdType = RK;
    ui->textEdit->clear();
    cmd = ui->lineEdit->text();

    g_infoWidget().setLabelText(g_myLan().msg_connecting);
    g_infoWidget().delayShow();
    m_pThread->start();
}

void SysInfoDebug::debugFinished()
{
    ui->textEdit->setText(m_pThread->strInfo);
    g_infoWidget().hide();
}


SysInfoDebugThread::SysInfoDebugThread(SysInfoDebug *parent)
{
    m_parent = parent;
}

void SysInfoDebugThread::run()
{
    strInfo.clear();
    int i = 0;
    if(m_parent->cmdType == ARM)
    {
        if(struGsh.debugMode == 1 && m_parent->cmd.contains("["))
        {
            QFile file("/sdcard/cnf/.log");
            if(file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                while (!file.atEnd())
                {
                    QByteArray line = file.readLine();
                    QString str(line);
                    if(!str.contains(m_parent->cmd)) continue;
    //                str.remove("\n");
                    if(str.contains("head:[53 b9] "))
                    {
                        QStringList list = str.split("head:[53 b9] ");
                        if(list.size() > 1)
                            str = list.last();
                    }
                    if(str.contains("HEAD[53 b9] "))
                        str.replace("HEAD[53 b9] ","");

                    strInfo.append(str);
                }
            }
            file.close();
            return;
        }
        QProcess m_cmd;
        m_cmd.start("sh",QStringList()<<"-c"<<m_parent->cmd);
        bool ret = m_cmd.waitForFinished(-1);
        if(!ret){
            strInfo = "update fail --------------->\n";
            strInfo.append(m_cmd.readAll());
            g_infoWidget().setLabelText(g_myLan().disconnect);
            g_infoWidget().showSecs();
        }
        else{
            strInfo = "update success --------------->\n";
            strInfo.append(m_cmd.readAll());
        }
        m_cmd.close();
    }
    else{
        unsigned char buf[125] = "/data/.res";
        unsigned char cmd[128];

        QByteArray ba = m_parent->cmd.toLocal8Bit();
        struIpcShare.struIpcReqStauts[struGsh.nView][struGsh.nUnit].finshShellCmdReq = IPC_REQ_RETURN_NULL;
        g_Udp.pushUdpCmdData(CMD_SCREEN_SHELL_CMD, true, struGsh.nView, struGsh.nUnit, m_parent->cmd.length(),(unsigned char*)ba.data() );
        for(i = 0; i < 30; i++ ){
            msleep(100);
            if(struIpcShare.struIpcReqStauts[struGsh.nView][struGsh.nUnit].finshShellCmdReq == 1){
                break;
            }
        }
        if( i == 30 ){
            strInfo = "update rk err --------------->\n";
            return;
        }
        cmd[0]=0;
        memcpy(&cmd[1],buf,sizeof(buf));
        g_Udp.pushUdpCmdData(CMD_SCREEN_GET_FILE, true, struGsh.nView, struGsh.nUnit, sizeof(cmd), cmd );

        g_Runtime().delFile("/sdcard/cnf/.res");
        for(i = 0; i < 30; i++ ){
            msleep(100);
            if(struIpcShare.struIpcReqStauts[struGsh.nView][struGsh.nUnit].getFileReq == 1){
                break;
            }
        }
        if( i == 30 ){
            strInfo = "update res err --------------->\n";
            return;
        }
        QFile file("/sdcard/cnf/.res");
        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            while (!file.atEnd())
            {
                QByteArray line = file.readLine();
                QString str(line);
                strInfo.append(str);
            }
        }
        file.close();
        return;
    }

}

/******************************MY FUNC*******************************/
QString SysInfoDebug::getZkTime(int type)
{
    char curYear[64];
    char curMonth[64];
    char curDay[64];
    char curHour[64];
    char curMin[64];
    char curSec[64];
    time_t curtime = 0;
    struct tm *stru_curtime;
    FILE *fp;

    if(type == 0)
    {
//        getSysTimeFromE2prom(&curtime);
    }
    else if(type == 1)
    {
        char strgettime[64];
        int year = struGsh.struDccryptData.nDccryptGetTime[0]*256+struGsh.struDccryptData.nDccryptGetTime[1];
        int month = struGsh.struDccryptData.nDccryptGetTime[2];
        int date = struGsh.struDccryptData.nDccryptGetTime[3];
        int hour = struGsh.struDccryptData.nDccryptGetTime[4];
        int min = struGsh.struDccryptData.nDccryptGetTime[5];
        int sec = struGsh.struDccryptData.nDccryptGetTime[6];

        sprintf(strgettime, "%04d/%02d/%02d %02d:%02d:%02d", year, month, date, hour, min, sec);
        return QString(strgettime);
    }
    if(type == 2)
    {
//        getSysTimeFromEmmc(&curtime);
    }

    stru_curtime = localtime(&curtime);
    strftime(curYear ,64,"%Y",stru_curtime);
    strftime(curMonth,64,"%m",stru_curtime);
    strftime(curDay  ,64,"%d",stru_curtime);
    strftime(curHour ,64,"%H",stru_curtime);
    strftime(curMin  ,64,"%M",stru_curtime);
    strftime(curSec  ,64,"%S",stru_curtime);

    char time[64];
    sprintf(time, "%04d/%02d/%02d %02d:%02d:%02d", atoi(curYear), atoi(curMonth),
            atoi(curDay),atoi(curHour),atoi(curMin),atoi(curSec));

    return QString(time);
}

/****************************END MY FUNC*****************************/
