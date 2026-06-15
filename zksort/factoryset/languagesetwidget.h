#ifndef LANGUAGESETWIDGET_H
#define LANGUAGESETWIDGET_H

#include "basewidget.h"
#include <QButtonGroup>
#include <QSignalMapper>
namespace Ui {
class LanguageSetWidget;
}

class LanguageSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit LanguageSetWidget(QWidget *parent = 0);
    ~LanguageSetWidget();
    void showPage(bool bshow = true);
protected:
    virtual void retranslateUiWidget();
private slots:

    void langBtn_clicked(int);

private:
    Ui::LanguageSetWidget *ui;
    QButtonGroup *m_pLanBtnGroup;
    QSignalMapper *m_pLangBtnMapper;

    void updateLanBtnState();
};

#endif // LANGUAGESETWIDGET_H
