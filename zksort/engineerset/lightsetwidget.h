#ifndef LIGHTSETWIDGET_H
#define LIGHTSETWIDGET_H

#include "basewidget.h"
#include "mylightcontrol.h"
namespace Ui {
class LightSetWidget;
}

class LightSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit LightSetWidget(QWidget *parent = 0);
    ~LightSetWidget();
    void retranslateUiWidget();
    void showPage(bool bshow);
    void OnLayerChange();
private:
    Ui::LightSetWidget *ui;
    void updateFrontRearState();
    void updateFrontLampState();
    void updateRearLampState();
    void getLampCount();
    MylightControl *m_frontLamp[MAX_LAMP_PER_BOARD];
    MylightControl *m_frontBackLamp[4];

    MylightControl *m_rearLamp[MAX_LAMP_PER_BOARD];
    MylightControl *m_rearBackLamp[4];
    int m_frontMaterialLampCount;
    int m_frontBackLampCount;

    int m_rearMaterialLampCount;
    int m_rearBackLampCount;

    int n_fMaterialLampCnt;
    int n_fBackLampCnt;
    int n_rMaterialLampCnt;
    int n_rBackLampCnt;
};

#endif // LIGHTSETWIDGET_H
