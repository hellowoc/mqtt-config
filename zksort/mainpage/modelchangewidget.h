#ifndef MODELCHANGEWIDGET_H
#define MODELCHANGEWIDGET_H

#include "basewidget.h"
#include <QListWidget>
namespace Ui {
class ModelChangeWidget;
}

class ModelChangeWidget : public basewidget
{
    Q_OBJECT

public:
    explicit ModelChangeWidget(QWidget *parent = 0);
    ~ModelChangeWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
    void OnViewChange(int view);
    void OnChuteChange();
    void OnLayerChange();
    void OnGroupChange(int groupIndex);
protected:
    void retranslateUiWidget();
private slots:
    void on_listWidget_currentRowChanged(int currentRow);
    void on_applyBtn_clicked();

    void on_recheckBtn_clicked();

    void on_replaceBtn_clicked();

    void on_delBtn_clicked();

    void on_ipcSelectComboBox_activated(int index);

    void on_addmodelbtn_clicked();

    void on_rebootIpcBtn_clicked();

    void updateIpcRestartBtnEnable(int view,int unit);

    void on_pushButton_9_clicked();

private:
    Ui::ModelChangeWidget *ui;
    QListWidget *m_plistWidget;
    QString m_strModePre;

    void updateListWidget();
    void updateAllBtnShow();
    void updateApplyBtnState();
    void apply();
    void doModelPreSelect();
    bool m_moreBtnUsed;
    int m_curIpcIndex, m_curModelIndex;
    bool m_modelSame;

};

#endif // MODELCHANGEWIDGET_H
