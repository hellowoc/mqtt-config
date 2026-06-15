#ifndef MYLISTWIDGET_H
#define MYLISTWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>
#include <QApplication>

class mylistwidget : public QListWidget
{
    Q_OBJECT
public:
    explicit mylistwidget(QWidget *parent = 0);
    void updateList();
    
protected:
//    void mouseDoubleClickEvent(QMouseEvent *e);
//    void mousePressEvent(QMouseEvent *event);
//    void mouseMoveEvent(QMouseEvent *e);
//    void dragEnterEvent(QDragEnterEvent *event);
//    void dragMoveEvent(QDragMoveEvent *e);
//    void dropEvent(QDropEvent *event);

public:
    QListWidgetItem *currentitem;
    bool iscut;
    QString path;
private:
    QPoint start;
    bool isUpdating;

    
};

#endif // MYLISTWIDGET_H
