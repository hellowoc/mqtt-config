#ifndef CAMERASETWIDGET_H
#define CAMERASETWIDGET_H

#include "basewidget.h"
#include <QTableWidget>
#include <QCheckBox>
namespace Ui {
class CameraSetWidget;
}
static int t1_sensor_freq[8] ={18000,9000,4500,3600,3214,3000,3000,3000};
static int d2_sensor_freq[8] ={0,1,2,3,4,5,6,7};
class CameraSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit CameraSetWidget(QWidget *parent = 0);
    ~CameraSetWidget();
    void retranslateUiWidget();
    void showPage(bool bshow);
    void OnViewChange(int view);
    void OnLayerChange();
    void receiveMsg(int msg1, int msg2, int msg3);
private slots:
    void viewBtnClicked(bool checked);
    void arithCheckboxClicked(bool checked);
    void on_chainAddrCombox_activated(int index);

    void on_CameraBoardModeComboBox_activated(int index);

    void on_SensorDimComboBox_activated(int index);

    void on_SensorDimComboBox_2_activated(int index);

    void on_SensBitWidthComboBox_activated(const QString &arg1);

//    void on_SensorRowFreComboBox_activated(const QString &arg1);

    void on_applyBtn_clicked();

    void on_SensorPixelcomboBox_activated(int index);

    void on_sensorType_activated(int index);

    void on_restoreBtn_clicked();

    void on_SensorRowFreComboBox_activated(int index);

    void on_SensorImageComboBox_activated(int index);

    void on_MachineTypeComboBox_activated(int index);

    void on_MirrorComboBox_activated(int index);

    void on_BrightSelectComboBox_activated(int index);

    void on_imgCompresscheckBox_clicked();

    void on_checkBox_B_clicked(bool checked);

    void on_checkBox_G_clicked(bool checked);

    void on_checkBox_R_clicked(bool checked);

    void on_checkBox_N_clicked(bool checked);

    void on_colorModeComboBox_activated(int index);

    void on_SensorCmdSpeedComboBox_activated(int index);

    void on_ResizeModeComboBox_activated(int index);

    void on_correctionCheckBox_clicked(bool checked);

    void getscreenshot();


    void on_TDscheme_activated(int index);

    void on_UserMode_activated(int index);

    void on_inhanceCheckBox_clicked(bool checked);
private:
    Ui::CameraSetWidget *ui;
    QTableWidget *m_pTableWidget;

    QCheckBox *m_viewCheckBox[MAX_VIEW];
    QCheckBox *m_arithCheckBox[ARITHMETIC_TOTAL];
    int m_arithEnable[ARITHMETIC_TOTAL];
    void updateArithState();
    void updateCameraSet();
    void updateSensorFreCombox();
    int getCurrentFreqIndex();//获取当前帧索引，对应转速
    QStringList m_list1,m_list2,m_list3,m_list4,m_list5,m_list6,m_list_7;
    int m_frep;
    struCnfGlobal m_temCnfg;
    struCnfProfile m_temCnfp;
    QMap<int,int> m_arithIndexMap;

    int splice[MAX_VIEW];
    float freq;

              QTimer *shottimer;
};

#endif // CAMERASETWIDGET_H
