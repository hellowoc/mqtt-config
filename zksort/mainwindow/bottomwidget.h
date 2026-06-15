#ifndef BOTTOMWIDGET_H
#define BOTTOMWIDGET_H

#include "baseui.h"

namespace Ui {
class BottomWidget;
}

class BottomWidget : public BaseUi
{
    Q_OBJECT

public:
    explicit BottomWidget(QWidget *parent = 0);
    ~BottomWidget();

    void showPage(bool bshow);
    void PageChanged(int pageId);
    void setValveEnable(bool enable);
public slots:
    void refreshFeedVavleStatus();
protected:
    virtual void retranslateUiWidget();
private slots:
    void onFeedOnOffSlt(bool onOff);
    void onVavleOnOffSlt(bool onOff);

    void on_simulateBtn_clicked();
    
    void on_backBtn_clicked();

    void on_moreBtn_clicked();

    void on_moreBtn_2_clicked();

    void on_vavletest_clicked();

    void on_imgPageCut_clicked();
    void on_ipcClassBtn_clicked();

private:
    Ui::BottomWidget *ui;
    QVector<int> m_showSimulatePageId;
    bool m_showSimulate;
    bool m_systemState;
    int ipcRstartStat;
};

#endif // BOTTOMWIDGET_H
