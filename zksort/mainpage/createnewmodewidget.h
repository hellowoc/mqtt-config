#ifndef CREATENEWMODEWIDGET_H
#define CREATENEWMODEWIDGET_H

#include "basewidget.h"
#include <QButtonGroup>
#include <QSignalMapper>
namespace Ui {
class CreateNewModeWidget;
}

class CreateNewModeWidget : public basewidget
{
    Q_OBJECT

public:
    explicit CreateNewModeWidget(QWidget *parent = 0);
    ~CreateNewModeWidget();
    void showPage(bool bshow = true);
    void receiveMsg(int msg1, int msg2, int msg3);
protected:
    void retranslateUiWidget();
private slots:
    void on_cancelBtn_clicked();

    void on_okBtn_clicked();

    void mapBtnClicked(int index);

private:
    Ui::CreateNewModeWidget *ui;
    QStringList m_riceModeList;
    QStringList m_foodModeList;
    QStringList m_modeNameLanList;
    int m_modeIndex;
    QButtonGroup *m_pBtnGroup;
    QSignalMapper *m_pSignalMapper;
};

#endif // CREATENEWMODEWIDGET_H
