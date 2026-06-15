#ifndef BASEUI_H
#define BASEUI_H

#include <QObject>
#include <QWidget>
#include "logger.h"
#include "mywid.h"
#include "globalparams.h"
class BaseUi : public QWidget
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
public:
    explicit BaseUi(QWidget *parent = 0);
    virtual void retranslateUiWidget() = 0;
signals:

public slots:

protected:
    void changeEvent(QEvent *);
    void paintEvent(QPaintEvent *);//通过stylesheet 设置 background相关样式时需要做的处理
public:
    virtual void showPage(bool bshow = true) = 0;
};

#endif // BASEUI_H
