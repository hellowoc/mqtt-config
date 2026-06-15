#ifndef MYARITHUI_H
#define MYARITHUI_H

#include <QWidget>

namespace Ui {
class MyArithUi;
}

class MyArithUi : public QWidget
{
    Q_OBJECT

public:
    explicit MyArithUi(int arithType,QWidget *parent = 0);
    ~MyArithUi();

    void showWidget();
    void updateWidget();
private slots:
    void on_checkBox_clicked(bool checked);

signals:
    void arithBtnCheckedSignal(bool checked,int type);
private:
    Ui::MyArithUi *ui;
    int m_arithType;

    void copySpliceDisplayEnable(int enable, int arithIndex);
};

#endif // MYARITHUI_H
