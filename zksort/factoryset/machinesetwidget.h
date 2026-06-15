#ifndef MACHINESETWIDGET_H
#define MACHINESETWIDGET_H

#include "basewidget.h"
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include "groupsetdialog.h"
class QTableWidget;
namespace Ui {
class MachineSetWidget;
}

class MachineSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit MachineSetWidget(QWidget *parent = 0);
    ~MachineSetWidget();

    void showPage(bool bshow);
protected:
    virtual void retranslateUiWidget();
private slots:
    void viewCheckboxClicked(bool checked);
    void viewXsysCheckboxClicked(bool checked);
    void groupDelBtnClicked(bool checked);
    void groupAddBtnClicked(bool checked);
    void on_comboBoxMachineType_activated(const QString &arg1);

    void on_comboBoxMachineChildType_activated(int index);

    void on_comboBoxVavle_activated(const QString &arg1);

    void on_segmentCheckBox_clicked(bool checked);

    void on_applyBtn_clicked();

    void curlayerBtnClickedSlots();

    void on_frontUintComboBox_activated(int index);

    void on_frontAuxiliUintComboBox_activated(int index);

    void on_rearUintComboBox_activated(int index);

    void on_rearAuxiUintComboBox_activated(int index);

    void on_lowLayerComboBox_activated(int index);

    void on_upLayerComboBox_activated(int index);

    void on_frontUintComboBox_3_activated(int index);
    
    void on_frontAuxiliUintComboBox_3_activated(int index);
    
    void on_rearUintComboBox_3_activated(int index);
    
    void on_rearAuxiUintComboBox_3_activated(int index);
    
    void on_comboBoxVavleRatio_activated(int index);

    void on_frontValveComboBox_activated(int index);

    void on_rearValveComboBox_activated(int index);

    void on_frontAuxiliValveComboBox_activated(int index);

    void on_rearAuxiValveComboBox_activated(int index);

    void on_frontValveComboBox_3_activated(int index);

    void on_rearValveComboBox_3_activated(int index);

    void on_frontAuxiliValveComboBox_3_activated(int index);

    void on_rearAuxiValveComboBox_3_activated(int index);


    void getscreenshot();

    void on_comboBoxCrossEjector_activated(int index);

    void on_comboBoxControlBoard_activated(int index);

    void on_InputSetBtn_clicked();
    void on_OutputSetBtn_clicked();
    void GroupSetBtnClicked(int );
private:
    Ui::MachineSetWidget *ui;

    QCheckBox *m_viewCheckBox[MAX_VIEW];
    QCheckBox *m_viewXsysCheckBox[MAX_VIEW];
    QPushButton *m_viewGroupset[MAX_VIEW];
    QSignalMapper *m_pGroupsetBtnMapper;
    void updateViewCheckboxAndViewBtnState();
    void updateViewXsysCheckboxAndViewBtnState();
    void updateMachineSetting();
    void updateLayer();
    void initVavleGroupNum();
    void updateGroup();
    bool setVavleGroup();
    void updateSpliceState();
    void updateVavleStep();
    void updateSpliceType(int valveNum);
    QStringList m_machineTypeStrList;
    QMap<QString,QStringList> m_machineTypeChuteMap;
    QMap<QString,int> m_machineTypeMap;
    int m_chuteNum,m_vavleNum,m_vavleStep, m_machineAttr;
    int m_groupVavleNum[MAX_GROUP_IDENTIFY];
    QLabel *m_groupVavleLabel[MAX_GROUP_IDENTIFY];
    QLabel *m_pGroupTitleLbl[MAX_GROUP_IDENTIFY];
    QPushButton* m_groupVavleBtnDel[MAX_GROUP_IDENTIFY];
    QPushButton* m_groupVavleBtnAdd[MAX_GROUP_IDENTIFY];
    struCnfGlobal m_temCnf;
    void setChuteNumInfo();//设置与料槽数保持一致的信息
    void updateAll();
    QPushButton *m_pCurLayerBtn[2];

    QComboBox *m_pViewUintComboBox[MAX_VIEW];
    QComboBox *m_pViewValveComboBox[MAX_VIEW];
    void updateViewUintComboBox();
    void updateViewValveComboBox();

    QComboBox *m_pVavleCountComboBox[2];
    void updateVavleCountComboBox();
    QStringList vavleList;

    bool m_groupShow;
    QTimer *shottimer;
    QStringList crossEjectorList;
        QStringList groupset;
};

#endif // MACHINESETWIDGET_H
