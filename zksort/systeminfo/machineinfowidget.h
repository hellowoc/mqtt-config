#ifndef MACHINEINFOWIDGET_H
#define MACHINEINFOWIDGET_H

#include "basewidget.h"
#include <QTimer>

namespace Ui {
class MachineInfoWidget;
}

class MachineInfoWidget : public basewidget
{
    Q_OBJECT

public:
    explicit MachineInfoWidget(QWidget *parent = 0);
    ~MachineInfoWidget();
    void showPage(bool bshow);
    void retranslateUiWidget();
private slots:
    void TimeOutSlt();
    void updateInfo();

    void on_pushButton_clicked();

private:
    Ui::MachineInfoWidget *ui;
    QTimer *myTimer;

    QLabel  *feedLevelLabel[CTRL_ALARM_MAX_FEED_SENSOR];
    QLabel  *tempSensorLabel[CTRL_ALARM_MAX_TEMP_SENSOR];
};

#endif // MACHINEINFOWIDGET_H
