#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QLineEdit>
#include "basewidget.h"
#include "myinputdialog.h"
class myLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit myLineEdit(QWidget *parent = 0);
    explicit myLineEdit(QString text, QRect rect,QWidget *parent = 0);
    explicit myLineEdit(EchoMode mode, QRect rect,QWidget *parent = 0);

    void setMinMax(AbstractInterface* parent, qint32 min, qint32 max, qint32 editIndex, qint32* pVariable);
    void setFloatMinMax(AbstractInterface* parent, float min, float max, qint32 editIndex, float* pVariable,bool bPercent = true);
    void setFloatMinMax2(AbstractInterface* parent, float min, float max, qint32 editIndex, float* pVariable,inputType type);
    void setMax(qint32 max);
    void setMin(qint32 min);
    void setHint(qint32 m_hint);
    void setVariable(qint32* pVariable);



    void setType(inputType type);
    qint32 getMax();
    qint32 getMin();
    qint32 getHint();
    void setLocation();

protected:
    void mousePressEvent(QMouseEvent *event);
signals:
    void valueSetSignal();
private:
     qint32 m_max, m_min, m_IndexNum;
     float m_maxF,m_minF;
     qint32 m_hint;
     qint32* m_pVariable;
     float *m_pVariableF;
    AbstractInterface *m_pParent;
    inputType m_inputType;
};

#endif // MYLINEEDIT_H
