#include "mymessagebox.h"
#include "mylanguage.h"

myMessageBox::myMessageBox(int style, QString text,QWidget *parent) :
    QDialog(parent)
{
    int margin = 25;

    switch(LCD_WIDTH) {
    case 1024:
        margin = 25;
        break;
    case 640:
        margin = 5;
        break;
    }

    this->setWindowModality(Qt::ApplicationModal);
    setWindowFlags(Qt::FramelessWindowHint);
    setGeometry(LCD_WIDTH/4,LCD_HEIGHT/4,LCD_WIDTH/2,LCD_HEIGHT/2);
    setStyleSheet("background-color: rgb(137, 136, 136);");


    textLabel   = new myLabel(text,
                              QRect(margin,this->height()/2-30,this->width()-2*margin,50),
                              this);
    textLabel->setAlignment(Qt::AlignHCenter);
    iconLabel   = new myLabel("",QRect(margin,margin,50,50),this);
    okBtn      =  new myPushButton(myLan.ok, ":/res/png/icon_ok.png",
                                   QRect(margin,this->height()-60,BTN_WIDTH,BTN_HEIGHT),
                                   this);
    cancelBtn  =  new myPushButton(myLan.cancel, ":/res/png/icon_cancel.png",
                                   QRect(this->width()-BTN_WIDTH-margin,this->height()-60,BTN_WIDTH,BTN_HEIGHT),
                                   this);
    switch(style)
    {
    case MSG_QUES:
        iconLabel->setPixmap(QPixmap(":/res/png/icon_question.png"));
        break;
    case MSG_INFO:
        iconLabel->setPixmap(QPixmap(":/res/png/icon_info.png"));
        okBtn->setHidden(TRUE);
        cancelBtn->setHidden(TRUE);
        break;
    case MSG_WARN:
        iconLabel->setPixmap(QPixmap(":/res/png/icon_warn.png"));
        okBtn->setHidden(TRUE);
        cancelBtn->setText(myLan.back);
        break;
    case MSG_ABOUT:
        iconLabel->setPixmap(QPixmap(":/res/png/icon_info.png"));
        okBtn->setHidden(TRUE);
        cancelBtn->setText(myLan.back);
        break;
    }

    connect(okBtn    ,   SIGNAL(clicked()), this,   SLOT(onOkBtnClicked()));
    connect(cancelBtn,  SIGNAL(clicked()),  this,   SLOT(onCancelBtnClicked()));

    if (style == MSG_INFO) {
      QTimer *timer=new QTimer(this);
      connect(timer,SIGNAL(timeout()),this,SLOT(close()));
      timer->start(3000);
    }
}
//  确定按钮
void myMessageBox::onOkBtnClicked()
{
    accept();
}
//  取消按钮
void myMessageBox::onCancelBtnClicked()
{
    reject();
}

void myMessageBox::setText(QString text)
{
    textLabel->setText(text);
}

myInfoWidget::myInfoWidget(QWidget *parent) :
    QWidget(parent)
{
    setGeometry(0, 0, LCD_WIDTH, LCD_HEIGHT);
    config = g_Config::getInstance();
    int margin = 25;

    widget = new QWidget(this);
    widget->setGeometry(width()/4, height()/4, width()/2, height()/2);
    widget->setStyleSheet("background-color: rgb(137, 136, 136);");
    warnningLabel = new QLabel(widget);
    warnningLabel->setFont(config->getFont());
    warnningLabel->setGeometry(margin, widget->height()/2, widget->width()-2*margin, 30);
    warnningLabel->setAlignment(Qt::AlignHCenter);
    iconLabel = new QLabel(widget);
    iconLabel->setGeometry(margin,margin,50,50);
    iconLabel->setPixmap(QPixmap(":/res/png/icon_info.png"));
}

/* 析构函数*/
myInfoWidget::~myInfoWidget()
{
}

/* 设置标签上的提醒文本 */
void myInfoWidget::setLabelText(QString text)
{
    widget->show();
    warnningLabel->setText(text);
}

/* 延迟一定时间后再显示 */
void myInfoWidget::delayShow()
{
    this->show();
    QTime dieTime = QTime::currentTime().addMSecs(10);
    while(QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    }
}

/* 隐藏提示框，仅覆盖当前页面 */
void myInfoWidget::hideCentralWidget()
{
    widget->hide();
}

/* 显示secs秒后隐藏 */
void myInfoWidget::showSecs(int secs)
{
    this->show();
    QTime dieTime = QTime::currentTime().addSecs(secs);
    while(QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    }
    this->hide();
}

/* 延迟一定时间后隐藏 */
void myInfoWidget::delayHide()
{
    QTime dieTime = QTime::currentTime().addSecs(1);
    while(QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    }
    this->hide();
}

myInfoWidget *infoWidget;
QLabel *titleLabel;
