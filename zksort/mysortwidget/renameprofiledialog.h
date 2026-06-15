#ifndef RENAMEPROFILEDIALOG_H
#define RENAMEPROFILEDIALOG_H

#include <QDialog>

namespace Ui {
class RenameProfileDialog;
}

class RenameProfileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RenameProfileDialog(QWidget *parent = 0);
    ~RenameProfileDialog();
    void initName(QString name);
    QString getName();
private slots:
    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

private:
    Ui::RenameProfileDialog *ui;
};

#endif // RENAMEPROFILEDIALOG_H
