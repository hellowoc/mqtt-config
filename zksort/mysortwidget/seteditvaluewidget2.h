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
    void setFloatParams(AbstractInterface *parent,float min,float max,int index, int bitCount, float *valueAddr);
    void setValue(float value);
    void setValue(float value, int bitCount);
    void setValue(int value);
    void setValueDecimalBitCount(int bitCount);
    void setTitle(QString titleText);
    void setTitle(QString titleText, int locationIndex );
    void receiveMsg(int msg1, int msg2, int msg3);
private slots:
    void on_subBtn_released();
    void on_subBtn_pressed();

    void sub_timeout();
    void add_timeout();

    void on_addBtn_pressed();
    void on_addBtn_released();

private:
    Ui::SetEditValueWidget *ui;
    AbstractInterface *m_pParent;
    int m_valueInt,m_minInt,m_maxInt,m_widgetIndex;
    float m_valueFloat,m_minFloat,m_maxFloat;
    int *m_valueAddr;
    int m_bitCount;     //小数位数
    float m_bitStep;    //小数的步长
    float *m_FloatvalueAddr;
    QTimer *m_pTimer1,*m_pTimer2;
    bool m_bPressed;
    void sendParams();
    ValueType m_type;
};

#endif // SETEDITVALUEWIDGET_H
