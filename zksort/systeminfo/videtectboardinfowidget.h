#ifndef VIDETECTBOARDINFOWIDGET_H
#define VIDETECTBOARDINFOWIDGET_H

#include "basewidget.h"
#include <QComboBox>
#include "mylineedit.h"
#include <QLabel>

namespace Ui {
class VIDetectBoardInfoWidget;
}

class VIDetectBoardInfoWidget : public basewidget
{
    Q_OBJECT

    enum{
        CONST_MAX_VIBOARDINFO_PER_PAGE = 10
    };

public:
    explicit VIDetectBoardInfoWidget(QWidget *parent = 0);
    ~VIDetectBoardInfoWidget();
    void showPage(bool bshow);
    void receiveMsg(int msg1, int msg2, int msg3);
    void updateAll();
    void updateVIInfo();

protected:
    virtual void retranslateUiWidget();

private slots:
    void viBoardAddrActivated(int );
    void viBoardTypeActivated(int );

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_checkBox_clicked(bool checked);

    void updateVIData();

private:
    Ui::VIDetectBoardInfoWidget *ui;
    QLabel *m_viboardInfoLabel[CONST_MAX_VIBOARDINFO_PER_PAGE][3];//名称-电压-电流
    QComboBox *m_viboardInfoCombox[CONST_MAX_VIBOARDINFO_PER_PAGE][2];//地址-类型

    QSignalMapper *m_pVIBoardAddrSigMapper;
    QSignalMapper *m_pVIBoardTypeSigMapper;

    int m_curPage;
    int m_toalPage;
};

#endif // VIDETECTBOARDINFOWIDGET_H
