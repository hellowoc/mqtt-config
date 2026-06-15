#ifndef TARGETIMAGEWIDGET_H
#define TARGETIMAGEWIDGET_H

#include "basewidget.h"
#include <QVector>
#include <QRadioButton>
#include <QSignalMapper>
#include <QThread>
#include <QDateTime>
#include <QDebug>
#include <QString>
#include <QVector>
#include <QMutex>

namespace Ui {
class TargetImageWidget;
}

using namespace std;



class CircularQueue
{
public:
    explicit CircularQueue(int capacity = 100);
    ~CircularQueue();

    // 入队：当队列满时自动覆盖最旧的数据
    void enqueue(const QString& value);

    // 出队：若队列为空，返回默认构造的QString()
    QString dequeue();

    // 获取当前元素个数
    int size() const;

    // 判断队列是否为空
    bool isEmpty() const;

    // 判断队列是否已满
    bool isFull() const;

    // 清空队列
    void clear();

private:
    QVector<QString> m_buffer;   // 环形缓冲区
    int m_head;                   // 队首索引（指向最早插入的元素）
    int m_tail;                   // 队尾索引（指向下一个可插入位置）
    int m_count;                  // 当前元素个数
    int m_capacity;               // 缓冲区容量
    mutable QMutex m_mutex;        // 互斥锁，保护上述成员变量
};




class TargetImageWidget : public basewidget
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    explicit TargetImageWidget(QWidget *parent = 0);
    ~TargetImageWidget();
    void showPage(bool bshow = true);
    void retranslateUiWidget();
    void receiveMsg(int msg1, int msg2, int msg3);
    void initUi();
     int index = 0;
    void setTimer(bool);
public slots:

    void  showImageSlot();

private:

    Ui::TargetImageWidget *ui;
    QPushButton *m_imgListBtn[4];  /* */
    QTimer *timer;


};
extern CircularQueue myCircularQueue;
#endif //
