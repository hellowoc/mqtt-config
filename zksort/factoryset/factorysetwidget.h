#ifndef FACTORYSETWIDGET_H
#define FACTORYSETWIDGET_H

#include "basewidget.h"

namespace Ui {
class FactorySetWidget;
}

class FactorySetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit FactorySetWidget(QWidget *parent = 0);
    ~FactorySetWidget();
    void showPage(bool bshow = true);
protected:
    virtual void retranslateUiWidget();
private slots:
    void toolBtnClick();
    void buildGridUi();
    void clearLayout(QLayout *_layout);

private:
    Ui::FactorySetWidget *ui;
    QList<ItemCell> m_itemList;
    QGridLayout *m_gridLayout;
    void onUpdateMainClicked();
    void onFpgaUpgradeClicked();
};

#endif // FACTORYSETWIDGET_H
