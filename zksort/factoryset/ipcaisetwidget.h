#ifndef IPCAISETWIDGET_H
#define IPCAISETWIDGET_H

#include <basewidget.h>
#include "mylineedit.h"
#include <QLabel>

namespace Ui {
class IpcAISetWidget;
}

class IpcAISetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit IpcAISetWidget(QWidget *parent = 0);
    ~IpcAISetWidget();
    void showPage(bool bshow);
    void receiveMsg(int msg1, int msg2, int msg3);

protected:
    virtual void retranslateUiWidget();
private slots:
    void on_pushButton_clicked();

private:
    Ui::IpcAISetWidget *ui;
    QLabel *m_pIpcAITitle[MAX_IPC_NUM];
    myLineEdit *m_pIpcAIPara[MAX_IPC_NUM][4];

    void updateView();
};

#endif // IPCAISETWIDGET_H
