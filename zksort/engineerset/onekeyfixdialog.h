#ifndef ONEKEYDIALOG_H
#define ONEKEYDIALOG_H

#include "basewidget.h"

class myLineEdit;
namespace Ui {
class OneKeyDialog;
}

class OneKeyDialog : public QDialog,public AbstractInterface
{
    Q_OBJECT

public:
    explicit OneKeyDialog(QWidget *parent = 0);
    ~OneKeyDialog();
    void receiveMsg(int msg1, int msg2, int msg3);
private slots:
    void on_cancelBtn_clicked();

    void on_okBtn_clicked();

    void updateCustomInfo();
    void InfoEnableChange(bool);


    void on_lineEdit_name_textChanged(const QString &arg1);

    void on_lineEdit_QCnum_textChanged(const QString &arg1);

    void on_lineEdit_phone_textChanged(const QString &arg1);

   // void on_lineEdit_place_textChanged(const QString &arg1);

    void on_checkBox_clicked(bool checked);

    void on_lineEdit_comname_textChanged(const QString &arg1);

    void on_lineEdit_place_1_textChanged(const QString &arg1);

    void on_lineEdit_place_2_textChanged(const QString &arg1);

    void on_lineEdit_place_3_textChanged(const QString &arg1);

    void on_lineEdit_place_4_textChanged(const QString &arg1);

    void on_lineEdit_type2_textChanged(const QString &arg1);

    void on_lineEdit_machineID_textChanged(const QString &arg1);
private:
    Ui::OneKeyDialog *ui;
    bool enablechange;

};

#endif // ONEKEYDIALOG_H
