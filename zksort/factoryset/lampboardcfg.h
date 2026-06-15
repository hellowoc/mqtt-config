#ifndef LAMPBOARDCFG_H
#define LAMPBOARDCFG_H

#include "basewidget.h"
#include "mylineedit.h"
namespace Ui {
class LampBoardCfg;
}

class LampBoardCfg : public basewidget
{
    Q_OBJECT

public:
    explicit LampBoardCfg(QWidget *parent = 0);
    ~LampBoardCfg();
 virtual void showPage(bool bshow = true);
 void retranslateUiWidget();
 void receiveMsg(int msg1, int msg2, int msg3);
private:
 void updateBoardBtn();
 void updateCurBoard();
 void updateLampBoardTypeBtnState(int portIndex);
 void updateLampPortEnableState(int portIndex);
 void updateLampPortViewBtnState(int portIndex);
 void updateLampLayerAddr(int portIndex);
 void updateLampBoardLimit(int portIndex);
 void updateLampCoef(int portIndex);
 void updateLampName(int portIndex);

 void updateLayerSetVisiable();
public slots:

 void lampBoardBtnClicked(int);
 void lampTypeBtnClicked(int);
 void lampViewBtnClicked(int);
 void lampPortEnableClicked(int);
 void sLampNameChanged();
private slots:
 void on_okbtn_clicked();

 void on_restoreBtn_clicked();

 void on_pushButton_allInOne_clicked(bool checked);

 void getscreenshot();

private:
    Ui::LampBoardCfg *ui;
    int m_CurBoardIndex;
    QButtonGroup *m_pBoardBtnGroup;
    QSignalMapper *m_pBoardBtnGroupSigMapper;
    QSignalMapper *m_pLampPortTypeBtnSigMapper;
    QSignalMapper *m_pLampPortViewBtnSigMapper;
    QSignalMapper *m_plampPortEnableSigMapper;
    QCheckBox *m_plampPortEnable[MAX_LAMP_PER_BOARD];
    QPushButton *m_plampViewBtn[MAX_LAMP_PER_BOARD];
    QPushButton *m_pLampPortTypeBtn[MAX_LAMP_PER_BOARD];
    myLineEdit *m_pLimitEdit[MAX_LAMP_PER_BOARD];
    myLineEdit *m_pboardLayerEdit[MAX_LAMP_PER_BOARD];
    myLineEdit *m_pLampCoef[MAX_LAMP_PER_BOARD];
    myLineEdit *m_pRenameEdit[MAX_LAMP_PER_BOARD];

              QTimer *shottimer;
};

#endif // LAMPBOARDCFG_H
