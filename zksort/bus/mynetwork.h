#ifndef MYNETWORK_H
#define MYNETWORK_H

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
#include <time.h>
#else
#include <winsock.h>
#include <ws2tcpip.h>
#endif

#include "globalparams.h"
#include "myqextserialport.h"
#include "logger.h"

#include <QWaitCondition>
#include <QMutex>

#define CAPTURE_SINGLE_WAVE         0       //单个波形
#define CAPTURE_IMAGE               1       //正常图像400行
#define CAPTURE_IMAGE_SYNC          2       //同步采图 //不再使用
#define CAPTURE_BLOW_COUNTS         3       //吹气频率
#define CAPTURE_BACKGROUND_WAVE     4       //背景波形采集
#define CAPTURE_IAMGE_CONTINUOUS    5       //连续采图
#define CAPTURE_SUSPEND             6       //线程挂起
#define CAPTURE_MULTI_IMAGE         7       //多张采集
#define CAPTURE_MULTI_IMAGE_GET     8       //多张采集上传
#define CAPTURE_IMAGE_TEMP          9       //采集临时图片
#define CAPTURE_IPC_EJECT           10      //ipc坏点数
#define CAPTURE_MULTI_IMAGE_SYNC        11      //同步多张采集
#define CAPTURE_MULTI_IMAGE_GET_SYNC    12      //同步多张上传
#define CAPTURE_IAMGE_CONTINUOUS_SYNC   13      //连续同步采图


#define FS_IMAGE_CONBINE 0   //分时图像合并，左右1024合成2048

#ifdef Q_OS_UNIX
#pragma pack(2)
//下面两个结构是位图的结构
typedef struct BITMAPFILEHEADER
{
    u_int16_t bfType;
    u_int32_t bfSize;
    u_int16_t bfReserved1;
    u_int16_t bfReserved2;
    u_int32_t bfOffBits;
}BITMAPFILEHEADER;

typedef struct BITMAPINFOHEADER
{
    u_int32_t biSize;
    u_int32_t biWidth;
    u_int32_t biHeight;
    u_int16_t biPlanes;
    u_int16_t biBitCount;
    u_int32_t biCompression;
    u_int32_t biSizeImage;
    u_int32_t biXPelsPerMeter;
    u_int32_t biYPelsPerMeter;
    u_int32_t biClrUsed;
    u_int32_t biClrImportant;
}BITMAPINFOHEADER;
/*
调色板结构
*/
#pragma pack(1)
typedef struct RGBQUAD
{
    unsigned char rgbBlue; //蓝色分量亮度
    unsigned char rgbGreen;//绿色分量亮度
    unsigned char rgbRed;//红色分量亮度
    unsigned char rgbReserved;
}rgbq;
#pragma pack()
/*
要注意一下几点：
（1）每一行的字节数必须是4的整倍数，如果不是，则需要补齐。如241补齐为244.
（2）一般来说，.bMP文件的数据从下到上，从左到右的。也就是说，从文件中最先读到的是图象最下面一行的左边第一个象素，
然后是左边第二个象素……接下来是倒数第二行左边第一个象素，左边第二个象素……依次类推 ，最后得到的是最上面一行的最右一个象素。
（3）图像是24位或是32位数据的位图的话，位图数据区就不是索引而是实际的像素值了。
（4）24位RGB按照BGR的顺序来存储每个像素的各颜色通道的值，一个像素的所有颜色分量值都存完后才存下一个像素，不进行交织存储。
（5）32位数据按照BGRA的顺序存储，其余与24位位图的方式一样。
*/

#pragma pack()
#endif


/*****
 *
 *     读取波形及图像线程
 *
 ********************************/
class MyNetWorkThread : public QThread
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    explicit MyNetWorkThread(QThread *parent = 0);
    void setSocket(int fd);
    void setRecvType(int type,int layer=0,int view=0,int unit=0);
    void updateCameraInfo(int view = -1);
    void waveDataPacketed(bool needFocus);
    void imageDataPacketed(int correctHeight);
    int  saveBMPFile(QString path,int w,int h);
    int  imageCheckSum(int packCnt);
    int  myGetTickCount();
    int  checkUdpSocketStatues(int seconds);
    void sendTransIncreaseBlowCounts();
    void serialSendTransStartStop(int flag = 0,int view = 0);
    void cleanUdpSocket(int seconds = 0);


    void setAddr_remote(struct sockaddr_in);
    void udpSendCmdData(int nCmd, char intAddr, char boardType, char boardAddr, char *data, int nCount);
    void udpSendTransStartStop(int flag = 0,int view = 0);
    int  udpSendTo(char *buf, int len);

    bool isrunning;
    uchar blowCountsPacketBuf[6+ 4096+3+1];
    int blowCountsTempDataBuf[12*80];

    int currenttype;
    int imgCorrectHight;
    int materCnt,totalCnt,materPropor;
    int nBlowQueryFreq;
protected:
    void run();

signals:
    void SignalWaveRecvFinished();
    void SignalImageRecvFinished(int correctWaveLines,QString path);
    void showContinuousImage(QString path);
    void SignalSocketFailed();
    void SignalBlowCountsRecvFinished();

private:
#ifdef Q_OS_UNIX
    int sockfd;
#else
    SOCKET sockfd;
#endif
    int curTotal;       // 当前累计接收的波形或图像字节数
    int retNum;         // 每次recv或recvfrom的返回的实际读取字节数
    int mlayer;
    int mview;
    int munit;
    int lockedLayer;//230218 防止连续采图及远程采图中切换视角通道
    int lockedview;
    int lockedunit;
    int m_nNumOfImg;
    struct sockaddr_in client;
    struct sockaddr_in r_addr;

    int dim;                // 传感器维度
    int curPixels;          // 传感器分辨率
    int rowPacketTotal;     // 传感器单色每行 包数
    int imageRawBufSize;    // 接收的图像缓冲区大小，单位：字节
    int waveBufSize;        // 接收的一行波形缓冲区大小，单位：字节
    int td_dim;             // 分時类型-相对1024来说，1-不分时，2-1024TD（1024+1024）， 4 2048GTD(2048+2048) 5-G3 2048TD(2048+2048+1024)
    int curImageRowCnt = 0;               // 每行波形的包计数，用于确认是否丢包

    uchar dirtydatabuf[SENSOR_MAX_PIXELS]; //2048
    uchar wavePacketBuf[TD_DIM*SENSOR_MAX_PIXELS/2*SENSOR_MAX_DIM+6];//5*1024*4+6
    uchar waveDataBuf[TD_DIM*SENSOR_MAX_PIXELS/2*SENSOR_MAX_DIM];//5*1024*4
    uchar imagePacketBuf[sizeof(wavePacketBuf)*IMAGE_HEIGHT];//（5*1024*4+6）*416
    uchar imageDataBuf[sizeof(waveDataBuf)*IMAGE_HEIGHT];//5*1024*4*416
    uchar tmpBuf[sizeof(wavePacketBuf)*IMAGE_HEIGHT];//（5*1024*4+6）*416
};

/*****
 *
 *     网络采图线程
 *
 ********************************/
class MyNetCptureThread : public QThread
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    explicit MyNetCptureThread(QThread *parent = 0);
    void myUdpSendTransStartStop(int flag = 0,int view = 0);
    void myUdpSendCmdData(int nCmd, char intAddr, char boardType,  char boardAddr, char *data, int nCount);

protected:
    void run();

};


/*****
 *
 *     网络接口类
 *
 *****************************/
class NetWork : public QWidget
{
    Q_OBJECT
public:
    explicit NetWork(QWidget *parent = 0);

    void udpBind(QString ip="162.254.129.100",QString port="8081");         //绑定本地IP、端口
    void setUdpRecvType(int type,int layer=0,int view=0,int unit=0);                                          //设置接收类型
    void setUdpRecvTimeOut(int t_s,int t_ms);                               //设置接收超时时间
    void udpStart();
    void udpStop();
//    void serialSendTransStartStop(int flag);                              // 1:开始上传， 0：停止上传
    void udpRestart();

    int udpSendTo(char *buf, int len);                                     //发送网络数据包
    void setBlowQueryFreq(int time);
    MyNetWorkThread *udpthread;
    int capType;

public slots:
    void doProcessDealSocketFailed();

signals:

private:
    QString IP;
    QString Port;

#ifdef Q_OS_UNIX
    int sockfd;
#else
    SOCKET sockfd;
#endif
    struct sockaddr_in addr_local;
    struct sockaddr_in addr_remote;
    struct hostent *host;
};

extern MyNetCptureThread *myNetCpture;

#endif // MYNETWORK_H
