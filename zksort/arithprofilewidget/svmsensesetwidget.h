#ifndef SVMSENSESETWIDGET_H
#define SVMSENSESETWIDGET_H

#include "basewidget.h"
#include "mylineedit.h"
#include <QLabel>
namespace Ui {
class SvmSenseSetWidget;
}

class SvmSenseSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit SvmSenseSetWidget(QWidget *parent = 0);
    ~SvmSenseSetWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
protected:
    void retranslateUiWidget();
private:
    Ui::SvmSenseSetWidget *ui;
    void updateChuteState();
    void updateEdgeBtn();
    void updateImpureBtn();
    QButtonGroup *m_pModeBtnGroup;
    QButtonGroup *m_pSvmTypeBtn;
    myLineEdit *m_pChuteSenseEdit[2][MAX_VIEW_UNIT_USE];
    float m_ChuteSensFloat[2][MAX_VIEW_UNIT_USE];
    float m_nPropotionSens[2];
    int m_nPropotionParaSensLow;
    int m_nEdge;
    QLabel *m_pChuteTitle[MAX_VIEW_UNIT_USE];
    QVector<QHBoxLayout*> m_hlayout;
    QVector<myLineEdit*> m_chuteEdit;
    QVector<QLabel*> m_chuteLabel;

    int m_rowInt;//行
    int m_percentInt;//纯度
    void setViewVisibleByArith();
    void updatePropertionState();
    void updateModeBtnState();
    void updateSvmTypeBtnState();
    void updateLineDefectState();
    void updateSizeState();
private slots:
    void on_scaleGroupbox_clicked(bool checked);
    void sNameChanged();
    void on_svmTypeBtn_clicked(bool checked);
    void on_modeTypeBtn_clicked(bool checked);
    void on_lineDefectGroupbox_clicked(bool checked);
    void on_extendBtn_clicked(bool checked);
    void on_edgeBtn_clicked(bool checked);
    void on_impureBtn_clicked(bool checked);
    void on_percentCheckBox_clicked(bool checked);
    void on_lineDefectCheckbox_clicked(bool checked);
    void on_comboBox_delay_activated(int index);
    void updateCornChuteState();
    void on_svmBtn_clicked();
    void on_imgBtn_clicked();
};

#endif // SVMSENSESETWIDGET_H
