#ifndef SYSVERSIONINFOWIDGET_H
#define SYSVERSIONINFOWIDGET_H

#include "basewidget.h"
#include "messagedialog.h"
namespace Ui {
class SysVersionInfoWidget;
}
class VersionInfo{
public:
    QString strName;
    int state;
    QString value;
    QString strMode;
    QString strSvn;
    VersionInfo(QString str1,int stateint,QString str3,QString str4,QString str5){
        strName = str1;
        state = stateint;
        value = str3;
        strMode = str4;
        strSvn = str5;
    }
};

class SysVersionInfoWidget : public basewidget
{
    Q_OBJECT

public:
    explicit SysVersionInfoWidget(QWidget *parent = 0);
    ~SysVersionInfoWidget();
    void showPage(bool bshow);
    void retranslateUiWidget();
    void receiveMsg(int msg1, int msg2, int msg3);
private slots:
    void on_search_clicked();
    void getscreenshot();
private:
    Ui::SysVersionInfoWidget *ui;

    QList<VersionInfo> m_verinfoVec;
    void updateVersionInfo();
    void updateTableWidget();
    MessageDialog *m_pMessage;

    QTimer *shottimer;

};

#endif // SYSVERSIONINFOWIDGET_H
