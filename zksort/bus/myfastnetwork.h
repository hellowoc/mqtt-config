#ifndef MYFASTNETWORK_H
#define MYFASTNETWORK_H

#include <QWidget>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

#ifdef Q_OS_UNIX
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#else
#include <winsock.h>
#include <ws2tcpip.h>
#endif

#include "globalparams.h"
#include "globalflow.h"
#include "myqextserialport.h"
#include "logger.h"

#include <QWaitCondition>
#include <QMutex>
#include <iostream>
#include <thread>

#define MAX_DIM 8
#define PACKET_HEAD_LEN 14
#define PACKET_DATA_START 12
#define MAX_SINGLE_LINE_LENGTH (SENSOR_MAX_PIXELS+PACKET_HEAD_LEN+PACKET_DATA_START)

#define FAST_CAPTURE_IMAGE                  1 //单张采图模式
#define FAST_FR_CAPTURE_IMAGE               2 //前后采图模式
#define FAST_TARGET_CAPTURE_IMAGE           3 //目标采图模式

struct img_stream_head{
    int imgView;
    int imgUnit;
    int imgSN;
    int imgHeight;
    int imgWidth;
    int imgDim;
    int imgClass;
};

#define QUEUE_CAPACITY 10000

// 定义数据包结构体
typedef struct {
    unsigned char *data;
    size_t length;
} Packet;

// 定义环形队列结构体
typedef struct {
    Packet *queue;
    int capacity;
    int head;
    int tail;
    int size;
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
} RingQueue;

class MyRingQueue
{
public:
    MyRingQueue(){
        q = (RingQueue *)malloc(sizeof(RingQueue));
        init_queue(QUEUE_CAPACITY);
    }
    ~MyRingQueue(){
        free_queue();
        free(q);
    }

    // 初始化环形队列
    void init_queue(int capacity) {
        q->queue = (Packet *)malloc(capacity * sizeof(Packet));
        memset(q->queue,0,capacity * sizeof(Packet) );
        q->capacity = capacity;
        q->head = 0;
        q->tail = 0;
        q->size = 0;
        q->count = 0;
        pthread_mutex_init(&q->mutex, NULL);
        pthread_cond_init(&q->not_full, NULL);
        pthread_cond_init(&q->not_empty, NULL);
    }
    // 入队操作
    void enqueue(const unsigned char *data, size_t length) {
        pthread_mutex_lock(&q->mutex);
        while (q->size >= q->capacity) {
            pthread_cond_wait(&q->not_full, &q->mutex);
        }
        q->queue[q->tail].data = (unsigned char *)malloc(length);
        memset(q->queue[q->tail].data,0,length);
        memcpy(q->queue[q->tail].data, data, length);
        q->queue[q->tail].length = length;
        q->tail = (q->tail + 1) % q->capacity;
        q->size++;
        pthread_cond_signal(&q->not_empty);
        pthread_mutex_unlock(&q->mutex);
    }
    // 出队操作
    bool dequeue(Packet &packet) {
        pthread_mutex_lock(&q->mutex);
        while (q->size == 0) {
            pthread_cond_wait(&q->not_empty, &q->mutex);
        }
        packet = q->queue[q->head];

        q->head = (q->head + 1) % q->capacity;
        q->size--;
        q->count ++;
        pthread_cond_signal(&q->not_full);
        pthread_mutex_unlock(&q->mutex);
        return true;
    }
    // 释放队列资源
    void free_queue() {
        int i;
        for (i = 0; i < q->size; i++) {
            free(q->queue[(q->head + i) % q->capacity].data);
        }
        free(q->queue);
        pthread_mutex_destroy(&q->mutex);
        pthread_cond_destroy(&q->not_full);
        pthread_cond_destroy(&q->not_empty);
    }
    int get_size(){
        return q->size;
    }
    int get_count(){
        return q->count;
    }

private:
    RingQueue *q;
};




class QThread;
class MyFastNetWorkThread : public QThread
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
public:
    enum Status{
        Stop,
        Start,
        FR_Start
    };
    enum Type{
        D_Mode,
        S_Mode,
        FR_Mode,
        TARGET_Mode
    };
    explicit MyFastNetWorkThread(QThread *parent = 0);
    void udpSocketInit(QString ip="192.168.1.100",QString port="20001");         //绑定本地IP、端口
    void udpServerInit(int ipIndex = 1); //命令端口走udpthread,不再使用
    void initData();
    void setUdpRecvType(int type,int layer=0,int view=0,int unit=0);
    void udpStart();
    void udpStop();                         // 1:开始上传， 0：停止上传
    void udpRestart();

    void updateCameraInfo(int view,int unit);
    void imageDataPacketed(int correctHeight);
    int  saveBMPFile(QString path,int w,int h);
//    void setRealImgSize(int dim, int pixels, int height, int trig);
    int  myGetTickCount();
    int  checkUdpSocketStatues(int seconds);
    void cleanUdpSocket(int seconds = 0);


    void setAddr_remote(struct sockaddr_in);
    void udpSendCmdData(int nCmd, char intAddr, char boardType, char boardAddr, char *data, int nCount);
    void udpSendTransStartStop(int status = 0,int type = 0,int view = 0,int unit = 0);
    int  udpSendTo(int view,int unit,char *buf, int len);

    bool isrunning;
    int currenttype;
    int view;
    int unit;
    int m_saveImgCount;

protected:
    void run();

signals:
    void SignalImageRecvFinished(QStringList viewList, QStringList saveImgCntList);
    void SignalImageRecvFull(int view,int unit);
    void SignalFRImageRecvOver();
    void SignalSocketFailed();

private:
#ifdef Q_OS_UNIX
    int sockfd;
#else
    SOCKET sockfd;
#endif

    struct sockaddr_in addr_local;
    struct sockaddr_in addr_remote[MAX_VIEW][MAX_VIEW_UNIT];
    struct hostent *host;

    unsigned int m_fpgaSendPacketSeq;
    bool imgBufHasNew;
    int m_packetPerLine;
    int m_imagePixels;
    int m_imageHeight;
    int m_imageDim;
    int m_imageTrig;

    int m_recvRowCount;
    int m_errCount;
    int m_recvImgCount;
    int imgPacketLen;

    uchar dirtydatabuf[MAX_SINGLE_LINE_LENGTH];

    uchar imageCombineBuf[MAX_SINGLE_LINE_LENGTH*SENSOR_MAX_DIM]; //(2048+14+12)×4
    uchar imagePacketBuf[MAX_SINGLE_LINE_LENGTH];//(2048+14+12)
    uchar imageDataBuf[SENSOR_MAX_PIXELS*SENSOR_MAX_DIM*IMAGE_HEIGHT*2]; //(2048+14+12)*4*832
    uchar targetImageDataBuf[8*12][3*320*180];//最多支持8*12个相机320*180的框

#define MAX_LOOP_PNG_SAVE_THREAD 2
    std::thread m_loop_thread_save[MAX_LOOP_PNG_SAVE_THREAD];

};

extern MyFastNetWorkThread *myFastNetThread;
extern MyRingQueue myRingQueue;

#endif // MYFASTNETWORK_H
