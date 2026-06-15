#ifndef IPCCLASSPARAMS_H
#define IPCCLASSPARAMS_H

#include <basewidget.h>
#include "mylineedit.h"
#include <QLabel>

namespace Ui {
class ipcclassparams;
}

static int enterCnts = 0;
class ipcclassparams : public basewidget
{
    Q_OBJECT
    enum{
        CONST_MAX_MODEL_PER_PAGE_SHOW=52,
        CONST_MAX_MODEL_PER_PAGE=24
    };

public:
    explicit ipcclassparams(QWidget *parent = 0);
    ~ipcclassparams();

    void showPage(bool bshow);
    void receiveMsg(int msg1, int msg2, int msg3);

    void OnViewChange(int view);
    void OnChuteChange();
    void OnLayerChange();
    void OnGroupChange(int groupIndex);

    void getCurrentViewIpcIndex();  //获取不同视对应的IPC序号

protected:
    virtual void retranslateUiWidget();

public slots:
    void sClassEnableSlots(int index);
    void sClass1NameChanged(int index);

    void sClassLabelBoxChanged(int index);
    void sClassTickEnableChanged(int index);
    void sClassDelayEnableChanged(int index);
    void sClassFixedChanged(int index);
    void sClassModeChanged(int index);
    void sSelReverseSlots(int index);
    void sClassANDEnableSlots(int index);

private slots:
    void on_syncModeNameBtn_clicked();

    void on_m_applyBtn_clicked();

    void getscreenshot();

private:
    Ui::ipcclassparams *ui;

    SetEditValueWidget* m_pClassSensEdit[MAX_IPC_MODEL_CLASS];
    SetEditValueWidget* m_pClassThresholdEdit[MAX_IPC_MODEL_CLASS];
    myLineEdit* m_nameClassEdit[MAX_IPC_MODEL_CLASS];
    QCheckBox* m_pClassEnable[MAX_IPC_MODEL_CLASS];
    //高级参数
    myLineEdit* m_pBoxClassEdit[MAX_IPC_MODEL_CLASS];
    myLineEdit* m_pBoxClassEdit2[MAX_IPC_MODEL_CLASS];
    QCheckBox* m_pTickModeClass[MAX_IPC_MODEL_CLASS];
    QCheckBox* m_pFixedClass[MAX_IPC_MODEL_CLASS];
    QCheckBox* m_pDelayModeClass[MAX_IPC_MODEL_CLASS];
    QLabel *m_pBoxClassLable[MAX_IPC_MODEL_CLASS];
    QLabel *m_pBoxClassLable2[MAX_IPC_MODEL_CLASS];
    QLabel *m_pModeLabel[MAX_IPC_MODEL_CLASS];
    QComboBox *m_pEjctorArithCombox[MAX_IPC_MODEL_CLASS];
    QLabel *m_pHWRatioLabel[MAX_IPC_MODEL_CLASS];
    myLineEdit* m_pHWRatioEdit[MAX_IPC_MODEL_CLASS];
    QLabel *m_pPosAdjustLabel[MAX_IPC_MODEL_CLASS];
    myLineEdit* m_pPosAdjustEdit[MAX_IPC_MODEL_CLASS];
    QLabel *m_pBDLabel[MAX_IPC_MODEL_CLASS];
    myLineEdit* m_pBDEdit[MAX_IPC_MODEL_CLASS];
    QLabel *m_pBXLabel[MAX_IPC_MODEL_CLASS];
    myLineEdit* m_pBXEdit[MAX_IPC_MODEL_CLASS];
    QCheckBox* m_pSelReverse[MAX_IPC_MODEL_CLASS];
    myLineEdit* m_pSelLimit1[MAX_IPC_MODEL_CLASS];
    myLineEdit* m_pSelLimit2[MAX_IPC_MODEL_CLASS];
    QCheckBox* m_pClassANDEnable[MAX_IPC_MODEL_CLASS];

    QPushButton *m_viewBtn[MAX_VIEW];
    int m_sensValue[MAX_IPC_MODEL_CLASS];
    int m_ThresholdValue[MAX_IPC_MODEL_CLASS];
    //高级参数
    int m_LabelBoxValue[MAX_IPC_MODEL_CLASS];
    int m_TickModeEnable[MAX_IPC_MODEL_CLASS];
    int m_FixedModeEnable[MAX_IPC_MODEL_CLASS];

    void updateView();

    int m_curLayer,m_curView;
    int m_curIpcIndex;
    int m_totalPage, m_curPage;
    int m_curViewIpcIndex[MAX_VIEW];

    QSignalMapper* m_classNameEditSigMapper;
    QSignalMapper* m_classEnableSigMapper;
    QSignalMapper* m_classANDEnableSigMapper;
    //高级参数
    QSignalMapper* m_classLabelBoxSigMapper;
    QSignalMapper* m_classTickEnableSigMapper;
    QSignalMapper* m_classDelayEnableSigMapper;
    QSignalMapper* m_classFixedEnableSigMapper;
    QSignalMapper* m_classModeSigMapper;
    QSignalMapper* m_pSelReverseEnableSigMapper;

    int tmpValue,tmpValue2;
        QTimer *shottimer;
};

#endif // IPCCLASSPARAMS_H
