#ifndef WHEATSPROUTSENSESETWIDGET_H
#define WHEATSPROUTSENSESETWIDGET_H

#include "basewidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include "mylineedit.h"
#include <QButtonGroup>

namespace Ui {
class WheatSproutSenseSetWidget;
}

class WheatSproutSenseSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit WheatSproutSenseSetWidget(QWidget *parent = 0);
    ~WheatSproutSenseSetWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);

private:
    Ui::WheatSproutSenseSetWidget *ui;

    void updateValue();
    void updateChuteState();
    float m_senseValue[2][MAX_VIEW_UNIT_USE];
    myLineEdit *m_pChuteSenseEdit[2][MAX_VIEW_UNIT_USE];
    float m_ChuteSensFloat[2][MAX_VIEW_UNIT_USE];
    QLabel *m_pChuteTitle[MAX_VIEW_UNIT_USE];    int m_percentInt;//纯度
    int m_low,m_percent;
    float m_h0,m_h1;

protected:
    void retranslateUiWidget();

public slots:
    void sNameChanged();
    void hColorChanged();
};

#endif // WHEATSPROUTSENSESETWIDGET_H
