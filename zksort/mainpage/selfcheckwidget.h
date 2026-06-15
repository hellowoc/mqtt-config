#ifndef SELFCHECKWIDGET_H
#define SELFCHECKWIDGET_H

#include "basewidget.h"

namespace Ui {
class SelfCheckWidget;
}

class SelfCheckWidget : public basewidget
{
    Q_OBJECT

public:
    explicit SelfCheckWidget(QWidget *parent = 0);
    ~SelfCheckWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
protected:
    void retranslateUiWidget();
private slots:
    void on_enterMainPageBtn_clicked();

    void on_reCheck_clicked();

private:
    Ui::SelfCheckWidget *ui;
    void showCheckInfo();
    void updateCheckInfo();
    QStringList m_infoList;
    QVector<int> m_infoStateVec;
};

#endif // SELFCHECKWIDGET_H
