#ifndef SETEDITVALUEWIDGET_H
#define SETEDITVALUEWIDGET_H

#include <QWidget>
#include "basewidget.h"
#include <QTimer>
enum ValueType{
    IntType = 0,
    floatType = 1
};

namespace Ui {
class SetEditValueWidget;
}

class SetEditValueWidget : public QWidget,public AbstractInterface
{
    Q_OBJECT

public:
    explicit SetEditValueWidget(QWidget *parent = 0);
    ~SetEditValueWidget();
    void setParams(AbstractInterface *parent,int min,int max,int index,int *valueAddr);
    void setFloatParams(AbstractInterface *parent,float min,float max,int index,float *valueAddr);
    void setValue(float value);
    void setValue(int value);
    void setTitle(QString titleText);
    void setTitle(QString titleText, int locationIndex );
    void receiveMsg(int msg1, int msg2, int msg3);
private slots:
    void on_subBtn_clicked();
    void on_addBtn_clicked();
private:
    Ui::SetEditValueWidget *ui;
    AbstractInterface *m_pParent;
    int m_valueInt,m_minInt,m_maxInt,m_widgetIndex;
    float m_valueFloat,m_minFloat,m_maxFloat;
    int *m_valueAddr;
    float *m_FloatvalueAddr;
    void sendParams();
    ValueType m_type;
};

#endif // SETEDITVALUEWIDGET_H
