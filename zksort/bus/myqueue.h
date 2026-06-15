#ifndef MYQUEUE_H
#define MYQUEUE_H

#include "constant.h"
#include "global.h"

#define QUEUE_SIZE	10000
//#define DATA_SIZE	25

typedef struct _SqQueue 
{
    char data[QUEUE_SIZE][SEND_PACKET_SIZE];
    unsigned int front;
    unsigned int rear;
}SqQueue;

class MySerialQueue
{
public:
    MySerialQueue();
    ~MySerialQueue();
    bool isEmpty();//队列是否为空
    bool push(char* data);//元素进队
    bool pop(char* data);//元素出队

private:
    SqQueue * m_queue;
    QMutex m_queueLock; //保护串口命令队列操作
    bool _isEmpty();//队列是否为空
    bool _push(char* data);//元素进队
    bool _pop(char* data);//元素出队


};


typedef struct _SqQueue2
{
    char data[QUEUE_SIZE][SEND_PACKET_RECTRL_BOARD_SIZE];
    unsigned int front;
    unsigned int rear;
}SqQueue2;


class MySerialQueue2
{
public:
    MySerialQueue2();
    ~MySerialQueue2();
    bool isEmpty();//队列是否为空
    bool push(char* data);//元素进队
    bool pop(char* data);//元素出队

private:
    SqQueue2 * m_queue2;
    QMutex m_queue2Lock; //保护串口命令队列操作
    bool _isEmpty();//队列是否为空
    bool _push(char* data);//元素进队
    bool _pop(char* data);//元素出队


};

extern MySerialQueue myQueue;

extern MySerialQueue2 myQueue2;

#endif // MYQUEUE_H
