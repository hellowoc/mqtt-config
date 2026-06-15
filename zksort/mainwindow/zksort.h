#ifndef ZKSORT_H
#define ZKSORT_H

#include <QApplication>

#include "baseui.h"

namespace Ui {
class ZKSort;
}

class ZKSort : public BaseUi
{
    Q_OBJECT

public:
    explicit ZKSort(QWidget *parent = 0);
    ~ZKSort();
    void PageChanged(const uint pageId);
    void setBottomEnable(bool enable);
    void refreshBottomStatus();
    virtual void showPage(bool bshow);
protected:
    void keyPressEvent(QKeyEvent *);
    void retranslateUiWidget();
private slots:
    void myWipeStartSlt();
    void lostFpgaPowerOffSlt();
    void alarmTurnOffFeedSlt();
    void alarmTurnOnFeedSlt();
    void alarmColorVoiceOnSlt();
    void alarmColorVoiceOffSlt();
    void warnDccryptSlt();
    void writeDccryptTimeSlt();
    void dccryptCheck();
    void write30DayDccryptTimeSlt();
    void waitSetFpgaMode();
    void infoStartValve();
    void infoUpdateZKSort(QVariant);
    void infoUpdateIpcModel(QVariant);
    void infoUploadIpcInfo();
    void infoStartIpcFilterCottonClean();
private:
    Ui::ZKSort *ui;
    void LoadSkin();
    void startEnterMainPage();
    void communication();
    QVector<int> m_hideBottomPageId;
    QVector<int> m_hideTopPageId;

    QTimer *mytimer;
    QTimer *mytimerWaitFpgaMode;
};

class zkApplication : public QApplication
{
public:
    zkApplication(int & argc, char ** argv);
};

#endif // ZKSORT_H
