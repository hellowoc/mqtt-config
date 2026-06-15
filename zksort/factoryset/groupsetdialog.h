#ifndef GROUPSETDIALOG_H
#define GROUPSETDIALOG_H

#include "basewidget.h"
#include "svmtool.h"
#include "QStringList"
#include<QPair>
class myLineEdit;
namespace Ui {
class GroupSetDialog;
}

class GroupSetDialog : public QDialog,public AbstractInterface
{
    Q_OBJECT

public:
    explicit GroupSetDialog(QWidget *parent = 0);
    ~GroupSetDialog();
    void receiveMsg(int msg1, int msg2, int msg3);

    QGroupBox *groupbox[5];
   // myLineEdit *m_pLimitEdit[5];
    QComboBox *m_combox[5][2];
    QStringList groupset; //
    stru_layer_info mLayerInfo;
    int nMachineType;
    int nView;
    int nGroupnum;

    void updateshow();
    void updateGroupInfo();
private slots:

    void on_cancelBtn_clicked();

    void on_okBtn_clicked();

//    void on_lineEdit_textChanged(const QString &arg1);

//    void on_lineEdit_2_textChanged(const QString &arg1);

//    void on_lineEdit_3_textChanged(const QString &arg1);

//    void on_lineEdit_4_textChanged(const QString &arg1);

//    void on_lineEdit_5_textChanged(const QString &arg1);

//    void on_lineEdit_6_textChanged(const QString &arg1);

//    void on_lineEdit_7_textChanged(const QString &arg1);

//    void on_lineEdit_8_textChanged(const QString &arg1);

private:
    Ui::GroupSetDialog *ui;
    bool checkgroupset();

    bool checkChannelGroupValid(const QList<QPair<int, int>>& groups,int );


};

#endif // AISETDIALOG_H
