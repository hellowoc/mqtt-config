#include "mylineedit.h"
#include <QDesktopWidget>
#include <QApplication>
#include "global.h"
#include <QRect>

myLineEdit::myLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    m_max=255, m_min=-255 ,m_IndexNum=-1,m_hint=0;
    m_maxF = 255.0;
    m_minF = 0.0;
    m_pVariableF = NULL;
    m_pParent = NULL;
    m_pVariable = NULL;
    m_inputType = intType;
    setAlignment(Qt::AlignCenter);
}
myLineEdit::myLineEdit(QString text, QRect rect, QWidget *parent):
    QLineEdit(parent)
{
    m_max=255, m_min=-255 ,m_IndexNum=-1,m_hint=0;
    m_maxF = 255.0;
    m_minF = 0.0;
    m_pVariableF = NULL;
    m_pParent = NULL;
    m_pVariable = NULL;
    m_inputType = intType;
    setText(text);
    setGeometry(rect);
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setSizePolicy(sizePolicy);
}

myLineEdit::myLineEdit(EchoMode mode, QRect rect, QWidget *parent):
    QLineEdit(parent)
{
    m_max=255, m_min=-255 ,m_IndexNum=-1,m_hint=0;
    m_maxF = 255.0;
    m_minF = 0.0;
    m_pVariableF = NULL;
    m_pParent = NULL;
    m_pVariable = NULL;
    m_inputType = intType;
    setEchoMode(mode);
    setGeometry(rect);
    setMaxLength(10);
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setSizePolicy(sizePolicy);
}

void myLineEdit::setMinMax( AbstractInterface *parent,qint32 min,qint32 max,qint32 editIndex, qint32 *pVariable)
{
    m_inputType = intType;
    m_max=max;
    m_min=min;
    m_IndexNum = editIndex;
    m_pParent = parent;
    m_pVariable = pVariable;
}

void myLineEdit::setFloatMinMax(AbstractInterface *parent, float min, float max, qint32 editIndex, float *pVariable,bool bPercent)
{
    if(bPercent)
        m_inputType = percentFloatType;
    else
        m_inputType = FloatType;
    m_maxF=max;
    m_minF=min;
    m_IndexNum = editIndex;
    m_pParent = parent;
    m_pVariableF = pVariable;
}

void myLineEdit::setFloatMinMax2(AbstractInterface* parent, float min, float max, qint32 editIndex, float* pVariable,inputType type)
{
    m_inputType = type;
    m_maxF=max;
    m_minF=min;
    m_IndexNum = editIndex;
    m_pParent = parent;
    m_pVariableF = pVariable;
}

void myLineEdit::setMax(qint32 max)
{
    m_max=max;
}

void myLineEdit::setMin(qint32 min)
{
    m_min=min;
}

void myLineEdit::setHint(qint32 hint)
{
    m_hint = hint;
}

void myLineEdit::setVariable(qint32* pVariable)
{
    m_pVariable = pVariable;
}

void myLineEdit::setType(inputType type)
{
    m_inputType = type;
}

qint32 myLineEdit::getMax()
{
    return m_max;
}

qint32 myLineEdit::getMin()
{
    return m_min;
}

qint32 myLineEdit::getHint()
{
    return m_hint;
}

void myLineEdit::setLocation()
{
    QRect rect;
    rect = this->rect();//获得edit框的矩形大小
    QDesktopWidget* desktop = QApplication::desktop();
    int screenW = desktop->width();
    int screenH = desktop->height();//获得屏幕分辨率
    if(screenW >= 1024)
        screenW = 1024;
    else
        screenW = 800;
    if(screenH >= 768)
        screenH =768;
    else
        screenH = 600;
    int dlgheight = g_InputDialog().rect().height();
    g_InputDialog().move((screenW - g_InputDialog().rect().width())/2,100);
//    QPoint ptR = mapToGlobal(QPoint(this->rect().right(),this->rect().bottom()));//把edit框的右下角坐标转换成在屏幕上的绝对坐标
//    QPoint ptL = mapToGlobal(QPoint(this->rect().left(),this->rect().bottom()));//左下角

//    int KeyBoardWidth = g_InputDialog().rect().width()+5;//357+5,加5防止越界
//    int KeyBoardHeight = g_InputDialog().rect().height()+5;//410+5,加5防止越界
//    if((ptR.x() + KeyBoardWidth) < g_Runtime().appPositionX + screenW - 5)//显示在edit框的右边
//    {
//        if( ptR.y()+KeyBoardHeight < g_Runtime().appPositionY + screenH)
//            g_InputDialog().move(ptR.x()+5,ptR.y()-this->rect().height());//下
//        else
//        {
//            if(ptR.y()-KeyBoardHeight < g_Runtime().appPositionY)//小键盘的上边超了
//                g_InputDialog().move(ptR.x()+5,g_Runtime().appPositionY + 10);//上
//            else
//                g_InputDialog().move(ptR.x()+5,ptR.y()-KeyBoardHeight);//上
//        }
//    }
//    else//显示在左边
//    {
//        if( ptL.y() + KeyBoardHeight < g_Runtime().appPositionY + screenH)
//            g_InputDialog().move(ptL.x()-KeyBoardWidth-15,ptL.y()-this->rect().height());//下
//        else
//        {
//            if(ptL.y()-KeyBoardHeight < g_Runtime().appPositionY)//小键盘的上边超了
//                g_InputDialog().move(ptL.x()-KeyBoardWidth-15,g_Runtime().appPositionY + 10);//上
//            else
//                g_InputDialog().move(ptL.x()-KeyBoardWidth-15,ptL.y()-KeyBoardHeight);//上
//        }
//    }
}

void myLineEdit::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        if(isEnabled() && !isReadOnly())
        {
            if(1 == m_hint)
            {
//                if( CDlgMessageBox::MyMessageBox(g_Resource.PhraseString[430], MSG_OKCANCEL ) == QDialog::Rejected )//430#关键参数，请勿擅自修改
//                    return;
            }
            if(m_inputType == textType){
                g_inputmethod().setTitleAndInput(g_myLan().input,this->text());
                g_inputmethod().setInputBtnText(g_myLan().ids_ok,g_myLan().ids_cancel,g_myLan().ids_delete,g_myLan().ids_space);
                if(g_inputmethod().exec() == QDialog::Accepted){
                     setText(g_inputmethod().getText());
                     emit valueSetSignal();
                }
            }else{

                g_InputDialog().SetRecvEdit(this);
                g_InputDialog().setInputType(m_inputType);

                if(m_inputType == intType){
                    qint32 value = text().replace("%","").toInt();
                    g_InputDialog().setParam(value, m_max, m_min, m_IndexNum, m_pVariable);
                }
                else{
                    float value = text().replace("%","").toFloat();
                     g_InputDialog().setParam(value, m_maxF, m_minF, m_IndexNum, m_pVariableF);
                }
                g_InputDialog().setLineParent(m_pParent);



                g_InputDialog().show();
                g_InputDialog().activateWindow();//成为顶层窗口

                QDesktopWidget* desktop = QApplication::desktop();
                int screenW = desktop->width();
                int screenH = desktop->height();//获得屏幕分辨率
                if(screenW >= 1024)
                    screenW = 1024;
                else
                    screenW = 800;
                if(screenH >= 768)
                    screenH =768;
                else
                    screenH = 600;
                g_InputDialog().move((screenW - g_InputDialog().rect().width())/2,150);
            }
//            QRect rt = g_InputDialog().rect();
//            QPoint pt = g_InputDialog().mapToGlobal(QPoint(0, 0));
//            g_InputDialog().m_WidgetRect = QRect(pt.x(), pt.y(), rt.width(), rt.height());
        }
    }
}
