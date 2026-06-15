#ifndef TOPWIDGET_H
#define TOPWIDGET_H

#include "baseui.h"
#define CALIBRATE_TIMES 5
namespace Ui {
class TopWidget;
}

class TopWidget : public BaseUi
{
    Q_OBJECT

public:
    explicit TopWidget(QWidget *parent = 0);
    ~TopWidget();
    void PageChanged(int pageId);
public:
    virtual void showPage(bool bshow);
protected:
    virtual void retranslateUiWidget();
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *e);
private slots:
    void on_levelBtn_clicked();

    void updateDateTime();

    void on_SysStateWidgetBtn_clicked();

    void on_saveBtn_clicked();

    void timeOut();

    void on_timeLabel_clicked();

    void on_pB_conveyor_clicked();

    
private:
    Ui::TopWidget *ui;
    QVector<int> m_levelPageId;
    QMap<int,QString> m_pageId_TitleMap;
    void reloadTitleMap();
    QTimer *m_pTimer;
    int m_calibrateTimes;
    QLabel *ipcstatus[9];
    void updateIpcStatus();
};

#endif // TOPWIDGET_H
