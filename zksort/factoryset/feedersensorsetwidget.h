#ifndef FEEDERSENSORSETWIDGET_H
#define FEEDERSENSORSETWIDGET_H

#include "basewidget.h"
#include <QButtonGroup>
#include <QSignalMapper>
namespace Ui {
class FeederSensorSetWidget;
}
class myLineEdit;
class FeederSensorSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit FeederSensorSetWidget(QWidget *parent = 0);
    ~FeederSensorSetWidget();
    void showPage(bool bshow);
    void receiveMsg(int msg1, int msg2, int msg3);
public slots:
    void checkboxClicked(int index);
    void checkboxPosiClicked(int index);
    void sNameChanged();
protected:
    virtual void retranslateUiWidget();
private:
    Ui::FeederSensorSetWidget *ui;
    void updateFeederSensorCheckboxState();
    QButtonGroup *m_pBtnGroup;
    QSignalMapper *m_pSignalMap;
    QButtonGroup *m_pBtnGroupPosi;
    QSignalMapper *m_pSignalMapPosi;
    myLineEdit *m_pFeederNameEdit[CTRL_ALARM_MAX_FEED_SENSOR];
};

#endif // FEEDERSENSORSETWIDGET_H
