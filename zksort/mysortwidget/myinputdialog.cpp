#include "myinputdialog.h"
#include "ui_myinputdialog.h"
#include <QPainter>
#include <QTimer>
#include <QMouseEvent>
#include "basewidget.h"
#include "global.h"

MyInputDialog::MyInputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyInputDialog)
{
    ui->setupUi(this);
    setModal(true);
    m_pRecvEdit=NULL;
    pParent = NULL;
    m_pVar = NULL;
    m_ConnectFlag=false;
    max=255, min=-255,m_value=1,msg_Par=-1;

    m_type = intType;
    ui->upBtn->setAutoRepeat(true);
    ui->upBtn->setAutoRepeatDelay(200);
    ui->upBtn->setAutoRepeatInterval(50);
    ui->upBtn->setObjectName("addBtn");

    ui->downBtn->setAutoRepeat(true);
    ui->downBtn->setAutoRepeatDelay(200);
    ui->downBtn->setAutoRepeatInterval(50);
    ui->downBtn->setObjectName("subBtn");

    m_bool_first = false;
    setParam(m_value,max,min,msg_Par);
    setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint);
    setFixedSize(size());
    signalMapper = new QSignalMapper(this);

    m_numberBtn[0] = ui->pushButton_0;
    m_numberBtn[1] = ui->pushButton_1;
    m_numberBtn[2] = ui->pushButton_2;
    m_numberBtn[3] = ui->pushButton_3;
    m_numberBtn[4] = ui->pushButton_4;
    m_numberBtn[5] = ui->pushButton_5;
    m_numberBtn[6] = ui->pushButton_6;
    m_numberBtn[7] = ui->pushButton_7;
    m_numberBtn[8] = ui->pushButton_8;

    m_numberBtn[9] = ui->pushButton_9;
    for (int i=0; i<10; i++)
    {
        signalMapper->setMapping(m_numberBtn[i], i);
        connect(m_numberBtn[i], SIGNAL(clicked(bool)), signalMapper, SLOT(map()));
    }

    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(OnBtnClick(int)));
}

MyInputDialog::~MyInputDialog()
{
    delete ui;
}


void MyInputDialog::SetRecvEdit(QLineEdit *edit)
{
    m_pRecvEdit=edit;
    m_value=0;
    if(m_pRecvEdit!=NULL && !m_ConnectFlag)
    {
        m_ConnectFlag = true;
    }
}

void MyInputDialog::setParam(qint32 value, qint32 max, qint32 min, qint32 msg_par, qint32* pVar)
{
    m_DispNum = value;
     m_value = value;
     this->max=max;
     this->min=min;
     msg_Par = msg_par;
     m_pVar = pVar;

     ui->labelUpper->setText(QString::number(max));
     ui->labelDown->setText(QString::number(min));
     ui->lineEdit->setText(QString::number(value));
     if(min < 0){
         ui->minusBtn->setEnabled(true);
     }else{
         ui->minusBtn->setEnabled(false);
     }

     ui->labelMax->setText(g_myLan().ids_uplimit);
     ui->labelMin->setText(g_myLan().ids_lowlimit);
     ui->delBtn->setText(g_myLan().clear);
     ui->confirmBtn->setText(g_myLan().ids_ok);
     ui->cancelBtn->setText(g_myLan().ids_cancel);
}

void MyInputDialog::setParam(float value, float max, float min, qint32 msg_par, float *pVar)
{
    m_DispNumf = value;
     m_valueF = value;
     maxF=max;
     minF=min;
     msg_Par = msg_par;
     m_pVarf = pVar;

     if(m_type == percentFloatType2){
         ui->labelUpper->setText(QString::number(max,'f',2));
         ui->labelDown->setText(QString::number(min,'f',2));
         ui->lineEdit->setText(QString::number(m_valueF,'f',2));
     }else if(m_type == FloatType3){
         ui->labelUpper->setText(QString::number(max,'f',3));
         ui->labelDown->setText(QString::number(min,'f',3));
         ui->lineEdit->setText(QString::number(m_valueF,'f',3));
     }else{
         ui->labelUpper->setText(QString::number(max,'f',1));
         ui->labelDown->setText(QString::number(min,'f',1));
         ui->lineEdit->setText(QString::number(m_valueF,'f',1));
     }
     if(min < 0){
         ui->minusBtn->setEnabled(true);
     }else{
         ui->minusBtn->setEnabled(false);
     }

}

void MyInputDialog::OnBtnClick(int id)
{
    FirstDigitClicked();
    if (ui->lineEdit->text() == "0" ||ui->lineEdit->text() == "") {
        m_DispNum = 0;
        m_value = m_DispNum;
        ui->lineEdit->setText("");
    }

    if(m_type == intType){
        ui->lineEdit->insert(m_numberBtn[id]->text());
        m_DispNum = ui->lineEdit->text().toInt();
        if(min>=0){
            if (m_DispNum >= max){
                m_DispNum = max;
            }
        }else{
        }
        QString str = QString("%1").arg(m_DispNum);
        ui->lineEdit->setText(str);
        m_value = m_DispNum;
    }else if(m_type == FloatType || m_type == percentFloatType || m_type == percentFloatType2){

        QString textStr = ui->lineEdit->text();
        if(textStr.length() > 2){
            if(m_type == percentFloatType2){
                if(textStr[textStr.length()-3] == '.'){
                    return;
                }
            }else{
                if(textStr[textStr.length()-2] == '.'){
                    return;
                }
            }
        }
        ui->lineEdit->insert(m_numberBtn[id]->text());
        textStr = ui->lineEdit->text();
        m_DispNumf = textStr.toDouble();

        if(minF>=-0.01){
            if (m_DispNumf >= maxF){
                m_DispNumf = maxF;
                QString str = QString("%1").arg(m_DispNumf);
                ui->lineEdit->setText(str);
            }
        }
//        if (m_DispNumf < minF || ui->lineEdit->text() == "")
//        {
//            m_DispNumf = minF;
//            QString str = QString("%1").arg(m_DispNumf);
//            ui->lineEdit->setText(str);
//        }
        m_valueF = m_DispNumf;
    }else if(m_type == FloatType3){

        QString textStr = ui->lineEdit->text();
        if(textStr.length() > 4){
            return;
        }
        ui->lineEdit->insert(m_numberBtn[id]->text());
        textStr = ui->lineEdit->text();
        m_DispNumf = textStr.toDouble();

        if(minF>=-0.0001){
            if (m_DispNumf >= maxF){
                m_DispNumf = maxF;
                QString str = QString("%1").arg(m_DispNum);
                ui->lineEdit->setText(str);
            }
        }
        if (m_DispNumf < minF || ui->lineEdit->text() == "")
        {
            m_DispNumf = minF;
            QString str = QString("%1").arg(m_DispNumf);
            ui->lineEdit->setText(str);
        }
        m_valueF = m_DispNumf;
    }

    checkDotBtn();
}

void MyInputDialog::setLineParent(AbstractInterface *parent)
{
    pParent = parent;
}

void MyInputDialog::setInputType(inputType type)
{
    m_bool_first = true;
    m_type = type;
    ui->dotBtn->setEnabled(m_type == percentFloatType || m_type == FloatType ||m_type == percentFloatType2 || m_type == FloatType3);
}

void MyInputDialog::on_confirmBtn_clicked()
{
    if(m_type == intType){
        m_value = ui->lineEdit->text().toInt();
        if(m_value <= max && m_value >= min)
        {
            //m_pRecvEdit->setText(QString::number(m_value));
            m_pRecvEdit->setText(QString("%1").arg(ui->lineEdit->text()));
            if(pParent!=NULL)
            {
                if(m_pVar)
                    *m_pVar = m_value;
                pParent->receiveMsg(MSG_DATA_CHANGED,msg_Par,m_value);
            }
            hide();
            close();
        }
    }else if(m_type == percentFloatType || m_type == FloatType || m_type == percentFloatType2||m_type == FloatType3){
        m_valueF = ui->lineEdit->text().toFloat();
        if(m_valueF <= maxF && m_valueF >= minF)
        {
            if(m_type == percentFloatType)
                m_pRecvEdit->setText(QString("%1%").arg(ui->lineEdit->text()));
            else
                m_pRecvEdit->setText(QString("%1").arg(ui->lineEdit->text()));
            if(pParent!=NULL)
            {
                if(m_pVarf)
                    *m_pVarf = m_valueF;
                pParent->receiveMsg(MSG_DATA_CHANGED,msg_Par);
            }
            hide();
            close();
        }
//        else if(m_valueF<minF)
//        {
//            if(m_type == percentFloatType)
//                m_pRecvEdit->setText(QString("%1%").arg(minF));
//            else
//                m_pRecvEdit->setText(QString("%1").arg(minF));
//            if(pParent!=NULL)
//            {
//                if(m_pVarf)
//                    *m_pVarf = minF;
//                pParent->receiveMsg(MSG_DATA_CHANGED,msg_Par);
//            }
//            hide();
//            close();
//        }
    }
}

void MyInputDialog::on_cancelBtn_clicked()
{
    hide();
    close();
}

void MyInputDialog::on_upBtn_clicked()
{
     QString str;
    if (m_type == intType) {
        if (m_DispNum < max)
            m_DispNum++;
        if (m_DispNum >= max){
            m_DispNum = max;
        }
        m_value = m_DispNum;
        str = QString("%1").arg(m_value);
    }else if (m_type == percentFloatType || m_type == FloatType){
        if(m_DispNumf < maxF)
            m_DispNumf += 0.1;
        if(m_DispNumf > maxF)
            m_DispNumf = maxF;
        m_valueF = m_DispNumf;
        str = QString::number(m_valueF,'f',1);
    }else if (m_type == percentFloatType2){
        if(m_DispNumf < maxF)
            m_DispNumf += 0.01;
        if(m_DispNumf > maxF)
            m_DispNumf = maxF;
        m_valueF = m_DispNumf;
        str = QString::number(m_valueF,'f',2);
    }else if (m_type == FloatType3){
        if(m_DispNumf < maxF)
            m_DispNumf += 0.001;
        if(m_DispNumf > maxF)
            m_DispNumf = maxF;
        m_valueF = m_DispNumf;
        str = QString::number(m_valueF,'f',3);
    }
    ui->lineEdit->setText(str);
    checkDotBtn();
}

void MyInputDialog::on_downBtn_clicked()
{
    QString str;
    if (m_type == intType) {
        if(m_DispNum > min)
            m_DispNum--;
        m_value = m_DispNum;
        str = QString("%1").arg(m_value);
    }else if(m_type == percentFloatType || m_type == FloatType){
        if(m_DispNumf - minF > 0.000001){
            m_DispNumf -= 0.1;
            if(m_DispNumf < 0.1 && m_DispNumf > 0)
                m_DispNumf = 0;

        }
        if(m_DispNumf < minF)
            m_DispNumf = minF;
        m_valueF = m_DispNumf;
        str = QString::number(m_valueF,'f',1);
    }else if (m_type == percentFloatType2){
        if(m_DispNumf - minF > 0.000001){
            m_DispNumf -= 0.01;
            if(m_DispNumf < 0.01 && m_DispNumf > 0)
                m_DispNumf = 0;
        }
        if(m_DispNumf < minF)
            m_DispNumf = minF;
        m_valueF = m_DispNumf;
        str = QString::number(m_valueF,'f',2);
    }else if (m_type == FloatType3){
        if(m_DispNumf - minF > 0.000001){
            m_DispNumf -= 0.001;
            if(m_DispNumf < 0.0001 && m_DispNumf > 0)
                m_DispNumf = 0;

        }
        if(m_DispNumf < minF)
            m_DispNumf = minF;
        m_valueF = m_DispNumf;
        str = QString::number(m_valueF,'f',3);
    }

    ui->lineEdit->setText(str);

    checkDotBtn();
}

void MyInputDialog::checkDotBtn()
{
    QString str;
    str = ui->lineEdit->text();
    if (str.length()) {
        if (m_type == percentFloatType || m_type == FloatType) {
            if (str[str.length()-1] == '.')
            {
                ui->dotBtn->setEnabled(false);
            }
        }else if(m_type == percentFloatType2){
            if(str.length()>=2){
                if (str[str.length()-2] == '.')
                {
                   ui->dotBtn->setEnabled(false);
                }
            }

        }
    }
}
void MyInputDialog::on_minusBtn_clicked()
{
    m_bool_first = false;
    if(m_type == intType){
        if(min>=0)
            return;
        m_value =m_value * -1;

        if(m_value < min)
            m_value = min;
        ui->lineEdit->setText(QString::number(m_value));
    }else{
        if(minF >= 0)
            return;
        float temp = m_valueF *-1;
        if(temp > minF)
            m_valueF = temp;
        ui->lineEdit->setText(QString::number(m_valueF,'f',1));
    }
}

void MyInputDialog::FirstDigitClicked(void)
{
    if(m_bool_first == true)
    {
        m_bool_first = false;
        m_value = 0;
        m_valueF = 0.0;
        ui->lineEdit->setText("");
    }
}

void MyInputDialog::on_dotBtn_clicked()
{
    QString str = ui->lineEdit->text();
    if(!str.contains("."))
    {
        m_DispNumf = QString("%1.").arg(str).toFloat();
        ui->lineEdit->setText(QString("%1.").arg(str));
    }
}

void MyInputDialog::on_delBtn_clicked()
{
    m_value=0;
    ui->lineEdit->setText(QString::number(m_value));
    if(m_type == percentFloatType || m_type == percentFloatType2 || m_type == FloatType){
        ui->dotBtn->setEnabled(true);
    }
}
