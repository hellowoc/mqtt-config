#ifndef MYINPUTDIALOG_H
#define MYINPUTDIALOG_H

#include <QDialog>
#include <QLineEdit>

#include <QRectF>
#include <QSignalMapper>
namespace Ui {
class MyInputDialog;
}
enum inputType{
    percentFloatType,
    FloatType,//保留一位小数
    percentFloatType2,//保留两位小数
    FloatType3,//保留三位小数
    intType,
    passwdType,
    textType
    };
class AbstractInterface;
class MyInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MyInputDialog(QWidget *parent = 0);
    ~MyInputDialog();
    void SetRecvEdit(QLineEdit *edit);
    void setParam(qint32 value, qint32 max, qint32 min, qint32 msg_par, qint32 *pVar = NULL);
    void setParam(float value,float max,float min,qint32 msg_par,float *pVar = NULL);
    void setLineParent(AbstractInterface *parent);
    void setInputType(inputType type);
    QRect m_WidgetRect;
public slots:
    void FirstDigitClicked();
private slots:
    void on_confirmBtn_clicked();

    void on_cancelBtn_clicked();
    void OnBtnClick(int id);




    void on_upBtn_clicked();
    void on_downBtn_clicked();

    void on_minusBtn_clicked();
    void on_dotBtn_clicked();

    void on_delBtn_clicked();

private:
    Ui::MyInputDialog *ui;
    QLineEdit *m_pRecvEdit;
    bool  m_ConnectFlag;

    qint32 m_value;
    qint32 max,min,msg_Par;
    qint32* m_pVar;

    int m_DispNum;
    float m_DispNumf;
    int    m_nValueDisp;
    AbstractInterface *pParent;
    int timerId;
    bool m_bool_first;
    QSignalMapper *signalMapper;

    QPushButton *m_numberBtn[10];
    inputType m_type;
    void checkDotBtn();


    float m_valueF,maxF,minF;
    float *m_pVarf;
};

#endif // MYINPUTDIALOG_H
