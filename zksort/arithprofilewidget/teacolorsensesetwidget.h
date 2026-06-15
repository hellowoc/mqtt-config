#ifndef TEACOLORSENSESETWIDGET_H
#define TEACOLORSENSESETWIDGET_H

#include "basewidget.h"

namespace Ui {
class TeaColorSenseSetWidget;
}

class TeaColorSenseSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit TeaColorSenseSetWidget(QWidget *parent = 0);
    ~TeaColorSenseSetWidget();

    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);


protected:
    void retranslateUiWidget();

private:
    void updateValue();
    QButtonGroup *m_pModeBtnGroup;

    void setViewVisibleByArith();
    void onModeChange(int mode);
    void updateSizeState();
    int m_rowInt;//行
    int m_percentInt;//纯度
    float m_percentResInt;
    int m_diffResSensInt;
    float m_diffResSensFloat;
    int m_grayResSensInt;

    void updateModeBtnState();
    private slots:
    void modeBtnClicked(bool);
    void sNameChanged();
    void on_checkBoxInhebit_clicked();

private:
    Ui::TeaColorSenseSetWidget *ui;
};

#endif // TEACOLORSENSESETWIDGET_H
