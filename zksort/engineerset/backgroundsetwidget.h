#ifndef BACKGROUNDSETWIDGET_H
#define BACKGROUNDSETWIDGET_H

#include "basewidget.h"

namespace Ui {
class BackGroundSetWidget;
}

class BackGroundSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit BackGroundSetWidget(QWidget *parent = 0);
    ~BackGroundSetWidget();
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
    void on_comboBox_activated(int index);

    void on_checkBox_bias_clicked(bool checked);

    void on_checkBox_cali_clicked(bool checked);

    void on_TDSwitchBtn_clicked();

    void getscreenshot();

private:
    Ui::BackGroundSetWidget *ui;
    void updateView();
    QLabel *m_labelAvg[4];

    void updateAvgVisiable();
    void updateAvgValue();
    void updateUpdateCaliState();
    void updateTDSwitchBtnStat();

              QTimer *shottimer;
signals:
   void addBtnClicked();
};

#endif // BACKGROUNDSETWIDGET_H
