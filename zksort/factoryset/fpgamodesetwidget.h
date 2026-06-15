#ifndef FPGAMODESETWIDGET_H
#define FPGAMODESETWIDGET_H

#include "basewidget.h"
#include "logger.h"
#include "mylineedit.h"
namespace Ui {
class FpgaModeSetWidget;
}

class FpgaModeSetWidget : public basewidget
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

    public:
        explicit FpgaModeSetWidget(QWidget *parent = 0);
    ~FpgaModeSetWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
protected:
    void retranslateUiWidget();

private slots:
    void on_comboBox_board_activated(int index);
    void on_comboBox_chute_activated(int index);
    void on_comboBox_chain_activated(int index);
    void on_comboBox_mode_activated(int index);
    void on_startUpgradeBtn_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void updateDebugdata();

    void on_pushButton_5_clicked();

private:
    Ui::FpgaModeSetWidget *ui;
    void updateUi();
    void initUi();
    void updateElecBoardCombobox();
    void updateChuteCombobox();
    void updateModeCombobox();
    void updateChainCombobox();
    void updateParamSet();


    QLabel *qlabel[16];
    myLineEdit *m_pParamEdit[16];
    int m_nBoardFactoryOk[MAX_VIEW][MAX_VIEW_UNIT]; //切换工厂模式成功
    int m_nBoardUpdateOk[MAX_VIEW][MAX_VIEW_UNIT];  //升级后切换用户模式成功
    int nCameraBoardMode[MAX_LAYER][MAX_VIEW][MAX_VIEW_UNIT];                  // ceshi
    int m_nBoardTypeIndex;
    int m_nChainIndex;
    int m_nChuteIndex;
    int m_nMirrorIndex;
    int m_view;
    int m_boardAddr;
    int m_versionToUpgrade[5];
    int m_count;
    QStringList viewList;
    QStringList chainList;
    QMap<int,QString> viewMap;

        struCnfProfile m_temCnf;



};

#endif // UPDATEFPGAWIDGET_H
