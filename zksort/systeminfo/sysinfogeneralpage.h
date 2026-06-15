#ifndef SYSINFOGENERALPAGE_H
#define SYSINFOGENERALPAGE_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

#include "globalflow.h"
#include "globalparams.h"
#include <QTimer>

class SysInfoGeneralPage : public QWidget
{
    Q_OBJECT
public:
    explicit SysInfoGeneralPage(QWidget *parent = 0);

    void CreateGeneralPage();
    void updateGeneralPage();
    void updateDateTime();

signals:

public slots:
    void TimeOutSlt();

private:
        // pageGeneral

        int       data[6];

        QLabel    *thisRunLabel;
        QLabel    *totalRunLabel;
        QLabel    *currentTime;
        QLabel    *feedLabel;
        QLabel    *ejectLabel;
        QLabel    *blowerLabel;
        QLabel    *lampLabel;
        QLabel    *pressLabel;
        QLabel    *beltStatus[3];
        QVBoxLayout *generalVBLayout;
        QSpacerItem *verticalSpacer_1;

        QTimer *timer;
};

#endif // SYSINFOGENERALPAGE_H
