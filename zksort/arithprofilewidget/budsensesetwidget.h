#ifndef BUDSENSESETWIDGET_H
#define BUDSENSESETWIDGET_H

#include "basewidget.h"
#include "mylineedit.h"
#include <QLabel>

namespace Ui {
class BudSenseSetWidget;
}

class BudSenseSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit BudSenseSetWidget(QWidget *parent = 0);
    ~BudSenseSetWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);

protected:
    void retranslateUiWidget();

private:
    Ui::BudSenseSetWidget *ui;
    void updateChuteState();
    myLineEdit *m_pChuteSenseEdit[2][MAX_VIEW_UNIT_USE];
    QLabel *m_pChuteTitle[MAX_VIEW_UNIT_USE];
    float m_ChuteSensFloat[2][MAX_VIEW_UNIT_USE];
    QVector<QHBoxLayout*> m_hlayout;
    QVector<myLineEdit*> m_chuteEdit;
    QVector<QLabel*> m_chuteLabel;

    int m_sizeInt;
    int m_limitInt;
    int m_blackSensInt;
    int m_yellowSensInt;

public slots:
    void sNameChanged();

};

#endif // BUDSENSESETWIDGET_H
