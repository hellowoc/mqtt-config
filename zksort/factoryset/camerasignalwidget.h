#ifndef CAMERASIGNALWIDGET_H
#define CAMERASIGNALWIDGET_H

#include "basewidget.h"
#include "mynetwork.h"
#include "mylineedit.h"
namespace Ui {
class CameraSignalWidget;
}

class CameraSignalWidget : public basewidget
{
    Q_OBJECT

public:
    explicit CameraSignalWidget(QWidget *parent = 0);
    ~CameraSignalWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
    void OnViewChange(int view);
    void OnChuteChange();
    void OnLayerChange();
protected:
    void retranslateUiWidget();
   public slots:
    void updateWave();
private slots:
    void on_darkSaveBtn_clicked();

    void on_checkBox_origin_clicked(bool checked);

    void on_checkBox_cali_clicked(bool checked);

    void on_brightCaliBtn_clicked();

    void on_darkCaliBtn_clicked();

    void on_pixelAdjust_clicked();

    void on_brightCaliSaveBtn_clicked();

    void on_pixelBad_clicked();

    void on_colorRestore_clicked();

    void on_brightCaliOneKeyBtn_clicked();

    void on_darkCaliOneKeyBtn_clicked();

    void on_TDSwitchBtn_clicked();

        void getscreenshot();

        void on_pushButton_9_clicked();

private:
    Ui::CameraSignalWidget *ui;
    int m_EditValueInt[3][4];
    myLineEdit *m_edit[3][4];
    QLabel *m_label[4];
    QLabel *m_labelAvg[4];
    QLabel *m_labelFocus;
    QLabel *m_labelFocus2;

    void updateSetValue();
    void updateAvgValue();
    void updateUpdateCaliState();
    void updateEditVisible();
    void updateAnaGainLockBtnStatus();
    void updateTDSwitchBtnStat();
    QTimer *shottimer;
    QStringList tdModeList;
    int bCurCal;

signals:
   void addBtnClicked();
//    void updateCaliTypeBtnState();
};

#endif // CAMERASIGNALWIDGET_H
