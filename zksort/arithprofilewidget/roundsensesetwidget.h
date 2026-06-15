#ifndef ROUNDSENSESETWIDGET_H
#define ROUNDSENSESETWIDGET_H

#include "basewidget.h"
#include "mylineedit.h"
#include <QLabel>
namespace Ui {
class RoundSenseSetWidget;
}

class RoundSenseSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit RoundSenseSetWidget(QWidget *parent = 0);
    ~RoundSenseSetWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
protected:
    void retranslateUiWidget();
private:
    Ui::RoundSenseSetWidget *ui;
    void updateChuteState();
    QButtonGroup *m_pModeBtnGroup;
    myLineEdit *m_pChuteSenseEdit[2][MAX_VIEW_UNIT_USE];
    QLabel *m_pChuteTitle[MAX_VIEW_UNIT_USE];
    float m_ChuteSensFloat[2][MAX_VIEW_UNIT_USE];
    QVector<QHBoxLayout*> m_hlayout;
    QVector<myLineEdit*> m_chuteEdit;
    QVector<QLabel*> m_chuteLabel;

    void onModeChange(int mode);
    void updateModeBtnState();
public slots:
    void modeBtnClicked(bool);
    void sNameChanged();
};

#endif // ROUNDSENSESETWIDGET_H
