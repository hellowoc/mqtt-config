#ifndef UiPageControl_H
#define UiPageControl_H
#include "uikits_shared.h"
#include <QWidget>
#include <QPushButton>
#include <QList>
#include <QHBoxLayout>
class UIKITS_SHARED UiPageControl : public QWidget
{
    Q_OBJECT
public:
    explicit UiPageControl(QWidget *parent = 0);
    void setPageNums(int pageNums);
    void setCurrentPageIndex(int pageIndex);
    int pageCount();

signals:
    void clickedPageControl(int clickPageIndex);
public slots:
    void pageControlClick();
protected:
    void initPageControlStyle();
private:
    int n_pageNums;
    int n_pageIndex;
    QList<QPushButton *> list_buttons;
    QString m_style;
    QString m_style_selected;
    QHBoxLayout *layout;

};

#endif // UiPageControl_H
