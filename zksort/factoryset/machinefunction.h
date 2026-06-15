#ifndef MACHINEFUNCTION_H
#define MACHINEFUNCTION_H
#include "basewidget.h"
#include <QButtonGroup>
#include <QSignalMapper>
#include "mylineedit.h"
namespace Ui {
class MachineFunction;
}


class MachineFunction : public basewidget
{
    Q_OBJECT

public:
    enum{
        machineModel,
        alarm,
        flow_set,
        recover_set,
        svm_set,
        param_set,
        acc,
        softfeed,
           btn_PLC,
        btn_valve,
        btn_filter,
        btn_wipe,
        btn_lamp,


        END
    };
    explicit MachineFunction(QWidget *parent = 0);
    ~MachineFunction();
    void showPage(bool bshow);
    void receiveMsg(int msg1, int msg2, int msg3);
    void GenerateQRcode(QString tempstr);

protected:
    virtual void retranslateUiWidget();

private slots:
    void btn_clicked(bool);

    void on_oldBtn_clicked(bool checked);

    void on_comboBox_activated(int index);

    void on_ageAll_clicked(bool checked);

    void updateAgeChute();

    void updateAgeAllState();

    void on_encryptBtn_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_checkBox_flow_clicked(bool checked);

    void on_pressureAlarmType_activated(int index);

    void on_resetModeName_clicked();

    void on_pushButton_clicked();

    void on_tempSensorBtn_clicked();

    void on_alarmVoiceTypeCombobox_activated(int index);

    void on_sensorNameRestoreBtn_clicked();

    void on_changelogoBtn_clicked();

    void on_cmboxPort_activated(int index);

    void on_emmcUgBtn_clicked();

    void on_emmcp1UgBtn_clicked();

    void on_ugCancelBtn_clicked();

    void on_identifyParamsSetByTimescheckBox_clicked(bool checked);

    void on_feederCtrlType1CheckBox_clicked(bool checked);

    void on_delayTimeSetByTimescheckBox_clicked(bool checked);

    void on_viceFeederCheckBox_clicked();

    void on_upsCheckBox_clicked();

    void on_sprayCheckbox_clicked(bool checked);

    void on_vibMode_activated(int index);

    void on_wheelRun_clicked(bool checked);

    void on_checkBox_clicked(bool checked);

    void on_crossMode1Checkbox_clicked(bool checked);

    void on_crossCheckbox_clicked(bool checked);

    void on_crossMode2Checkbox_clicked(bool checked);

    void on_vavleSite_clicked(bool checked);

    void on_autoImageCheckBox_clicked();

    void on_ipcEnableCheckBox_clicked();

    void on_ledit_onoffDelay_textChanged(const QString &arg1);

    void on_comboBox_valveType_activated(int index);

    void on_comboBox_vibFreq_activated(int index);

    void on_comboBox_vibFreq_2_activated(int index);

    void on_vibUseEnable_clicked(bool checked);

    void on_pushButton_2_clicked();

    void on_lvMachine_clicked(bool checked);

    void on_plcControl_clicked(bool checked);

    void on_moreBtn_clicked();

    void on_checkBoxChs_clicked();

    void on_dpMachine_clicked(bool checked);

    void on_allInOne_clicked(bool checked);

    void on_ledit_onoffDelay_2_textChanged(const QString &arg1);

    void on_valveDefend_clicked(bool checked);

    void on_alarmVoiceTestBtn_clicked();

    void on_screenSetCheckBox_clicked(bool checked);

    void on_bmMachine_clicked(bool checked);

    void on_d2Mode1Edit_textChanged(const QString &arg1);

    void on_d2Mode2Edit_textChanged(const QString &arg1);

    void on_d2Mode3Edit_textChanged(const QString &arg1);

    void on_d2Mode4Edit_textChanged(const QString &arg1);

    void on_lowcfg_sync_clicked(bool checked);

    void on_restoreBtn_clicked();

    void on_camParaInCnfCheckBox_clicked(bool checked);

    void on_autoApplyBtn_clicked();

    void on_balmodecomboBox_activated(int index);

    void on_sensortypecmx_activated(int index);

    void on_accelerationBtn_clicked();

    void on_alarmCmdTypeCombobox_activated(int index);

    void on_accCheckbox_clicked(bool checked);

    void on_updateWifiBtn_clicked();

    void on_pushButton_12_clicked();

    void on_comboBox_2_activated(int index);

    void on_comboBox_5_activated(int index);

    void on_AutoCtrlcheckBox_clicked(bool checked);
    void on_PowerCutcheckBox_clicked(bool checked);
    void on_DefaultPlccheckBox_clicked(bool checked);
    void on_lampcheckBox_clicked(bool checked);
    void on_screenShot_clicked();
    void on_connectButton_clicked();
    void on_disconnectButton_clicked();
    void on_serverPortEdit_textChanged(const QString &arg1);
    void on_imgSaturationEnable_clicked(bool checked);

    void on_crossModeCombox_activated(int index);

    void on_ledit_turnRatio_textChanged(const QString &arg1);

    void on_ledit_turnRatioTime_textChanged(const QString &arg1);

    void on_ledit_AreaThres_textChanged(const QString &arg1);
    void on_checkBox_2_clicked(bool checked);
    void updateLogoLight();
private:
    Ui::MachineFunction *ui;
    void updateoldBtnState();
    void updateViceFeedState();
    void updateAutoImageCaptureState();
    void updateUseIpcEnableState();
    void updateVavleTimeSetModeState();
    void updateArithParamsSetByTimes();
    void updateFeedCtrlTpye();
    void updateConnectName();
    void updatePressureAlarmTypeCombox();
    void updateFlowStaticsEnableState();
    void updateFlowStaticsInWorkingState();
    void updatePartsUsedInfo();
    void updatePortUsedCombox();
    void updateSpray();
    void updateVibType();
    void updateWheelType();
    void updateCrossCam();
    void updateCrossMode1();
    void updateCrossMode2();
    void updateValveSiteState();
    void updateValveType();
    void updateVibFreq();
    void updateVibFreq2();
    void updateBigVibPos();
    void updateBigVibEnable();
    void updateSVMPara();
    void updateAllInOne();
    void updateValveDefend();
    void updateScreenSet();
    void updateSGTimeoutSet();  

    void updateBalanceCtrl();
    void updateSelectModeSet();

    void updateConnectState(bool isConnected);

    int m_sTotalStaticsGetDuration;

    QString ipindex;

    QStringList presAlarmTypeList;
    QStringList alarmVoiceTypeList;
    QButtonGroup *m_pbtnGroup;

    stru_feed_auto_ctrl m_feedAuto;

    QLabel *lightid[MAX_LAMP_PER_BOARD];
    myLineEdit *lightmode[MAX_LAMP_PER_BOARD];
    myLineEdit *lightlength[MAX_LAMP_PER_BOARD];
};

#endif // MACHINEFUNCTION_H
