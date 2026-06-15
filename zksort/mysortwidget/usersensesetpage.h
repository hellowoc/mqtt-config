#ifndef USERSENSESETPAGE_H
#define USERSENSESETPAGE_H

#include <QWidget>
#include "UiVerticalProgressExt.h"
#define MAX_NUM_HISTOGRAM 3
#include <QLabel>

namespace Ui {
class UserSenseSetPage;
}
class UserWidget;
class UserSenseSetPage : public QWidget
{
    Q_OBJECT

public:
    explicit UserSenseSetPage(int pageNo, QWidget* container, QWidget *parent = 0);
    ~UserSenseSetPage();


    void InitUiPage();      //初始化柱状图界面

private slots:
    void onSenseValueChange(int value);

private:
    UserWidget*  m_Attach;
private:
    Ui::UserSenseSetPage *ui;
    int m_pageNo;       //该页面所在的序号，从0开始
    int m_count;        //该页面的算法数量

    UiVerticalProgressExt*  m_Histogram[MAX_NUM_HISTOGRAM];
    QHBoxLayout*    m_layout[MAX_NUM_HISTOGRAM];
    QLabel* m_label[MAX_NUM_HISTOGRAM];
};

#endif // USERSENSESETPAGE_H
