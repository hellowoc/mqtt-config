#ifndef MODEMANAGEWIDGET_H
#define MODEMANAGEWIDGET_H

#include "basewidget.h"
#include <QListWidget>
namespace Ui {
class ModeManageWidget;
}

class ModeManageWidget : public basewidget
{
    Q_OBJECT

public:
    explicit ModeManageWidget(QWidget *parent = 0);
    ~ModeManageWidget();
    void showPage(bool bshow = true);
protected:
    void retranslateUiWidget();
private slots:
    void on_newBtn_clicked();

    void on_delBtn_clicked();

    void on_listWidget_currentRowChanged(int currentRow);

    void on_applyBtn_clicked();

    void on_renameBtn_clicked();

    void on_copyBtn_clicked();

    void on_lockBtn_clicked();

    void on_exportBtn_clicked();

    void on_importBtn_clicked();

    void on_backupBtn_clicked();

    void on_recoveryBtn_clicked();

    void on_sMoreBtn_clicked();

    void on_ModelChangeBtn_clicked();

    void on_invalidClearBtn_clicked();

    void on_checkBox_clicked(bool checked);

    void getscreenshot();

    void on_comboBox_activated(int index);
    void on_presetBtn_clicked();

    void on_recoveryAllBtn_clicked();

private:
    Ui::ModeManageWidget *ui;
    QListWidget *m_plistWidget;
    QString m_strModePre;
    void updateListWidget();
    void updateAllBtnShow();
    void updateApplyBtnState();
    void apply();
    bool m_moreBtnUsed;
    QTimer *shottimer;
       QStringList samelist;
};

#endif // MODEMANAGEWIDGET_H
