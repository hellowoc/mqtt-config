#ifndef MYTOOLBUTTON_H
#define MYTOOLBUTTON_H

#include <QWidget>
#include <QMouseEvent>
#include <QPixmap>
#include <QPushButton>
#include <QColor>
class myToolButton : public QPushButton
{
public:
    myToolButton(QWidget *parent = 0);
    void setPixFileStr(QString file1,QString file2,int btnIndex);
    void updateText(QString text);
    void updateRun(bool bRun);
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *);
signals:

public slots:

private:
    QPixmap pix;
    QString m_fileStrNM,m_fileStrPress;
    QColor m_color;
    int m_btnIndex;
    QString m_text;

};

#endif // MYTOOLBUTTON_H
