#ifndef IPCEJECTWIDGET_H
#define IPCEJECTWIDGET_H

#include "basewidget.h"
#include "mynetwork.h"
#include "mylineedit.h"

namespace Ui {
class IpcEjectWidget;
}

class IpcEjectWidget : public basewidget
{
    Q_OBJECT

public:
    explicit IpcEjectWidget(QWidget *parent = 0);
    ~IpcEjectWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
    void OnViewChange(int view);
    void OnChuteChange();
    void OnLayerChange();
protected:
    void retranslateUiWidget();
public slots:
    void updateWave();

private slots:
    void on_pushButton_5_clicked();

    void on_pushButton_10_clicked();

private:
    Ui::IpcEjectWidget *ui;
    void updateValue();

};

#endif // IPCEJECTWIDGET_H
