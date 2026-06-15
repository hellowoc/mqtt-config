#ifndef LINESENSESETWIDGET_H
#define LINESENSESETWIDGET_H

#include "basewidget.h"
#include "mylineedit.h"
#include <QLabel>

namespace Ui {
class LineSenseSetWidget;
}

class LineSenseSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit LineSenseSetWidget(QWidget *parent = 0);
    ~LineSenseSetWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);

protected:
    void retranslateUiWidget();

private:
    Ui::LineSenseSetWidget *ui;
    void updateChuteState();
    myLineEdit *m_pChuteSenseEdit[2][MAX_VIEW_UNIT_USE];
    QLabel *m_pChuteTitle[MAX_VIEW_UNIT_USE];
    float m_ChuteSensFloat[2][MAX_VIEW_UNIT_USE];
    QVector<QHBoxLayout*> m_hlayout;
    QVector<myLineEdit*> m_chuteEdit;
    QVector<QLabel*> m_chuteLabel;

    void updateSizeState();
    void updateColorState();
    int m_rowInt;//行
    int m_percentInt;//纯度
    int m_senseInt;

public slots:
    void sNameChanged();
private slots:
    void on_colorComboBox_activated(int index);
};

#endif // LINESENSESETWIDGET_H
