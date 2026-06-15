#ifndef IPCCLASSSETWIDGET_H
#define IPCCLASSSETWIDGET_H


#include "basewidget.h"

class myLineEdit;
#define BIGCLASSNUM  20
namespace Ui {
class IpcClassSetWidget;
}

class IpcClassSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit IpcClassSetWidget(QWidget *parent = 0);
    ~IpcClassSetWidget();

    void receiveMsg(int msg1,int msg2 = 0,int msg3=0);
    void showPage(bool bshow = true);
    void retranslateUiWidget();

    QString currentpiciname;
    QStringList classNameStrList;  // 模型类别名称
    QStringList bigclassNameStrList;  // 统计类别名称

    myLineEdit* m_nameClassEdit[MAX_IPC_MODEL_CLASS];
    myLineEdit* m_pClassStatisticsLineEdit[MAX_IPC_MODEL_CLASS];
    QCheckBox* m_pClassStatisticsQbox[MAX_IPC_MODEL_CLASS];

    myLineEdit* m_nameBigClassEdit[BIGCLASSNUM];
    myLineEdit* m_pBigClassStatisticsCoeff[BIGCLASSNUM];
    QLabel *m_pBigClassStatisticsId[BIGCLASSNUM];

    QSignalMapper* m_classNameEditSigMapper;
    QSignalMapper* m_alarmEnableSigMapper;
    struCnfGlobal   m_temCnfg;
    struCnfProfile  m_temCnfp;

    void updateClassInfo();
    void updateClassShow();
    void updateBigClassShow();
    void updatePagelabel();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_currentpiciname_textChanged(const QString &arg1);

    void sClass1NameChanged(int);
        void sAlarmEnableSlots(int index);

private:
    Ui::IpcClassSetWidget *ui;
//    int m_nIndexOfPage,m_nTotalPage;
};

#endif // SUPPRESSSETDIALOG_H
