#ifndef MYLIGHTCONTROL_H
#define MYLIGHTCONTROL_H

#include <QWidget>
#include "basewidget.h"
#include "globalflow.h"
#include "logger.h"
namespace Ui {
class MylightControl;
}

class MylightControl : public QWidget,public AbstractInterface
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
public:
    explicit MylightControl(QWidget *parent = 0);
    void initState(int interfaceindex,int boardindex,QString text,int value,int stateValue);

    void initSeq(int interfaceindex,int boardindex,int value);
    ~MylightControl();
    void receiveMsg(int msg1, int msg2, int msg3);
private slots:
    void on_checkBox_clicked(bool checked);

private:
    Ui::MylightControl *ui;
    int m_portIndex;
    int m_boardIndex;
    void updateBtnText();
};

#endif // MYLIGHTCONTROL_H
