#ifndef NEWMAINPAGE_H
#define NEWMAINPAGE_H

#include "basewidget.h"

namespace Ui {
class NewMainPage;
}

class NewMainPage : public basewidget
{
    Q_OBJECT

public:
    explicit NewMainPage(QWidget *parent = 0);
    ~NewMainPage();
    void showPage(bool bshow);
    void retranslateUiWidget();
    void receiveMsg(int msg1, int msg2, int msg3);
protected:
    void paintEvent(QPaintEvent *e);
private slots:
    void on_runBtn_clicked(bool checked);

    void on_wheelBtn_clicked(bool checked);

    void on_shutdownBtn_clicked();

    void on_senseBtn_clicked();

    void on_feedBtn_clicked();

    void on_wipeBtn_clicked();

    void on_vavleBtn_clicked();

    void on_modeBtn_clicked();

    void on_setBtn_clicked();

    void updateUi();

    void updateshowdata();
private:
    Ui::NewMainPage *ui;


    void updateLevel();
};

#endif // NEWMAINPAGE_H
