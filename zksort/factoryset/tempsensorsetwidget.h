#ifndef TEMPSENSORSETWIDGET_H
#define TEMPSENSORSETWIDGET_H

#include "basewidget.h"
#include <QButtonGroup>
#include <QSignalMapper>

class myLineEdit;
namespace Ui {
class TempSensorSetWidget;
}

class TempSensorSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit TempSensorSetWidget(QWidget *parent = 0);
    ~TempSensorSetWidget();
    void showPage(bool bshow);
    void receiveMsg(int msg1, int msg2, int msg3);

protected:
    virtual void retranslateUiWidget();
public slots:
    void checkBtnClicked(int index);
    void sNameChanged();
private:
    Ui::TempSensorSetWidget *ui;
    void updateWidget();
    myLineEdit *m_pSensorEdit[CTRL_ALARM_MAX_TEMP_SENSOR];
    myLineEdit *m_pSensorNameEdit[CTRL_ALARM_MAX_TEMP_SENSOR];
    QButtonGroup *m_pBtnGroup;
    QSignalMapper *m_pSignalMap;
};

#endif // TEMPSENSORSETWIDGET_H
