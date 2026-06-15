#include "sysinfolog.h"
#include "ui_sysinfolog.h"
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QColor>

SysInfoLog::SysInfoLog(QWidget *parent) :
    basewidget(parent),
    ui(new Ui::SysInfoLog)
{
    ui->setupUi(this);


    QDateTime  dateTime = QDateTime::currentDateTime();

    m_dateTime[0] = dateTime.date().year();
    m_dateTime[1] = dateTime.date().month();
    m_dateTime[2] = dateTime.date().day();

    ui->widget_1->setTitle(g_myLan().year,1);
    ui->widget_2->setTitle(g_myLan().month,1);
    ui->widget_3->setTitle(g_myLan().day,1);
    ui->pushButton->setText(g_myLan().update);

    ui->widget_1->setValue( m_dateTime[0]);
    ui->widget_2->setValue( m_dateTime[1]);
    ui->widget_3->setValue( m_dateTime[2]);
    ui->widget_1->setParams(this,2000,2099,MSG_ID_YEAR,&m_dateTime[0]);
    ui->widget_2->setParams(this,1,12,MSG_ID_MONTH,&m_dateTime[1]);
    ui->widget_3->setParams(this,1,31,MSG_ID_DAY,&m_dateTime[2]);
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(updateLogInfo()));
}

SysInfoLog::~SysInfoLog()
{
    delete ui;
}

void SysInfoLog::showPage(bool bshow)
{
    if(g_Runtime().checkUsbExist()){
        ui->pushButton_2->show();
    }
    else{
        ui->pushButton_2->hide();
    }
}
void SysInfoLog::receiveMsg(int msg1, int msg2, int msg3)
{

    if(msg1 == MSG_DATA_CHANGED)
    {
//        if(msg2 == MSG_ID_YEAR)
//        {
//            qDebug()<<"2020~";
//        }
    }
}
void SysInfoLog::retranslateUiWidget()
{
    ui->widget_1->setTitle(g_myLan().year,1);
    ui->widget_2->setTitle(g_myLan().month,1);
    ui->widget_3->setTitle(g_myLan().day,1);
    ui->pushButton->setText(g_myLan().update);
    ui->pushButton_2->setText(g_myLan().ai_analysis);
}
void SysInfoLog::updateLogInfo()
{
    g_infoWidget().setLabelText(g_myLan().ids_query+"...");
    ui->textEdit->clear();
    bool cur = true; //查询当天的日志
    QString name = "log.txt";
    QDateTime  dateTime = QDateTime::currentDateTime();

    QString year  = QString("%1").arg(m_dateTime[0],4,10,QChar('0'));
    QString month = QString("%1").arg(m_dateTime[1],2,10,QChar('0'));
    QString day   = QString("%1").arg(m_dateTime[2],2,10,QChar('0'));

    //行头：日期信息
    QString linehead = QString(year)+"-"+QString(month)+"-"+QString(day);

//    QString name = linehead+ ".txt";  //拟查找文件
    if ( m_dateTime[0] != dateTime.date().year()
        || m_dateTime[1] != dateTime.date().month()
        || m_dateTime[2] != dateTime.date().day())
    {
        cur = false;
        name = name + "."+ linehead;
    }
//    QString filename = "";            //实际日志文件

    QString filename(DEBUG_INFO);
    if(year == "2099" && month == "12" && day == "31")
        goto update;

    {
#ifdef Q_OS_WIN
        QString path = qApp->applicationDirPath()+"/logs/";
#else
        QString path = "/sdcard/logs/";
#endif

        QFileInfo logFileInfo(path+name);

        QString str1 = "touch " + logFileInfo.absoluteFilePath();
        const char *cmd1 = QString(str1).toLocal8Bit().constData();

        //查找实际日志文件
        QDir dir(path);
        if(!logFileInfo.exists())
        {
#ifdef Q_OS_WIN
            return;
#endif
            g_Runtime().mySystem(str1);
            QStringList filter;
            filter<<"log.txt.*";
        dir.setNameFilters(filter);
        QList<QFileInfo> fileInfo = QList<QFileInfo>(dir.entryInfoList(filter,QDir::AllEntries,QDir::Name));

        int i;
        for(i = 0; i < fileInfo.size(); i++)
        {
            filename = fileInfo.at(i).suffix();
            if (linehead == filename)
            {
                //所创建的为所有文件中日期最早的或只有当前创建文件
                if( i == 0 || fileInfo.size() == 1 )
                {
                    QFile::remove(fileInfo.at(i).absoluteFilePath());
                    g_infoWidget().hide();
                    return;
                }
                filename = fileInfo.at(i-1).absoluteFilePath();
                break;
            }
        }
        if(fileInfo.size() != 0)
            QFile::remove(fileInfo.at(i).absoluteFilePath());
    }
    else{
        filename = logFileInfo.absoluteFilePath();
    }
    }
update:
    //日志文件读取解析
    QFile file(filename);

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        if(!filename.contains(".log"))
        {
            while (!file.atEnd())
            {
                QByteArray line = file.readLine();
                QString str(line);
                str.remove("\n");// 去掉日志换行显示

                if(str == "header" || str == "footer") continue;
                QStringList sections =  str.split(QRegExp(" "));
                if(sections.at(0) == linehead)
                    ui->textEdit->append(str);
            }
        }
        else
        {
            QByteArray line = file.readAll();
            ui->textEdit->setText(line);
        }
    }

    ui->textEdit->moveCursor(QTextCursor::Start);
    file.close();
    g_infoWidget().hide();
}

void SysInfoLog::on_pushButton_2_clicked()
{
//    unsigned char buf[64] = "touch /data/test.txt;echo this is test >/data/test2.txt";
//    qDebug()<<buf;
//    for(int i = 0; i < struCnfg.nLayerTotal; i++)
//    {
//        for(int j = 0; j < struCnfg.struLayerInfo[i].nViewChainTotal; j++)
//        {
//            if(struCnfg.struLayerInfo[i].nViewIsUsed[j] == 1)
//            {
//                for(int k = 0; k <struCnfg.struLayerInfo[i].nViewUnitTotal[j]; k++)
//                {
//                    if(struCnfg.struIpcCameraParams[i][j][k].ipcType != IPC_CAMERA_TYPE_NULL)
//                    {
//                        g_Udp.pushUdpCmdData(CMD_SCREEN_SHELL_CMD, true, j, k, sizeof(buf), buf );
//                    }
//                }
//            }
//        }
//    }
//    return;

    unsigned char buf[125] = "/tmp/res";
    unsigned char cmd[128];
    cmd[0]=0;
    memcpy(&cmd[1],buf,sizeof(buf));
    printf("%s\n",buf);

    g_Udp.pushUdpCmdData(CMD_SCREEN_GET_FILE, true, struGsh.nView, struGsh.nUnit, sizeof(cmd), cmd );
    myFlow.sleep(1);
    QFile file("/sdcard/cnf/.res");

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray line = file.readAll();
        ui->textEdit->setText(line);
    }

    ui->textEdit->moveCursor(QTextCursor::Start);
    file.close();
    return;

    ui->textEdit->clear();
    if(g_Runtime().checkUsbExist()){
        QString bmpCnt="0";
        QStringList sdSpace;
        QString path = g_Runtime().getUsbPath();
        if (path != ""){
            QDateTime  dateTime = QDateTime::currentDateTime();
            QString year  = QString("%1").arg(m_dateTime[0],4,10,QChar('0'));
            QString month = QString("%1").arg(m_dateTime[1],2,10,QChar('0'));
            QString day   = QString("%1").arg(m_dateTime[2],2,10,QChar('0'));

            //行头：日期信息
            QString fileName = QString(year)+QString(month)+QString(day);
            QString filePath;
            QString imagePath = path+"IMG/"+fileName;
            QFileInfo fileinfo(imagePath);
            if(fileinfo.exists()){
                g_infoWidget().setLabelText(g_myLan().ids_query+"...");
                QProcess m_cmd;
                QString cmd = "ls -lR "+imagePath+"|grep ^-|grep .bmp|wc -l";
                m_cmd.start("sh",QStringList()<<"-c"<<cmd);
                m_cmd.waitForFinished();
                bmpCnt = QLatin1String(m_cmd.readAll());
                bmpCnt.replace("\n","");

                sdSpace = g_Runtime().getUsbSpace("/dev/sd");
                if (sdSpace.size() > 5){
                    ui->textEdit->setText("SD: "+bmpCnt+"  "+sdSpace.at(2)+"/"+sdSpace.at(1));
                }
                else{
                    ui->textEdit->setText("SD: "+bmpCnt);
                }
                g_infoWidget().hide();
            }
            else{
                ui->textEdit->setText("no Img file");
            }
        }
        else{
            ui->textEdit->setText("no usb disk find!");
        }
    }
    else{
        ui->textEdit->setText("no usb device find!");
    }
}
