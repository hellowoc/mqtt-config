#ifndef MODELSELECTWIDGET_H
#define MODELSELECTWIDGET_H

#include "basewidget.h"
#include <QListWidget>
#include <QComboBox>
namespace Ui {
class ModelSelectWidget;
}

class ModelSelectWidget : public basewidget
{
    Q_OBJECT

public:
    explicit ModelSelectWidget(QWidget *parent = 0);
    ~ModelSelectWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
    void OnViewChange(int view);
    void OnChuteChange();
    void OnLayerChange();
public slots:
    void getModelistResult();
    void uploadEvaluateResult();



protected:
    void retranslateUiWidget();
private slots:

    void updatelabelname();

    void on_recheckBtn_clicked();

    void on_downloadBtn_clicked();

    void on_btn_prePage_clicked();

    void on_btn_nextPage_clicked();

    void sClassModeChanged(int);

    void on_lineEdit_textChanged(const QString &arg1);

    void updateListWidget();
    void updateBtn();
    void getmodellist();



    void on_introducBtn_clicked();

    void on_pushButton_3_clicked();

    void on_extendBtn_clicked(bool checked);

private:
    Ui::ModelSelectWidget *ui;
    QListWidget *m_plistWidget;

    int m_curModelIndex;

    int m_nIndexOfPage;
    int m_nTotalPage;
    QLabel *biglabel[5];
    QComboBox *labelname[5];
    QSignalMapper *m_classModeSigMapper;
    QTimer *timer;




};

#endif // MODELCHANGEWIDGET_H
