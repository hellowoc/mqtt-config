#ifndef SYSCAMCALIINFOWIDGET_H
#define SYSCAMCALIINFOWIDGET_H

#include "basewidget.h"
#include "messagedialog.h"
namespace Ui {
class SysCamCaliInfoWidget;
}
class CamCaliInfo{
public:
    QString strName;
    QString cali0;
    QString cali1;
    QString cali2;
    QString cali3;
    QString cali4;
    QString cali5;

    CamCaliInfo(QString str1,QString str2,QString str3,QString str4,QString str5,QString str6,QString str7){
        strName = str1;
        cali0 = str2;
        cali1 = str3;
        cali2 = str4;
        cali3 = str5;
        cali4 = str6;
        cali5 = str7;
    }
};

class SysCamCaliInfoWidget : public basewidget
{
    Q_OBJECT

public:
    explicit SysCamCaliInfoWidget(QWidget *parent = 0);
    ~SysCamCaliInfoWidget();
    void showPage(bool bshow);
    void retranslateUiWidget();
    void receiveMsg(int msg1, int msg2, int msg3);
private slots:
    void on_search_clicked();
    void getscreenshot();
private:
    Ui::SysCamCaliInfoWidget *ui;

    QList<CamCaliInfo> m_verinfoVec;
    void updateVersionInfo();
    void updateTableWidget();
    MessageDialog *m_pMessage;

    QTimer *shottimer;
    QStringList tdModeList;

};

#endif // SYSCAMCALIINFOWIDGET_H
