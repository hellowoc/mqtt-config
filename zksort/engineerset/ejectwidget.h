#ifndef EJECTWIDGET_H
#define EJECTWIDGET_H

#include "basewidget.h"
#include <QButtonGroup>
namespace Ui {
class EjectWidget;
}

class EjectWidget : public basewidget
{
    Q_OBJECT

public:
    explicit EjectWidget(QWidget *parent = 0);
    ~EjectWidget();
    void retranslateUiWidget();
    void showPage(bool bshow);
    void OnGroupChange(int groupIndex);
    void OnViewChange(int view);
    void OnLayerChange();
    void receiveMsg(int msg1, int msg2, int msg3);

public slots:
//    void BelowModeBtnClicked(bool);
    void ViewModeBtnClicked(bool);
private slots:
    void on_ReverseSortBtn1_clicked(bool checked);

    void on_ReverseSortBtn2_clicked(bool checked);

    void on_pushButton_3_clicked(bool checked);

private:
    Ui::EjectWidget *ui;
    void updateView();
    void updateViewValue();
    void updateGroupWidgetStatus();
    void updateReverseSortBtnState();
    void updateRelationBtnState();
    QButtonGroup *m_pBelowModeGroup;
    QButtonGroup *m_pMainfrontRearMode;
    QButtonGroup *m_pViceFrontRearMode;

    int curVavle;
    int curGroup;
    int curView;

    float m_delay;
    bool abnormalDisplay;
};

#endif // EJECTWIDGET_H
