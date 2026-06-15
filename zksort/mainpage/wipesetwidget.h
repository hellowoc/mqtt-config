#ifndef WIPESETWIDGET_H
#define WIPESETWIDGET_H

#include "basewidget.h"

namespace Ui {
class wipeSetWidget;
}

class wipeSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit wipeSetWidget(QWidget *parent = 0);
    ~wipeSetWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);

protected:
    void retranslateUiWidget();
private slots:
    void on_manuWipePushBtn_clicked();

    void on_wipeSingleCheckBox_clicked();

    void on_timerWipeRadioBtn_clicked();

    void on_combineWipeRadioBtn_clicked();

    void on_aiWipeBtn_clicked();

    void on_checkBox_clicked(bool checked);

    void on_checkBox_2_clicked(bool checked);

private:
    Ui::wipeSetWidget *ui;

    int m_wipeDuration;
    int m_wipeInterval;
};

#endif // WIPESETWIDGET_H
