#ifndef FEEDBANLANCEAIMSINGLE_H
#define FEEDBANLANCEAIMSINGLE_H

#include "basewidget.h"
#include "seteditvaluewidget.h"
namespace Ui {
class FeedBanlanceAimSingle;
}

class FeedBanlanceAimSingle : public basewidget
{
    Q_OBJECT

public:
    explicit FeedBanlanceAimSingle(QWidget *parent = 0);
    ~FeedBanlanceAimSingle();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
    void OnLayerChange();
protected:
    void retranslateUiWidget();

private slots:


private:
    Ui::FeedBanlanceAimSingle *ui;
    SetEditValueWidget* m_pFeedEdit[MAX_VIEW_UNIT_USE];
    int m_FeedValue[MAX_VIEW_UNIT_USE];
    void updateFeedValue();
};

#endif // FEEDBANLANCEAIMSINGLE_H
