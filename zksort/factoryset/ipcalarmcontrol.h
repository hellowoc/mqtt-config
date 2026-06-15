#ifndef IPCALARMCONTROL_H
#define IPCALARMCONTROL_H

#include <basewidget.h>
#include "mylineedit.h"
#include <QLabel>

namespace Ui {
class ipcAlarmControlWidget;
}

class ipcAlarmControlWidget : public basewidget
{
    Q_OBJECT

public:
    explicit ipcAlarmControlWidget(QWidget *parent = 0);
    ~ipcAlarmControlWidget();

    void showPage(bool bshow);
    void receiveMsg(int msg1, int msg2, int msg3);


protected:
    virtual void retranslateUiWidget();

private slots:


    void on_alarmfeedoffCheckBox_clicked();

    void on_alarmfeedoffCheckBox_2_clicked();

    void on_pushButton_2_clicked();

    void on_watcher_filechanged(QString);

    void on_watcher_dirchanged(QString);

    void on_pushButton_clicked();

private:

    Ui::ipcAlarmControlWidget *ui;

    void updateView();
};

#endif // IPCALARMCONTROL_H
