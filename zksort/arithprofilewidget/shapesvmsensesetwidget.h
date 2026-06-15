#ifndef SHAPESVMSENSESETWIDGET_H
#define SHAPESVMSENSESETWIDGET_H

#include "basewidget.h"
#include "mylineedit.h"
#include <QLabel>
namespace Ui {
class ShapeSvmSenseSetWidget;
}

class ShapeSvmSenseSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit ShapeSvmSenseSetWidget(QWidget *parent = 0);
    ~ShapeSvmSenseSetWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
protected:
    void retranslateUiWidget();
private:
    Ui::ShapeSvmSenseSetWidget *ui;
    void updateChuteState();
    QButtonGroup *m_pModeBtnGroup;
    myLineEdit *m_pChuteSenseEdit[2][MAX_VIEW_UNIT_USE];
    float m_ChuteSensFloat[2][MAX_VIEW_UNIT_USE];
    float m_nPropotionSens[2];
    int m_nPropotionParaSensLow;
    int m_nEdge;
    QLabel *m_pChuteTitle[MAX_VIEW_UNIT_USE];
    QVector<QHBoxLayout*> m_hlayout;
    QVector<myLineEdit*> m_chuteEdit;
    QVector<QLabel*> m_chuteLabel;

    int m_areaLow,m_areaUp;
    void setViewVisibleByArith();
    void updateModeBtnState();
    void updateShapeAreaState();
private slots:
    void sNameChanged();
    void on_modeTypeBtn_clicked(bool checked);
    void on_extendBtn_clicked(bool checked);
};

#endif // SHAPESVMSENSESETWIDGET_H
