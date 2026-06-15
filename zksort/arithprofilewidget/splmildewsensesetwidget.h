#ifndef SPLMILDEWSENSESETWIDGET_H
#define SPLMILDEWSENSESETWIDGET_H

#include "basewidget.h"
#include "mylineedit.h"
#include <QLabel>

namespace Ui {
class splMildewSenseSetWidget;
}

class splMildewSenseSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit splMildewSenseSetWidget(QWidget *parent = 0);
    ~splMildewSenseSetWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
protected:
    void retranslateUiWidget();
private:
    Ui::splMildewSenseSetWidget *ui;
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

#endif // SPLMILDEWSENSESETWIDGET_H
