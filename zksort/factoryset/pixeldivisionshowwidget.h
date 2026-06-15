#ifndef PIXELDIVISIONSHOWWIDGET_H
#define PIXELDIVISIONSHOWWIDGET_H

#include "basewidget.h"
#include <QVector>
namespace Ui {
class PixelDivisionShowWidget;
}
typedef struct PixelDivision{
    QString cameraNamestr;
    QString startstr;
    QString endstr;
    QString midstr;
    QString starteject;
    QString endeject;
    QString startstr2;
    QString endstr2;
    QString starteject2;
    QString endeject2;
}PixelDivision;

class PixelDivisionShowWidget : public basewidget
{
    Q_OBJECT

public:
    explicit PixelDivisionShowWidget(QWidget *parent = 0);
    ~PixelDivisionShowWidget();
    void showPage(bool bshow);
protected:
    void retranslateUiWidget();
private slots:
    void on_resetAllBtn_clicked();

private:
    Ui::PixelDivisionShowWidget *ui;
    QVector<PixelDivision> m_pixVec;
    void updateTable();
    void updatePixVec();
};

#endif // PIXELDIVISIONSHOWWIDGET_H
