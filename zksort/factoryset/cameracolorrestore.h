#ifndef CAMERACOLORRESTORE_H
#define CAMERACOLORRESTORE_H

#include <basewidget.h>
#include "mylineedit.h"
#include <QLabel>

namespace Ui {
class cameraColorRestoreWidget;
}

class cameraColorRestoreWidget : public basewidget
{
    Q_OBJECT

public:
    explicit cameraColorRestoreWidget(QWidget *parent = 0);
    ~cameraColorRestoreWidget();

    void showPage(bool bshow);
    void receiveMsg(int msg1, int msg2, int msg3);

    void OnViewChange(int view);
    void OnChuteChange();
    void OnLayerChange();

protected:
    virtual void retranslateUiWidget();

private slots:
    void on_colorRestoreCheckBox_clicked();

private:
    Ui::cameraColorRestoreWidget *ui;


    void updateView();
};

#endif // CAMERACOLORRESTORE_H
