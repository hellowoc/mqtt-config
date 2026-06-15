#ifndef CIRCLELONGSENSESETWIDGET_H
#define CIRCLELONGSENSESETWIDGET_H

#include "basewidget.h"
#include "mylineedit.h"
#include <QLabel>
#include <QButtonGroup>
namespace Ui {
class CircleLongSenseSetWidget;
}

class CircleLongSenseSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit CircleLongSenseSetWidget(QWidget *parent = 0);
    ~CircleLongSenseSetWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
protected:
    void retranslateUiWidget();
private:
    Ui::CircleLongSenseSetWidget *ui;
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
    void on_comboBox_delay_activated(int index);
};

#endif // CIRCLELONGSENSESETWIDGET_H
