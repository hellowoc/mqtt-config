#ifndef ACCELERATIONSENSORSETWIDGET_H
#define ACCELERATIONSENSORSETWIDGET_H

#include "basewidget.h"
#include "mylineedit.h"
#include <QCheckBox>
namespace Ui {
class AccelerationSensorSetWidget;
}

class AccelerationSensorSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit AccelerationSensorSetWidget(QWidget *parent = 0);
    ~AccelerationSensorSetWidget();

    void showPage(bool bshow);
    void retranslateUiWidget();
    void receiveMsg(int msg1, int msg2, int msg3);

private slots:
    void on_pushButton_clicked();

    void on_feederSelectionCbx_activated(int index);

    void on_feederSelectionCbx_2_activated(int index);

private:
    Ui::AccelerationSensorSetWidget *ui;

    myLineEdit *pAmplitudeLdt[MAX_ACCELERATION];
    myLineEdit *pDutyCycleLdt[MAX_ACCELERATION];
    myLineEdit *pDutyCycleChangedLdt[MAX_ACCELERATION];
    QCheckBox  *pAccelerationEnable[MAX_ACCELERATION];
    QSignalMapper *pAccelerationEnableSigMapper;
    QSignalMapper *pAccelerationEnableSigMapper2;
    char cmdData[SEND_PACKET_DATA_SIZE];
    char cmdEnableData[SEND_PACKET_DATA_SIZE];
    char cmdEnableData2[SEND_PACKET_DATA_SIZE];

    myLineEdit *pAmplitudeLdt2[MAX_ACCELERATION];
    myLineEdit *pDutyCycleLdt2[MAX_ACCELERATION];
    myLineEdit *pDutyCycleChangedLdt2[MAX_ACCELERATION];
    QCheckBox  *pAccelerationEnable2[MAX_ACCELERATION];

    QLabel *pAccelerationState[MAX_ACCELERATION];
    QLabel *pAccelerationState2[MAX_ACCELERATION];

    QLabel *pAmplitudeLbl[MAX_ACCELERATION];
    QLabel *pAmplitudeLbl2[MAX_ACCELERATION];

    QLabel *pDutyCycleLbl[MAX_ACCELERATION];
    QLabel *pDutyCycleLbl2[MAX_ACCELERATION];

    QLabel *pDutyCycleChangedLbl[MAX_ACCELERATION];
    QLabel *pDutyCycleChangedLbl2[MAX_ACCELERATION];

    QButtonGroup *m_pbtnGroup;

public:
    void updateAccelerationEnable(int index);
    void updateAccelerationData(int index);
    void updateAccelerationTolerance();

    void updateAcceleration2Enable(int index);

    void updateFeederSelection();

public slots:
    void accelerationEnableClicked(int index);
    void accelerationEnable2Clicked(int index);

    void btn_clicked(bool);
};

#endif // ACCELERATIONSENSORSETWIDGET_H
