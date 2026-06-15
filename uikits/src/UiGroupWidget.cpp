#include "UiGroupWidget.h"
#include <QDebug>
#include <QPalette>
const int DEFALUT_WIDTH = 150;
const int DEFALUT_HEIGHT = 50;
const int DEFALUT_SPACE = 1;

#define DEFULT_BUTTON_STYLE "\
QPushButton{\
    color:white;\
    font:20px;\
    background-color:#007aee;\
    padding:3px;\
    border-radius:1;\
}\
QPushButton::pressed{\
    background-color:white;\
    color:#007aee;\
} \
"
#define DEFULT_BUTTON_STYLE_SELECTED "\
QPushButton{\
    color:#007aee;\
    font:20px;\
    background-color:white;\
    padding:3px;\
    border-radius:1;\
}\
QPushButton::pressed{\
    background-color:rgb(51,153,255,125);\
    color:#000000;\
} \
"

UiGroupWidget::UiGroupWidget(QWidget *parent) :
    QWidget(parent)
{

    m_btn_width = DEFALUT_WIDTH;
    m_btn_height = DEFALUT_HEIGHT;

    m_style = DEFULT_BUTTON_STYLE;
    m_style_selected = DEFULT_BUTTON_STYLE_SELECTED;

    m_selected_pos = 0;

    layout = new QHBoxLayout(this);
    layout->setMargin(0);

}
void UiGroupWidget::caculateWidgetSize(){
//    int size = m_buttons.size();
//    this->setFixedSize(size*m_btn_width+(size+1)*DEFALUT_SPACE,m_btn_height+10);

    m_btn_width = (width()-m_buttons.size()+1)/m_buttons.size();

    for(int i=0;i<m_buttons.size();i++){
       m_buttons[i]->setFixedSize(m_btn_width,m_btn_height);
       //m_buttons[i]->move(m_btn_width*i+(i+1)*DEFALUT_SPACE,0);
    }

}
void UiGroupWidget::initButtonStyle(){

    if(m_selected_pos<0||m_selected_pos>=m_buttons.size()){
        m_selected_pos = 0;
    }

    for(int i=0;i<m_buttons.size();i++){
       if(i==m_selected_pos){
//          m_buttons[i]->setStyleSheet(m_style_selected);
          m_buttons[i]->setChecked(true);
       }else{
//          m_buttons[i]->setStyleSheet(m_style);
          m_buttons[i]->setChecked(false);
       }
       if(m_buttons.size() == 1){
          m_buttons[i]->setVisible(false);
       }else{
          m_buttons[i]->setVisible(true);
       }
    }



}

void UiGroupWidget::setButtonSize(int w,int h){
    m_btn_width = w;
    m_btn_height = h;

    caculateWidgetSize();
}
void UiGroupWidget::setButtonStyle(const QString &style){
    m_style = style;
    initButtonStyle();
}

void UiGroupWidget::buttonClick(){
    QPushButton* btn= qobject_cast<QPushButton*>(sender());

    int n_whatsThis = btn->whatsThis().toInt();

    if(n_whatsThis < m_buttons.size() && n_whatsThis >= 0)
    {

        for(int i=0;i<m_buttons.size();i++){
//            m_buttons[i]->setStyleSheet(m_style);
            m_buttons[i]->setChecked(false);
        }

        m_buttons[n_whatsThis]->setChecked(true);

        m_selected_pos = n_whatsThis;
        emit clickedButton(m_selected_pos);
    }
}


void UiGroupWidget::setGroupContents(QList<GroupItem> contents){
     m_contents = contents;

     if(m_buttons.size()>0){
         for(int i=0;i<m_buttons.size();i++){
            delete  m_buttons[i];
         }
         m_buttons.clear();

     }

     for(int i=0;i<m_contents.size();i++){
         QPushButton *btn=new QPushButton();
         btn->setText(m_contents[i].itemName);
         btn->setCheckable(true);
         btn->setWhatsThis(QString("%1").arg(i));
         connect(btn,SIGNAL(clicked()),this,SLOT(buttonClick()));
         m_buttons.append(btn);


        layout->addWidget(btn,0,Qt::AlignVCenter);
     }

     layout->setSpacing(DEFALUT_SPACE);

     initButtonStyle();
     caculateWidgetSize();
}

int UiGroupWidget::GetSelected_pos() const
{
    return m_selected_pos;
}

void UiGroupWidget::setSelected_pos(int selected_pos)
{
    m_selected_pos = selected_pos;
    initButtonStyle();
}

void UiGroupWidget::setPosEnable(int pos, bool enable)
{
    m_buttons[pos]->setEnabled(enable);
}
