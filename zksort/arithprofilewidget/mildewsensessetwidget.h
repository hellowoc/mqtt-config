#ifndef MILDEWSENSESSETWIDGET_H
#define MILDEWSENSESSETWIDGET_H

#include "basewidget.h"
#include "mylineedit.h"
#include <QLabel>
namespace Ui {
class MildewSensesSetWidget;
}

class MildewSensesSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit MildewSensesSetWidget(QWidget *parent = 0);
    ~MildewSensesSetWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
protected:
    void retranslateUiWidget();

private:
    Ui::MildewSensesSetWidget *ui;
    void updateChuteState();
    void updateLimit();
    QButtonGroup *m_pModeBtnGroup;
    QButtonGroup *m_pTypeBtnGroup;
    myLineEdit *m_pChuteSenseEdit[2][MAX_VIEW_UNIT_USE];
    QLabel *m_pChuteTitle[MAX_VIEW_UNIT_USE];
    float m_ChuteSensFloat[2][MAX_VIEW_UNIT_USE];
    QVector<QHBoxLayout*> m_hlayout;
    QVector<myLineEdit*> m_chuteEdit;
    QVector<QLabel*> m_chuteLabel;

    void onModeChange(int mode);
    void updateModeBtnState();

    void onTypeChange(int type);
    void updateTypeBtnState();

    int m_percentInt;   // 纯度
    int m_limitInt;     // 限值

public slots:
    void modeBtnClicked(bool);
    void typeBtnClicked(bool);
    void sNameChanged();
};

#endif // MILDEWSENSESSETWIDGET_H
