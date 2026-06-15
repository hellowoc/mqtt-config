#ifndef NIRDOTCORRECTIVELISTWIDGET_H
#define NIRDOTCORRECTIVELISTWIDGET_H

#include "basewidget.h"
typedef struct DotStruct{
    QString redOrgreen;
    QString dotValue;
}DotStruct;

namespace Ui {
class NirDotCorrectiveListWidget;
}

class NirDotCorrectiveListWidget : public basewidget
{
    Q_OBJECT

public:
    explicit NirDotCorrectiveListWidget(QWidget *parent = 0);
    ~NirDotCorrectiveListWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
    void OnViewChange(int view);
    void OnChuteChange();
    void OnLayerChange();
protected:
    void retranslateUiWidget();
private slots:
    void on_sendAll_clicked();

    void on_dotDataCapture_clicked();

    void on_pushButton_9_clicked();

private:
    Ui::NirDotCorrectiveListWidget *ui;
    QVector<DotStruct> m_dotVec;
    void updateTable();
    void updateDotVec();
    void updateValue();
};

#endif // NIRDOTCORRECTIVELISTWIDGET_H
