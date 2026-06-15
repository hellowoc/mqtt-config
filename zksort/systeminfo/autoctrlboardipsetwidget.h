#ifndef AUTOCTRLBOARDIPSETWIDGET_H
#define AUTOCTRLBOARDIPSETWIDGET_H

#include "basewidget.h"

namespace Ui {
class AutoCtrlBoardIPSetWidget;
}


class AutoCtrlBoardIPSetWidget : public basewidget
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    explicit AutoCtrlBoardIPSetWidget(QWidget *parent = 0);
    ~AutoCtrlBoardIPSetWidget();
    void showPage(bool bshow);
    void retranslateUiWidget();
private slots:

    void on_IpqueryBtn_clicked();

    void on_IpSetBtn_clicked();

    void updateSlt();

    void on_ModbusSetBtn_clicked();

    void on_BaudRateCmbox_activated(int index);
private:
    Ui::AutoCtrlBoardIPSetWidget *ui;
    void initUi();
    void updateUi();
    QString ipStr1;

    int m_nCtrlBoardPort;                          //

    int m_nModbusIP;                                //
    int m_nBaudRate;                                //
};


#endif // AUTOCTRLBOARDIPSETWIDGET_H

