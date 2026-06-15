#ifndef UiAbstractProgressExt_H
#define UiAbstractProgressExt_H
#include "uikits_shared.h"
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include "UiAbstractProgress.h"
#define DEFULT_BUTTON_STYLE "\
QPushButton{\
    color:black;\
    font:50px bold;\
    padding:0px;\
    min-width:50px; \
    max-width:50px; \
    min-height:50px; \
    max-height:50px;\
    background-color:white;\
}\
QPushButton::pressed{\
    color:#eeeeee;\
    background-color:rgb(51,153,255,155);\
} \
"
extern const int DEFALUE_PROGRESS_WIDTH;//defalut progress width
extern const int DEFAULT_BUTTON_WIDTH ;//default button width
extern const int VIEW_DIVIDE;
extern const float F_RATIO ;

class UIKITS_SHARED UiAbstractProgressExt : public QWidget
{
    Q_OBJECT
public:
    explicit UiAbstractProgressExt(QWidget *parent = 0);

    virtual void onLayout() = 0;
    virtual void moveEvent(QMoveEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    void setProgressEnabled(bool enabled);

    void setMaxAndMinValue(int maxValue,int minValue);
    void setFloatMaxAndMinValue(float maxValue,float minValue);

    void setCurrentValue(int currentValue);
    void setFloatCurrentValue(float currentValue);

    void setStep(float step);
    void setBackgroundColor(const QColor &color);
    void setFrontgroundColor(const QColor &color);
    void setFontColor(const QColor &color);

    void slotsConnect();
    void setbPercent(bool bPercent);
signals:
   void onValueChangeExt(int value);
public slots:
   void clickAdd();
   void clickSub();
   void valueChange(int changeValue);
   void setButtonStyle(const QString &style);
   void updateValue(int changeValue);
protected:
    void buttonStyleInit(QPushButton *btn,const QString &style);
    UiAbstractProgress *progress;
    QPushButton  *button_add;
    QPushButton  *button_sub;
    QLabel       *label_ValueText;
    int m_maxValue;
    int m_minValue;
    float m_maxValueF,m_minValueF;
    float m_step;
    bool m_bPercent;

};

#endif // UiAbstractProgressExt_H
