#ifndef DIVIDEPIXELWIDGET_H
#define DIVIDEPIXELWIDGET_H

#include "basewidget.h"
#include "mynetwork.h"
#include "mylineedit.h"

namespace Ui {
class DividePixelWidget;
}

class DividePixelWidget : public basewidget
{
    Q_OBJECT

public:
    explicit DividePixelWidget(QWidget *parent = 0);
    ~DividePixelWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
    void OnViewChange(int view);
    void OnChuteChange();
    void OnLayerChange();
protected:
    void retranslateUiWidget();
public slots:
    void updateWave();
private slots:
    void on_pushButton_5_clicked();

    void on_sendBtn_clicked();

    void on_checkBox_origin_clicked(bool checked);

    void on_checkBox_cali_clicked(bool checked);

    void on_checkBox_source_clicked(bool checked);

    void on_checkBox_stop_clicked(bool checked);

private:
    Ui::DividePixelWidget *ui;
    void updateValue();
    void updateUpdateCaliState();
    void updateAvgVisiable();
    void updateAvgValue();
    QLabel *m_labelAvg[4];

    bool m_showSourceData;
    bool m_showStop;
};

#endif // DIVIDEPIXELWIDGET_H
