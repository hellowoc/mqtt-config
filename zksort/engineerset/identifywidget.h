#ifndef IDENTIFYWIDGET_H
#define IDENTIFYWIDGET_H

#include "basewidget.h"
#include <QGridLayout>
#include "myarithui.h"
namespace Ui {
class IdentifyWidget;
}

class IdentifyWidget : public basewidget
{
    Q_OBJECT

public:
    explicit IdentifyWidget(QWidget *parent = 0);
    ~IdentifyWidget();
    void OnViewChange(int view);
    virtual void showPage(bool bshow);
    void OnGroupChange(int groupIndex);
    void OnLayerChange();
protected:
    virtual void retranslateUiWidget();
public slots:
    void arithWidgetSlots(bool checked,int arithType);
private slots:
    void on_comboBox_activated(int index);
    void getscreenshot();


private:
    Ui::IdentifyWidget *ui;
    QGridLayout *m_gridLayout;
    QVector<MyArithUi*> m_ArithUiVector;

    QMap<int,int> m_ArithWidgetMap;

    int m_preArithUiIndex,m_curArithUiIndex;
    void updateGroupWidgetStatus();
    void updateArithState();
    void updateGroupArithState();
    void updateBtnVisiable();
    void copySpliceDisplay();

              QTimer *shottimer;
};

#endif // IDENTIFYWIDGET_H
