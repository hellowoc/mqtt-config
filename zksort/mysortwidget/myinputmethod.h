#ifndef MYINPUTMETHOD_H
#define MYINPUTMETHOD_H
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDialog>
#include "mypinyin.h"
#include <QLabel>
#include <QLineEdit>
#include <QSignalMapper>
#include <QApplication>
extern "C" {
#include <hangul.h>
}

#define KB_NORM   0
#define KB_DC     1
#define KB_PD     2

enum inputMethod{
    chinese = 0,
    english = 1,
    korean = 2
};

class myInputMethod : public QDialog
{
    Q_OBJECT
    
public:
    myInputMethod(QWidget *parent = 0);
    ~myInputMethod();
    QString getText(void);
    void setTitleAndInput(QString text,QString inputText);
    void setInputBtnText(QString okText,QString cancelText,QString delText,QString spaceText);
    void setType(int type);

    void setTitle(QString text);
private slots:
    void createButtons(void);   //创建按钮
    void displayAlphabet(void); //显示字母
    void displayNumber(void);   //显示数字
    void displaySpecialChar(void);//显示特殊符号
    void sendChar(int indexOfCharToSend);//打印所按的符号
    void changeInputMethod();  //更换输入法类型english or chinese
    void chineseSelectFont();  //汉字选择
    void pageUp();             //上一页
    void pageDown();           //下一页
    void clear(bool clearInputString);//清零
    void enableInputMethod();   //输入法使能

    void onDelBtnClicked(void); //删除字符
    void buttonC3Function(void);//字母与数字切换
    void buttonC4Function(void);//空格
    void buttonC5Function(void);//确定
    void buttonC6Function(void);//取消


private:
    QWidget *parentWidget;
    QString     strTitle;
    QString     strInput;

    QLabel     *titleLabel; //文字显示区
    QLineEdit   *textLbe;    //文字显示区
    QPushButton *delBtn;    //删除按钮

    QPushButton *listUp;    //上一页面
    QLineEdit   *pinyin;    //拼音显示区
    QPushButton *font_1;    //汉字1
    QPushButton *font_2;    //汉字2
    QPushButton *font_3;    //汉字3
    QPushButton *font_4;    //汉字4
    QPushButton *font_5;    //汉字5
    QPushButton *font_6;    //汉字6
    QPushButton *font_7;    //汉字7
    QPushButton *font_8;    //汉字7
    QPushButton *listDown;  //下一页
    QPushButton *methodBtn; //中英文切换按钮

    QString strPinyin;
    QString strText;
    QStringList inputCharList;
    QString transPy(QString pyInput);   //转换成汉字

    bool findNothing;                   //此字是否存在
    inputMethod method;                 //输入法模式
    int keyboardType;                   //KB_NORM KB_DC
    int currentPage;                    //当前页面

    QStringList readyCharList;          //显示汉字列表
    void showPage(int index);           //显示页面
    int pageCount;                      //页面数

    /* Boutons et signalMapper */
    QSignalMapper *signalMapper;
    QSignalMapper *signalMapper2;

    QVector<QPushButton *> button;      //26个字母

    QPushButton *buttonC3;              //字母与拼音切换
    QPushButton *buttonC4;              //空格
    QPushButton *buttonC5;              //OK
    QPushButton *buttonC6;              //取消

    //布局界面
    QWidget *verticalLayoutWidget;
    QVBoxLayout *mainVerticalLayout;

    QHBoxLayout *horizontalLayout_0;
    QHBoxLayout *horizontalLayout_1;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout_4;
    QHBoxLayout *horizontalLayout_5;
    QHBoxLayout *horizontalLayout_6;
    QHBoxLayout *horizontalLayout_7;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_2;

    int kw; // Keyboard custom width
    int kh; // Keyboard custom height

    //korean
    HangulInputContext *m_hic = NULL; // libhangul 输入上下文
    QString getPreeditStringFromHic();   // 将 hic 的 preedit 转为 QString
    QString getCommitStringFromHic();    // 将 hic 的 commit 转为 QString
    void commitHangulIfAny();            // 如果有 pending preedit -> commit 并 append 到 strText
    void updateCandidates();


};

#endif // MYINPUTMETHOD_H
