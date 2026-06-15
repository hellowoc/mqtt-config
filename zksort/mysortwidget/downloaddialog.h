#ifndef DOWNLOADDIALOG_H
#define DOWNLOADDIALOG_H

#include <QDialog>
#include "myhttpfileclient.h"

#define UNIT_KB 1024            //KB
#define UNIT_MB 1024*1024       //MB
#define UNIT_GB 1024*1024*1024  //GB

#define TIME_INTERVAL 300       //0.3s

namespace Ui {
class DownloadDialog;
}

class DownloadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DownloadDialog(QWidget *parent = 0);
    ~DownloadDialog();
    void init();
    void setDownloadInfo(QVariant m_unit);
    QString transformUnit(qint64 bytes , bool isSpeed);
    QString transformTime(qint64 seconds);

private slots:
    void onDownloadProcess(qint64, qint64);
    void onReplyFinished(int);
    void onDownloadError();

    void on_startBtn_clicked();
    void on_stopBtn_clicked();
    void on_closeBtn_clicked();

private:
    Ui::DownloadDialog *ui;
    httpDownloadManager *mydownloadManager;
    QVariant unit;
    UpdateUnit u_unit;
    qint64 m_intervalDownload,m_currentDownload,m_timeInterval;
    QTime m_timeRecord;
};

#endif // DOWNLOADDIALOG_H
