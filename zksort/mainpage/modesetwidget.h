#ifndef MODESETWIDGET_H
#define MODESETWIDGET_H

#include "BaseWidget.h"
#include <QButtonGroup>
namespace Ui {
class ModeSetWidget;
}

class ModeSetWidget : public BaseWidget
{
    Q_OBJECT

public:
    explicit ModeSetWidget(QWidget *parent = 0);
    ~ModeSetWidget();
    void showPage(bool bshow = true);
protected:
    void retranslateUiWidget();
private slots:
    void btn_clicked(bool);

private:
    Ui::ModeSetWidget *ui;
    QButtonGroup *m_pbtnGroup;
};

#endif // MODESETWIDGET_H
