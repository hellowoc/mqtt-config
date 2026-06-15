#ifndef FEEDERAOTUAJUST_H
#define FEEDERAOTUAJUST_H

#include "basewidget.h"

namespace Ui {
class FeederAotuAjust;
}

class FeederAotuAjust : public basewidget
{
    Q_OBJECT

public:
    explicit FeederAotuAjust(QWidget *parent = 0);
    ~FeederAotuAjust();
    virtual void showPage(bool bshow);
    void OnGroupChange(int groupIndex);
    void OnLayerChange();
protected:
    virtual void retranslateUiWidget();
private slots:
    void on_checkBox_enable_clicked(bool checked);

    void on_pushButton_clicked();

    void on_startBtn_clicked();

    void on_pushButton_2_clicked();

    void on_checkBox_quick_balance_clicked();

    void on_chuteBalSetBtn_clicked();

    void on_singleaimcheckBox_clicked();

private:
    Ui::FeederAotuAjust *ui;
    void updateWidget();

    int feedPixelCaliDuration;          //每KG大米对应像素点标定周期,单位:秒,最大110秒
    int totalRicePixels;                //标定大米重量时获取的像素点总数
    float m_quality;
    float m_quality_2;                  //理论标定产量(客户设置)
    bool m_fixedFlag;
};

#endif // FEEDERAOTUAJUST_H
