#ifndef USERWIDGET_H
#define USERWIDGET_H

#include "basewidget.h"
#include "usersensesetpage.h"


namespace Ui {
class UserWidget;
}

class UserWidget : public basewidget
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
public:
    explicit UserWidget(QWidget *parent = 0);
    ~UserWidget();
    void showPage(bool bshow = true);
    void OnGroupChange(int groupIndex);
     QList<SingleSenseData> m_singleSenseDataVec;
protected:
    void retranslateUiWidget();
private slots:

    void updatePreNextBtnState();
    void on_preBtn_clicked();

    void on_nextBtn_clicked();
    void on_comboBox_activated(int index);

private:
    Ui::UserWidget *ui;
    UserSenseSetPage *m_pSensePage[3];
    void initSenseSetStackedWidget();
    void loadSenseData();
    int m_pageCount;

};

#endif // USERWIDGET_H
