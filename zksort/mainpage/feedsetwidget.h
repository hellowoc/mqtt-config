#ifndef FEEDSETWIDGET_H
#define FEEDSETWIDGET_H

#include "basewidget.h"
#include "seteditvaluewidget.h"
#include <QButtonGroup>
namespace Ui {
class FeedSetWidget;
}

class FeedSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit FeedSetWidget(QWidget *parent = 0);
    ~FeedSetWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
    void OnLayerChange();
public slots:
    void ctrlBtnClickedSlots();
    void feedTypebtnClicked(bool check);
protected:
    void retranslateUiWidget();
    void updateStackwidget();
private slots:
    void on_adjustBtn_clicked();

    void on_clearfeedcheckBox_clicked();

private:
    Ui::FeedSetWidget *ui;

    int m_ctrlBoardIndex;
    QPushButton *m_pctrlBoardBtn[2];
    QButtonGroup *m_pfeedTypebtnGroup;
};

#endif // FEEDSETWIDGET_H
