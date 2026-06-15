#include "mywid.h"
#include "global.h"


//myInfoWidget *infoWidget;

/******************************************************************************
 *
 *                      myLabel
 *
 *******************************************************************************/
myLabel::myLabel()
{

}

myLabel::myLabel(QString text, QRect rect, QWidget *parent):
    QLabel(parent)
{
    setText(text);
    setGeometry(rect);
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setSizePolicy(sizePolicy);
}

/******************************************************************************
 *
 *                      myLabel
 *
 *******************************************************************************/
myPushButton::myPushButton(QString text, QString iconName, QRect rect, bool enable, QWidget *parent) :
    QPushButton(parent)
{
    setText(text);
    setIcon(QIcon(iconName));
    setIconSize(QSize(32,32));
    setGeometry(rect);
    setEnabled(enable);
    m_nName = iconName;
    m_sText = text;

    if(iconName == "" || text == ""){
        setStyleSheet("QPushButton {"
                          "text-align:center;"
                          "background-color: palegoldenrod;"
                          "border-width: 2px;"
                          "border-style: solid;"
                          "border-radius: 10;"
                          "padding: 3px;}"

                      "QPushButton:pressed {"
                          "padding-left: 5px;"
                         " padding-top: 5px;"
                          "background-color: #d0d67c;}");
    }
    else{
        setStyleSheet("QPushButton {"
                          "text-align:left;"
                          "background-color: palegoldenrod;"
                          "border-width: 2px;"
                          "border-style: solid;"
                          "border-radius: 10;"
                          "padding: 3px;}"

                      "QPushButton:pressed {"
                          "padding-left: 5px;"
                         " padding-top: 5px;"
                          "background-color: #d0d67c;}");
    }

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setSizePolicy(sizePolicy);
}

myPushButton::myPushButton(QString text, QString iconName, QRect rect, QWidget *parent) :
    QPushButton(parent)
{
    setText(text);
    setIcon(QIcon(iconName));
    setIconSize(QSize(32,32));
    setGeometry(rect);
    m_nName = iconName;
    m_sText = text;

    if(iconName == "" || text == ""){
        setStyleSheet("QPushButton {"
                          "text-align:center;"
                          "background-color: palegoldenrod;"
                          "border-width: 2px;"
                          "border-style: solid;"
                          "border-radius: 10;"
                          "padding: 3px;}"

                      "QPushButton:pressed {"
                          "padding-left: 5px;"
                         " padding-top: 5px;"
                          "background-color: #d0d67c;}");
    }
    else{
        setStyleSheet("QPushButton {"
                          "text-align:left;"
                          "background-color: palegoldenrod;"
                          "border-width: 2px;"
                          "border-style: solid;"
                          "border-radius: 10;"
                          "padding: 3px;}"

                      "QPushButton:pressed {"
                          "padding-left: 5px;"
                         " padding-top: 5px;"
                          "background-color: #d0d67c;}");
    }
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setSizePolicy(sizePolicy);
}

myPushButton::myPushButton(QString iconName, QSize size, QWidget *parent) :
    QPushButton(parent)
{
    setIcon(QIcon(iconName));
    setFixedSize(size);
    setIconSize(QSize(32,32));
    m_nName = iconName;
    m_sText = "";

    setStyleSheet("QPushButton {"
                      "text-align:center;"
                      "background-color: palegoldenrod;"
                      "border-width: 2px;"
                      "border-style: solid;"
                      "border-radius: 10;"
                      "padding: 3px;}"

                  "QPushButton:pressed {"
                      "padding-left: 5px;"
                     " padding-top: 5px;"
                      "background-color: #d0d67c;}");

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setSizePolicy(sizePolicy);
}

void myPushButton::setRedColor(int color)
{
    if((m_nName != "") && (m_sText != "")){
        switch(color)
        {
        case BTN_COLOR_RED:
            setStyleSheet("QPushButton {"
                              "text-align:left;"
                              "background-color: red;"
                              "border-width: 2px;"
                              "border-style: solid;"
                              "border-radius: 10;"
                              "padding: 3px;}"

                          "QPushButton:pressed {"
                              "padding-left: 5px;"
                              "padding-top: 5px;"
                              "background-color: #d0d67c;}");
            break;
        case BTN_COLOR_GREEN:
            setStyleSheet("QPushButton {"
                              "text-align:left;"
                              "background-color: chartreuse;"
                              "border-width: 2px;"
                              "border-style: solid;"
                              "border-radius: 10;"
                              "padding: 3px;}"

                          "QPushButton:pressed {"
                              "padding-left: 5px;"
                              "padding-top: 5px;"
                              "background-color: #d0d67c;}");
            break;
        case BTN_COLOR_DEF:
            setStyleSheet("QPushButton {"
                              "text-align:left;"
                              "background-color: palegoldenrod;"
                              "border-width: 2px;"
                              "border-style: solid;"
                              "border-radius: 10;"
                              "padding: 3px;}"

                          "QPushButton:pressed {"
                              "padding-left: 5px;"
                              "padding-top: 5px;"
                              "background-color: #d0d67c;}");
            break;
        }
    }
    else{
        switch(color)
        {
        case BTN_COLOR_RED:
            setStyleSheet("QPushButton {"
                              "text-align:center;"
                              "background-color: chartreuse;"
                              "border-width: 2px;"
                              "border-style: solid;"
                              "border-radius: 10;"
                              "padding: 3px;}"

                          "QPushButton:pressed {"
                              "padding-left: 5px;"
                             " padding-top: 5px;"
                              "background-color: #d0d67c;}");
            break;
        case BTN_COLOR_GREEN:
            setStyleSheet("QPushButton {"
                              "text-align:center;"
                              "background-color: chartreuse;"
                              "border-width: 2px;"
                              "border-style: solid;"
                              "border-radius: 10;"
                              "padding: 3px;}"

                          "QPushButton:pressed {"
                              "padding-left: 5px;"
                             " padding-top: 5px;"
                              "background-color: #d0d67c;}");
            break;
        case BTN_COLOR_DEF:
            setStyleSheet("QPushButton {"
                              "text-align:center;"
                              "background-color: palegoldenrod;"
                              "border-width: 2px;"
                              "border-style: solid;"
                              "border-radius: 10;"
                              "padding: 3px;}"

                          "QPushButton:pressed {"
                              "padding-left: 5px;"
                             " padding-top: 5px;"
                              "background-color: #d0d67c;}");
            break;
        }
    }
}


/******************************************************************************
 *
 *                      myMessageBox
 *
 *******************************************************************************/
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

    setParent(QApplication::activeWindow());
    this->setWindowModality(Qt::ApplicationModal);
    setWindowFlags(Qt::FramelessWindowHint);
    setGeometry(LCD_WIDTH/4, struCnfg.screenH/4,LCD_WIDTH/2, struCnfg.screenH/2);
//    this->setStyleSheet("background-color: rgb(10, 100, 240);");
//    setStyleSheet("background-color: rgb(137, 136, 136);");

    textLabel   = new myLabel(text, QRect(margin,this->height()/2-30,this->width()-2*margin,100), this);
    textLabel->setAlignment(Qt::AlignHCenter);
    textLabel->setWordWrap(true);
    iconLabel   = new myLabel("",QRect(margin,margin,50,50),this);
    okBtn      =  new myPushButton(g_myLan().ids_ok, "",
                                   QRect(margin,this->height()-60,BTN_WIDTH,BTN_HEIGHT),
                                   this);
    okBtn->setObjectName("okBtn");
    cancelBtn  =  new myPushButton(g_myLan().ids_cancel, "",
                                   QRect(this->width()-BTN_WIDTH-margin,this->height()-60,BTN_WIDTH,BTN_HEIGHT),
                                   this);
    cancelBtn->setObjectName("cancelBtn");
    switch(style)
    {
    case MSG_QUES:
        iconLabel->setPixmap(QPixmap(":/white/icon_question.png"));
        break;
    case MSG_INFO:
        iconLabel->setPixmap(QPixmap(":/white/icon_info.png"));
        okBtn->setHidden(true);
        cancelBtn->setHidden(true);
        break;
    case MSG_WARN:
        iconLabel->setPixmap(QPixmap(":/white/icon_warn.png"));
        okBtn->setHidden(true);
        cancelBtn->setText(g_myLan().ids_back);
        break;
    case MSG_ABOUT:
        iconLabel->setPixmap(QPixmap("::/white/icon_info.png"));
        okBtn->setHidden(true);
        cancelBtn->setText(g_myLan().ids_back);
        break;
    }

    connect(okBtn    ,   SIGNAL(pressed()), this,   SLOT(onOkBtnClicked()));
    connect(cancelBtn,  SIGNAL(pressed()),  this,   SLOT(onCancelBtnClicked()));

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

/******************************************************************************
 *
 *                      myLongMessageBox
 *
 *******************************************************************************/
myLongMessageBox::myLongMessageBox(int style, QString text,QWidget *parent) :
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
    setGeometry(LCD_WIDTH/4, struCnfg.screenH/4,LCD_WIDTH/2, struCnfg.screenH/2);
//    this->setStyleSheet("background-color: rgb(10, 100, 240);");
//    setStyleSheet("background-color: rgb(137, 136, 136);");


    textBro  = new QTextBrowser(this);
    textBro->setText(text);
    textBro->setGeometry(QRect(margin,margin*2,this->width()-2*margin,this->height()-120));

//    textBro->setFixedSize(this->width()-2*margin,this->height()-2*margin);
    textBro->setAlignment(Qt::AlignHCenter);
    iconLabel   = new myLabel("",QRect(margin,margin,50,50),this);
    okBtn      =  new myPushButton(g_myLan().ids_ok, "",
                                   QRect(margin,this->height()-60,BTN_WIDTH,BTN_HEIGHT),
                                   this);
    okBtn->setObjectName("okBtn");
    cancelBtn  =  new myPushButton(g_myLan().ids_cancel, "",
                                   QRect(this->width()-BTN_WIDTH-margin,this->height()-60,BTN_WIDTH,BTN_HEIGHT),
                                   this);
    cancelBtn->setObjectName("cancelBtn");
    switch(style)
    {
    case MSG_QUES:
        iconLabel->setPixmap(QPixmap(":/white/icon_question.png"));
        break;
    case MSG_INFO:
        iconLabel->setPixmap(QPixmap(":/white/icon_info.png"));
        okBtn->setHidden(true);
        cancelBtn->setHidden(true);
        break;
    case MSG_WARN:
        iconLabel->setPixmap(QPixmap(":/white/icon_warn.png"));
        okBtn->setHidden(true);
        cancelBtn->setText(g_myLan().ids_back);
        break;
    case MSG_ABOUT:
        iconLabel->setPixmap(QPixmap("::/white/icon_info.png"));
        okBtn->setHidden(true);
        cancelBtn->setText(g_myLan().ids_back);
        break;
    }

    connect(okBtn    ,   SIGNAL(pressed()), this,   SLOT(onOkBtnClicked()));
    connect(cancelBtn,  SIGNAL(pressed()),  this,   SLOT(onCancelBtnClicked()));

    if (style == MSG_INFO) {
      QTimer *timer=new QTimer(this);
      connect(timer,SIGNAL(timeout()),this,SLOT(close()));
      timer->start(3000);
    }
}
//  确定按钮
void myLongMessageBox::onOkBtnClicked()
{
    accept();
}
//  取消按钮
void myLongMessageBox::onCancelBtnClicked()
{
    reject();
}

void myLongMessageBox::setText(QString text)
{
    textBro->setText(text);
}

/******************************************************************************
 *
 *                      myInfoWidget
 *
 *******************************************************************************/
myInfoWidget::myInfoWidget(QWidget *parent) :
    QDialog(parent)
{
//    setGeometry(0, 0, LCD_WIDTH, LCD_HEIGHT);
//    setGeometry(LCD_WIDTH/8,LCD_HEIGHT/8,LCD_WIDTH*3/4,LCD_HEIGHT*3/4);

    this->setModal(true);

    this->setWindowFlags(Qt::FramelessWindowHint);
//    this->setStyleSheet("background-color: rgb(10, 100, 210);");
//    config = g_Config::getInstance();
    int margin = 25;

    widget = new QWidget(this);
    widget->setGeometry(width()/4, height()/4, width()/2, height()/2);
//    widget->setStyleSheet("background-color: rgb(137, 136, 136);");
    warnningLabel = new QLabel(widget);
//    warnningLabel->setFont(config->getFont());
    warnningLabel->setGeometry(margin, widget->height()/2-100, widget->width()-2*margin, 200);
    warnningLabel->setAlignment(Qt::AlignCenter);
    warnningLabel->setWordWrap(true);
    iconLabel = new QLabel(widget);
    iconLabel->setGeometry(margin,margin,50,50);
    iconLabel->setPixmap(QPixmap(":/white/icon_info.png"));
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

/******************************************************************************
 *
 *                      myMutex
 *
 *******************************************************************************/
myMutex::myMutex(Qt::Orientation flag, QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *lay = new QHBoxLayout(this);
    lay->setContentsMargins(0, 0, 0, 0);
    group = new QGroupBox(this);
    group->setGeometry(0,0,width(),height());
    for(int i=0; i<selectedNum; i++){
        btns[i] = new myPushButton("",QSize(BTN_WIDTH,BTN_HEIGHT));
        labels[i] = new QLabel;
    }

    if(flag == Qt::Horizontal){
        QHBoxLayout *hLayout = new QHBoxLayout;
        for(int i=0; i<selectedNum; i++){
            hLayout->addWidget(btns[i]);
            hLayout->addWidget(labels[i]);
            hLayout->addSpacing(10);
        }
        group->setLayout(hLayout);
    }

    if(flag == Qt::Vertical){
        QGridLayout *vLayout = new QGridLayout;
        for(int i=0; i<selectedNum; i++){
            vLayout->addWidget(btns[i],i,0);
            vLayout->addWidget(labels[i],i,1);
        }
        vLayout->setColumnMinimumWidth(0,60);
        group->setLayout(vLayout);
    }
    lay->addWidget(group);

    connect(btns[0],SIGNAL(pressed()),this,SLOT(onBtn1Clicked()));
    connect(btns[1],SIGNAL(pressed()),this,SLOT(onBtn2Clicked()));
    connect(btns[2],SIGNAL(pressed()),this,SLOT(onBtn3Clicked()));
}

myMutex::myMutex(Qt::Orientation flag, int selIndex, QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *lay = new QHBoxLayout(this);
    lay->setContentsMargins(0, 0, 0, 0);

    group = new QGroupBox(this);
    group->setGeometry(0,0,width(),height());
    for(int i=0; i<selectedNum; i++){
//        btns[i] = new myPushButton("",btnSize);
        btns[i] = new myPushButton("",QSize(BTN_WIDTH,BTN_HEIGHT));
        labels[i] = new QLabel;
    }

    if(flag == Qt::Horizontal){
        QHBoxLayout *hLayout = new QHBoxLayout;
        for(int i=0; i<selectedNum; i++){
            hLayout->addWidget(btns[i]);
            hLayout->addWidget(labels[i]);
            hLayout->addSpacing(10);
        }
        group->setLayout(hLayout);
    }

    if(flag == Qt::Vertical){
        QGridLayout *vLayout = new QGridLayout;
        if(LCD_WIDTH == 640)
            vLayout->setContentsMargins(10, 0, 10, 0);
        for(int i=0; i<selectedNum; i++){
            vLayout->addWidget(btns[i],i,0);
            vLayout->addWidget(labels[i],i,1);
        }
        vLayout->setColumnMinimumWidth(0,60);
        group->setLayout(vLayout);
    }

    curIndex = selIndex;
    btns[curIndex]->setIcon(QIcon(":/white/icon_ok.png"));

    lay->addWidget(group);

    connect(btns[0],SIGNAL(pressed()),this,SLOT(onBtn1Clicked()));
    connect(btns[1],SIGNAL(pressed()),this,SLOT(onBtn2Clicked()));
    connect(btns[2],SIGNAL(pressed()),this,SLOT(onBtn3Clicked()));
}

myMutex::~myMutex()
{
}

/* 设置组中的可选项 */
void myMutex::setNum(int num)
{
    for(int i=0; i<num; i++)
        btns[i]->setEnabled(true);

    for(int i=num; i<selectedNum; i++){
        btns[i]->hide();
        labels[i]->hide();
    }
}

/* 设置组的标题 */
void myMutex::setTitle(QString text)
{
    group->setTitle(text);
//    group->setFont(config->getFont());
}

/* 设置组中的标签的文本 */
void myMutex::setLabelText(QString label1Text, QString label2Text, QString label3Text)
{
//    for(int i=0; i<selectedNum; i++){
//        labels[i]->setFont(config->getFont(DEFAULT_FONT_SIZE));
//    }
    labels[0]->setText(label1Text);
    labels[1]->setText(label2Text);
    labels[2]->setText(label3Text);
}

/* 清空按钮组的图标 */
void myMutex::clearBtnsIcon()
{
    for(int i=0; i<selectedNum; i++)
        btns[i]->setIcon(QIcon());
}

/* 获取当前被选择的索引号 */
int myMutex::getCurrentIndex()
{
    return curIndex;
}

/* 设置当前组别选择的索引号 */
void myMutex::setCurrentIndex(int index)
{
    clearBtnsIcon();
    btns[index]->setIcon(QIcon(":/white/icon_ok.png"));
}

/* 设置索引号是否可按 */
void myMutex::setIndexEnabled(int index, bool enabled)
{
    btns[index]->setEnabled(enabled);
}

/* 响应组中的第一个按钮 */
void myMutex::onBtn1Clicked()
{
    clearBtnsIcon();
    btns[0]->setIcon(QIcon(":/white/icon_ok.png"));
    curIndex = 0;
    emit indexChanged(curIndex);
}

/* 响应组中的第二个按钮 */
void myMutex::onBtn2Clicked()
{
    clearBtnsIcon();
    btns[1]->setIcon(QIcon(":/white/icon_ok.png"));
    curIndex = 1;
    emit indexChanged(curIndex);
}

/* 响应组中的第三个按钮 */
void myMutex::onBtn3Clicked()
{
    clearBtnsIcon();
    btns[2]->setIcon(QIcon(":/white/icon_ok.png"));
    curIndex = 2;
    emit indexChanged(curIndex);
}
