#ifndef MYSETDATETIME_H
#define MYSETDATETIME_H
#include "basewidget.h"
#include <QDialog>

namespace Ui {
class MySetDateTime;
}

class MySetDateTime : public QDialog,public AbstractInterface
{
    Q_OBJECT

public:
    explicit MySetDateTime(QWidget *parent = 0);
    ~MySetDateTime();
    void receiveMsg(int msg1, int msg2, int msg3);
    void retranslateUiWidget();

private slots:
    void on_cancelBtn_clicked();

    void on_okBtn_clicked();

private:
    Ui::MySetDateTime *ui;
    int data[6];
};

#endif // MYSETDATETIME_H
