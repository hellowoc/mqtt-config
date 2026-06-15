#ifndef PEPPERHANDLESENSEWIDGET_H
#define PEPPERHANDLESENSEWIDGET_H

#include "basewidget.h"
#include "mylineedit.h"

namespace Ui {
class PepperHandleSenseWidget;
}

class PepperHandleSenseWidget : public basewidget
{
    Q_OBJECT

public:
    explicit PepperHandleSenseWidget(QWidget *parent = 0);
    ~PepperHandleSenseWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
protected:
    void retranslateUiWidget();
private:
    Ui::PepperHandleSenseWidget *ui;
    void updateChuteState();
    myLineEdit *m_pChuteSenseEdit[2][MAX_VIEW_UNIT_USE];
    QLabel *m_pChuteTitle[MAX_VIEW_UNIT_USE];
    float m_ChuteSensFloat[2][MAX_VIEW_UNIT_USE];
    QVector<QHBoxLayout*> m_hlayout;
    QVector<myLineEdit*> m_chuteEdit;
    QVector<QLabel*> m_chuteLabel;

public slots:
    void sNameChanged();
};

#endif // PEPPERHANDLESENSEWIDGET_H
