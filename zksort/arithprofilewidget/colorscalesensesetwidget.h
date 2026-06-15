#ifndef COLORSCALESENSESETWIDGET_H
#define COLORSCALESENSESETWIDGET_H

#include "basewidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include "mylineedit.h"
#include <QButtonGroup>

namespace Ui {
class ColorScaleSenseSetWidget;
}

class ColorScaleSenseSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit ColorScaleSenseSetWidget(QWidget *parent = 0);
    ~ColorScaleSenseSetWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);

private:
    Ui::ColorScaleSenseSetWidget *ui;

    void updateValue();
    void updateChuteState();
    float m_senseValue[2][MAX_VIEW_UNIT_USE];
    myLineEdit *m_pChuteSenseEdit[2][MAX_VIEW_UNIT_USE];
    float m_ChuteSensFloat[2][MAX_VIEW_UNIT_USE];
    QLabel *m_pChuteTitle[MAX_VIEW_UNIT_USE];    int m_percentInt;//纯度
    int m_scaleLimit;//算法下限

protected:
    void retranslateUiWidget();

public slots:
    void sNameChanged();
    void on_comboBox_index_activated(int index);
};

#endif // COLORSCALESENSESETWIDGET_H
