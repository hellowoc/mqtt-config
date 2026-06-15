#ifndef SUPPRESSLARGESENSESETWIDGET_H
#define SUPPRESSLARGESENSESETWIDGET_H

#include "basewidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include "mylineedit.h"
#include <QButtonGroup>

namespace Ui {
class SuppressLargeSenseSetWidget;
}

class SuppressLargeSenseSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit SuppressLargeSenseSetWidget(QWidget *parent = 0);
    ~SuppressLargeSenseSetWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);

private:
    Ui::SuppressLargeSenseSetWidget *ui;

    void updateValue();
    void updateChuteState();
    float m_senseValue[2][MAX_VIEW_UNIT_USE];
    myLineEdit *m_pChuteSenseEdit[2][MAX_VIEW_UNIT_USE];
    float m_ChuteSensFloat[2][MAX_VIEW_UNIT_USE];
    QLabel *m_pChuteTitle[MAX_VIEW_UNIT_USE];    int m_percentInt;//纯度
    int m_size1,m_size2,m_percent1,m_percent2;

protected:
    void retranslateUiWidget();

public slots:
    void sNameChanged();
    void on_comboBox_index_activated(int index);
private slots:
    void on_comboBox_circle_activated(int index);
};

#endif // SUPPRESSLARGESENSESETWIDGET_H
