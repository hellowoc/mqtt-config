#ifndef MESSAGEDIALOG_H
#define MESSAGEDIALOG_H

#include <QDialog>

namespace Ui {
class MessageDialog;
}

class MessageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MessageDialog(QWidget *parent = 0);
    explicit MessageDialog(QString msg,QString title,int type,QWidget *parent = 0);
    ~MessageDialog();

    void setText(QString msg);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MessageDialog *ui;
};

#endif // MESSAGEDIALOG_H
