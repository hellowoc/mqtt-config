#ifndef MYSWITCHWIDGET_H
#define MYSWITCHWIDGET_H

#include <QWidget>

namespace Ui {
class MySwitchWidget;
}

class MySwitchWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString text READ getText WRITE setText)
public:
    explicit MySwitchWidget(QWidget *parent = 0);
    ~MySwitchWidget();

    void setToggle(bool checked);
    QString getText();
    void setText(const QString titletext);
private:
    Ui::MySwitchWidget *ui;
    QString text;
signals:
    void switchToggledSignal(bool checked);//与外部关联的信号
private slots:
    void switchToggledSlots(bool checked);
};

#endif // MYSWITCHWIDGET_H
