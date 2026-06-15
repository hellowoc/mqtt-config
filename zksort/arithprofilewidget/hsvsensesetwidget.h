#ifndef HSVSENSESETWIDGET_H
#define HSVSENSESETWIDGET_H

#include "basewidget.h"
#include "mylineedit.h"
#include <QButtonGroup>
namespace Ui {
class HsvSenseSetWidget;
}

class HsvSenseSetWidget : public basewidget
{
    Q_OBJECT
    enum{
        HueLow,
        HueUp,
        SatLow,
        SatUp,
        ValLow,
        ValUp
    };
public:
    explicit HsvSenseSetWidget(QWidget *parent = 0);
    ~HsvSenseSetWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
protected:
    void retranslateUiWidget();
private:
    Ui::HsvSenseSetWidget *ui;
    void updateValue();
    void updateHsvEdit();
    void updateHsvValue();
    void updateChuteState();
    float m_senseValue[2][MAX_VIEW_UNIT_USE];
    myLineEdit *m_pChuteSenseEdit[2][MAX_VIEW_UNIT_USE];
    float m_ChuteSensFloat[2][MAX_VIEW_UNIT_USE];
    QLabel *m_pChuteTitle[MAX_VIEW_UNIT_USE];
    QButtonGroup *m_pResBtnGroup;
    QButtonGroup *m_pModeBtnGroup;
    void setViewVisibleByArith();
    void onModeChange(int mode);

    void updateSizeState();
    int m_rowInt;//行
    int m_percentInt;//纯度
    float m_percentResInt;
    int m_diffResSensInt;
    float m_diffResSensFloat;
    int m_grayResSensInt;
    float m_diffResUpFloat;
    float m_diffResLowFloat;

    float m_HsvSensLowH;
    float m_HsvSensUpH;

    void updateModeBtnState();
    private slots:
    void modeBtnClicked(bool);
    void on_comboBox_activated(int index);
    void sNameChanged();
    void hsvColorChanged();
    void on_comboBox_2_activated(int index);
    void on_comboBox_ratio_activated(int index);
    void on_comboBox_delay_activated(int index);
    void on_comboBox_gray_activated(int index);
    void on_comboBox_diff_activated(int index);
    void on_extendBtn_clicked(bool checked);
};

#endif // HSVSENSESETWIDGET_H
