#ifndef REMOTECONTROLWIDGET_H
#define REMOTECONTROLWIDGET_H

#include "basewidget.h"
#include "logger.h"


#ifdef Q_OS_UNIX
namespace Ui {
class RemoteControlWidget;
}

class RemoteControlWidget : public basewidget
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    explicit RemoteControlWidget(QWidget *parent = 0);
    ~RemoteControlWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);


    QTimer timer_start_1;
    QTimer timer_start_2;

protected:
    void retranslateUiWidget();

private slots:


    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void autolockscreem();

    void on_pushButton_2_clicked();

    void dealendremotecontrol();

private:
    Ui::RemoteControlWidget *ui;

};

#endif
#endif // UPDATEFPGAWIDGET_H
