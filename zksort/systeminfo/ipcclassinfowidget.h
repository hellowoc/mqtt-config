#ifndef IPCCLASSINFOWIDGET_H
#define IPCCLASSINFOWIDGET_H

#include "basewidget.h"

namespace Ui {
class IPCClassInfoWidget;
}
class IPCModelClassInfo{
public:
    QString strName;
    QString strClassName;
    IPCModelClassInfo(QString strName1,QString strName2){
        strName = strName1;
        strClassName = strName2;
    }
};


class IPCClassInfoWidget : public basewidget
{
    Q_OBJECT

public:
    explicit IPCClassInfoWidget(QWidget *parent = 0);
    ~IPCClassInfoWidget();

    virtual void showPage(bool bshow);
    void retranslateUiWidget();
    void receiveMsg(int msg1, int msg2, int msg3);
private slots:
    void on_search_clicked();

private:
    QList<IPCModelClassInfo> m_classInfoVec;
    void updateVersionInfo();
    void updateTableWidget();

    Ui::IPCClassInfoWidget *ui;
};

#endif // IPCCLASSINFOWIDGET_H
