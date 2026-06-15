#ifndef SETSVMUSEDIALOG_H
#define SETSVMUSEDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QLabel>

namespace Ui {
class SetSvmUseDialog;
}

class SetSvmUseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetSvmUseDialog(QWidget *parent = 0);
    ~SetSvmUseDialog();

private slots:
    void on_cancelBtn_clicked();

    void on_okBtn_clicked();

private:
    Ui::SetSvmUseDialog *ui;
    QCheckBox* m_pUseCheckbox[8][5];
    QLabel* m_pViewLbl[8];
    QLabel* m_pTimesLbl[5];
};

#endif // SETSVMUSEDIALOG_H
