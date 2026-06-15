#ifndef FEEDSETBYGROUPPAGE_H
#define FEEDSETBYGROUPPAGE_H

#include "basewidget.h"
#include "UiHorizontalProgressExt.h"
#include <QLabel>
namespace Ui {
class FeedSetByGroupPage;
}

class FeedSetByGroupPage : public basewidget
{
    Q_OBJECT

public:
    explicit FeedSetByGroupPage(QWidget *parent = 0);
    ~FeedSetByGroupPage();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
    void OnLayerChange();
public slots:
    void onFeedValueChanged(int value);
protected:
    void retranslateUiWidget();

private:
    Ui::FeedSetByGroupPage *ui;

    UiHorizontalProgressExt*  m_pFeedProgress[MAX_GROUP_TICK];
    QLabel *m_pFeedNameLbl[MAX_GROUP_TICK];

    bool m_autoFeedEnable;
};

#endif // FEEDSETBYGROUPPAGE_H
