#ifndef BASEARITHWIDGET_H
#define BASEARITHWIDGET_H
#include "baseui.h"
#include <QButtonGroup>
#include <QPushButton>
class BaseArithWidget : public BaseUi
{
    Q_OBJECT
public:
    explicit BaseArithWidget(QWidget *parent = 0);
    virtual ~BaseArithWidget();
    virtual void setArithType(int type);

    virtual void onModeChange(int mode) = 0;
public:
    int m_arithType;
    QButtonGroup *m_pModeBtnGroup;
    void connectSlots();
public slots:
    void modeBtnClicked(bool checked);
    void modePressed();
};

#endif // BASEARITHWIDGET_H
