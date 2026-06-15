#include "downloaddialog.h"
#include "ui_downloaddialog.h"

DownloadDialog::DownloadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DownloadDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    mydownloadManager = 0;
    init();
    ui->closeBtn->setText(g_myLan().ids_close);
    ui->startBtn->setText(g_myLan().start);
    ui->stopBtn->setText(g_myLan().pause);
    ui->infoLbl->hide();
    ui->statusLbl->setText("");
    ui->labelSpeed->setText(g_myLan().Download_rate + ": 0 KB/S");
    ui->labelRemainTime->setText(g_myLan().Remaining_time + ": 0s");
    ui->labelCurrentDownload->setText( g_myLan().download + g_myLan().File_size + ": 0 B");
    ui->labelFileSize->setText(g_myLan().File_size + ": 0 B");
}

DownloadDialog::~DownloadDialog()
{
    delete ui;
    if (mydownloadManager != NULL)
    {
        disconnect(mydownloadManager , SIGNAL(sigDownloadProcess(qint64, qint64)), this, SLOT(onDownloadProcess(qint64, qint64)));
        disconnect(mydownloadManager, SIGNAL(sigDownloadFinished(int)), this, SLOT(onReplyFinished(int)));
        disconnect(mydownloadManager,SIGNAL(sigDownloadError()),this,SLOT(onDownloadError()));
    }
    mydownloadManager = 0;
}

void DownloadDialog::init()
{
    ui->progressBar->setValue(0);
    ui->progressBar->setStyleSheet("\
                                   QProgressBar\
    {\
                                       border-width: 0 12 0 12;\
                                       border-left: 1px, gray;\
                                       border-right: 1px, gray;\
                                       border-image:url(:/white/ok_nm.png);\
                                   }\
                                   QProgressBar::chunk\
    {\
                                       border-width: 0 12 0 12;\
                                       border-image:url(:/white/ok_press.png);\
                                   }");
                                   ui->progressBar->setAlignment(Qt::AlignRight | Qt::AlignVCenter);  // 对齐方式
}

void DownloadDialog::setDownloadInfo(QVariant m_unit)
{
    unit = m_unit;
    u_unit = QVariant(m_unit).value<UpdateUnit>();
}
// 开始下载;
void DownloadDialog::on_startBtn_clicked()
{
    if (mydownloadManager == NULL)
    {
        mydownloadManager = new httpDownloadManager(this);
        connect(mydownloadManager , SIGNAL(sigDownloadProcess(qint64, qint64)), this, SLOT(onDownloadProcess(qint64, qint64)));
        connect(mydownloadManager, SIGNAL(sigDownloadFinished(int)), this, SLOT(onReplyFinished(int)));
        connect(mydownloadManager,SIGNAL(sigDownloadError()),this,SLOT(onDownloadError()));
    }

    mydownloadManager->setDownloadInfo(unit);
    mydownloadManager->startDownload();
    m_timeRecord.start();
    m_timeInterval = 0;
    ui->statusLbl->setText(g_myLan().downloading);
}

// 暂停下载;
void DownloadDialog::on_stopBtn_clicked()
{
    ui->statusLbl->setText(g_myLan().download_pause);
    if (mydownloadManager != NULL)
    {
        mydownloadManager->stopDownload();
    }
    ui->labelSpeed->setText(g_myLan().Download_rate +": 0 KB/S");
    ui->labelRemainTime->setText(g_myLan().Remaining_time +": 0s");
}

// 关闭下载;
void DownloadDialog::on_closeBtn_clicked()
{
    if(mydownloadManager != NULL)
        mydownloadManager->closeDownload();
    ui->progressBar->setValue(0);
    ui->labelSpeed->setText(g_myLan().Download_rate +": 0 KB/S");
    ui->labelRemainTime->setText(g_myLan().Remaining_time +": 0s");
    ui->statusLbl->setText(g_myLan().download_stop);
#ifdef Q_OS_UNIX
//    myMqttMsgParaseThread->onCmdReturn(u_unit.m_uuid,u_unit.m_cmd,USERREJECT);
#endif
    reject();

    ui->labelCurrentDownload->setText(g_myLan().download + g_myLan().File_size + ": 0 B");
    ui->labelFileSize->setText(g_myLan().File_size + ": 0 B");
}

// 更新下载进度;
void DownloadDialog::onDownloadProcess(qint64 bytesReceived, qint64 bytesTotal)
{
    // 输出当前下载进度;
    //    qDebug() << QString("%1").arg(bytesReceived * 100 / bytesTotal + 1);
    // 更新进度条;
    ui->progressBar->setMaximum(bytesTotal);
    ui->progressBar->setValue(bytesReceived);
#if 1
    // m_intervalDownload 为下次计算速度之前的下载字节数;
    m_intervalDownload += bytesReceived - m_currentDownload;
    m_currentDownload = bytesReceived;

    uint timeNow = m_timeRecord.elapsed();

    // 超过0.3s更新计算一次速度;
    if (timeNow - m_timeInterval > TIME_INTERVAL)
    {
        qint64 ispeed = m_intervalDownload * 1000 / (timeNow - m_timeInterval);
        QString strSpeed = transformUnit(ispeed, true);
        ui->labelSpeed->setText(g_myLan().Download_rate +": "+strSpeed);
        // 剩余时间;
        qint64 timeRemain = (bytesTotal - bytesReceived) / ispeed;
        ui->labelRemainTime->setText(g_myLan().Remaining_time +": "+transformTime(timeRemain));

        ui->labelCurrentDownload->setText(g_myLan().download + g_myLan().File_size+transformUnit(m_currentDownload,false));
        ui->labelFileSize->setText(g_myLan().File_size +": "+transformUnit(bytesTotal,false));

        m_intervalDownload = 0;
        m_timeInterval = timeNow;
    }
#endif
}

// 下载完成;
void DownloadDialog::onReplyFinished(int res)
{
#ifdef Q_OS_UNIX
    if(struGsh.selectDownloadModel == 0)
    {
//        myMqttMsgParaseThread->onCmdReturn(u_unit.m_uuid,u_unit.m_cmd,res);
    }
    accept();
    g_infoWidget().setLabelText(g_myLan().download_ok);
    g_infoWidget().showSecs();
    if(u_unit.force == 1 && res == SUCCESS)
    {
        QTimer timer;
        timer.setSingleShot(true);

        myMessageBox msgBox(MSG_QUES, g_myLan().accept_remote_upgrade);
        QObject::connect(&timer, SIGNAL(timeout()), &msgBox, SLOT(hide()));
        timer.start(10000);
        int ret = msgBox.exec();
        if (timer.isActive())
        {
            timer.stop();
            if (ret == QDialog::Accepted){ //用户操作，同意进行

                if(struGsh.selectDownloadModel == 1)
                {
                    g_MainManager().ShowWidget(SM_IPC_UPGRADE_MODEL_WIDGET);
                    return;
                }
                system("rm /sdcard/udisk/* -fr");
                system("sync");
                g_infoWidget().setLabelText(g_myLan().ids_apply);
                g_infoWidget().delayShow();
                g_Runtime().dirExist("/sdcard/udisk");
                system("unzip -q -o /sdcard/tmp.zip -d /sdcard/udisk");
                system("sync");
                g_infoWidget().hide();
                QString size = g_Runtime().mySystemStr("cd /sdcard/udisk/;ls -lh| awk '{print $5}'");
                QString name = g_Runtime().mySystemStr("cd /sdcard/udisk/;ls");
                if(size == "48.0M" || name.contains(".bin"))
                {
                    QSettings setting(DEBUG_MODE_Set,QSettings::IniFormat);
                    setting.setValue("debugmode",1);
                    setting.sync();
                    struGsh.poweroff_status = 1;
                    setting.setValue("poweroffstatus",struGsh.poweroff_status);
                    setting.sync();
                    this->close();
                    myFlow.msleep(100);
                    QProcess::startDetached("/app/MyUpdate.sh");
                    qApp->quit();
                }
                else
                {
                    g_MainManager().ShowWidget(SM_IPC_UPGRADE_MODEL_WIDGET);
                }
            }
        }
        else{ //超时用户未操作，直接进行
//            myMqttMsgParaseThread->onCmdReturn(u_unit.m_uuid,u_unit.m_cmd,USERREJECT);
        }
    }
#endif
}
void DownloadDialog::onDownloadError()
{
#ifdef Q_OS_UNIX
//    myMqttMsgParaseThread->onCmdReturn(u_unit.m_uuid,u_unit.m_cmd,DOWNLOADERROR);
#endif
    g_infoWidget().setLabelText(g_myLan().download_fail);
    g_infoWidget().showSecs(2);

    reject();
}
#if 1
// 转换单位;
QString DownloadDialog::transformUnit(qint64 bytes , bool isSpeed)
{
    QString strUnit = " B";
    if (bytes <= 0)
    {
        bytes = 0;
    }
    else if (bytes < UNIT_KB)
    {
    }
    else if (bytes < UNIT_MB)
    {
        bytes /= UNIT_KB;
        strUnit = " KB";
    }
    else if (bytes < UNIT_GB)
    {
        bytes /= UNIT_MB;
        strUnit = " MB";
    }
    else if (bytes > UNIT_GB)
    {
        bytes /= UNIT_GB;
        strUnit = " GB";
    }

    if (isSpeed)
    {
        strUnit += "/S";
    }
    return QString("%1%2").arg(bytes).arg(strUnit);
}

// 转换时间;
QString DownloadDialog::transformTime(qint64 seconds)
{
    QString strValue;
    QString strSpacing(" ");
    if (seconds <= 0)
    {
        strValue = QString("%1s").arg(0);
    }
    else if (seconds < 60)
    {
        strValue = QString("%1s").arg(seconds);
    }
    else if (seconds < 60 * 60)
    {
        int nMinute = seconds / 60;
        int nSecond = seconds - nMinute * 60;

        strValue = QString("%1m").arg(nMinute);

        if (nSecond > 0)
            strValue += strSpacing + QString("%1s").arg(nSecond);
    }
    else if (seconds < 60 * 60 * 24)
    {
        int nHour = seconds / (60 * 60);
        int nMinute = (seconds - nHour * 60 * 60) / 60;
        int nSecond = seconds - nHour * 60 * 60 - nMinute * 60;

        strValue = QString("%1h").arg(nHour);

        if (nMinute > 0)
            strValue += strSpacing + QString("%1m").arg(nMinute);

        if (nSecond > 0)
            strValue += strSpacing + QString("%1s").arg(nSecond);
    }
    else
    {
        int nDay = seconds / (60 * 60 * 24);
        int nHour = (seconds - nDay * 60 * 60 * 24) / (60 * 60);
        int nMinute = (seconds - nDay * 60 * 60 * 24 - nHour * 60 * 60) / 60;
        int nSecond = seconds - nDay * 60 * 60 * 24 - nHour * 60 * 60 - nMinute * 60;

        strValue = QString("%1d").arg(nDay);

        if (nHour > 0)
            strValue += strSpacing + QString("%1h").arg(nHour);

        if (nMinute > 0)
            strValue += strSpacing + QString("%1m").arg(nMinute);

        if (nSecond > 0)
            strValue += strSpacing + QString("%1s").arg(nSecond);
    }

    return strValue;
}
#endif

