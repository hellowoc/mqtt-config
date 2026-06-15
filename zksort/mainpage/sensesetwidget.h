#ifndef SENSESETWIDGET_H
#define SENSESETWIDGET_H

#include "basewidget.h"
#include "UiHorizontalProgressExt.h"
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#define UI_ARITH_COUNT 21
namespace Ui {
class SenseSetWidget;
}

class SenseSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit SenseSetWidget(QWidget *parent = 0);
    ~SenseSetWidget();
    void showPage(bool bshow = true);
    void OnViewChange(int view);
    void OnGroupChange(int groupIndex);
    void OnLayerChange();
public slots:
    void onSenseValueChanged(int value);
    void onUseBtnClicked(bool checked);
    void onMoreSetBtnClicked(int index);
    void onRatioValueChanged(int value);

    void getscreenshot();
protected:
    void retranslateUiWidget();
private:
    Ui::SenseSetWidget *ui;
    QList<SingleSenseData> m_SenseDataList;

    UiHorizontalProgressExt*  m_pHoriSenseProgress[UI_ARITH_COUNT];
    QCheckBox *m_pUsedBtn[UI_ARITH_COUNT];
    QPushButton *m_pMoreSetBtn[UI_ARITH_COUNT];
    QLabel *m_pSenseNameLabel[UI_ARITH_COUNT];
    QLabel *m_pViewNameLabel[UI_ARITH_COUNT];

    QHBoxLayout *m_pSenseHLayout[UI_ARITH_COUNT];
    void initSense();
    void loadSenseData();
    void loadSenseDataSplice();
    int mOldHighHue;
    int mOldLowHue;
    void updateSenseByRunMode();

    QSignalMapper *m_pMoreSetBtnsignalMapper;
    void updateUseBtnText(int index);

              QTimer *shottimer;
};

#endif // SENSESETWIDGET_H
