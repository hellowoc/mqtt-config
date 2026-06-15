#ifndef ENGINEERWIDGET_H
#define ENGINEERWIDGET_H

#include "basewidget.h"

namespace Ui {
class EngineerWidget;
}

class EngineerWidget : public basewidget
{
    Q_OBJECT

public:
    explicit EngineerWidget(QWidget *parent = 0);
    ~EngineerWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
protected:
    void retranslateUiWidget();



private slots:

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_radioButton_engineer_clicked(bool checked);

    void on_radioButton_user_clicked(bool checked);

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_radioButton_factory_clicked(bool checked);

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_onekeyfix_clicked();

    void on_plcCtrlBtn_clicked();
    void on_pushButton_9_clicked();
private:
    Ui::EngineerWidget *ui;
    void updateTimeEdit();
    void updateLevelRadioBtn();
    int data[6];
};

#endif // ENGINEERWIDGET_H
