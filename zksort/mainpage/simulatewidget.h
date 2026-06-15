#ifndef SIMULATEWIDGET_H
#define SIMULATEWIDGET_H

#include "basewidget.h"
class QListWidgetItem;
namespace Ui {
class SimulateWidget;
}

class SimulateWidget : public basewidget
{
    Q_OBJECT

public:
    explicit SimulateWidget(QWidget *parent = 0);
    ~SimulateWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
    void retranslateUiWidget();
private slots:
    void imageListItemSelect(int row);
private:
    Ui::SimulateWidget *ui;
    void updateImageList();
    int m_imageIndex;
    void updateCurrentImage(int row ,bool show = false);
};

#endif // SIMULATEWIDGET_H
