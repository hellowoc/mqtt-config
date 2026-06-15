#ifndef UiAbstractProgress_H
#define UiAbstractProgress_H
#include <QtGui>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOptionButton>
class UiAbstractProgress : public QWidget
{
    Q_OBJECT
public:
    explicit UiAbstractProgress(QWidget *parent = 0);
    virtual void onEvent(int x,int y) = 0;
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

    virtual void dragMoveEvent(QDragMoveEvent *event);
    virtual void dragLeaveEvent(QDragLeaveEvent *event);



    void setBackgroundColor(const QColor &color=Qt::white);
    void setFrontgroundColor(const QColor &color=Qt::yellow);
    void setFontColor(const QColor &color=Qt::black);


    int getCurrentValue() const;
    void setCurrentValue(int value);


    void setFontSize(int fontSize);

    void sendChangeStep(int step);

    float getMinValue() const;
    void setMinValue(float value);
    int getMaxValue() const;
    void setMaxValue(int value);

    void setMaxAndMinValue(int max,int min);
    void setFloatMaxAndMinValue(float max,float min);

signals:
    void onValueChange(int value);
    void onUpdateValueChange(int value);
public slots:

protected:
    float currentValue;//范围0 - 100 的float，表示百分比
    float maxValue;
    float minValue;

    int m_tmp_x;
    int m_tmp_y;
    float m_step;

    QColor m_bgColor;
    QColor m_fgColor;
    QColor m_fontColor;

    int m_fontSize;

};

#endif // UiAbstractProgress_H
