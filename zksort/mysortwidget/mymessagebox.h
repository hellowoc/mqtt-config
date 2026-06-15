#ifndef MYMESSAGEBOX_H
#define MYMESSAGEBOX_H

#include <QDialog>
#include <QWidget>
#include "mypushbutton.h"
#include "globalconfig.h"
#include "mylabel.h"

#define MSG_QUES    0       //  询问操作
#define MSG_INFO    1       //  信息提示
#define MSG_WARN    2       //  警告操作
#define MSG_ABOUT   3       //  关于信息

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

class myInfoWidget : public QWidget
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

private:
    QFont font;
    QWidget *widget;
    QLabel *warnningLabel, *iconLabel;
    g_Config *config;
};

extern myInfoWidget *infoWidget;
extern QLabel *titleLabel;

#endif // MYMESSAGEBOX_H
