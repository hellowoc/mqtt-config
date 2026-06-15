#ifndef IPCCLASSADVPARAMS_H
#define IPCCLASSADVPARAMS_H

#include <basewidget.h>
#include "mylineedit.h"
#include <QLabel>

namespace Ui {
class ipcclassadvparams;
}

class ipcclassadvparams : public basewidget
{
    Q_OBJECT
    enum{
        CONST_MAX_MODEL_PER_PAGE=18
    };

public:
    explicit ipcclassadvparams(QWidget *parent = 0);
    ~ipcclassadvparams();

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
    void sClassFixedChanged(int index);
    void sClassModeChanged(int index);
//    void sClass1NameChanged();
//    void sClass2NameChanged();
//    void sClass3NameChanged();
//    void sClass4NameChanged();
//    void sClass5NameChanged();
//    void sClass6NameChanged();

private slots:
    void on_comboBoxIpcSelect_activated(int index);

    void on_applyBtn_clicked();

    void on_lastPageBtn_clicked();

    void on_nextPageBtn_clicked();

private:
    Ui::ipcclassadvparams *ui;
    myLineEdit* m_nameClassEdit[CONST_MAX_MODEL_PER_PAGE];
    QCheckBox* m_pClassEnable[CONST_MAX_MODEL_PER_PAGE];

    myLineEdit* m_pBoxClassEdit[CONST_MAX_MODEL_PER_PAGE];
    QCheckBox* m_pTickModeClass[CONST_MAX_MODEL_PER_PAGE];
    QCheckBox* m_pFixedClass[CONST_MAX_MODEL_PER_PAGE];
    QLabel *m_pBoxClassLable[CONST_MAX_MODEL_PER_PAGE];
    QLabel *m_pModeLabel[CONST_MAX_MODEL_PER_PAGE];
    QComboBox *m_pModeCombox[CONST_MAX_MODEL_PER_PAGE];

//    SetEditValueWidget* m_pCurUnitEdit;

    QPushButton *m_viewBtn[MAX_VIEW];
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

    QSignalMapper* m_classLabelBoxSigMapper;
    QSignalMapper* m_classTickEnableSigMapper;
    QSignalMapper* m_classFixedEnableSigMapper;
    QSignalMapper* m_classModeSigMapper;

    int labelBoxValueLimit;
};

#endif // IPCCLASSADVPARAMS_H
