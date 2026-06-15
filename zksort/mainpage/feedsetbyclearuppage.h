#ifndef FEEDSETBYCLEARUPPAGE_H
#define FEEDSETBYCLEARUPPAGE_H

#include "basewidget.h"
#include "seteditvaluewidget.h"
namespace Ui {
class FeedSetByClearUpPage;
}

class FeedSetByClearUpPage : public basewidget
{
    Q_OBJECT

public:
    explicit FeedSetByClearUpPage(QWidget *parent = 0);
    ~FeedSetByClearUpPage();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
protected:
    void retranslateUiWidget();

private slots:


private:
    Ui::FeedSetByClearUpPage *ui;
};

#endif // FEEDSETBYCLEARUPPAGE_H
