#ifndef SYSIPCBADPOINTSWIDGET_H
#define SYSIPCBADPOINTSWIDGET_H

#include "basewidget.h"
#include "messagedialog.h"
#include "mylineedit.h"
#include <QThread>

class getBadPointsThread;
namespace Ui {
class SysIpcBadPointsWidget;
}
class BadPointInfo{
public:
    QString strDate;
    QString modelName;
    int ezaValue;
    BadPointInfo(QString str1,QString str2,int val){
        strDate = str1;
        modelName = str2;
        ezaValue = val;
    }
};

enum
{
    MSG_ID_YEAR = 30,
    MSG_ID_MONTH = 31,
    MSG_ID_DAY = 32
};

class SysIpcBadPointsWidget : public basewidget
{
    Q_OBJECT

public:
    explicit SysIpcBadPointsWidget(QWidget *parent = 0);
    ~SysIpcBadPointsWidget();
    void showPage(bool bshow);
    void retranslateUiWidget();
    void receiveMsg(int msg1, int msg2, int msg3);

    QString startDateStr,endDateStr;
private slots:
    void on_search_clicked();
    void updateTableWidget();

    void on_pushButton_clicked();

private:
    Ui::SysIpcBadPointsWidget *ui;

    void updateVersionInfo();
    MessageDialog *m_pMessage;

    int m_dateTime[2][3];
    myLineEdit *m_dateTimeWidget[2][3];
    getBadPointsThread *getThread;
};

class getBadPointsThread : public QThread
{
    Q_OBJECT
public:
    getBadPointsThread() {
        m_pParentWidget = NULL;
    }

    QList<BadPointInfo> m_verinfoVec;
    QString sql;
    QString m_dateStr,m_modelNmae;
    int m_value;

    SysIpcBadPointsWidget *m_pParentWidget;

protected:
    void run();
signals:
    void sGetBadPointsFinished();

};

#endif // SYSIPCBADPOINTSWIDGET_H
