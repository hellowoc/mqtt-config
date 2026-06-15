#ifndef UiToast_H
#define UiToast_H
#include "uikits_shared.h"
#include <QLabel>
#include <QString>
#define DEFULT_TOAST_STYLE "\
QLabel{\
    color:#FFFFFF;\
    font:18px;\
    font-weight:500;\
    background-color:rgb(125,125,125,255);\
    padding:3px;\
    border-radius:5;\
}\
"

#define GREEN_TOAST_STYLE "\
QLabel{\
    color:#FFFFFF;\
    font:18px;\
    font-weight:500;\
    background-color:rgb(17,205,124,255);\
    padding:3px;\
    border-radius:5;\
}\
"

#define TH_TOAST_STYLE "\
QLabel{\
    color:#FFFFFF;\
    font:18px;\
    font-weight:500;\
    background-color:rgb(19,196,163,255);\
    padding:3px;\
    border-radius:5;\
}\
"


/**
 * @brief The UiToast class
 *
 * 类似于 Android 的 Toast
 *
 */
class UIKITS_SHARED UiToast : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(int alpha READ getAlpha WRITE setAlpha )
public:
    enum TOAST_FLAG{
        TOAST_SHORT,
        TOAST_LENGTH
    };
    enum TOAST_POSITION_FLAG{
        TOAST_POSITION_DEFAULT,
        TOAST_POSITION_BOTTOM,
        TOAST_POSITION_TOP
    };

    explicit UiToast(QWidget *parent ,QString message,const QString &style);
    explicit UiToast(QWidget *parent ,QString message);

    void showToast(TOAST_FLAG flag = TOAST_SHORT,TOAST_POSITION_FLAG posFlag = TOAST_POSITION_DEFAULT);
    void showToast(QWidget *anchor,int offsetX,int offsetY,TOAST_FLAG flag = TOAST_SHORT,TOAST_POSITION_FLAG posFlag = TOAST_POSITION_DEFAULT);



    QColor getFontColor() const;
    void setFontColor(const QColor &value);

    int getFontSize() const;
    void setFontSize(int value);

    QColor getBgColor() const;
    void setBgColor(const QColor &value);

    int getAlpha() const;
    void setAlpha(int value);

protected:
    void caculatePosition(TOAST_POSITION_FLAG posFlag);

signals:

public slots:
    void dismiss();
    void animationFinished();
private:
    QWidget *m_parent;
    QColor bgColor;
    QColor fontColor;
    int    fontSize;
    QString m_message;

    TOAST_FLAG m_flag;
    int startx;
    int starty;
    int endy;
    int m_width;
    int m_height;
    int mode;
    int alpha;
    void setAutoSize();
};

#endif // UiToast_H
