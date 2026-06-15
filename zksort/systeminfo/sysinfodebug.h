#ifndef SYSINFODEBUG_H
#define SYSINFODEBUG_H

#include "basewidget.h"
#include "mylineedit.h"

namespace Ui {
class SysInfoDebug;
}
enum CMD{
    ARM,
    RK
};

class SysInfoDebug;
class SysInfoDebugThread : public QThread
{
public:
    SysInfoDebugThread(SysInfoDebug* );
    void run();
    QString strInfo;
private:
    SysInfoDebug *m_parent;
};

class SysInfoDebug : public basewidget
{
    Q_OBJECT
public:
    explicit SysInfoDebug(QWidget *parent = 0);
    ~SysInfoDebug();

    QString cmd;
    SysInfoDebugThread *m_pThread;
    CMD cmdType;

protected:
    void retranslateUiWidget();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void debugFinished();

private:
    Ui::SysInfoDebug *ui;
    QString getZkTime(int type);
};

#endif // SYSINFODEBUG_H
