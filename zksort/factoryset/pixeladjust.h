#ifndef PIXELADJUST_H
#define PIXELADJUST_H

#include "basewidget.h"
#include "mylineedit.h"
namespace Ui {
class PixelAdjust;
}

class PixelAdjust : public basewidget
{
    Q_OBJECT

public:
    explicit PixelAdjust(QWidget *parent = 0);
    ~PixelAdjust();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
    void OnViewChange(int view);
    void OnLayerChange();
    void OnChuteChange();
protected:
    void retranslateUiWidget();

private slots:
    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

private:
    Ui::PixelAdjust *ui;
    myLineEdit *m_edit[4];
    QLabel *m_label[4];
    QPushButton *m_pushButton[4];
    void updateSetValue();
    void updateEditVisible();
};

#endif // PIXELADJUST_H
