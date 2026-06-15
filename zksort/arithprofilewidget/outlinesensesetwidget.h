#ifndef OUTLINESENSESETWIDGET_H
#define OUTLINESENSESETWIDGET_H

#include "basewidget.h"
#include "mylineedit.h"
#include <QLabel>

namespace Ui {
class OutlineSenseSetWidget;
}

class OutlineSenseSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit OutlineSenseSetWidget(QWidget *parent = 0);
    ~OutlineSenseSetWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
protected:
    void retranslateUiWidget();

private:
    Ui::OutlineSenseSetWidget *ui;
    void updateChuteState();
    myLineEdit *m_pChuteSenseEdit[2][MAX_VIEW_UNIT_USE];
    QLabel *m_pChuteTitle[MAX_VIEW_UNIT_USE];
    float m_ChuteSensFloat[2][MAX_VIEW_UNIT_USE];
    QVector<QHBoxLayout*> m_hlayout;
    QVector<myLineEdit*> m_chuteEdit;
    QVector<QLabel*> m_chuteLabel;

    void updateModeBtnState();

    int m_percentInt;
    int m_highLimit;
    int m_lowLimit;

public slots:
    void sNameChanged();
};

#endif // OUTLINESENSESETWIDGET_H
