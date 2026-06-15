#ifndef SYSINFONET_H
#define SYSINFONET_H

#include "basewidget.h"
#include <QTimer>

namespace Ui {
class SysInfoNet;
}
#define ACTIVE_IP_PATH "/sdcard/ts/active_ip.txt"

class SysInfoNet : public basewidget
{
    Q_OBJECT

public:
    explicit SysInfoNet(QWidget *parent = 0);
    ~SysInfoNet();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
protected:
    void retranslateUiWidget();

private slots:
    void on_checkBtn_clicked();
    void updateTextEdit();

private:
    Ui::SysInfoNet *ui;
    int m_start,m_end;
    QTimer *timer;
};

#endif // SYSINFONET_H
