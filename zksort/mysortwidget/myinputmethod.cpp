#include "myinputmethod.h"
#define LCD_WIDTH 1024
#define LCD_CONTENT_HEIGTH 600
#define FONT_NUM  8 //每页显示汉字数量

myInputMethod::myInputMethod(QWidget *parent)
    : QDialog(parent)
{
    strTitle = "";
    strInput = "";
    strPinyin = "";

    keyboardType = KB_NORM;

    createButtons();
    signalMapper = new QSignalMapper(this);

    for (int i=0; i<37; i++)
    {
        signalMapper->setMapping(button[i], i);
        connect(button[i], SIGNAL(clicked(bool)), signalMapper, SLOT(map()));
    }

    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(sendChar(int)));

    // 信号槽
    connect(buttonC3, SIGNAL(clicked()), this, SLOT(buttonC3Function()));
    connect(buttonC4, SIGNAL(clicked()), this, SLOT(buttonC4Function()));
    connect(buttonC5, SIGNAL(clicked()), this, SLOT(buttonC5Function()));
    connect(buttonC6, SIGNAL(clicked()), this, SLOT(buttonC6Function()));

    connect(delBtn, SIGNAL(clicked()),this,SLOT(onDelBtnClicked()));
    connect(methodBtn,SIGNAL(clicked()), this, SLOT(changeInputMethod()));
    connect(font_1,SIGNAL(clicked()),this,SLOT(chineseSelectFont()));
    connect(font_2,SIGNAL(clicked()),this,SLOT(chineseSelectFont()));
    connect(font_3,SIGNAL(clicked()),this,SLOT(chineseSelectFont()));
    connect(font_4,SIGNAL(clicked()),this,SLOT(chineseSelectFont()));
    connect(font_5,SIGNAL(clicked()),this,SLOT(chineseSelectFont()));
    connect(font_6,SIGNAL(clicked()),this,SLOT(chineseSelectFont()));
    connect(font_7,SIGNAL(clicked()),this,SLOT(chineseSelectFont()));
    connect(font_8,SIGNAL(clicked()),this,SLOT(chineseSelectFont()));

    connect(listUp,SIGNAL(clicked()),this,SLOT(pageUp()));
    connect(listDown,SIGNAL(clicked()),this,SLOT(pageDown()));

//    m_hic = hangul_ic_new("2");
}
/**
 * @brief myInputMethod::~myInputMethod
 */
myInputMethod::~myInputMethod()
{
    if (m_hic) {
//        hangul_ic_delete(m_hic); // 释放 libhangul 资源
//        m_hic = NULL;
    }
}

/**
 * @brief myInputMethod::getText
 * @return
 */
QString myInputMethod::getText()
{
    return textLbe->text();
}

void myInputMethod::setTitleAndInput(QString text, QString inputText)
{
    strTitle = text;
    strInput = inputText;
    titleLabel->setText(strTitle);
    textLbe->setText(strInput);
    strText = strInput;
}

void myInputMethod::setInputBtnText(QString okText, QString cancelText,QString delText,QString spaceText)
{
    buttonC5->setText(okText);
    buttonC6->setText(cancelText);
    delBtn->setText(delText);
    buttonC4->setText(spaceText);
}
/**
 * @brief myInputMethod::setTitle
 * @param text
 */
void myInputMethod::setTitle(QString text)
{
    titleLabel->setText(text);
}

void myInputMethod::setType(int type)
{
    keyboardType = type;

    if (keyboardType == KB_DC){
        listUp->setHidden(true);
        listDown->setHidden(true);
        methodBtn->setHidden(true);
        buttonC3->setHidden(true);
        // pinyin->setHidden(true);

        font_1->setHidden(true);
        font_2->setHidden(true);
        font_3->setHidden(true);
        font_4->setHidden(true);
        font_5->setHidden(true);
        font_6->setHidden(true);
        font_7->setHidden(true);
        font_8->setHidden(true);
    }
    else if(keyboardType == KB_PD){
        listUp->setHidden(true);
        listDown->setHidden(true);
        methodBtn->setHidden(true);
        buttonC3->setHidden(true);
        // pinyin->setHidden(true);
        textLbe->setEchoMode(QLineEdit::Password);
        textLbe->setMaxLength(20);

        font_1->setHidden(true);
        font_2->setHidden(true);
        font_3->setHidden(true);
        font_4->setHidden(true);
        font_5->setHidden(true);
        font_6->setHidden(true);
        font_7->setHidden(true);
        font_8->setHidden(true);
    }
    else {
        listUp->setHidden(false);
        listDown->setHidden(false);
        methodBtn->setHidden(false);
        buttonC3->setHidden(false);
        // pinyin->setHidden(false);

        font_1->setHidden(false);
        font_2->setHidden(false);
        font_3->setHidden(false);
        font_4->setHidden(false);
        font_5->setHidden(false);
        font_6->setHidden(false);
        font_7->setHidden(false);
        font_8->setHidden(false);
    }
    displayAlphabet();
}
/**
 * @brief myInputMethod::createButtons
 * 初始化
 */
void myInputMethod::createButtons(void)
{
    method = english;
    strText = strInput;
    setWindowFlags(Qt::FramelessWindowHint);
    kw = LCD_WIDTH;
    kh = LCD_CONTENT_HEIGTH-2;
    setFocusPolicy(Qt::NoFocus);

    //    setStyleSheet("background-color: rgb(190,190,190);");

    // Laypout principal vertical
    mainVerticalLayout = new QVBoxLayout(this);
    mainVerticalLayout->setSpacing(10);
    mainVerticalLayout->setContentsMargins(20, 20, 20, 20);

    // Layout Horizontal title -----------------------------------------------
    titleLabel = new QLabel(strTitle,this);
    titleLabel->setMaximumHeight(40);
    horizontalLayout_0 = new QHBoxLayout();
    horizontalLayout_0->addWidget(titleLabel);
    mainVerticalLayout->addLayout(horizontalLayout_0);

    // Layout Horizontal 1 -----------------------------------------------
    delBtn = new QPushButton(this);
    delBtn->setObjectName("delBtn");
    textLbe    = new QLineEdit(strInput,this);
    textLbe->setObjectName("inputTextEdit");
    textLbe->setFocusPolicy(Qt::StrongFocus);
    horizontalLayout_1 = new QHBoxLayout();
    horizontalLayout_1->setSpacing(10);
    horizontalLayout_1->addWidget(textLbe);
    horizontalLayout_1->addWidget(delBtn);
    mainVerticalLayout->addLayout(horizontalLayout_1);

    // Layout Horizontal 2 -----------------------------------------------
    listUp   = new QPushButton(this);
    listUp->setObjectName("listUpBtn");
    listDown = new QPushButton(this);
    listDown->setObjectName("listDownBtn");
    pinyin    = new QLineEdit(tr(""),this);
    pinyin->hide();

    delBtn->setFocusPolicy(Qt::NoFocus);
    textLbe->setFocusPolicy(Qt::NoFocus);
    listUp->setFocusPolicy(Qt::NoFocus);
    listDown->setFocusPolicy(Qt::NoFocus);
    pinyin->setFocusPolicy(Qt::NoFocus);

    font_1    = new QPushButton("1",this);
    font_2    = new QPushButton("2",this);
    font_3    = new QPushButton("3",this);
    font_4    = new QPushButton("4",this);
    font_5    = new QPushButton("5",this);
    font_6    = new QPushButton("6",this);
    font_7    = new QPushButton("7",this);
    font_8    = new QPushButton("8",this);
    font_1->setMinimumWidth(30);
    font_2->setMinimumWidth(30);
    font_3->setMinimumWidth(30);
    font_4->setMinimumWidth(30);
    font_5->setMinimumWidth(30);
    font_6->setMinimumWidth(30);
    font_7->setMinimumWidth(30);
    font_8->setMinimumWidth(30);
    horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setSpacing(10);
    //horizontalLayout_2->addWidget(pinyin);
    horizontalLayout_2->addWidget(listUp);
    horizontalLayout_2->addWidget(font_1);
    horizontalLayout_2->addWidget(font_2);
    horizontalLayout_2->addWidget(font_3);
    horizontalLayout_2->addWidget(font_4);
    horizontalLayout_2->addWidget(font_5);
    horizontalLayout_2->addWidget(font_6);
    horizontalLayout_2->addWidget(font_7);
    horizontalLayout_2->addWidget(font_8);
    horizontalLayout_2->addWidget(listDown);

    mainVerticalLayout->addLayout(horizontalLayout_2);

    for(int i=0; i < 37; i++)
    {
        button.append(new QPushButton(".", this));
        button[i]->setFocusPolicy(Qt::NoFocus);
    }

    // Layout Horizontal 3-----------------------------------------------
    horizontalLayout_3 = new QHBoxLayout();
    horizontalLayout_3->setSpacing(10);


    for(int i=0; i < 10; i++)
    {
        horizontalLayout_3->addWidget(button[i]);
    }

    mainVerticalLayout->addLayout(horizontalLayout_3);


    // Layout Horizontal 4 -----------------------------------------------
    horizontalLayout_4 = new QHBoxLayout();
    horizontalLayout_4->setSpacing(10);


    for(int i=10; i < 20; i++)
    {
        horizontalLayout_4->addWidget(button[i]);
    }

    mainVerticalLayout->addLayout(horizontalLayout_4);


    // Layout Horizontal 5 -----------------------------------------------
    horizontalLayout_5 = new QHBoxLayout();
    horizontalLayout_5->setSpacing(10);
    horizontalLayout_5->setContentsMargins(button[0]->width()/2, 0, button[0]->width()/2, 0);

    for(int i=20; i < 29; i++)
    {
        horizontalLayout_5->addWidget(button[i]);
    }

    mainVerticalLayout->addLayout(horizontalLayout_5);


    // Layout Horizontal 6 -----------------------------------------------
    horizontalLayout_6 = new QHBoxLayout();
    horizontalLayout_6->setContentsMargins(button[0]->width(), 0, button[0]->width(), 0);
    horizontalLayout_6->setSpacing(10);

    horizontalSpacer = new QSpacerItem(10, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);
    horizontalLayout_6->addItem(horizontalSpacer);

    for(int i=29; i < 37; i++)
    {
        horizontalLayout_6->addWidget(button[i]);
    }

    horizontalSpacer_2 = new QSpacerItem(10, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);
    horizontalLayout_6->addItem(horizontalSpacer_2);

    mainVerticalLayout->addLayout(horizontalLayout_6);

    // Layout Horizontal 7 -----------------------------------------------
    horizontalLayout_7 = new QHBoxLayout();
    horizontalLayout_7->setSpacing(10);

    methodBtn = new QPushButton(this);
    methodBtn->setObjectName("inputTypeBtn");
    methodBtn->setFocusPolicy(Qt::NoFocus);
    horizontalLayout_7->addWidget(methodBtn);
    //字母与符号切换
    buttonC3 = new QPushButton(QIcon(""),"",this);
    // buttonC3->setFocusPolicy(Qt::NoFocus);
    buttonC3->setText(QString(".?123"));
    horizontalLayout_7->addWidget(buttonC3);

    //空格
    buttonC4 = new QPushButton(QIcon(""),"",this);
    buttonC4->setObjectName("spaceBtn");
    // buttonC4->setFocusPolicy(Qt::NoFocus);
    horizontalLayout_7->addWidget(buttonC4);

    // Layout Horizontal 7 -----------------------------------------------
    horizontalLayout_8 = new QHBoxLayout();
    horizontalLayout_8->setSpacing(30);
    //确定按钮
    buttonC5 = new QPushButton(this);
    //buttonC5->setFocusPolicy(Qt::NoFocus);
    buttonC5->setObjectName("inputokBtn");
    horizontalLayout_8->addWidget(buttonC5);

    horizontalLayout_8->addLayout(horizontalLayout_7);
    //取消按钮
    buttonC6 = new QPushButton(this);
    buttonC6->setObjectName("inputcancelBtn");
    // buttonC6->setFocusPolicy(Qt::NoFocus);
    horizontalLayout_8->addWidget(buttonC6);


    mainVerticalLayout->addLayout(horizontalLayout_8);

    displayAlphabet();

    //中文输入法不使能
    enableInputMethod();
    currentPage = 0;
    pageCount = 0;
    findNothing = true;

    return;
}
/**
 * @brief myInputMethod::enableInputMethod
 * 输入法使能
 */
void myInputMethod::enableInputMethod()
{
    methodBtn->setEnabled(true);
    bool enable = false;
    if (method == chinese || method == korean){
        enable = true;
    }
    pinyin->setEnabled(enable);
    font_1->setEnabled(enable);
    font_2->setEnabled(enable);
    font_3->setEnabled(enable);
    font_4->setEnabled(enable);
    font_5->setEnabled(enable);
    font_6->setEnabled(enable);
    font_7->setEnabled(enable);
    font_8->setEnabled(enable);
    listDown->setEnabled(enable);
    listUp->setEnabled(enable);
}

/**
 * @brief myInputMethod::displayAlphabet
 * 显示大写字母
 */
void myInputMethod::displayAlphabet(void)
{
    unsigned char *key;

    unsigned char qwertyUpper[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0','Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '.'};
    unsigned char qwertyLower[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0','q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'z', 'x', 'c', 'v', 'b', 'n', 'm', '.'};
    unsigned char qwertyDccrypt[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0','Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '-'};
    QStringList koreanShowList;
    koreanShowList << "ㅃ" << "ㅉ" << "ㄸ" << "ㄲ" << "ㅆ" << "" << "" << "" << "ㅒ" << "ㅖ"
                   << "ㅂ" << "ㅈ" << "ㄷ" << "ㄱ" << "ㅅ" << "ㅛ" << "ㅕ" << "ㅑ" << "ㅐ" << "ㅔ"
                   << "ㅁ" << "ㄴ" << "ㅇ" << "ㄹ" << "ㅎ" << "ㅗ" << "ㅓ" << "ㅏ" << "ㅣ"
                   << "ㅋ" << "ㅌ" << "ㅊ" << "ㅍ" << "ㅠ" << "ㅜ" << "ㅡ" << ".";

    if (keyboardType == KB_DC){
        key = qwertyDccrypt;
        for(int i=0; i < 37; i++)
        {
            button[i]->setText(QChar::fromLatin1(key[i]));
        }
    }
    else
    {
        if(method == chinese)
        {
            key = qwertyLower;
            for(int i=0; i < 37; i++)
            {
                button[i]->setText(QChar::fromLatin1(key[i]));
            }
        }
        else if(method == korean){
            for(int i=0; i < 37; i++)
            {
                button[i]->setText(koreanShowList.at(i));
            }
        }else{
            key = qwertyUpper;
            for(int i=0; i < 37; i++)
            {
                button[i]->setText(QChar::fromLatin1(key[i]));
            }
        }
    }


    return;
}

/**
 * @brief myInputMethod::displayNumber
 * 显示字母
 */
void myInputMethod::displayNumber(void)
{
    unsigned short int number[] = {'[', ']', '{', '}', '#', '%', '^', '*', '|', '=', '_', '/', ':', ';', '(', ')', '+', '&', '@', '"', '-', ',', '?', '!', '\\', '%', '<', '>', '$',0x20ac,0x00B2,39,' ',' ',' ',' ',' '};

    for(int i=0; i < 37; i++)
    {
        if(number[i] != '&')
        {
            button[i]->setText(QChar(number[i]));
        }
        else
        {
            button[i]->setText(QString(QChar(number[i]))+QString(QChar(number[i])));
        }
    }

    return;
}
/**
 * @brief myInputMethod::displaySpecialChar
 * 显示特殊字符
 */
void myInputMethod::displaySpecialChar(void)
{
    unsigned char SpecialChar[] = {'[', ']', '{', '}', '#', '%', '^', '*', '+', '=', '_', '\\', '|', '~', '<', '>', '$', ' ', ' ', ' ', '-', ',', '?', '!', '\'', ' '};

    for(int i=0; i < 26; i++)
    {
        button[i]->setText(QChar::fromLatin1(SpecialChar[i]));
    }

    return;
}

/**
 * @brief myInputMethod::buttonC3Function
 * 字母与特殊字符的切换
 */
void myInputMethod::buttonC3Function(void)
{
    if(buttonC3->text() == QString(".?123"))
    {
        pinyin->setEnabled(false);
        listUp->setEnabled(false);
        font_1->setEnabled(false);
        font_2->setEnabled(false);
        font_3->setEnabled(false);
        font_4->setEnabled(false);
        font_5->setEnabled(false);
        font_6->setEnabled(false);
        font_7->setEnabled(false);
        font_8->setEnabled(false);
        listDown->setEnabled(false);
        methodBtn->setEnabled(false);

        buttonC3->setText(QString("ABC"));

        displayNumber();
    }
    else {
        enableInputMethod();
        buttonC3->setText(QString(".?123"));

        displayAlphabet();
    }
    pinyin->clear();

    return;
}
/**
 * @brief myInputMethod::buttonC4Function
 * 空格
 */
void myInputMethod::buttonC4Function(void)
{
    if (method == korean) {
        commitHangulIfAny();      // 把 preedit 变为 commit（如果有）
        strText += " ";
        textLbe->setText(strText);
        pinyin->clear();
    }
    else
    {
        QString str;
        str = textLbe->text();
        str += " ";
        textLbe->setText(str);
    }
}
/**
 * @brief myInputMethod::buttonC5Function
 * 确定按钮
 */
void myInputMethod::buttonC5Function(void)
{
    if (method == korean) {
        commitHangulIfAny();
        textLbe->setText(strText);
        pinyin->clear();
    }
    // 清理并 accept
    this->clear(true);
    accept();
}
/**
 * @brief myInputMethod::buttonC6Function
 * 取消按钮
 */
void myInputMethod::buttonC6Function(void)
{
    reject();
}
/**
 * @brief myInputMethod::sendChar
 * @param indexOfCharToSend
 * 所按按钮上面的字符
 */
void myInputMethod::sendChar(int indexOfCharToSend)
{
    if(buttonC3->text() == QString(".?123"))
    {
        if (method == chinese) {
            if (indexOfCharToSend > 9) {
                pinyin->insert(button[indexOfCharToSend]->text().toLower());
                strPinyin = pinyin->text();
                transPy(strPinyin);
                textLbe->setText(strText+strPinyin);
            }
            else {
                textLbe->insert(button[indexOfCharToSend]->text());
                strText = textLbe->text();
            }
        }
        else if(method == korean)
        {
//            static unsigned char qwertyKorean[] = {'Q', 'W', 'E', 'R', 'T',' ', ' ', ' ', 'O', 'P', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'z', 'x', 'c', 'v', 'b', 'n', 'm', '.'};
//            char c = qwertyKorean[indexOfCharToSend];
//            // 送入 libhangul 处理
//            hangul_ic_process(m_hic, (ucschar)c);

//            // 如果有 commit（已完成音节）会被 hangul_ic_get_commit_string 返回
//            QString commit = getCommitStringFromHic();
//            if (!commit.isEmpty()) {
//                // 将 commit 添加到已提交文本（注意：我们仍保留 strText 作为已提交）
//                strText += commit;
//                // reset commit buffer in libhangul isn't necessary here since commit buffer is managed by libhangul;
//                // updateCandidates() 会查找新 strText/preedit
//            }

//            // 更新 preedit 显示（实时）
//            QString pre = getPreeditStringFromHic();
//            pinyin->setText(pre);
//            textLbe->setText(strText + pre);

//            // ---- 新增：每次输入后刷新候选（韩字 -> 한자 候选） ----
//            updateCandidates();
        }
        else {
            textLbe->insert(button[indexOfCharToSend]->text());
            strText = textLbe->text();
        }
    }
    else {
        textLbe->insert(button[indexOfCharToSend]->text());
        strText = textLbe->text();
    }

    return;
}
/**
 * @brief myInputMethod::onDelBtnClicked
 * 删除输入
 * 当中文输入法启用时，拼音内有输入时，删除拼音，否则删除汉字
 */
void myInputMethod::onDelBtnClicked()
{
    if (method == korean) {
//        // 若 preedit 不为空，则后退一个 jamo/成分
//        QString pre = getPreeditStringFromHic();
//        if (!pre.isEmpty()) {
//            hangul_ic_backspace(m_hic);
//            QString newpre = getPreeditStringFromHic();
//            pinyin->setText(newpre);
//            textLbe->setText(strText + newpre);
//            updateCandidates(); // <-- 在这里刷新候选
//            return;
//        }

//        // 若 preedit 已空但存在已提交的文字，则删除已提交的最后一个字符（与原逻辑一致）
//        if (!strText.isEmpty()) {
//            strText.chop(1); // 删除最后一个字符（注意：若为多字节，按需改为适当处理）
//            textLbe->setText(strText);
//        }
    }else{

        if (strPinyin == "") {
            strText.remove(strText.length()-1,strText.length());
            textLbe->setText(strText);
        }
        else {
            strPinyin = pinyin->text();
            strPinyin.remove(strPinyin.length()-1,strPinyin.length());
            pinyin->setText(strPinyin);
            textLbe->setText(strText+strPinyin);
            transPy(strPinyin);
        }
    }
}
/**
 * @brief myInputMethod::changeInputMethod
 * 输入法切换
 */
void myInputMethod::changeInputMethod()
{
    if(method == english){
        method = chinese;
    }else if(method == chinese){
        method = korean;
//        hangul_ic_reset(m_hic);
    }else
    {
        method = english;
    }
    displayAlphabet();
    enableInputMethod();
}
/**
 * @brief myInputMethod::pageUp
 * 汉字列表，上一页
 */
void myInputMethod::pageUp()
{
    currentPage--;
    if(currentPage > -1)
    {
        listDown->setEnabled(true);
        if(currentPage-- == 0)
            listUp->setDisabled(true);
        currentPage++;
        showPage(currentPage);
    }
    else
    {
        currentPage++;
        listUp->setDisabled(true);
    }
}
/**
 * @brief myInputMethod::pageDown
 * 汉字列表，下一页
 */
void myInputMethod::pageDown()
{
    currentPage++;
    if(currentPage < pageCount)
    {
        listUp->setEnabled(true);
        if(currentPage++ == pageCount-1)
            listDown->setDisabled(true);
        currentPage--;
        showPage(currentPage);
    }
    else
    {
        currentPage--;
        listDown->setDisabled(true);
    }
}
/**
 * @brief myInputMethod::chineseSelectFont
 * 选择汉字
 */
void myInputMethod::chineseSelectFont()
{
    QPushButton *btn = (QPushButton*)QObject::sender();
    strText += btn->text();
    textLbe->setText(strText);
    readyCharList.clear();
    commitHangulIfAny();
    this->clear(true);
}
/**
 * @brief myInputMethod::clear
 * @param clearInputString
 * 清除汉字列表
 */
void myInputMethod::clear(bool clearInputString)
{
    if(clearInputString) {
        pinyin->clear();
        strPinyin = pinyin->text();
    }
    font_1->setText("");
    font_2->setText("");
    font_3->setText("");
    font_4->setText("");
    font_5->setText("");
    font_6->setText("");
    font_7->setText("");
    font_8->setText("");
}
/**
 * @brief myInputMethod::transPy
 * @param pyInput
 * @return
 * 输入拼音，翻译成汉字
 */
QString myInputMethod::transPy(QString pyInput)
{
    int count = 0;
    QString py = pyInput;
    readyCharList.clear();

    for(unsigned int i = 0; i < sizeof(PY_IdxTable) / sizeof(PY_IdxTable[0]); i++)
    {
        if(py.compare(PY_IdxTable[i].PY) == 0)
        {
            findNothing = false;
            QString txt = PY_IdxTable[i].MB;

            if(txt.contains(","))
            {
                readyCharList = txt.split(",");
                count = readyCharList.count();
            }
            else
            {
                int c;
                count = strlen(PY_IdxTable[i].MB);
                for(c = 0; c < count;)
                {
                    char tt[4] = "";
                    tt[0] = PY_IdxTable[i].MB[c++];
                    tt[1] = PY_IdxTable[i].MB[c++];
                    tt[2] = PY_IdxTable[i].MB[c++];

                    readyCharList += tt;
                }
                count = readyCharList.count();
            }
        }
    }

    if(count%FONT_NUM == 0)
    {
        pageCount = count/FONT_NUM;
    }
    else
    {
        pageCount = (count/FONT_NUM)+1;
    }


    if(findNothing)
    {
        this->clear(false);
        return NULL;
    }

    listUp->setEnabled(false);
    listDown->setEnabled(true);
    currentPage = 0;
    findNothing = true;
    showPage(currentPage);
    //currentPage = 0;
    return py;
}
/**
 * @brief myInputMethod::showPage
 * @param index
 * 汉字显示列表，每一页
 */
void myInputMethod::showPage(int index)
{
    // index: 页号，从 0 开始
    // 先清空所有按钮文本
    font_1->setText("");
    font_2->setText("");
    font_3->setText("");
    font_4->setText("");
    font_5->setText("");
    font_6->setText("");
    font_7->setText("");
    font_8->setText("");

    // 填充当前页
    for (int i = 0; i < FONT_NUM; ++i) {
        int pos = index * FONT_NUM + i;
        if (pos < readyCharList.count()) {
            QString t = readyCharList.at(pos);
            switch (i) {
            case 0: font_1->setText(t); break;
            case 1: font_2->setText(t); break;
            case 2: font_3->setText(t); break;
            case 3: font_4->setText(t); break;
            case 4: font_5->setText(t); break;
            case 5: font_6->setText(t); break;
            case 6: font_7->setText(t); break;
            case 7: font_8->setText(t); break;
            }
        } else {
            // 保持为空
            switch (i) {
            case 0: font_1->setText(""); break;
            case 1: font_2->setText(""); break;
            case 2: font_3->setText(""); break;
            case 3: font_4->setText(""); break;
            case 4: font_5->setText(""); break;
            case 5: font_6->setText(""); break;
            case 6: font_7->setText(""); break;
            case 7: font_8->setText(""); break;
            }
        }
    }

    // 翻页按钮状态
    listUp->setEnabled(index > 0);
    listDown->setEnabled(index < pageCount - 1);
}

QString myInputMethod::getPreeditStringFromHic()
{
//    if (!m_hic) return QString();

//    const ucschar *pre = hangul_ic_get_preedit_string(m_hic);
//    if (!pre) return QString();

//    // ucschar 本质是 uint32_t，Qt 直接支持
//    return QString::fromUcs4(reinterpret_cast<const uint*>(pre));
}

QString myInputMethod::getCommitStringFromHic()
{
    if (!m_hic) return QString();

//    const ucschar *c = hangul_ic_get_commit_string(m_hic);
//    if (!c) return QString();

//    return QString::fromUcs4(reinterpret_cast<const uint*>(c));
}

void myInputMethod::commitHangulIfAny()
{
    if (!m_hic) return;

//    hangul_ic_reset(m_hic);

    QString commit = getCommitStringFromHic();
    if (!commit.isEmpty()) {
        strText += commit;
    }

    // 提交之后，候选（对应 preedit）通常应该刷新/清空
    updateCandidates();
}

void myInputMethod::updateCandidates()
{
    readyCharList.clear();

    if (!m_hic) {
        clear(false);
        return;
    }

    // 1. 取当前正在组合的 Hangul（preedit）
    QString preedit = getPreeditStringFromHic();
    if (preedit.isEmpty()) {
        clear(false);
        return;
    }

    // --- 第一层：韩文自身候选（当前音节） ---
    // 放在最前面，让用户可以直接选原文
    readyCharList << preedit;

    // 3. 分页显示
    pageCount = (readyCharList.size() + FONT_NUM - 1) / FONT_NUM;
    currentPage = 0;
    showPage(0);
}

