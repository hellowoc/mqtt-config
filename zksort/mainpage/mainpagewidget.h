#ifndef MAINPAGEWIDGET_H
#define MAINPAGEWIDGET_H

#include "basewidget.h"

namespace Ui {
class MainPageWidget;
}

class MainPageWidget : public basewidget
{
    Q_OBJECT

public:
    explicit MainPageWidget(QWidget *parent = 0);
    ~MainPageWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
protected:
    void retranslateUiWidget();
    void paintEvent(QPaintEvent *);
private slots:

    void on_senseSetBtn_clicked();

    void on_cleanBtn_clicked();

    void on_feedSetBtn_clicked();

    void on_valveBtn_clicked();

    void on_setBtn_clicked();

    void on_shutDownBtn_clicked();

    void on_runBtn_clicked();

    void on_modeBtn_clicked();

private:
    Ui::MainPageWidget *ui;
    void updateLevel2();
    void setText();
    void updateFeederState();
    void updateUi();
};

#endif // MAINPAGEWIDGET_H
