#ifndef UPDATEACSWIDGET_H
#define UPDATEACSWIDGET_H

#include "basewidget.h"
#include "logger.h"
class mylistwidget;
class UpdateAcsWidget;
class AcsUpdateThread;

namespace Ui {
class UpdateAcsWidget;
}

class AcsUpdateThread : public QThread
{
    Q_OBJECT
public:
    explicit AcsUpdateThread(QThread *parent = 0);
    ~AcsUpdateThread();

protected:
     void run(void);
signals:
     void sAcsUpdateFinished();
public:
     UpdateAcsWidget *m_parentWidget;
};

class UpdateAcsWidget : public basewidget
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    explicit UpdateAcsWidget(QWidget *parent = 0);
    ~UpdateAcsWidget();
    void showPage(bool bshow);
    void changeValue(mylistwidget *src, mylistwidget *dst);
    void goIntoFolder(mylistwidget *src);
    void goBackFolder(mylistwidget *src);
    void updateIpcCount();
    bool checkIpcMounted(int ipcindex);
    bool unMountIpc(int ipcindex);
protected:
    void retranslateUiWidget();
private slots:
    void on_checkBox_clicked(bool checked);

    void on_refreshBtn_clicked();

    void on_copyL2RBtn_clicked();

    void on_copyR2LBtn_clicked();

    void on_ipcSelectCombox_currentIndexChanged(int index);

    void on_connectBtn_clicked();

    void updateAll();

    void on_armBackBtn_clicked();

    void on_armGoBtn_clicked();

    void on_acsBackBtn_clicked();

    void on_acsGoBtn_clicked();

    void on_acsDelBtn_clicked();

    void on_acsReadBtn_clicked();

    void on_acsSelectAllBtn_clicked(bool clicked);

    void on_armSelectAllBtn_clicked(bool checked);

    void on_disconnectBtn_clicked();

public:
    bool iscut;
    int m_curIpcIndex;
    QString srcPath,distPath;
    QStringList srcPathList,distPathList;
    AcsUpdateThread *acsUpdateThread;
    QString localDir, m_curIpcIpAddr ;
private:
    Ui::UpdateAcsWidget *ui;

};

#endif // UPDATEACSWIDGET_H
