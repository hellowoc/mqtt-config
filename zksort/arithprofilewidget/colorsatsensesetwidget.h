#ifndef COLORSATSENSESETWIDGET_H
#define COLORSATSENSESETWIDGET_H

#include "basewidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include "mylineedit.h"
#include <QButtonGroup>

namespace Ui {
class ColorSatSenseSetWidget;
}

class ColorSatSenseSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit ColorSatSenseSetWidget(QWidget *parent = 0);
    ~ColorSatSenseSetWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);

private:
    Ui::ColorSatSenseSetWidget *ui;
    void updateValue();
    void updateChuteState();
    void updateEdgeBtn();
    void updateinfectBtn();
    float m_senseValue[2][MAX_VIEW_UNIT_USE];
    myLineEdit *m_pChuteSenseEdit[2][MAX_VIEW_UNIT_USE];
    float m_ChuteSensFloat[2][MAX_VIEW_UNIT_USE];
    QLabel *m_pChuteTitle[MAX_VIEW_UNIT_USE];
    QButtonGroup *m_pResBtnGroup;
    QButtonGroup *m_pModeBtnGroup;
    QButtonGroup *m_pModeBtnGroupColorRes;
    QButtonGroup *m_pModeBtnGroupDiffRes;

    void setViewVisibleByArith();
    void onModeChange(int mode);

    void updatePercentViewState();
    void updateLineDefectState();
    void updateSizeState();
    int m_rowInt;//行
    int m_percentInt;//纯度
    int m_lightLimit;//亮度下限
    float m_percentResInt;
    int m_diffResSensInt;
    int m_grayResSensInt;

    void updateModeBtnState();

protected:
    void retranslateUiWidget();

public slots:
    void resRadioBtnClicked(bool checked);
    private slots:
    void on_edgeBtn_clicked(bool checked);
    void on_infectBtn_clicked(bool checked);
    void on_SensUpdateCheckBox_clicked(bool checked);
    void on_comboBox_diff_activated(int index);
    void on_comboBox_gray_activated(int index);
    void modeBtnClicked(bool);
    void modeBtnRes1Clicked(bool);
    void modeBtnRes2Clicked(bool);
    void on_percentCheckBox_clicked(bool checked);
    void on_comboBox_activated(int index);
    void sNameChanged();
    void on_pushButton_3_clicked(bool checked);
    void on_lineDefectCheckbox_clicked(bool checked);
};


#endif // COLORSATSENSESETWIDGET_H
