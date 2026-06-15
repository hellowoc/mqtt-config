#ifndef FEEDSETBYCHUTEPAGE_H
#define FEEDSETBYCHUTEPAGE_H

#include "basewidget.h"
#include "seteditvaluewidget.h"
namespace Ui {
class FeedSetByChutePage;
}

class FeedSetByChutePage : public basewidget
{
    Q_OBJECT

public:
    explicit FeedSetByChutePage(QWidget *parent = 0);
    ~FeedSetByChutePage();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
    void OnLayerChange();
protected:
    void retranslateUiWidget();

private slots:
    void on_conveyorBtn_clicked();
    void updateFeedValue();

private:
    Ui::FeedSetByChutePage *ui;
    SetEditValueWidget* m_pFeedEdit[MAX_VIEW_UNIT_USE];
    int m_FeedValue[MAX_VIEW_UNIT_USE];
    void updateConveyor();
};

#endif // FEEDSETBYCHUTEPAGE_H
