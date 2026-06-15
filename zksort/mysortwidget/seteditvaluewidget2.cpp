#include "seteditvaluewidget.h"
#include "ui_seteditvaluewidget.h"
SetEditValueWidget::SetEditValueWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetEditValueWidget)
{
    ui->setupUi(this);
    m_pTimer1 = new QTimer(this);
    m_pTimer2 = new QTimer(this);
    connect(m_pTimer1,SIGNAL(timeout()),this,SLOT(sub_timeout()));
    connect(m_pTimer2,SIGNAL(timeout()),this,SLOT(add_timeout()));
    ui->label_2->hide();
    ui->label_3->hide();
}

SetEditValueWidget::~SetEditValueWidget()
{
    delete ui;
}

void SetEditValueWidget::setParams(AbstractInterface *parent, int min, int max, int index,int *valueAddr)
{
    m_type = IntType;
    m_pParent = parent;
    m_maxInt = max;
    m_minInt = min;
    m_widgetIndex = index;
    m_valueAddr = valueAddr;
    ui->lineEdit->setMinMax(this,min,max,index,valueAddr);
}

void SetEditValueWidget::setFloatParams(AbstractInterface *parent, float min, float max, int index, int bitCount, float *valueAddr)
{
    m_type = floatType;
    m_pParent = parent;
    m_maxFloat = max;
    m_minFloat = min;
    m_widgetIndex = index;
    m_FloatvalueAddr = valueAddr;
    m_bitCount = bitCount;
    m_bitStep  = 1.0*
    ui->lineEdit->setFloatMinMax(this,min,max,index,valueAddr,false);
}

void SetEditValueWidget::setValue(float value)
{
    m_valueFloat = value;
}

void SetEditValueWidget::setValue(float value, int bitCount)
{
    m_valueFloat = value;
    ui->lineEdit->setText(QString::number(value,'f',bitCount));
}

void SetEditValueWidget::setValue(int value)
{
    m_valueInt = value;
    ui->lineEdit->setText(QString::number(value));
}

void SetEditValueWidget::setValueDecimalBitCount(int bitCount)
{
    m_bitCount = bitCount;
}

void SetEditValueWidget::setTitle(QString titleText)
{
    ui->label->setText(titleText);
}

void SetEditValueWidget::setTitle(QString titleText,int locationIndex)
{
    QLabel *label[3];
    label[0] = ui->label;
    label[1] = ui->label_2;
    label[2] = ui->label_3;

    for(int i = 0; i < 3; i++)
    {
        label[i]->hide();
    }
    if(locationIndex < 3)
    {
        label[locationIndex]->show();
        label[locationIndex]->setText(titleText);
    }
}

void SetEditValueWidget::receiveMsg(int msg1, int msg2, int msg3)
{
    if(msg1 == MSG_DATA_CHANGED){
     if(m_type == IntType)
         m_valueInt = *m_valueAddr;
     else
         m_valueFloat = *m_FloatvalueAddr;
    }
    if(m_pParent){
        m_pParent->receiveMsg(msg1,msg2);
    }
}

void SetEditValueWidget::on_subBtn_released()
{
    m_pTimer1->stop();
  if(m_bPressed)
      sub_timeout();
    sendParams();
}


void SetEditValueWidget::on_subBtn_pressed()
{
    m_pTimer1->start(100);
    m_bPressed = true;
}

void SetEditValueWidget::sub_timeout()
{
    m_bPressed = false;
    if(m_type == IntType){
        if(m_valueInt > m_minInt){
            m_valueInt--;
            ui->lineEdit->setText(QString::number(m_valueInt));
        }
    }else{
        if(m_valueFloat - m_minFloat > 0.000000001){
            m_valueFloat-= 1.0/m_bitCount;
            if(m_valueFloat < 0.1 && m_valueFloat > 0)
                m_valueFloat = 0;
            ui->lineEdit->setText(QString::number(m_valueFloat,'f',1));
        }
    }
}

void SetEditValueWidget::add_timeout()
{
    m_bPressed = false;
    if(m_type == IntType){
        if(m_valueInt < m_maxInt){
            m_valueInt++;
            ui->lineEdit->setText(QString::number(m_valueInt));
        }
    }else{
        if(m_valueFloat < m_maxFloat){
            m_valueFloat+=0.1;
            ui->lineEdit->setText(QString::number(m_valueFloat,'f',1));
        }
    }
}

void SetEditValueWidget::sendParams()
{
    if(m_type == IntType){
        ui->lineEdit->setText(QString::number(m_valueInt));
        if(m_pParent){
            *m_valueAddr = m_valueInt;
            m_pParent->receiveMsg(MSG_DATA_CHANGED,m_widgetIndex);
        }
    }else{
        ui->lineEdit->setText(QString::number(m_valueFloat,'f',1));
        if(m_pParent){
            *m_FloatvalueAddr = m_valueFloat;
            m_pParent->receiveMsg(MSG_DATA_CHANGED,m_widgetIndex);
        }
    }
}

void SetEditValueWidget::on_addBtn_pressed()
{
    m_pTimer2->start(100);
    m_bPressed = true;
}

void SetEditValueWidget::on_addBtn_released()
{
    m_pTimer2->stop();
    if(m_bPressed)
        add_timeout();
    sendParams();
}
