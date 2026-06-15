#ifndef IRBADPOINTSETWIDGET_H
#define IRBADPOINTSETWIDGET_H

#include "basewidget.h"
#include "mynetwork.h"
#include "mylineedit.h"

namespace Ui {
class IrBadPointSetWidget;
}

class IrBadPointSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit IrBadPointSetWidget(QWidget *parent = 0);
    ~IrBadPointSetWidget();
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
    void on_pushButton_5_clicked();

    void on_checkBox_green_clicked(bool checked);

    void on_checkBox_red_clicked(bool checked);

//    void on_checkBox_tdi_clicked(bool checked);

    void on_checkBox_enable_updown_clicked(bool checked);

    void on_pushButton_8_clicked();

    void on_checkBox_cali_clicked(bool checked);
    void on_checkBox_origin_clicked(bool checked);

    void on_checkBox_correct_clicked(bool checked);

    void on_checkBox_tdi_clicked();

    void on_checkBox_blue_clicked(bool checked);

    void on_checkBox_red_2_clicked(bool checked);

    void on_checkBox_green_2_clicked(bool checked);

    void on_checkBox_blue_2_clicked(bool checked);

    void on_checkBox_n_clicked(bool checked);

    void on_checkBox_n_2_clicked(bool checked);

private:
    void updateUpdateCaliState();
    void updateValue();
    void updateCheckBox();
    void updateUpDown();//更新TDI上下当前是上还是下
    void updateRGB();//显示坏点对应的RGB值
    Ui::IrBadPointSetWidget *ui;
};
#endif // IRBADPOINTSETWIDGET_H
