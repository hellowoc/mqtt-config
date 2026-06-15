#ifndef SYSINFOLOG_H
#define SYSINFOLOG_H

#include "basewidget.h"
#include <QSignalMapper>
#include "mylineedit.h"

#define DEBUG_INFO "/sdcard/cnf/.log"

namespace Ui {
class SysInfoLog;
}

class SysInfoLog : public basewidget
{
    Q_OBJECT
enum
{
    MSG_ID_YEAR = 30,
    MSG_ID_MONTH = 31,
    MSG_ID_DAY = 32
};

public:
    explicit SysInfoLog(QWidget *parent = 0);
    ~SysInfoLog();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
protected:
    void retranslateUiWidget();

public slots:
    void updateLogInfo();
//    void setTimeInfo();

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::SysInfoLog *ui;

//    myLineEdit *m_pDateTimeEdit[3];
    int m_dateTime[3];

    QSignalMapper *timeSignalMapper;
};

#endif // SYSINFOLOG_H
