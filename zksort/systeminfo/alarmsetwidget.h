#ifndef ALARMSETWIDGET_H
#define ALARMSETWIDGET_H

#include "basewidget.h"
#include <QCheckBox>

#define CHECKBOXCOUNT 7
namespace Ui {
class AlarmSetWidget;
}

class AlarmSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit AlarmSetWidget(QWidget *parent = 0);
    ~AlarmSetWidget();
    void showPage(bool bshow);
    void retranslateUiWidget();

    void updateUi();

protected slots:
    void updateCheckBoxStatus(bool);

private slots:
    void on_clearRunTime_clicked();

private:
    Ui::AlarmSetWidget *ui;
    QCheckBox *cBox[CHECKBOXCOUNT];

    QMap<QCheckBox*,int> cBoxMap;


//    QGridLayout *m_gridLayout;
};

#endif // ALARMSETWIDGET_H
