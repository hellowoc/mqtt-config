#ifndef GRAYSENSESETWIDGET_H
#define GRAYSENSESETWIDGET_H

#include "basewidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include "mylineedit.h"
#include <QButtonGroup>
namespace Ui {
class GraySenseSetWidget;
}

class GraySenseSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit GraySenseSetWidget(QWidget *parent = 0);
    ~GraySenseSetWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);


protected:
    void retranslateUiWidget();

private:
    Ui::GraySenseSetWidget *ui;
    void updateValue();
    void updateChuteState();
    void updateEdgeBtn();
    void updateinfectBtn();
    float m_senseValue[2][MAX_VIEW_UNIT_USE];
    myLineEdit *m_pChuteSenseEdit[2][MAX_VIEW_UNIT_USE];
    float m_ChuteSensFloat[2][MAX_VIEW_UNIT_USE];
    QLabel *m_pChuteTitle[MAX_VIEW_UNIT_USE];
    QButtonGroup *m_pModeBtnGroup;
    void setViewVisibleByArith();
    void onModeChange(int mode);

    void updatePercentViewState();
    void updateLineDefectState();
    void updateSizeState();
    int m_rowInt;//行
    int m_percentInt;//纯度
    float m_percentResInt;
    float m_diffResUpFloat;
    float m_diffResLowFloat;
    int m_arithEdge; //算法切边

    void updateModeBtnState();
public slots:
    private slots:
    void on_edgeBtn_clicked(bool checked);
    void on_infectBtn_clicked(bool checked);
    void on_SensUpdateCheckBox_clicked(bool checked);
    void on_comboBox_diff_activated(int index);
    void on_comboBox_gray_activated(int index);
    void modeBtnClicked(bool);
    void on_percentCheckBox_clicked(bool checked);
    void on_comboBox_activated(int index);
    void sNameChanged();
    void on_extendBtn_clicked(bool checked);
    void on_lineDefectCheckbox_clicked(bool checked);
    void on_comboBox_delay_activated(int index);
};

#endif // GRAYSENSESETWIDGET_H
