#ifndef AISETDIALOG_H
#define AISETDIALOG_H

#include "basewidget.h"
#include "svmtool.h"
class myLineEdit;
namespace Ui {
class AiSetDialog;
}

class AiSetDialog : public QDialog,public AbstractInterface
{
    Q_OBJECT

public:
    explicit AiSetDialog(QWidget *parent = 0);
    ~AiSetDialog();
    void receiveMsg(int msg1, int msg2, int msg3);
private slots:
    void on_cancelBtn_clicked();

    void on_okBtn_clicked();

    void updateSvmTypeChecked(int );
private:
    Ui::AiSetDialog *ui;
    int data[4][4];
    myLineEdit *lineEdit[4][4];

    float data2[6][4];
    myLineEdit *lineEdit2[6][4];

    int m_data[2];
    myLineEdit *m_lineEdit[2];
    QList<QCheckBox*> svmTypeList;
    QSignalMapper *svmTypeSignalMap;
};

#endif // AISETDIALOG_H
