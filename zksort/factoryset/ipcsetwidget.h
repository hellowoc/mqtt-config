#ifndef IPCSETWIDGET_H
#define IPCSETWIDGET_H

#include "basewidget.h"
namespace Ui {
class IpcSetWidget;
}

class IpcSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit IpcSetWidget(QWidget *parent = 0);
    ~IpcSetWidget();
    void showPage(bool bshow = true);
protected:
    virtual void retranslateUiWidget();
private slots:
    void toolBtnClick();
    void buildGridUi();
    void clearLayout(QLayout *_layout);

private:
    Ui::IpcSetWidget *ui;
    QList<ItemCell> m_itemList;
    QGridLayout *m_gridLayout;
};

#endif // IPCSETWIDGET_H
