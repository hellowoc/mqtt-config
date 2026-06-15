#ifndef BIGSMALLSENSESETWIDGET_H
#define BIGSMALLSENSESETWIDGET_H

#include "basewidget.h"
#include "mylineedit.h"
#include <QLabel>
#include <QButtonGroup>
namespace Ui {
class BigSmallSenseSetWidget;
}

class BigSmallSenseSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit BigSmallSenseSetWidget(QWidget *parent = 0);
    ~BigSmallSenseSetWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
protected:
    void retranslateUiWidget();
private:
    Ui::BigSmallSenseSetWidget *ui;
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
    void updateValue();
public slots:
    void modeBtnClicked(bool);
    void sNameChanged();
    void setBigSmallLimit();
    void on_comboBox_delay_activated(int index);
};

#endif // BIGSMALLSENSESETWIDGET_H
