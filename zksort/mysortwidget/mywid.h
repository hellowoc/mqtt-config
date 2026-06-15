#ifndef MYWID_H
#define MYWID_H

#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QTextBrowser>
#include <QApplication>

#define ICON_WIDTH          32
#define ICON_HEIGHT         32

#if(LCD_WIDTH == 1024)
    #define BTN_HEIGHT          50
    #define BTN_WIDTH           160
    #define SMALL_BTN_WIDTH     50
    #define DEFAULT_BTN_WIDTH   120
    #define BIG_BTN_WIDTH       250
    #define LCD_TITLE_HEIGHT    50
    #define LCD_CONTENT_HEIGTH  668
    #define LCD_STATUS_HEIGHT   60
    #define CURVE_WIDGET_HEIGHT 420
    #define CURVE_WIDGET_WIDTH	850
    #define TAB_HEIGHT          45
    #define PAGE_BTN_WIDTH      350
    #define PAGE_BTN_HEIGHT      60
#else
    #define BTN_HEIGHT          40
    #define BTN_WIDTH           135
    #define SMALL_BTN_WIDTH     40
    #define DEFAULT_BTN_WIDTH   100
    #define BIG_BTN_WIDTH       200
    #define LCD_TITLE_HEIGHT    45
    #define LCD_CONTENT_HEIGTH  390
    #define LCD_STATUS_HEIGHT   45
    #define CURVE_WIDGET_HEIGHT 310
    #define CURVE_WIDGET_WIDTH	500
    #define TAB_HEIGHT          40
    #define PAGE_BTN_WIDTH      250
    #define PAGE_BTN_HEIGHT      45
#endif

#define MSG_QUES    0       //  询问操作
#define MSG_INFO    1       //  信息提示
#define MSG_WARN    2       //  警告操作
#define MSG_ABOUT   3       //  关于信息

#define BTN_COLOR_DEF         0
#define BTN_COLOR_RED         1
#define BTN_COLOR_GREEN       2

class myLabel;
class myPushButton;
class myMutex;


/******************************************************************************
 *
 *                      myLabel
 *
 *******************************************************************************/
class myLabel : public QLabel
{
public:
    explicit myLabel();
    explicit myLabel(QString text, QRect rect=QRect(), QWidget *parent = 0);

private:

};


/******************************************************************************
 *
 *                      myPushButton
 *
 *******************************************************************************/


class myPushButton : public QPushButton
{
    //Q_OBJECT
public:
    explicit myPushButton(QString text, QString iconName, QRect rect, bool enable, QWidget *parent = 0);
    explicit myPushButton(QString text, QString iconName, QRect rect, QWidget *parent = 0);
    explicit myPushButton(QString iconName, QSize size, QWidget *parent = 0);
    void setRedColor(int color);

signals:

public slots:

private:
    QString m_nName;
    QString m_sText;
};

/******************************************************************************
 *
 *                      myMessageBox
 *
 *******************************************************************************/
class myMessageBox : public QDialog
{
    Q_OBJECT
public:
    explicit myMessageBox(int style, QString text,QWidget *parent = 0);
    void setText(QString text);

signals:
    
public slots:
    void onOkBtnClicked();
    void onCancelBtnClicked();

private:
    myPushButton    *okBtn;
    myPushButton    *cancelBtn;
    myLabel         *textLabel;
    myLabel         *iconLabel;
};

/******************************************************************************
 *
 *                      myLongMessageBox
 *
 *******************************************************************************/
class myLongMessageBox : public QDialog
{
    Q_OBJECT
public:
    explicit myLongMessageBox(int style, QString text,QWidget *parent = 0);
    void setText(QString text);

signals:

public slots:
    void onOkBtnClicked();
    void onCancelBtnClicked();

private:
    myPushButton    *okBtn;
    myPushButton    *cancelBtn;
    QTextBrowser    *textBro;
    myLabel         *iconLabel;
};

/******************************************************************************
 *
 *                      myInfoWidget
 *
 *******************************************************************************/
class myInfoWidget : public QDialog
{
    Q_OBJECT
public:
    explicit myInfoWidget(QWidget *parent = 0);
    ~myInfoWidget();
    void setLabelText(QString text);
    void delayShow(void);
    void hideCentralWidget(void);
    void showSecs(int secs = 2);
    void delayHide(void);

    QWidget *widget;
private:
    QFont font;
    QLabel *warnningLabel, *iconLabel;
};

/******************************************************************************
 *
 *                      myMutex
 *              该类用于实现二种或三种间的互斥选择
 *
 *******************************************************************************/
class myMutex : public QWidget
{
    Q_OBJECT

public:
    explicit myMutex(Qt::Orientation flag, QWidget* parent = 0);
    explicit myMutex(Qt::Orientation flag, int selIndex, QWidget* parent = 0);
    ~myMutex();
    void setNum(int num);
    void setTitle(QString text);
    void setLabelText(QString label1Text, QString label2Text="", QString label3Text="");
    void clearBtnsIcon(void);
    int getCurrentIndex(void);
    void setCurrentIndex(int index);
    void setIndexEnabled(int index, bool enabled);

signals:
    void indexChanged(int);

private slots:
    void onBtn1Clicked();
    void onBtn2Clicked();
    void onBtn3Clicked();

private:
    enum{selectedNum = 3};
    QGroupBox *group;
    myPushButton *btns[selectedNum];
    QLabel *labels[selectedNum];
    quint8 curIndex;
};

//extern myInfoWidget *infoWidget;

#endif // MYWID_H
