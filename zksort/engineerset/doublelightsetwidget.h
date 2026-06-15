#ifndef DOUBLELIGHTSETWIDGET_H
#define DOUBLELIGHTSETWIDGET_H
#include "basewidget.h"
#include "mylightcontrol.h"

namespace Ui {
class DoubleLightSetWidget;
}

class DoubleLightSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit DoubleLightSetWidget(QWidget *parent = 0);
    ~DoubleLightSetWidget();

    void retranslateUiWidget();
    void showPage(bool bshow);
private slots:
    void lightBoardBtnClicked();

    void on_pushButton_allInOne_clicked(bool checked);

        void getscreenshot();

private:
    Ui::DoubleLightSetWidget *ui;
    void updateState();
    void updateLampState();
    MylightControl *m_materialLamp[MAX_LAMP_PER_BOARD];
    MylightControl *m_BackLamp[MAX_LAMP_PER_BOARD];

    int m_MaterialLampCount;
    int m_BackLampCount;
    int m_lightBoardIndex;
    QPushButton *m_pLightBoardBtn[MAX_LAMP_BOARD_PER_INTERFACE];
    void updateLightBoardBtn();
    void getLampCount();

    int n_MaterialLampCnt;
    int n_BackLampCnt;
              QTimer *shottimer;

};

#endif // DOUBLELIGHTSETWIDGET_H
