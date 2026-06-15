#ifndef PLCAUTOCTRLMANAGER_H
#define PLCAUTOCTRLMANAGER_H

#include <QObject>
#include "logger.h"
#include "global.h"

class PlcAutoCtrlManager : public QObject
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    explicit PlcAutoCtrlManager(QObject *parent = 0);
    ~PlcAutoCtrlManager();

private slots:

    void shutdownSlt();

    void feedOnOffSlt(int flag);

    void sendStatusSlt(int power);

    void sendMachineInfoSlt();

    void FeederMainValueSlt();

    void FeederViceValueSlt();

    void onValveClear();
private:

//    int remoteCtrlIsRunning;
    QTimer *timer;

};

extern PlcAutoCtrlManager *plcautoctrl;
#endif // PLCAUTOCTRLMANAGER_H

