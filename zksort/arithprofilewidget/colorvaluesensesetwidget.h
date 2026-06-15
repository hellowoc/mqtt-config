#ifndef COLORVALUESENSESETWIDGET_H
#define COLORVALUESENSESETWIDGET_H

#include "basewidget.h"
#include "mylineedit.h"
#include <QLabel>

namespace Ui {
class ColorValueSenseSetWidget;
}

class ColorValueSenseSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit ColorValueSenseSetWidget(QWidget *parent = 0);
    ~ColorValueSenseSetWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
protected:
    void retranslateUiWidget();
private:
    Ui::ColorValueSenseSetWidget *ui;
    void updateChuteState();
    void updateSizeState();
    QButtonGroup *m_pModeBtnGroup;
    myLineEdit *m_pChuteSenseEdit[2][MAX_VIEW_UNIT_USE];
    QLabel *m_pChuteTitle[MAX_VIEW_UNIT_USE];
    float m_ChuteSensFloat[2][MAX_VIEW_UNIT_USE];
    QVector<QHBoxLayout*> m_hlayout;
    QVector<myLineEdit*> m_chuteEdit;
    QVector<QLabel*> m_chuteLabel;

    int m_rowInt;//行
    int m_percentInt;//纯度

    void onModeChange(int mode);
    void updateModeBtnState();
public slots:
    void modeBtnClicked(bool);
    void sNameChanged();
};

#endif // COLORVALUESENSESETWIDGET_H
