#ifndef IPCSTATEWIDGET_H
#define IPCSTATEWIDGET_H

#include "basewidget.h"

namespace Ui {
class IPCStateWidget;
}
class IPCStateInfo{
public:
    QString strName;
    int state;
    int badNum;
    IPCStateInfo(QString strName1,int state1, int badNum1){
        strName = strName1;
        state = state1;
        badNum = badNum1;
    }
};


class IPCStateWidget : public basewidget
{
    Q_OBJECT

public:
    explicit IPCStateWidget(QWidget *parent = 0);
    ~IPCStateWidget();

    virtual void showPage(bool bshow);
    void retranslateUiWidget();
    void receiveMsg(int msg1, int msg2, int msg3);
private slots:
    void on_search_clicked();

private:
    QList<IPCStateInfo> m_verinfoVec;
    void updateVersionInfo();
    void updateTableWidget();

    Ui::IPCStateWidget *ui;
};

#endif // IPCSTATEWIDGET_H
