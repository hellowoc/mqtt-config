#ifndef AIWIPESETWIDGET_H
#define AIWIPESETWIDGET_H

#include "basewidget.h"

namespace Ui {
class AiWipeSetWidget;
}

class AiWipeSetWidget : public basewidget
{
    Q_OBJECT

public:
    explicit AiWipeSetWidget(QWidget *parent = 0);
    ~AiWipeSetWidget();
    virtual void showPage(bool bshow);
protected:
    virtual void retranslateUiWidget();
private slots:

private:
    Ui::AiWipeSetWidget *ui;
};

#endif // AIWIPESETWIDGET_H
