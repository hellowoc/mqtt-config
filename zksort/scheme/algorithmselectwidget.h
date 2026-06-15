#ifndef ALGORITHMSELECTWIDGET_H
#define ALGORITHMSELECTWIDGET_H

#include <QWidget>

namespace Ui {
class AlgorithmSelectWidget;
}

class AlgorithmSelectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AlgorithmSelectWidget(QWidget *parent = 0);
    ~AlgorithmSelectWidget();

private:
    Ui::AlgorithmSelectWidget *ui;
};

#endif // ALGORITHMSELECTWIDGET_H
